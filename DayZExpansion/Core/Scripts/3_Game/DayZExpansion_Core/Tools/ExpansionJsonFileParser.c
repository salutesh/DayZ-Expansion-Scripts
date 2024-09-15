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
	static const string BACKSPACE = (8).AsciiToString();
	static const string FORMFEED = (12).AsciiToString();

	static string s_Indent;

	typename GetValueType()
	{
		typename nullType;
		return nullType;
	}

	array<ref ExpansionJSONElementBase> GetArray()
	{
		return null;
	}

	map<string, ref ExpansionJSONElementBase> GetMap()
	{
		return null;
	}

	TBoolArray GetBoolArray()
	{
		return null;
	}

	TIntArray GetIntArray()
	{
		return null;
	}

	TFloatArray GetFloatArray()
	{
		return null;
	}

	TStringArray GetStringArray()
	{
		return null;
	}

	int Insert(ExpansionJSONElementBase element)
	{
		return 0;
	}

	int InsertAt(ExpansionJSONElementBase element, int index)
	{
		return 0;
	}

	bool Insert(string name, ExpansionJSONElementBase element)
	{
		return false;
	}

	void Set(string name, ExpansionJSONElementBase element)
	{
	}

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

	override string GetDebugName()
	{
		return ToString();
	}

	bool IsNull()
	{
		return false;
	}

	bool IsUndefined()
	{
		return false;
	}

	bool IsCollection()
	{
		return false;
	}

	bool IsArray()
	{
		return false;
	}

	bool IsObject()
	{
		return false;
	}

	int Count()
	{
		return 0;
	}

	string ToJSONString(bool pretty = true)
	{
		return "";
	}

	static string ToJSONString(string value)
	{
		value.Replace(BACKSPACE, "\\b");
		value.Replace(FORMFEED, "\\f");
		value.Replace("\n", "\\n");
		value.Replace("\r", "\\r");
		value.Replace("\t", "\\t");
		value.Replace("\"", "\\" + "\"");
		return string.Format("\"%1\"", value);
	}
}

class ExpansionJSONBool: ExpansionJSONElementBase
{
	bool m_Value;

	void ExpansionJSONBool(bool value)
	{
		m_Value = value;
	}

	override typename GetValueType()
	{
		return bool;
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

	override string GetDebugName()
	{
		return string.Format("%1(%2)", ToString(), GetString());
	}

	override string ToJSONString(bool pretty = true)
	{
		return GetString();
	}
}

class ExpansionJSONFloat: ExpansionJSONElementBase
{
	float m_Value;

	void ExpansionJSONFloat(float value)
	{
		m_Value = value;
	}

	override typename GetValueType()
	{
		return float;
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
		string s = m_Value.ToString();
		if (s.IndexOf(".") == -1 && s.IndexOf("e") == -1)
			s += ".0";
		return s;
	}

	override string GetDebugName()
	{
		return string.Format("%1(%2)", ToString(), GetString());
	}

	override string ToJSONString(bool pretty = true)
	{
		return GetString();
	}
}

class ExpansionJSONInt: ExpansionJSONElementBase
{
	int m_Value;

	void ExpansionJSONInt(int value)
	{
		m_Value = value;
	}

