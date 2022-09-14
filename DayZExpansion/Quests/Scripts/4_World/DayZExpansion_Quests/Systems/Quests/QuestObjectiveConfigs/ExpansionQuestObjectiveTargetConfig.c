/**
 * ExpansionQuestObjectiveTargetConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTargetConfigBase: ExpansionQuestObjectiveConfig
{
	vector Position = vector.Zero;
	float MaxDistance = -1;
	autoptr ExpansionQuestObjectiveTarget Target;
}

class ExpansionQuestObjectiveTargetConfig: ExpansionQuestObjectiveTargetConfigBase
{
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

	override float GetMaxDistance()
	{
		return MaxDistance;
	}

	void SetTarget(ExpansionQuestObjectiveTarget target)
	{
		Target = target;
	}

	override ExpansionQuestObjectiveTarget GetTarget()
	{
		return Target;
	}
	
	static ExpansionQuestObjectiveTargetConfig Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestObjectiveTargetConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveTargetConfig config;
		ExpansionQuestObjectiveTargetConfigBase configBase;
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveTargetConfigBase>.Load(fileName, configBase);
		if (!configBase)
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestObjectiveTargetConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveTargetConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);
			
			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			ExpansionJsonFileParser<ExpansionQuestObjectiveTargetConfig>.Load(fileName, config);
			if (!config)
				return NULL;
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveTargetConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}
	
	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveTargetConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName + ".json", this);
	}
	
	void CopyConfig(ExpansionQuestObjectiveTargetConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		
		Position = configBase.Position;
		MaxDistance = configBase.MaxDistance;
		Target = configBase.Target;
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
		if (Target)
			Target.QuestDebug();
	#endif
	}
};