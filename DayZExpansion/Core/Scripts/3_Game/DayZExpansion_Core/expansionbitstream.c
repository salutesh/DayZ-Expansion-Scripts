/**
 * @brief Bit streams with packing/unpacking
 * 
 * @code
 *  auto ctx = new ScriptReadWriteContext();
 *  
 *  // Writing
 *  auto writer = new ExpansionBitStreamSerializerWriter(ctx.GetWriteContext());
 *  
 *  bool   b = true;
 *  int    i = 123;
 *  int    j = -64;
 *  string s = "test";
 *  float  f = 0.123456789;
 *  vector v = Vector(7500.75, 250.25, 5000.5);
 *  
 *  writer.WriteBool(b);
 *  writer.WriteUInt(i, 7);
 *  writer.Write(j, 7);
 *  writer.WriteHalf(f);  // fp16 (4 digits of precision for [0..1], range +/- 65504)
 *  writer.Write(f, 16);  // bFloat16 (3 digits of precision for [0..1], range +/- 3.39e+38)
 *  writer.Write(s, 3);
 *  writer.Write(v, 24);
 *  writer.Flush();
 * 
 *  // Reading
 *  auto reader = new ExpansionBitStreamSerializerReader(ctx.GetReadContext());
 *  
 *  bool   b;
 *  int    i;
 *  int    j;
 *  string s;
 *  float  f;
 *  vector v;
 *  
 *  if (reader.ReadBool(b)) Print(b);
 *  >> bool b = true
 * 
 *  if (reader.ReadUInt(i, 7)) Print(i);
 *  >> int i = 123
 * 
 *  if (reader.Read(j, 7)) Print(j);
 *  >> int j = -64
 * 
 *  if (reader.ReadHalf(f)) Print(f);
 *  >> float f = 0.123474
 * 
 *  if (reader.Read(f, 16)) Print(f);
 *  >> float f = 0.123047
 * 
 *  if (reader.Read(s, 3)) Print(s);
 *  >> string test = 'Test'
 * 
 *  if (reader.Read(v, 24)) Print(v);
 *  >> vector v = {7500.75, 250.25, 5000.5}
 * @endcode
 */

class ExpansionBitStream
{
	static const string CLASSNAME_ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
	static const string CLASSNAME_ALPHABET_LOWERCASE = "0123456789_abcdefghijklmnopqrstuvwxyz";

	int m_Packed;
	int m_PackedBits;

	/**
	 * @brief Bit size of integer value
	 * 
	 * @return Number of bits
	 */
	static int BitSize(int value)
	{
		return Math.Log2(value) + 1;
	}

	/**
	 * @brief float to fp16
	 * 
	 * @note this version implements round-to-nearest for simplicity sake
	 */
	static int FloatToFP16(float value)
	{
		int v = CF_Cast<float, int>.Reinterpret(value);

		int sign = (v >> 16) & 0x8000;
		int exponent = (v >> 23) & 0xff;
		int mantissa = v & 0x007fffff;

		if (exponent == 0xff)
		{
			if (mantissa != 0)
			{
				//! NaN

				mantissa = (mantissa >> 13) & 0x03ff;

				if (mantissa == 0)
					mantissa = 0x0200;  //! Quiet-NaN

				return sign | 0x7c00 | mantissa;
			}

			//! Infinity
			return sign | 0x7c00;
		}

		//! Account for different exponent bias between float (127) and fp16 (15)
		exponent -= 112;  //! 127 - 15

		if (exponent <= 0)
		{
			if (exponent < -10)
				return sign;  //! Signed zero

			//! Subnormal

			mantissa |= 0x00800000;  //! 1.xxxxx in float mantissa (24 bits)

			int shift = 1 - exponent;  //! Shift needed to make it subnormal

			//! Round to nearest
			int round = 1 << (shift - 1);
			mantissa = (mantissa + round) >> shift;

			mantissa = (mantissa >> 13) & 0x03ff;

			return sign | mantissa;
		}

		if (exponent >= 31)
		{
			//! Overflow to infinity
			return sign | 0x7c00;
		}

		//! Normal

		//! Add rounding bit (1 << 12) before shifting to get round-to-nearest
		mantissa = mantissa + 0x00001000;

		//! If rounding caused mantissa overflow (i.e., 0x00800000 bit set),
		//! increment exponent accordingly
		//! (@note mantissa overflow bit is cleared implicitly by bitshifting and bitmasking later)
		if ((mantissa & 0x00800000) != 0)
			exponent += 1;

		if (exponent >= 31)
		{
			//! Overflow to infinity
			return sign | 0x7c00;
		}

		exponent = (exponent & 0x1f) << 10;
		mantissa = (mantissa >> 13) & 0x03ff;

		return sign | exponent | mantissa;
	}

