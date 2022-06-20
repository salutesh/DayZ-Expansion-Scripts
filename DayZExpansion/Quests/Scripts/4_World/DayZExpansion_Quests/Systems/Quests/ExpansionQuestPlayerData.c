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

class ExpansionQuestPlayerDataBase
{
	int ConfigVersion = 0;
}

class ExpansionQuestPlayerDatasBaseV0
{
	ref array<ref ExpansionQuestTimestampPlayerData> QuestTimestamps = new array<ref ExpansionQuestTimestampPlayerData>;
	ref array<ref ExpansionQuestObjectivePlayerDataV0> QuestObjectives = new array<ref ExpansionQuestObjectivePlayerDataV0>; //! Contains quest id with its current presistent quest KeyState
	ref map<int, int> QuestStates = new map<int, int>; //! Contains quest id with its current presistent quest KeyState

	array<ref ExpansionQuestObjectivePlayerDataV0> GetQuestObjectivesByQuestID(int questID)
	{
		array<ref ExpansionQuestObjectivePlayerDataV0> validObjectives = new array<ref ExpansionQuestObjectivePlayerDataV0>;
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectivePlayerDataV0 currentObjective = QuestObjectives[i];
			if (currentObjective.QuestID == questID)
				validObjectives.Insert(currentObjective);
		}

		return validObjectives;
	}

	int GetQuestTimeStampByQuestID(int questID)
	{
		for (int i = 0; i < QuestTimestamps.Count(); i++)
		{
			ExpansionQuestTimestampPlayerData timeStamp = QuestTimestamps[i];
			if (timeStamp && timeStamp.QuestID == questID)
				return timeStamp.Timestamp;
		}

		return -1;
	}
};

//! Pesistent player quest data handled in the quest module
class ExpansionQuestPlayerData: ExpansionQuestPlayerDataBase
{
	ref array<ref ExpansionQuestPresistentPlayerData> QuestDatas = new array<ref ExpansionQuestPresistentPlayerData>;

	[NonSerialized()]
	static int CONFIGVERSION = 1;
	[NonSerialized()]
	private ExpansionQuestModule m_QuestModule;

	void ExpansionQuestPlayerData()
	{
		ConfigVersion = 1;
	}

	void SetQuestModule(ExpansionQuestModule questModule)
	{
		m_QuestModule = questModule;
	}

	void AddQuestData(ExpansionQuestConfig questConfig)
	{
		ExpansionQuestPresistentPlayerData data = new ExpansionQuestPresistentPlayerData();
		data.QuestID = questConfig.GetID();
		QuestDatas.Insert(data);
	}

