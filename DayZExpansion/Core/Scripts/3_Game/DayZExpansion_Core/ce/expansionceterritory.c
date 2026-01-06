/**
 * ExpansionCETerritory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCETerritory
 * @brief		structure for mission\env xmls
 **/

class ExpansionCETerritory 
{
	string Color;
	ref array<ref ExpansionCETerritoryZone> Zones;

	void ExpansionCETerritory(string color)
	{
		Color = color;
		Zones = {};
	}
}

class ExpansionCETerritoryZone 
{
	string Name;
	int StaticMin;
	int StaticMax;
	int DynamicMin;
	int DynamicMax;
	float X;
	float Z;
	float Radius;

	void ExpansionCETerritoryZone(string name, int sMin, int sMax, int dMin, int dMax, float x, float z, float radius)
	{
		Name = name;
		StaticMin = sMin;
		StaticMax = sMax;
		DynamicMin = dMin;
		DynamicMax = dMax;
		X = x;
		Z = z;
		Radius = radius;
	}
}

class ExpansionCETerritories: array<ref ExpansionCETerritory>
{
	static ExpansionCETerritories LoadTerritories(string fileName)
	{
		ExpansionCETerritories territories = {};
		territories.ReadTerritories(fileName);
		return territories;
	}

	void ReadTerritories(string fileName)
	{
		CF_XML_Document document = CF_XML.ReadDocumentEx(fileName);
		if (document)
		{
			auto root = document.Get("territory-type")[0];
			if (!root)
			{
				CF.FormatError("No root tag 'territory-type' in '%1'", fileName);
				return;
			}

			auto territories = root.GetContent().GetTags();
			foreach (CF_XML_Tag territory: territories)
			{
				string tagName = territory.GetName();
				if (tagName != "territory")
					continue;

				string color = ExpansionXML.GetAttributeString(territory, "color");

				auto ceTerritory = new ExpansionCETerritory(color);

				auto zones = territory.GetTag("zone");
				foreach (CF_XML_Tag zone: zones)
				{
					string name = ExpansionXML.GetAttributeString(zone, "name");
					int sMin = ExpansionXML.GetAttributeInt(zone, "smin");
					int sMax = ExpansionXML.GetAttributeInt(zone, "smax");
					int dMin = ExpansionXML.GetAttributeInt(zone, "dmin");
					int dMax = ExpansionXML.GetAttributeInt(zone, "dmax");
					float x = ExpansionXML.GetAttributeFloat(zone, "x");
					float z = ExpansionXML.GetAttributeFloat(zone, "z");
					float radius = ExpansionXML.GetAttributeFloat(zone, "r");

					ceTerritory.Zones.Insert(new ExpansionCETerritoryZone(name, sMin, sMax, dMin, dMax, x, z, radius));
				}

				Insert(ceTerritory);
			}
		}
	}
}