	/**
	 * @brief fp16 to float
	 */
	static float FP16ToFloat(int value)
	{
		int sign = (value >> 15) & 0x00000001;
		int exponent = (value >> 10) & 0x1f;
		int mantissa = value & 0x03ff;

		if (exponent == 0)
		{
			if (mantissa == 0)
			{
				//! Signed zero
				return CF_Cast<int, float>.Reinterpret(sign << 31);
			}

			//! Subnormal

			float f = mantissa * 0.000000059604644775390625;  //! 2^-24

			if (sign)
				return -f;

			return f;
		}

		if (exponent == 31)
		{
			if (mantissa == 0)
			{
				//! Infinity
				return CF_Cast<int, float>.Reinterpret((sign << 31) | 0x7f800000);
			}
			else
			{
				//! NaN
				return CF_Cast<int, float>.Reinterpret((sign << 31) | 0x7f800000 | (mantissa << 13));
			}
		}

		//! Normal

		//! Account for different exponent bias between float (127) and fp16 (15)
		exponent -= -112;  //! 15 - 127

		return CF_Cast<int, float>.Reinterpret((sign << 31) | (exponent << 23) | (mantissa << 13));
	}

	/**
	 * @brief pack value into <n> bits
	 * 
	 * @param value
	 * @param bits
	 * @param [inout] shift
	 * 
	 * @return Packed value
	 */
	static int Pack(int value, int bits, inout int shift)
	{
		if (bits < 1 || bits > BIT_INT_SIZE || shift + bits > BIT_INT_SIZE)
		{
			CF.FormatError("Invalid value %1 for parameter `bits`: Out of range [1..%2]", bits.ToString(), BIT_INT_SIZE.ToString());
			return 0;
		}

		int packed = (value & ((1 << bits) - 1)) << shift;
		shift += bits;

		return packed;
	}

	/**
	 * @brief Encode string to TIntArray
	 * 
	 * @note bits is used to encode string length, not characters!
	 */
	static TIntArray Pack(string value, int bits, string alphabet)
	{
		int packed;
		int packedBits;
		ExpansionArraySerializer serializer = new ExpansionArraySerializer;

		ExpansionBitStreamArrayWriter.Write(serializer, value, bits, alphabet, packed, packedBits);
		ExpansionBitStreamArrayWriter.Flush(serializer, packed, packedBits);

		return serializer.m_Data;
	}

	/**
	 * @brief Unpack value from <n> bits of packed value
	 * 
	 * @param packed
	 * @param bits
	 * @param [inout] shift
	 * 
	 * @return Unpacked value
	 */
	static int Unpack(int packed, int bits, inout int shift)
	{
		if (bits < 1 || bits > BIT_INT_SIZE || shift + bits > BIT_INT_SIZE)
		{
			CF.FormatError("Invalid value %1 for parameter `bits`: Out of range [1..%2]", bits.ToString(), BIT_INT_SIZE.ToString());
			return 0;
		}

		int value = (packed >> shift) & ((1 << bits) - 1);
		shift += bits;

		return value;
	}

	/**
	 * @brief Decode TIntArray to string
	 *  
	 * @note bits is used to encode string length, not characters!
	 */
	static string Unpack(TIntArray packedValues, int bits, string alphabet)
	{
		int packed;
		int packedBits;
		ExpansionArraySerializer serializer = new ExpansionArraySerializer(packedValues);
		string value;

		ExpansionBitStreamArrayReader.Read(serializer, value, bits, alphabet, packed, packedBits);

		return value;
	}

