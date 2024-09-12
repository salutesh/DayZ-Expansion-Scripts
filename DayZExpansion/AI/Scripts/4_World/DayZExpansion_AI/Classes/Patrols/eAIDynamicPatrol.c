class eAIDynamicPatrol : eAIPatrol
{
	static ExpansionAIPatrolSettings s_AIPatrolSettings;
	private static int s_NumberOfDynamicPatrols;

	ref ExpansionAIDynamicSpawnBase m_Config;
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
	ref eAIFaction m_Faction;
	ref eAIFormation m_Formation;
	float m_AccuracyMin; // zero or negative = use general setting
	float m_AccuracyMax; // zero or negative = use general setting
	float m_ThreatDistanceLimit; // zero or negative = use general setting
	float m_NoiseInvestigationDistanceLimit; // zero or negative = use general setting
	float m_DamageMultiplier; // zero or negative = use general setting
	float m_DamageReceivedMultiplier; // zero or negative = use general setting

	eAIGroup m_Group;
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
	 * @param config
	 * @param startpos the position that the trigger distance is calculated from. If zero vector, waypoint from config is used
	 * @param autoStart
	 * 
	 * @return the patrol instance
	 */
	static eAIDynamicPatrol CreateEx(ExpansionAIDynamicSpawnBase config, vector startpos, bool autoStart = true)
	{
		return eAIDynamicPatrolT<eAIDynamicPatrol>.CreateEx(config, startpos, autoStart);
	}

	bool Setup(ExpansionAIDynamicSpawnBase config, vector startpos, bool autoStart = true)
	{
		InitSettings();

		m_Config = config;

		if (config.NumberOfAI != 0)
		{
			if (config.NumberOfAI < 0)
			{
				m_NumberOfAI = Math.RandomIntInclusive(1, -config.NumberOfAI);
			} else {
				m_NumberOfAI = config.NumberOfAI;
			}
		}
		else {
			Log(config, "WARNING: NumberOfAI shouldn't be set to 0, skipping this patrol...");
			return false;
		}

		m_WaypointBehaviour = config.GetBehaviour();
		m_Waypoints = config.GetWaypoints(startpos, m_WaypointBehaviour);

		if (config.Persist && config.m_BaseName)
		{
			string fileName = eAIGroup.GetStorageDirectory(config.m_BaseName) + eAIGroup.BASENAME;
			if (FileExist(fileName))
				eAIGroup.ReadPosition(fileName, startpos);
		}

		if (startpos == vector.Zero)
		{
			int startPosIndex;
			ExpansionAIPatrol patrolWithWaypoints;
			if (Class.CastTo(patrolWithWaypoints, config) && patrolWithWaypoints.UseRandomWaypointAsStartPoint)
				startPosIndex = Math.RandomInt(0, m_Waypoints.Count());
			startpos = m_Waypoints[startPosIndex];

			if (startpos == vector.Zero)
			{
				Log(config, "!!! ERROR !!!");
				Log(config, "Couldn't find a spawn location. First waypoint is set to 0 0 0 or could not be read by the system (validate your file with a json validator)");
				Log(config, "!!! ERROR !!!");
				return false;
			}
		}

		m_Position = startpos;

		if (config.RespawnTime == -2)
			m_RespawnTime = s_AIPatrolSettings.RespawnTime;
		else
			m_RespawnTime = config.RespawnTime;

		if (config.DespawnTime < 0)
			m_DespawnTime = s_AIPatrolSettings.DespawnTime;
		else
			m_DespawnTime = config.DespawnTime;

		if (config.MinDistRadius <= 0)
			m_MinimumRadius = s_AIPatrolSettings.MinDistRadius;
		else
			m_MinimumRadius = config.MinDistRadius;

		if (config.MaxDistRadius <= 0)
			m_MaximumRadius = s_AIPatrolSettings.MaxDistRadius;
		else
			m_MaximumRadius = config.MaxDistRadius;

		if (config.DespawnRadius <= 0)
			m_DespawnRadius = s_AIPatrolSettings.DespawnRadius;
		else
			m_DespawnRadius = config.DespawnRadius;
		
		if (m_MinimumRadius > m_MaximumRadius)
		{
			Log(config, "!!! ERROR !!!");
			Log(config, "MinDistRadius (" + m_MinimumRadius + ") should be smaller than MaxDistRadius (" + m_MaximumRadius + ")");
			Log(config, "!!! ERROR !!!");
			float actualMax = m_MinimumRadius;
			m_MinimumRadius = m_MaximumRadius;
			m_MaximumRadius = actualMax;
		}

		m_Formation = eAIFormation.Create(config.Formation);
		m_Formation.SetLooseness(config.FormationLooseness);
		m_Faction = eAIFaction.Create(config.Faction);

		float accuracyMin;
		if (config.AccuracyMin <= 0)
			accuracyMin = s_AIPatrolSettings.AccuracyMin;
		else
			accuracyMin = config.AccuracyMin;

		float accuracyMax;
		if (config.AccuracyMin <= 0)
			accuracyMax = s_AIPatrolSettings.AccuracyMax;
		else
			accuracyMax = config.AccuracyMax;

		float threatDistanceLimit;
		if (config.ThreatDistanceLimit <= 0)
			threatDistanceLimit = s_AIPatrolSettings.ThreatDistanceLimit;
		else
			threatDistanceLimit = config.ThreatDistanceLimit;

		float noiseDistanceLimit;
		if (config.NoiseInvestigationDistanceLimit <= 0)
			noiseDistanceLimit = s_AIPatrolSettings.NoiseInvestigationDistanceLimit;
		else
			noiseDistanceLimit = config.NoiseInvestigationDistanceLimit;

		float damageMultiplier;
		if (config.DamageMultiplier <= 0)
			damageMultiplier = s_AIPatrolSettings.DamageMultiplier;
		else
			damageMultiplier = config.DamageMultiplier;

		float damageReceivedMultiplier;
		if ( config.DamageReceivedMultiplier <= 0 )
			damageReceivedMultiplier = s_AIPatrolSettings.DamageReceivedMultiplier;
		else
			damageReceivedMultiplier = config.DamageReceivedMultiplier;

		SetAccuracy(accuracyMin, accuracyMax);
		SetThreatDistanceLimit(threatDistanceLimit);
		SetNoiseInvestigationDistanceLimit(noiseDistanceLimit);
		SetDamageMultiplier(damageMultiplier);
		SetDamageReceivedMultiplier(damageReceivedMultiplier);

		if (config.Units && config.Units.Count())
			SetUnits(config.Units);

		m_MovementSpeedLimit = config.GetSpeed();
		m_MovementThreatSpeedLimit = config.GetThreatSpeed();
		//m_AccuracyMin = accuracyMin;
		//m_AccuracyMax = accuracyMax;
		m_CanSpawn = true;
		if (m_Faction == null) m_Faction = new eAIFactionCivilian();
		if (m_Formation == null) m_Formation = new eAIFormationVee();

		if (m_Config.Loadout == "")
			m_Config.Loadout = m_Faction.GetDefaultLoadout();

		if (autoStart) Start();

		return true;
	}

	static bool InitSettings()
	{
		if ( !s_AIPatrolSettings )
			s_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

		return s_AIPatrolSettings.Enabled;
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
	#ifdef EXTRACE_DIAG
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
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		pos = ExpansionAIPatrol.GetPlacementPosition(pos);

		eAIBase ai = CreateAI(pos);
		if (!ai)
			return null;

		ai.SetPosition(pos);

		ExpansionHumanLoadout.Apply(ai, m_Config.Loadout, false);

		SetupAI(ai);

		return ai;
	}

	void SetupAI(eAIBase ai)
	{
		ai.SetMovementSpeedLimits(m_MovementSpeedLimit, m_MovementThreatSpeedLimit);
		ai.Expansion_SetCanBeLooted(m_Config.CanBeLooted);
		ai.eAI_SetUnlimitedReload(m_Config.UnlimitedReload);
		ai.eAI_SetAccuracy(m_AccuracyMin, m_AccuracyMax);
		ai.eAI_SetThreatDistanceLimit(m_ThreatDistanceLimit);
		ai.eAI_SetNoiseInvestigationDistanceLimit(m_NoiseInvestigationDistanceLimit);
		ai.eAI_SetDamageMultiplier(m_DamageMultiplier);
		ai.eAI_SetDamageReceivedMultiplier(m_DamageReceivedMultiplier);
		ai.eAI_SetSniperProneDistanceThreshold(m_Config.SniperProneDistanceThreshold);
		ai.eAI_SetLootingBehavior(m_Config.GetLootingBehaviour());
	}

	bool WasGroupDestroyed()
	{
		if (!m_Group)
			return false;

		if (m_WasGroupDestroyed)
			return true;

		if (m_Group.Count())
			return false;

		m_WasGroupDestroyed = true;

		if (s_NumberOfDynamicPatrols)
			s_NumberOfDynamicPatrols--;

		Log(m_Config, GetNameForLog() + " bots were wiped out (spawn position " + m_Position + ", " + (m_NumberOfAI - m_Group.Count()) + "/" + m_NumberOfAI + " deceased)");

		return true;
	}

	bool CanSpawn()
	{
		if (m_Group)
			return false;

		if (!m_CanSpawn)
			return false;

		int maxPatrols = GetExpansionSettings().GetAI().MaximumDynamicPatrols;
		if (maxPatrols > -1 && s_NumberOfDynamicPatrols >= maxPatrols)
			return false;

		return true;
	}

	void Spawn()
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (m_Group) return;

		m_TimeSinceLastSpawn = 0;
		m_CanSpawn = false;

		bool loaded;
		if (!m_WasGroupDestroyed && m_Config.Persist && m_Config.m_BaseName)
		{
			string fileName = eAIGroup.GetStorageDirectory(m_Config.m_BaseName) + eAIGroup.BASENAME;
			if (FileExist(fileName))
				loaded = eAIGroup.Load(fileName, m_Group);
		}

		if (loaded)
			Log(m_Config, "Loaded " + m_Group.Count() + "/" + m_NumberOfAI + " persistent " + GetNameForLog() + " bots at " + m_Position);
		else
			Log(m_Config, "Spawning " + m_NumberOfAI + " " + GetNameForLog() + " bots at " + m_Position);

		m_WasGroupDestroyed = false;

		eAIBase ai;
		if (!loaded)
		{
			ai = SpawnAI(m_Position);
			m_Group = ai.GetGroup();
			ai.m_eAI_GroupMemberID = m_Group.m_NextGroupMemberID++;
		}
		else
		{
			for (int i = 0; i < m_Group.Count(); i++)
			{
				if (Class.CastTo(ai, m_Group.GetMember(i)))
				{
					SetupAI(ai);
				}
			}
		}

		m_Group.m_Persist = m_Config.Persist;
		m_Group.m_BaseName = m_Config.m_BaseName;
		if (m_Group.m_Persist && m_Group.m_BaseName)
			eAIGroup.s_PersistentGroups.Insert(m_Group);

		m_Group.SetName(m_Config.Name);
		m_Group.SetFaction(m_Faction);
		m_Group.SetFormation(m_Formation);

		if (!loaded && m_NumberOfAI > 1)
			m_Group.SetWaypointBehaviour(eAIWaypointBehavior.HALT);  //! Only start moving after all AI spawned
		else
			m_Group.SetWaypointBehaviour(m_WaypointBehaviour);

		foreach (int idx, vector waypoint: m_Waypoints)
		{
			m_Group.AddWaypoint(waypoint);
			if (waypoint == m_Position)
			{
				m_Group.m_CurrentWaypointIndex = idx;
				if (idx != 0 && Math.RandomIntInclusive(0, 1))
					m_Group.m_BackTracking = true;
			}
		}

		if (!loaded && m_NumberOfAI > 1)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(SpawnAI_Deferred, 1);

		s_NumberOfDynamicPatrols++;
	}

	void SpawnAI_Deferred(int i)
	{
	#ifdef EXTRACE_DIAG
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
		#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
		#endif

		m_TimeSinceLastSpawn = 0;

		if (m_Group)
		{
			Log(m_Config, "Despawning " + m_Group.Count() + " " + GetNameForLog() + " bots (spawn position " + m_Position + ", " + (m_NumberOfAI - m_Group.Count()) + "/" + m_NumberOfAI + " deceased)");

			if (m_Group.m_Persist && m_Group.Count() && m_Group.m_BaseName)
			{
				m_Group.Save(true);
				m_TimeSinceLastSpawn = m_RespawnTime;  //! Allow "respawn" instantly if persistent group wasn't killed
			}

			m_Group.ClearAI(true, deferDespawnUntilLoosingAggro);
			m_Group = null;
		}

		if (!m_WasGroupDestroyed && s_NumberOfDynamicPatrols)
			s_NumberOfDynamicPatrols--;
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

			if ((m_WasGroupDestroyed && m_Group.DeceasedCount() == 0) || GetCEApi().AvoidPlayer(patrolPos, m_DespawnRadius))
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

	string GetNameForLog()
	{
		string name = m_Config.Name;
		if (name == string.Empty)
			name = m_Faction.GetName();

		return name;
	}

	static void Log(ExpansionAIDynamicSpawnBase config, string msg)
	{
		if (config && config.IsInherited(ExpansionAIObjectPatrol))
		{
			if (GetExpansionSettings().GetLog().AIObjectPatrol)
				GetExpansionSettings().GetLog().PrintLog("[AI Object Patrol] " + msg);
		}
		else if (GetExpansionSettings().GetLog().AIPatrol)
		{
			GetExpansionSettings().GetLog().PrintLog("[AI Patrol] " + msg);
		}
	}
};

class eAIDynamicPatrolT<Class T>
{
	static T CreateEx(ExpansionAIDynamicSpawnBase config, vector startpos, bool autoStart = true)
	{
		T patrol;
		Class.CastTo(patrol, ((typename)T).Spawn());

		if (patrol.Setup(config, startpos, autoStart))
			return patrol;

		return null;
	}
}
