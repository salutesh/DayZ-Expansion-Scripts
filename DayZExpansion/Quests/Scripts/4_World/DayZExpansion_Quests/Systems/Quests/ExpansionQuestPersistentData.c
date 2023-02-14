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
	ref array<ref ExpansionQuestPersistentQuestData> QuestDatas;

	void ExpansionQuestPersistentData()
	{
		if (!QuestDatas)
			QuestDatas = new array<ref ExpansionQuestPersistentQuestData>
	}

	void AddQuestData(int questID, int state)
	{
		QuestDebugPrint(ToString() + "::AddQuestData - Start");
		QuestDebugPrint(ToString() + "::AddQuestData - Quest ID: " + questID + " | State: " + state);

		bool hasData;
		foreach (ExpansionQuestPersistentQuestData currentData: QuestDatas)
		{
			if (currentData.QuestID == questID)
				hasData = true;
		}

		if (hasData)
		{
			QuestDebugPrint(ToString() + "::AddQuestData - There is already a data set for the quest with ID: " + questID + ". Skip..");
			return;
		}

		ExpansionQuestPersistentQuestData questData = new ExpansionQuestPersistentQuestData();
		questData.QuestID = questID;
		questData.State = state;
		questData.UpdateLastUpdateTime();
		QuestDatas.Insert(questData);

		QuestDebugPrint(ToString() + "::AddQuestData - End");
	}

	void RemoveQuestDataByQuestID(int questID)
	{
		for (int i = QuestDatas.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestPersistentQuestData currentData = QuestDatas[i];
			if (currentData && currentData.QuestID == questID)
			{
				currentData.ClearObjectiveData();
				QuestDatas.RemoveOrdered(i);
			}
		}
	}

	bool HasDataForQuest(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData currentData: QuestDatas)
		{
			if (currentData.QuestID == questID)
				return true;
		}

		return false;
	}

	int GetQuestStateByQuestID(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData currentData: QuestDatas)
		{
			if (currentData.QuestID == questID)
				return currentData.State;
		}

		return ExpansionQuestState.NONE;
	}

	int GetQuestCompletionCountByQuestID(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData currentData: QuestDatas)
		{
			if (currentData.QuestID == questID)
				return currentData.CompletionCount;
		}

		return 0;
	}

	ExpansionQuestPersistentQuestData GetQuestDataByQuestID(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData currentData: QuestDatas)
		{
			if (currentData.QuestID == questID)
				return currentData;
		}

		return NULL;
	}

	array<ref ExpansionQuestPersistentQuestData> GetQuestDatas()
	{
		return QuestDatas;
	}

	int GetQuestTimestampByQuestID(int questID)
	{
		foreach (ExpansionQuestPersistentQuestData currentData: QuestDatas)
		{
			if (currentData.QuestID == questID)
				return currentData.Timestamp;
		}

		return -1;
	}

	array<ref ExpansionQuestObjectiveData> GetQuestObjectivesByQuestID(int questID)
	{
		QuestDebugPrint(ToString() + "::GetQuestObjectivesByQuestID - Start");

		array<ref ExpansionQuestObjectiveData> validObjectives = new array<ref ExpansionQuestObjectiveData>;
		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (questData)
		{
			array<ref ExpansionQuestObjectiveData> dataObjectives = questData.QuestObjectives;
			foreach (ExpansionQuestObjectiveData currentObjective: dataObjectives)
			{
				validObjectives.Insert(currentObjective);
			}
		}

		QuestDebugPrint(ToString() + "::GetQuestObjectivesByQuestID - End and return objectives: " + validObjectives.Count());

		return validObjectives;
	}

	ExpansionQuestObjectiveData GetQuestObjectiveByQuestIDAndIndex(int questID, int index)
	{
		QuestDebugPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Start");

		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (questData)
		{
			foreach (ExpansionQuestObjectiveData currentObjective: questData.QuestObjectives)
			{
				if (currentObjective.GetObjectiveIndex() == index)
				{
					QuestDebugPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return ExpansionQuestObjectiveData: " + currentObjective.ToString());
					return currentObjective;
				}
			}
		}

		QuestDebugPrint(ToString() + "::GetQuestObjectiveByQuestIDAndIndex - Return NULL");
		return NULL;
	}

	void UpdateQuestState(int questID, int state)
	{
		QuestDebugPrint(ToString() + "::UpdateQuestState - Start");
		QuestDebugPrint(ToString() + "::UpdateQuestState - Quest ID: " + questID + " | State: " + state);

		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (!questData)
		{
			Error(ToString() + "::UpdateQuestState - Could not get persistent quest data!");
		}

		questData.State = state;
		questData.UpdateLastUpdateTime();
		questData.QuestDebug();

		QuestDebugPrint(ToString() + "::UpdateQuestState - End");
	}

	void UpdateQuestTimestamp(int questID, int time)
	{
		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (!questData)
		{
			Error(ToString() + "::UpdateQuestTimestamp - Could not get persistent quest data!");
		}

		questData.Timestamp = time;
		questData.UpdateLastUpdateTime();
		questData.QuestDebug();
	}

	bool UpdateObjective(int questID, int objectiveIndex, ExpansionQuestObjectiveData newData)
	{
		QuestDebugPrint(ToString() + "::UpdateObjective - Update quest objective data for quest: " + questID + " | File: " + FileName);
		QuestDebugPrint(ToString() + "::UpdateObjective - New data:");
		newData.QuestDebug();

		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (!questData)
		{
			Error(ToString() + "::UpdateObjective - Could not get persistent quest data!");
			return false;
		}

		questData.QuestObjectives.RemoveOrdered(objectiveIndex);
		questData.UpdateLastUpdateTime();
		questData.QuestObjectives.InsertAt(newData, objectiveIndex);
		questData.QuestDebug();

		return true;
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
				Save(fileName);
		}

		return true;
	}

	void Save(string fileName)
	{
		EXTrace.Print(EXTrace.QUESTS, this, "::Save - Save existing data file: " + fileName);

		DataCheck();

		FileSerializer file = new FileSerializer();
		if (file.Open(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".bin", FileMode.WRITE))
		{
			file.Write(DataVersion);
			OnWrite(file);
			file.Close();
		}
	}

	bool DataCheck()
	{
		bool changed = false;
		for (int i = QuestDatas.Count() - 1; i >= 0; i--)
		{
			//! Data mismatch checks
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (!data)
				continue;

			int questID = data.QuestID;

			QuestDebugPrint(ToString() + "::DataCheck - Check data for quest with ID: " + questID + " | File: " + FileName);
			data.QuestDebug();
			ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(data.QuestID);
			if (!questConfig)
			{
				Error(ToString() + "::DataCheck - Could not get quest config for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + FileName);
				QuestDatas.Remove(questID);
				changed = true;
				continue;
			}

			bool removed = false;
			for (int o = 0; o < data.QuestObjectives.Count(); o++)
			{
				ExpansionQuestObjectiveData questObjectiveData = data.QuestObjectives[o];
				ExpansionQuestObjectiveConfig questObjectiveConfig = questConfig.GetObjectives()[o];

				if (questObjectiveData && questObjectiveConfig)
					QuestDebugPrint(ToString() + "::DataCheck - Check objective with index: " + o + " | Objective Config Type: " + questObjectiveData.GetObjectiveType() + " | Objective Data Type: " + questObjectiveConfig.GetObjectiveType() + " | Quest ID: " + data.QuestID);

				if (questObjectiveData && questObjectiveConfig && questObjectiveData.GetObjectiveType() != questObjectiveConfig.GetObjectiveType())
				{
					QuestDebugPrint(ToString() + "::DataCheck - Quest objectives type missmatch for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + FileName);
					QuestDatas.Remove(questID);
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

				QuestDebugPrint(ToString() + "::DataCheck - Cleanup quest data for quest with ID:" + data.QuestID + " | State: " + data.State + " | File: " + FileName);
				QuestDatas.Remove(questID);
				changed = true;
			}
		}

		return changed;
	}

	bool HasCooldownOnQuest(int questID, out int timestamp)
	{
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data.QuestID != questID)
				continue;

			ExpansionQuestConfig questConfig;
			if (GetGame().IsClient())
			{
				questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigClientByID(data.QuestID);
			}
			else if (GetGame().IsServer() && GetGame().IsMultiplayer())
			{
				questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(data.QuestID);
			}

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
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (!data)
				continue;

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

		int dataCount;
		if (!ctx.Read(dataCount))
			return false;

		for (int i = 0; i < dataCount; i++)
		{
			ExpansionQuestPersistentQuestData data = new ExpansionQuestPersistentQuestData();
			if (!data.OnRead(ctx))
			{
				Error(ToString() + "::OnRead - ExpansionQuestPersistentQuestData");
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
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		Print(text);
	#endif
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
		if (GetGame().IsServer() || QuestDatas.Count())
			Print("------------------------------------------------------------------------------------------------");
		if (GetGame().IsServer())
		{
			Print(ToString() + "::QuestDebug - File name: " + FileName);
			Print(ToString() + "::QuestDebug - Data version: " + DataVersion);
		}
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			QuestDatas.Get(i).QuestDebug();
		}
		if (GetGame().IsServer() || QuestDatas.Count())
			Print("------------------------------------------------------------------------------------------------");
	#endif
	}
};