	static void Test()
	{
		auto ctx = new ScriptReadWriteContext();

		auto writer = new ExpansionBitStreamSerializerWriter(ctx.GetWriteContext());

		bool   b = true;
		int    i = 123;
		int    j = -64;
		int    k = 255;
		int    l = -128;
		int    m = 65535;
		int    n = -32768;
		float  f = 0.123456789;
		string s = "test";
		vector v = Vector(7500.75, 250.25, 5000.5);

		writer.WriteBool(b);
		writer.WriteUInt(i, 7);
		writer.Write(j, 7);
		writer.WriteUChar(k);
		writer.WriteChar(l);
		writer.WriteUShort(m);
		writer.WriteShort(n);
		writer.WriteHalf(f);
		writer.Write(f, 16);
		writer.Write(s, 3);
		writer.Write(v, 24);
		writer.Flush();

		auto reader = new ExpansionBitStreamSerializerReader(ctx.GetReadContext());

		b = false;
		i = 0;
		j = 0;
		k = 0;
		l = 0;
		m = 0;
		n = 0;
		f = 0.0;
		s = "";
		v = vector.Zero;

		if (reader.ReadBool(b)) Print(b);
		if (reader.ReadUInt(i, 7)) Print(i);
		if (reader.Read(j, 7)) Print(j);
		if (reader.ReadUChar(k)) Print(k);
		if (reader.ReadChar(l)) Print(l);
		if (reader.ReadUShort(m)) Print(m);
		if (reader.ReadShort(n)) Print(n);
		if (reader.ReadHalf(f)) Print(f);
		if (reader.Read(f, 16)) Print(f);
		if (reader.Read(s, 3)) Print(s);
		if (reader.Read(v, 24)) Print(v);
	}
}

class ExpansionBitStreamWriter: ExpansionBitStream
{
	void WriteUInt(int value, int bits);

	void Write(int value, int bits);

	void Write(int value, int min, int max);

	void WriteBool(bool value);

	void WriteChar(int value);

	void WriteUChar(int value);

	void WriteShort(int value);

	void WriteUShort(int value);

	void Write(float value, int bits);

	void WriteHalf(float value);

	void Write(vector value, int bits);

	void WriteHalf(vector value);

	void Write(string value, int bits);

	void Write(string value, int bits, string alphabet);

	void WriteClassName(string value);

	void WriteClassNameLower(string value);

	void Flush();

	/* Pass through write -------------------------------------------------- */

	void Write(bool value);

	void Write(int value);

	void Write(float value);

	void Write(vector value);

	void Write(string value);

	void Write(TBoolArray values);

	void Write(TIntArray values);

	void Write(TFloatArray values);

	void Write(TStringArray values);

	void Write(TVectorArray values);
}

class ExpansionBitStreamWriterT<Class T>: ExpansionBitStreamWriter
{
	T m_Serializer;

	void ExpansionBitStreamWriterT(T serializer)
	{
		m_Serializer = serializer;
	}

	static void WriteUInt(T serializer, int value, int bits, inout int packed, inout int packedBits)
	{
		if (bits < 1 || bits > BIT_INT_SIZE)
		{
			CF.FormatError("Invalid value %1 for parameter `bits`: Out of range [1..%2]", bits.ToString(), BIT_INT_SIZE.ToString());
			return;
		}

		if (bits < BIT_INT_SIZE)
			value &= ((1 << bits) - 1);

		int remainingBits = bits;

		while (remainingBits > 0)
		{
			int free = BIT_INT_SIZE - packedBits;

			if (free <= 0)
			{
				CF.FormatError("Something went terribly wrong. free = %1", free.ToString());
				Flush(serializer, packed, packedBits);
				return;
			}

			int chunkBits;

			if (remainingBits < free)
				chunkBits = remainingBits;
			else
				chunkBits = free;

			int chunk = value;

			if (chunkBits < BIT_INT_SIZE)
				chunk &= ((1 << chunkBits) - 1);

			packed |= (chunk << packedBits);
			packedBits += chunkBits;

			value >>= chunkBits;
			remainingBits -= chunkBits;

			if (packedBits == BIT_INT_SIZE)
				Flush(serializer, packed, packedBits);
		}
	}

	/**
	 * @brief Write signed integer packed into <n> bits
	 */
	static void Write(T serializer, int value, int bits, inout int packed, inout int packedBits)
	{
		if (bits < BIT_INT_SIZE)
			value += Math.Pow(2, bits - 1);

		WriteUInt(serializer, value, bits, packed, packedBits);
	}

	/**
	 * @brief Optimized writing of integer in range [min..max]
	 */
	static void Write(T serializer, int value, int min, int max, inout int packed, inout int packedBits)
	{
		WriteUInt(serializer, value - min, BitSize(max - min), packed, packedBits);
	}

