/**
 * ExpansionCommunityGoal.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCommunityGoal
{
    protected int ID;
	#ifdef EXPANSIONMODAI
	protected int FactionID;
	#endif
    protected int GoalValue;
    protected int GoalProgress;
	protected bool Finished;
	protected string Name;
	protected string Desc;

	void SetID(int id)
	{
		ID = id;
	}

    int GetID()
    {
        return ID;
    }

	#ifdef EXPANSIONMODAI
	void SetFactionID(int factionID)
	{
		FactionID = factionID;
	}

	int GetFactionID()
	{
		return FactionID;
	}
	#endif

    int GetProgress()
    {
        return GoalProgress;
    }

	void SetGoalValue(int goalVal)
	{
		GoalValue = goalVal;
	}

    int GetGoal()
    {
        return GoalValue;
    }

    void AddProgress(int progress)
    {
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

        GoalProgress = GoalProgress + progress;
    }

	void SetFinished()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		Finished = true;
	}

	bool IsFinished()
	{
		return Finished;
	}

	void SetName(string name)
	{
		Name = name;
	}

	string GetName()
	{
		return Name;
	}

	void SetDescription(string desc)
	{
		Desc = desc;
	}

	string GetDescription()
	{
		return Desc;
	}

	void OnSend(ParamsWriteContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		ctx.Write(ID);
		#ifdef EXPANSIONMODAI
		ctx.Write(FactionID);
		#endif
		ctx.Write(GoalValue);
		ctx.Write(GoalProgress);
		ctx.Write(Name);
		ctx.Write(Desc);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ID))
		{
			Error(ToString() + "::OnRecieve - ID");
			return false;
		}

		#ifdef EXPANSIONMODAI
		if (!ctx.Read(FactionID))
		{
			Error(ToString() + "::OnRecieve - FactionID");
			return false;
		}
		#endif

		if (!ctx.Read(GoalValue))
		{
			Error(ToString() + "::OnRecieve - GoalValue");
			return false;
		}

		if (!ctx.Read(GoalProgress))
		{
			Error(ToString() + "::OnRecieve - GoalProgress");
			return false;
		}

		if (!ctx.Read(Name))
		{
			Error(ToString() + "::OnRecieve - Name");
			return false;
		}

		if (!ctx.Read(Desc))
		{
			Error(ToString() + "::OnRecieve - Desc");
			return false;
		}

		return true;
	}
};