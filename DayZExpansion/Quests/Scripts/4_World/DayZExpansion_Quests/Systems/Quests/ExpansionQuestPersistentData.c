/**
 * ExpansionQuestPersistentData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestPersistentDataBase
{
	int ConfigVersion = 0;
}

class ExpansionQuestPersistentDatasBaseV0
{
	ref array<ref ExpansionQuestTimestampData> QuestTimestamps = new array<ref ExpansionQuestTimestampData>;
	ref array<ref ExpansionQuestObjectiveDataV0> QuestObjectives = new array<ref ExpansionQuestObjectiveDataV0>; //! Contains quest id with its current persistent quest KeyState
	ref map<int, int> QuestStates = new map<int, int>; //! Contains quest id with its current persistent quest KeyState

	array<ref ExpansionQuestObjectiveDataV0> GetQuestObjectivesByQuestID(int questID)
	{
		array<ref ExpansionQuestObjectiveDataV0> validObjectives = new array<ref ExpansionQuestObjectiveDataV0>;
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveDataV0 currentObjective = QuestObjectives[i];
			if (currentObjective.QuestID == questID)
				validObjectives.Insert(currentObjective);
		}

		return validObjectives;
	}

	int GetQuestTimeStampByQuestID(int questID)
	{
		for (int i = 0; i < QuestTimestamps.Count(); i++)
		{
			ExpansionQuestTimestampData timeStamp = QuestTimestamps[i];
			if (timeStamp && timeStamp.QuestID == questID)
				return timeStamp.Timestamp;
		}

		return -1;
	}
};

//! Pesistent player quest data handled in the quest module
class ExpansionQuestPersistentData: ExpansionQuestPersistentDataBase
{
	[NonSerialized()]
	static int CONFIGVERSION = 1;
	[NonSerialized()]
	private ExpansionQuestModule m_QuestModule;

	ref array<ref ExpansionQuestPersistentQuestData> QuestDatas = new array<ref ExpansionQuestPersistentQuestData>;
	
	void ExpansionQuestPersistentData()
	{
		ConfigVersion = 1;
	}

	void SetQuestModule(ExpansionQuestModule questModule)
	{
		m_QuestModule = questModule;
	}

	void AddQuestData(ExpansionQuestConfig questConfig)
	{
		ExpansionQuestPersistentQuestData data = new ExpansionQuestPersistentQuestData();
		data.QuestID = questConfig.GetID();
		QuestDatas.Insert(data);
	}

	bool HasDataForQuest(int questID)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
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
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				return data.State;
			}
		}

		return ExpansionQuestState.NONE;
	}
	
	ExpansionQuestPersistentQuestData GetQuestDataByQuestID(int questID)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				return data;
			}
		}

		return NULL;
	}

	array<ref ExpansionQuestPersistentQuestData> GetQuestDatas()
	{
		return QuestDatas;
	}

	int GetQuestTimestampByQuestID(int questID)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				return data.Timestamp;
			}
		}

		return -1;
	}

	array<ref ExpansionQuestObjectiveData> GetQuestObjectivesByQuestID(int questID)
	{
		array<ref ExpansionQuestObjectiveData> validObjectives = new array<ref ExpansionQuestObjectiveData>;
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID != questID)
				continue;

			for (int j = 0; j < data.QuestObjectives.Count(); j++)
			{
				ExpansionQuestObjectiveData currentObjective = data.QuestObjectives[j];
				if (!currentObjective)
					continue;

				validObjectives.Insert(currentObjective);
			}
		}

		return validObjectives;
	}

	ref ExpansionQuestObjectiveData GetQuestObjectiveByQuestIDAndIndex(int questID, int index)
	{
		QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Start");
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - ExpansionQuestPersistentQuestData: " + data.ToString());
				for (int j = 0; j < data.QuestObjectives.Count(); j++)
				{
					ref ExpansionQuestObjectiveData currentObjective = data.QuestObjectives[j];
					if (!currentObjective)
					{
						QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - F1");
						continue;
					}

					if (currentObjective.GetObjectiveIndex() == index)
					{
						QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return ExpansionQuestObjectiveData: " + currentObjective.ToString());
						return currentObjective;
					}
				}
			}
		}

		QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return ExpansionQuestObjectiveData: NULL");
		return NULL;
	}

	void UpdateQuestState(int questID, int state)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				data.State = state;
				data.LastUpdateTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
			}
		}
	}

	void UpdateQuestTimestamp(int questID, int time)
	{
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				data.Timestamp = time;
				data.LastUpdateTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
			}
		}
	}

	void UpdateObjective(int questID, int objectiveIndex, ExpansionQuestObjectiveData newData)
	{
		QuestPrint(ToString() + "::UpdateObjective - Start");
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (data.QuestID == questID)
			{
				for (int j = 0; j < data.QuestObjectives.Count(); j++)
				{
					ExpansionQuestObjectiveData currentObjective = data.QuestObjectives[j];
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

				data.LastUpdateTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
				data.QuestObjectives.Insert(newData);
			}
		}
		QuestPrint(ToString() + "::UpdateObjective - End");
	}

	static ExpansionQuestPersistentData Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestPersistentData] Load existing configuration file:" + fileName);

		ExpansionQuestPersistentData playerData;
		ExpansionQuestPersistentDataBase playerDataBase;
		JsonFileLoader<ExpansionQuestPersistentDataBase>.JsonLoadFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", playerDataBase);

		if (playerDataBase.ConfigVersion < CONFIGVERSION)
		{			
			//! Copy over old configuration that haven't changed
			playerData.CopyConfig(playerDataBase);
			playerData.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			JsonFileLoader<ExpansionQuestPersistentData>.JsonLoadFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", playerData);
			//! Cleanup the file if there is no quest states and return no data
			if (playerData.QuestDatas.Count() == 0)
			{
				DeleteFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON");
				return NULL;
			}
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestPersistentData>.JsonSaveFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", playerData);
		}

		return playerData;
	}

	void Save(string fileName)
	{
		if (CleanupQuestData()) //! Cleanup all quest states that have no state
		{
			JsonFileLoader<ExpansionQuestPersistentData>.JsonSaveFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".JSON", this);
		}
		else
		{
			Error(ToString() + "::Save - Could not save player state data. Something went rly wrong!");
		}
	}

	void CopyConfig(ExpansionQuestPersistentDataBase dataBase)
	{
		//! Nothing to do here yet!
	}

	private bool CleanupQuestData()
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
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
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
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
			ExpansionQuestPersistentQuestData data = QuestDatas.Get(i);
			data.OnSend(ctx);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!QuestDatas)
		{
			QuestDatas = new array<ref ExpansionQuestPersistentQuestData>;
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
			ExpansionQuestPersistentQuestData data = new ExpansionQuestPersistentQuestData();
			if (!data.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRecieve - ExpansionQuestPersistentQuestData");
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