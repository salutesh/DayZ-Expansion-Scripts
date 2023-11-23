/**
 * PlayerIdentity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerIdentity
{
	static const string EXPANSION_ID64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	/**
	 * @brief Get plain ID as array of ints (2)
	 */
	void Expansion_PlainIdToInt(out int plainId[2])
	{
		string steamId = GetPlainId();
		int idLen = steamId.Length();

		if (idLen > 18)
		{
			Error("Unsupported plain ID length " + idLen);
		}
		else
		{
			plainId[0] = steamId.Substring(0, 9).ToInt();
			plainId[1] = steamId.Substring(9, idLen - 9).ToInt();
		}
	}

	static string Expansion_PlainIdToString(int plainId[2])
	{
		return plainId[0].ToString() + plainId[1].ToString();
	}

	/**
	 * @brief Get ID as array of ints (9)
	 */
	void Expansion_IdToInt(out int id[9])
	{
		string uid = GetId();
		int idLen = uid.Length();

		if (idLen > 44)
		{
			Error("Unsupported ID length " + idLen);
		}
		else
		{
			int inputLength = idLen - 1;
			int outputLength = 9;

			for (int i = 0, j = 0; i < outputLength; i++)
			{
				int packedValue = 0;

				//! Up to five 6-bit numbers can be packed into one 32-bit signed int 
				for (int k = 0; k < 5 && j < inputLength; k++)
				{
					packedValue |= (EXPANSION_ID64_ALPHABET.IndexOf(uid[j]) & 0x3F) << (k * 6);
					j++;
				}

				id[i] = packedValue;
			}
		}
	}

	static string Expansion_IdToString(int id[9])
	{
		int inputLength = 9;
		string output;

		for (int i = 0; i < inputLength; i++)
		{
			int packedValue = id[i];

			for (int k = 0; k < 5; k++)
			{
				output += EXPANSION_ID64_ALPHABET[(packedValue >> (k * 6)) & 0x3F];

				if (i == inputLength - 1 && k == 2)
					break;
			}
		}

		return output + "=";
	}

	static string Expansion_EncodeDigest(CF_Byte digest[32])
	{
		string id;

		int i = 0;
		while (i < 32)
		{
			CF_Uint value = 0;

			value = value.Add(digest[i] << 16);
			i++;

			if (i < 32)
			{
				value = value.Add(digest[i] << 8);
				i++;
			}
			
			if (i < 32)
			{
				value = value.Add(digest[i]);
				i++;
			}

			id += EXPANSION_ID64_ALPHABET[value.ShiftRight(18) & 63];
			id += EXPANSION_ID64_ALPHABET[value.ShiftRight(12) & 63];
			id += EXPANSION_ID64_ALPHABET[value.ShiftRight(6) & 63];
			if (i < 32)
				id += EXPANSION_ID64_ALPHABET[value & 63];
		}

		return id + "=";
	}
}