	static void WriteBool(T serializer, bool value, inout int packed, inout int packedBits)
	{
		int v = value;
		WriteUInt(serializer, v, 1, packed, packedBits);
	}

	static void WriteChar(T serializer, int value, inout int packed, inout int packedBits)
	{
		WriteUInt(serializer, value + 128, 8, packed, packedBits);
	}

	static void WriteUChar(T serializer, int value, inout int packed, inout int packedBits)
	{
		WriteUInt(serializer, value, 8, packed, packedBits);
	}

	static void WriteShort(T serializer, int value, inout int packed, inout int packedBits)
	{
		WriteUInt(serializer, value + 32768, 16, packed, packedBits);
	}

	static void WriteUShort(T serializer, int value, inout int packed, inout int packedBits)
	{
		WriteUInt(serializer, value, 16, packed, packedBits);
	}

	static void Write(T serializer, float value, int bits, inout int packed, inout int packedBits)
	{
		WriteUInt(serializer, CF_Cast<float, int>.Reinterpret(value) >> (BIT_INT_SIZE - bits), bits, packed, packedBits);
	}

	//! @note converts float to fp16
	static void WriteHalf(T serializer, float value, inout int packed, inout int packedBits)
	{
		int fp16 = FloatToFP16(value);
		WriteUInt(serializer, fp16, 16, packed, packedBits);
	}

	static void Write(T serializer, vector value, int bits, inout int packed, inout int packedBits)
	{
		for (int i = 0; i < 3; ++i)
		{
			Write(serializer, value[i], bits, packed, packedBits);
		}
	}

	//! @note converts float to fp16
	static void WriteHalf(T serializer, vector value, inout int packed, inout int packedBits)
	{
		for (int i = 0; i < 3; ++i)
		{
			WriteHalf(serializer, value[i], packed, packedBits);
		}
	}

	//! @note bits is used to encode string length, not characters!
	static void Write(T serializer, string value, int bits, inout int packed, inout int packedBits)
	{
		int length = value.Length();

		if (bits < BIT_INT_SIZE)
		{
			int maxLength = Math.Pow(2, bits) - 1;

			if (length > maxLength)
			{
				CF.FormatError("string length %1 exceeds maximum %2 according to given bits %3", length.ToString(), maxLength.ToString(), bits.ToString());
				length = maxLength;
			}
		}

		WriteUInt(serializer, length, bits, packed, packedBits);

		for (int i = 0; i < length; ++i)
		{
			WriteUInt(serializer, value[i].ToAscii(), 8, packed, packedBits);
		}
	}

	/**
	 * @brief Optimized writing of string consisting of characters in <alpahabet>
	 *
	 * @note bits is used to encode string length, not characters!
	 */
	static void Write(T serializer, string value, int bits, string alphabet, inout int packed, inout int packedBits)
	{
		int length = value.Length();

		if (bits < BIT_INT_SIZE)
		{
			int maxLength = Math.Pow(2, bits) - 1;

			if (length > maxLength)
			{
				CF.FormatError("string length %1 exceeds maximum %2 according to given bits %3", length.ToString(), maxLength.ToString(), bits.ToString());
				length = maxLength;
			}
		}

		WriteUInt(serializer, length, bits, packed, packedBits);

		int alphabetBits = BitSize(alphabet.Length());

		for (int i = 0; i < length; ++i)
		{
			int v = alphabet.IndexOf(value[i]);

			if (v == -1)
			{
				CF.FormatError("Invalid character '%1' at index %2", value[i], i.ToString());
				v = 0;
			}

			WriteUInt(serializer, v, alphabetBits, packed, packedBits);
		}
	}

	/**
	 * @brief Optimzed writing of classname (5 characters fit into 4 bytes)
	 * 
	 * @note Length is limited to 127 characters
	 */
	static void WriteClassName(T serializer, string value, inout int packed, inout int packedBits)
	{
		Write(serializer, value, 7, CLASSNAME_ALPHABET, packed, packedBits);
	}

	/**
	 * @brief Optimzed writing of lowercase classname (5 characters fit into 4 bytes)
	 * 
	 * @note Length is limited to 127 characters
	 */
	static void WriteClassNameLower(T serializer, string value, inout int packed, inout int packedBits)
	{
		Write(serializer, value, 7, CLASSNAME_ALPHABET_LOWERCASE, packed, packedBits);
	}

