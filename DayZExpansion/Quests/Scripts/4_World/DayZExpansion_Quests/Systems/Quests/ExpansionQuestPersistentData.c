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
	string m_FileName;

	static const int DATAVERSION = 2;
	int DataVersion;
	ref map<int, ref ExpansionQuestPersistentQuestData> QuestData = new map<int, ref ExpansionQuestPersistentQuestData>;

	[NonSerialized()];
	bool m_SynchDirty = true;

	[NonSerialized()]
	bool m_RemoveQuestData;

	void ~ExpansionQuestPersistentData()
	{
		if (!g_Game)
			return;

		if (QuestData)
			EXPrint(ToString() + "~ExpansionQuestPersistentData (" + QuestData.Count() + ")");
		else
			EXPrint(ToString() + "~ExpansionQuestPersistentData");
	}

	void AddQuestData(int questID, ExpansionQuestState state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		QuestDebugPrint("Quest ID: " + questID + " | State: " + state + " | File: " + m_FileName);

		if (QuestData.Contains(questID))
		{
			QuestDebugPrint("There is already a data set for the quest with ID: " + questID + ". Skip...");
			return;
		}

		ExpansionQuestPersistentQuestData questData = new ExpansionQuestPersistentQuestData();
		questData.QuestID = questID;
		questData.State = state;
		questData.UpdateLastUpdateTime();
		QuestData[questID] = questData;

		m_SynchDirty = true;
	}

	void RemoveQuestDataByQuestID(int questID)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		ExpansionQuestPersistentQuestData currentData = QuestData[questID];
		if (currentData)
		{
			QuestDebugPrint("Remove data for quest ID: " + currentData.QuestID);
			currentData.ClearObjectiveData();
			QuestData.Remove(questID);
			m_SynchDirty = true;
		}
	}

	ExpansionQuestState GetQuestStateByQuestID(int questID)
	{
		ExpansionQuestPersistentQuestData currentData = QuestData[questID];
		if (currentData)
			return currentData.State;

		return ExpansionQuestState.NONE;
	}

	int GetQuestCompletionCountByQuestID(int questID)
	{
		ExpansionQuestPersistentQuestData currentData = QuestData[questID];
		if (currentData)
			return currentData.CompletionCount;

		return 0;
	}

	ExpansionQuestPersistentQuestData GetQuestDataByQuestID(int questID)
	{
		return QuestData[questID];
	}

	int GetQuestTimestampByQuestID(int questID)
	{
		ExpansionQuestPersistentQuestData currentData = QuestData[questID];
		if (currentData)
			return currentData.Timestamp;

		return -1;
	}

	array<ref ExpansionQuestObjectiveData> GetQuestObjectivesByQuestID(int questID)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		ExpansionQuestPersistentQuestData questData = QuestData[questID];
		if (questData)
			return questData.QuestObjectives;

		return null;
	}

	ExpansionQuestObjectiveData GetQuestObjectiveByQuestIDAndIndex(int questID, int index)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		ExpansionQuestPersistentQuestData questData = QuestData[questID];
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		QuestDebugPrint("Quest ID: " + questID + " | State: " + state + " | File: " + m_FileName);

		ExpansionQuestPersistentQuestData questData = QuestData[questID];
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
		ExpansionQuestPersistentQuestData questData = QuestData[questID];
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
		QuestDebugPrint("Update quest objective data for quest: " + questID + " | File: " + m_FileName);
		QuestDebugPrint("New data:");
		newData.QuestDebug();

		ExpansionQuestPersistentQuestData questData = QuestData[questID];
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
		QuestDebugPrint("Update quest completion count for quest: " + questID + " | File: " + m_FileName);		
		ExpansionQuestPersistentQuestData questData = QuestData[questID];
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

	bool Load(string fileName, string folder)
	{
		EXTrace.Print(EXTrace.QUESTS, this, "::Load - Load existing data file: " + fileName);

		m_FileName = fileName;

		string path = folder + fileName + ".bin";
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
		else
		{
			return false;
		}

		if (DataCheck())
			save = true;

		if (save)
			Save(fileName, folder);

		m_SynchDirty = true;

		return true;
	}

	void Save(string fileName, string path)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, fileName);
