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
class ExpansionQuestObjectiveAIPatrolConfigBase: ExpansionQuestObjectiveConfig
{
	ref ExpansionQuestObjectiveAIPatrol AIPatrol;
}

class ExpansionQuestObjectiveAIPatrolConfig: ExpansionQuestObjectiveAIPatrolConfigBase
{
	float MinDistRadius = 50;
	float MaxDistRadius = 150;
	float DespawnRadius = 880;
	bool CanLootAI = true;

	void SetAIPatrol(ExpansionQuestObjectiveAIPatrol patrol)
	{
		AIPatrol = patrol;
	}

	override ExpansionQuestObjectiveAIPatrol GetAIPatrol()
	{
		return AIPatrol;
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

	static ExpansionQuestObjectiveAIPatrolConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveAIPatrolConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveAIPatrolConfig config;
		ExpansionQuestObjectiveAIPatrolConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfigBase>.Load(fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveAIPatrolConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);

			if (configBase.ConfigVersion < 4)
			{
				config = new ExpansionQuestObjectiveAIPatrolConfig();

				//! Copy over old configuration that haven't changed
				config.CopyConfig(configBase);

				config.MinDistRadius = 50;
				config.MaxDistRadius = 150;
				config.DespawnRadius = 880;
			}
			else
			{
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfig>.Load(fileName, config))
					return NULL;
			}

			if (configBase.ConfigVersion < 5)
			{
				if (config.AIPatrol)
					config.AIPatrol.NPCFormation = "RANDOM";
			}

			if (configBase.ConfigVersion < 6)
			{
				config.CanLootAI = true;
			}

			if (configBase.ConfigVersion < 7)
			{
				if (config.AIPatrol)
				{
					config.AIPatrol.NPCAccuracyMin = -1;
					config.AIPatrol.NPCAccuracyMax = -1;
				}
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIPatrolConfig>.Load(fileName, config))
				return NULL;
		}

		if (!config.DespawnRadius)
			config.DespawnRadius = 880;

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveAIPatrolConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveAIPatrolConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName + ".json", this);
	}

	void CopyConfig(ExpansionQuestObjectiveAIPatrolConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;

		AIPatrol = configBase.AIPatrol;
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

		if (!AIPatrol)
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
		if (AIPatrol)
			AIPatrol.QuestDebug();
	#endif
	}
};
#endif