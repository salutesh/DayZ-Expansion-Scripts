/**
 * ExpansionItemNameTabel.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionItemNameTabel
{
	static ref map<string, ref array<string>> s_NameTabel = new map<string, ref array<string>>;
	static void LoadNameTabel()
	{
		array<string> configClasses = {CFG_WEAPONSPATH, CFG_MAGAZINESPATH, CFG_VEHICLESPATH};
		foreach(string configClass: configClasses)
		{
			int childCount = GetGame().ConfigGetChildrenCount(configClass);
			for (int i = 0; i < childCount; i++)
			{
				string childName;
				GetGame().ConfigGetChildName(configClass, i, childName);
				string displayName;
				GetGame().ConfigGetText(configClass + " " + childName + " displayName", displayName);
				displayName.ToLower();
				childName.ToLower();
				
				if (displayName == "")
					continue;
				
				array<string> currentTypes;
				if (!s_NameTabel.Find(displayName, currentTypes))
					currentTypes = new array<string>;
				
				if (currentTypes.Find(childName) == -1)
					currentTypes.Insert(childName);
				
				s_NameTabel.Set(displayName, currentTypes);
			}
		}
		
		#ifdef DIAG_DEVELOPER
		foreach(string dN, array<string> tN: s_NameTabel)
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
		array<string> validTypeNames = new array<string>;
		
		string lowerName = displayName;
		lowerName.ToLower();
		TStringArray tokens = new TStringArray;
		lowerName.Split(" ", tokens);
		
		set<string> dST = new set<string>;
		foreach (string sM: tokens)
		{
			dST.Insert(sM);
		}
		
		foreach(string dN, array<string> tN: s_NameTabel)
		{
			#ifdef DIAG_DEVELOPER
			ErrorEx("Display name: " + dN + " | Types count: " + tN.Count(), ErrorExSeverity.INFO);
			#endif
			
			TStringArray dNT = new TStringArray;
			if (dN.Contains(" "))
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
					if (nW.IndexOf(dW) != -1 || nW.Contains(dW))
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