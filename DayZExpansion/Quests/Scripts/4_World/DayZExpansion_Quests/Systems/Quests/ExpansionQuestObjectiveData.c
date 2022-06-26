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

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
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
		Print("------------------------------------------------------------");
	#endif
	}
};