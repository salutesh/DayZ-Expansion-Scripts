/**
 * ExpansionString.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionString
{
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
}
