/**
 * ExpansionQuestObjectiveCollectionConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class ExpansionQuestObjectiveCollectionConfigBase: ExpansionQuestObjectiveConfig
{
	ref ExpansionQuestObjectiveCollection Collection = new ExpansionQuestObjectiveCollection();
}

class ExpansionQuestObjectiveCollectionConfig: ExpansionQuestObjectiveCollectionConfigBase
{
	float MaxDistance = 10.0;
	string MarkerName = "Deliver Items";
	bool ShowDistance = true;
	
	void SetMaxDistance(float max)
	{
		MaxDistance = max;
	}

	override float GetMaxDistance()
	{
		return MaxDistance;
	}
	
	bool ShowDistance()
	{
		return ShowDistance;
	}

	void SetMarkerName(string name)
	{
		MarkerName = name;
	}
	
	override string GetMarkerName()
	{
		return MarkerName;
	}
	
	void AddCollection(ExpansionQuestObjectiveCollection collection)
	{
		Collection = collection;
	}

	override ExpansionQuestObjectiveCollection GetCollection()
	{
		return Collection;
	}
	
	static ExpansionQuestObjectiveCollectionConfig Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestObjectiveCollectionConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveCollectionConfig config;
		ExpansionQuestObjectiveCollectionConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveCollectionConfigBase>.Load(fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestObjectiveCollectionConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveCollectionConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);
			
			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveCollectionConfig>.Load(fileName, config))
				return NULL;
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveCollectionConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveCollectionConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName + ".json", this);
	}
	
	void CopyConfig(ExpansionQuestObjectiveCollectionConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		
		Collection = configBase.Collection;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);
		
		Collection.OnSend(ctx);
		
		ctx.Write(MaxDistance);
		ctx.Write(MarkerName);
		ctx.Write(ShowDistance);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!Collection)
			Collection = new ExpansionQuestObjectiveCollection();

		if (!Collection.OnRecieve(ctx))
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
		if (Collection)
			Collection.QuestDebug();
	#endif
	}
};