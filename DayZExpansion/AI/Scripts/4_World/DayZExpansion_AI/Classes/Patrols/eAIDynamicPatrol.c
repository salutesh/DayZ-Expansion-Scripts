class eAIDynamicPatrol : eAIPatrol
{
	private static int m_NumberOfDynamicPatrols;

	vector m_Position;
	autoptr array<vector> m_Waypoints;
	eAIWaypointBehavior m_WaypointBehaviour;
	float m_MinimumRadius;
	float m_MaximumRadius;
	float m_DespawnRadius;
	float m_MovementSpeedLimit;
	float m_MovementThreatSpeedLimit;
	ref ExpansionArray<string> m_Units;
	int m_NumberOfAI;
	int m_RespawnTime; // negative respawn time = patrol won't respawn
	int m_DespawnTime; // if all players outside despawn radius, ticks up time. When despawn time reached, patrol is deleted
	string m_Loadout;
	ref eAIFaction m_Faction;
	ref eAIFormation m_Formation;
	bool m_CanBeLooted;
	bool m_UnlimitedReload;
	float m_SniperProneDistanceThreshold;
	float m_AccuracyMin; // zero or negative = use general setting
	float m_AccuracyMax; // zero or negative = use general setting
	float m_ThreatDistanceLimit; // zero or negative = use general setting
	float m_NoiseInvestigationDistanceLimit; // zero or negative = use general setting
	float m_DamageMultiplier; // zero or negative = use general setting
	float m_DamageReceivedMultiplier; // zero or negative = use general setting

	eAIGroup m_Group;
	string m_GroupName;
	float m_TimeSinceLastSpawn;
	bool m_CanSpawn;
	private bool m_WasGroupDestroyed;
	private eAIDynamicPatrolSphereTrigger m_Trigger;

	void ~eAIDynamicPatrol()
	{
		if (GetGame() && m_Trigger)
			GetGame().ObjectDelete(m_Trigger);
	}

	/**
	 * @brief Creates a dynamic patrol which spawns a patrol under the right conditions.
	 * 
	 * @param pos the position that the trigger distance is calculated from
	 * @param waypoints an array of points which the patrol will traverse
	 * @param behaviour how the waypoints will be traversed
	 * @param loadout the loadout each member is given @todo change to AI "type" which may have a different FSM/Goal tree
	 * @param count the number of ai to be spawned in the patrol
	 * @param respawnTime the time between patrol spawns
	 * @param despawnTime the time until patrol despawns if no players in despawn radius
	 * @param faction the faction the patrol will be spawned to
	 * @param formation the group's formation
	 * @param autoStart whether the patrol should automatically start its update loop - otherwise, have to call Start() manually
	 * @param minR miminum distance between the patrol and nearest player for a patrol to not (re)spawn
	 * @param maxR maximum distance between the patrol and nearest player for a patrol to (re)spawn
	 * @param despawnR maximum distance between the patrol and nearest player for a patrol to despawn
	 * @param speedLimit movement speed limit 1.0 = walk, 2.0 = jog, 3.0 = sprint
	 * @param threatspeedLimit movement speed limit under threat 1.0 = walk, 2.0 = jog, 3.0 = sprint
	 * @param unlimitedReload if spare mag in inventory, will be able to reload infinitely
	 * 
	 * @return the patrol instance
	 */
	//! @note hard function param limit seems to be 17, adding anymore will cause CTD
	static eAIDynamicPatrol CreateEx(vector pos, array<vector> waypoints, eAIWaypointBehavior behaviour, string loadout = "", int count = 1, int respawnTime = 600, int despawnTime = 600, eAIFaction faction = null, eAIFormation formation = null, bool autoStart = true, float minR = 300, float maxR = 800, float despawnR = 880, float speedLimit = 3.0, float threatspeedLimit = 3.0, bool canBeLooted = true, bool unlimitedReload = false/*, float accuracyMin = -1, float accuracyMax = -1*/)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAIDynamicPatrol", "Create");
		#endif

		eAIDynamicPatrol patrol;
		Class.CastTo(patrol, ((typename)eAIDynamicPatrol).Spawn());
		patrol.m_Position = pos;
		patrol.m_Waypoints = waypoints;
		patrol.m_WaypointBehaviour = behaviour;
		patrol.m_NumberOfAI = count;
		patrol.m_Loadout = loadout;
		patrol.m_RespawnTime = respawnTime;
		patrol.m_DespawnTime = despawnTime;
		patrol.m_MinimumRadius = minR;
		patrol.m_MaximumRadius = maxR;
		patrol.m_DespawnRadius = despawnR;
		patrol.m_MovementSpeedLimit = speedLimit;
		patrol.m_MovementThreatSpeedLimit = threatspeedLimit;
		patrol.m_Faction = faction;
		patrol.m_Formation = formation;
		patrol.m_CanBeLooted = canBeLooted;
		patrol.m_UnlimitedReload = unlimitedReload;
		//patrol.m_AccuracyMin = accuracyMin;
		//patrol.m_AccuracyMax = accuracyMax;
		patrol.m_CanSpawn = true;
		if (patrol.m_Faction == null) patrol.m_Faction = new eAIFactionCivilian();
		if (patrol.m_Formation == null) patrol.m_Formation = new eAIFormationVee();
		if (autoStart) patrol.Start();
		return patrol;
	}

	//! Legacy w/ despawnR = maxR * 1.1 for people still using it
	static eAIDynamicPatrol Create(vector pos, array<vector> waypoints, eAIWaypointBehavior behaviour, string loadout = "", int count = 1, int respawnTime = 600, eAIFaction faction = null, bool autoStart = true, float minR = 300, float maxR = 800, float speedLimit = 3.0, float threatspeedLimit = 3.0, bool canBeLooted = true, bool unlimitedReload = false)
	{
		return CreateEx(pos, waypoints, behaviour, loadout, count, respawnTime, 600, faction, null, autoStart, minR, maxR, maxR * 1.1, speedLimit, threatspeedLimit, canBeLooted, unlimitedReload);
	}

	void SetAccuracy(float accuracyMin, float accuracyMax)
	{
		m_AccuracyMin = accuracyMin;
		m_AccuracyMax = accuracyMax;
	}

	void SetThreatDistanceLimit(float distance)
	{
		m_ThreatDistanceLimit = distance;
	}

	void SetNoiseInvestigationDistanceLimit(float distance)
	{
		m_NoiseInvestigationDistanceLimit = distance;
	}

	void SetDamageMultiplier(float multiplier)
	{
		m_DamageMultiplier = multiplier;
	}

	void SetDamageReceivedMultiplier(float multiplier)
	{
		m_DamageReceivedMultiplier = multiplier;
	}

	void SetGroupName(string name)
	{
		m_GroupName = name;
	}

	void SetSniperProneDistanceThreshold(float distance)
	{
		m_SniperProneDistanceThreshold = distance;
	}

	void SetUnits(TStringArray units)
	{
		if (m_Units)
			m_Units.Clear();
		else
			m_Units = new ExpansionArray<string>;

		m_Units.InsertAll(units);
	}

	private eAIBase CreateAI(vector pos)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		string unit;

		if (m_Units)
			unit = m_Units.GetQuasiRandomElementAvoidRepetition();
		else
			unit = eAISurvivor.GetQuasiRandom();

		return eAIBase.Cast(GetGame().CreateObject(unit, pos));
	}

	private eAIBase SpawnAI(vector pos)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		pos = ExpansionAIPatrol.GetPlacementPosition(pos);

		eAIBase ai = CreateAI(pos);
		if (!ai)
			return null;

		ai.SetPosition(pos);

		if ( m_Loadout == "" )
			m_Loadout = m_Faction.GetDefaultLoadout();

		ExpansionHumanLoadout.Apply(ai, m_Loadout, false);
	
		ai.SetMovementSpeedLimits(m_MovementSpeedLimit, m_MovementThreatSpeedLimit);
		ai.Expansion_SetCanBeLooted(m_CanBeLooted);
		ai.eAI_SetUnlimitedReload(m_UnlimitedReload);
		ai.eAI_SetAccuracy(m_AccuracyMin, m_AccuracyMax);
		ai.eAI_SetThreatDistanceLimit(m_ThreatDistanceLimit);
		ai.eAI_SetNoiseInvestigationDistanceLimit(m_NoiseInvestigationDistanceLimit);
		ai.eAI_SetDamageMultiplier(m_DamageMultiplier);
		ai.eAI_SetDamageReceivedMultiplier(m_DamageReceivedMultiplier);
		ai.eAI_SetSniperProneDistanceThreshold(m_SniperProneDistanceThreshold);

		return ai;
	}

	bool WasGroupDestroyed()
	{
		if (!m_Group)
			return false;

		if (m_WasGroupDestroyed)
			return true;

		for (int i = 0; i < m_Group.Count(); i++)
		{
			DayZPlayerImplement member = m_Group.GetMember(i);
			if (member && member.IsInherited(PlayerBase) && member.IsAlive())
			{
				return false;
			}
		}

		m_WasGroupDestroyed = true;

		if (m_NumberOfDynamicPatrols)
			m_NumberOfDynamicPatrols--;

		return true;
	}

	bool CanSpawn()
	{
		if (m_Group)
			return false;

		if (!m_CanSpawn)
			return false;

		int maxPatrols = GetExpansionSettings().GetAI().MaximumDynamicPatrols;
		if (maxPatrols > -1 && m_NumberOfDynamicPatrols >= maxPatrols)
			return false;

		return true;
	}

	void Spawn()
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (m_Group) return;

		if (GetExpansionSettings().GetLog().AIPatrol)
		{
			string name = m_GroupName;
			if (name == string.Empty)
				name = m_Faction.GetName();
            GetExpansionSettings().GetLog().PrintLog("[AI Patrol] Spawning " + m_NumberOfAI + " " + name + " bots at " + m_Position);
        }

		m_TimeSinceLastSpawn = 0;
		m_CanSpawn = false;
		m_WasGroupDestroyed = false;

		eAIBase ai = SpawnAI(m_Position);
		m_Group = ai.GetGroup();
		m_Group.SetName(m_GroupName);
		m_Group.SetFaction(m_Faction);
		m_Group.SetFormation(m_Formation);

		if (m_NumberOfAI > 1)
			m_Group.SetWaypointBehaviour(eAIWaypointBehavior.HALT);  //! Only start moving after all AI spawned
		else
			m_Group.SetWaypointBehaviour(m_WaypointBehaviour);

		foreach (int idx, vector waypoint: m_Waypoints)
		{
			m_Group.AddWaypoint(waypoint);
			if (waypoint == m_Position)
			{
				m_Group.m_CurrentWaypointIndex = idx;
				if (Math.RandomIntInclusive(0, 1))
					m_Group.m_BackTracking = true;
			}
		}

		if (m_NumberOfAI > 1)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(SpawnAI_Deferred, 1);

		m_NumberOfDynamicPatrols++;
	}

	void SpawnAI_Deferred(int i)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (m_Group)
		{
			eAIBase ai = SpawnAI(m_Formation.ToWorld(m_Formation.GetPosition(i)));
			ai.SetGroup(m_Group);

			if (++i < m_NumberOfAI)
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(SpawnAI_Deferred, i);
			else
				m_Group.SetWaypointBehaviour(m_WaypointBehaviour);  //! Only start moving after all AI spawned
		}
	}

	void Despawn(bool deferDespawnUntilLoosingAggro = false)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Despawn");
		#endif

		m_TimeSinceLastSpawn = 0;

		if (m_Group)
		{
			m_Group.ClearAI(true, deferDespawnUntilLoosingAggro);
			m_Group = null;
		}

		if (!m_WasGroupDestroyed && m_NumberOfDynamicPatrols)
			m_NumberOfDynamicPatrols--;
	}

	override void OnUpdate()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "OnUpdate");
		#endif

		if ( WasGroupDestroyed() && m_RespawnTime < 0 )
		{
			return;
		}

		if (!m_Group || m_WasGroupDestroyed)
		{
			m_TimeSinceLastSpawn += eAIPatrol.UPDATE_RATE_IN_SECONDS;
			//! https://feedback.bistudio.com/T173348
			if (!m_CanSpawn && m_RespawnTime > -1 && m_TimeSinceLastSpawn >= m_RespawnTime)
				m_CanSpawn = true;
		}

		if (!m_Group)
		{
			if (!m_Trigger && CanSpawn())
			{
				m_Trigger = eAIDynamicPatrolSphereTrigger.Cast(GetGame().CreateObjectEx("eAIDynamicPatrolSphereTrigger", m_Position, ECE_LOCAL));
				m_Trigger.eAI_SetParams(this, m_MaximumRadius, m_MinimumRadius);
			}
		}
		else
		{
			//! CE API is only available after game is loaded
			if (!GetCEApi())
				return;

			vector patrolPos = m_Position;
			DayZPlayerImplement leader = m_Group.GetLeader();
			if (leader)
				patrolPos = leader.GetPosition();

			if (GetCEApi().AvoidPlayer(patrolPos, m_DespawnRadius))
			{
				if (!m_WasGroupDestroyed)
					m_TimeSinceLastSpawn += eAIPatrol.UPDATE_RATE_IN_SECONDS;
				if (m_TimeSinceLastSpawn >= m_DespawnTime)
					Despawn();
			}
		}
	}

	override void Debug()
	{
		super.Debug();
		
		Print(m_Group);
		Print(m_TimeSinceLastSpawn);
		Print(m_CanSpawn);
		Print(m_NumberOfAI);
		Print(WasGroupDestroyed());
	}
};
