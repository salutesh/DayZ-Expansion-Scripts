/**
 * ExpansionItemNameTable.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionItemNameTable
{
	static ref map<string, ref array<string>> s_NameTable = new map<string, ref array<string>>;
	static ref map<int, ref map<int, string>> s_HashTable = new map<int, ref map<int, string>>;
	static ref set<string> s_Collisions = new set<string>;

	static void LoadNameTable()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, ExpansionItemNameTable);
		#endif

		if (s_NameTable.Count() > 0)
			return;

		array<string> configClasses = {CFG_WEAPONSPATH, CFG_MAGAZINESPATH, CFG_VEHICLESPATH};
		foreach(string configClass: configClasses)
		{
			int childCount = g_Game.ConfigGetChildrenCount(configClass);
			for (int i = 0; i < childCount; i++)
			{
				string childName;
				g_Game.ConfigGetChildName(configClass, i, childName);
				
				int scope = g_Game.ConfigGetInt(configClass + " " + childName + " scope");
				if (scope != 2)
					continue;

				//! --------------------------------------------

				//! Item name table - maps display name to types

				string displayName;
				g_Game.ConfigGetText(configClass + " " + childName + " displayName", displayName);
				string childNameLower = childName;
				childNameLower.ToLower();
				
				if (displayName != "")
				{
					displayName.ToLower();

					array<string> currentTypes;
					if (!s_NameTable.Find(displayName, currentTypes))
					{
						currentTypes = new array<string>;
						s_NameTable[displayName] = currentTypes;
					}
					
					if (currentTypes.Find(childNameLower) == -1)
						currentTypes.Insert(childNameLower);
				}

				//! --------------------------------------------

				//! Hash table - maps hash to type. We use a combined 16 bit + 8 bit hash for optimized network efficiency

				int hashA;
				int hashB;
				Hash(childNameLower, hashA, hashB);
				
				map<int, string> hashTable;
				if (!s_HashTable.Find(hashA, hashTable))
				{
					hashTable = new map<int, string>;
					s_HashTable[hashA] = hashTable;
				}

				if (hashTable.Contains(hashB))
				{
					#ifdef DIAG_DEVELOPER
					EXError.Info(null, "Hash collision: " + childName + " collides with " + hashTable[hashB], {});
					#endif
					s_Collisions.Insert(childNameLower);
				}
				else
				{
					hashTable[hashB] = childName;
				}
			}
		}
		
		#ifdef EXTRACE
		trace = null;
		#endif

		#ifdef DIAG_DEVELOPER
		foreach(string dN, array<string> tN: s_NameTable)
		{
			ErrorEx("Display name:" + dN + " | Types count: " + tN.Count(), ErrorExSeverity.INFO);
			foreach(string t: tN)
			{
				ErrorEx("----- Type:" + t, ErrorExSeverity.INFO);
			}
		}
		#endif
	}

	static void Hash(string type, out int hashA, out int hashB)
	{
		ExpansionString tmp = type;
		hashA = tmp.DJB2Hash() & 0xffff;
		hashB = tmp.JenkinsHash() & 0xff;
	}

	static void WriteHash(ParamsWriteContext ctx, int hashA, int hashB)
	{
		int hash = hashA;
		hash |= hashB << 16;
		ctx.Write(hash);
	}

	static bool ReadHash(ParamsReadContext ctx, out int hashA, out int hashB)
	{
		int hash;
		if (!ctx.Read(hash))
			return false;

		hashA = hash & 0x0000ffff;

		int shift = 16;
		hashB = ExpansionBitStream.Unpack(hash, 8, shift);

		return true;
	}

	static void WriteHash(ExpansionBitStreamWriter writer, int hashA, int hashB)
	{
		writer.WriteUInt(hashA, 16);
		writer.WriteUInt(hashB, 8);
	}

	static bool ReadHash(ExpansionBitStreamReader reader, out int hashA, out int hashB)
	{
		if (!reader.ReadUInt(hashA, 16))
			return false;

		if (!reader.ReadUInt(hashB, 8))
			return false;

		return true;
	}

	static bool IsHashColliding(string type)
	{
		type.ToLower();
		if (s_Collisions.Find(type) > -1)
			return true;
		return false;
	}

	static string GetTypeByHash(int hashA, int hashB)
	{
		map<int, string> hashTable;
		if (s_HashTable.Find(hashA, hashTable))
			return hashTable[hashB];

		return "";
	}

	static string GetTypeLowerByHash(int hashA, int hashB)
	{
		string type = GetTypeByHash(hashA, hashB);
		type.ToLower();
		return type;
	}
	
	static array<string> GetTypeNamesByString(string displayName)
	{
		TStringArray validTypeNames = {};
		string lowerName = displayName;
		lowerName.ToLower();
		TStringArray tokens = {};
		lowerName.Split(" ", tokens);
		
		set<string> dST = new set<string>;
		foreach (string sM: tokens)
		{
			dST.Insert(sM);
		}
		
		foreach(string dN, array<string> tN: s_NameTable)
		{
			#ifdef DIAG_DEVELOPER
			ErrorEx("Display name: " + dN + " | Types count: " + tN.Count(), ErrorExSeverity.INFO);
			#endif
			
			TStringArray dNT = {};
			if (dN.IndexOf(" ") > -1)
				dN.Split(" ", dNT);
			else
				dNT.Insert(dN);
			
			set<string> dNST = new set<string>;
			foreach (string sT: dNT)
			{
				dNST.Insert(sT);
			}
			
			foreach(string dW: dST) //! Display name tokens
			{
				foreach(string nW: dNST) //! Type name tokens
				{
					#ifdef DIAG_DEVELOPER
					ErrorEx("Compare [" + nW + "] with [" + dW + "]", ErrorExSeverity.INFO);
					#endif
					if (nW.IndexOf(dW) != -1)
					{
						#ifdef DIAG_DEVELOPER
						ErrorEx("Compare [" + nW + "] with [" + dW + "] - LEGIT", ErrorExSeverity.INFO);
						#endif
						foreach(string t: tN)
						{
							if (validTypeNames.Find(t) == -1)
								validTypeNames.Insert(t);
						}
					}
					#ifdef DIAG_DEVELOPER
					else
					{
						ErrorEx("Compare [" + nW + "] with [" + dW + "] - SKIP", ErrorExSeverity.INFO);
					}
					#endif
				}
			}
		}
		
		return validTypeNames;
	}
}