	/**
	 * @brief Flush remaining packed bits (if any) to serializer
	 */
	static void Flush(T serializer, inout int packed, inout int packedBits)
	{
		if (packedBits > 0)
		{
			serializer.Write(packed);

			packed = 0;
			packedBits = 0;
		}
	}

	override void Write(int value, int bits)
	{
		Write(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}
	
	override void WriteUInt(int value, int bits)
	{
		WriteUInt(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	/**
	 * @brief Optimized writing of integer in range [min..max]
	 */
	override void Write(int value, int min, int max)
	{
		Write(m_Serializer, value, min, max, m_Packed, m_PackedBits);
	}

	override void WriteBool(bool value)
	{
		WriteBool(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void WriteChar(int value)
	{
		WriteChar(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void WriteUChar(int value)
	{
		WriteUChar(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void WriteShort(int value)
	{
		WriteShort(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void WriteUShort(int value)
	{
		WriteUShort(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void Write(float value, int bits)
	{
		Write(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	//! @note converts float to fp16
	override void WriteHalf(float value)
	{
		WriteHalf(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void Write(vector value, int bits)
	{
		Write(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	//! @note converts float to fp16
	override void WriteHalf(vector value)
	{
		WriteHalf(m_Serializer, value, m_Packed, m_PackedBits);
	}

	//! @note bits is used to encode string length, not characters!
	override void Write(string value, int bits)
	{
		Write(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	//! @note bits is used to encode string length, not characters!
	override void Write(string value, int bits, string alphabet)
	{
		Write(m_Serializer, value, bits, alphabet, m_Packed, m_PackedBits);
	}

	override void WriteClassName(string value)
	{
		WriteClassName(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void WriteClassNameLower(string value)
	{
		WriteClassNameLower(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override void Flush()
	{
		Flush(m_Serializer, m_Packed, m_PackedBits);
	}

	/* Pass through write -------------------------------------------------- */

	override void Write(bool value)
	{
		m_Serializer.Write(value);
	}

	override void Write(int value)
	{
		m_Serializer.Write(value);
	}

	override void Write(float value)
	{
		m_Serializer.Write(value);
	}

	override void Write(vector value)
	{
		m_Serializer.Write(value);
	}

	override void Write(string value)
	{
		m_Serializer.Write(value);
	}

	override void Write(TBoolArray values)
	{
		m_Serializer.Write(values);
	}

	override void Write(TIntArray values)
	{
		m_Serializer.Write(values);
	}

	override void Write(TFloatArray values)
	{
		m_Serializer.Write(values);
	}

	override void Write(TStringArray values)
	{
		m_Serializer.Write(values);
	}

	override void Write(TVectorArray values)
	{
		m_Serializer.Write(values);
	}
}

class ExpansionBitStreamReader: ExpansionBitStream
{
	bool ReadUInt(out int value, int bits);

	bool Read(out int value, int bits);

	bool Read(out int value, int min, int max);

	bool ReadBool(out bool value);

	bool ReadChar(out int value);

	bool ReadUChar(out int value);

	bool ReadShort(out int value);

	bool ReadUShort(out int value);

	bool Read(out float value, int bits);

	bool ReadHalf(out float value);

	bool Read(out vector value, int bits);

	bool ReadHalf(out vector value);

	bool Read(out string value, int bits);

	bool Read(out string value, int bits, string alphabet);

	bool ReadClassName(out string value);

	bool ReadClassNameLower(out string value);

	void Reset();

	/* Pass through read --------------------------------------------------- */

	bool Read(out bool value);

	bool Read(out int value);

	bool Read(out float value);

	bool Read(out vector value);

	bool Read(out string value);

	bool Read(out TBoolArray values);

	bool Read(out TIntArray values);

	bool Read(out TFloatArray values);

	bool Read(out TStringArray values);

	bool Read(out TVectorArray values);
}

class ExpansionBitStreamReaderT<Class T>: ExpansionBitStreamReader
{
	T m_Serializer;

	void ExpansionBitStreamReaderT(T serializer)
	{
		m_Serializer = serializer;
	}

	/**
	 * @brief Read value from next <n> bits
	 * 
	 * @param [out] value  Unpacked value
	 * @param bits
	 */
	static bool ReadUInt(T serializer, out int value, int bits, inout int packed, inout int packedBits)
	{
		if (bits < 1 || bits > BIT_INT_SIZE)
		{
			CF.FormatError("Invalid value %1 for parameter `bits`: Out of range [1..%2]", bits.ToString(), BIT_INT_SIZE.ToString());
			return false;
		}

		value = 0;

		int remainingBits = bits;
		int shift;

		while (remainingBits > 0)
		{
			if (packedBits <= 0)
			{
				if (!serializer.Read(packed))
					return false;

				packedBits = BIT_INT_SIZE;
			}

			int chunkBits;

			if (remainingBits < packedBits)
				chunkBits = remainingBits;
			else
				chunkBits = packedBits;

			int chunk = packed;

			if (chunkBits < BIT_INT_SIZE)
				chunk &= (1 << chunkBits) - 1;

			value |= (chunk << shift);

			packed >>= chunkBits;
			packedBits -= chunkBits;

			shift += chunkBits;
			remainingBits -= chunkBits;
		}

		if (bits < BIT_INT_SIZE)
			value &= ((1 << bits) - 1);

		return true;
	}

	static bool Read(T serializer, out int value, int bits, inout int packed, inout int packedBits)
	{
		if (!ReadUInt(serializer, value, bits, packed, packedBits))
			return false;

		if (bits < BIT_INT_SIZE)
			value -= Math.Pow(2, bits - 1);

		return true;
	}

	static bool Read(T serializer, out int value, int min, int max, inout int packed, inout int packedBits)
	{
		if (!ReadUInt(serializer, value, BitSize(max - min), packed, packedBits))
			return false;

		value += min;

		return true;
	}

	static bool ReadBool(T serializer, out bool value, inout int packed, inout int packedBits)
	{
		int v;

		if (!ReadUInt(serializer, v, 1, packed, packedBits))
			return false;

		value = v;

		return true;
	}

	static bool ReadChar(T serializer, out int value, inout int packed, inout int packedBits)
	{
		if (!ReadUInt(serializer, value, 8, packed, packedBits))
			return false;

		value -= 128;

		return true;
	}

	static bool ReadUChar(T serializer, out int value, inout int packed, inout int packedBits)
	{
		return ReadUInt(serializer, value, 8, packed, packedBits);
	}

	static bool ReadShort(T serializer, out int value, inout int packed, inout int packedBits)
	{
		if (!ReadUInt(serializer, value, 16, packed, packedBits))
			return false;

		value -= 32768;

		return true;
	}

	static bool ReadUShort(T serializer, out int value, inout int packed, inout int packedBits)
	{
		return ReadUInt(serializer, value, 16, packed, packedBits);
	}

	static bool Read(T serializer, out float value, int bits, inout int packed, inout int packedBits)
	{
		int v;

		if (!ReadUInt(serializer, v, bits, packed, packedBits))
			return false;

		value = CF_Cast<int, float>.Reinterpret(v << (BIT_INT_SIZE - bits));

		return true;
	}

	//! @note reads fp16 and converts to float
	static bool ReadHalf(T serializer, out float value, inout int packed, inout int packedBits)
	{
		int fp16;

		if (!ReadUInt(serializer, fp16, 16, packed, packedBits))
			return false;

		value = FP16ToFloat(fp16);

		return true;
	}

	static bool Read(T serializer, out vector value, int bits, inout int packed, inout int packedBits)
	{
		for (int i = 0; i < 3; ++i)
		{
			float v;

			if (!Read(serializer, v, bits, packed, packedBits))
				return false;

			value[i] = v;
		}

		return true;
	}

	//! @note reads fp16 and converts to float
	static bool ReadHalf(T serializer, out vector value, inout int packed, inout int packedBits)
	{
		for (int i = 0; i < 3; ++i)
		{
			float v;

			if (!ReadHalf(serializer, v, packed, packedBits))
				return false;

			value[i] = v;
		}

		return true;
	}

	//! @note bits is used to decode string length, not characters!
	static bool Read(T serializer, out string value, int bits, inout int packed, inout int packedBits)
	{
		int length;

		if (!ReadUInt(serializer, length, bits, packed, packedBits))
			return false;

		if (length < 0)
			return false;

		value = "";

		while (length--)
		{
			int v;

			if (!ReadUInt(serializer, v, 8, packed, packedBits))
				return false;

			value += v.AsciiToString();
		}

		return true;
	}

	static bool Read(T serializer, out string value, int bits, string alphabet, inout int packed, inout int packedBits)
	{
		int length;

		if (!ReadUInt(serializer, length, bits, packed, packedBits))
			return false;

		if (length < 0)
			return false;

		int alphabetBits = BitSize(alphabet.Length());
		int maxIndex = alphabet.Length() - 1;

		value = "";

		while (length--)
		{
			int i;

			if (!ReadUInt(serializer, i, alphabetBits, packed, packedBits))
				return false;

			if (i > maxIndex)
			{
				CF.FormatError("Index out of bounds [0..%1]: %2", maxIndex.ToString(), i.ToString());
				continue;
			}

			value += alphabet[i];
		}

		return true;
	}

	static bool ReadClassName(T serializer, out string value, inout int packed, inout int packedBits)
	{
		return Read(serializer, value, 7, CLASSNAME_ALPHABET, packed, packedBits);
	}

	static bool ReadClassNameLower(T serializer, out string value, inout int packed, inout int packedBits)
	{
		return Read(serializer, value, 7, CLASSNAME_ALPHABET_LOWERCASE, packed, packedBits);
	}

	static void Reset(out int packed, out int packedBits)
	{
		packed = 0;
		packedBits = 0;
	}

	override bool ReadUInt(out int value, int bits)
	{
		return ReadUInt(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	override bool Read(out int value, int bits)
	{
		return Read(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	override bool Read(out int value, int min, int max)
	{
		return Read(m_Serializer, value, min, max, m_Packed, m_PackedBits);
	}

	override bool ReadBool(out bool value)
	{
		return ReadBool(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override bool ReadChar(out int value)
	{
		return ReadChar(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override bool ReadUChar(out int value)
	{
		return ReadUChar(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override bool ReadShort(out int value)
	{
		return ReadShort(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override bool ReadUShort(out int value)
	{
		return ReadUShort(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override bool Read(out float value, int bits)
	{
		return Read(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	//! @note reads fp16 and converts to float
	override bool ReadHalf(out float value)
	{
		return ReadHalf(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override bool Read(out vector value, int bits)
	{
		return Read(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	//! @note reads fp16 and converts to float
	override bool ReadHalf(out vector value)
	{
		return ReadHalf(m_Serializer, value, m_Packed, m_PackedBits);
	}

	//! @note bits is used to decode string length, not characters!
	override bool Read(out string value, int bits)
	{
		return Read(m_Serializer, value, bits, m_Packed, m_PackedBits);
	}

	//! @note bits is used to decode string length, not characters!
	override bool Read(out string value, int bits, string alphabet)
	{
		return Read(m_Serializer, value, bits, alphabet, m_Packed, m_PackedBits);
	}

	override bool ReadClassName(out string value)
	{
		return ReadClassName(m_Serializer, value, m_Packed, m_PackedBits);
	}

	override bool ReadClassNameLower(out string value)
	{
		return ReadClassNameLower(m_Serializer, value, m_Packed, m_PackedBits);
	}

	/**
	 * @brief Reset internal state. Only call this if you know what you're doing.
	 */
	override void Reset()
	{
		m_Packed = 0;
		m_PackedBits = 0;
	}

	/* Pass through read --------------------------------------------------- */

	override bool Read(out bool value)
	{
		return m_Serializer.Read(value);
	}

	override bool Read(out int value)
	{
		return m_Serializer.Read(value);
	}

	override bool Read(out float value)
	{
		return m_Serializer.Read(value);
	}

	override bool Read(out vector value)
	{
		return m_Serializer.Read(value);
	}

	override bool Read(out string value)
	{
		return m_Serializer.Read(value);
	}

	override bool Read(out TBoolArray values)
	{
		return m_Serializer.Read(values);
	}

	override bool Read(out TIntArray values)
	{
		return m_Serializer.Read(values);
	}

	override bool Read(out TFloatArray values)
	{
		return m_Serializer.Read(values);
	}

	override bool Read(out TStringArray values)
	{
		return m_Serializer.Read(values);
	}

	override bool Read(out TVectorArray values)
	{
		return m_Serializer.Read(values);
	}
}

class ExpansionArraySerializer
{
	ref TIntArray m_Data;
	int m_Index;

	void ExpansionArraySerializer(TIntArray data = null)
	{
		if (data)
			m_Data = data;
		else
			m_Data = {};
	}

	void Write(bool value)
	{
		Write((int)value);
	}

	void Write(int value)
	{
		m_Data.Insert(value);
	}

	void Write(float value)
	{
		Write(CF_Cast<float, int>.Reinterpret(value));
	}

	void Write(vector value)
	{
		for (int i = 0; i < 3; ++i)
			Write(value[i]);
	}

	void Write(string value)
	{
		int packed;
		int packedBits;
		ExpansionBitStreamArrayWriter.Write(this, value, 32, packed, packedBits);
		ExpansionBitStreamArrayWriter.Flush(this, packed, packedBits);
	}

	void Write(TBoolArray values)
	{
		Write(values.Count());
		foreach (bool value: values)
			Write(value);
	}

	void Write(TIntArray values)
	{
		Write(values.Count());
		foreach (int value: values)
			Write(value);
	}

	void Write(TFloatArray values)
	{
		Write(values.Count());
		foreach (float value: values)
			Write(value);
	}

	void Write(TStringArray values)
	{
		Write(values.Count());
		foreach (string value: values)
			Write(value);
	}

	void Write(TVectorArray values)
	{
		Write(values.Count());
		foreach (vector value: values)
			Write(value);
	}

	int Count()
	{
		return m_Data.Count();
	}

	int Get(int index)
	{
		return m_Data[index];
	}

	void Seek(int index)
	{
		if (index >= m_Data.Count())
			CF.FormatError("Index %1 out of bounds [0..%2]", index.ToString(), (m_Data.Count() - 1).ToString());
		else
			m_Index = index;
	}

	bool Read(out bool value)
	{
		int v;
		if (!Read(v))
			return false;

		value = v;

		return true;
	}

	bool Read(out int value)
	{
		if (m_Index >= m_Data.Count())
			return false;

		value = m_Data[m_Index++];

		return true;
	}

	bool Read(out float value)
	{
		int v;
		if (!Read(v))
			return false;

		value = CF_Cast<int, float>.Reinterpret(v);

		return true;
	}

	bool Read(out vector value)
	{
		for (int i = 0; i < 3; ++i)
		{
			float v;
			if (!Read(v))
				return false;

			value[i] = v;
		}

		return true;
	}

	bool Read(out string value)
	{
		int packed;
		int packedBits;
		return ExpansionBitStreamArrayReader.Read(this, value, 32, packed, packedBits);
	}

	bool Read(out TBoolArray values)
	{
		int count;
		if (!Read(count) || count < 0)
			return false;

		values = {};

		while (count--)
		{
			bool v;
			if (!Read(v))
				return false;

			values.Insert(v);
		}

		return true;
	}

	bool Read(out TIntArray values)
	{
		int count;
		if (!Read(count) || count < 0)
			return false;

		values = {};

		while (count--)
		{
			int v;
			if (!Read(v))
				return false;

			values.Insert(v);
		}

		return true;
	}

	bool Read(out TFloatArray values)
	{
		int count;
		if (!Read(count) || count < 0)
			return false;

		values = {};

		while (count--)
		{
			float v;
			if (!Read(v))
				return false;

			values.Insert(v);
		}

		return true;
	}

	bool Read(out TStringArray values)
	{
		int count;
		if (!Read(count) || count < 0)
			return false;

		values = {};

		while (count--)
		{
			string v;
			if (!Read(v))
				return false;

			values.Insert(v);
		}

		return true;
	}

	bool Read(out TVectorArray values)
	{
		int count;
		if (!Read(count) || count < 0)
			return false;

		values = {};

		while (count--)
		{
			vector v;
			if (!Read(v))
				return false;

			values.Insert(v);
		}

		return true;
	}
}

typedef ExpansionBitStreamWriterT<ExpansionArraySerializer> ExpansionBitStreamArrayWriter;
typedef ExpansionBitStreamReaderT<ExpansionArraySerializer> ExpansionBitStreamArrayReader;
typedef ExpansionBitStreamWriterT<Serializer> ExpansionBitStreamSerializerWriter;
typedef ExpansionBitStreamReaderT<Serializer> ExpansionBitStreamSerializerReader;
