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
	autoptr ExpansionQuestObjectiveCollection Collection = new ExpansionQuestObjectiveCollection();
};

class ExpansionQuestObjectiveCollectionConfigBase: ExpansionQuestObjectiveDeliveryConfigBase 
{
	bool NeedAnyCollection = false;
};

class ExpansionQuestObjectiveCollectionConfig: ExpansionQuestObjectiveCollectionConfigBase
{
	override bool NeedAnyCollection()
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
			
			if (configBase.ConfigVersion < 19)
			{
				foreach (ExpansionQuestObjectiveDelivery deliveryV18: config.Collections)
				{
					deliveryV18.SetQuantity(-1);
				}
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
	
	void CopyConfig(ExpansionQuestObjectiveCollectionConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		ShowDistance = configBase.ShowDistance;

		Collections.Clear();
		foreach (ExpansionQuestObjectiveDelivery collection: configBase.Collections)
		{
			ExpansionQuestObjectiveDelivery collectionCopy = new ExpansionQuestObjectiveDelivery();
			collectionCopy.Copy(collection);
			Collections.Insert(collectionCopy);
		}
		
		NeedAnyCollection = configBase.NeedAnyCollection;
	}

	override void Save(string fileName)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
		
		ExpansionJsonFileParser<ExpansionQuestObjectiveCollectionConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName, this);
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		ctx.Write(NeedAnyCollection);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;
		
		if (!ctx.Read(NeedAnyCollection))
			return false;

		return true;
	}
};
