/**
 * ExpansionQuestObjectiveAIEscortConfigBase.c
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
class ExpansionQuestObjectiveAIEscortConfig_V25: ExpansionQuestObjectiveConfig
{
	autoptr ExpansionQuestObjectiveAIVIP AIVIP;
};

class ExpansionQuestObjectiveAIEscortConfigBase: ExpansionQuestObjectiveConfig
{
	vector Position = vector.Zero;
	float MaxDistance = 0;
	string MarkerName = string.Empty;
	bool ShowDistance = true;
	bool CanLootAI = false;
};

class ExpansionQuestObjectiveAIEscortConfig: ExpansionQuestObjectiveAIEscortConfigBase
{
	string NPCLoadoutFile;
	string NPCClassName;
	string NPCName;
	
	void SetPosition(vector pos)
	{
		Position = pos;
	}

	override vector GetPosition()
	{
		return Position;
	}

	void SetMaxDistance(float max)
	{
		MaxDistance = max;
	}

	float GetMaxDistance()
	{
		return MaxDistance;
	}

	void SetMarkerName(string name)
	{
		MarkerName = name;
	}

	string GetMarkerName()
	{
		return MarkerName;
	}

	bool ShowDistance()
	{
		return ShowDistance;
	}
	
	void SetCanBeLooted(bool state)
	{
		CanLootAI = state;
	}
	
	bool CanBeLooted()
	{
		return CanLootAI;
	}
	
	void SetLoadout(string loadoutFile)
	{
		NPCLoadoutFile = loadoutFile;
	}

	string GetLoadout()
	{
		return NPCLoadoutFile;
	}
	
	void SetNPCClassName(string className)
	{
		NPCClassName = className;
	}

	string GetNPCClassName()
	{
		return NPCClassName;
	}
	
	void SetNPCName(string name)
	{
		NPCName = name;
	}

	string GetNPCName()
	{
		return NPCName;
	}

	static ExpansionQuestObjectiveAIEscortConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveAIEscortConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName);

		ExpansionQuestObjectiveAIEscortConfig config;
		ExpansionQuestObjectiveAIEscortConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIEscortConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName, config))
			return NULL;

		//! Uncomment this once needed
		//if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIEscortConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName, configBase))
			//return NULL;

		//! Comment this out once needed
		configBase = config;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveAIEscortConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName + " to version " + CONFIGVERSION);
			if (!config)
				config = new ExpansionQuestObjectiveAIEscortConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);
			
			if (configBase.ConfigVersion < 26)
			{
				ExpansionQuestObjectiveAIEscortConfig_V25 configV25;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIEscortConfig_V25>.Load(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName, configV25))
					return NULL;

				if (configV25.AIVIP)
				{
					config.NPCLoadoutFile = configV25.AIVIP.NPCLoadoutFile;
					config.NPCClassName = configV25.AIVIP.NPCClassName;
					config.NPCName = "Survivor";
				}
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		
		string removeExt = ExpansionString.StripExtension(config.GetLoadout(), ".json");
		if (removeExt != config.GetLoadout())
		{
			config.SetLoadout(removeExt);
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveAIEscortConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveAIEscortConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;

		Position = configBase.Position;
		MaxDistance = configBase.MaxDistance;
		MarkerName = configBase.MarkerName;
		ShowDistance = configBase.ShowDistance;
		CanLootAI = configBase.CanLootAI;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		ctx.Write(Position);
		ctx.Write(ShowDistance);
		ctx.Write(MarkerName);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!ctx.Read(Position))
			return false;

		if (!ctx.Read(ShowDistance))
			return false;
		
		if (!ctx.Read(MarkerName))
			return false;

		return true;
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		return true;
	}
};
#endif
