/**
 * ExpansionQuestObjectiveActionConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveActionConfigBase: ExpansionQuestObjectiveConfig
{
	autoptr array<string> ActionNames = new array<string>;
	autoptr array<string> AllowedClassNames = new array<string>;
	autoptr array<string> ExcludedClassNames = new array<string>;
	int ExecutionAmount = 1;
};

class ExpansionQuestObjectiveActionConfig: ExpansionQuestObjectiveActionConfigBase
{
	void AddActionName(string name)
	{
		ActionNames.Insert(name);
	}

	array<string> GetActionNames()
	{
		return ActionNames;
	}

	void AddAllowedClassName(string typeName)
	{
		AllowedClassNames.Insert(typeName);
	}

	array<string> GetAllowedClassNames()
	{
		return AllowedClassNames;
	}

	void AddExcludedClassName(string typeName)
	{
		ExcludedClassNames.Insert(typeName);
	}

	array<string> GetExcludedClassNames()
	{
		return ExcludedClassNames;
	}
	
	void SetExecutionAmount(int amount)
	{
		ExecutionAmount = amount;
	}
	
	int GetExecutionAmount()
	{
		return ExecutionAmount;
	}

	static ExpansionQuestObjectiveActionConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveActionConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName);

		ExpansionQuestObjectiveActionConfig config;
		ExpansionQuestObjectiveActionConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveActionConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveActionConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveActionConfig();
			
			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);
			
			if (configBase.ConfigVersion < 11)
			{
				//! Copy over old configuration that haven't HandAnimEventChanged
				config.ObjectiveText == "";
			}
			
			if (configBase.ConfigVersion < 12)
			{
				config.ExecutionAmount = 1;
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveActionConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName, config))
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveActionConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveActionConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;

		ActionNames = configBase.ActionNames;
		AllowedClassNames = configBase.AllowedClassNames;
		ExcludedClassNames = configBase.ExcludedClassNames;
		ExecutionAmount = configBase.ExecutionAmount;
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		if (!ActionNames || ActionNames && ActionNames.Count() == 0)
			return false;

		return true;
	}
};