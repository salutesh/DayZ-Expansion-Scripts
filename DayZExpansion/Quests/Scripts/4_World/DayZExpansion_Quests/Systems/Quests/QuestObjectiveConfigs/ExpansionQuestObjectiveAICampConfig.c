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
class ExpansionQuestObjectiveAICampConfigBase: ExpansionQuestObjectiveConfig
{
	ref ExpansionQuestObjectiveAICamp AICamp;
	float MinDistRadius = 50;
	float MaxDistRadius = 150;
	float DespawnRadius = 880;
	bool CanLootAI = true;
}

class ExpansionQuestObjectiveAICampConfig: ExpansionQuestObjectiveAICampConfigBase
{
	int InfectedDeletionRadius = 500;
	
	void SetAICamp(ExpansionQuestObjectiveAICamp camp)
	{
		AICamp = camp;
	}

	ExpansionQuestObjectiveAICamp GetAICamp()
	{
		return AICamp;
	}

	void SetMinDistRadius(float dist)
	{
		MinDistRadius = dist;
	}

	float GetMinDistRadius()
	{
		return MinDistRadius;
	}

	void SetMaxDistRadius(float dist)
	{
		MaxDistRadius = dist;
	}

	float GetMaxDistRadius()
	{
		return MaxDistRadius;
	}

	void SetDespawnRadius(float dist)
	{
		DespawnRadius = dist;
	}

	float GetDespawnRadius()
	{
		return DespawnRadius;
	}

	void SetCanLootAI(bool state)
	{
		CanLootAI = state;
	}

	bool CanLootAI()
	{
		return CanLootAI;
	}
	
	int GetInfectedDeletionRadius()
	{
		return InfectedDeletionRadius;
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
			config = new ExpansionQuestObjectiveAICampConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			if (configBase.ConfigVersion < 6)
			{
				config.CanLootAI = true;
			}

			if (configBase.ConfigVersion < 7)
			{
				if (config.AICamp)
				{
					config.AICamp.NPCAccuracyMin = -1;
					config.AICamp.NPCAccuracyMax = -1;
				}
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName, config))
				return NULL;
		}

		if (!config.DespawnRadius)
			config.DespawnRadius = 880;
		
		string removeExt = ExpansionString.StripExtension(config.AICamp.GetNPCLoadoutFile(), ".json");
		if (removeExt != config.AICamp.GetNPCLoadoutFile())
		{
			config.AICamp.SetNPCLoadoutFile(removeExt);
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName);

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

		AICamp = configBase.AICamp;
		MinDistRadius = configBase.MinDistRadius;
		MaxDistRadius = configBase.MaxDistRadius;
		DespawnRadius = configBase.DespawnRadius;
		CanLootAI = configBase.CanLootAI;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		return true;
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		if (!AICamp)
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
		if (AICamp)
			AICamp.QuestDebug();
	#endif
	}
};
#endif