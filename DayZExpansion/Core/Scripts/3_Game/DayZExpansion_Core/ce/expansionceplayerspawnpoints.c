/**
 * ExpansionCEPlayerSpawnpoints.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionCEPlayerSpawnpointsParams
{
	int MinDistInfected;
	int MaxDistInfected;
	int MinDistPlayer;
	int MaxDistPlayer;
	int MinDistStatic;
	int MaxDistStatic;
	int MinDistTrigger;
	int MaxDistTrigger;
}

class ExpansionCEPlayerSpawnpointsGeneratorParams
{
	int GridDensity;
	int GridWidth;
	int GridHeight;
	int MinDistStatic;
	int MaxDistStatic;
	int MinSteepness;
	int MaxSteepness;
}

class ExpansionCEPlayerSpawnpointsGroupParams
{
	bool EnableGroups;
	bool GroupsAsRegular;
	int Lifetime;
	int Counter;
}

class ExpansionCEPlayerSpawnpointsGroup
{
	string Name;
	ref TVectorArray Positions = {};

	void ExpansionCEPlayerSpawnpointsGroup(string name)
	{
		Name = name;
	}
}

class ExpansionCEPlayerSpawnpointsSettings
{
	ref ExpansionCEPlayerSpawnpointsParams Params;
	ref ExpansionCEPlayerSpawnpointsGeneratorParams GeneratorParams;
	ref ExpansionCEPlayerSpawnpointsGroupParams GroupParams;
	ref array<ref ExpansionCEPlayerSpawnpointsGroup> Groups = {};
}

class ExpansionCEPlayerSpawnpoints
{
	ref ExpansionCEPlayerSpawnpointsSettings Fresh;
	ref ExpansionCEPlayerSpawnpointsSettings Hop;
	ref ExpansionCEPlayerSpawnpointsSettings Travel;

	[NonSerialized()]
	protected bool m_IsLoaded;

	void Load(string fileName)
	{
		CF_XML_Document document = CF_XML.ReadDocumentEx(fileName);
		if (document)
		{
			auto root = document.Get("playerspawnpoints")[0];
			if (!root)
			{
				CF.FormatError("No root tag 'playerspawnpoints' in '%1'", fileName);
				return;
			}

			auto tags = root.GetContent().GetTags();
			foreach (CF_XML_Tag tag: tags)
			{
				auto settings = new ExpansionCEPlayerSpawnpointsSettings();

				string tagName = tag.GetName();
				switch (tagName)
				{
					case "fresh":
						Fresh = settings;
						break;

					case "hop":
						Hop = settings;
						break;

					case "travel":
						Travel = settings;
						break;

					default:
						continue;
				}

				auto spawnParams = tag.GetTag("spawn_params")[0];
				if (spawnParams)
				{
					settings.Params = new ExpansionCEPlayerSpawnpointsParams();
					settings.Params.MinDistInfected = ExpansionXML.GetTagContentInt(spawnParams, "min_dist_infected");
					settings.Params.MaxDistInfected = ExpansionXML.GetTagContentInt(spawnParams, "max_dist_infected");
					settings.Params.MinDistPlayer = ExpansionXML.GetTagContentInt(spawnParams, "min_dist_player");
					settings.Params.MaxDistPlayer = ExpansionXML.GetTagContentInt(spawnParams, "max_dist_player");
					settings.Params.MinDistStatic = ExpansionXML.GetTagContentInt(spawnParams, "min_dist_static");
					settings.Params.MaxDistStatic = ExpansionXML.GetTagContentInt(spawnParams, "max_dist_static");
					settings.Params.MinDistTrigger = ExpansionXML.GetTagContentInt(spawnParams, "min_dist_trigger");
					settings.Params.MaxDistTrigger = ExpansionXML.GetTagContentInt(spawnParams, "max_dist_trigger");
				}

				auto generatorParams = tag.GetTag("generator_params")[0];
				if (generatorParams)
				{
					settings.GeneratorParams = new ExpansionCEPlayerSpawnpointsGeneratorParams();
					settings.GeneratorParams.GridDensity = ExpansionXML.GetTagContentInt(generatorParams, "grid_density");
					settings.GeneratorParams.GridWidth = ExpansionXML.GetTagContentInt(generatorParams, "grid_width");
					settings.GeneratorParams.GridHeight = ExpansionXML.GetTagContentInt(generatorParams, "grid_height");
					settings.GeneratorParams.MinDistStatic = ExpansionXML.GetTagContentInt(generatorParams, "min_dist_static");
					settings.GeneratorParams.MaxDistStatic = ExpansionXML.GetTagContentInt(generatorParams, "max_dist_static");
					settings.GeneratorParams.MinSteepness = ExpansionXML.GetTagContentInt(generatorParams, "min_steepness");
					settings.GeneratorParams.MaxSteepness = ExpansionXML.GetTagContentInt(generatorParams, "max_steepness");
				}

				auto groupParams = tag.GetTag("group_params")[0];
				if (groupParams)
				{
					settings.GroupParams = new ExpansionCEPlayerSpawnpointsGroupParams();
					settings.GroupParams.EnableGroups = ExpansionXML.GetTagContentInt(groupParams, "enablegroups");
					settings.GroupParams.GroupsAsRegular = ExpansionXML.GetTagContentInt(groupParams, "groups_as_regular");
					settings.GroupParams.Lifetime = ExpansionXML.GetTagContentInt(groupParams, "lifetime");
					settings.GroupParams.Counter = ExpansionXML.GetTagContentInt(groupParams, "counter");
				}

				auto posbubbles = tag.GetTag("generator_posbubbles")[0];
				if (posbubbles)
				{
					auto groups = posbubbles.GetContent().GetTags();
					foreach (CF_XML_Tag groupTag: groups)
					{
						string groupName = ExpansionXML.GetAttributeString(groupTag, "name");
						auto group = new ExpansionCEPlayerSpawnpointsGroup(groupName);

						auto positions = groupTag.GetTag("pos");
						foreach (CF_XML_Tag pos: positions)
						{
							float x = ExpansionXML.GetAttributeFloat(pos, "x");
							float y = ExpansionXML.GetAttributeFloat(pos, "y");  //! Unused
							float z = ExpansionXML.GetAttributeFloat(pos, "z");
							group.Positions.Insert(Vector(x, y, z));
						}

						settings.Groups.Insert(group);
					}
				}
			}
		}

		m_IsLoaded = true;
	}

	bool IsLoaded()
	{
		return m_IsLoaded;
	}
}
