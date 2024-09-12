/**
 * ExpansionQuestObjectiveAIPatrolConfig.c
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
class ExpansionQuestObjectiveAIPatrolConfig_V25: ExpansionQuestObjectiveConfig
{
	autoptr ExpansionQuestObjectiveAIPatrol AIPatrol;
	float MinDistRadius;
	float MaxDistRadius;
	float DespawnRadius;
	bool CanLootAI;
};

class ExpansionQuestObjectiveAIPatrolConfigBase: ExpansionQuestObjectiveConfig
{
	float MaxDistance = -1;
	float MinDistance = -1;
	autoptr TStringArray AllowedWeapons = new TStringArray;
	autoptr TStringArray AllowedDamageZones = new TStringArray;
};

class ExpansionQuestObjectiveAIPatrolConfig_V27: ExpansionQuestObjectiveConfig
{
	autoptr ExpansionQuestAISpawn_V27 AISpawn;
};

class ExpansionQuestObjectiveAIPatrolConfig: ExpansionQuestObjectiveAIPatrolConfigBase
{
	autoptr ExpansionQuestAISpawn AISpawn;

	void SetAISpawn(ExpansionQuestAISpawn aiSpawn)
	{
		AISpawn = aiSpawn;
	}

	ExpansionQuestAISpawn GetAISpawn()
	{
		return AISpawn;
	}
	
	void AddAllowedWeapon(string name)
	{
		AllowedWeapons.Insert(name);
	}

	TStringArray GetAllowedWeapons()
	{
		return AllowedWeapons;
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

	static ExpansionQuestObjectiveAIPatrolConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveAIPatrolConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName);

		ExpansionQuestObjectiveAIPatrolConfig config;
		ExpansionQuestObjectiveAIPatrolConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveAIPatrolConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName + " to version " + CONFIGVERSION);
			if (!config)
				config = new ExpansionQuestObjectiveAIPatrolConfig();
			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			if (configBase.ConfigVersion < 26)
			{
				ExpansionQuestObjectiveAIPatrolConfig_V25 configV25;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfig_V25>.Load(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName, configV25))
					return NULL;

				if (configV25.AIPatrol)
				{
					config.AllowedWeapons = configV25.AIPatrol.AllowedWeapons;

					ExpansionQuestAISpawn aiSpawn = new ExpansionQuestAISpawn();
					aiSpawn.SetNumberOfAI(configV25.AIPatrol.NPCUnits);
					aiSpawn.SetName("Quest Target");
					aiSpawn.SetWaypoints(configV25.AIPatrol.Waypoints);
					aiSpawn.SetLoadout(configV25.AIPatrol.NPCLoadoutFile);
					aiSpawn.SetFaction(configV25.AIPatrol.NPCFaction);
					aiSpawn.SetBehaviour(eAIWaypointBehavior.HALT);
					aiSpawn.SetFormation(configV25.AIPatrol.NPCFormation);
					aiSpawn.SetSpeed(1.0);
					aiSpawn.SetThreatSpeed(3.0);
					aiSpawn.SetMinAccuracy(configV25.AIPatrol.NPCAccuracyMin);
					aiSpawn.SetMaxAccuracy(configV25.AIPatrol.NPCAccuracyMax);
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

					config.SetAISpawn(aiSpawn);
				}
			}
			else if (configBase.ConfigVersion < 28)
			{
				ExpansionQuestObjectiveAIPatrolConfig_V27 configV27;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfig_V27>.Load(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName, configV27))
					return NULL;

				if (configV27.AISpawn)
				{
					ExpansionQuestAISpawn aiSpawnV28 = new ExpansionQuestAISpawn();
					aiSpawnV28.CopyFrom(configV27.AISpawn);
					config.SetAISpawn(aiSpawnV28);
				}
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName, config))
		{
			return NULL;
		}

		string removeExt = ExpansionString.StripExtension(config.AISpawn.GetLoadout(), ".json");
		if (removeExt != config.AISpawn.GetLoadout())
		{
			config.AISpawn.SetLoadout(removeExt);
			save = true;
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveAIPatrolConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;

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