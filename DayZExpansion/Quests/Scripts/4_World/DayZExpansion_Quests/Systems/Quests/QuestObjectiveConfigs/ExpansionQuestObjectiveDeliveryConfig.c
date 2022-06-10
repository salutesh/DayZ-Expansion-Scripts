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

class ExpansionQuestObjectiveDeliveryConfig: ExpansionQuestObjectiveConfigBase
{
	private autoptr array<ref ExpansionQuestObjectiveDelivery> Deliveries = new array<ref ExpansionQuestObjectiveDelivery>;
	private vector Position = vector.Zero;
	private float MaxDistance = 0;
	private string MarkerName = string.Empty;
	
	void AddDelivery(int amount, string name)
	{
		ExpansionQuestObjectiveDelivery delivery = new ExpansionQuestObjectiveDelivery();
		delivery.SetAmount(amount);
		delivery.SetClassName(name);
		Deliveries.Insert(delivery);
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

	override float GetMaxDistance()
	{
		return MaxDistance;
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

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveDeliveryConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER + fileName + ".JSON", this);
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
		
		ctx.Write(Position);
		ctx.Write(MaxDistance);
		ctx.Write(MarkerName);
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
		
		if (!ctx.Read(Position))
			return false;

		if (!ctx.Read(MaxDistance))
			return false;
		
		if (!ctx.Read(MarkerName))
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		for (int i = 0; i < Deliveries.Count(); i++)
		{
			ExpansionQuestObjectiveDelivery delivery = Deliveries[i];
			if (delivery)
				delivery.QuestDebug();
		}
	#endif
	}
};