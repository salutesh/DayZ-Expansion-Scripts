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

		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_TotalUnitsAmount, aiPatrol.GetNPCSpeed(), aiPatrol.GetNPCMode(), aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), aiPatrol.GetStartPosition(), aiPatrol.GetWaypoints());
		AIGroupsData.Group.Insert(group);
		InitQuestPatrols();

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = "[Target] Bandit Patrol";
		GetQuest().CreateClientMarker(aiPatrol.GetStartPosition(), markerName);
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

		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_UnitsToSpawn, aiPatrol.GetNPCSpeed(), aiPatrol.GetNPCMode(), aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), aiPatrol.GetStartPosition(), aiPatrol.GetWaypoints());
		AIGroupsData.Group.Insert(group);
		InitQuestPatrols();

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = "[Target] Bandit Patrol";
		GetQuest().CreateClientMarker(aiPatrol.GetStartPosition(), markerName);
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
	
	void OnEntityKilled(string typeName, string killer)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		Print(ToString() + "::OnEntityKilled - Victim type: " + typeName);
		Print(ToString() + "::OnEntityKilled - Killer type: " + killer);

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;
		
		int findIndex;
		//! If the ai camp need to be killed with a special weapon check incoming killer class type
		if (aiPatrol.NeedSpecialWeapon())
		{
			findIndex = -1;
			findIndex = aiPatrol.GetAllowedWeapons().Find(killer);
			if (findIndex == -1)
				return;

			Print(ToString() + "::OnEntityKilled - Player has killed with special weapon!");
		}

		int amount = aiPatrol.GetNPCUnits();
		m_TotalUnitsAmount = amount;
		findIndex = -1;
		findIndex = aiPatrol.GetClassNames().Find(typeName);
		Print(ToString() + "::OnEntityKilled - Target find index: " + findIndex);

		if (findIndex > -1)
		{
			if (m_TotalKillCount < m_TotalUnitsAmount)
				m_TotalKillCount++;
		}
	}
	
	override void OnUpdate(float timeslice)
	{
		if (!IsInitialized() || IsCompleted())
			return;
		
		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{	
			if (m_TotalKillCount >= m_TotalUnitsAmount)
			{
				SetCompleted(true);
				OnComplete();
			}

			m_UpdateQueueTimer = 0.0;
		}
	}

	private void InitQuestPatrols()
	{
		ObjectivePrint(ToString() + "::InitQuestPatrols - Start");

		Print("=================== Expansion Quest Patrol AI ===================");
	    foreach (ExpansionQuestAIGroup group: AIGroupsData.Group)
	    {
			int aiSum;
			if ( group.NumberOfAI != 0 )
			{
				if ( group.NumberOfAI < 0 )
				{
					aiSum = Math.RandomInt(1,-group.NumberOfAI);
				}
				else
				{
					aiSum = group.NumberOfAI - m_KillCount;
				}
			}
			else
			{
	            Print("[QUESTS] WARNING: NumberOfAI shouldn't be set to 0, skipping this group...");
				continue;
			}

	        if ( !group.Waypoints )
	        {
	            Print("!!! ERROR !!!");
	            Print("[QUESTS] Couldn't read the Waypoints (validate your file with a json validator)");
	            Print("!!! ERROR !!!");
	            continue;
	        }

			vector startpos = group.StartPos;
			if ( !startpos || startpos == "0 0 0" )
			{
			    if ( !group.Waypoints[0] || group.Waypoints[0] == "0 0 0" )
	            {
	                Print("!!! ERROR !!!");
	                Print("[QUESTS] Couldn't find a spawn location. StartPos and at least the first Waypoints are both set to 0 0 0 or cannot be read by the system (validate your file with a json validator)");
	                Print("!!! ERROR !!!");
	                continue;
	            }

				startpos = group.Waypoints[0];
			}

			// Safety in case the Y is bellow the ground
			startpos = ExpansionStatic.GetSurfacePosition(startpos);
			if ( startpos[1] < group.StartPos[1] )
				startpos[1] = group.StartPos[1];

			Print("[QUESTS] Spawning "+aiSum+" "+group.Faction+" bots at "+group.StartPos+" and will patrol at "+group.Waypoints);
	 		Class.CastTo(QuestAIPatrol, eAIDynamicPatrol.Create(startpos, group.Waypoints, GetAIBehaviour(group.Behaviour), group.LoadoutFile, aiSum, AIGroupsData.RespawnTime, GetAIFaction(group.Faction), true, AIGroupsData.MinDistRadius, AIGroupsData.MaxDistRadius, GetAISpeed(group.Speed)));
		}
		Print("=================== Expansion Quest Patrol AI ===================");
		ObjectivePrint(ToString() + "::InitQuestPatrols - End");
	}

	private float GetAISpeed(string speed)
	{
	    switch (speed)
	    {
	        case "WALK":
	        {
	            return 1.0;
	            break;
	        }
	        case "JOG":
	        {
	            return 2.0;
	            break;
	        }
	        case "SPRINT":
	        {
	            return 3.0;
	            break;
	        }
	    }

	    //! Unknown speed, sending default speed
	    return 2.0;
	}

	private int GetAIBehaviour(string beh)
	{
	    switch (beh)
	    {
	        case "ALTERNATE":
	        {
	            return eAIWaypointBehavior.ALTERNATE; // Follow the waypoints in reverse (from end to start)
	            break;
	        }
	        case "HALT":
	        {
	            return eAIWaypointBehavior.HALT; // They just don't move, they stay at their position
	            break;
	        }
	        case "LOOP":
	        {
	            return eAIWaypointBehavior.LOOP; // Follow the waypoint's in the normal order (from start to finish)
	            break;
	        }
	    }

	    //! Unknown Behaviour, sending default behaviour
	    return eAIWaypointBehavior.ALTERNATE;
	}

	private eAIFaction GetAIFaction(string faction)
	{
	    switch (faction)
	    {
	        case "WEST":
	        {
	            return new eAIFactionWest(); // Friendly toward WEST and CIVILIANS
	            break;
	        }
	        case "EAST":
	        {
	            return new eAIFactionEast(); // Friendly toward EAST and CIVILIANS
	            break;
	        }
	        case "INSURGENT":
	        {
	            return new eAIFactionRaiders(); // Hostile toward everyone
	            break;
	        }
	        case "CIVILIAN":
	        {
	            return new eAIFactionCivilian(); // They like everyone
	            break;
	        }
	    }

	    //! Unknown Faction, sending default faction
	    return new eAIFactionCivilian();
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