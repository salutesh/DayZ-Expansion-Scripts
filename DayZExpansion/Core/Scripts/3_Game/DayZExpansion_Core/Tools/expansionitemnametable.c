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

	static void LoadNameTable()
	{
		if (s_NameTable.Count() > 0)
			return;

		array<string> configClasses = {CFG_WEAPONSPATH, CFG_MAGAZINESPATH, CFG_VEHICLESPATH};
		foreach(string configClass: configClasses)
		{
			int childCount = GetGame().ConfigGetChildrenCount(configClass);
			for (int i = 0; i < childCount; i++)
			{
				string childName;
				GetGame().ConfigGetChildName(configClass, i, childName);
				
				int scope = GetGame().ConfigGetInt(configClass + " " + childName + " scope");
				if (scope != 2)
					continue;
				
				string displayName;
				GetGame().ConfigGetText(configClass + " " + childName + " displayName", displayName);
				displayName.ToLower();
				childName.ToLower();
				
				if (displayName == "")
					continue;
				
				array<string> currentTypes;
				if (!s_NameTable.Find(displayName, currentTypes))
				{
					currentTypes = new array<string>;
					s_NameTable[displayName] = currentTypes;
				}
				
				if (currentTypes.Find(childName) == -1)
					currentTypes.Insert(childName);
			}
		}
		
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