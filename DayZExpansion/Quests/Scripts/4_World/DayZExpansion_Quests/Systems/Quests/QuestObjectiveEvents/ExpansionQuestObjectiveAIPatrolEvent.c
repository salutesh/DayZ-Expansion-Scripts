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
	private ref ExpansionQuestAIGroups AIGroupsData;
	private eAIDynamicPatrol QuestAIPatrol;
	private int m_TotalUnitsAmount = 0;
	private int m_TotalKillCount = 0;
	private int m_UnitsToSpawn = 0;
	private float m_UpdateQueueTimer = 0;
	private const float UPDATE_TICK_TIME = 2.0;
	private int m_UnitsAmount = 0;
	private int m_KillCount = 0;

	//! Event called when the player starts the quest
	override void OnStart()
	{
		ObjectivePrint(ToString() + "::OnStart - Start");

		if (!GetObjectiveConfig() || !GetQuest() || !GetQuest().GetQuestConfig())
			return;

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		if (!AIGroupsData)
			AIGroupsData = new ExpansionQuestAIGroups();

		m_UnitsAmount = aiPatrol.GetNPCUnits();
		m_TotalUnitsAmount = m_UnitsAmount;

		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_TotalUnitsAmount, aiPatrol.GetNPCSpeed(), aiPatrol.GetNPCMode(), "ALTERNATE", aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), true, false, aiPatrol.GetWaypoints());
		AIGroupsData.Group = group;
		InitQuestPatrols();

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = "[Target] Bandit Patrol";
		GetQuest().CreateClientMarker(aiPatrol.GetWaypoints()[0], markerName);
	#endif

		super.OnStart();

		ObjectivePrint(ToString() + "::OnStart - End");
	}

	//! Event called when the player continues the quest after a server restart/reconnect
	override void OnContinue()
	{
		ObjectivePrint(ToString() + "::OnStart - Start");

		if (!GetObjectiveConfig() || !GetQuest() || !GetQuest().GetQuestConfig())
			return;

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		m_TotalUnitsAmount = aiPatrol.GetNPCUnits();

		//! If all the targets are already killed dont create patrol group
		if (m_TotalKillCount >= m_TotalUnitsAmount)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		if (m_UnitsToSpawn <= 0)
			return;

		if (!AIGroupsData)
			AIGroupsData = new ExpansionQuestAIGroups();

		m_UnitsAmount = aiPatrol.GetNPCUnits();

		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_TotalUnitsAmount, aiPatrol.GetNPCSpeed(), aiPatrol.GetNPCMode(), "ALTERNATE", aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), true, false, aiPatrol.GetWaypoints());
		AIGroupsData.Group = group;
		InitQuestPatrols();

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = "[Target] Bandit Patrol";
		GetQuest().CreateClientMarker(aiPatrol.GetWaypoints()[0], markerName);
	#endif

		super.OnStart();

		ObjectivePrint(ToString() + "::OnStart - End");
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override void OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		eAIGroup group = QuestAIPatrol.m_Group;
		if (group)
		{
			for (int j = 0; j < group.Count(); j++)
			{
				DayZPlayerImplement member = group.GetMember(j);
				if (!member || member && !member.IsAlive())
					continue;

				ObjectivePrint(ToString() + "::OnCancel - Delete member: [" + j + "] " + member.ToString());

				GetGame().ObjectDelete(member);
			}

			group.Delete();
			QuestAIPatrol.Delete();
		}

		super.OnCleanup();

		ObjectivePrint(ToString() + "::OnCleanup - End");
	}

	//! Event called when the quest gets manualy canceled by the player.
	override void OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		eAIGroup group = QuestAIPatrol.m_Group;
		if (group)
		{
			for (int j = 0; j < group.Count(); j++)
			{
				DayZPlayerImplement member = group.GetMember(j);
				if (!member)
					continue;

				ObjectivePrint(ToString() + "::OnCancel - Delete member: [" + j + "] " + member.ToString());

				GetGame().ObjectDelete(member);
			}

			group.Delete();
			QuestAIPatrol.Delete();
		}

		super.OnCancel();

		ObjectivePrint(ToString() + "::OnCancel - End");
	}

	void OnEntityKilled(EntityAI victim, EntityAI killer)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		int findIndex;
		//! If the ai camp need to be killed with a special weapon check incoming killer class type
		if (aiPatrol.NeedSpecialWeapon())
		{
			findIndex = -1;
			findIndex = aiPatrol.GetAllowedWeapons().Find(killer.GetType());
			if (findIndex == -1)
				return;
		}
		
		//! Check if killed entities class name is a valid one from our objective config
		int amount = aiPatrol.GetNPCUnits();
		m_TotalUnitsAmount = amount;
		findIndex = -1;
		findIndex = aiPatrol.GetClassNames().Find(victim.ClassName());

		if (findIndex == -1)
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
	
	private bool KilledAIPatrolMember(EntityAI victim)
	{
		DayZPlayerImplement victimPlayer;
		if (!Class.CastTo(victimPlayer, victim))
			return false;

		if (!QuestAIPatrol || QuestAIPatrol.m_CanSpawn)
			return false;

		eAIGroup group = QuestAIPatrol.m_Group;
		if (group && group.IsMember(victimPlayer))
		{
			return true;
		}

		return false;
	}

	private void InitQuestPatrols()
	{
		ObjectivePrint(ToString() + "::InitQuestPatrols - Start");
		QuestAIPatrol = ExpansionQuestObjectiveAICampEvent.CreateQuestPatrol(AIGroupsData.Group, m_KillCount, AIGroupsData.RespawnTime, AIGroupsData.MinDistRadius, AIGroupsData.MaxDistRadius);
		ObjectivePrint(ToString() + "::InitQuestPatrols - End");
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