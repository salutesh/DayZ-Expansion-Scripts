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

//! Pesistent player quest data handled in the quest module
class ExpansionQuestPersistentData: ExpansionQuestPersistentDataBase
{
	static const int CONFIGVERSION = 1;
	ref array<ref ExpansionQuestPersistentQuestData> QuestDatas = new array<ref ExpansionQuestPersistentQuestData>;

	[NonSerialized()]
	protected ExpansionQuestModule m_QuestModule;

	void ExpansionQuestPersistentData()
	{
		ConfigVersion = CONFIGVERSION;
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
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				return true;
			}
		}

		return false;
	}

	int GetQuestStateByQuestID(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				return data.State;
			}
		}

		return ExpansionQuestState.NONE;
	}

	ExpansionQuestPersistentQuestData GetQuestDataByQuestID(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
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
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
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
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID != questID)
				continue;

			array<ref ExpansionQuestObjectiveData> dataObjectives = data.QuestObjectives;
			foreach (ExpansionQuestObjectiveData currentObjective: dataObjectives)
			{
				validObjectives.Insert(currentObjective);
			}
		}

		return validObjectives;
	}

	ref ExpansionQuestObjectiveData GetQuestObjectiveByQuestIDAndIndex(int questID, int index)
	{
		QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Start");
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				QuestPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - ExpansionQuestPersistentQuestData: " + data.ToString());
				foreach (ExpansionQuestObjectiveData currentObjective: data.QuestObjectives)
				{
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
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				data.State = state;
				data.LastUpdateTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
			}
		}
	}

	void UpdateQuestTimestamp(int questID, int time)
	{
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
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
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				//! No need for that check when we can just remove the objective based on the objectiveIndex?!
				for (int i = data.QuestObjectives.Count() - 1; i >= 0; i--)
				{
					ExpansionQuestObjectiveData currentObjective = data.QuestObjectives[i];
					if (currentObjective && currentObjective.GetObjectiveIndex() != objectiveIndex)
					{
						QuestPrint(ToString() + "::UpdateObjective - F2");
						continue;
					}

					data.QuestObjectives.RemoveOrdered(i);
				}

				data.LastUpdateTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
				data.QuestObjectives.Insert(newData);
			}
		}
		QuestPrint(ToString() + "::UpdateObjective - End");
	}

	bool Load(string fileName)
	{
		CF_Log.Info("[ExpansionQuestPersistentData] Load existing configuration file:" + fileName);

		string path = EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".bin";
		if (FileExist(path))
		{
			FileSerializer file = new FileSerializer();

			if (file.Open(path, FileMode.READ))
			{
				file.Read(ConfigVersion);
				OnRecieve(file);
				file.Close();
			}

			if (CleanupQuestData())
				return true;
		}

		return false;
	}

	void Save(string fileName)
	{
		if (CleanupQuestData()) //! Cleanup all quest states that have no state
		{
			FileSerializer file = new FileSerializer();
			if (file.Open(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".bin", FileMode.WRITE))
			{
				file.Write(ConfigVersion);
				OnSend(file);
				file.Close();
			}
		}
		else
		{
			CF_Log.Error(ToString() + "::Save - Could not save player state data. Something went rly wrong!");
		}
	}

	void CopyConfig(ExpansionQuestPersistentDataBase dataBase)
	{
		//! Nothing to do here yet!
	}

	void CopyConfig(ExpansionQuestPersistentData data)
	{
		QuestDatas = data.QuestDatas;
	}

	protected bool CleanupQuestData()
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		for (int i = QuestDatas.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigByID(data.QuestID);
			if (!questConfig)
			{
				CF_Log.Error(ToString() + "::CleanupQuestData - Could not get quest config for quest ID: " + data.QuestID);
				return false;
			}

			//! Cleanup old persistent quest data if quest configuration has changed
			//! TODO: This is FFS not a good solution
			/*if (!QuestChangesCheck(questConfig, data))
				QuestDatas.RemoveOrdered(i);*/

			if (data.State == ExpansionQuestState.NONE || data.State == ExpansionQuestState.COMPLETED)
			{
				//! Never cleanup quest data if the quest is a achivement quest.
				if (questConfig.IsAchivement())
					continue;

				//! Never cleanup daylie/weekly quest data for quests that have a cooldown.
				if (questConfig.IsWeeklyQuest() || questConfig.IsDailyQuest())
				{
					if (data.State == ExpansionQuestState.COMPLETED)
						data.State = ExpansionQuestState.NONE;
					
					int timestamp;
					if (HasCooldownOnQuest(data.QuestID, timestamp))
						continue;
				}
				
				//! Never cleanup quest data for quests that have a pre/followup quest.
				if (questConfig.GetFollowUpQuestID() > -1 || questConfig.GetPreQuestID() > -1)
					continue;

				//! Never cleanup quest data if the quest is not repeatable
				if (!questConfig.IsRepeatable())
					continue;

				QuestPrint(ToString() + "::CleanupQuestData - Cleanup quest state data for quest with ID:" + data.QuestID + " | State: " + data.State);
				QuestDatas.RemoveOrdered(i);
			}
		}

		return true;
	}

	protected bool QuestChangesCheck(ExpansionQuestConfig questConfig, ExpansionQuestPersistentQuestData data)
	{
		for (int i = 0; i < data.QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveData playerQuestObjectiveData = data.QuestObjectives[i];
			if (!playerQuestObjectiveData)
				continue;

			ExpansionQuestObjectiveConfig questObjectiveConfig = questConfig.GetObjectives()[i];
			if (!questObjectiveConfig)
				continue;

			if (playerQuestObjectiveData.GetObjectiveType() != questObjectiveConfig.GetObjectiveType())
				return false;
		}

		return true;
	}

	bool HasCooldownOnQuest(int questID, out int timestamp)
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID != questID)
				continue;

			ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigByID(data.QuestID);
			if (!questConfig)
			{
				CF_Log.Error(ToString() + "::CleanupQuestStates - Could not get quest config for quest ID: " + questID);
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
				CF_Log.Error(ToString() + "::OnRecieve - ExpansionQuestPersistentQuestData");
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
		CF_Log.Debug(text);
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