#endif

		FileSerializer file = new FileSerializer();
		if (file.Open(path + fileName + ".bin", FileMode.WRITE))
		{
			file.Write(DataVersion);
			OnWrite(file, false);
			file.Close();
		}
		else
		{
			Error("Could not open " + path + fileName + ".bin" + " for writing!");
		}
	}

	protected bool DataCheck()
	{
		bool changed = false;
		foreach (ExpansionQuestPersistentQuestData data: QuestData)
		{
			if (data)
			{
				int questID = data.QuestID;
				QuestDebugPrint("Check data for quest with ID: " + questID + " | File: " + m_FileName);
				data.QuestDebug();
				ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(data.QuestID);
				if (!questConfig)
				{
					EXPrint(ToString() + " ERROR: Could not get quest config for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + m_FileName);
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
						EXPrint(ToString() + " ERROR: Could not get quest objective config!");
						RemoveQuestDataByQuestID(questID);
						changed = true;
						removedQuestData = true;
						break;
					}
					
					if (questObjectiveData && questObjectiveConfig && questObjectiveData.GetObjectiveType() != questObjectiveConfig.GetObjectiveType())
					{
						EXPrint(ToString() + " ERROR: Quest objectives type missmatch for quest ID: " + data.QuestID + ". Removed data for this quest! File: " + m_FileName);
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
	
					EXPrint(ToString() + " Cleanup quest data for quest with ID:" + data.QuestID + " | State: " + data.State + " | File: " + m_FileName);
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
		auto data = QuestData[questID];
		if (data)
		{
			ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(data.QuestID);

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

	void OnWrite(ParamsWriteContext ctx, bool resetSynchDirty = true, int questID = -1)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		if (questID == -1)
		{
			int dataCount = QuestData.Count();
#ifdef EXTRACE
			EXTrace.Add(trace, "Count: " + dataCount);
#endif
			ctx.Write(dataCount);
			foreach (auto data: QuestData)
			{
				data.OnWrite(ctx);
			}
		}
		else
		{
			auto questData = QuestData[questID];
			if (questData)
			{
				ctx.Write(1);
				questData.OnWrite(ctx);
			}
			else
			{
				ctx.Write(-1);
			}
		}

		if (resetSynchDirty)
			m_SynchDirty = false;
	}

	bool OnRead(ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		QuestData.Clear();

		int dataCount;
		if (!ctx.Read(dataCount))
		{
			Error(ToString() + "::OnRead - couldn't read persistent quest data count!");
			return false;
		}

		m_RemoveQuestData = dataCount == -1;

#ifdef EXTRACE
		EXTrace.Add(trace, "Count: " + dataCount);
#endif 
		
		for (int i = 0; i < dataCount; i++)
		{
			ExpansionQuestPersistentQuestData data = new ExpansionQuestPersistentQuestData();
			if (!data.OnRead(ctx))
			{
				Error(ToString() + "::OnRead - couldn't read persistent quest data!");
				return false;
			}

			QuestData[data.QuestID] = data;
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
		if (g_Game.IsServer() || QuestData.Count())
			Print("------------------------------------------------------------------------------------------------");
		if (g_Game.IsServer())
		{
			Print(ToString() + "::QuestDebug - File name: " + m_FileName);
			Print(ToString() + "::QuestDebug - Data version: " + DataVersion);
		}
		foreach (auto data: QuestData)
		{
			data.QuestDebug();
		}
		if (g_Game.IsServer() || QuestData.Count())
			Print("------------------------------------------------------------------------------------------------");
	#endif
	}
};