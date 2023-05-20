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

	[NonSerialized()];
	bool m_SynchDirty;

	void ExpansionQuestPersistentData()
	{
		QuestDatas = new array<ref ExpansionQuestPersistentQuestData>;
	}

	void ~ExpansionQuestPersistentData()
	{
		if (QuestDatas)
			EXPrint(this, "~ExpansionQuestPersistentData (" + QuestDatas.Count() + ")");
		else
			EXPrint(this, "~ExpansionQuestPersistentData");
	}

	void AddQuestData(int questID, ExpansionQuestState state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestDebugPrint("Quest ID: " + questID + " | State: " + state);

		if (HasDataForQuest(questID))
		{
			QuestDebugPrint("There is already a data set for the quest with ID: " + questID + ". Skip..");
			return;
		}

		ExpansionQuestPersistentQuestData questData = new ExpansionQuestPersistentQuestData();
		questData.QuestID = questID;
		questData.State = state;
		questData.UpdateLastUpdateTime();
		QuestDatas.Insert(questData);

		m_SynchDirty = true;
	}

	void RemoveQuestDataByQuestID(int questID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		for (int i = QuestDatas.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestPersistentQuestData currentData = QuestDatas[i];
			QuestDebugPrint("Check data for quest with ID: " + currentData.QuestID);
			if (currentData.QuestID == questID)
			{
				QuestDebugPrint("Remove data for quest ID: " + currentData.QuestID);
				currentData.ClearObjectiveData();
				QuestDatas.RemoveOrdered(i);
				m_SynchDirty = true;
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

	ExpansionQuestState GetQuestStateByQuestID(int questID)
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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

		QuestDebugPrint("End and return objectives: " + validObjectives.Count());

		return validObjectives;
	}

	ExpansionQuestObjectiveData GetQuestObjectiveByQuestIDAndIndex(int questID, int index)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (questData)
		{
			foreach (ExpansionQuestObjectiveData currentObjective: questData.QuestObjectives)
			{
				if (currentObjective.GetObjectiveIndex() == index)
				{
					QuestDebugPrint("Return ExpansionQuestObjectiveData: " + currentObjective.ToString());
					return currentObjective;
				}
			}
		}

		QuestDebugPrint("Return NULL");
		return NULL;
	}

	void UpdateQuestState(int questID, int state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestDebugPrint("Quest ID: " + questID + " | State: " + state);

		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (!questData)
		{
			Error(ToString() + "::UpdateQuestState - Could not get persistent quest data!");
		}

		questData.State = state;
		questData.UpdateLastUpdateTime();
		questData.QuestDebug();

		m_SynchDirty = true;
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

		m_SynchDirty = true;
	}

	bool UpdateObjective(int questID, int objectiveIndex, ExpansionQuestObjectiveData newData)
	{
		QuestDebugPrint("Update quest objective data for quest: " + questID + " | File: " + FileName);
		QuestDebugPrint("New data:");
		newData.QuestDebug();

		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (questData)
		{
			questData.QuestObjectives.RemoveOrdered(objectiveIndex);
			questData.UpdateLastUpdateTime();
			questData.QuestObjectives.InsertAt(newData, objectiveIndex);
			questData.QuestDebug();
			m_SynchDirty = true;
			return true;
		}
		
		return false;
	}
	
	bool UpdateCompletionCount(int questID)
	{
		QuestDebugPrint("Update quest completion count for quest: " + questID + " | File: " + FileName);		
		ExpansionQuestPersistentQuestData questData = GetQuestDataByQuestID(questID);
		if (questData)
		{
			questData.CompletionCount = (questData.CompletionCount + 1);
			questData.UpdateLastUpdateTime();
			QuestDebugPrint("Updated completion count for quest. Quest ID: " + questID + " | Completion count: " + questData.CompletionCount);
			m_SynchDirty = true;
			return true;
		}
		
		return false;
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

			m_SynchDirty = true;
		}

		return true;
	}

	void Save(string fileName, bool doCheck = true)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, fileName, "doCheck: " + doCheck);

		if (doCheck)
			DataCheck();

		FileSerializer file = new FileSerializer();
		if (file.Open(EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".bin", FileMode.WRITE))
		{
			file.Write(DataVersion);
			OnWrite(file, false);
			file.Close();
		}
		else
		{
			Error("Could not open " + EXPANSION_QUESTS_PLAYERDATA_FOLDER + fileName + ".bin" + " for writing!");
		}
	}

	protected bool DataCheck()
	{
		bool changed = false;
		foreach (ExpansionQuestPersistentQuestData data: QuestDatas)
		{
			if (data)
			{
				int questID = data.QuestID;
				QuestDebugPrint("Check data for quest with ID: " + questID + " | File: " + FileName);
				data.QuestDebug();
				ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(data.QuestID);
				if (!questConfig)
				{
					EXPrint(this, "ERROR: Could not get quest config for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + FileName);
					RemoveQuestDataByQuestID(questID);
					changed = true;
					continue;
				}
	
				bool removedQuestData = false;
				for (int o = 0; o < data.QuestObjectives.Count(); o++)
				{
					ExpansionQuestObjectiveData questObjectiveData = data.QuestObjectives[o];
					ExpansionQuestObjectiveConfigBase questObjectiveConfig = questConfig.GetObjectives()[o];
	
				#ifdef EXPANSIONMODQUESTSPLAYERDATADEBUG
					if (questObjectiveData && questObjectiveConfig)
						QuestDebugPrint("Check objective with index: " + o + " | Objective Config Type: " + questObjectiveData.GetObjectiveType() + " | Objective Data Type: " + questObjectiveConfig.GetObjectiveType() + " | Quest ID: " + data.QuestID);
				#endif
					
					if (!questObjectiveConfig)
					{
						EXPrint(this, "ERROR: Could not get quest objective config!");
						RemoveQuestDataByQuestID(questID);
						changed = true;
						removedQuestData = true;
						break;
					}
					
					if (questObjectiveData && questObjectiveConfig && questObjectiveData.GetObjectiveType() != questObjectiveConfig.GetObjectiveType())
					{
						EXPrint(this, "ERROR: Quest objectives type missmatch for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + FileName);
						RemoveQuestDataByQuestID(questID);
						changed = true;
						removedQuestData = true;
						break;
					}
				}

				if (removedQuestData)
					continue;
	
				if (data.State == ExpansionQuestState.NONE || data.State == ExpansionQuestState.COMPLETED)
				{
					//! Never cleanup quest data if the quest is a achievement, auto-complete or group quest.
					if (questConfig.IsAchievement() || questConfig.IsAutocomplete() || questConfig.IsGroupQuest())
						continue;
	
					//! Never cleanup quest data that match auto-start quest configuration
					if (questConfig.GetPreQuestIDs().Count() == 0 && questConfig.GetQuestGiverIDs().Count() == 0 && !questConfig.IsAchievement() && !questConfig.IsGroupQuest())
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
	
					EXPrint(this, "Cleanup quest data for quest with ID:" + data.QuestID + " | State: " + data.State + " | File: " + FileName);
					RemoveQuestDataByQuestID(questID);
					changed = true;
				}
			}
		}
		
		QuestDebugPrint("End and return " + changed);

		if (changed)
			m_SynchDirty = true;

		return changed;
	}

	bool HasCooldownOnQuest(int questID, out int remaining)
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
			
			if (data.Timestamp > 0)
			{
				if (questConfig.IsDailyQuest() || questConfig.IsWeeklyQuest())
				{
					int timestamp = data.Timestamp;
					int currentTime = CF_Date.Now(true).GetTimestamp();
					remaining = timestamp - currentTime;
					return remaining > 0;
				}
			}
		}

		return false;
	}

	void OnWrite(ParamsWriteContext ctx, bool resetSynchDirty = true)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int dataCount = QuestDatas.Count();

		EXTrace.Add(trace, "Count: " + dataCount);

		ctx.Write(dataCount);
		for (int i = 0; i < QuestDatas.Count(); i++)
		{
			ExpansionQuestPersistentQuestData data = QuestDatas[i];
			if (!data)
				continue;

			data.OnWrite(ctx);
		}

		if (resetSynchDirty)
			m_SynchDirty = false;
	}

	bool OnRead(ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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
		{
			Error(ToString() + "::OnRead - couldn't read persistent quest data count!");
			return false;
		}

		EXTrace.Add(trace, "Count: " + dataCount);

		for (int i = 0; i < dataCount; i++)
		{
			ExpansionQuestPersistentQuestData data = new ExpansionQuestPersistentQuestData();
			if (!data.OnRead(ctx))
			{
				Error(ToString() + "::OnRead - couldn't read persistent quest data!");
				return false;
			}

			QuestDatas.Insert(data);
		}

		return true;
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