/**
 * ExpansionCommunityGoalPlayerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCommunityGoalContribution
{
	int GoalID;
	int Points;

	void OnSend(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ctx.Write(GoalID);
		ctx.Write(Points);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(GoalID))
		{
			Error(ToString() + "::OnRecieve - GoalID");
			return false;
		}

		if (!ctx.Read(Points))
		{
			Error(ToString() + "::OnRecieve - Points");
			return false;
		}

		return true;
	}
};

class ExpansionCommunityGoalPlayerDataBase
{
	int Version;
};

class ExpansionCommunityGoalPlayerData: ExpansionCommunityGoalPlayerDataBase
{
	[NonSerialized()];
	protected static const int VERSION = 0;

	string UID;
	string Name;
	ref map<int, ref ExpansionCommunityGoalContribution> Contributions = new map<int, ref ExpansionCommunityGoalContribution>;

	void SetUID(string uid)
	{
		UID = uid;
	}

	string GetUID()
	{
		return UID;
	}

	void SetName(string name)
	{
		Name = name;
	}

	string GetName()
	{
		return Name;
	}

	void UpdateContribution(int goalID, int points)
	{
		ExpansionCommunityGoalContribution contribution;
		if (Contributions.Find(goalID, contribution))
		{
			contribution.Points = contribution.Points + points;
			Contributions.Set(goalID, contribution);
		}
		else
		{
			contribution = new ExpansionCommunityGoalContribution();
			contribution.GoalID = goalID;
			contribution.Points = points;
			Contributions.Insert(goalID, contribution);
		}
	}

	int GetGoalPoints(int goalID)
	{
		int points;
		ExpansionCommunityGoalContribution contribution;
		if (Contributions.Find(goalID, contribution))
		{
			return contribution.Points;
		}

		return points;
	}

	static ExpansionCommunityGoalPlayerData Load(string fileName)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionCommunityGoalPlayerData - Load");

		CF_Log.Info("[ExpansionCommunityGoalPlayerData] Load existing namalsk adventure community goal player data file:" + fileName);
		ExpansionCommunityGoalPlayerDataBase dataBase;
		ExpansionJsonFileParser<ExpansionCommunityGoalPlayerDataBase>.Load(fileName, dataBase);

		bool save;
		ExpansionCommunityGoalPlayerData data = new ExpansionCommunityGoalPlayerData();
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
			if (!ExpansionJsonFileParser<ExpansionCommunityGoalPlayerData>.Load(fileName, data))
				return NULL;
		}

		return data;
	}

	void Save()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionJsonFileParser<ExpansionCommunityGoalPlayerData>.Save(ExpansionCommunityGoalsModule.s_dataFolderPath + "contributors\\" + UID + ".json", this);
	}

	static void Save(ExpansionCommunityGoalPlayerData data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionCommunityGoalPlayerData - Save");

		ExpansionJsonFileParser<ExpansionCommunityGoalPlayerData>.Save(ExpansionCommunityGoalsModule.s_dataFolderPath + "contributors\\" + data.UID + ".json", data);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ctx.Write(Name);

		int contributionsCount = Contributions.Count();
		ctx.Write(contributionsCount);

		foreach (int goalID, ExpansionCommunityGoalContribution contribution: Contributions)
		{
			contribution.OnSend(ctx);
		}
	}

	void CopyFromBaseClass(ExpansionCommunityGoalPlayerDataBase base)
	{
		//! Nothing to do here yet
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(Name))
		{
			Error(ToString() + "::OnRecieve - Couldn't read Name");
			return false;
		}

		int contributionsCount;
		if (!ctx.Read(contributionsCount))
		{
			Error(ToString() + "::OnRecieve - Couldn't read contributionsCount");
			return false;
		}

		for (int i = 0; i < contributionsCount; i++)
		{
			ExpansionCommunityGoalContribution contribution = new ExpansionCommunityGoalContribution();
			if (!contribution.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRecieve - Couldn't read community goal contribution!");
				return false;
			}

			Contributions.Insert(contribution.GoalID, contribution);
		}

		return true;
	}
};