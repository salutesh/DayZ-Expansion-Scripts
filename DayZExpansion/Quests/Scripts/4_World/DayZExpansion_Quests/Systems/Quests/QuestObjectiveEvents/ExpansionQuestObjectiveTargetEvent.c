/**
 * ExpansionQuestObjectiveTargetEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTargetEvent: ExpansionQuestObjectiveEventBase
{
	private int Count = 0;
	private int Amount = 0;
	private float m_UpdateQueueTimer = 0;
	private const float UPDATE_TICK_TIME = 2.0;

	void SetCount(int count)
	{
		Count = count;
	}

	int GetCount()
	{
		return Count;
	}

	int GetAmount()
	{
		return Amount;
	}

	override void OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		ExpansionQuestObjectiveTarget target = GetObjectiveConfig().GetTarget();
		if (!target)
			return;

		Amount = target.GetAmount();

		super.OnStart();
	}

	override void OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		ExpansionQuestObjectiveTarget target = GetObjectiveConfig().GetTarget();
		if (!target)
			return;

		Amount = target.GetAmount();

		super.OnContinue();
	}

	void OnEntityKilled(EntityAI victim, EntityAI killer)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		string className = victim.ClassName();
		string killerName = killer.GetType();
		bool maxRangeCheck = false;
		
		Print(ToString() + "::OnEntityKilled - Victim class name: " + victim.ClassName());
		Print(ToString() + "::OnEntityKilled - Victim type: " + victim.GetType());
		
		//! Use max range check if used in config
		if (GetObjectiveConfig().GetMaxDistance() != -1)
			maxRangeCheck = true;

		PlayerBase killerPlayer;
		vector playerPos;
		if (Class.CastTo(killerPlayer, killer))
		{
			if (maxRangeCheck && !IsInMaxRange(killerPlayer.GetPosition()))
				return;
		}
		else if (Class.CastTo(killerPlayer, killer.GetHierarchyParent()))
		{
			if (maxRangeCheck && !IsInMaxRange(killerPlayer.GetPosition()))
			{
				Print(ToString() + "::OnEntityKilled - Killer is out of legit kill range!");
				return;
			}
		}

		ExpansionQuestObjectiveTarget target = GetObjectiveConfig().GetTarget();
		if (!target)
			return;

		//! If the target need to be killed with a special weapon check incoming killer class type
		int findIndex;
		if (target.NeedSpecialWeapon())
		{
			findIndex = -1;
			findIndex = target.GetAllowedWeapons().Find(killerName);
			if (findIndex == -1)
				return;

			Print(ToString() + "::OnEntityKilled - Player has killed with special weapon!");
		}

		int amount = target.GetAmount();
		Amount = amount;
		findIndex = -1;
		findIndex = target.GetClassNames().Find(className);
		Print(ToString() + "::OnEntityKilled - Target find index: " + findIndex);

		if (findIndex > -1)
		{
			if (Count < amount)
			{
				Count++;
				
				if (GetQuest())
					GetQuest().UpdateQuestPlayersObjectiveData();
			}
		}
		
		if (Count >= amount)
		{
			SetCompleted(true);
			OnComplete();
		}
	}

	private bool IsInMaxRange(vector playerPos)
	{
		vector position = GetObjectiveConfig().GetPosition();
		float maxDistance = GetObjectiveConfig().GetMaxDistance();
		float currentDistance = vector.Distance(playerPos, position);
		position[1] = GetGame().SurfaceY(position[0], position[2]);

		if (position != vector.Zero && currentDistance <= maxDistance)
		{
			return true;
		}

		return false;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TARGET;
	}
};