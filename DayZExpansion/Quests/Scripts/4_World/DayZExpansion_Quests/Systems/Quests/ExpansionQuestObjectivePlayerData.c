/**
 * ExpansionQuestObjectivePlayerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestTimestampPlayerData
{
	private int QuestID = -1;
	private int Timestamp = -1;

	void SetQuestID(int id)
	{
		QuestID = id;
	}

	int GetQuestID()
	{
		return QuestID;
	}

	void SetTimestamp(int time)
	{
		Timestamp = time;
	}

	int GetTimestamp()
	{
		return Timestamp;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(QuestID);
		ctx.Write(Timestamp);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(QuestID))
			return false;

		if (!ctx.Read(Timestamp))
			return false;

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - QuestID: " + QuestID);
		Print(ToString() + "::QuestDebug - Timestamp: " + Timestamp);
		Print("------------------------------------------------------------");
	#endif
	}
};

class ExpansionQuestObjectivePlayerData
{
	private int QuestID = -1;
	private int ObjectiveIndex = -1;
	private int ObjectiveType = ExpansionQuestObjectiveType.NONE;
	private bool IsCompleted = false;
	private bool IsActive = false;
	private int ObjectiveAmount = -1;
	private int ObjectiveCount = -1;
	private vector ObjectivePosition = vector.Zero;

	void SetQuestID(int id)
	{
		QuestID = id;
	}

	int GetQuestID()
	{
		return QuestID;
	}

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

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(QuestID);
		ctx.Write(ObjectiveIndex);
		ctx.Write(ObjectiveType);
		ctx.Write(IsCompleted);
		ctx.Write(IsActive);
		ctx.Write(ObjectiveAmount);
		ctx.Write(ObjectiveCount);
		ctx.Write(ObjectivePosition);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(QuestID))
			return false;

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

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - QuestID: " + QuestID);
		Print(ToString() + "::QuestDebug - ObjectiveIndex: " + ObjectiveIndex);
		Print(ToString() + "::QuestDebug - ObjectiveType: " + ObjectiveType);
		Print(ToString() + "::QuestDebug - IsCompleted: " + IsCompleted);
		Print(ToString() + "::QuestDebug - IsActive: " + IsActive);
		Print(ToString() + "::QuestDebug - ObjectiveAmount: " + ObjectiveAmount);
		Print(ToString() + "::QuestDebug - ObjectiveCount: " + ObjectiveCount);
		Print(ToString() + "::QuestDebug - ObjectivePosition: " + ObjectivePosition);
		Print("------------------------------------------------------------");
	#endif
	}
};