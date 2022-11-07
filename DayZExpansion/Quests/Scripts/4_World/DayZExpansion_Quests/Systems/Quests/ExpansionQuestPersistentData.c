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

//! Pesistent player quest data handled in the quest module
class ExpansionQuestPersistentData
{
	[NonSerialized()];
	string FileName;

	static const int DATAVERSION = 2;
	int DataVersion;
	ref array<ref ExpansionQuestPersistentQuestData> QuestDatas = new array<ref ExpansionQuestPersistentQuestData>;

	[NonSerialized()]
	protected ExpansionQuestModule m_QuestModule;

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

	void RemoveQuestDataByQuestID(int questID)
	{
		for (int i = QuestDatas.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestPersistentQuestData persQuestData = QuestDatas[i];
			if (persQuestData.QuestID == questID)
				QuestDatas.RemoveOrdered(i);
		}
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

	int GetQuestCompletionCountByQuestID(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				return data.CompletionCount;
			}
		}

		return 0;
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

	ExpansionQuestObjectiveData GetQuestObjectiveByQuestIDAndIndex(int questID, int index)
	{
		QuestDebugPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Start");
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				QuestDebugPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - ExpansionQuestPersistentQuestData: " + data.ToString());
				foreach (ExpansionQuestObjectiveData currentObjective: data.QuestObjectives)
				{
					if (currentObjective.GetObjectiveIndex() == index)
					{
						QuestDebugPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return ExpansionQuestObjectiveData: " + currentObjective.ToString());
						return currentObjective;
					}
				}
			}
		}

		QuestDebugPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return ExpansionQuestObjectiveData: NULL");
		return NULL;
	}

	void UpdateQuestState(int questID, int state)
	{
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				data.State = state;
				data.UpdateLastUpdateTime();
				data.QuestDebug();
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
				data.UpdateLastUpdateTime();
				data.QuestDebug();
			}
		}
	}

	void UpdateObjective(int questID, int objectiveIndex, ExpansionQuestObjectiveData newData)
	{
		QuestDebugPrint(ToString() + "::UpdateObjective - Update quest objective data for quest: " + questID + " | File: " + FileName);
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID == questID)
			{
				//! No need for that check when we can just remove the objective based on the objectiveIndex?!
				for (int i = data.QuestObjectives.Count() - 1; i >= 0; i--)
				{
					ExpansionQuestObjectiveData currentObjective = data.QuestObjectives[i];
					if (currentObjective && currentObjective.GetObjectiveIndex() != objectiveIndex)
						continue;

					data.QuestObjectives.RemoveOrdered(i);
					break;
				}

				data.UpdateLastUpdateTime();
				data.QuestObjectives.InsertAt(newData, objectiveIndex);
				data.QuestDebug();
			}
		}
	}

	bool Load(string fileName)
	{
		EXTrace.Print(EXTrace.QUESTS, this, "::Load - Load existing data file: " + fileName);

		FileName = fileName;

		string path = EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".bin";
		if (FileExist(path))
		{
			FileSerializer file = new FileSerializer();
			bool save;
			if (file.Open(path, FileMode.READ))
			{
				file.Read(DataVersion);
				EXTrace.Print(EXTrace.QUESTS, this, "::Load - Data version of file " + fileName + " is " + DataVersion + ".");

				bool success = OnRead(file);

				file.Close();

				if (!success)
					return false;

				if (DataVersion < DATAVERSION)
				{
					EXTrace.Print(EXTrace.QUESTS, this, "::Load - Data conversion from version " + DataVersion + " to version " + DATAVERSION + " completed for file: " + fileName);
					save = true;
					DataVersion = DATAVERSION;
				}
			}

			if (DataCheck())
				save = true;

			if (save)
				Save(fileName, false);
		}

		return true;
	}

	void Save(string fileName, bool dataCheck = true)
	{
		EXTrace.Print(EXTrace.QUESTS, this, "::Save - Save existing data file: " + fileName);

		if (dataCheck)
			DataCheck();

		FileSerializer file = new FileSerializer();
		if (file.Open(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".bin", FileMode.WRITE))
		{
			file.Write(DataVersion);
			OnWrite(file);
			file.Close();
		}
	}

	protected bool DataCheck()
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		bool changed = false;
		for (int i = QuestDatas.Count() - 1; i >= 0; i--)
		{
			//! Data mismatch checks
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			EXTrace.Print(EXTrace.QUESTS, this, "::DataCheck - Check data for quest with ID: " + data.QuestID + " | File: " + FileName);
			ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigByID(data.QuestID);
			if (!questConfig)
			{
				Error(ToString() + "::DataCheck - Could not get quest config for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + FileName);
				QuestDatas.RemoveOrdered(i);
				changed = true;
				continue;
			}

			bool removed = false;
			for (int o = 0; o < data.QuestObjectives.Count(); o++)
			{
				ExpansionQuestObjectiveData questObjectiveData = data.QuestObjectives[o];
				ExpansionQuestObjectiveConfig questObjectiveConfig = questConfig.GetObjectives()[o];

				if (questObjectiveData && questObjectiveConfig)
					EXTrace.Print(EXTrace.QUESTS, this, "::DataCheck - Check objective with index: " + o + " | Objective Config Type: " + questObjectiveData.GetObjectiveType() + " | Objective Data Type: " + questObjectiveConfig.GetObjectiveType() + " | Quest ID: " + data.QuestID);
				if (questObjectiveData && questObjectiveConfig && questObjectiveData.GetObjectiveType() != questObjectiveConfig.GetObjectiveType())
				{
					EXTrace.Print(EXTrace.QUESTS, this, "::DataCheck - Quest objectives type missmatch for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + FileName);
					QuestDatas.RemoveOrdered(i);
					removed = true;
					changed = true;
					break;
				}
			}

			if (removed)
				continue;

			if (data.State == ExpansionQuestState.NONE || data.State == ExpansionQuestState.COMPLETED)
			{
				//! Never cleanup quest data if the quest is a achivement, auto-complete or group quest.
				if (questConfig.IsAchivement() || questConfig.IsAutocomplete() || questConfig.IsGroupQuest())
					continue;

				//! Never cleanup quest data that match auto-start quest configuration
				if (questConfig.GetPreQuestIDs().Count() == 0 && questConfig.GetQuestGiverIDs().Count() == 0 && !questConfig.IsAchivement() && !questConfig.IsGroupQuest())
					continue;

				//! Never cleanup daylie/weekly quest data.
				if (questConfig.IsWeeklyQuest() || questConfig.IsDailyQuest())
					continue;

				//! Never cleanup quest data for quests that have a pre/followup quest.
				if (questConfig.GetFollowUpQuestID() > -1 || questConfig.GetPreQuestIDs().Count() > 0)
					continue;

				//! Never cleanup quest data if the quest is repeatable or not repeatable
				if (questConfig.IsRepeatable() || !questConfig.IsRepeatable())
					continue;

				EXTrace.Print(EXTrace.QUESTS, this, "::DataCheck - Cleanup quest data for quest with ID:" + data.QuestID + " | State: " + data.State + " | File: " + FileName);
				QuestDatas.RemoveOrdered(i);
				changed = true;
			}
		}

		return changed;
	}

	bool HasCooldownOnQuest(int questID, out int timestamp)
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID != questID)
				continue;

			ExpansionQuestConfig questConfig;
			if (GetGame().IsClient())
				questConfig = m_QuestModule.GetQuestConfigClientByID(data.QuestID);
			else if (GetGame().IsServer() && GetGame().IsMultiplayer())
				questConfig = m_QuestModule.GetQuestConfigByID(data.QuestID);

			if (!questConfig)
			{
				Error(ToString() + "::HasCooldownOnQuest - Could not get quest config for quest ID: " + questID);
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

	void OnWrite(ParamsWriteContext ctx)
	{
		int dataCount = QuestDatas.Count();
		ctx.Write(dataCount);
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas.Get(i);
			data.OnWrite(ctx);
		}
	}

	bool OnRead(ParamsReadContext ctx)
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
			if (DataVersion == 1)
			{
				QuestDebugPrint(ToString() + "::OnRead - Start conversion of persisten quest data to version 2..");
				if (!data.OnRead_V1(ctx))
				{
					Error(ToString() + "::OnRead - Conversion of persistent quest data to version 2 failed!");
					return false;
				}

				data.QuestDebug();
			}
			else
			{

				if (!data.OnRead(ctx))
				{
					Error(ToString() + "::OnRead - ExpansionQuestPersistentQuestData");
					return false;
				}
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
			QuestDebugPrint(ToString() + "::HasCooldown - Time -1 for quest with ID: " + questID);
			return false;
		}

		timedif = GetTimeDiff(timestamp);
		QuestDebugPrint("------------------------------------------------------------------------------------------------");
		QuestDebugPrint(ToString() + "::HasCooldown - Time for quest with ID: " + timestamp);
		QuestDebugPrint(ToString() + "::HasCooldown - Formated time for quest with ID: " + ExpansionStatic.FormatTime(timestamp, false));
		QuestDebugPrint("------------------------------------------------------------------------------------------------");
		QuestDebugPrint(ToString() + "::HasCooldown - Time difference for quest with ID: " + timedif);
		QuestDebugPrint(ToString() + "::HasCooldown - Formated time difference for quest with ID: " + ExpansionStatic.FormatTime(timedif, false));
		QuestDebugPrint("------------------------------------------------------------------------------------------------");

		if (timedif < cooldown)
			return true;

		return false;
	}

	int GetTimeDiff(int timestamp)
	{
		int currentTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
		return (currentTime - timestamp);
	}

	void QuestDebugPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print(text);
	#endif
	}

	void QuestDebug()
	{
		Print("------------------------------------------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - File name: " + FileName);
		Print(ToString() + "::QuestDebug - Data version: " + DataVersion);
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			QuestDatas[i].QuestDebug();
		}
		Print("------------------------------------------------------------------------------------------------");
	}
};