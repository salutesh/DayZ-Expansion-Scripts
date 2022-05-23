class eAIDynamicPatrol : eAIPatrol
{
	private static int m_NumberOfDynamicPatrols;

	vector m_Position;
	autoptr array<vector> m_Waypoints;
	eAIWaypointBehavior m_WaypointBehaviour;
	float m_MinimumRadiusSq;
	float m_MaximumRadiusSq;
	float m_DespawnRadiusSq; // m_MaximumRadiusSq + 10%
	float m_MovementSpeedLimit;
	float m_MovementThreatSpeedLimit;
	int m_NumberOfAI;
	int m_RespawnTime;
	string m_Loadout;
	ref eAIFaction m_Faction;
	bool m_CanBeLooted;
	bool m_UnlimitedReload;

	eAIGroup m_Group;
	float m_TimeSinceLastSpawn;
	bool m_CanSpawn;
	private bool m_WasGroupDestroyed;

	/**
	 * @brief Creates a dynamic patrol which spawns a patrol under the right conditions.
	 * 
	 * @param pos the position that the trigger distance is calculated from
	 * @param waypoints an array of points which the patrol will traverse
	 * @param behaviour how the waypoints will be traversed
	 * @param loadout the loadout each member is given @todo change to AI "type" which may have a different FSM/Goal tree
	 * @param count the number of ai to be spawned in the patrol
	 * @param faction the faction the patrol will be spawned to
	 * @param respawnTime the time between patrol spawns
	 * @param minR miminum distance between the patrol and nearest player for a patrol to not (re)spawn
	 * @param maxR maximum distance between the patrol and nearest player for a patrol to (re)spawn
	 * @param speedLimit movement speed limit 1.0 = walk, 2.0 = jog, 3.0 = sprint
	 * 
	 * @return the patrol instance
	 */
	static eAIDynamicPatrol Create(vector pos, array<vector> waypoints, eAIWaypointBehavior behaviour, string loadout = "", int count = 1, int respawnTime = 600, eAIFaction faction = null, bool autoStart = true, float minR = 300, float maxR = 800, float speedLimit = 3.0, float threatspeedLimit = 3.0, bool canBeLooted = true, bool unlimitedReload = false)
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
		patrol.m_MinimumRadiusSq = Math.SqrFloat(minR);
		patrol.m_MaximumRadiusSq = Math.SqrFloat(maxR);
		patrol.m_DespawnRadiusSq = Math.SqrFloat(maxR * 1.1);
		patrol.m_MovementSpeedLimit = speedLimit;
		patrol.m_MovementThreatSpeedLimit = threatspeedLimit;
		patrol.m_Faction = faction;
		patrol.m_CanBeLooted = canBeLooted;
		patrol.m_UnlimitedReload = unlimitedReload;
		patrol.m_CanSpawn = true;
		if (patrol.m_Faction == null) patrol.m_Faction = new eAIFactionCivilian();
		if (autoStart) patrol.Start();
		return patrol;
	}

	private eAIBase SpawnAI(vector pos)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI");
		#endif

		eAIBase ai;
		if (!Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), pos))) return null;

		ai.SetPosition(pos);

		if ( m_Loadout == "" )
			m_Loadout = m_Faction.GetDefaultLoadout();

		ExpansionHumanLoadout.Apply(ai, m_Loadout, false);
				
		ai.SetMovementSpeedLimit(m_MovementSpeedLimit, m_MovementThreatSpeedLimit);

		return ai;
	}

	bool WasGroupDestroyed()
	{
		if (!m_Group)
			return false;

		if (m_WasGroupDestroyed)
			return false;

		for (int i = 0; i < m_Group.Count(); i++)
		{
			DayZPlayerImplement member = m_Group.GetMember(i);
			if (member && member.IsInherited(PlayerBase) && member.IsAlive())
			{
				return false;
			}
		}

		m_WasGroupDestroyed = true;

		return true;
	}

	void Spawn()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Spawn");
		#endif

		if (m_Group) return;

		m_TimeSinceLastSpawn = 0;
		m_CanSpawn = false;
		m_WasGroupDestroyed = false;

		eAIBase ai = SpawnAI(m_Position);
		ai.eAI_SetCanBeLooted(m_CanBeLooted);
		ai.eAI_SetUnlimitedReload(m_UnlimitedReload);
		m_Group = ai.GetGroup();
		m_Group.SetFaction(m_Faction);
		m_Group.SetWaypointBehaviour(m_WaypointBehaviour);
		foreach (vector v : m_Waypoints) m_Group.AddWaypoint(v);

		int count = m_NumberOfAI - 1;
		while (count != 0)
		{
			ai = SpawnAI(m_Position);
			ai.eAI_SetCanBeLooted(m_CanBeLooted);
			ai.eAI_SetUnlimitedReload(m_UnlimitedReload);
			ai.SetGroup(m_Group);
			count--;
		}

		m_NumberOfDynamicPatrols++;
	}

	void Despawn()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Despawn");
		#endif

		if (!m_Group) return;

		m_Group.ClearAI();
		m_Group = null;
		m_TimeSinceLastSpawn = 0;

		if (m_NumberOfDynamicPatrols)
			m_NumberOfDynamicPatrols--;
	}

	override void OnUpdate()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "OnUpdate");
		#endif

		if ( WasGroupDestroyed() && m_RespawnTime == -1 )
		{
			return;
		}

		super.OnUpdate();

		vector patrolPos = m_Position;
		DayZPlayerImplement leader = null;
		if (m_Group && m_Group.GetLeader())
		{
			leader = m_Group.GetLeader();
			patrolPos = leader.GetPosition();
		}
		
		array<Man> players = {};
		GetGame().GetPlayers(players);
		float minimumDistanceSq = float.MAX;
		foreach (auto player : players)
		{
			float distSq = vector.DistanceSq(patrolPos, player.GetPosition());
			if (distSq < minimumDistanceSq && leader != player) minimumDistanceSq = distSq;
		}

		if (m_Group)
		{
			m_TimeSinceLastSpawn += eAIPatrol.UPDATE_RATE_IN_SECONDS;
			m_CanSpawn = m_RespawnTime > -1 && m_TimeSinceLastSpawn >= m_RespawnTime;

			if (minimumDistanceSq > m_DespawnRadiusSq && m_CanSpawn)
			{
				Despawn();
			}
		}
		else if (m_CanSpawn)
		{
			int maxPatrols = GetExpansionSettings().GetAI().MaximumDynamicPatrols;
			if (maxPatrols > -1 && m_NumberOfDynamicPatrols >= maxPatrols)
			{
				return;
			}

			if (minimumDistanceSq < m_MaximumRadiusSq && minimumDistanceSq > m_MinimumRadiusSq)
			{
				Spawn();
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
