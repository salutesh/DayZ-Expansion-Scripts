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

class ExpansionQuestObjectiveDeliveryConfigBase:ExpansionQuestObjectiveConfig
{
	autoptr array<ref ExpansionQuestObjectiveDelivery> Deliveries = new array<ref ExpansionQuestObjectiveDelivery>;
	float MaxDistance = 0;
	string MarkerName = string.Empty;
};

class ExpansionQuestObjectiveDeliveryConfig: ExpansionQuestObjectiveDeliveryConfigBase
{
	bool ShowDistance = true;

	void AddDelivery(int amount, string name)
	{
		ExpansionQuestObjectiveDelivery delivery = new ExpansionQuestObjectiveDelivery();
		delivery.SetAmount(amount);
		delivery.SetClassName(name);
		Deliveries.Insert(delivery);
	}

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

	override array<ref ExpansionQuestObjectiveDelivery> GetDeliveries()
	{
		return Deliveries;
	}

	static ExpansionQuestObjectiveDeliveryConfig Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestObjectiveDeliveryConfig] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveDeliveryConfig config;
		ExpansionQuestObjectiveDeliveryConfigBase configBase;

		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveDeliveryConfigBase>.Load(fileName, configBase))
			return NULL;

		if (configBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestObjectiveDeliveryConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			config = new ExpansionQuestObjectiveDeliveryConfig();

			//! Copy over old configuration that haven't changed
			config.CopyConfig(configBase);

			config.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestObjectiveDeliveryConfig>.Load(fileName, config))
				return NULL;
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestObjectiveDeliveryConfig>.JsonSaveFile(fileName, config);
		}

		return config;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveDeliveryConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName + ".json", this);
	}

	void CopyConfig(ExpansionQuestObjectiveDeliveryConfigBase configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
		Deliveries = configBase.Deliveries;
		MaxDistance = configBase.MaxDistance;
		MarkerName = configBase.MarkerName;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		int deliveryCount = Deliveries.Count();
		ctx.Write(deliveryCount);

		foreach (ExpansionQuestObjectiveDelivery delivery: Deliveries)
		{
			delivery.OnSend(ctx);
		}

		ctx.Write(MaxDistance);
		ctx.Write(MarkerName);
		ctx.Write(ShowDistance);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!Deliveries)
			Deliveries = new array<ref ExpansionQuestObjectiveDelivery>;
		else
			Deliveries.Clear();

		int deliveryCount;
		if (!ctx.Read(deliveryCount))
			return false;

		for (int i = 0; i < deliveryCount; i++)
		{
			ExpansionQuestObjectiveDelivery delivery = new ExpansionQuestObjectiveDelivery();
			if (!delivery.OnRecieve(ctx))
				return false;

			Deliveries.Insert(delivery);
		}

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

		if (!Deliveries || Deliveries && Deliveries.Count() == 0)
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
		for (int i = 0; i < Deliveries.Count(); i++)
		{
			ExpansionQuestObjectiveDelivery delivery = Deliveries[i];
			if (delivery)
				delivery.QuestDebug();
		}
	#endif
	}
};