	bool HasDataForQuest(int questID)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				return true;
			}
		}

		return false;
	}

	int GetQuestStateByQuestID(int questID)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				return data.State;
			}
		}

		return ExpansionQuestState.NONE;
	}

	array<ref ExpansionQuestPresistentPlayerData> GetQuestDatas()
	{
		return QuestDatas;
	}

	int GetQuestTimestampByQuestID(int questID)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				return data.Timestamp;
			}
		}

		return -1;
	}

	array<ref ExpansionQuestObjectivePlayerData> GetQuestObjectivesByQuestID(int questID)
	{
		array<ref ExpansionQuestObjectivePlayerData> validObjectives = new array<ref ExpansionQuestObjectivePlayerData>;
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID != questID)
				continue;

			for (int j = 0; j < data.QuestObjectives.Count(); j++)
			{
				ExpansionQuestObjectivePlayerData currentObjective = data.QuestObjectives[j];
				if (!currentObjective)
					continue;

				validObjectives.Insert(currentObjective);
			}
		}

		return validObjectives;
	}

	ref ExpansionQuestObjectivePlayerData GetQuestObjectiveByQuestIDAndIndex(int questID, int index)
	{
		QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Start");
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - ExpansionQuestPresistentPlayerData: " + data.ToString());
				for (int j = 0; j < data.QuestObjectives.Count(); j++)
				{
					ref ExpansionQuestObjectivePlayerData currentObjective = data.QuestObjectives[j];
					if (!currentObjective)
					{
						QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - F1");
						continue;
					}

					if (currentObjective.GetObjectiveIndex() == index)
					{
						QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return ExpansionQuestObjectivePlayerData: " + currentObjective.ToString());
						return currentObjective;
					}
				}
			}
		}

		QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return ExpansionQuestObjectivePlayerData: NULL");
		return NULL;
	}

	void UpdateQuestState(int questID, int state)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				data.State = state;
			}
		}
	}

	void UpdateQuestTimestamp(int questID, int time)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				data.Timestamp = time;
			}
		}
	}

	void UpdateObjective(int questID, int objectiveIndex, ExpansionQuestObjectivePlayerData newData)
	{
		QuestPrint(ToString() + "::UpdateObjective - Start");
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				for (int j = 0; j < data.QuestObjectives.Count(); j++)
				{
					ExpansionQuestObjectivePlayerData currentObjective = data.QuestObjectives[j];
					if (!currentObjective)
					{
						QuestPrint(ToString() + "::UpdateObjective - F1");
						continue;
					}

					if (currentObjective.GetObjectiveIndex() != objectiveIndex)
					{
						QuestPrint(ToString() + "::UpdateObjective - F2");
						continue;
					}

					data.QuestObjectives.RemoveItem(currentObjective);
				}

				data.QuestObjectives.Insert(newData);
			}
		}
		QuestPrint(ToString() + "::UpdateObjective - End");
	}

	static ExpansionQuestPlayerData Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestPlayerData] Load existing configuration file:" + fileName);

		ExpansionQuestPlayerData playerData;
		ExpansionQuestPlayerDataBase playerDataBase;
		JsonFileLoader<ExpansionQuestPlayerDataBase>.JsonLoadFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", playerDataBase);

		if (playerDataBase.ConfigVersion < CONFIGVERSION)
		{
			//! Version 0 -> 1
			if (playerDataBase.ConfigVersion == 0)
			{
				ExpansionQuestPlayerDatasBaseV0 dataV0;
				JsonFileLoader<ExpansionQuestPlayerDatasBaseV0>.JsonLoadFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", dataV0);
				CF_Log.Info("[ExpansionQuestPlayerData] Convert existing configuration file:" + fileName + " from version " + playerDataBase.ConfigVersion + " to version " + CONFIGVERSION);
				playerData = new ExpansionQuestPlayerData();

				//! Cleanup the file if there is no quest states
				if (dataV0.QuestStates.Count() == 0)
				{
					DeleteFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON");
					return NULL;
				}

				for (int i = 0; i < dataV0.QuestStates.Count(); i++)
				{
					int questID = dataV0.QuestStates.GetKey(i);
					int state = dataV0.QuestStates.GetElement(i);

					ExpansionQuestPresistentPlayerData data = new ExpansionQuestPresistentPlayerData();
					data.QuestID = questID;
					data.State = state;

					if (state > ExpansionQuestState.NONE || state < ExpansionQuestState.COMPLETED)
					{
						//! Objective conversion V0 -> V1
						ref array<ref ExpansionQuestObjectivePlayerDataV0> validObjectives = new array<ref ExpansionQuestObjectivePlayerDataV0>;
						validObjectives = dataV0.GetQuestObjectivesByQuestID(questID);
						for (int j = 0; j < dataV0.GetQuestObjectivesByQuestID(questID).Count(); j++)
						{
							ExpansionQuestObjectivePlayerDataV0 objectiveDataV0 = validObjectives[j];

							ExpansionQuestObjectivePlayerData objectiveData = new ExpansionQuestObjectivePlayerData();
							objectiveData.ObjectiveIndex = objectiveDataV0.ObjectiveIndex;
							objectiveData.ObjectiveType = objectiveDataV0.ObjectiveType;
							objectiveData.IsCompleted = objectiveDataV0.IsCompleted;
							objectiveData.IsActive = objectiveDataV0.IsActive;
							objectiveData.ObjectiveAmount = objectiveDataV0.ObjectiveAmount;
							objectiveData.ObjectiveCount = objectiveDataV0.ObjectiveCount;
							objectiveData.ObjectivePosition = objectiveDataV0.ObjectivePosition;
							data.QuestObjectives.Insert(objectiveData);
						}
					}

					data.Timestamp = dataV0.GetQuestTimeStampByQuestID(questID);
					playerData.QuestDatas.Insert(data);
				}
			}

			//! Copy over old configuration that haven't changed
			playerData.CopyConfig(playerDataBase);
			playerData.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			JsonFileLoader<ExpansionQuestPlayerData>.JsonLoadFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", playerData);
			//! Cleanup the file if there is no quest states and return no data
			if (playerData.QuestDatas.Count() == 0)
			{
				DeleteFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON");
				return NULL;
			}
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestPlayerData>.JsonSaveFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", playerData);
		}

		return playerData;
	}

	void Save(string fileName)
	{
		if (CleanupQuestData()) //! Cleanup all quest states that have no state
		{
			JsonFileLoader<ExpansionQuestPlayerData>.JsonSaveFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", this);
		}
		else
		{
			Error(ToString() + "::Save - Could not save player state data. Something went rly wrong!");
		}
	}

	void CopyConfig(ExpansionQuestPlayerDataBase dataBase)
	{

	}

	private bool CleanupQuestData()
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigByID(data.QuestID);
			if (!questConfig)
			{
				Error(ToString() + "::CleanupQuestData - Could not get quest config for quest ID: " + data.QuestID);
				return false;
			}

			int timestamp;
			if (data.State == ExpansionQuestState.NONE && !questConfig.IsAchivement() && !HasCooldownOnQuest(data.QuestID, timestamp))
			{
				if (questConfig.GetFollowUpQuestID() > 0 || questConfig.GetPreQuestID() > 0)
					continue;
				
				QuestPrint(ToString() + "::CleanupQuestData - Cleanup quest state data for quest with ID:" + data.QuestID + " | State: " + data.State);
				QuestDatas.Remove(i);
			}
		}

		return true;
	}

	bool HasCooldownOnQuest(int questID, out int timestamp)
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas[i];
			if (data.QuestID != questID)
				continue;

			ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigByID(data.QuestID);
			if (!questConfig)
			{
				Error(ToString() + "::CleanupQuestStates - Could not get quest config for quest ID: " + questID);
				return false;
			}

			int daylie = 86400; //! Day in seconds
			int weekly = 604800; //! Week in seconds
			int remaining;

			if (data.Timestamp > 0)
			{
				timestamp = data.Timestamp;
				if (questConfig.IsDailyQuest())
				{
					return HasCooldown(data.QuestID, daylie, remaining);
				}
				else if (questConfig.IsWeeklyQuest())
				{
					return HasCooldown(data.QuestID, weekly, remaining);
				}
			}
		}

		return false;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		int dataCount = QuestDatas.Count();
		ctx.Write(dataCount);
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPresistentPlayerData data = QuestDatas.Get(i);
			data.OnSend(ctx);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!QuestDatas)
		{
			QuestDatas = new array<ref ExpansionQuestPresistentPlayerData>;
		}
		else
		{
			QuestDatas.Clear();
		}

		int objectivesCount;
		if (!ctx.Read(objectivesCount))
			return false;

		for (int i = 0; i < objectivesCount; i++)
		{
			ExpansionQuestPresistentPlayerData data = new ExpansionQuestPresistentPlayerData();
			if (!data.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRecieve - ExpansionQuestPresistentPlayerData");
				return false;
			}

			QuestDatas.Insert(data);
		}

		return true;
	}

	bool HasCooldown(int questID, int cooldown, out int timedif)
	{
		int timestamp = GetQuestTimestampByQuestID(questID);
		if (timestamp == -1)
		{
			QuestPrint(ToString() + "::HasCooldown - Time -1 for quest with ID: " + questID);
			return false;
		}

		timedif = GetTimeDiff(timestamp);
		QuestPrint("------------------------------------------------------------------------------------------------");
		QuestPrint(ToString() + "::HasCooldown - Time for quest with ID: " + timestamp);
		QuestPrint(ToString() + "::HasCooldown - Formated time for quest with ID: " + ExpansionStatic.FormatTime(timestamp, false));
		QuestPrint("------------------------------------------------------------------------------------------------");
		QuestPrint(ToString() + "::HasCooldown - Time difference for quest with ID: " + timedif);
		QuestPrint(ToString() + "::HasCooldown - Formated time difference for quest with ID: " + ExpansionStatic.FormatTime(timedif, false));
		QuestPrint("------------------------------------------------------------------------------------------------");

		if (timedif < cooldown)
			return true;

		return false;
	}

	int GetTimeDiff(int timestamp)
	{
		int currentTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
		return (currentTime - timestamp);
	}

	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print(text);
	#endif
	}

	void QuestDebug()
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			QuestDatas[i].QuestDebug();
		}
	}
};