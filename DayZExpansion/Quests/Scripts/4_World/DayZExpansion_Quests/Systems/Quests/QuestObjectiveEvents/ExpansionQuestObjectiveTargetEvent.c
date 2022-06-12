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

	void OnEntityKilled(string typeName, string killer)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		Print(ToString() + "::OnEntityKilled - Victim type: " + typeName);
		Print(ToString() + "::OnEntityKilled - Killer type: " + killer);

		ExpansionQuestObjectiveTarget target = GetObjectiveConfig().GetTarget();
		if (!target)
			return;

		//! If the target need to be killed with a special weapon check incoming killer class type
		int findIndex;
		if (target.NeedSpecialWeapon())
		{
			findIndex = -1;
			findIndex = target.GetAllowedWeapons().Find(killer);
			if (findIndex == -1)
				return;

			Print(ToString() + "::OnEntityKilled - Player has killed with special weapon!");
		}

		int amount = target.GetAmount();
		Amount = amount;
		findIndex = -1;
		findIndex = target.GetClassNames().Find(typeName);
		Print(ToString() + "::OnEntityKilled - Target find index: " + findIndex);

		if (findIndex > -1)
		{
			if (Count < amount)
				Count++;
		}
	}

	override void OnUpdate(float timeslice)
	{
		if (!IsInitialized() || IsCompleted())
			return;
		
		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			ExpansionQuestObjectiveTarget target = GetObjectiveConfig().GetTarget();
			if (!target)
				return;

			int amount = target.GetAmount();
			if (Count >= amount)
			{
				SetCompleted(true);
				OnComplete();
			}

			m_UpdateQueueTimer = 0.0;
		}
	}
	
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TARGET;
	}
};