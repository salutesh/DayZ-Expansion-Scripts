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
class ExpansionQuestObjectiveTreasureHuntConfigBase:ExpansionQuestObjectiveConfig
{
	bool ShowDistance = true;
	ref ExpansionQuestObjectiveTreasureHunt TreasureHunt;
};

class ExpansionQuestObjectiveTreasureHuntConfig: ExpansionQuestObjectiveTreasureHuntConfigBase
{
	void SetTreasureHunt(ExpansionQuestObjectiveTreasureHunt hunt)
	{
		TreasureHunt = hunt;
	}

	override ExpansionQuestObjectiveTreasureHunt GetTreasureHunt()
	{
		return TreasureHunt;
	}

	bool ShowDistance()
	{
		return ShowDistance;
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
			config = new ExpansionQuestObjectiveTreasureHuntConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

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
		Print(ToString() + "::Save - FileName: " + EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName);
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

		TreasureHunt = configBase.TreasureHunt;
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

		if (!TreasureHunt)
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
		if (TreasureHunt)
			TreasureHunt.QuestDebug();
	#endif
	}
};