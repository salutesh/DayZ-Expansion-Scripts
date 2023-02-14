/**
 * ExpansionQuestObjectiveAIEscordConfigBase.c
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
class ExpansionQuestObjectiveAIEscordConfigBase: ExpansionQuestObjectiveConfig
{
	vector Position = vector.Zero;
	float MaxDistance = 0;
	ref ExpansionQuestObjectiveAIVIP AIVIP;
	string MarkerName = string.Empty;
};

class ExpansionQuestObjectiveAIEscordConfig: ExpansionQuestObjectiveAIEscordConfigBase
{
	bool ShowDistance = true;

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

	void SetAIVIP(ExpansionQuestObjectiveAIVIP vip)
	{
		AIVIP = vip;
	}

	override ExpansionQuestObjectiveAIVIP GetAIVIP()
	{
		return AIVIP;
	}

	void SetMarkerName(string name)
	{
		MarkerName = name;
	}

	override string GetMarkerName()
	{
		return MarkerName;
	}

	bool ShowDistance()
	{
		return ShowDistance;
	}

	static ExpansionQuestObjectiveAIEscordConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveAIEscordConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName);

		ExpansionQuestObjectiveAIEscordConfig config;
		ExpansionQuestObjectiveAIEscordConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIEscordConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveAIEscordConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveAIEscordConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIEscordConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName, config))
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
		Print(ToString() + "::Save - FileName: " + EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName);
		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveAIEscordConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveAIEscordConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;

		Position = configBase.Position;
		MaxDistance = configBase.MaxDistance;
		AIVIP = configBase.AIVIP;
		MarkerName = configBase.MarkerName;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		ctx.Write(Position);
		ctx.Write(MaxDistance);
		ctx.Write(MarkerName);
		ctx.Write(ShowDistance);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!ctx.Read(Position))
			return false;

		if (!ctx.Read(MaxDistance))
			return false;

		if (!ctx.Read(MarkerName))
			return false;

		if (!ctx.Read(ShowDistance))
			return false;

		return true;
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		if (!AIVIP)
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
		if (AIVIP)
			AIVIP.QuestDebug();
	#endif
	}
};
#endif