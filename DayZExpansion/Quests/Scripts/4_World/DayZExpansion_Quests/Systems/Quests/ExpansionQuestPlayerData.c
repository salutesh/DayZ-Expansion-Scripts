/**
 * ExpansionQuestPlayerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Pesistent player quest data handled in the quest module
class ExpansionQuestPlayerData
{
	private ref array<ref ExpansionQuestTimestampPlayerData> QuestTimestamps = new array<ref ExpansionQuestTimestampPlayerData>;
	private ref array<ref ExpansionQuestObjectivePlayerData> QuestObjectives = new array<ref ExpansionQuestObjectivePlayerData>; //! Contains quest id with its current presistent quest KeyState
	private ref map<int, int> QuestStates = new map<int, int>; //! Contains quest id with its current presistent quest KeyState

	[NonSerialized()]
	private ExpansionQuestModule m_QuestModule;

	void SetQuestModule(ExpansionQuestModule questModule)
	{
		m_QuestModule = questModule;
	}

	int GetQuestStateByQuestID(int questID)
	{
		int questState;
		if (QuestStates.Find(questID, questState))
			return questState;

		return ExpansionQuestState.NONE;
	}

	map<int, int> GetQuestStates()
	{
		return QuestStates;
	}

	int GetQuestTimestampByQuestID(int questID)
	{
		for (int i = 0; i < QuestTimestamps.Count(); i++)
		{
			ExpansionQuestTimestampPlayerData currentTimestamp = QuestTimestamps[i];
			if (!currentTimestamp)
			{
				Error(ToString() + "::GetQuestTimestampByQuestID - Could not get timestamp player data for quest ID: " + questID);
				return -1;
			}

			if (currentTimestamp.GetQuestID() == questID)
				return currentTimestamp.GetTimestamp();
		}

		return -1;
	}

	array<ref ExpansionQuestTimestampPlayerData> GetQuestTimestamps()
	{
		return QuestTimestamps;
	}

	array<ref ExpansionQuestObjectivePlayerData> GetQuestObjectivesByQuestID(int questID)
	{
		array<ref ExpansionQuestObjectivePlayerData> validObjectives = new array<ref ExpansionQuestObjectivePlayerData>;
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectivePlayerData currentObjective = QuestObjectives[i];
			if (currentObjective && currentObjective.GetQuestID() == questID)
				validObjectives.Insert(currentObjective);
		}

		return validObjectives;
	}

	void UpdateQuestState(int questID, int state)
	{
		int questState;
		if (QuestStates.Find(questID, questState))
			QuestStates.Set(questID, state);
		else
			QuestStates.Insert(questID, state);
	}

	void UpdateQuestTimestamp(int questID, int time)
	{
		bool hasEntry = false;
		for (int i = 0; i < QuestTimestamps.Count(); i++)
		{
			ExpansionQuestTimestampPlayerData currentTimestamp = QuestTimestamps[i];
			if (!currentTimestamp)
			{
				Error(ToString() + "::UpdateQuestTimestamp - Could not get timestamp player data for quest ID: " + questID);
				continue;
			}

			if (currentTimestamp && currentTimestamp.GetQuestID() == questID)
			{
				currentTimestamp.SetTimestamp(time);
				hasEntry = true;
			}
		}

		if (!hasEntry)
		{
			ExpansionQuestTimestampPlayerData newTimestamp = new ExpansionQuestTimestampPlayerData();
			newTimestamp.SetQuestID(questID);
			newTimestamp.SetTimestamp(time);
			QuestTimestamps.Insert(newTimestamp);
		}
	}

	void UpdateObjective(int questID, int objectiveIndex, ExpansionQuestObjectivePlayerData newData)
	{
		foreach (ExpansionQuestObjectivePlayerData currentObjective: QuestObjectives)
		{
			if (currentObjective)
			{
				if (currentObjective.GetQuestID() != questID)
					continue;

				if (currentObjective.GetObjectiveIndex() != objectiveIndex)
					continue;

				QuestObjectives.RemoveItem(currentObjective);
			}
		}

		QuestObjectives.Insert(newData);
	}

	array<ref ExpansionQuestObjectivePlayerData> GetQuestObjectives()
	{
		return QuestObjectives;
	}

	void Save(string fileName)
	{
		if (CleanupQuestStates()) //! Cleanup all quest states that have no state
		{
			JsonFileLoader<ExpansionQuestPlayerData>.JsonSaveFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", this);
		}
		else
		{
			Error(ToString() + "::Save - Could not save player state data. Something went rly wrong!");
		}
	}

	private bool CleanupQuestStates()
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		foreach (int questID, int state: QuestStates)
		{
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::CleanupQuestStates - Check quest with ID:" + questID + " | State: " + state);
		#endif

			ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigByID(questID);
			if (!questConfig)
			{
				Error(ToString() + "::CleanupQuestStates - Could not get quest config for quest ID: " + questID);
				return false;
			}

			if (questConfig.IsAchivement())
				continue;

			ExpansionQuestTimestampPlayerData timestamp;
			if (state == ExpansionQuestState.NONE || state == ExpansionQuestState.COMPLETED && questConfig.IsRepeatable() && !HasCooldownOnQuest(questID, timestamp) && !questConfig.IsAchivement())
			{
			#ifdef EXPANSIONMODQUESTSMODULEDEBUG
				Print(ToString() + "::CleanupQuestStates - Cleanup quest state data for quest with ID:" + questID + " | State: " + state);
			#endif

				for (int j = 0; j < QuestObjectives.Count(); j++)
				{
					ExpansionQuestObjectivePlayerData currentObjective = QuestObjectives[j];
					if (!currentObjective)
					{
						Error(ToString() + "::CleanupQuestStates - Could not get players quest objective data!");
						return false;
					}

					if (currentObjective.GetQuestID() == questID)
					{
						QuestObjectives.Remove(j);
						j--;
					}
				}

				QuestStates.Remove(questID);
			}
		}

		return true;
	}

	bool HasCooldownOnQuest(int questID, out ExpansionQuestTimestampPlayerData timestamp)
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigByID(questID);
		if (!questConfig)
		{
			Error(ToString() + "::HasCooldownOnQuest - Could not get quest config for quest ID: " + questID);
			return false;
		}

		int daylie = 86400; //! Day in seconds
		int weekly = 604800; //! Week in seconds
		int remaining;

		foreach (ExpansionQuestTimestampPlayerData stamp: QuestTimestamps)
		{
			timestamp = stamp;
			if (timestamp.GetQuestID() == questID)
			{
				if (questConfig.IsDailyQuest())
				{
					return HasCooldown(questConfig.GetID(), daylie, remaining);
				}
				else if (questConfig.IsWeeklyQuest())
				{
					return HasCooldown(questConfig.GetID(), weekly, remaining);
				}
			}
		}

		return false;
	}

	static ExpansionQuestPlayerData LoadPlayerQuestData(string fileName)
	{
		ExpansionQuestPlayerData data = new ExpansionQuestPlayerData();
		JsonFileLoader<ExpansionQuestPlayerData>.JsonLoadFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".json", data);

		return data;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		int i;

		//! Quest objective states
		int objectivesCount = QuestObjectives.Count();
		ctx.Write(objectivesCount);

		for (i = 0; i < QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectivePlayerData objective = QuestObjectives.Get(i);
			if (objective)
				objective.OnSend(ctx);
		}

		//! Quest states
		int statesCount = QuestStates.Count();
		ctx.Write(statesCount);

		for (i = 0; i < QuestStates.Count(); i++)
		{
			int idc = QuestStates.GetKey(i);
			int state = QuestStates.GetElement(i);

			ctx.Write(idc);
			ctx.Write(state);
		}

		//! Quest cooldowns
		int cooldownCount = QuestTimestamps.Count();
		ctx.Write(cooldownCount);

		for (i = 0; i < QuestTimestamps.Count(); i++)
		{
			ExpansionQuestTimestampPlayerData timestampData = QuestTimestamps.Get(i);
			if (timestampData)
				timestampData.OnSend(ctx);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		int i;

		int objectivesCount;
		if (!ctx.Read(objectivesCount))
		{
			Error(ToString() + "::OnRecieve - objectivesCount");
			return false;
		}

		if (!QuestObjectives)
		{
			QuestObjectives = new array<ref ExpansionQuestObjectivePlayerData>;
		}
		else
		{
			QuestObjectives.Clear();
		}

		for (i = 0; i < objectivesCount; i++)
		{
			ExpansionQuestObjectivePlayerData objective = new ExpansionQuestObjectivePlayerData();
			if (!objective.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRecieve - objective");
				return false;
			}

			QuestObjectives.Insert(objective);
		}

		int statesCount;
		if (!ctx.Read(statesCount))
		{
			Error(ToString() + "::OnRecieve - statesCount");
			return false;
		}

		if (!QuestStates)
		{
			QuestStates = new map<int, int>;
		}
		else
		{
			QuestStates.Clear();
		}

		for (i = 0; i < statesCount; i++)
		{
			int idc;
			if (!ctx.Read(idc))
			{
				Error(ToString() + "::OnRecieve - idc!");
				return false;
			}

			int state;
			if (!ctx.Read(state))
			{
				Error(ToString() + "::OnRecieve - state!");
				return false;
			}

			QuestStates.Insert(idc, state);
		}

		int cooldownCount;
		if (!ctx.Read(cooldownCount))
		{
			Error(ToString() + "::OnRecieve - cooldownCount");
			return false;
		}

		if (!QuestTimestamps)
		{
			QuestTimestamps = new array<ref ExpansionQuestTimestampPlayerData>;
		}
		else
		{
			QuestTimestamps.Clear();
		}

		for (i = 0; i < cooldownCount; i++)
		{
			ExpansionQuestTimestampPlayerData timestamp = new ExpansionQuestTimestampPlayerData();
			if (!timestamp.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRecieve - timestamp");
				return false;
			}

			QuestTimestamps.Insert(timestamp);
		}

		return true;
	}

	bool HasCooldown(int questID, int cooldown, out int timedif)
	{
		int timestamp = GetQuestTimestampByQuestID(questID);
		if (timestamp == -1)
		{
			Print(ToString() + "::HasCooldown - Time -1 for quest with ID: " + questID);
		}

		timedif = GetTimeDiff(timestamp);
		if (timedif < cooldown)
			return true;

		return false;
	}

	int GetTimeDiff(int timestamp)
	{
		int currentTime = CF_Date.Now(true).GetTimestamp();
		return (currentTime - timestamp);
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		int i;

		foreach (ExpansionQuestTimestampPlayerData currentTimestamp: QuestTimestamps)
		{
			currentTimestamp.QuestDebug();
		}

		foreach (ExpansionQuestObjectivePlayerData currentObjective: QuestObjectives)
		{
			currentObjective.QuestDebug();
		}

		foreach (int questID, int state: QuestStates)
		{
			Print("------------------------------------------------------------");
			Print(ToString() + "::QuestDebug - Quest ID: " + questID);
			Print(ToString() + "::QuestDebug - Quest state: " + state);
			Print("------------------------------------------------------------");
		}
	#endif
	}
};