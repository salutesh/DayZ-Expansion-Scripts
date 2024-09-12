/**
 * ExpansionQuestObjectiveAICampConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAICampConfig_V25: ExpansionQuestObjectiveConfig
{
	autoptr ExpansionQuestObjectiveAICamp AICamp;
	float MinDistRadius;
	float MaxDistRadius;
	float DespawnRadius;
	bool CanLootAI;
	int InfectedDeletionRadius;
};

class ExpansionQuestObjectiveAICampConfigBase: ExpansionQuestObjectiveConfig
{
	float InfectedDeletionRadius = 500.0;
	float MaxDistance = -1;
	float MinDistance = -1;
	autoptr TStringArray AllowedWeapons = new TStringArray;
	autoptr TStringArray AllowedDamageZones = new TStringArray;
};

class ExpansionQuestObjectiveAICampConfig_V27: ExpansionQuestObjectiveAICampConfigBase
{
	autoptr array<ref ExpansionQuestAISpawn_V27> AISpawns = new array<ref ExpansionQuestAISpawn_V27>;
};

class ExpansionQuestObjectiveAICampConfig: ExpansionQuestObjectiveAICampConfigBase
{
	autoptr array<ref ExpansionQuestAISpawn> AISpawns = new array<ref ExpansionQuestAISpawn>;

	void SetAISpawns(array<ref ExpansionQuestAISpawn> aiSpawns)
	{
		if (!AISpawns)
			AISpawns = new array<ref ExpansionQuestAISpawn>;

		AISpawns = aiSpawns;
	}

	void AddAISpawn(ExpansionQuestAISpawn aiSpawn)
	{
		if (!AISpawns)
			AISpawns = new array<ref ExpansionQuestAISpawn>;

		AISpawns.Insert(aiSpawn);
	}

	array<ref ExpansionQuestAISpawn> GetAISpawns()
	{
		return AISpawns;
	}
	
	void AddAllowedWeapon(string name)
	{
		AllowedWeapons.Insert(name);
	}

	TStringArray GetAllowedWeapons()
	{
		return AllowedWeapons;
	}
	
	void SetInfectedDeletionRadius(float radius)
	{
		InfectedDeletionRadius = radius;
	}

	float GetInfectedDeletionRadius()
	{
		return InfectedDeletionRadius;
	}

	void SetMaxDistance(float max)
	{
		MaxDistance = max;
	}

	float GetMaxDistance()
	{
		return MaxDistance;
	}

	void SetMinDistance(float max)
	{
		MinDistance = max;
	}

	float GetMinDistance()
	{
		return MinDistance;
	}
	
	void AddAllowedDamageZone(string zone)
	{
		if (!AllowedDamageZones.Find(zone))
			AllowedDamageZones.Insert(zone);
	}

	TStringArray GetAllowedDamageZones()
	{
		return AllowedDamageZones;
	}

	static ExpansionQuestObjectiveAICampConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveAICampConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName);

		ExpansionQuestObjectiveAICampConfig config;
		ExpansionQuestObjectiveAICampConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveAICampConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName + " to version " + CONFIGVERSION);
			if (!config)
				config = new ExpansionQuestObjectiveAICampConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			if (configBase.ConfigVersion < 26)
			{
				ExpansionQuestObjectiveAICampConfig_V25 configV25;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfig_V25>.Load(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName, configV25))
					return NULL;

				config.InfectedDeletionRadius = configV25.InfectedDeletionRadius;

				if (configV25.AICamp)
				{
					config.AllowedWeapons = configV25.AICamp.AllowedWeapons;

					foreach (vector pos: configV25.AICamp.Positions)
					{
						ExpansionQuestAISpawn aiSpawn = new ExpansionQuestAISpawn();
						aiSpawn.SetNumberOfAI(1);
						aiSpawn.SetName("Quest Target");
						aiSpawn.AddWaypoint(pos);
						aiSpawn.SetLoadout(configV25.AICamp.NPCLoadoutFile);
						aiSpawn.SetFaction(configV25.AICamp.NPCFaction);
						aiSpawn.SetBehaviour(eAIWaypointBehavior.HALT);
						aiSpawn.SetFormation("RANDOM");
						aiSpawn.SetSpeed(0);
						aiSpawn.SetThreatSpeed(3.0);
						aiSpawn.SetMinAccuracy(configV25.AICamp.NPCAccuracyMin);
						aiSpawn.SetMaxAccuracy(configV25.AICamp.NPCAccuracyMax);
						aiSpawn.SetCanBeLooted(configV25.CanLootAI);
						aiSpawn.SetUnlimitedReload(true);
						aiSpawn.SetThreatDistanceLimit(1000.0);
						aiSpawn.SetDamageMultiplier(1.0);
						aiSpawn.SetDamageReceivedMultiplier(1.0);
						aiSpawn.SetClassNames(ExpansionQuestAISpawn.eAI_UNITS);
						aiSpawn.SetSniperProneDistanceThreshold(300.0);

						aiSpawn.SetRespawnTime(1.0);
						aiSpawn.SetDespawnTime(1.0);
						aiSpawn.SetMinDistanceRadius(configV25.MinDistRadius);
						aiSpawn.SetMaxDistanceRadius(configV25.MaxDistRadius);
						aiSpawn.SetDespawnRadius(configV25.DespawnRadius);

						config.AddAISpawn(aiSpawn);
					}
				}
			}
			else if (configBase.ConfigVersion < 28)
			{
				ExpansionQuestObjectiveAICampConfig_V27 configV27;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfig_V27>.Load(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName, configV27))
					return NULL;

				foreach (auto aiSpawnV27: configV27.AISpawns)
				{
					ExpansionQuestAISpawn aiSpawnV28 = new ExpansionQuestAISpawn();
					aiSpawnV28.CopyFrom(aiSpawnV27);
					config.AddAISpawn(aiSpawnV28);
				}
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName, config))
		{
			return NULL;
		}

		foreach (ExpansionQuestAISpawn spawn: config.AISpawns)
		{
			string removeExt = ExpansionString.StripExtension(spawn.GetLoadout(), ".json");
			if (removeExt != spawn.GetLoadout())
			{
				spawn.SetLoadout(removeExt);
				save = true;
			}
			
			//! Make sure number of AI for each AI spawn is set to 1.
			int numOfAI = spawn.GetNumberOfAI();
			if (numOfAI <= 0 || numOfAI > 1)
			{
				spawn.SetNumberOfAI(1);
				save = true;
			}
		}

		if (save)
		{
			config.Save(fileName);
		}

		return config;
	}
	
	override void Save(string fileName)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveAICampConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		
		InfectedDeletionRadius = configBase.InfectedDeletionRadius;
		MaxDistance = configBase.MaxDistance;
		MinDistance = configBase.MinDistance;
		AllowedWeapons.Copy(configBase.AllowedWeapons);
		AllowedDamageZones.Copy(configBase.AllowedDamageZones);
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		return true;
	}
};
#endif