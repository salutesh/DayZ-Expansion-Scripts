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

class ExpansionQuestObjectiveCollectionConfig_V10: ExpansionQuestObjectiveCollectionConfigBase
{
	ref ExpansionQuestObjectiveCollection Collection = new ExpansionQuestObjectiveCollection();
};

class ExpansionQuestObjectiveCollectionConfigBase: ExpansionQuestObjectiveConfig
{
	ref array<ref ExpansionQuestObjectiveDelivery> Collections;
	float MaxDistance = 10.0;
	string MarkerName = "Deliver Items";
	bool ShowDistance = true;
};

class ExpansionQuestObjectiveCollectionConfig: ExpansionQuestObjectiveCollectionConfigBase
{
#ifdef EXPANSIONMODMARKET
	bool AddItemsToNearbyMarketZone = false;
#endif
	
	bool NeedAnyCollection = false;

	void ExpansionQuestObjectiveCollectionConfig()
	{
		Collections = new array<ref ExpansionQuestObjectiveDelivery>;
	}

	void AddCollection(int amount, string name)
	{
		ExpansionQuestObjectiveDelivery collection = new ExpansionQuestObjectiveDelivery();
		collection.SetAmount(amount);
		collection.SetClassName(name);
		Collections.Insert(collection);
	}

	void SetMaxDistance(float max)
	{
		MaxDistance = max;
	}

	float GetMaxDistance()
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

	string GetMarkerName()
	{
		return MarkerName;
	}

	array<ref ExpansionQuestObjectiveDelivery> GetDeliveries()
	{
		return Collections;
	}

#ifdef EXPANSIONMODMARKET
	void SetAddItemsToNearbyMarketZone(bool state)
	{
		AddItemsToNearbyMarketZone = state;
	}

	bool AddItemsToNearbyMarketZone()
	{
		return AddItemsToNearbyMarketZone;
	}
#endif
	
	bool NeedAnyCollection()
	{
		return NeedAnyCollection;
	}

	static ExpansionQuestObjectiveCollectionConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveCollectionConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName);

		ExpansionQuestObjectiveCollectionConfig config;
		ExpansionQuestObjectiveCollectionConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveCollectionConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveCollectionConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveCollectionConfig();

			//! Copy over old configuration that haven't HandAnimEventChanged
			config.CopyConfig(configBase);

			if (configBase.ConfigVersion < 11)
			{
				ExpansionQuestObjectiveCollectionConfig_V10 configV10;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveCollectionConfig_V10>.Load(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName, configV10))
					return NULL;

				config.AddCollection(configV10.Collection.GetAmount(), configV10.Collection.GetClassName());
			}

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveCollectionConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName, config))
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
		Print(ToString() + "::Save - FileName: " + EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName);
		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveCollectionConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestObjectiveCollectionConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		MaxDistance = configBase.MaxDistance;
		MarkerName = configBase.MarkerName;
		ShowDistance = configBase.ShowDistance;
		Collections = configBase.Collections;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		int collectionCount = Collections.Count();
		ctx.Write(collectionCount);

		foreach (ExpansionQuestObjectiveDelivery collection: Collections)
		{
			collection.OnSend(ctx);
		}

		ctx.Write(ShowDistance);
		ctx.Write(NeedAnyCollection);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!Collections)
			Collections = new array<ref ExpansionQuestObjectiveDelivery>;
		else
			Collections.Clear();

		int collectionCount;
		if (!ctx.Read(collectionCount))
			return false;

		for (int i = 0; i < collectionCount; i++)
		{
			ExpansionQuestObjectiveDelivery collection = new ExpansionQuestObjectiveDelivery();
			if (!collection.OnRecieve(ctx))
				return false;

			Collections.Insert(collection);
		}

		if (!ctx.Read(ShowDistance))
			return false;
		
		if (!ctx.Read(NeedAnyCollection))
			return false;

		return true;
	}

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		if (!Collections || Collections && Collections.Count() == 0)
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
		if (Collections)
		{
			foreach (ExpansionQuestObjectiveDelivery collection: Collections)
			{
				collection.QuestDebug();
			}
		}
	#endif
	}
};