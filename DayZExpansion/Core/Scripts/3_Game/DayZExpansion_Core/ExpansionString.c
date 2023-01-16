/**
 * ExpansionString.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionString
{
	const string ZERO_WIDTH_SPACE = "​"; //! This may look like an empty string, but it is not. It's a zero-width space as UTF-8;

	static ref map<string, int> s_ToAscii = new map<string, int>;

	protected string m_String;

	void ExpansionString(string str)
	{
		m_String = str;
	}

	string Get()
	{
		return m_String;
	}

	bool StartsWith(string prefix)
	{
		return m_String.IndexOf(prefix) == 0;
	}

	static bool StartsWith(string str, string prefix)
	{
		return str.IndexOf(prefix) == 0;
	}

	bool StartsWithIgnoreCase(string prefix)
	{
		return StartsWithIgnoreCase(m_String, prefix);
	}

	static bool StartsWithIgnoreCase(string str, string prefix)
	{
		str.ToLower();
		prefix.ToLower();
		return StartsWith(str, prefix);
	}

	bool EndsWith(string suffix)
	{
		return EndsWith(m_String, suffix);
	}

	static bool EndsWith(string str, string suffix)
	{
		int suffixLength = suffix.Length();
		if (str.Length() < suffixLength)
			return false;
		return str.IndexOf(suffix) == str.Length() - suffixLength;
	}

	bool EndsWithIgnoreCase(string suffix)
	{
		return EndsWithIgnoreCase(m_String, suffix);
	}

	static bool EndsWithIgnoreCase(string str, string suffix)
	{
		str.ToLower();
		suffix.ToLower();
		return EndsWith(str, suffix);
	}

	bool Equals(string cmp)
	{
		return m_String == cmp;
	}

	//! DEPRECATED, alias for EqualsIgnoreCase
	bool EqualsCaseInsensitive(string cmp)
	{
		Error("DEPRECATED, use EqualsIgnoreCase");
		return EqualsIgnoreCase(m_String, cmp);
	}

	bool EqualsIgnoreCase(string cmp)
	{
		return EqualsIgnoreCase(m_String, cmp);
	}

	static bool EqualsIgnoreCase(string str, string cmp)
	{
		str.ToLower();
		cmp.ToLower();
		return str == cmp;
	}

	//! Reimplement LastIndexOf (vanilla string.LastIndexOf is broken...)
	int LastIndexOf(string sample)
	{
		return LastIndexOf(m_String, sample);
	}

	static int LastIndexOf(string str, string sample)
	{
		int idx = -1;
		int lastIdx = -1;
		while (true)
		{
			idx = str.IndexOfFrom(idx + 1, sample);
			if (idx > -1)
				lastIdx = idx;
			else
				break;
		}
		return lastIdx;
	}

	//! Reimplement ToAscii (vanilla string.ToAscii is broken...)
	int ToAscii()
	{
		return ToAscii(m_String[0]);
	}

	static int ToAscii(string character)
	{
		if (!character)
		{
			Error("Expected a character, but got a string of length 0");
			return NAN;
		}

		if (!s_ToAscii.Count())
		{
			for (int i = 0; i < 256; i++)
			{
				s_ToAscii.Insert(i.AsciiToString(), i);
			}
		}

		return s_ToAscii.Get(character[0]);
	}

	/**
	 * @brief Removes the first character from the string
	 * 
	 * @param str input string
	 * 
	 * @return a copy of str with the first character removed
	 */
	string RemoveFirstChar() 
	{
		return m_String.Substring( 1, m_String.Length() - 1 );
	}

	static string RemoveFirstChar( string str ) 
	{
		return str.Substring( 1, str.Length() - 1 );
	}

	/**
	 * @brief Removes the last character from the string
	 * 
	 * @param str input string
	 * 
	 * @return a copy of str with the last character removed
	 */
	string RemoveLastChar() 
	{
		return m_String.Substring( 0, m_String.Length() - 1 );
	}

	static string RemoveLastChar( string str ) 
	{
		return str.Substring( 0, str.Length() - 1 );
	}

	static string JoinStrings(array<ExpansionString> strings, string glue = ", ")
	{
		string output = "";
		for (int i = 0; i < strings.Count(); i++)
		{
			if (glue && output)
				output += glue;
			output += strings[i].Get();
		}
		return output;
	}

	static string JoinStrings(TStringArray strings, string glue = ", ")
	{
		string output = "";
		for (int i = 0; i < strings.Count(); i++)
		{
			if (glue && output)
				output += glue;
			output += strings[i];
		}
		return output;
	}
}