	override typename GetValueType()
	{
		return int;
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

	override string GetDebugName()
	{
		return string.Format("%1(%2)", ToString(), m_Value);
	}

	override string ToJSONString(bool pretty = true)
	{
		return GetString();
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

	override string ToJSONString(bool pretty = true)
	{
		return "null";
	}
}

class ExpansionJSONString: ExpansionJSONElementBase
{
	string m_Value;

	void ExpansionJSONString(string value)
	{
		m_Value = value;
	}

	override typename GetValueType()
	{
		return string;
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

	override string GetDebugName()
	{
		return string.Format("%1(%2)", ToString(), ToJSONString(m_Value));
	}

	override string ToJSONString(bool pretty = true)
	{
		return ToJSONString(m_Value);
	}
}

class ExpansionJSONUndefined: ExpansionJSONElementBase
{
	override bool IsUndefined()
	{
		return true;
	}
}

class ExpansionJSONCollection: ExpansionJSONElementBase
{
	override bool IsCollection()
	{
		return true;
	}
}

class ExpansionJSONArray: ExpansionJSONCollection
{
	ref array<ref ExpansionJSONElementBase> m_Elements = new array<ref ExpansionJSONElementBase>;

	override typename GetValueType()
	{
		return array;
	}

	override array<ref ExpansionJSONElementBase> GetArray()
	{
		return m_Elements;
	}

	override TBoolArray GetBoolArray()
	{
		TBoolArray a = {};

		foreach (ExpansionJSONElementBase element: m_Elements)
		{
			a.Insert(element.GetBool());
		}

		return a;
	}

	override TIntArray GetIntArray()
	{
		TIntArray a = {};

		foreach (ExpansionJSONElementBase element: m_Elements)
		{
			a.Insert(element.GetInt());
		}

		return a;
	}

	override TFloatArray GetFloatArray()
	{
		TFloatArray a = {};

		foreach (ExpansionJSONElementBase element: m_Elements)
		{
			a.Insert(element.GetFloat());
		}

		return a;
	}

	override TStringArray GetStringArray()
	{
		TStringArray a = {};

		foreach (ExpansionJSONElementBase element: m_Elements)
		{
			a.Insert(element.GetString());
		}

		return a;
	}

	override int Insert(ExpansionJSONElementBase element)
	{
		return m_Elements.Insert(element);
	}

	override int InsertAt(ExpansionJSONElementBase element, int index)
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

	override bool IsArray()
	{
		return true;
	}

	override int Count()
	{
		return m_Elements.Count();
	}

	override string ToJSONString(bool pretty = true)
	{
		string s = "[";

		if (pretty && m_Elements.Count())
		{
			s += "\n";
			s_Indent += "    ";
		}

		foreach (int i, ExpansionJSONElementBase element: m_Elements)
		{
			if (i)
			{
				s += ",";

				if (pretty)
					s += "\n";
			}

			if (pretty)
				s += s_Indent;

			s += element.ToJSONString(pretty);
		}

		if (pretty && m_Elements.Count())
		{
			if (i)
				s += "\n";

			s_Indent = s_Indent.Substring(0, s_Indent.Length() - 4);
			s += s_Indent;
		}

		s += "]";

		return s;
	}
}

class ExpansionJSONObject: ExpansionJSONCollection
{
	ref TStringArray m_Keys = {};
	ref map<string, ref ExpansionJSONElementBase> m_Elements = new map<string, ref ExpansionJSONElementBase>;

	override typename GetValueType()
	{
		return map;
	}

	override map<string, ref ExpansionJSONElementBase> GetMap()
	{
		return m_Elements;
	}

	override bool Insert(string name, ExpansionJSONElementBase element)
	{
		m_Keys.Insert(name);

		return m_Elements.Insert(name, element);
	}

	override void Set(string name, ExpansionJSONElementBase element)
	{
		if (!m_Elements.Contains(name))
			m_Keys.Insert(name);

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

	override bool IsObject()
	{
		return true;
	}

	override int Count()
	{
		return m_Elements.Count();
	}

	override string ToJSONString(bool pretty = true)
	{
		string s = "{";

		if (pretty && m_Elements.Count())
		{
			s += "\n";
			s_Indent += "    ";
		}

		foreach (int i, string name: m_Keys)
		{
			ExpansionJSONElementBase element = m_Elements[name];

			if (i)
			{
				s += ",";

				if (pretty)
					s += "\n";
			}

			if (pretty)
				s += s_Indent;

			s += ToJSONString(name) + ": " + element.ToJSONString(pretty);
		}

		if (pretty && m_Elements.Count())
		{
			if (i)
				s += "\n";

			s_Indent = s_Indent.Substring(0, s_Indent.Length() - 4);
			s += s_Indent;
		}

		s += "}";

		return s;
	}
}

#ifdef DIAG_DEVELOPER
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
	static const int TOKEN_ERROR = 0;
	static const int TOKEN_SPECIAL = 1;
	static const int TOKEN_STRING = 2;
	static const int TOKEN_LITERAL = 3;
	static const int TOKEN_NUMBER = 4;
	static const int TOKEN_EOL = 5;

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
	string m_LineBuffer;
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
	bool m_ExpectEOF;
	string m_Error;

	override typename GetValueType()
	{
		return m_Root.GetValueType();
	}

	override array<ref ExpansionJSONElementBase> GetArray()
	{
		return m_Root.GetArray();
	}

	override map<string, ref ExpansionJSONElementBase> GetMap()
	{
		return m_Root.GetMap();
	}

	override TBoolArray GetBoolArray()
	{
		return m_Root.GetBoolArray();
	}

	override TIntArray GetIntArray()
	{
		return m_Root.GetIntArray();
	}

	override TFloatArray GetFloatArray()
	{
		return m_Root.GetFloatArray();
	}

	override TStringArray GetStringArray()
	{
		return m_Root.GetStringArray();
	}

	override int Insert(ExpansionJSONElementBase element)
	{
		return m_Root.Insert(element);
	}

	override int InsertAt(ExpansionJSONElementBase element, int index)
	{
		return m_Root.InsertAt(element, index);
	}

	override bool Insert(string name, ExpansionJSONElementBase element)
	{
		return m_Root.Insert(name, element);
	}

	override void Set(string name, ExpansionJSONElementBase element)
	{
		m_Root[name] = element;
	}

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

	override bool IsCollection()
	{
		return m_Root.IsCollection();
	}

	override bool IsArray()
	{
		return m_Root.IsArray();
	}

	override bool IsObject()
	{
		return m_Root.IsObject();
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

		int n;
		while (FGets(file, line) >= 0)
		{
			if (n++)
				data += "\n";

			data += line;
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

	override string ToJSONString(bool pretty = true)
	{
		return m_Root.ToJSONString(pretty);
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
		m_LineBuffer = "";
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
		m_Expect = TOKEN_START;
		m_ExpectEOF = false;
		m_Error = "";
	}

	bool Parse(string data)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
	#endif

		Reset();

	#ifdef EXPANSIONJSON_DIAG
		DumpState();
	#endif

	#ifdef EXPANSIONJSON_PARSEBYCHAR
		m_Buffer = data;
		data = "";

		while (m_CurrentOffset < m_Buffer.Length())
		{
			if (!ParseSingleCharacter(m_Buffer[m_CurrentOffset], false))
				return false;
		}
	#else
		string token;
		int type;

		while (data)
		{
			type = data.ParseStringEx(token);
			if (!type || !ParseToken(type, token))
				return false;
		}
	#endif

		if (m_InString)
			return ParseError("Unexpected EOF at line %2 column %3, expected '\"'");
		else if (m_Expect && !m_ExpectEOF)
			return ParseError("Unexpected EOF at line %2 column %3, expected '%4'", "", m_Expect);
		else if (m_InNumber)
			ParseNumber(m_Token);

	#ifdef EXPANSIONJSON_PARSEBYCHAR
		m_Buffer = "";
	#endif

		return true;
	}

	bool ParseToken(int type, string token)
	{
		switch (type)
		{
			case TOKEN_EOL:
				m_LineBuffer = "";
				m_LineNumber++;

				return true;

			case TOKEN_LITERAL:
				m_LineBuffer += token;

				if (m_Current && m_Current.IsObject() && !m_HasKey)
					return ParseError("Invalid character '%1' at line %2 column %3, expected '%4'", token, m_Expect);

				if (!ParseLiteral(token))
					return false;

				Expect("", ",", true);

			#ifdef EXPANSIONJSON_DIAG
				DumpState();
			#endif

				return true;

			case TOKEN_NUMBER:
				m_LineBuffer += token;

				if (m_Current && m_Current.IsObject() && !m_HasKey)
					return ParseError("Invalid character '%1' at line %2 column %3, expected '%4'", token, m_Expect);

				if (!ParseNumber(token))
					return false;

				Expect("", ",", true);

			#ifdef EXPANSIONJSON_DIAG
				DumpState();
			#endif

				return true;

			case TOKEN_STRING:
				m_LineBuffer += "\"" + token + "\"";

				if (m_Current && m_Current.IsObject() && !m_HasKey)
				{
					m_Key = token;
					Expect("", ":");
				}
				else
				{
					if (!ParseString(token))
						return false;

					Expect("", ",", true);
				}

			#ifdef EXPANSIONJSON_DIAG
				DumpState();
			#endif

				return true;

			case TOKEN_SPECIAL:
				m_LineBuffer += token;

				if (!m_Current && !m_Root.IsUndefined())
				{
					if (token == ",")
						return ParseError("Invalid character '%1' at line %2 column %3 (JSON can only have one root element)", token);
					else
						return ParseError("Invalid character '%1' at line %2 column %3", token);
				}
				else if (m_Expect)
				{
					if (m_Expect.IndexOf(token) > -1)
						m_Expect = "";
					else
						return ParseError("Invalid character '%1' at line %2 column %3, expected '%4'", token, m_Expect);
				}

				break;
		}

		switch (token)
		{
			case "{":
				OnStartObjectEx(m_Key);

				Expect("", "\"", true);

				break;

			case "}":
				OnEndObjectEx();

				if (m_Current)
					Expect("", ",", true);
			#ifdef EXPANSIONJSON_DIAG
				else
					m_StateChangeCount++;
			#endif

				break;

			case "[":
				OnStartArray(m_Key);

				Expect("", TOKEN_START, true);

				break;

			case "]":
				OnEndArray(m_Current.Count());

				if (m_Current)
					Expect("", ",", true);
			#ifdef EXPANSIONJSON_DIAG
				else
					m_StateChangeCount++;
			#endif

				break;

			case ":":
				m_HasKey = true;

				Expect("", TOKEN_START);

				break;

			case ",":
				if (!m_Current)
					return ParseError("Invalid character '%1' at line %2 column %3 (JSON can only have one root element)", token);
				else if (m_Current.IsArray())
					Expect("", TOKEN_START);
				else
					Expect("", "\"");

				break;

			default:
				return ParseError("Invalid character '%1' at line %2 column %3, expected '%4'", token, m_Expect);
		}

	#ifdef EXPANSIONJSON_DIAG
		DumpState();
	#endif

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
				if (m_InString)
					return ParseError("Unterminated string at line %2 column %3, expected '\"'");

				if (m_InNumber)
					ParseNumber(m_Token);

				m_LineBuffer = "";
				m_LineNumber++;
			}
			else
			{
				m_LineBuffer += c;
			}

			if (!m_InString)
			{
				if (!c.Trim() || c == "\n")
				{
					//! Skip whitespace if not in string
					return IncrementOffset();
				}

				if (!m_Current && !m_Root.IsUndefined())
				{
					if (c == ",")
						return ParseError("Invalid character '%1' at line %2 column %3 (JSON can only have one root element)", c);
					else
						return ParseError("Invalid character '%1' at line %2 column %3", c);
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
			#endif

				return IncrementOffset();
			}
			else if (m_Escaped)
			{
				if (c == "u")
				{
					if (m_Buffer.Length() > m_CurrentOffset + 4)
					{
						//! If we have a buffer, look ahead
						m_Token += UnescapeUnicode(m_Buffer.Substring(m_CurrentOffset + 1, 4));

						m_CurrentOffset += 4;

						m_Escaped = false;
					}
					else
					{
						m_InUnicodeEscapeSequence = true;
					}
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

				if (!m_Escaped)
					m_UnicodeEscapeSequence = "";

			#ifdef EXPANSIONJSON_DIAG
				m_StateChangeCount++;
			#endif

				return IncrementOffset();
			}
			else if (c != "\"")
			{
				if (m_Buffer.Length() > m_CurrentOffset + 1)
				{
					//! If we have a buffer, look ahead

					int index = m_Buffer.IndexOfFrom(m_CurrentOffset, "\"");

					if (index > -1)
					{
						int len = index - m_CurrentOffset;

						if (len > 0)
						{
							string token = m_Buffer.Substring(m_CurrentOffset, len);

							index = token.IndexOf("\\");
							int newline = token.IndexOf("\n");

							if (newline > -1 && (index == -1 || newline < index))
								index = newline;

							if (index > -1)
							{
								len = index;
								token = token.Substring(0, len);
							}

							m_Token += token;
						}

						m_CurrentOffset += len;

						return true;
					}
				}

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
					/*
					if (m_Current.IsObject())
					{
					*/
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
					/*
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}
					*/

					break;

				case "[":
					OnStartArray(m_Key);

					Expect("", TOKEN_START, true);

					break;

				case "]":
					/*
					if (m_Current.IsArray())
					{
					*/
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
					/*
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}
					*/

					break;

				case "\"":
					if (m_InString)
					{
						m_InString = false;

						if (m_Current && m_Current.IsObject() && !m_HasKey)
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
					/*
					else if (m_Token)
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}
					*/
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
					/*
					if (m_Current.IsObject() && !m_HasKey)
					{
					*/
						m_HasKey = true;

						Expect("", TOKEN_START);
					/*
					}
					else
					{
						return ParseError("Invalid character '%1' at line %2 column %3", c);
					}
					*/

					break;

				case ",":
					if (m_Token)
					{
						//! float or int
						ParseNumber(m_Token);
					}

					if (!m_Current)
					{
						return ParseError("Invalid character '%1' at line %2 column %3 (JSON can only have one root element)", c);
					}
					else if (m_Current.IsArray())
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
					if (m_Buffer.Length() > m_CurrentOffset + 3)
					{
						//! If we have a buffer, look ahead
						m_Token += m_Buffer.Substring(m_CurrentOffset, 4);

						m_CurrentOffset += 3;

						Expect("", ",", true);

						if (!ParseLiteral(m_Token))
							return false;
					}
					else
					{
						m_InBool = true;
						Expect(c, "r");
					}
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
					if (m_Buffer.Length() > m_CurrentOffset + 4)
					{
						//! If we have a buffer, look ahead
						m_Token += m_Buffer.Substring(m_CurrentOffset, 5);

						m_CurrentOffset += 4;

						Expect("", ",", true);

						if (!ParseLiteral(m_Token))
							return false;
					}
					else
					{
						m_InBool = true;
						Expect(c, "a");
					}
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
						ParseLiteral(m_Token);
					}
					break;

				case "s":
					Expect(c, "e");
					break;

				//! null
				case "n":
					if (m_Buffer.Length() > m_CurrentOffset + 3)
					{
						//! If we have a buffer, look ahead
						m_Token += m_Buffer.Substring(m_CurrentOffset, 4);

						m_CurrentOffset += 3;

						Expect("", ",", true);

						if (!ParseLiteral(m_Token))
							return false;
					}
					else
					{
						Expect(c, "u");
					}
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
						ParseLiteral(m_Token);
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

			return IncrementOffset();
		}

		return false;
	}

	void Expect(string c, string e, bool expectElementEnd = false)
	{
	#ifdef EXPANSIONJSON_PARSEBYCHAR
		if (c)
			m_Token += c;
	#endif

		m_Expect = e;
		m_ExpectEOF = false;

		if (expectElementEnd)
		{
			if (m_Current)
			{
				if (m_Current.IsArray())
					m_Expect += "]";
				else
					m_Expect += "}";
			}
			else
			{
				m_ExpectEOF = true;
			}
		}

	#ifdef EXPANSIONJSON_DIAG
		m_StateChangeCount++;
	#endif
	}

	bool IncrementOffset()
	{
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

		m_CurrentOffset++;
		return true;
	}

	bool ParseError(string error, string c = "", string expect = string.Empty)
	{
		int column = m_LineBuffer.Length() - 1;
			
		string line = m_LineBuffer;
		line.Replace("\t", " ");

		m_Error = string.Format(error, c, m_LineNumber, column, expect) + "\n";
		m_Error += line + "\n";
		m_Error += GetParseMarker();

		Error(m_Error);

		return false;
	}

	string Unescape(string c)
	{
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("Unescape %1", c);
	#endif

		switch (c)
		{
			case "b":
				return BACKSPACE;

			case "f":
				return FORMFEED;

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
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("UnescapeUnicode %1", sequence);
	#endif

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

		InsertIntoCurrentHierarchy(name, new ExpansionJSONObject);
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

		InsertIntoCurrentHierarchy(name, new ExpansionJSONArray);
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
		InsertIntoCurrentHierarchy(name, new ExpansionJSONString(value));

	#ifdef EXPANSIONJSON_PARSEBYCHAR
		m_ResetToken = true;
	#endif
	}

	bool ParseLiteral(string token)
	{
	#ifdef EXPANSIONJSON_PARSEBYCHAR
		m_InBool = false;
		m_ResetToken = true;
	#endif

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
				return ParseError("Invalid token %1 at line %2 column %3", token);
		}

		return InsertIntoCurrentHierarchy(m_Key, element);
	}

	bool ParseNumber(string token)
	{
	#ifdef EXPANSIONJSON_PARSEBYCHAR
		m_HasDecimalPoint = false;
		m_HasExponent = false;
		m_InNumber = false;
		m_ResetToken = true;
	#endif

		ExpansionJSONElementBase element;

		if (token.IndexOf(".") > -1 || token.IndexOf("e") > -1)
			element = new ExpansionJSONFloat(token.ToFloat());
		else
			element = new ExpansionJSONInt(token.ToInt());

		return InsertIntoCurrentHierarchy(m_Key, element);
	}

	bool ParseString(string token)
	{
		int index;
		string s;

		while (true)
		{
			index = token.IndexOf("\\");

			if (index == -1 || index == token.Length() - 1)
			{
				s += token;
				break;
			}

			s += token.Substring(0, index);

			string c = token[index + 1];
			string sequence;

			switch (c)
			{
				case "u":
					sequence = token.Substring(index + 2, 4);
					s += UnescapeUnicode(sequence);
					token = token.Substring(index + 6, token.Length() - index - 6);
					break;

				default:
					s += Unescape(c);
					token = token.Substring(index + 2, token.Length() - index - 2);
					break;
			}
		}

		OnString(m_Key, s);

		return true;
	}

	bool InsertIntoCurrentHierarchy(string name, ExpansionJSONElementBase element)
	{
		ExpansionJSONArray arr;
		ExpansionJSONObject obj;

		if (Class.CastTo(arr, m_Current))
		{
		#ifdef EXPANSIONJSON_DIAG
			DiagPrint("InsertIntoCurrentHierarchy %1 %2 %3", string.Join("", m_CurrentHierarchyPath), arr.Count().ToString(), element.GetDebugName());

			if (element.IsCollection())
				m_CurrentHierarchyPath.Insert(string.Format("[%1]", arr.Count()));
		#endif

			arr.Insert(element);
		}
		else if (Class.CastTo(obj, m_Current))
		{
		#ifdef EXPANSIONJSON_DIAG
			DiagPrint("InsertIntoCurrentHierarchy %1 %2 %3", string.Join("", m_CurrentHierarchyPath), name, element.GetDebugName());

			if (element.IsCollection())
				m_CurrentHierarchyPath.Insert(string.Format("[\"%1\"]", name));
		#endif

			obj[name] = element;
		}
		else
		{
		#ifdef EXPANSIONJSON_DIAG
			DiagPrint("InsertIntoCurrentHierarchy %1", element.GetDebugName());

			m_CurrentHierarchyPath.Insert("ROOT");
		#endif

			m_Root = element;
		}

		if (element.IsCollection())
		{
			m_Current = element;
			m_CurrentHierarchy.Insert(element);
		}

		m_Key = "";
		m_HasKey = false;

		return true;
	}

	void InsertIntoCurrentHierarchyAt(int index, ExpansionJSONElementBase element)
	{
	#ifdef EXPANSIONJSON_DIAG
		DiagPrint("InsertIntoCurrentHierarchyAt %1 %2", index.ToString(), element.GetDebugName());
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

		for (int i = 0; i < m_LineBuffer.Length() - 1; i++)
		{
			marker += "-";
		}

		marker += "^";

		return marker;
	}

	void DumpState()
	{
		string prefix = "JSON parser state[" + m_StateChangeCount + "]: %1";

		if (m_LineBuffer)
		{
			string line = m_LineBuffer;
			line.Replace("\t", " ");
			DiagPrint(prefix, line);
			DiagPrint(prefix, GetParseMarker());
		}

		DiagPrint(prefix, "m_LineNumber = " + m_LineNumber);
	#ifdef EXPANSIONJSON_PARSEBYCHAR
		DiagPrint(prefix, "m_CurrentOffset = " + m_CurrentOffset);
	#endif
		DiagPrint(prefix, "m_HasKey = " + m_HasKey);
		DiagPrint(prefix, "m_Key = " + m_Key);
	#ifdef EXPANSIONJSON_PARSEBYCHAR
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
	#endif
		DiagPrint(prefix, "m_Expect = " + m_Expect);
		DiagPrint(prefix, "m_ExpectEOF = " + m_ExpectEOF);
		DiagPrint("------------------------------------------------------------------------");
	}
}
