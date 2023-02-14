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
class ExpansionQuestObjectiveAIEventBase: ExpansionQuestObjectiveEventBase
{
	protected int m_TotalKillCount = 0;
	protected int m_TotalUnitsAmount = 0;
	protected int m_UnitsToSpawn = 0;

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase OnEventStart
	// -----------------------------------------------------------
	//! Event called when the player starts the quest.
	protected override bool OnEventStart()
	{
		if (!super.OnEventStart())
			return false;

		CheckQuestAIPatrol();

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase OnContinue
	// -----------------------------------------------------------
	//! Event called when the player starts the quest.
	protected override bool OnContinue()
	{
		if (!super.OnContinue())
			return false;

		CheckQuestAIPatrol();
		m_Quest.QuestCompletionCheck();

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase OnCleanup
	// -----------------------------------------------------------
	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override bool OnCleanup()
	{
		if (!super.OnCleanup())
			return false;

		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(m_Quest))
			CleanupPatrol();

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase OnCancel
	// -----------------------------------------------------------
	//! Event called when the quest gets manualy canceled by the player.
	override bool OnCancel()
	{
		if (!super.OnCancel())
			return false;

		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(m_Quest))
			CleanupPatrol(true);

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase OnComplete
	// -----------------------------------------------------------
	override bool OnComplete()
	{
		if (!super.OnComplete())
			return false;

		if (ExpansionQuestModule.GetModuleInstance().CanDeleteQuestPatrol(m_Quest.GetQuestConfig().GetID()))
		{
			CleanupPatrol();
		}

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CleanupPatrol
	// -----------------------------------------------------------
	protected void CleanupPatrol(bool despawn = false)
	{
		array<eAIDynamicPatrol> questPatrols;
		if (ExpansionQuestModule.GetModuleInstance().QuestPatrolExists(m_Quest.GetQuestConfig().GetID(), questPatrols))
		{
			foreach(eAIDynamicPatrol patrol: questPatrols)
			{
				eAIGroup group = patrol.m_Group;
				ObjectivePrint(ToString() + "::CleanupPatrol - Patrol: " + patrol.ToString());
				if (group)
				{
					ObjectivePrint(ToString() + "::CleanupPatrol - Patrol group: " + group.ToString());
					ObjectivePrint(ToString() + "::CleanupPatrol - Patrol group members: " + group.Count());
				}

				if (despawn)
					patrol.Despawn();

				patrol.Delete();
			}
		}

		ExpansionQuestModule.GetModuleInstance().RemoveQuestPatrol(m_Quest.GetQuestConfig().GetID());
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase KilledAIMember
	// -----------------------------------------------------------
	protected bool KilledAIMember(EntityAI victim)
	{
		DayZPlayerImplement victimPlayer;
		if (!Class.CastTo(victimPlayer, victim))
			return false;

		array<eAIDynamicPatrol> questPatrols;
		if (!ExpansionQuestModule.GetModuleInstance().QuestPatrolExists(m_Quest.GetQuestConfig().GetID(), questPatrols))
			return false;

		foreach (eAIDynamicPatrol patrol: questPatrols)
		{
			eAIGroup group = patrol.m_Group;
			if (group && group.IsMember(victimPlayer))
				return true;
		}

		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase OnEntityKilled
	// -----------------------------------------------------------
	void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
		//! Check if killed ai entity was part of this objective event group
		if (KilledAIMember(victim))
		{
			if (m_TotalKillCount < m_TotalUnitsAmount)
			{
				m_TotalKillCount++;
				m_Quest.UpdateQuest();
				m_Quest.QuestCompletionCheck();
			}
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CheckQuestAIPatrol
	// -----------------------------------------------------------
	protected void CheckQuestAIPatrol();

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CheckQuestAIPatrol
	// -----------------------------------------------------------
	protected void CheckQuestAIPatrol(int patrolsToKill)
	{
		//! If all the targets are already killed dont create patrols
		if (m_TotalKillCount >= m_TotalUnitsAmount)
			return;

		array<eAIDynamicPatrol> questPatrols;
		if (ExpansionQuestModule.GetModuleInstance().QuestPatrolExists(m_Quest.GetQuestConfig().GetID(), questPatrols))
		{
			//! Check if the previous patrol groups related to this quest have been killed
			int killedPatrolCount;
			foreach (eAIDynamicPatrol questPatrol: questPatrols)
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

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CreateQuestAIPatrol
	// -----------------------------------------------------------
	protected void CreateQuestAIPatrol();

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CreateQuestPatrol
	// -----------------------------------------------------------
	static eAIDynamicPatrol CreateQuestPatrol(ExpansionQuestAIGroup group, int killCount = 0, int respawnTime = -1, int despawnTime = 0, float minDistRadius = 20, float maxDistRadius = 600, float despawnRadius = 880)
	{
		Print("=================== Expansion Quest AI Patrol ===================");
		int aiSum;
		if ( group.NumberOfAI != 0 )
		{
			if ( group.NumberOfAI < 0 )
			{
				aiSum = Math.RandomInt(1,-group.NumberOfAI);
			}
			else
			{
				aiSum = group.NumberOfAI - killCount;
			}
		}
		else
		{
            Error("[QUESTS] WARNING: NumberOfAI shouldn't be set to 0, skipping this group...");
			return NULL;
		}

        if ( !group.Waypoints )
        {
        Error("[QUESTS] No waypoints (validate your file with a json validator)");
           return NULL;
        }

		vector startpos = group.Waypoints[0];
		if ( !startpos || startpos == "0 0 0" )
		{
			Error("[QUESTS] Couldn't find a spawn location. First waypoint is set to 0 0 0 or cannot be read by the system (validate your file with a json validator)");
			return NULL;
		}

		// Safety in case the Y is bellow the ground
		startpos = ExpansionStatic.GetSurfacePosition(startpos);
		if ( startpos[1] < group.Waypoints[0][1] )
			startpos[1] = group.Waypoints[0][1];

		Print("[QUESTS] Created trigger for "+aiSum+" "+group.Faction+" bots at "+group.Waypoints[0]);

		eAIDynamicPatrol patrol = eAIDynamicPatrol.CreateEx(startpos, group.Waypoints, group.GetBehaviour(), group.LoadoutFile, aiSum, respawnTime, despawnTime, eAIFaction.Create(group.Faction), eAIFormation.Create(group.Formation), true, minDistRadius, maxDistRadius, despawnRadius, group.GetSpeed(), group.GetThreatSpeed(), group.CanBeLooted, group.UnlimitedReload);
        patrol.SetAccuracy(group.AccuracyMin, group.AccuracyMax);

		Print("=================== Expansion Quest AI Patrol ===================");
		return patrol;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CleanupZeds
	// -----------------------------------------------------------
	protected void CleanupZeds()
	{
		ExpansionQuestObjectiveAICamp aiCamp = m_ObjectiveConfig.GetAICamp();
		if (!aiCamp)
			return;

		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(aiCamp.GetPositions()[0], 500, objects, NULL);
		foreach (Object obj: objects)
		{
			if (obj.IsInherited(ZombieBase))
				GetGame().ObjectDelete(obj);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase SetKillCount
	// -----------------------------------------------------------
	void SetKillCount(int count)
	{
		m_TotalKillCount = count;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase GetCount
	// -----------------------------------------------------------
	int GetCount()
	{
		return m_TotalKillCount;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase GetAmount
	// -----------------------------------------------------------
	int GetAmount()
	{
		return m_TotalUnitsAmount;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CanComplete
	// -----------------------------------------------------------
	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_TotalKillCount: " + m_TotalKillCount);
		ObjectivePrint(ToString() + "::CanComplete - m_TotalUnitsAmount: " + m_TotalUnitsAmount);

		bool conditionsResult = (m_TotalKillCount == m_TotalUnitsAmount);
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIEventBase CanComplete
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AICAMP;
	}
};
#endif