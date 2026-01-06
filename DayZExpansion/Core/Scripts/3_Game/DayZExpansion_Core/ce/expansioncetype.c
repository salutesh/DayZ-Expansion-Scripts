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
	ref ExpansionCETypeFlags Flags;
	ref set<string> Categories = new set<string>();
	ref set<string> Usages = new set<string>();
	ref set<string> Values = new set<string>();
	
	void ExpansionCEType(string name)
	{
		Name = name;
	}

	//! See https://community.bistudio.com/wiki/DayZ:Central_Economy_mission_files_modding for file specifics
	void Merge(ExpansionCEType other)
	{
		if (other.Nominal != ExpansionCE.NOT_SET)
			Nominal = other.Nominal;

		if (other.Lifetime != ExpansionCE.NOT_SET)
			Lifetime = other.Lifetime;

		if (other.Restock != ExpansionCE.NOT_SET)
			Restock = other.Restock;

		if (other.Min != ExpansionCE.NOT_SET)
			Min = other.Min;

		if (other.QuantMin != ExpansionCE.NOT_SET)
			QuantMin = other.QuantMin;

		if (other.QuantMax != ExpansionCE.NOT_SET)
			QuantMax = other.QuantMax;

		if (other.Cost != ExpansionCE.NOT_SET)
			Cost = other.Cost;

		if (other.Flags)
		{
			if (!Flags)
				Flags = new ExpansionCETypeFlags();

			Flags.Merge(other.Flags);
		}

		if (other.Categories)
			Categories.Copy(other.Categories);

		if (other.Usages)
			Usages.Copy(other.Usages);

		if (other.Values)
			Values.Copy(other.Values);
	}
}

class ExpansionCETypeFlags
{
	int CountInCargo;
	int CountInHoarder;
	int CountInMap;
	int CountInPlayer;
	int Crafted;
	int DELoot;

	void Merge(ExpansionCETypeFlags other)
	{
		if (other.CountInCargo != ExpansionCE.NOT_SET)
			CountInCargo = other.CountInCargo;

		if (other.CountInHoarder != ExpansionCE.NOT_SET)
			CountInHoarder = other.CountInHoarder;

		if (other.CountInMap != ExpansionCE.NOT_SET)
			CountInMap = other.CountInMap;

		if (other.CountInPlayer != ExpansionCE.NOT_SET)
			CountInPlayer = other.CountInPlayer;

		if (other.Crafted != ExpansionCE.NOT_SET)
			Crafted = other.Crafted;

		if (other.DELoot != ExpansionCE.NOT_SET)
			DELoot = other.DELoot;
	}
}

class ExpansionCETypes: map<string, ref ExpansionCEType>
{
	static ExpansionCETypes LoadTypes(string fileName)
	{
		ExpansionCETypes types = new ExpansionCETypes;
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

			auto types = root.GetContent().GetTags();
			foreach (CF_XML_Tag type: types)
			{
				string tagName = type.GetName();
				if (tagName != "type")
					continue;

				string name = ExpansionXML.GetAttributeString(type, "name");

				string key = name;
				key.ToLower();

				int defaultValue = 0;
				int defaultQuantity = -1;
				int defaultCost = 100;

				if (Contains(key))
				{
					defaultValue = ExpansionCE.NOT_SET;
					defaultQuantity = ExpansionCE.NOT_SET;
					defaultCost = ExpansionCE.NOT_SET;
				}

				auto ceType = new ExpansionCEType(name);

				ceType.Nominal = ExpansionXML.GetTagContentInt(type, "nominal", defaultValue);
				ceType.Lifetime = ExpansionXML.GetTagContentInt(type, "lifetime", defaultValue);
				ceType.Restock = ExpansionXML.GetTagContentInt(type, "restock", defaultValue);
				ceType.Min = ExpansionXML.GetTagContentInt(type, "min", defaultValue);
				ceType.QuantMin = ExpansionXML.GetTagContentInt(type, "quantmin", defaultQuantity);
				ceType.QuantMax = ExpansionXML.GetTagContentInt(type, "quantmax", defaultQuantity);
				ceType.Cost = ExpansionXML.GetTagContentInt(type, "cost", defaultCost);

				//! Flags
				auto flags = type.GetTag("flags")[0];
				if (flags)
				{
					ceType.Flags = new ExpansionCETypeFlags();
					ceType.Flags.CountInCargo = ExpansionXML.GetAttributeInt(flags, "count_in_cargo", defaultValue);
					ceType.Flags.CountInHoarder = ExpansionXML.GetAttributeInt(flags, "count_in_hoarder", defaultValue);
					ceType.Flags.CountInMap = ExpansionXML.GetAttributeInt(flags, "count_in_map", defaultValue);
					ceType.Flags.CountInPlayer = ExpansionXML.GetAttributeInt(flags, "count_in_player", defaultValue);
					ceType.Flags.Crafted = ExpansionXML.GetAttributeInt(flags, "crafted", defaultValue);
					ceType.Flags.DELoot = ExpansionXML.GetAttributeInt(flags, "deloot", defaultValue);
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

				Merge(key, ceType);
			}
		}
	}

	void Merge(string key, ExpansionCEType ceType)
	{
		ExpansionCEType existing;
		if (Find(key, existing))
			existing.Merge(ceType);
		else
			Insert(key, ceType);
	}
}
