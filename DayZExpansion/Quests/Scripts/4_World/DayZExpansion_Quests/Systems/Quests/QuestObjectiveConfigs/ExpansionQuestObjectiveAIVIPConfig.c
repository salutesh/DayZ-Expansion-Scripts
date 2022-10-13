/**
 * ExpansionQuestObjectiveAIVIPConfig.c
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
class ExpansionQuestObjectiveAIVIPConfigBase: ExpansionQuestObjectiveConfig
{
	vector Position = vector.Zero;
	float MaxDistance = 0;
	ref ExpansionQuestObjectiveAIVIP AIVIP;
	string MarkerName = string.Empty;
};

class ExpansionQuestObjectiveAIVIPConfig: ExpansionQuestObjectiveAIVIPConfigBase
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

	static ExpansionQuestObjectiveAIVIPConfig Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestObjectiveAIVIPConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveAIVIPConfig config;
		ExpansionQuestObjectiveAIVIPConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIVIPConfigBase>.Load(fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestObjectiveAIVIPConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveAIVIPConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveAIVIPConfig>.Load(fileName, config))
				return NULL;
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveAIVIPConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}
	
	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveAIVIPConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName + ".json", this);
	}
	
	void CopyConfig(ExpansionQuestObjectiveAIVIPConfigBase configBase)
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

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (AIVIP)
			AIVIP.QuestDebug();
	#endif
	}
};
#endif