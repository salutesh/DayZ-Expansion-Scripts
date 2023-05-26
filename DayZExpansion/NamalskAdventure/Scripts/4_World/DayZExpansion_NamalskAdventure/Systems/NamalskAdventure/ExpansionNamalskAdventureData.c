/**
 * ExpansionNamalskAdventureData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNamalskAdventureDataBase
{
	int Version;
};

class ExpansionCommunityGoal
{
    protected int m_ID;
    protected int m_GoalValue;
    protected int m_GoalProgress;
	protected bool m_Finished;
    
    void ExpansionCommunityGoal(int id, int goal)
    {
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
        m_ID = id;
        m_GoalValue = goal;
    }
    
    int GetID()
    {
        return m_ID;
    }
    
    int GetProgress()
    {
        return m_GoalProgress;
    }
    
    int GetGoal()
    {
        return m_GoalValue;
    }
    
    void AddProgress(int progress)
    {
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
        m_GoalProgress = m_GoalProgress + progress;
    }
	
	void SetFinished()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_Finished = true;
	}
	
	bool IsFinished()
	{
		return m_Finished;
	}
};

class ExpansionNamalskAdventureData: ExpansionNamalskAdventureDataBase
{
	[NonSerialized()];
	static const int VERSION = 0;

	protected ref array<int> m_LastMerchantItemSets = new array<int>;
	protected ref array<int> m_LastMerchantPositions = new array<int>;
	protected ref array<ref ExpansionCommunityGoal> m_CommunityGoalData = new array<ref ExpansionCommunityGoal>;

	void ExpansionNamalskAdventureData()
	{
		Version = VERSION;
	}

	void CopyFromBaseClass(ExpansionNamalskAdventureDataBase base)
	{
		//! Nothing to do here yet
	}

	static ExpansionNamalskAdventureData Load(string fileName)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionNamalskAdventureData - Load");
		
		CF_Log.Info("[ExpansionNamalskAdventureData] Load existing namalsk adventure server data file:" + fileName);
		ExpansionNamalskAdventureDataBase dataBase;
		ExpansionJsonFileParser<ExpansionNamalskAdventureDataBase>.Load(fileName, dataBase);

		bool save;
		ExpansionNamalskAdventureData data = new ExpansionNamalskAdventureData();
		if (dataBase.Version < VERSION)
		{
			save = true;
			data.CopyFromBaseClass(dataBase); //! Copy over old data that has not changed.
			data.Version = VERSION;

			if (save)
				Save(data);
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Load(fileName, data))
				return NULL;
		}

		return data;
	}

	void Save()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Save(ExpansionNamalskModule.s_dataFolderPath + "ServerData.json", this);
	}

	static void Save(ExpansionNamalskAdventureData data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionNamalskAdventureData - Save");
		
		ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Save(ExpansionNamalskModule.s_dataFolderPath + "ServerData.json", data);
	}

	void ResetUsedItemSets()
	{
		m_LastMerchantItemSets.Clear();
	}

	void ResetUsedPositions()
	{
		m_LastMerchantPositions.Clear();
	}

	array<int> GetUsedMerchantItemSets()
	{
		return m_LastMerchantItemSets:
	}

	array<int> GetUsedMerchantPositions()
	{
		return m_LastMerchantPositions:
	}
	
	void SetCommunityGoal(ExpansionCommunityGoal goal)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!m_CommunityGoalData)
			m_CommunityGoalData = new array<ref ExpansionCommunityGoal>;
		
		bool newData = true;
	   	foreach (int i, ExpansionCommunityGoal goalData: m_CommunityGoalData)
		{
			if (goalData.GetID() == goal.GetID())
			{
				m_CommunityGoalData.Remove(i);
				m_CommunityGoalData.Insert(goal);
				Save();
				
				newData = false;
				return;
			}
		}
		
		if (newData)
		{
			m_CommunityGoalData.Insert(goal);
		}
	}
	
	array<ref ExpansionCommunityGoal> GetCommunityGoalsData()
	{
	    return m_CommunityGoalData;
	}
	
	ExpansionCommunityGoal GetCommunityGoalDataByID(int id)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
	    ExpansionCommunityGoal goal;
	  	foreach (int i, ExpansionCommunityGoal goalData: m_CommunityGoalData)
		{
			if (goalData.GetID() == id)
			{
				goal = goalData;
			}
		}
	    return goal;
	}
};
