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

class ExpansionString: string
{
	const string ZERO_WIDTH_SPACE = "​"; //! This may look like an empty string, but it is not. It's a zero-width space as UTF-8;

/*
	protected string value;

	void ExpansionString(string str)
	{
		value = str;
	}

	string Get()
	{
		return value;
	}
*/

	string BaseName()
	{
		string tmp = value;
		tmp.Replace("\\", "/");
		int index = tmp.LastIndexOf("/");
		if (index > -1)
			return value.Substring(index + 1, value.Length() - index - 1);
		return value;
	}

	static string BaseName(string fileName)
	{
		ExpansionString tmp = fileName;
		return tmp.BaseName();
	}

	string DirName()
	{
		string tmp = value;
		tmp.Replace("\\", "/");
		int index = tmp.LastIndexOf("/");
		if (index > -1)
			return value.Substring(0, index);
		return value;
	}

	static string DirName(string fileName)
	{
		ExpansionString tmp = fileName;
		return tmp.DirName();
	}

	//! Neat little hash function, good for small datasets (< 64 k)
	//! https://stackoverflow.com/a/19661491
	static int Hash16(string str)
	{
		int hash;

		for (int i = 0; i < str.Length(); i++)
		{
			int ch = str[i].Hash() + i;
			hash += (hash << 5) + ch + (ch << 7);
		}

		return (hash ^ (hash >> 16)) & 0xffff;
	}

	//! Like vanilla Hash(), but using different prime (109 instead of 37) for better collision resilience
	static int BetterHash(string str)
	{
		int hash;

		for (int i = 0; i < str.Length(); i++)
		{
			int ascii = str[i].Hash();
			hash = hash * 109 + ascii;
			if (ascii > 127)
				hash -= 256;
		}
		   
		return hash;
	}

	static string CamelCaseToWords(string str, string sep = " ")
	{
		string output;
		string c;
		string cUpper;
		string cLower;
		bool wasLower;

		for (int j = 0; j < str.Length(); j++)
		{
			c = str[j];
			cUpper = c;
			cUpper.ToUpper();
			cLower = c;
			cLower.ToLower();
			if (wasLower && c == cUpper && c != cLower)
				output += sep;
			output += c;
			//! https://feedback.bistudio.com/T173348
			if (c != cUpper && c == cLower)
				wasLower = true;
			else
				wasLower = false;
		}

		return output;
	}

	static bool ContainsAny(string text, TStringArray words)
	{
		foreach (string word: words)
		{
			if (text.Contains(word))
				return true;
		}

		return false;
	}

	//! Vanilla string.Replace truncates long text >:-(
	//! https://feedback.bistudio.com/T177558
	static int Replace(inout string text, string search, string replace, bool ignoreCase = false)
	{
		string tmp;
		int index;

		if (ignoreCase)
		{
			tmp = text;
			tmp.ToLower();
			search.ToLower();
			index = tmp.IndexOf(search);
		}
		else
		{
			index = text.IndexOf(search);
		}

		int count;
		int searchLen = search.Length();
		int replaceLen = replace.Length();

		while (index > -1)
		{
			text = text.Substring(0, index) + replace + text.Substring(index + searchLen, text.Length() - index - searchLen);
			count++;
			if (ignoreCase)
			{
				tmp = tmp.Substring(0, index) + replace + tmp.Substring(index + searchLen, tmp.Length() - index - searchLen);
				index = tmp.IndexOfFrom(index + replaceLen, search);
			}
			else
			{
				index = text.IndexOfFrom(index + replaceLen, search);
			}
		}

		return count;
	}

	bool StartsWith(string prefix)
	{
		return value.IndexOf(prefix) == 0;
	}

	static bool StartsWith(string str, string prefix)
	{
		return str.IndexOf(prefix) == 0;
	}

	bool StartsWithIgnoreCase(string prefix)
	{
		ExpansionString tmp = value;
		tmp.ToLower();
		prefix.ToLower();
		return tmp.StartsWith(prefix);
	}

	static bool StartsWithIgnoreCase(string str, string prefix)
	{
		ExpansionString tmp = str;
		return tmp.StartsWithIgnoreCase(prefix);
	}

	bool EndsWith(string suffix, out int index = -1)
	{
		int suffixLength = suffix.Length();
		if (value.Length() < suffixLength)
			return false;
		index = value.LastIndexOf(suffix);
		return index == value.Length() - suffixLength;
	}

	static bool EndsWith(string str, string suffix, out int index = -1)
	{
		ExpansionString tmp = str;
		return tmp.EndsWith(suffix, index);
	}

	bool EndsWithIgnoreCase(string suffix, out int index = -1)
	{
		ExpansionString tmp = value;
		tmp.ToLower();
		suffix.ToLower();
		return tmp.EndsWith(suffix, index);
	}

