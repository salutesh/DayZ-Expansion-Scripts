/**
 * ExpansionCommunityGoalsData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCommunityGoalsDataBase
{
	int Version;
};

class ExpansionCommunityGoalsData: ExpansionCommunityGoalsDataBase
{
	[NonSerialized()];
	static const int VERSION = 0;

	ref array<ref ExpansionCommunityGoal> CommunityGoals = new array<ref ExpansionCommunityGoal>;
	ref array<ref ExpansionCommunityGoalsBoard> CommunityGoalsBoards = new array<ref ExpansionCommunityGoalsBoard>;

	void ExpansionCommunityGoalsData()
	{
		Version = VERSION;
	}

	void CopyFromBaseClass(ExpansionCommunityGoalsDataBase base)
	{
		//! Nothing to do here yet
	}

	static ExpansionCommunityGoalsData Load(string fileName)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionCommunityGoalsData - Load");

		CF_Log.Info("[ExpansionCommunityGoalsData] Load existing community goals server data file:" + fileName);
		ExpansionCommunityGoalsDataBase dataBase;
		ExpansionJsonFileParser<ExpansionCommunityGoalsDataBase>.Load(fileName, dataBase);

		bool save;
		ExpansionCommunityGoalsData data = new ExpansionCommunityGoalsData();
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
			if (!ExpansionJsonFileParser<ExpansionCommunityGoalsData>.Load(fileName, data))
				return NULL;
		}

		return data;
	}

	void Save()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionJsonFileParser<ExpansionCommunityGoalsData>.Save(ExpansionCommunityGoalsModule.s_dataFolderPath + "CommunityGoals.json", this);
	}

	static void Save(ExpansionCommunityGoalsData data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionCommunityGoalsData - Save");

		ExpansionJsonFileParser<ExpansionCommunityGoalsData>.Save(ExpansionCommunityGoalsModule.s_dataFolderPath + "CommunityGoals.json", data);
	}

	void SetCommunityGoal(ExpansionCommunityGoal goal)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!CommunityGoals)
			CommunityGoals = new array<ref ExpansionCommunityGoal>;

		bool newData = true;
	   	foreach (int i, ExpansionCommunityGoal goalData: CommunityGoals)
		{
			if (goalData.GetID() == goal.GetID())
			{
				CommunityGoals.Remove(i);
				CommunityGoals.Insert(goal);
				Save();

				newData = false;
				return;
			}
		}

		if (newData)
		{
			CommunityGoals.Insert(goal);
			Save();
		}
	}

	void AddCommunityGoalBoard(ExpansionCommunityGoalsBoard goalBoard)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		CommunityGoalsBoards.Insert(goalBoard);
		Save();
	}

	array<ref ExpansionCommunityGoal> GetCommunityGoalsData()
	{
	    return CommunityGoals;
	}

	array<ref ExpansionCommunityGoalsBoard> GetCommunityGoalsBoards()
	{
		return CommunityGoalsBoards;
	}

	ExpansionCommunityGoal GetCommunityGoalDataByID(int id)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	    ExpansionCommunityGoal goal;
	  	foreach (int i, ExpansionCommunityGoal goalData: CommunityGoals)
		{
			if (goalData.GetID() == id)
			{
				goal = goalData;
			}
		}
	    return goal;
	}

	void LoadDefaults()
	{
		DefaultCommunityGoals();
		DefaultCommunityBoards();
	}

	protected void DefaultCommunityGoals()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!CommunityGoals)
			return;

		ExpansionCommunityGoal communityGoal = new ExpansionCommunityGoal();
		communityGoal.SetID(NamalskAdventureCommunityGoals.SURVIVORS_CAMP_SUPPLIES_1);
		communityGoal.SetFactionID(ExpansionNamalskAdventureFaction.SURVIVORS);
		communityGoal.SetGoalValue(100);
		communityGoal.SetName("Medical Supplies Delivery");
		communityGoal.SetDescription("Craft and deliver Medical Supplies to the Survivors camp to restock certain items at the trader.");
		SetCommunityGoal(communityGoal);

		communityGoal = new ExpansionCommunityGoal();
		communityGoal.SetID(NamalskAdventureCommunityGoals.SURVIVORS_CAMP_SUPPLIES_2);
		communityGoal.SetFactionID(ExpansionNamalskAdventureFaction.SURVIVORS);
		communityGoal.SetGoalValue(100);
		communityGoal.SetName("Ammunition Supplies Delivery");
		communityGoal.SetDescription("Craft and deliver Ammunition Supplies to the Survivors camp to restock certain items at the trader.");
		SetCommunityGoal(communityGoal);

		/*communityGoal = new ExpansionCommunityGoal();
		communityGoal.SetID(NamalskAdventureCommunityGoals.SURVIVORS_CAMP_REPAIR);
		communityGoal.SetFactionID(ExpansionNamalskAdventureFaction.SURVIVORS);
		communityGoal.SetGoalValue(1);
		communityGoal.SetName("");
		communityGoal.SetDescription("");
		SetCommunityGoal(communityGoal);*/
	}

	protected void DefaultCommunityBoards()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!CommunityGoalsBoards)
			return;

		ExpansionCommunityGoalsBoard communityGoalsBoard = new ExpansionCommunityGoalsBoard("Expansion_CommunityGoals_Board", "8596.825195 15.537852 10492.101563", "-158.963867 0.000000 -0.000000", ExpansionNamalskAdventureFaction.SURVIVORS);
		AddCommunityGoalBoard(communityGoalsBoard);
	}
};