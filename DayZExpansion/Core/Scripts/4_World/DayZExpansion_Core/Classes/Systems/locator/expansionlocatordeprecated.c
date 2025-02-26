/**
 * ExpansionLocatorDeprecated.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLocatorArray
{
	vector position;
	string classname;
	string name;
	string type;
	Object object;
	int index;
	
	void ExpansionLocatorArray( vector pos, string className, string displayName, string typeName, Object obj = null, int idx = -1 ) 
	{
	}
}

class ExpansionLocatorStatic
{
	static const ref TStringArray SETTLEMENTS = {};

	static int GetRadius( string type )
	{
		DeprecationError();
		
		return 100;
	}

	static array<ref ExpansionLocatorArray> GetWorldLocations(int include = 0)
	{
		DeprecationError();

		return {};
	}

	static void DeprecationError()
	{
		string tmp;
		DumpStackString(tmp);
		TStringArray lines = {};
		tmp.Split("\n", lines);

		lines.RemoveOrdered(0);
		lines.RemoveOrdered(0);

		tmp = lines[0];
		TStringArray path = {};
		tmp.Split("/", path);

		if (ExpansionString.EndsWith(path[0], ") DayZExpansion"))
			EXError.ErrorOnce(null, "@DayZ-Expansion-" + path[1] + " is out-of-date! Please update.", lines);
		else
			EXError.Error(null, "DEPRECATED, use ExpansionLocation::GetWorldLocations", lines);
	}
}
