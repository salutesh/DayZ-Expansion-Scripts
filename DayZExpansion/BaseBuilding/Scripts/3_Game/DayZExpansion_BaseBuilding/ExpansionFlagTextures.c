/**
 * ExpansionFlagTextures.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionFlagTextures
{
	//! path -> name
	protected ref map<string, string> m_FlagTextures = new map<string, string>;

	void ExpansionFlagTextures()
	{
		Load();
	}

	array<ref ExpansionFlagTexture> GetAll()
	{
		TStringArray flagDisplayNames = {};

		//! name -> path
		map<string, string> flags = new map<string, string>;

		map<string, int> counts = new map<string, int>;

		foreach (string path, string name: m_FlagTextures)
		{
			counts[name] = counts[name] + 1;
			if (counts[name] > 1)
				name += " " + counts[name];
			flagDisplayNames.Insert(name);
			flags[name] = path;
		}

		flagDisplayNames.Sort();

		array<ref ExpansionFlagTexture> flagTextures = {};

		foreach (string displayName: flagDisplayNames)
		{
			flagTextures.Insert(new ExpansionFlagTexture(flags[displayName], displayName));
		}

		return flagTextures;
	}
	
	void Load()
	{
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);

		//! Load all flags from config.cpp

		int count = GetGame().ConfigGetChildrenCount(CFG_VEHICLESPATH);
		string childName;
		TStringArray hiddenSelectionsTextures;
		string path;
		string color;
		int index;
		string tempName;
		string displayName;

		//! name -> path
		map<string, string> flags = new map<string, string>;

		for (int i = 0; i < count; i++)
		{
			GetGame().ConfigGetChildName(CFG_VEHICLESPATH, i, childName);

			if (!GetGame().IsKindOf(childName, "Flag_Base") || GetGame().ConfigGetInt(CFG_VEHICLESPATH + " " + childName + " scope") != 2)
				continue;

			if (IsExcludedClassName(childName))
				continue;

			hiddenSelectionsTextures = {};
			GetGame().ConfigGetTextArray(CFG_VEHICLESPATH + " " + childName + " hiddenSelectionsTextures", hiddenSelectionsTextures);
			if (!hiddenSelectionsTextures.Count() || !hiddenSelectionsTextures[0])
			{
				EXPrint(this, "WARNING: Flag without hiddenSelectionTextures '" + childName + "'");
				continue;
			}

			path = hiddenSelectionsTextures[0];
			if (path[0] == "\\")
				path = path.Substring(1, path.Length() - 1);

			//! Use color as flag name, if not set, fall back to using classname
			color = "";
			GetGame().ConfigGetText(CFG_VEHICLESPATH + " " + childName + " color", color);
			if (color)
			{
				tempName = color;
			}
			else
			{
				index = childName.IndexOf("Flag_");
				if (index > -1)
					tempName = childName.Substring(index + 5, childName.Length() - index - 5);
				else
					tempName = childName;
			}

			switch (tempName)
			{
				case "BrainZ":
				case "DayZ":
				case "HunterZ":
					displayName = tempName;
					break;
				case "DayZBlack":
					displayName = "DayZ (Black)";
					break;
				case "DayZWhite":
					displayName = "DayZ (White)";
					break;
				default:
					displayName = ExpansionString.CamelCaseToWords(tempName);
					displayName.Replace("_", " ");
					break;
			}

			Add(path, displayName);
		}
	}
	
	void Add(string path, string name)
	{
		if (m_FlagTextures.Contains(path))
		{
			EXPrint(this, "WARNING: Flag already added '" + path + "'");
			return;
		}

		m_FlagTextures[path] = name;

		EXTrace.Print(EXTrace.BASEBUILDING, this, string.Format("Added flag '%1' '%2'", name, path));
	}

	bool IsExcludedClassName(string className)
	{
		switch (className)
		{
			case "Expansion_Flag_Scottish":
			case "Expansion_Flag_Swedish":
			case "Expansion_Flag_Borduriens":
			case "Expansion_Flag_Irish":
			case "Expansion_Flag_Skilanka":
			case "Expansion_Flag_Gubraltar":
			case "Expansion_Flag_Czech":
			case "Expansion_Flag_Finnish":
				return true;
		}

		return false;
	}
	
	bool Remove(string path)
	{
		if (m_FlagTextures.Contains(path))
		{
			m_FlagTextures.Remove(path);
			return true;
		}
		
		return false;
	}
}

static ref ExpansionFlagTextures g_exGlobalFlagTextures;

static ExpansionFlagTextures GetExpansionFlagTextures()
{
	if ( !g_exGlobalFlagTextures )
	{
		g_exGlobalFlagTextures = new ExpansionFlagTextures();
	}

	return g_exGlobalFlagTextures;
}
