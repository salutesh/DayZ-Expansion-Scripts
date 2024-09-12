/**
 * ExpansionQuestObjectiveData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestDeliveryObjectiveData
{
	int Index;
	int Count;

	void SetFromDelivery(int index, int count)
	{
		Index = index;
		Count = count;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(Index);
		ctx.Write(Count);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(Index))
			return false;

		if (!ctx.Read(Count))
			return false;

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Index: " + Index);
		Print(ToString() + "::QuestDebug - Count: " + Count);
		Print("------------------------------------------------------------");
	#endif
	}
};

class ExpansionQuestObjectiveData
{
	int ObjectiveIndex = -1;
	ExpansionQuestObjectiveType ObjectiveType = ExpansionQuestObjectiveType.NONE;
	bool IsCompleted = false;
	bool IsActive = false;
	int ObjectiveAmount;
	int ObjectiveCount;
	vector ObjectivePosition = vector.Zero;

	bool ActionState = false;
	int TimeLimit = -1;

	ref array<ref ExpansionQuestDeliveryObjectiveData> DeliveryData;

	void SetObjectiveIndex(int index)
	{
		ObjectiveIndex = index;
	}

	int GetObjectiveIndex()
	{
		return ObjectiveIndex;
	}

	void SetObjectiveType(int type)
	{
		ObjectiveType =type;
	}

	ExpansionQuestObjectiveType GetObjectiveType()
	{
		return ObjectiveType;
	}

	void SetCompleted(bool state)
	{
		IsCompleted = state;
	}

	bool IsCompleted()
	{
		return IsCompleted;
	}

	void SetActive(bool state)
	{
		IsActive = state;
	}

	bool IsActive()
	{
		return IsActive;
	}

	void SetObjectiveAmount(int count)
	{
		ObjectiveAmount = count;
	}

	int GetObjectiveAmount()
	{
		return ObjectiveAmount;
	}

	void SetObjectiveCount(int count)
	{
		ObjectiveCount = count;
	}

	int GetObjectiveCount()
	{
		return ObjectiveCount;
	}

	void SetObjectivePosition(vector pos)
	{
		ObjectivePosition = pos;
	}

	vector GetObjectivePosition()
	{
		return ObjectivePosition;
	}

	void SetActionState(bool state)
	{
		ActionState = state;
	}

	bool GetActionState()
	{
		return ActionState;
	}

	void SetTimeLimit(int time)
	{
		TimeLimit = time;
	}

	int GetTimeLimit()
	{
		return TimeLimit;
	}
	
	void Copy(ExpansionQuestObjectiveData copyData)
	{
		ObjectiveIndex = copyData.ObjectiveIndex;
		ObjectiveType = copyData.ObjectiveType;
		IsCompleted = copyData.IsCompleted;
		IsActive = copyData.IsActive;
		ObjectiveAmount = copyData.ObjectiveAmount;
		ObjectiveCount = copyData.ObjectiveCount;
		ObjectivePosition = copyData.ObjectivePosition;
		ActionState = copyData.ActionState;
		TimeLimit = copyData.TimeLimit;
		DeliveryData = copyData.DeliveryData;
	}

	void SetDeliveries(array<ref ExpansionQuestDeliveryObjectiveData> deliveryData)
	{
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!DeliveryData)
			DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
		else
			DeliveryData.Clear();

		foreach (ExpansionQuestDeliveryObjectiveData data: deliveryData)
		{
			DeliveryData.Insert(data);
		}

	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		EXTrace.Print(EXTrace.QUESTS, this, "Set delivery data: " + DeliveryData.ToString() + " | Count: " + DeliveryData.Count());
	#endif
	}

	int GetDeliveryCountByIndex(int index)
	{
		if (!DeliveryData)
			return 0;

		foreach (ExpansionQuestDeliveryObjectiveData data: DeliveryData)
		{
			if (data.Index == index)
				return data.Count;
		}

		return 0;
	}

	void OnSend(ParamsWriteContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, typename.EnumToString(ExpansionQuestObjectiveType, ObjectiveType));
#endif

		ctx.Write(ObjectiveIndex);
		ctx.Write(ObjectiveType);
		ctx.Write(IsCompleted);
		ctx.Write(IsActive);
		ctx.Write(ObjectiveAmount);
		ctx.Write(ObjectiveCount);
		ctx.Write(ObjectivePosition);
		ctx.Write(ActionState);
		ctx.Write(TimeLimit);

		if (DeliveryData && (ObjectiveType == ExpansionQuestObjectiveType.DELIVERY || ObjectiveType == ExpansionQuestObjectiveType.COLLECT))
		{
			int deliveryCount = DeliveryData.Count();
			ctx.Write(deliveryCount);

			for (int i = 0; i < deliveryCount; ++i)
			{
				ExpansionQuestDeliveryObjectiveData data = DeliveryData[i];
				data.OnSend(ctx);
			}
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		if (!ctx.Read(ObjectiveIndex))
			return false;

		if (!ctx.Read(ObjectiveType))
			return false;

#ifdef EXTRACE
		EXTrace.Add(trace, typename.EnumToString(ExpansionQuestObjectiveType, ObjectiveType));
#endif
		
		if (!ctx.Read(IsCompleted))
			return false;

		if (!ctx.Read(IsActive))
			return false;

		if (!ctx.Read(ObjectiveAmount))
			return false;

		if (!ctx.Read(ObjectiveCount))
			return false;

		if (!ctx.Read(ObjectivePosition))
			return false;

		if (!ctx.Read(ActionState))
			return false;

		if (!ctx.Read(TimeLimit))
			return false;

		if (ObjectiveType == ExpansionQuestObjectiveType.DELIVERY || ObjectiveType == ExpansionQuestObjectiveType.COLLECT)
		{
			if (!DeliveryData)
				DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
			else
				DeliveryData.Clear();

			int deliveryCount;
			ctx.Read(deliveryCount);

			for (int i = 0; i < deliveryCount; ++i)
			{
				ExpansionQuestDeliveryObjectiveData data = new ExpansionQuestDeliveryObjectiveData();
				if (!data.OnRecieve(ctx))
					return false;

				DeliveryData.Insert(data);
			}
		}

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - ObjectiveIndex: " + ObjectiveIndex);
		Print(ToString() + "::QuestDebug - ObjectiveType: " + ObjectiveType);
		Print(ToString() + "::QuestDebug - IsCompleted: " + IsCompleted);
		Print(ToString() + "::QuestDebug - IsActive: " + IsActive);
		Print(ToString() + "::QuestDebug - ObjectiveAmount: " + ObjectiveAmount);
		Print(ToString() + "::QuestDebug - ObjectiveCount: " + ObjectiveCount);
		Print(ToString() + "::QuestDebug - ObjectivePosition: " + ObjectivePosition);
		Print(ToString() + "::QuestDebug - ActionState: " + ActionState);
		Print(ToString() + "::QuestDebug - TimeLimit: " + TimeLimit);
		Print(ToString() + "::QuestDebug - Deliveries:");
		if (ObjectiveType == ExpansionQuestObjectiveType.DELIVERY || ObjectiveType == ExpansionQuestObjectiveType.COLLECT)
		{
			if (!DeliveryData || DeliveryData.Count() == 0)
				return;
			foreach (ExpansionQuestDeliveryObjectiveData delivery: DeliveryData)
			{
				delivery.QuestDebug();
			}
		}
		Print("------------------------------------------------------------");
	#endif
	}
};