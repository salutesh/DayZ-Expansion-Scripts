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
	
	static ExpansionQuestObjectiveAIPatrolConfig Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestObjectiveAIPatrolConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveAIPatrolConfig config;
		ExpansionQuestObjectiveAIPatrolConfigBase configBase;
		JsonFileLoader<ExpansionQuestObjectiveAIPatrolConfigBase>.JsonLoadFile(fileName, configBase);

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestObjectiveAIPatrolConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveAIPatrolConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			JsonFileLoader<ExpansionQuestObjectiveAIPatrolConfig>.JsonLoadFile(fileName, config);
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveAIPatrolConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveAIPatrolConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER + fileName + ".JSON", this);
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

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (AIPatrol)
			AIPatrol.QuestDebug();
	#endif
	}
};
#endif