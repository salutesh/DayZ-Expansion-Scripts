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
		return null;
	}

	ExpansionJSONElementBase Get(string name)
	{
		return null;
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

	override ExpansionJSONElementBase Get(int index)
	{
		return new ExpansionJSONUndefined;
	}

	override ExpansionJSONElementBase Get(string name)
	{
		return new ExpansionJSONUndefined;
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

class ExpansionJSON: ExpansionJSONObject
{
	//! @note the following are all related to parser state (see Reset/DumpState)
	int m_StateChangeCount;
	int m_PrevStateChangeCount;
	ExpansionJSONElementBase m_Current;
	ref array<ExpansionJSONElementBase> m_CurrentHierarchy = {};
#ifdef DIAG
	ref TStringArray m_CurrentHierarchyPath = {};
#endif
	string m_Buffer;
	int m_CurrentLineOffset;
	int m_CurrentOffset;
	int m_LineNumber;
	bool m_Escaped;
	bool m_InString;
	bool m_IsNumber;
	string m_Token;
	int m_DecimalPointCount;
	int m_ExponentCount;
	bool m_ResetToken;
	bool m_HasKey;
	string m_Key;
	bool m_ExpectToken;
	bool m_ExpectBool;
	bool m_ExpectUnicode;
	string m_UnicodeEscapeSequence;
	string m_Expect;
	string m_Error;

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

	static ExpansionJSON FromString(string data)
	{
		auto json = new ExpansionJSON;

		if (json.Parse(data))
			return json;

		return null;
	}

	/**
	 * @brief Reset internal parser state
	 */
	void Reset()
	{
		m_Elements.Clear();

		m_StateChangeCount = 0;
		m_PrevStateChangeCount = 0;
		m_Current = this;
		m_CurrentHierarchy.Clear();
		m_CurrentHierarchy.Insert(this);
	#ifdef DIAG
		m_CurrentHierarchyPath.Clear();
		m_CurrentHierarchyPath.Insert("ROOT");
	#endif
		m_Buffer = "";
		m_CurrentLineOffset = 0;
		m_CurrentOffset = 0;
		m_LineNumber = 1;
		m_Escaped = false;
		m_InString = false;
		m_IsNumber = false;
		m_Token = "";
		m_DecimalPointCount = 0;
		m_ExponentCount = 0;
		m_ResetToken = false;
		m_HasKey = false;
		m_Key = "";
		m_ExpectToken = false;
		m_ExpectBool = false;
		m_ExpectUnicode = false;
		m_UnicodeEscapeSequence = "";
		m_Expect = "{";
		m_Error = "";
	}

	bool Parse(string data)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
	#endif

		Reset();

	#ifdef DIAG
		DumpState();
	#endif

		m_Buffer = data;
		data = "";

		while (m_CurrentOffset < m_Buffer.Length())
		{
			if (!ParseSingleCharacter(m_Buffer[m_CurrentOffset], false))
				return false;
		}

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

				if (m_Expect)
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
				else if (m_ExpectToken && !m_Token)
				{
					switch (c)
					{
						case "}":
						case "]":
						case ":":
						case ",":
							return ParseError("Invalid character '%1' at line %2 column %3, expected token", c);
					}
				}
			}
			else if (c == "\\" && !m_Escaped)
			{
				m_Escaped = true;

			#ifdef DIAG
				DumpState();
			#endif

				return IncrementOffset();
			}
			else if (m_Escaped)
			{
				if (c == "u")
				{
					m_ExpectUnicode = true;
				}
				else if (m_ExpectUnicode)
				{
					if (m_UnicodeEscapeSequence.Length() < 4)
					{
						m_UnicodeEscapeSequence += c;

						if (m_UnicodeEscapeSequence.Length() == 4)
						{
							m_Token += UnescapeUnicode(m_UnicodeEscapeSequence);
							m_ExpectUnicode = false;
							m_Escaped = false;
						}
					}
				}
				else
				{
					m_Token += Unescape(c);
					m_Escaped = false;
				}


			#ifdef DIAG
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

			m_ResetToken = false;

			switch (c)
			{
				case "{":
					if (m_ExpectToken)
						OnStartObjectEx(m_Key);

					m_HasKey = false;
					m_Expect = "\"";
					m_ExpectToken = false;

				#ifdef DIAG
					m_StateChangeCount++;
				#endif
					break;

				case "}":
					if (m_Current.IsInherited(ExpansionJSONObject))
					{
						if (m_Token)
						{
							//! float, int, bool or null
							ParseToken(m_Token);

							m_HasKey = false;
							m_Key = "";
							m_ResetToken = true;
							m_ExpectToken = false;
						}

						if (m_Current != this)
							OnEndObjectEx();
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3, expected token", c);
					}

				#ifdef DIAG
					m_StateChangeCount++;
				#endif
					break;

				case "[":
					OnStartArray(m_Key);

				#ifdef DIAG
					m_StateChangeCount++;
				#endif
					break;

				case "]":
					if (m_Current.IsInherited(ExpansionJSONArray))
					{
						if (m_Token)
						{
							//! float, int, bool or null
							ParseToken(m_Token);

							m_HasKey = false;
							m_Key = "";
							m_ResetToken = true;
							m_ExpectToken = false;
						}

						OnEndArray(m_Current.Count());
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}

				#ifdef DIAG
					m_StateChangeCount++;
				#endif
					break;

				case "\"":
					if (m_InString)
					{
						m_InString = false;

						if (m_Current.IsInherited(ExpansionJSONObject) && !m_HasKey)
						{
							m_Key = m_Token;
							m_ResetToken = true;
							m_ExpectToken = false;
							m_Expect = ":";
						}
						else
						{
							OnString(m_Key, m_Token);

							m_ResetToken = true;
							m_ExpectToken = false;
							m_HasKey = false;
							m_Key = "";
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
						m_ExpectToken = false;
					}

				#ifdef DIAG
					m_StateChangeCount++;
				#endif
					break;

				case ":":
					if (m_Current.IsInherited(ExpansionJSONObject) && !m_HasKey)
					{
						m_HasKey = true;
						m_ExpectToken = true;
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}

				#ifdef DIAG
					m_StateChangeCount++;
				#endif
					break;

				case ",":
					if (m_Token)
					{
						//! float, int, bool or null
						ParseToken(m_Token);

						m_HasKey = false;
						m_Key = "";
						m_ResetToken = true;
					}

					if (m_Current.IsInherited(ExpansionJSONArray))
					{
						m_ExpectToken = true;
					}
					else
					{
						m_Expect = "\"";
						m_ExpectToken = false;
					}

				#ifdef DIAG
					m_StateChangeCount++;
				#endif
					break;

				//! true
				case "t":
					m_ExpectBool = true;
					Expect(c, "r");
					break;

				case "r":
					Expect(c, "u");
					break;

				case "u":
					if (m_ExpectBool)
						Expect(c, "e");  //! Expect true
					else
						Expect(c, "l");  //! Expect null
					break;

				//! false
				case "f":
					m_ExpectBool = true;
					Expect(c, "a");
					break;

				case "a":
					Expect(c, "l");
					break;

				case "l":
					if (m_ExpectBool)
					{
						Expect(c, "s");  //! Expect false
					}
					else if (m_Token.Length() < 3)
					{
						Expect(c, "l");  //! Expect null
					}
					else
					{
						m_Token += c;

					#ifdef DIAG
						m_StateChangeCount++;
					#endif
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
					m_IsNumber = true;
					Expect(c, "0123456789");
					break;

				case ".":
					if (m_DecimalPointCount)
						return ParseError("Invalid character '%1' at line %2 column %3, expected one of " + m_Expect, c);
					m_DecimalPointCount++;
					Expect(c, "0123456789");
					break;

				case "e":
					if (m_IsNumber)
					{
						if (m_ExponentCount)
							return ParseError("Invalid character '%1' at line %2 column %3, expected one of " + m_Expect, c);
						m_ExponentCount++;
						Expect(c, "+-0123456789");
					}
					else
					{
						m_Token += c;

					#ifdef DIAG
						m_StateChangeCount++;
					#endif
					}
					break;

				default:
					if (c.ToInt().ToString() == c)
					{
						m_IsNumber = true;
						Expect(c, ".0123456789e,]}");
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}
					break;
			}

		#ifdef DIAG
			if (m_StateChangeCount != m_PrevStateChangeCount)
			{
				DumpState();
				m_PrevStateChangeCount = m_StateChangeCount;
			}
		#endif

			if (m_ResetToken)
				m_Token = "";

			return IncrementOffset();
		}

		return false;
	}

	void Expect(string c, string e)
	{
		m_Token += c;
		m_Expect = e;

	#ifdef DIAG
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
	#ifdef DIAG
		DiagPrint("OnStartObjectEx");
	#endif

		Insert(name, new ExpansionJSONObject);
	}

	void OnEndObjectEx()
	{
	#ifdef DIAG
		DiagPrint("OnEndObjectEx %1", string.Join("", m_CurrentHierarchyPath));
	#endif

		OnEndElement();
	}

	override void OnStartArray(string name)
	{
	#ifdef DIAG
		DiagPrint("OnStartArray");
	#endif

		Insert(name, new ExpansionJSONArray);
	}

	override void OnEndArray(int itemCount)
	{
	#ifdef DIAG
		DiagPrint("OnEndArray %1", string.Join("", m_CurrentHierarchyPath));
	#endif

		OnEndElement();
	}

	void OnEndElement()
	{
	#ifdef DIAG
		ExpansionJSONElementBase element = m_Current;
	#endif

		m_CurrentHierarchy.Remove(m_CurrentHierarchy.Count() - 1);
		m_Current = m_CurrentHierarchy[m_CurrentHierarchy.Count() - 1];

	#ifdef DIAG
		m_CurrentHierarchyPath.Remove(m_CurrentHierarchyPath.Count() - 1);
		DiagPrint("OnEndElement %1 -> %2", element.ToString(), m_Current.ToString());
	#endif
	}

	override void OnString(string name, string value)
	{
		Insert(name, new ExpansionJSONString(value));
	}

	bool ParseToken(string token)
	{
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
				float f = token.ToFloat();
				int i = token.ToInt();
				if (f != i)
					element = new ExpansionJSONFloat(f);
				else
					element = new ExpansionJSONInt(i);
				m_DecimalPointCount = 0;
				m_ExponentCount = 0;
				m_IsNumber = false;
				break;
		}

		m_ExpectBool = false;

		return Insert(m_Key, element);
	}

	override bool Insert(string name, ExpansionJSONElementBase element)
	{
		ExpansionJSONArray arr;
		ExpansionJSONObject obj;

		if (Class.CastTo(arr, m_Current))
		{
		#ifdef DIAG
			DiagPrint("Insert %1 %2 %3", string.Join("", m_CurrentHierarchyPath), arr.Count().ToString(), element.ToString());

			if (element.IsInherited(ExpansionJSONArray) || element.IsInherited(ExpansionJSONObject))
				m_CurrentHierarchyPath.Insert(string.Format("[%1]", arr.Count()));
		#endif

			arr.Insert(element);
		}
		else if (Class.CastTo(obj, m_Current))
		{
		#ifdef DIAG
			DiagPrint("Insert %1 %2 %3", string.Join("", m_CurrentHierarchyPath), name, element.ToString());

			if (element.IsInherited(ExpansionJSONArray) || element.IsInherited(ExpansionJSONObject))
				m_CurrentHierarchyPath.Insert(string.Format("[\"%1\"]", name));
		#endif

			obj[name] = element;
		}

		if (element.IsInherited(ExpansionJSONArray) || element.IsInherited(ExpansionJSONObject))
		{
			m_Current = element;
			m_CurrentHierarchy.Insert(element);
		}

		return true;
	}

	void InsertAt(int index, ExpansionJSONElementBase element)
	{
	#ifdef DIAG
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

		DiagPrint(prefix, "m_InString = " + m_InString);
		DiagPrint(prefix, "m_Escaped = " + m_Escaped);
		DiagPrint(prefix, "m_Token = " + m_Token);
		DiagPrint(prefix, "m_ResetToken = " + m_ResetToken);
		DiagPrint(prefix, "m_HasKey = " + m_HasKey);
		DiagPrint(prefix, "m_Key = " + m_Key);
		DiagPrint(prefix, "m_ExpectToken = " + m_ExpectToken);
		DiagPrint(prefix, "m_ExpectUnicode = " + m_ExpectUnicode);
		DiagPrint(prefix, "m_UnicodeEscapeSequence = " + m_UnicodeEscapeSequence);
		DiagPrint(prefix, "m_Expect = " + m_Expect);
		DiagPrint("------------------------------------------------------------------------");
	}
}