	static bool EndsWithIgnoreCase(string str, string suffix, out int index = -1)
	{
		ExpansionString tmp = str;
		return tmp.EndsWithIgnoreCase(suffix, index);
	}

	bool Equals(string cmp)
	{
		return value == cmp;
	}

	//! DEPRECATED, alias for EqualsIgnoreCase
	bool EqualsCaseInsensitive(string cmp)
	{
		Error("DEPRECATED, use EqualsIgnoreCase");
		return EqualsIgnoreCase(cmp);
	}

	bool EqualsIgnoreCase(string cmp)
	{
		ExpansionString tmp = value;
		tmp.ToLower();
		cmp.ToLower();
		return tmp == cmp;
	}

	static bool EqualsIgnoreCase(string str, string cmp)
	{
		ExpansionString tmp = str;
		return tmp.EqualsIgnoreCase(cmp);
	}

/*
	//! Reimplement LastIndexOf (vanilla string.LastIndexOf is broken...)
	int LastIndexOf(string sample)
	{
		if (sample == string.Empty)
			return 0;
		int idx = -1;
		int lastIdx = -1;
		while (true)
		{
			idx = value.IndexOfFrom(idx + 1, sample);
			if (idx > -1)
				lastIdx = idx;
			else
				break;
		}
		return lastIdx;
	}
*/

	static int LastIndexOf(string str, string sample)
	{
		return str.LastIndexOf(sample);
	}

	string JustifyLeft(int length, string pad)
	{
		string tmp = value;
		length = length - value.Length();

		for (int index = 0; index < length; index++)
		{
			tmp += pad;
		}

		return tmp;
	}

	static string JustifyLeft(string str, int length, string pad)
	{
		ExpansionString tmp = str;
		return tmp.JustifyLeft(length, pad);
	}

	string JustifyRight(int length, string pad)
	{
		string tmp = value;
		length = length - value.Length();

		for (int index = 0; index < length; index++)
		{
			tmp = pad + tmp;
		}

		return tmp;
	}

	static string JustifyRight(string str, int length, string pad)
	{
		ExpansionString tmp = str;
		return tmp.JustifyRight(length, pad);
	}

/*
	//! Reimplement ToAscii (vanilla string.ToAscii is broken...)
	int ToAscii(int index = 0)
	{
		return value[index].Hash();
	}
*/

	static int ToAscii(string str, int index = 0)
	{
		return str[index].ToAscii();
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
		return value.Substring( 1, value.Length() - 1 );
	}

	static string RemoveFirstChar( string str ) 
	{
		return str.Substring( 1, str.Length() - 1 );
	}

	string StripExtension(string ext = string.Empty)
	{
		if (ext == string.Empty)
		{
			int index = value.LastIndexOf(".");

			if (index > -1)
				return value.Substring(0, index);

			return value;
		}

		ExpansionString tmp = value;
		if (tmp.EndsWithIgnoreCase(ext))
			return value.Substring( 0, value.Length() - ext.Length() );

		return value;
	}

	static string StripExtension(string str, string ext)
	{
		ExpansionString tmp = str;
		return tmp.StripExtension(ext);
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
		return value.Substring( 0, value.Length() - 1 );
	}

	static string RemoveLastChar( string str ) 
	{
		return str.Substring( 0, str.Length() - 1 );
	}

	static string JoinStrings(array<ExpansionString> strings, string glue = ", ", bool reverse = false)
	{
		string output = "";
		int count = strings.Count();
		int i, delta;

		if (reverse)
		{
			i = count - 1;
			delta = -1;
		}
		else
		{
			i = 0;
			delta = 1;
		}

		while (count--)
		{
			if (glue && output)
				output += glue;
			output += strings[i];
			i += delta;
		}

		return output;
	}

	static string JoinStrings(TStringArray strings, string glue = ", ", bool reverse = false)
	{
		string output = "";
		int count = strings.Count();
		int i, delta;

		if (reverse)
		{
			i = count - 1;
			delta = -1;
		}
		else
		{
			i = 0;
			delta = 1;
		}

		while (count--)
		{
			if (glue && output)
				output += glue;
			output += strings[i];
			i += delta;
		}

		return output;
	}

	static int StrCmp(string a, string b)
	{
		for (int i = 0; i < Math.Min(a.Length(), b.Length()); i++)
		{
			if (a[i] < b[i])
				return -1;
			else if (a[i] > b[i])
				return 1;
		}

		return a.Length() - b.Length();
	}

	static int StrCaseCmp(string a, string b)
	{
		a.ToLower();
		b.ToLower();

		return StrCmp(a, b);
	}
}

typedef string ExpansionString;
