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
class ExpansionQuestObjectiveAICampEvent: ExpansionQuestObjectiveEventBase
{
	private int m_TotalKillCount = 0;
	private int m_UnitsToSpawn = 0;
	private int m_TotalUnitsAmount = 0;

	//! Event called when the player starts the quest
	override void OnStart()
	{
		ObjectivePrint(ToString() + "::OnStart - Start");

		CheckQuestAIPatrol();
		super.OnStart();

		ObjectivePrint(ToString() + "::OnStart - End");
	}

	//! Event called when the player continues the quest after a server restart/reconnect
	override void OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

		CheckQuestAIPatrol();
		super.OnContinue();

		ObjectivePrint(ToString() + "::OnContinue - End");
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override void OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		if (!GetQuest().GetQuestModule().IsOtherQuestInstanceActive(GetQuest()))
			CleanupPatrol();

		super.OnCleanup();

		ObjectivePrint(ToString() + "::OnCleanup - End");
	}

	//! Event called when the quest gets manualy canceled by the player.
	override void OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		if (!GetQuest().GetQuestModule().IsOtherQuestInstanceActive(GetQuest()))
			CleanupPatrol();

		super.OnCancel();

		ObjectivePrint(ToString() + "::OnCancel - End");
	}

	private void CleanupPatrol()
	{
		array<eAIDynamicPatrol> questPatrols;
		if (GetQuest().GetQuestModule().QuestPatrolExists(GetQuest().GetQuestConfig().GetID(), questPatrols))	
		{
			for (int i = 0; i < questPatrols.Count(); i++)
			{
				eAIDynamicPatrol patrol = questPatrols[i];
				if (patrol.m_CanSpawn)
					continue;
	
				eAIGroup group = patrol.m_Group;
				ObjectivePrint(ToString() + "::CleanupPatrol - Patrol: " + patrol.ToString());
				if (group)
				{
					ObjectivePrint(ToString() + "::CleanupPatrol - Patrol group: " + group.ToString());
					ObjectivePrint(ToString() + "::CleanupPatrol - Patrol group members: " + group.Count());
				}
	
				patrol.Despawn();
				patrol.Delete();
			}
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CleanupZeds);
		GetQuest().GetQuestModule().RemoveQuestPatrol(GetQuest().GetQuestConfig().GetID());
	}
	
	private bool KilledAICampMember(EntityAI victim)
	{
		DayZPlayerImplement victimPlayer;
		if (!Class.CastTo(victimPlayer, victim))
			return false;
			
		array<eAIDynamicPatrol> questPatrols;
		if (!GetQuest().GetQuestModule().QuestPatrolExists(GetQuest().GetQuestConfig().GetID(), questPatrols))
			return false;
		
		for (int i = 0; i < questPatrols.Count(); i++)
		{
			eAIDynamicPatrol patrol = questPatrols[i];
			if (!patrol)
				continue;

			eAIGroup group = patrol.m_Group;
			if (group && group.IsMember(victimPlayer))
				return true;
		}

		return false;
	}
	
	void OnEntityKilled(EntityAI victim, EntityAI killer)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
		if (!aiCamp)
			return;
		
		//! If the ai camp need to be killed with a special weapon check incoming killer class type
		if (aiCamp.NeedSpecialWeapon())
		{
			if (!ExpansionStatic.IsAnyOf(killer.GetType(), aiCamp.GetAllowedWeapons(), killer.ClassName()))
				return;
		}
		
		//! Check if killed entities class name is a valid one from our objective config
		bool found = ExpansionStatic.IsAnyOf(victim.GetType(), aiCamp.GetClassNames(), victim.ClassName());

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnEntityKilled - Target found: " + found);
	#endif

		if (!found)
			return;
		
		//! Check if killed ai entity was part of this objective event group
		if (KilledAICampMember(victim))	
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

	private void CheckQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - Start");
		
		if (!GetQuest() || !GetQuest().GetQuestModule() || !GetQuest().GetQuestConfig() || !GetObjectiveConfig())
			return;
		
		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
		if (!aiCamp)
			return;
		
		m_TotalUnitsAmount = aiCamp.GetPositions().Count();
		
		//! If all the targets are already killed dont create patrols
		if (m_TotalKillCount >= m_TotalUnitsAmount)
			return;
		
		array<eAIDynamicPatrol> questPatrols;
		if (GetQuest().GetQuestModule().QuestPatrolExists(GetQuest().GetQuestConfig().GetID(), questPatrols))
		{
			//! Check if the previous patrol groups related to this quest have been killed
			int killedPatrolCount;
			for (int i = 0; i < questPatrols.Count(); i++)
			{
				eAIDynamicPatrol questPatrol = questPatrols[i];
				if (!questPatrol)
					continue;
				
				if (questPatrol.WasGroupDestroyed())
					killedPatrolCount++;
			}
			
			//! If all patrols related to this quest have been wiped we can recreate all the patrols.
			if (killedPatrolCount == m_TotalUnitsAmount)
			{
				CreateQuestAIPatrol();
			}
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
		
		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
		if (!aiCamp)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;
		
		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		for (int i = 0; i < m_UnitsToSpawn; i++)
		{
			vector pos = aiCamp.GetPositions()[i];
			array<vector> waypoint = new array<vector>;
			waypoint.Insert(pos);

			ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(1, aiCamp.GetNPCSpeed(), aiCamp.GetNPCMode(), "HALT", aiCamp.GetNPCFaction(), aiCamp.GetNPCLoadoutFile(), GetObjectiveConfig().CanLootAI(), false, waypoint);
			group.Formation = "RANDOM";  //! Just set a default, it's not really used as the NPCs are separate
			group.AccuracyMin = aiCamp.NPCAccuracyMin;
			group.AccuracyMax = aiCamp.NPCAccuracyMax;
			eAIDynamicPatrol patrol = CreateQuestPatrol(group, 0, 600, 300, GetObjectiveConfig().GetMinDistRadius(), GetObjectiveConfig().GetMaxDistRadius(), GetObjectiveConfig().GetDespawnRadius());
			if (!patrol)
				return;

			questPatrols.Insert(patrol);
		}
		
		GetQuest().GetQuestModule().SetQuestPatrols(GetQuest().GetQuestConfig().GetID(), questPatrols);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CleanupZeds, 30000, true);
		
	#ifdef EXPANSIONMODNAVIGATION
		string markerName = GetQuest().GetQuestConfig().GetObjectives().Get(GetIndex()).GetObjectiveText();
		GetQuest().CreateClientMarker(aiCamp.GetPositions()[0], markerName);
	#endif
		
		ObjectivePrint(ToString() + "::CreateQuestAIPatrol - End");
	}
	
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
            Print("[QUESTS] WARNING: NumberOfAI shouldn't be set to 0, skipping this group...");
			return NULL;
		}

        if ( !group.Waypoints )
        {
            Print("!!! ERROR !!!");
            Print("[QUESTS] No waypoints (validate your file with a json validator)");
            Print("!!! ERROR !!!");
           return NULL;
        }

		vector startpos = group.Waypoints[0];
		if ( !startpos || startpos == "0 0 0" )
		{
			Print("!!! ERROR !!!");
			Print("[QUESTS] Couldn't find a spawn location. First waypoint is set to 0 0 0 or cannot be read by the system (validate your file with a json validator)");
			Print("!!! ERROR !!!");
			return NULL;
		}

		// Safety in case the Y is bellow the ground
		startpos = ExpansionStatic.GetSurfacePosition(startpos);
		if ( startpos[1] < group.Waypoints[0][1] )
			startpos[1] = group.Waypoints[0][1];

		Print("[QUESTS] Spawning "+aiSum+" "+group.Faction+" bots at "+group.Waypoints[0]+" and will patrol at "+group.Waypoints);

		eAIDynamicPatrol patrol = eAIDynamicPatrol.CreateEx(startpos, group.Waypoints, group.GetBehaviour(), group.LoadoutFile, aiSum, respawnTime, despawnTime, eAIFaction.Create(group.Faction), eAIFormation.Create(group.Formation), true, minDistRadius, maxDistRadius, despawnRadius, group.GetSpeed(), group.GetThreatSpeed(), group.CanBeLooted, group.UnlimitedReload);
        patrol.SetAccuracy(group.AccuracyMin, group.AccuracyMax);

		Print("=================== Expansion Quest AI Patrol ===================");
		return patrol;
	}

	private void CleanupZeds()
	{
		Print(ToString() + "::CleanupZeds - Start");

		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
		if (!aiCamp)
			return;

		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(aiCamp.GetPositions()[0], 500, objects, NULL);
		for (int i = 0; i < objects.Count(); i++)
		{
			Object obj = objects[i];
			if (obj.IsInherited(ZombieBase))
				GetGame().ObjectDelete(obj);
		}

		Print(ToString() + "::CleanupZeds - End");
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
		return ExpansionQuestObjectiveType.AICAMP;
	}
};
#endif