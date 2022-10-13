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
}

class ExpansionQuestObjectiveAICampConfig: ExpansionQuestObjectiveAICampConfigBase
{
	float MinDistRadius = 50;
	float MaxDistRadius = 150;
	float DespawnRadius = 880;
	bool CanLootAI = true;
	
	void SetAICamp(ExpansionQuestObjectiveAICamp camp)
	{
		AICamp = camp;
	}

	override ExpansionQuestObjectiveAICamp GetAICamp()
	{
		return AICamp;
	}
	
	void SetMinDistRadius(float dist)
	{
		MinDistRadius = dist;
	}
	
	override float GetMinDistRadius()
	{
		return MinDistRadius;
	}
	
	void SetMaxDistRadius(float dist)
	{
		MaxDistRadius = dist;
	}
	
	override float GetMaxDistRadius()
	{
		return MaxDistRadius;
	}
	
	void SetDespawnRadius(float dist)
	{
		DespawnRadius = dist;
	}
	
	override float GetDespawnRadius()
	{
		return DespawnRadius;
	}
	
	void SetCanLootAI(bool state)
	{
		CanLootAI = state;
	}
	
	override bool CanLootAI()
	{
		return CanLootAI;
	}

	static ExpansionQuestObjectiveAICampConfig Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestObjectiveAICampConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveAICampConfig config;
		ExpansionQuestObjectiveAICampConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfigBase>.Load(fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestObjectiveAICampConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
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
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAICampConfig>.Load(fileName, config))
				return NULL;
		}

		if (!config.DespawnRadius)
			config.DespawnRadius = 880;

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveAICampConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}
	
	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveAICampConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName + ".json", this);
	}
	
	void CopyConfig(ExpansionQuestObjectiveAICampConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		
		AICamp = configBase.AICamp;
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

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (AICamp)
			AICamp.QuestDebug();
	#endif
	}
};
#endif