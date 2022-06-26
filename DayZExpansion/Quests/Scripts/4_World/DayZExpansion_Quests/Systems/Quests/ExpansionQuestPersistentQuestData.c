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
	int State = ExpansionQuestState.NONE;
	int Timestamp = -1;
	ref array<ref ExpansionQuestObjectiveData> QuestObjectives = new array<ref ExpansionQuestObjectiveData>;
	int LastUpdateTime;
		
	void OnSend(ParamsWriteContext ctx)
	{
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
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(QuestID))
		{
			Error(ToString() + "::OnRecieve - QuestID");
			return false;
		}

		if (!ctx.Read(State))
		{
			Error(ToString() + "::OnRecieve - State");
			return false;
		}

		if (!ctx.Read(Timestamp))
		{
			Error(ToString() + "::OnRecieve - Timestamp");
			return false;
		}

		int objectivesCount;
		if (!ctx.Read(objectivesCount))
		{
			Error(ToString() + "::OnRecieve - objectivesCount");
			return false;
		}

		if (!QuestObjectives)
		{
			QuestObjectives = new array<ref ExpansionQuestObjectiveData>;
		}
		else
		{
			QuestObjectives.Clear();
		}

		for (int i = 0; i < objectivesCount; i++)
		{
			ExpansionQuestObjectiveData objective = new ExpansionQuestObjectiveData();
			if (!objective.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRecieve - objective");
				return false;
			}

			QuestObjectives.Insert(objective);
		}


		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Quest ID: " + QuestID);
		Print(ToString() + "::QuestDebug - Quest State: " + State);
		Print(ToString() + "::QuestDebug - Quest Timestamp: " + Timestamp);
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			QuestObjectives[i].QuestDebug();
		}
		Print("------------------------------------------------------------");
	#endif
	}
};