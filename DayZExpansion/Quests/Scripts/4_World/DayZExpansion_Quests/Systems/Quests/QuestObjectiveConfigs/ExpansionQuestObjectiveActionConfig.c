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
};

class ExpansionQuestObjectiveActionConfig: ExpansionQuestObjectiveActionConfigBase
{
	protected autoptr array<string> AllowedClassNames = new array<string>;
	protected autoptr array<string> ExcludedClassNames = new array<string>;

	void AddActionName(string name)
	{
		ActionNames.Insert(name);
	}

	override array<string> GetActionNames()
	{
		return ActionNames;
	}

	void AddAllowedClassName(string typeName)
	{
		AllowedClassNames.Insert(typeName);
	}

	override array<string> GetAllowedClassNames()
	{
		return AllowedClassNames;
	}

	void AddExcludedClassName(string typeName)
	{
		ExcludedClassNames.Insert(typeName);
	}

	override array<string> GetExcludedClassNames()
	{
		return ExcludedClassNames;
	}

	static ExpansionQuestObjectiveActionConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveActionConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveActionConfig config;
		ExpansionQuestObjectiveActionConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveActionConfigBase>.Load(fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveActionConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveActionConfig();

			if (configBase.ConfigVersion < 11)
			{
				//! Copy over old configuration that haven't HandAnimEventChanged
				config.CopyConfig(configBase);
				config.ObjectiveText == "";
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveActionConfig>.Load(fileName, config))
				return NULL;
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveActionConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveActionConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName + ".json", this);
	}

	void CopyConfig(ExpansionQuestObjectiveActionConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;

		ActionNames = configBase.ActionNames;
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

		if (!ActionNames || ActionNames && ActionNames.Count() == 0)
			return false;

		return true;
	}
};