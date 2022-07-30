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

	bool EndsWith(string suffix)
	{
		int suffixLength = suffix.Length();
		if (m_String.Length() < suffixLength)
			return false;
		return m_String.IndexOf(suffix) == m_String.Length() - suffixLength;
	}

	bool EqualsCaseInsensitive(string cmp)
	{
		string str = m_String;
		str.ToLower();
		cmp.ToLower();
		return str == cmp;
	}

	//! Reimplement LastIndexOf (vanilla string.LastIndexOf is broken...)
	int LastIndexOf(string sample)
	{
		int idx = -1;
		int lastIdx = -1;
		while (true)
		{
			idx = m_String.IndexOfFrom(idx + 1, sample);
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
			return 0;

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
	 * @brief Removes the last character from the string
	 * 
	 * @param str input string
	 * 
	 * @return a copy of str with the last character removed
	 */
	static string RemoveLastChar( string str ) 
	{
		return str.Substring( 0, str.Length() - 1 );
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
