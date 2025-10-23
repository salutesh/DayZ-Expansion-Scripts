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
	static const string ZERO_WIDTH_SPACE = "​"; //! This may look like an empty string, but it is not. It's a zero-width space as UTF-8;

	static const int PERMUTATION_TABLE[256] = {
		98, 6, 85, 150, 36, 23, 112, 164, 135, 207, 169, 5, 26, 64, 165, 219,
		61, 20, 68, 89, 130, 63, 52, 102, 24, 229, 132, 245, 80, 216, 195, 115,
		90, 168, 156, 203, 177, 120, 2, 190, 188, 7, 100, 185, 174, 243, 162, 10,
		237, 18, 253, 225, 8, 208, 172, 244, 255, 126, 101, 79, 145, 235, 228, 121,
		123, 251, 67, 250, 161, 0, 107, 97, 241, 111, 181, 82, 249, 33, 69, 55,
		59, 153, 29, 9, 213, 167, 84, 93, 30, 46, 94, 75, 151, 114, 73, 222,
		197, 96, 210, 45, 16, 227, 248, 202, 51, 152, 252, 125, 81, 206, 215, 186,
		39, 158, 178, 187, 131, 136, 1, 49, 50, 17, 141, 91, 47, 129, 60, 99,
		154, 35, 86, 171, 105, 34, 38, 200, 147, 58, 77, 118, 173, 246, 76, 254,
		133, 232, 196, 144, 198, 124, 53, 4, 108, 74, 223, 234, 134, 230, 157, 139,
		189, 205, 199, 128, 176, 19, 211, 236, 127, 192, 231, 70, 233, 88, 146, 44,
		183, 201, 22, 83, 13, 214, 116, 109, 159, 32, 95, 226, 140, 220, 57, 12,
		221, 31, 209, 182, 143, 92, 149, 184, 148, 62, 113, 65, 37, 27, 106, 166,
		3, 14, 204, 72, 21, 41, 56, 66, 28, 193, 40, 217, 25, 54, 179, 117,
		238, 87, 240, 155, 180, 170, 242, 212, 191, 163, 78, 218, 137, 194, 175, 110,
		43, 119, 224, 71, 122, 142, 42, 160, 104, 48, 247, 103, 15, 11, 138, 239
	};

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

	int DJB2Hash()
	{
		int hash = 5381;

		for (int i = 0; i < value.Length(); ++i)
		{
			hash = ((hash << 5) + hash) + value[i].ToAscii();  //! Equivalent to hash * 33 + value[i].ToAscii()
		}

		return hash;
	}
	
	static int DJB2Hash(string str)
	{
		ExpansionString tmp = str;
		return tmp.DJB2Hash();
	}

	int JenkinsHash()
	{
		int hash = 0;

		for (int i = 0; i < value.Length(); ++i)
		{
			hash += value[i].ToAscii();
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
	
	static int JenkinsHash(string str)
	{
		ExpansionString tmp = str;
		return tmp.JenkinsHash();
	}

	int PearsonHash8()
	{
		int hash = value.Length();  //! Initial value can be anything

		for (int i = 0; i < value.Length(); ++i)
		{
			int ascii = value[i].ToAscii();
			hash = PERMUTATION_TABLE[(hash ^ ascii) & 0xff];
		}

		return hash;
	}
	
	static int PearsonHash8(string str)
	{
		ExpansionString tmp = str;
		return tmp.PearsonHash8();
	}

	//! Neat little hash function, good for small datasets (< 64 k)
	//! https://stackoverflow.com/a/19661491
	int Hash16()
	{
		int hash;

		for (int i = 0; i < value.Length(); ++i)
		{
			int ascii = value[i].ToAscii() + i;
			hash += (hash << 5) + ascii + (ascii << 7);
		}

		return (hash ^ (hash >> 16)) & 0xffff;
	}
	
	static int Hash16(string str)
	{
		ExpansionString tmp = str;
		return tmp.Hash16();
	}

	//! Like vanilla Hash(), but using different prime (109 instead of 37) for better collision resilience
	int BetterHash()
	{
		int hash;

		for (int i = 0; i < value.Length(); ++i)
		{
			int ascii = value[i].ToAscii();
			hash = hash * 109 + ascii;
			if (ascii > 127)
				hash -= 256;
		}
		   
		return hash;
	}
	
	static int BetterHash(string str)
	{
		ExpansionString tmp = str;
		return tmp.BetterHash();
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

	string Reversed()
	{
		string result;

		for (int i = value.Length() - 1; i >= 0; --i)
		{
			result += value[i];
		}

		return result;
	}

	bool StartsWith(string prefix)
	{
		return value.IndexOf(prefix) == 0;
	}

	static bool StartsWith(string str, string prefix)
	{
		return str.IndexOf(prefix) == 0;
	}

	static bool StartsWithAny(string text, TStringArray words)
	{
		foreach (string word: words)
		{
			if (ExpansionString.StartsWith(text, word))
				return true;
		}

		return false;
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

	/**
	 * @brief Find longest common prefix in values
	 * 
	 * @param prefix   Starting prefix
	 * @param values
	 * @param skipNotFound  If true, skip values where a common prefix was not found (else return empty string)
	 * 
	 * @return longest common prefix or empty string
	 */
	static string FindLongestCommonPrefix(string prefix, TStringArray values, bool skipNotFound = false)
	{
		if (values.Count() == 0)
			return "";

		bool found;

		foreach (string current: values)
		{
			int i = 0;

			while (i < prefix.Length() && i < current.Length() && prefix[i] == current[i])
				i++;

			if (i == 0)
			{
				if (skipNotFound)
					continue;

				return "";
			}
			else
			{
				found = true;
			}

			prefix = prefix.Substring(0, i);
		}

		if (found)
			return prefix;

		return "";
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
