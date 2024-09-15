/**
 * ExpansionQuestObjectiveDeliveryConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! DEPRICATED - Only still here for conversion reasons.
class ExpansionQuestObjectiveDeliveryConfig_v17: ExpansionQuestObjectiveConfig
{
	autoptr array<ref ExpansionQuestObjectiveDelivery> Deliveries = new array<ref ExpansionQuestObjectiveDelivery>;
	float MaxDistance = 5.0;
	string MarkerName;
	bool ShowDistance = true;
};

class ExpansionQuestObjectiveDeliveryConfigBase: ExpansionQuestObjectiveConfig
{
	autoptr array<ref ExpansionQuestObjectiveDelivery> Collections = new array<ref ExpansionQuestObjectiveDelivery>;
	bool ShowDistance = true;

	bool AddItemsToNearbyMarketZone = false;

	void AddCollection(int amount, string name, int quantityPercent = -1)
	{
		ExpansionQuestObjectiveDelivery collection = new ExpansionQuestObjectiveDelivery();
		collection.SetAmount(amount);
		collection.SetClassName(name);
		collection.SetQuantity(quantityPercent);
		Collections.Insert(collection);
	}

	void SetMaxDistance(float max)
	{
	}

	float GetMaxDistance()
	{
		return 0.0;
	}

	bool ShowDistance()
	{
		return ShowDistance;
	}

	void SetMarkerName(string name)
	{
	}

	string GetMarkerName()
	{
		return "";
	}

	array<ref ExpansionQuestObjectiveDelivery> GetCollections()
	{
		return Collections;
	}

	void SetAddItemsToNearbyMarketZone(bool state)
	{
		AddItemsToNearbyMarketZone = state;
	}

	bool AddItemsToNearbyMarketZone()
	{
		return AddItemsToNearbyMarketZone;
	}
	
	bool NeedAnyCollection()
	{
		return false;
	}
	
	void CopyConfig(ExpansionQuestObjectiveDeliveryConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		ShowDistance = configBase.ShowDistance;
		
		Collections.Clear();
		foreach (ExpansionQuestObjectiveDelivery delivey: configBase.Collections)
		{
			ExpansionQuestObjectiveDelivery deliveryCopy = new ExpansionQuestObjectiveDelivery();
			deliveryCopy.Copy(delivey);
			Collections.Insert(deliveryCopy);
		}
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

class ExpansionQuestObjectiveDeliveryConfig: ExpansionQuestObjectiveDeliveryConfigBase
{
	float MaxDistance = 10.0;
	string MarkerName = "Deliver Items";

	static ExpansionQuestObjectiveDeliveryConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveDeliveryConfig] Load existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName);

		ExpansionQuestObjectiveDeliveryConfig config;
		ExpansionQuestObjectiveDeliveryConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveDeliveryConfigBase>.Load(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveDeliveryConfig] Convert existing configuration file:" + EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveDeliveryConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			if (configBase.ConfigVersion < 18)
			{
				ExpansionQuestObjectiveDeliveryConfig_v17 configV17;
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveDeliveryConfig_v17>.Load(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName, configV17))
					return NULL;
				
				foreach (ExpansionQuestObjectiveDelivery deliveryV17: configV17.Deliveries)
				{
					string className = deliveryV17.GetClassName();
					int amount = deliveryV17.GetAmount();
					config.AddCollection(amount, className);
				}
			}
			
			if (configBase.ConfigVersion < 19)
			{
				foreach (ExpansionQuestObjectiveDelivery deliveryV18: configBase.Collections)
				{
					deliveryV18.SetQuantity(-1);
				}
			}
			
			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveDeliveryConfig>.Load(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName, config))
				return NULL;
		}

		if (save)
			config.Save(fileName);

		return config;
	}
	
	override void Save(string fileName)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveDeliveryConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName, this);
	}

	override void SetMaxDistance(float max)
	{
		MaxDistance = max;
	}

	override float GetMaxDistance()
	{
		return MaxDistance;
	}

	override void SetMarkerName(string name)
	{
		MarkerName = name;
	}

	override string GetMarkerName()
	{
		return MarkerName;
	}
	
	void CopyConfig(ExpansionQuestObjectiveDeliveryConfig config)
	{
		super.CopyConfig(config);

		MaxDistance = config.MaxDistance;
		MarkerName = config.MarkerName;
	}
};
