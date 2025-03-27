/**
 * ExpansionSHA256.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**
 * @brief SHA256 implementation in EnforceScript
 * 
 * @code
 *	// Example usage
 *	string inputString = "Hello, EnforceScript!";
 *	ExpansionSHA256.Update(inputString);
 *	string hash = ExpansionSHA256.HexDigest();
 *  ExpansionSHA256.Reset();  // Don't forget to reset for next use!
 *	Print("Input: " + inputString);
 *	Print("SHA-256: " + hash);
 * @endcode
*/
class ExpansionSHA256
{
	protected static CF_Uint s_K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	protected static CF_Uint s_InitialHash[8] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

	protected static CF_Uint s_Hash[8];
	protected static CF_Uint s_TotalBits[2];
	protected static CF_Uint s_Buffer[64];
	protected static int s_BufferIndex;

	protected static bool s_Reset = Reset();
	protected static bool s_Test = DoUnitTest();

	static bool Reset()
	{
		for (int i = 0; i < 8; i++)
		{
			s_Hash[i] = s_InitialHash[i];
		}

		s_TotalBits[0] = 0;
		s_TotalBits[1] = 0;
		s_BufferIndex = 0;

		return true;
	}

	protected static bool DoUnitTest()
	{
		Update("76561197968396131");

	#ifdef DIAG_DEVELOPER
		Print(HexDigest());
	#endif

		CF_Byte digest[32];
		Digest(digest);

		Reset();

		string id = PlayerIdentity.Expansion_EncodeDigest(digest);

	#ifdef DIAG_DEVELOPER
		Print(id);
	#endif

		if (id != "XoIwo96jCqOGY-Rs9_PlsZf1YHNQ-ey54aBO8cuT0VY=")
		{
			Error("ExpansionSHA256 generated wrong hash!");
			return false;
		}

		return true;
	}

	protected static void Process()
	{
		CF_Uint words[64];
		int i;

		for (i = 0; i < 16; i++)
		{
			words[i] = (s_Buffer[i * 4] << 24) | (s_Buffer[i * 4 + 1] << 16) | (s_Buffer[i * 4 + 2] << 8) | s_Buffer[i * 4 + 3];
		}

		CF_Uint s0;
		CF_Uint s1;

		for (i = 16; i < 64; i++)
		{
			s0 = CF_Uint.SIG0(words[i - 15]);
			s1 = CF_Uint.SIG1(words[i - 2]);
			words[i] = words[i - 16].Add(s0).Add(words[i - 7]).Add(s1);
		}

		CF_Uint a = s_Hash[0];
		CF_Uint b = s_Hash[1];
		CF_Uint c = s_Hash[2];
		CF_Uint d = s_Hash[3];
		CF_Uint e = s_Hash[4];
		CF_Uint f = s_Hash[5];
		CF_Uint g = s_Hash[6];
		CF_Uint h = s_Hash[7];

		for (i = 0; i < 64; i++)
		{
			s1 = CF_Uint.EP1(e);
			CF_Uint ch = CF_Uint.CH(e, f, g);
			CF_Uint temp1 = h.Add(s1).Add(ch).Add(s_K[i]).Add(words[i]);
			s0 = CF_Uint.EP0(a);
			CF_Uint maj = CF_Uint.MAJ(a, b, c);
			CF_Uint temp2 = s0.Add(maj);

			h = g;
			g = f;
			f = e;
			e = d.Add(temp1);
			d = c;
			c = b;
			b = a;
			a = temp1.Add(temp2);
		}

		s_Hash[0] = s_Hash[0].Add(a);
		s_Hash[1] = s_Hash[1].Add(b);
		s_Hash[2] = s_Hash[2].Add(c);
		s_Hash[3] = s_Hash[3].Add(d);
		s_Hash[4] = s_Hash[4].Add(e);
		s_Hash[5] = s_Hash[5].Add(f);
		s_Hash[6] = s_Hash[6].Add(g);
		s_Hash[7] = s_Hash[7].Add(h);
	}

	protected static void IncrementTotalBitsBy(CF_Uint amount)
	{
		CF_Uint max = 0xffffffff;
		CF_Uint test = max - amount;

		if (s_TotalBits[0].IsGt(test))
			s_TotalBits[1] = s_TotalBits[1] + 1;

		s_TotalBits[0] = s_TotalBits[0].Add(amount);
	}

	static void Update(string input, bool reset = true, bool finalize = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, ExpansionSHA256);
#endif
		if (reset && IsDirty())
			Reset();

		for (int i = 0; i < input.Length(); i++)
		{
			s_Buffer[s_BufferIndex++] = input[i].ToAscii() & 255;
			if (s_BufferIndex == 64)
			{
				Process();
				IncrementTotalBitsBy(512);
				s_BufferIndex = 0;
			}
		}

		if (finalize)
			Finalize();
	}

	static bool IsDirty()
	{
		if (s_BufferIndex || s_TotalBits)
			return true;

		return false;
	}

	static void Finalize()
	{
		IncrementTotalBitsBy(s_BufferIndex * 8);

		//! Pad the message
		s_Buffer[s_BufferIndex++] = 0x80;
		while (s_BufferIndex != 56)
		{
			if (s_BufferIndex == 64)
			{
				Process();
				s_BufferIndex = 0;
			}
			s_Buffer[s_BufferIndex++] = 0;
		}

		//! Append the total bits at the end
		for (int i = 0, k = 63; i < 2; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s_Buffer[k--] = s_TotalBits[i].ShiftRight(j * 8) & 0xFF;
			}
		}

		Process();
	}

	static void Digest(out CF_Byte digest[32])
	{
		int n;
		for (int i = 0; i < 8; i++)
        {
			for (int j = 0; j < 4; j++)
			{
				CF_Byte value = s_Hash[i].ShiftRight(24 - j * 8) & 0xFF;
				n = i * 4 + j;
				digest[n] = value;
			}
		}
	}

	static string HexDigest()
	{
		string result;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				result += ExpansionStatic.BASE16[s_Hash[i].ShiftRight(28 - j * 4) & 0xF];
			}
		}

		return result;
	}
}
