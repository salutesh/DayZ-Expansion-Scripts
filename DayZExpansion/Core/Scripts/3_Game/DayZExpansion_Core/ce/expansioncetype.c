/**
 * ExpansionCEType.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCEType
{
	string Name;
	int Nominal;
	int Lifetime;
	int Restock;
	int Min;
	int QuantMin;
	int QuantMax;
	int Cost;
	ref ExpansionCETypeFlags Flags = new ExpansionCETypeFlags();
	ref set<string> Categories = new set<string>();
	ref set<string> Usages = new set<string>();
	ref set<string> Values = new set<string>();
	
	void ExpansionCEType(string name, int nominal = -1, int lifetime = -1, int restock = -1, int min = -1, int quantMin = -1, int quantMax = -1, int cost = -1)
	{
		Name = name;
		Nominal = nominal;
		Lifetime = lifetime;
		restock = restock;
		Min = min;
		QuantMin = quantMin;
		QuantMax = quantMax;
		Cost = cost;
	}

	void Merge(ExpansionCEType other)
	{
		if (other.Nominal != -1)
			Nominal = other.Nominal;

		if (other.Lifetime != -1)
			Lifetime = other.Lifetime;

		if (other.Restock != -1)
			Restock = other.Restock;

		if (other.Min != -1)
			Min = other.Min;

		if (other.QuantMin != -1)
			QuantMin = other.QuantMin;

		if (other.QuantMax != -1)
			QuantMax = other.QuantMax;

		if (other.Cost != -1)
			Cost = other.Cost;

		Categories.InsertSet(other.Categories);
		Usages.InsertSet(other.Usages);
		Values.InsertSet(other.Values);
	}
}

class ExpansionCETypeFlags
{
	bool CountInCargo;
	bool CountInHoarder;
	bool CountInMap;
	bool CountInPlayer;
	bool Crafted;
	bool DELoot;
}

class ExpansionCETypes: array<ref ExpansionCEType>
{
	static ExpansionCETypes LoadTypes(string fileName)
	{
		ExpansionCETypes types = {};
		types.ReadTypes(fileName);
		return types;
	}

	void ReadTypes(string fileName)
	{
		CF_XML_Document document = CF_XML.ReadDocumentEx(fileName);
		if (document)
		{
			auto root = document.Get("types")[0];
			if (!root)
			{
				CF.FormatError("No root tag 'types' in '%1'", fileName);
				return;
			}

			auto types = root.GetTag("type");
			foreach (CF_XML_Tag type: types)
			{
				string name = ExpansionXML.GetAttributeString(type, "name");
				int nominal = ExpansionXML.GetTagContentInt(type, "nominal", -1);
				int lifetime = ExpansionXML.GetTagContentInt(type, "lifetime", -1);
				int restock = ExpansionXML.GetTagContentInt(type, "restock", -1);
				int min = ExpansionXML.GetTagContentInt(type, "min", -1);
				int quantmin = ExpansionXML.GetTagContentInt(type, "quantmin", -1);
				int quantmax = ExpansionXML.GetTagContentInt(type, "quantmax", -1);
				int cost = ExpansionXML.GetTagContentInt(type, "cost", -1);

				auto ceType = new ExpansionCEType(name, nominal, lifetime, restock, min, quantmin, quantmax, cost);

				//! Flags
				auto flags = type.GetTag("flags")[0];
				if (flags)
				{
					ceType.Flags.CountInCargo = ExpansionXML.GetAttributeBool(flags, "count_in_cargo", ceType.Flags.CountInCargo);
					ceType.Flags.CountInHoarder = ExpansionXML.GetAttributeBool(flags, "count_in_hoarder", ceType.Flags.CountInHoarder);
					ceType.Flags.CountInMap = ExpansionXML.GetAttributeBool(flags, "count_in_map", ceType.Flags.CountInMap);
					ceType.Flags.CountInPlayer = ExpansionXML.GetAttributeBool(flags, "count_in_player", ceType.Flags.CountInPlayer);
					ceType.Flags.Crafted = ExpansionXML.GetAttributeBool(flags, "crafted", ceType.Flags.Crafted);
					ceType.Flags.DELoot = ExpansionXML.GetAttributeBool(flags, "deloot", ceType.Flags.DELoot);
				}

				//! Categories
				auto categories = type.GetTag("category");
				foreach (CF_XML_Tag category: categories)
				{
					ceType.Categories.Insert(ExpansionXML.GetAttributeString(category, "name"));
				}

				//! Usage
				auto usages = type.GetTag("usage");
				foreach (CF_XML_Tag usage: usages)
				{
					ceType.Usages.Insert(ExpansionXML.GetAttributeString(usage, "name"));
				}

				//! Values
				auto values = type.GetTag("value");
				foreach (CF_XML_Tag value: values)
				{
					ceType.Values.Insert(ExpansionXML.GetAttributeString(value, "name"));
				}

				Insert(ceType);
			}
		}
	}
}
