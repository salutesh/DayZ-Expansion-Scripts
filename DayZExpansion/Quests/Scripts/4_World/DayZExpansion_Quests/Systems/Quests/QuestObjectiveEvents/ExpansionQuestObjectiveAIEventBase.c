/**
 * ExpansionQuestObjectiveAIPatrolEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAIEventBase: ExpansionQuestObjectiveEventBase
{
	protected int m_TotalKillCount = 0;
	protected int m_TotalUnitsAmount = 0;
	protected int m_UnitsToSpawn = 0;

	//! Event called when the player starts the quest.
	protected override bool OnEventStart()
	{
		if (!super.OnEventStart())
			return false;

		CheckQuestAIPatrol();

		return true;
	}

	//! Event called when the player starts the quest.
	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;

		//! Only check and create the AI patrol when not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED && !IsCompleted())
			CheckQuestAIPatrol();
		
		m_Quest.QuestCompletionCheck(true);

		return true;
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override bool OnCleanup()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnCleanup())
			return false;

		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(m_Quest.GetQuestConfig().GetID()))
			CleanupPatrol(true);

		return true;
	}

	//! Event called when the quest gets manualy canceled by the player.
	override bool OnCancel()
	{
		if (!super.OnCancel())
			return false;

		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(m_Quest.GetQuestConfig().GetID()))
			CleanupPatrol(true);

		return true;
	}

	override bool OnComplete()
	{
		if (!super.OnComplete())
			return false;

		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(m_Quest.GetQuestConfig().GetID()))
			CleanupPatrol();

		return true;
	}

	protected void CleanupPatrol(bool despawn = false)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		array<eAIQuestPatrol> questPatrols;
		if (ExpansionQuestModule.GetModuleInstance().QuestPatrolExists(m_Quest.GetQuestConfig().GetID(), questPatrols))
		{
			foreach(eAIQuestPatrol patrol: questPatrols)
			{
				eAIGroup group = patrol.m_Group;
				ObjectivePrint("Patrol: " + patrol.ToString());
				if (group)
				{
					ObjectivePrint("Patrol group: " + group.ToString());
					ObjectivePrint("Patrol group members: " + group.Count());
				}

				if (despawn)
					patrol.Despawn();

				patrol.Delete();
			}
		}

		ExpansionQuestModule.GetModuleInstance().RemoveQuestPatrol(m_Quest.GetQuestConfig().GetID());
	}

	protected bool KilledAIMember(EntityAI victim)
	{
		DayZPlayerImplement victimPlayer;
		if (!Class.CastTo(victimPlayer, victim))
		{
			ObjectivePrint("::KilledAIMember - F1 - FALSE");
			return false;
		}

		array<eAIQuestPatrol> questPatrols = {};
		if (!ExpansionQuestModule.GetModuleInstance().QuestPatrolExists(m_Quest.GetQuestConfig().GetID(), questPatrols))
		{
			ObjectivePrint("::KilledAIMember - F2 - FALSE");
			return false;
		}

		foreach (eAIQuestPatrol patrol: questPatrols)
		{
			eAIGroup group = patrol.m_Group;
			if (group && group.IsMember(victimPlayer))
			{
				ObjectivePrint("::KilledAIMember - TRUE");
				return true;
			}
		}

		ObjectivePrint("::KilledAIMember - F3 - FALSE");
		return false;
	}

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null, map<Man, ref ExpansionEntityHitInfo> hitMap = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		//! Check if killed ai entity was part of this objective event group
		if (KilledAIMember(victim))
		{
			ObjectivePrint("::OnEntityKilled - Entity killed was a valid target! Type: " + victim.GetType());
			if (m_TotalKillCount < m_TotalUnitsAmount)
			{
				m_TotalKillCount++;
				m_Quest.QuestCompletionCheck(true);
			}

			if (m_TotalKillCount >= m_TotalUnitsAmount)
				RemoveObjectiveMarkers();
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			ObjectivePrint("::OnEntityKilled - Entity killed was not a valid target! Type: " + victim.GetType());
		}
	#endif
	}

	protected void CheckQuestAIPatrol();

	protected void CheckQuestAIPatrol(int patrolsToKill)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		//! If all the targets are already killed dont create patrols
		if (m_TotalKillCount >= m_TotalUnitsAmount)
			return;

		array<eAIQuestPatrol> questPatrols;
		if (ExpansionQuestModule.GetModuleInstance().QuestPatrolExists(m_Quest.GetQuestConfig().GetID(), questPatrols))
		{
			//! Check if the previous patrol groups related to this quest have been killed
			int killedPatrolCount;
			foreach (eAIQuestPatrol questPatrol: questPatrols)
			{
				if (questPatrol.WasGroupDestroyed())
					killedPatrolCount++;
			}

			//! If all patrols related to this quest have been wiped we can recreate all the patrols.
			if (killedPatrolCount == patrolsToKill)
			{
				CreateQuestAIPatrol();
			}
		}
		else
		{
			CreateQuestAIPatrol();
		}
	}

	protected void CreateQuestAIPatrol();

	void SetKillCount(int count)
	{
		m_TotalKillCount = count;
	}

	int GetCount()
	{
		return m_TotalKillCount;
	}

	void SetKillAmount(int amount)
	{
		m_TotalUnitsAmount = amount;
	}

	int GetAmount()
	{
		return m_TotalUnitsAmount;
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ObjectivePrint("m_TotalKillCount: " + m_TotalKillCount);
		ObjectivePrint("m_TotalUnitsAmount: " + m_TotalUnitsAmount);

		if (!super.CanComplete())
			return false;

		//! @note need to check for zero here because m_TotalUnitsAmount will be zero until set
		if (m_TotalUnitsAmount > 0 && m_TotalKillCount >= m_TotalUnitsAmount)
		{
			ObjectivePrint("End and return: TRUE");
			return true;
		}

		ObjectivePrint("End and return: FALSE");

		return false;
	}

	override bool CanCreateMarkers()
	{
		return (m_TotalKillCount < m_TotalUnitsAmount);
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AICAMP;
	}
};
#endif