/**
 * ExpansionJsonFileParser.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionJsonFileParser<Class T>
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;

	static bool Load(string filename, out T data)
	{
		if (!FileExist(filename))
			return false;

		FileHandle handle = OpenFile(filename, FileMode.READ);
		if (!handle)
			return false;

		string file_content;
		string line_content;

		while (FGets(handle, line_content) >= 0)
		{
			file_content += line_content;
		}

		CloseFile(handle);

		string error;
		bool ret = ReadFromString(file_content, data, error);
		if (!ret)
			Error(string.Format("File %1:\n%2", filename, error));

		return ret;
	}

	static void Save(string filename, T data)
	{
		FileHandle handle = OpenFile(filename, FileMode.WRITE);
		if (!handle)
			return;

		FPrint(handle, AsString(data));

		CloseFile(handle);
	}

	static bool ReadFromString(string string_data, out T data, out string error = string.Empty)
	{
		if (m_Serializer.ReadFromString(data, string_data, error))
			return true;

		return false;
	}

	static string AsString(T data)
	{
		string string_data;
		m_Serializer.WriteToString(data, true, string_data);
		return string_data;
	}
}

class ExpansionJSONElementBase: JsonApiStruct
{
	ExpansionJSONElementBase Get(int index)
	{
		return new ExpansionJSONUndefined;
	}

	ExpansionJSONElementBase Get(string name)
	{
		return new ExpansionJSONUndefined;
	}

	bool GetBool()
	{
		return false;
	}

	int GetInt()
	{
		return 0;
	}

	float GetFloat()
	{
		return 0.0;
	}

	string GetString()
	{
		return string.Empty;
	}

	bool IsNull()
	{
		return false;
	}

	bool IsUndefined()
	{
		return false;
	}

	int Count()
	{
		return 0;
	}
}

class ExpansionJSONBool: ExpansionJSONElementBase
{
	bool m_Value;

	void ExpansionJSONBool(bool value)
	{
		m_Value = value;
	}

	override bool GetBool()
	{
		return m_Value;
	}

	override int GetInt()
	{
		return m_Value;
	}

	override float GetFloat()
	{
		return m_Value;
	}

	override string GetString()
	{
		return m_Value.ToString();
	}
}

class ExpansionJSONFloat: ExpansionJSONElementBase
{
	float m_Value;

	void ExpansionJSONFloat(float value)
	{
		m_Value = value;
	}

	override bool GetBool()
	{
		return m_Value;
	}

	override int GetInt()
	{
		return m_Value;
	}

	override float GetFloat()
	{
		return m_Value;
	}

	override string GetString()
	{
		return m_Value.ToString();
	}
}

class ExpansionJSONInt: ExpansionJSONElementBase
{
	int m_Value;

	void ExpansionJSONInt(int value)
	{
		m_Value = value;
	}

	override bool GetBool()
	{
		return m_Value;
	}

	override int GetInt()
	{
		return m_Value;
	}

	override float GetFloat()
	{
		return m_Value;
	}

	override string GetString()
	{
		return m_Value.ToString();
	}
}

class ExpansionJSONNull: ExpansionJSONElementBase
{
	override bool IsNull()
	{
		return true;
	}

	override string GetString()
	{
		return "NULL";
	}
}

class ExpansionJSONString: ExpansionJSONElementBase
{
	string m_Value;

	void ExpansionJSONString(string value)
	{
		m_Value = value;
	}

	override bool GetBool()
	{
		if (m_Value)
			return true;

		return false;
	}

	override int GetInt()
	{
		return m_Value.ToInt();
	}

	override float GetFloat()
	{
		return m_Value.ToFloat();
	}

	override string GetString()
	{
		return m_Value;
	}
}

class ExpansionJSONUndefined: ExpansionJSONElementBase
{
	override bool IsUndefined()
	{
		return true;
	}
}

class ExpansionJSONArray: ExpansionJSONElementBase
{
	ref array<ref ExpansionJSONElementBase> m_Elements = new array<ref ExpansionJSONElementBase>;

	int Insert(ExpansionJSONElementBase element)
	{
		return m_Elements.Insert(element);
	}

	int InsertAt(ExpansionJSONElementBase element, int index)
	{
		return m_Elements.InsertAt(element, index);
	}

	override ExpansionJSONElementBase Get(int index)
	{
		ExpansionJSONElementBase element = m_Elements[index];
		if (!element)
			element = new ExpansionJSONUndefined;
		return element;
	}

	override int Count()
	{
		return m_Elements.Count();
	}
}

class ExpansionJSONObject: ExpansionJSONElementBase
{
	ref map<string, ref ExpansionJSONElementBase> m_Elements = new map<string, ref ExpansionJSONElementBase>;

	bool Insert(string name, ExpansionJSONElementBase element)
	{
		return m_Elements.Insert(name, element);
	}

	void Set(string name, ExpansionJSONElementBase element)
	{
		m_Elements[name] = element;
	}

	override ExpansionJSONElementBase Get(string name)
	{
		ExpansionJSONElementBase element = m_Elements[name];
		if (!element)
			element = new ExpansionJSONUndefined;
		return element;
	}

	bool Contains(string name)
	{
		return m_Elements.Contains(name);
	}

	override int Count()
	{
		return m_Elements.Count();
	}
}

#ifdef DIAG
#define EXPANSIONJSON_DIAG
#endif

class ExpansionJSON: ExpansionJSONElementBase
{
	static const string NUMBER = "0123456789";
	static const string NUMBER_START = "+-" + NUMBER;
	static const string NUMBER_END = "." + NUMBER + "e,";
	static const string NUMBER_END_DECIMAL = NUMBER + "e,";
	static const string NUMBER_END_EXPONENT = "." + NUMBER + ",";
	static const string NUMBER_END_EXPONENT_DECIMAL = NUMBER + ",";
	static const string TOKEN_START = "\"{[" + NUMBER_START + "tfn";

	ref ExpansionJSONElementBase m_Root;

	//! @note the following are all related to parser state (see Reset/DumpState)
	int m_StateChangeCount;
	int m_PrevStateChangeCount;
	ExpansionJSONElementBase m_Current;
	ref array<ExpansionJSONElementBase> m_CurrentHierarchy = {};
#ifdef EXPANSIONJSON_DIAG
	ref TStringArray m_CurrentHierarchyPath = {};
#endif
	string m_Buffer;
	int m_CurrentLineOffset;
	int m_CurrentOffset;
	int m_LineNumber;
	bool m_HasKey;
	string m_Key;
	string m_Token;
	bool m_ResetToken;
	bool m_InString;
	bool m_InBool;
	bool m_InNumber;
	bool m_HasDecimalPoint;
	bool m_HasExponent;
	bool m_Escaped;
	bool m_InUnicodeEscapeSequence;
	string m_UnicodeEscapeSequence;
	string m_Expect;
	string m_Error;

	override ExpansionJSONElementBase Get(int index)
	{
		return m_Root.Get(index);
	}

	override ExpansionJSONElementBase Get(string name)
	{
		return m_Root.Get(name);
	}

	override bool GetBool()
	{
		return m_Root.GetBool();
	}

	override int GetInt()
	{
		return m_Root.GetInt();
	}

	override float GetFloat()
	{
		return m_Root.GetFloat();
	}

	override string GetString()
	{
		return m_Root.GetString();
	}

	override bool IsNull()
	{
		return m_Root.IsNull();
	}

	override bool IsUndefined()
	{
		return m_Root.IsUndefined();
	}

	override int Count()
	{
		return m_Root.Count();
	}

	static ExpansionJSON Load(string fileName)
	{
		FileHandle file = OpenFile(fileName, FileMode.READ);

		if (!file)
			return null;

		string data;
		string line;

		while (FGets(file, line) >= 0)
		{
			data += line + "\n";
		}

		CloseFile(file);

		return FromString(data);
	}

	static ExpansionJSON FromString(string data, out bool success = false)
	{
		auto json = new ExpansionJSON;

		if (json.Parse(data))
			success = true;

		return json;
	}

	/**
	 * @brief Reset internal parser state
	 */
	void Reset()
	{
		m_Root = new ExpansionJSONUndefined;

		m_StateChangeCount = 0;
		m_PrevStateChangeCount = 0;
		m_Current = null;
		m_CurrentHierarchy.Clear();
	#ifdef EXPANSIONJSON_DIAG
		m_CurrentHierarchyPath.Clear();
	#endif
		m_Buffer = "";
		m_CurrentLineOffset = 0;
		m_CurrentOffset = 0;
		m_LineNumber = 1;
		m_HasKey = false;
		m_Key = "";
		m_Token = "";
		m_ResetToken = false;
		m_InString = false;
		m_InBool = false;
		m_InNumber = false;
		m_HasDecimalPoint = false;
		m_HasExponent = false;
		m_Escaped = false;
		m_InUnicodeEscapeSequence = false;
		m_UnicodeEscapeSequence = "";
		m_Expect = "";
		m_Error = "";
	}

	bool Parse(string data)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
	#endif

		Reset();

	#ifdef EXPANSIONJSON_DIAG
		DumpState();
	#endif

		m_Buffer = data;
		data = "";

		while (m_CurrentOffset < m_Buffer.Length())
		{
			if (!ParseSingleCharacter(m_Buffer[m_CurrentOffset], false))
				return false;
		}

		m_Buffer = "";

		return true;
	}

	bool ParseSingleCharacter(string c, bool buffer = true)
	{
		if (c)
		{
			if (buffer)
				m_Buffer += c;

			if (c == "\n")
			{
				m_CurrentLineOffset = m_CurrentOffset + 1;
				m_LineNumber++;
			}

			if (!m_InString)
			{
				if (!c.Trim() || c == "\n")
				{
					//! Skip whitespace if not in string
					return IncrementOffset();
				}

				if (!m_Current && !m_Root.IsInherited(ExpansionJSONUndefined))
				{
					return ParseError("Invalid character '%1' at line %2 column %3 (JSON can only have one root element)", c);
				}
				else if (m_Expect)
				{
					if (m_Expect.IndexOf(c) > -1)
					{
						m_Expect = "";
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3, expected '%4'", c, m_Expect);
					}
				}
			}
			else if (c == "\\" && !m_Escaped)
			{
				m_Escaped = true;

			#ifdef EXPANSIONJSON_DIAG
				m_StateChangeCount++;
				DumpState();
			#endif

				return IncrementOffset();
			}
			else if (m_Escaped)
			{
				if (c == "u")
				{
					m_InUnicodeEscapeSequence = true;
				}
				else if (m_InUnicodeEscapeSequence)
				{
					if (m_UnicodeEscapeSequence.Length() < 4)
					{
						m_UnicodeEscapeSequence += c;

						if (m_UnicodeEscapeSequence.Length() == 4)
						{
							m_Token += UnescapeUnicode(m_UnicodeEscapeSequence);
							m_InUnicodeEscapeSequence = false;
							m_Escaped = false;
						}
					}
				}
				else
				{
					m_Token += Unescape(c);
					m_Escaped = false;
				}


			#ifdef EXPANSIONJSON_DIAG
				m_StateChangeCount++;
				DumpState();
			#endif

				if (!m_Escaped)
					m_UnicodeEscapeSequence = "";

				return IncrementOffset();
			}
			else if (c != "\"")
			{
				m_Token += c;

				return IncrementOffset();
			}

			switch (c)
			{
				case "{":
					OnStartObjectEx(m_Key);

					Expect("", "\"", true);

					break;

				case "}":
					if (m_Current.IsInherited(ExpansionJSONObject))
					{
						if (m_Token)
						{
							//! float or int
							ParseNumber(m_Token);
						}

						OnEndObjectEx();

						if (m_Current)
							Expect("", ",", true);
					#ifdef EXPANSIONJSON_DIAG
						else
							m_StateChangeCount++;
					#endif
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3, expected token", c);
					}

					break;

				case "[":
					OnStartArray(m_Key);

					Expect("", TOKEN_START, true);

					break;

				case "]":
					if (m_Current.IsInherited(ExpansionJSONArray))
					{
						if (m_Token)
						{
							//! float or int
							ParseNumber(m_Token);
						}

						OnEndArray(m_Current.Count());

						if (m_Current)
							Expect("", ",", true);
					#ifdef EXPANSIONJSON_DIAG
						else
							m_StateChangeCount++;
					#endif
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}

					break;

				case "\"":
					if (m_InString)
					{
						m_InString = false;

						if (m_Current && m_Current.IsInherited(ExpansionJSONObject) && !m_HasKey)
						{
							m_Key = m_Token;
							m_ResetToken = true;
							Expect("", ":");
						}
						else
						{
							OnString(m_Key, m_Token);

							Expect("", ",", true);
						}
					}
					else if (m_Token)
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}
					else
					{
						m_InString = true;
						m_Expect = "";

					#ifdef EXPANSIONJSON_DIAG
						m_StateChangeCount++;
					#endif
					}

					break;

				case ":":
					if (m_Current.IsInherited(ExpansionJSONObject) && !m_HasKey)
					{
						m_HasKey = true;

						Expect("", TOKEN_START);
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}

					break;

				case ",":
					if (m_Token)
					{
						//! float or int
						ParseNumber(m_Token);
					}

					if (m_Current.IsInherited(ExpansionJSONArray))
					{
						Expect("", TOKEN_START);
					}
					else
					{
						Expect("", "\"");
					}

					break;

				//! true
				case "t":
					m_InBool = true;
					Expect(c, "r");
					break;

				case "r":
					Expect(c, "u");
					break;

				case "u":
					if (m_InBool)
						Expect(c, "e");  //! Expect true
					else
						Expect(c, "l");  //! Expect null
					break;

				//! false
				case "f":
					m_InBool = true;
					Expect(c, "a");
					break;

				case "a":
					Expect(c, "l");
					break;

				case "l":
					if (m_InBool)
					{
						Expect(c, "s");  //! Expect false
					}
					else if (m_Token.Length() < 3)
					{
						Expect(c, "l");  //! Expect null
					}
					else
					{
						Expect(c, ",", true);

						//! null
						ParseToken(m_Token);
					}
					break;

				case "s":
					Expect(c, "e");
					break;

				//! null
				case "n":
					Expect(c, "u");
					break;

				//! Numbers
				case "+":
				case "-":
					m_InNumber = true;
					Expect(c, NUMBER);
					break;

				case ".":
					m_HasDecimalPoint = true;
					Expect(c, NUMBER);
					break;

				case "e":
					if (m_InNumber)
					{
						m_HasDecimalPoint = false;
						m_HasExponent = true;
						Expect(c, NUMBER_START);
					}
					else
					{
						Expect(c, ",", true);

						//! true or false
						ParseToken(m_Token);
					}
					break;

				default:
					if (c.ToInt().ToString() == c)
					{
						if (!m_InNumber)
							m_InNumber = true;

						if (m_HasDecimalPoint)
						{
							if (m_HasExponent)
								Expect(c, NUMBER_END_EXPONENT_DECIMAL, true);
							else
								Expect(c, NUMBER_END_DECIMAL, true);
						}
						else
						{
							if (m_HasExponent)
								Expect(c, NUMBER_END_EXPONENT, true);
							else
								Expect(c, NUMBER_END, true);
						}
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}
					break;
			}

		#ifdef EXPANSIONJSON_DIAG
			if (m_StateChangeCount != m_PrevStateChangeCount)
			{
				DumpState();
				m_PrevStateChangeCount = m_StateChangeCount;
			}
		#endif

			if (m_ResetToken)
			{
				m_Token = "";
				m_ResetToken = false;
			}

			return IncrementOffset();
		}

		return false;
	}

	void Expect(string c, string e, bool expectElementEnd = false)
	{
		if (c)
			m_Token += c;

		m_Expect = e;

		if (expectElementEnd && m_Current)
		{
			if (m_Current.IsInherited(ExpansionJSONArray))
				m_Expect += "]";
			else
				m_Expect += "}";
		}

	#ifdef EXPANSIONJSON_DIAG
		m_StateChangeCount++;
	#endif
	}

	bool IncrementOffset()
	{
		m_CurrentOffset++;
		return true;
	}

	bool ParseError(string error, string c, string expect = string.Empty)
	{
		string line = m_Buffer.Substring(m_CurrentLineOffset, m_CurrentOffset - m_CurrentLineOffset + 1);
		line.Replace("\t", " ");

		m_Error = string.Format(error, c, m_LineNumber, m_CurrentOffset - m_CurrentLineOffset, expect) + "\n";
		m_Error += line + "\n";
		m_Error += GetParseMarker();

		Error(m_Error);

		return false;
	}

	string Unescape(string c)
	{
		switch (c)
		{
			case "b":
				return (8).AsciiToString();

			case "f":
				return (12).AsciiToString();

			case "n":
				return "\n";

			case "r":
				return "\r";

			case "t":
				return "\t";
		}

		return c;
	}

	string UnescapeUnicode(string sequence)
	{
		int codepoint = ("0x" + sequence).HexToInt();

		string s;

		if (codepoint <= 0x7f)
		{
			//! Single-byte UTF-8
			s = codepoint.AsciiToString();
		}
		else if (codepoint <= 0x7ff)
		{
			//! Two-byte UTF-8
			s = (0xc0 | (codepoint >> 6)).AsciiToString();
			s += (0x80 | (codepoint & 0x3f)).AsciiToString();
		}
		else if (codepoint <= 0xffff)
		{
			//! Three-byte UTF-8
			s = (0xe0 | (codepoint >> 12)).AsciiToString();
			s += (0x80 | ((codepoint >> 6) & 0x3f)).AsciiToString();
			s += (0x80 | (codepoint & 0x3f)).AsciiToString();
		}
		//! @note four-byte UTF-8 not supported by JSON standard
		else if (codepoint <= 0x10ffff)
		{
			//! Four-byte UTF-8
			s = (0xf0 | (codepoint >> 18)).AsciiToString();
			s += (0x80 | ((codepoint >> 12) & 0x3f)).AsciiToString();
			s += (0x80 | ((codepoint >> 6) & 0x3f)).AsciiToString();
			s += (0x80 | (codepoint & 0x3f)).AsciiToString();
		}
		else
		{
			//! Invalid code point
			Error("Invalid Unicode escape sequence: " + sequence);
		}

		return s;
	}

	void OnStartObjectEx(string name)
	{
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("OnStartObjectEx");
	#endif

		Insert(name, new ExpansionJSONObject);
	}

	void OnEndObjectEx()
	{
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("OnEndObjectEx %1", string.Join("", m_CurrentHierarchyPath));
	#endif

		OnEndElement();
	}

	override void OnStartArray(string name)
	{
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("OnStartArray");
	#endif

		Insert(name, new ExpansionJSONArray);
	}

	override void OnEndArray(int itemCount)
	{
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("OnEndArray %1", string.Join("", m_CurrentHierarchyPath));
	#endif

		OnEndElement();
	}

	void OnEndElement()
	{
	#ifdef EXPANSIONJSON_DIAG
		ExpansionJSONElementBase element = m_Current;
	#endif

		m_CurrentHierarchy.Remove(m_CurrentHierarchy.Count() - 1);
		m_Current = m_CurrentHierarchy[m_CurrentHierarchy.Count() - 1];

	#ifdef EXPANSIONJSON_DIAG
		m_CurrentHierarchyPath.Remove(m_CurrentHierarchyPath.Count() - 1);
		DiagPrint("OnEndElement %1 -> %2", element.ToString(), m_Current.ToString());
	#endif
	}

	override void OnString(string name, string value)
	{
		Insert(name, new ExpansionJSONString(value));

		m_ResetToken = true;
	}

	bool ParseToken(string token)
	{
		m_InBool = false;
		m_ResetToken = true;

		ExpansionJSONElementBase element;

		switch (token)
		{
			case "true":
				element = new ExpansionJSONBool(true);
				break;

			case "false":
				element = new ExpansionJSONBool(false);
				break;

			case "null":
				element = new ExpansionJSONNull();
				break;

			default:
				//! Should never happen
				ParseError("Encountered internal parser error at line %2 column %3, token '%1'", token);
				return false;
		}

		return Insert(m_Key, element);
	}

	bool ParseNumber(string token)
	{
		m_HasDecimalPoint = false;
		m_HasExponent = false;
		m_InNumber = false;
		m_ResetToken = true;

		ExpansionJSONElementBase element;

		float f = token.ToFloat();
		int i = token.ToInt();

		if (f != i)
			element = new ExpansionJSONFloat(f);
		else
			element = new ExpansionJSONInt(i);

		return Insert(m_Key, element);
	}

	bool Insert(string name, ExpansionJSONElementBase element)
	{
		ExpansionJSONArray arr;
		ExpansionJSONObject obj;

		if (Class.CastTo(arr, m_Current))
		{
		#ifdef EXPANSIONJSON_DIAG
			DiagPrint("Insert %1 %2 %3", string.Join("", m_CurrentHierarchyPath), arr.Count().ToString(), element.ToString());

			if (element.IsInherited(ExpansionJSONArray) || element.IsInherited(ExpansionJSONObject))
				m_CurrentHierarchyPath.Insert(string.Format("[%1]", arr.Count()));
		#endif

			arr.Insert(element);
		}
		else if (Class.CastTo(obj, m_Current))
		{
		#ifdef EXPANSIONJSON_DIAG
			DiagPrint("Insert %1 %2 %3", string.Join("", m_CurrentHierarchyPath), name, element.ToString());

			if (element.IsInherited(ExpansionJSONArray) || element.IsInherited(ExpansionJSONObject))
				m_CurrentHierarchyPath.Insert(string.Format("[\"%1\"]", name));
		#endif

			obj[name] = element;
		}
		else
		{
		#ifdef EXPANSIONJSON_DIAG
			m_CurrentHierarchyPath.Insert("ROOT");
		#endif

			m_Root = element;
		}

		if (element.IsInherited(ExpansionJSONArray) || element.IsInherited(ExpansionJSONObject))
		{
			m_Current = element;
			m_CurrentHierarchy.Insert(element);
		}

		m_Key = "";
		m_HasKey = false;

		return true;
	}

	void InsertAt(int index, ExpansionJSONElementBase element)
	{
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("InsertAt %1 %2", index.ToString(), element.ToString());
	#endif

		ExpansionJSONArray arr;
		if (Class.CastTo(arr, m_Current))
			arr.InsertAt(element, index);
	}

	void DiagPrint(string s, string p1 = "", string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		PrintFormat(s, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}

	string GetParseMarker()
	{
		string marker;

		for (int i = m_CurrentLineOffset; i < m_CurrentOffset; i++)
		{
			marker += "-";
		}

		marker += "^";

		return marker;
	}

	void DumpState()
	{
		string prefix = "JSON parser state[" + m_StateChangeCount + "]: %1";

		if (m_Buffer)
		{
			string line = m_Buffer.Substring(m_CurrentLineOffset, m_CurrentOffset - m_CurrentLineOffset + 1);
			line.Replace("\t", " ");
			DiagPrint(prefix, line);
			DiagPrint(prefix, GetParseMarker());
		}

		DiagPrint(prefix, "m_HasKey = " + m_HasKey);
		DiagPrint(prefix, "m_Key = " + m_Key);
		DiagPrint(prefix, "m_Token = " + m_Token);
		DiagPrint(prefix, "m_ResetToken = " + m_ResetToken);
		DiagPrint(prefix, "m_InString = " + m_InString);
		DiagPrint(prefix, "m_InBool = " + m_InBool);
		DiagPrint(prefix, "m_InNumber = " + m_InNumber);
		DiagPrint(prefix, "m_HasDecimalPoint = " + m_HasDecimalPoint);
		DiagPrint(prefix, "m_HasExponent = " + m_HasExponent);
		DiagPrint(prefix, "m_Escaped = " + m_Escaped);
		DiagPrint(prefix, "m_InUnicodeEscapeSequence = " + m_InUnicodeEscapeSequence);
		DiagPrint(prefix, "m_UnicodeEscapeSequence = " + m_UnicodeEscapeSequence);
		DiagPrint(prefix, "m_Expect = " + m_Expect);
		DiagPrint("------------------------------------------------------------------------");
	}
}
