/**
 * ExpansionQuestPersistentQuestData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestPersistentQuestData
{
	int QuestID = -1;
	ExpansionQuestState State = ExpansionQuestState.NONE;
	int Timestamp = -1;
	ref array<ref ExpansionQuestObjectiveData> QuestObjectives = new array<ref ExpansionQuestObjectiveData>;
	int LastUpdateTime = -1;
	int CompletionCount = 0;

	ExpansionQuestObjectiveData GetObjectiveByIndex(int index)
	{
		foreach (ExpansionQuestObjectiveData objective: QuestObjectives)
		{
			if (objective && objective.GetObjectiveIndex() == index)
				return objective;
		}

		return NULL;
	}

	void ClearObjectiveData()
	{
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		
		QuestObjectives.Clear();
	}
	
	array<ref ExpansionQuestObjectiveData> GetQuestObjectives()
	{
		return QuestObjectives;
	}

	void OnWrite(ParamsWriteContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Quest ID: " + QuestID + " " + typename.EnumToString(ExpansionQuestState, State));
#endif

		ctx.Write(QuestID);
		ctx.Write(State);
		ctx.Write(Timestamp);

		//! Quest objective states
		int objectivesCount = QuestObjectives.Count();
		ctx.Write(objectivesCount);
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveData objective = QuestObjectives.Get(i);
			if (objective)
				objective.OnSend(ctx);
		}

		ctx.Write(LastUpdateTime);
		ctx.Write(CompletionCount);
	}

	bool OnRead_V1(ParamsReadContext ctx)
	{
		if (!ctx.Read(QuestID))
		{
			Error(ToString() + "::OnRead - QuestID");
			return false;
		}

		if (!ctx.Read(State))
		{
			Error(ToString() + "::OnRead - State");
			return false;
		}

		if (!ctx.Read(Timestamp))
		{
			Error(ToString() + "::OnRead - Timestamp");
			return false;
		}

		int objectivesCount;
		if (!ctx.Read(objectivesCount))
		{
			Error(ToString() + "::OnRead - objectivesCount");
			return false;
		}

		if (QuestObjectives)
			QuestObjectives.Clear();

		for (int i = 0; i < objectivesCount; i++)
		{
			ExpansionQuestObjectiveData objective = new ExpansionQuestObjectiveData();
			if (!objective.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRead - objective");
				return false;
			}

			QuestObjectives.Insert(objective);
		}

		return true;
	}

	bool OnRead(ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		if (!OnRead_V1(ctx))
			return false;

#ifdef EXTRACE
		EXTrace.Add(trace, "Quest ID: " + QuestID);
		EXTrace.Add(trace, typename.EnumToString(ExpansionQuestState, State));
#endif
		
		if (!ctx.Read(LastUpdateTime))
		{
			Error(ToString() + "::OnRead - LastUpdateTime");
			return false;
		}

		if (!ctx.Read(CompletionCount))
		{
			Error(ToString() + "::OnRead - CompletionCount");
			return false;
		}

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Quest ID: " + QuestID);
		string stateName = typename.EnumToString(ExpansionQuestState, State);
		Print(ToString() + "::QuestDebug - Quest State: " + stateName);
		Print(ToString() + "::QuestDebug - Quest Timestamp: " + Timestamp);
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			QuestObjectives[i].QuestDebug();
		}
		
		CF_Date date = CF_Date.Epoch(LastUpdateTime);
		Print(ToString() + "::QuestDebug - Quest LastUpdateTime (UTC): " + date.Format(CF_Date.DATETIME));
		Print(ToString() + "::QuestDebug - Quest CompletionCount: " + CompletionCount);
		Print("------------------------------------------------------------");
	#endif
	}

	void UpdateLastUpdateTime()
	{
		LastUpdateTime = CF_Date.Now(true).GetTimestamp();
	}
};