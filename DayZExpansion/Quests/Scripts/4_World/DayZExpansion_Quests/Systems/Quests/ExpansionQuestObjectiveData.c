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

//! DEPRECATED BUT STILL HERE FOR CONVERSION
class ExpansionQuestTimestampData
{
	int QuestID = -1;
	int Timestamp = -1;
};

//! DEPRECATED BUT STILL HERE FOR CONVERSION
class ExpansionQuestObjectiveDataV0
{
	int QuestID = -1;
	int ObjectiveIndex = -1;
	int ObjectiveType = ExpansionQuestObjectiveType.NONE;
	bool IsCompleted = false;
	bool IsActive = false;
	int ObjectiveAmount = -1;
	int ObjectiveCount = -1;
	vector ObjectivePosition = vector.Zero;
};

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
};

class ExpansionQuestObjectiveData
{
	int ObjectiveIndex = -1;
	int ObjectiveType = ExpansionQuestObjectiveType.NONE;
	bool IsCompleted = false;
	bool IsActive = false;
	int ObjectiveAmount = -1;
	int ObjectiveCount = -1;
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

	int GetObjectiveType()
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
	
	void SetDeliveries(array<ref ExpansionQuestDeliveryObjectiveData> deliveryData)
	{
		if (!DeliveryData)
			DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
		else
			DeliveryData.Clear();
		
		foreach (ExpansionQuestDeliveryObjectiveData data: deliveryData)
		{
			DeliveryData.Insert(data);
		}
		
		Print(ToString() + "::SetDeliveries - Set delivery data: " + DeliveryData.ToString() + " | Count: " + DeliveryData.Count());
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
		ctx.Write(ObjectiveIndex);
		ctx.Write(ObjectiveType);
		ctx.Write(IsCompleted);
		ctx.Write(IsActive);
		ctx.Write(ObjectiveAmount);
		ctx.Write(ObjectiveCount);
		ctx.Write(ObjectivePosition);
		ctx.Write(ActionState);
		ctx.Write(TimeLimit);
		
		if (DeliveryData && ObjectiveType == ExpansionQuestObjectiveType.DELIVERY)
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
		if (!ctx.Read(ObjectiveIndex))
			return false;

		if (!ctx.Read(ObjectiveType))
			return false;

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
		
		if (ObjectiveType == ExpansionQuestObjectiveType.DELIVERY)
		{
			if (!DeliveryData)
				DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
			
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
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		CF_Log.Debug("------------------------------------------------------------");
		CF_Log.Debug(ToString() + "::QuestDebug - ObjectiveIndex: " + ObjectiveIndex);
		CF_Log.Debug(ToString() + "::QuestDebug - ObjectiveType: " + ObjectiveType);
		CF_Log.Debug(ToString() + "::QuestDebug - IsCompleted: " + IsCompleted);
		CF_Log.Debug(ToString() + "::QuestDebug - IsActive: " + IsActive);
		CF_Log.Debug(ToString() + "::QuestDebug - ObjectiveAmount: " + ObjectiveAmount);
		CF_Log.Debug(ToString() + "::QuestDebug - ObjectiveCount: " + ObjectiveCount);
		CF_Log.Debug(ToString() + "::QuestDebug - ObjectivePosition: " + ObjectivePosition);
		CF_Log.Debug(ToString() + "::QuestDebug - ActionState: " + ActionState);
		CF_Log.Debug(ToString() + "::QuestDebug - TimeLimit: " + TimeLimit);
		CF_Log.Debug("------------------------------------------------------------");
	#endif
	}
};