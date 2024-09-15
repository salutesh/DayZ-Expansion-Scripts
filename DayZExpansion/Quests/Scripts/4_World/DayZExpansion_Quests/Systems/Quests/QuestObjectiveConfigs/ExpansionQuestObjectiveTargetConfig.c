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

class ExpansionQuestObjectiveTargetConfig_25: ExpansionQuestObjectiveConfig
{
	autoptr ExpansionQuestObjectiveTarget Target;
};

class ExpansionQuestObjectiveTargetConfigBase: ExpansionQuestObjectiveConfig
{
	vector Position = vector.Zero;
	float MaxDistance = -1;
};

class ExpansionQuestObjectiveTargetConfig: ExpansionQuestObjectiveTargetConfigBase
{
	float MinDistance = -1;
	int Amount = -1;
	autoptr TStringArray ClassNames = new TStringArray;
	bool CountSelfKill;
	autoptr TStringArray AllowedWeapons = new TStringArray;
	autoptr TStringArray ExcludedClassNames = new TStringArray;

	bool CountAIPlayers = false;
	autoptr TStringArray AllowedTargetFactions = new TStringArray;

	autoptr TStringArray AllowedDamageZones = new TStringArray;

	void SetAmount(int amount)
	{
		Amount = amount;
	}

	int GetAmount()
	{
		return Amount;
	}

	void AddClassName(string name)
	{
		if (!ClassNames.Find(name))
			ClassNames.Insert(name);
	}

	TStringArray GetClassNames()
	{
		return ClassNames;
	}

	void AddAllowedWeapon(string name)
	{
		if (!AllowedWeapons.Find(name))
			AllowedWeapons.Insert(name);
	}

	TStringArray GetAllowedWeapons()
	{
		return AllowedWeapons;
	}

	void AddExcludedClassName(string name)
	{
		if (!ExcludedClassNames.Find(name))
			ExcludedClassNames.Insert(name);
	}

	TStringArray GetExcludedClassNames()
	{
		return ExcludedClassNames;
	}
	
	void SetCountSelfKill(bool state)
	{
		CountSelfKill = state;
	}
	
	bool CountSelfKill()
	{
		return CountSelfKill;
	}
	
	void SetCountAIPlayers(bool state)
	{
		CountAIPlayers = state;
	}
	
	bool CountAIPlayers()
	{
		return CountAIPlayers;
	}
	
	void AddAllowedFaction(string factionName)
	{
		if (!AllowedTargetFactions.Find(factionName))
			AllowedTargetFactions.Insert(factionName);
	}

	TStringArray GetAllowedTargetFactions()
	{
		return AllowedTargetFactions;
	}
	
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

	float GetMaxDistance()
	{
		return MaxDistance;
	}

	void SetMinDistance(float min)
	{
		MinDistance = min;
	}

	float GetMinDistance()
	{
		return MinDistance;
	}
	
	void AddAllowedDamageZone(string zone)
	{
		if (!AllowedDamageZones.Find(zone))
			AllowedDamageZones.Insert(zone);
	}

	TStringArray GetAllowedDamageZones()
	{
		return AllowedDamageZones;
	}

	static ExpansionQuestObjectiveTargetConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveTargetConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName);

		ExpansionQuestObjectiveTargetConfig config;
		ExpansionQuestObjectiveTargetConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTargetConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName, config))
			return NULL;

		//! Uncomment this once needed
		//if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTargetConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName, configBase))
			//return NULL;

		//! Comment this out once needed
		configBase = config;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveTargetConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName + " to version " + CONFIGVERSION);
			if (!config)
				config = new ExpansionQuestObjectiveTargetConfig();
			
			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			if (configBase.ConfigVersion < 13)
			{
				config.SetCountAIPlayers(false);
			}
			
			if (configBase.ConfigVersion < 26)
			{
				ExpansionQuestObjectiveTargetConfig_25 configV25;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveTargetConfig_25>.Load(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName, configV25))
					return NULL;

				if (configV25.Target)
				{
					config.Amount = configV25.Target.Amount;
					config.ClassNames = configV25.Target.ClassNames;
					config.CountSelfKill = configV25.Target.CountSelfKill;
					config.AllowedWeapons = configV25.Target.AllowedWeapons;
					config.ExcludedClassNames = configV25.Target.ExcludedClassNames;

					config.CountAIPlayers = configV25.Target.CountAIPlayers;
					config.AllowedTargetFactions = configV25.Target.AllowedTargetFactions;
				}
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}

		if (save)
			config.Save(fileName);

		return config;
	}
	
	override void Save(string fileName)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveTargetConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveTargetConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;

		Position = configBase.Position;
		MaxDistance = configBase.MaxDistance;
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		return true;
	}
};