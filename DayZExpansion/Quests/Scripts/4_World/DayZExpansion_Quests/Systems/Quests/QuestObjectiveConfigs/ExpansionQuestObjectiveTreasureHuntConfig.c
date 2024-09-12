/**
 * ExpansionQuestObjectiveTreasureHuntConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! DEPRICATED - Only still here for conversion reasons.
class ExpansionQuestObjectiveTreasureHuntConfig_V17: ExpansionQuestObjectiveTreasureHuntConfigBase
{
	ref ExpansionQuestObjectiveTreasureHunt TreasureHunt;
};

class ExpansionQuestObjectiveTreasureHuntConfig_V20: ExpansionQuestObjectiveTreasureHuntConfigBase
{
	ref array <ref ExpansionLootV1> Loot;
};

class ExpansionQuestObjectiveTreasureHuntConfigBase: ExpansionQuestObjectiveConfig
{
	bool ShowDistance = true;
	
	string ContainerName = "ExpansionQuestSeaChest";
	bool DigInStash = true;
	string MarkerName = "???";	
	int MarkerVisibility = EXPANSION_MARKER_VIS_MAP; //! 4 - visible on map | 2 - visible in world | 6 - visible on map and in world.
	ref array<vector> Positions;
	int LootItemsAmount;
	float MaxDistance = 10.0;
};

class ExpansionQuestObjectiveTreasureHuntConfig: ExpansionQuestObjectiveTreasureHuntConfigBase
{	
	ref array <ref ExpansionLoot> Loot;

	void ExpansionQuestObjectiveTreasureHuntConfig()
	{
		Positions = new array<vector>;
		Loot = new array <ref ExpansionLoot>;
	}

	bool ShowDistance()
	{
		return ShowDistance;
	}
	
	string GetContainerName()
	{
		return ContainerName;
	}
	
	bool DigInStash()
	{
		return DigInStash;
	}
	
	string GetMarkerName()
	{
		return MarkerName;
	}
	
	int GetMarkerVisibility()
	{
		return MarkerVisibility;
	}
	
	void AddPosition(vector pos)
	{
		int findIndex = -1;
		findIndex = Positions.Find(pos);
		if (findIndex == -1)
			Positions.Insert(pos);
	}
	
	array<vector> GetPositions()
	{
		return Positions;
	}
	
	void AddLoot(ExpansionLoot loot)
	{
		Loot.Insert(loot);
	}
	
	void InsertLoot(array<ref ExpansionLoot> loots)
	{
		foreach (ExpansionLoot loot: loots)
		{
			Loot.Insert(loot);
		}
	}
	
	array <ref ExpansionLoot> GetLoot()
	{
		return Loot;
	}
	
	void SetLootItemsAmount(int amount)
	{
		LootItemsAmount = amount;
	}
	
	int GetLootItemsAmount()
	{
		return LootItemsAmount;
	}
	
	void SetMaxDistance(float max)
	{
		MaxDistance = max;
	}

	float GetMaxDistance()
	{
		return MaxDistance;
	}

	static ExpansionQuestObjectiveTreasureHuntConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveTreasureHuntConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName);

		ExpansionQuestObjectiveTreasureHuntConfig config;
		ExpansionQuestObjectiveTreasureHuntConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTreasureHuntConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName, configBase))
			return NULL;
		
		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveTreasureHuntConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName + " to version " + CONFIGVERSION);

			if (configBase.ConfigVersion < 27)
			{
				config = new ExpansionQuestObjectiveTreasureHuntConfig();

				//! Copy over old configuration that haven't changed
				config.CopyConfig(configBase);
			}
			else if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTreasureHuntConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName, config))
			{
				return null;
			}
			
			if (configBase.ConfigVersion < 18)
			{
				ExpansionQuestObjectiveTreasureHuntConfig_V17 configV17;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTreasureHuntConfig_V17>.Load(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName, configV17))
				{
					Error("ExpansionQuestObjectiveTreasureHuntConfig::Load - Config conversion failed for file: " + EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName + " | Error Code: 1");
					return NULL;
				}
				
				ExpansionQuestObjectiveTreasureHunt treasureHunt = configV17.TreasureHunt;
				if (!treasureHunt)
				{
					Error("ExpansionQuestObjectiveTreasureHuntConfig::Load - Config conversion failed for file: " + EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName + " | Error Code: 2");
					return NULL;
				}
				
				foreach (vector pos: treasureHunt.Positions)
				{
					config.AddPosition(pos);
				}
				
				ExpansionLoot loot;
				foreach (string className, int amount: treasureHunt.Items)
				{
					loot = new ExpansionLoot(className, null, 0.30, -1, null, -1);					
					config.AddLoot(loot);
				}
				
				config.SetLootItemsAmount(treasureHunt.Items.Count());
			}
			else if (configBase.ConfigVersion < 27)
			{
				ExpansionQuestObjectiveTreasureHuntConfig_V20 configV20;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTreasureHuntConfig_V20>.Load(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName, configV20))
				{
					Error("ExpansionQuestObjectiveTreasureHuntConfig::Load - Config conversion failed for file: " + EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName + " | Error Code: 1");
					return NULL;
				}
				
				array <ref ExpansionLootV1> lootV1Array = configV20.Loot;
				if (!lootV1Array)
				{
					Error("ExpansionQuestObjectiveTreasureHuntConfig::Load - Config conversion failed for file: " + EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName + " | Error Code: 2");
					return NULL;
				}

				array <ref ExpansionLoot> lootArray = new array <ref ExpansionLoot>;
				foreach(ExpansionLootV1 lootv1: lootV1Array)
				{
					lootArray.Insert(lootv1.Convert());
				}

				config.Loot = lootArray;
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTreasureHuntConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName, config))
				return NULL;
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveTreasureHuntConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveTreasureHuntConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		
		ShowDistance = configBase.ShowDistance;
		ContainerName = configBase.ContainerName;
		DigInStash = configBase.DigInStash;
		MarkerName = configBase.MarkerName;
		MarkerVisibility = configBase.MarkerVisibility;
		Positions = configBase.Positions;
		LootItemsAmount = configBase.LootItemsAmount;
		MaxDistance = configBase.MaxDistance;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		ctx.Write(ShowDistance);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!ctx.Read(ShowDistance))
			return false;

		return true;
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;
		
		if (!ContainerName.ToType().IsInherited(ExpansionQuestContainerBase))
			return false;

		if (!Loot || Loot.Count() == 0)
			return false;
		
		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
	#endif
	}
};