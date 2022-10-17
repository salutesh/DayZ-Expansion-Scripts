/**
 * ExpansionQuestObjectiveAIPatrolEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAIPatrolEvent: ExpansionQuestObjectiveEventBase
{
	protected int m_TotalUnitsAmount = 0;
	protected int m_TotalKillCount = 0;
	protected int m_UnitsToSpawn = 0;

	//! Event called when the player starts the quest
	override bool OnStart()
	{
		ObjectivePrint(ToString() + "::OnStart - Start");

		if (!super.OnStart())
			return false;

		CheckQuestAIPatrol();

		return true;
	}

	//! Event called when the player continues the quest after a server restart/reconnect
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnStart - Start");

		if (!super.OnContinue())
			return false;

		CheckQuestAIPatrol();

		return true;
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override bool OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		if (!super.OnCleanup())
			return false;

		if (!GetQuest().GetQuestModule().IsOtherQuestInstanceActive(GetQuest()))
			CleanupPatrol();

		return true;
	}

	//! Event called when the quest gets manualy canceled by the player.
	override bool OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		if (!super.OnCancel())
			return false;

		if (!GetQuest().GetQuestModule().IsOtherQuestInstanceActive(GetQuest()))
			CleanupPatrol();

		return true;
	}

	override bool OnComplete()
	{
		ObjectivePrint(ToString() + "::OnComplete - Start");

		if (!super.OnComplete())
			return false;

		if (GetQuest().GetQuestModule().CanDeleteQuestPatrol(GetQuest().GetQuestConfig().GetID()))
		{
			CleanupPatrol();
		}

		return true;
	}

	void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		//! If the ai camp need to be killed with a special weapon check incoming killer class type
		if (aiPatrol.NeedSpecialWeapon())
		{
			if (!ExpansionStatic.IsAnyOf(killer.GetType(), aiPatrol.GetAllowedWeapons(), killer.ClassName()))
				return;
		}

		//! Check if killed entities class name is a valid one from our objective config
		int amount = aiPatrol.GetNPCUnits();
		m_TotalUnitsAmount = amount;
		bool found = ExpansionStatic.IsAnyOf(victim.GetType(), aiPatrol.GetClassNames(), victim.ClassName());

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnEntityKilled - Target found: " + found);
	#endif

		if (!found)
			return;

		//! Check if killed ai entity was part of this objective event group
		if (KilledAIPatrolMember(victim))
		{
			if (m_TotalKillCount < m_TotalUnitsAmount)
			{
				m_TotalKillCount++;

				if (GetQuest())
					GetQuest().UpdateQuestPlayersObjectiveData();
			}
		}

		if (m_TotalKillCount >= m_TotalUnitsAmount)
		{
			SetCompleted(true);
			OnComplete();
		}
	}

	protected bool KilledAIPatrolMember(EntityAI victim)
	{
		DayZPlayerImplement victimPlayer;
		if (!Class.CastTo(victimPlayer, victim))
			return false;

		array<eAIDynamicPatrol> questPatrols;
		if (!GetQuest().GetQuestModule().QuestPatrolExists(GetQuest().GetQuestConfig().GetID(), questPatrols))
			return false;

		foreach (eAIDynamicPatrol patrol: questPatrols)
		{
			eAIGroup group = patrol.m_Group;
			if (group && group.IsMember(victimPlayer))
				return true;
		}

		return false;
	}

	protected void CheckQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - Start");

		if (!GetQuest() || !GetQuest().GetQuestModule() || !GetQuest().GetQuestConfig() || !GetObjectiveConfig())
			return;

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		m_TotalUnitsAmount = aiPatrol.GetNPCUnits();

		//! If all the targets are already killed dont create patrols
		if (m_TotalKillCount >= m_TotalUnitsAmount)
			return;

		array<eAIDynamicPatrol> questPatrols;
		if (GetQuest().GetQuestModule().QuestPatrolExists(GetQuest().GetQuestConfig().GetID(), questPatrols))
		{
			//! Check if the previous patrol groups related to this quest have been killed
			int killedPatrolCount;
			foreach (eAIDynamicPatrol questPatrol: questPatrols)
			{
				if (questPatrol.WasGroupDestroyed())
					killedPatrolCount++;
			}

			//! If all patrols related to this quest have been wiped we can recreate all the patrols.
			if (killedPatrolCount == 1)
				CreateQuestAIPatrol();
		}
		else
		{
			CreateQuestAIPatrol();
		}

		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - End");
	}

	void CreateQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CreateQuestAIPatrol - Start");

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_UnitsToSpawn, aiPatrol.GetNPCSpeed(), aiPatrol.GetNPCMode(), "ALTERNATE", aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), GetObjectiveConfig().CanLootAI(), false, aiPatrol.GetWaypoints());
		group.Formation = aiPatrol.NPCFormation;
		group.AccuracyMin = aiPatrol.NPCAccuracyMin;
		group.AccuracyMax = aiPatrol.NPCAccuracyMax;
		eAIDynamicPatrol patrol = ExpansionQuestObjectiveAICampEvent.CreateQuestPatrol(group, 0, 600, 300, GetObjectiveConfig().GetMinDistRadius(), GetObjectiveConfig().GetMaxDistRadius(), GetObjectiveConfig().GetDespawnRadius());
		if (!patrol)
			return;

		questPatrols.Insert(patrol);
		GetQuest().GetQuestModule().SetQuestPatrols(GetQuest().GetQuestConfig().GetID(), questPatrols);

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = GetQuest().GetQuestConfig().GetObjectives().Get(GetIndex()).GetObjectiveText();
		if (markerName != string.Empty)
			GetQuest().CreateClientMarker(aiPatrol.GetWaypoints()[0], markerName);
	#endif

		ObjectivePrint(ToString() + "::CreateQuestAIPatrol - End");
	}

	protected void CleanupPatrol()
	{
		array<eAIDynamicPatrol> questPatrols;
		if (GetQuest().GetQuestModule().QuestPatrolExists(GetQuest().GetQuestConfig().GetID(), questPatrols))
		{
			foreach (eAIDynamicPatrol patrol: questPatrols)
			{
				if (patrol.m_CanSpawn)
					continue;

				eAIGroup group = patrol.m_Group;
				if (group)
				{
					ObjectivePrint(ToString() + "::CleanupPatrol - Patrol: " + patrol.ToString());
					ObjectivePrint(ToString() + "::CleanupPatrol - Patrol group: " + group.ToString());
					ObjectivePrint(ToString() + "::CleanupPatrol - Patrol group members: " + group.Count());
				}

				patrol.Despawn();
				patrol.Delete();
			}
		}

		GetQuest().GetQuestModule().RemoveQuestPatrol(GetQuest().GetQuestConfig().GetID());
	}

	void SetKillCount(int count)
	{
		m_TotalKillCount = count;
	}

	int GetCount()
	{
		return m_TotalKillCount;
	}

	int GetAmount()
	{
		return m_TotalUnitsAmount;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIPATROL;
	}
};
#endif