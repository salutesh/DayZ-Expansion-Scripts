class eAIDynamicPatrol : eAIPatrol
{
	static ExpansionAIPatrolSettings s_AIPatrolSettings;
	private static int s_PatrolCount;

	static ref map<string, ref ExpansionAIPatrolLoadBalancing> s_LoadBalancing = new map<string, ref ExpansionAIPatrolLoadBalancing>;
	static ref ExpansionAIPatrolLoadBalancing s_LoadBalancingGlobal;
	static bool s_LoadBalancing_IsScheduled;

	static ref map<string, ref array<ref ExpansionPrefab>> s_LootDropsOnDeath = new map<string, ref array<ref ExpansionPrefab>>;

	ref ExpansionAIDynamicSpawnBase m_Config;
	ref array<ref ExpansionPrefab> m_LootDropOnDeath;
	ref ExpansionAIPatrolLoadBalancing m_LoadBalancing;
	ref ExpansionAIPatrolLoadBalancingTracker m_PatrolCountTracker;
	vector m_Position;
	autoptr array<vector> m_Waypoints;
	eAIWaypointBehavior m_WaypointBehaviour;
	int m_WaypointIdx;
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
	float m_FormationScale;
	float m_AccuracyMin; // zero or negative = use general setting
	float m_AccuracyMax; // zero or negative = use general setting
	float m_ThreatDistanceLimit; // zero or negative = use general setting
	float m_NoiseInvestigationDistanceLimit; // zero or negative = use general setting
	float m_MaxFlankingDistance; // zero or negative = use general setting
	int m_EnableFlankingOutsideCombat; // zero or negative = use general setting
	float m_DamageMultiplier; // zero or negative = use general setting
	float m_DamageReceivedMultiplier; // zero or negative = use general setting

	eAIGroup m_Group;
	float m_TimeSinceLastSpawn;
	bool m_CanSpawn;
	private bool m_IsSpawned;
	private bool m_WasGroupDestroyed;
	private eAIDynamicPatrolSphereTrigger m_Trigger;
	private string m_NameForLog;

	void ~eAIDynamicPatrol()
	{
		if (g_Game && m_Trigger)
			g_Game.ObjectDelete(m_Trigger);
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

		if (!m_Config.LoadBalancingCategory)
		{
			if (m_Config.ObjectClassName)
			{
				m_Config.LoadBalancingCategory = "ObjectPatrol";
			}
			else
			{
				switch (m_Config.ClassName())
				{
					case "ExpansionQuestAISpawn":
						m_Config.LoadBalancingCategory = "Quest";
						break;

					default:
						m_Config.LoadBalancingCategory = "Patrol";
						break;
				}
			}
		}

		if (!s_AIPatrolSettings.LoadBalancingCategories[m_Config.LoadBalancingCategory])
			m_Config.LoadBalancingCategory = "Global";

		LoadBalancing_Update();

		if (config.NumberOfAI == 0)
		{
			EXError.Error(null, "NumberOfAI shouldn't be set to 0", {});
			return false;
		}

		m_Waypoints = config.GetWaypoints(startpos);

		if (config.Persist && config.m_BaseName)
		{
			string fileName = eAIGroup.GetStorageDirectory(config.m_BaseName) + eAIGroup.BASENAME;
			if (FileExist(fileName))
			{
				eAIGroup.ReadPosition(fileName, startpos);

				if (config.GetBehaviour() != eAIWaypointBehavior.ROAMING)
				{
					//! Since this patrol is using waypoints, find the closest one
					float minDistSq = float.MAX;
					foreach (int idx, vector waypoint: m_Waypoints)
					{
						float distSq = vector.DistanceSq(startpos, waypoint);
						if (distSq < minDistSq)
						{
							minDistSq = distSq;
							m_WaypointIdx = idx;
						}
					}
				}
			}
			else
			{
				startpos = GetInitialSpawnPosition();
			}
		}
		else
		{
			startpos = GetInitialSpawnPosition();
		}

		if (startpos == vector.Zero)
		{
			EXError.Error(null, "Invalid spawn position - waypoint is set to zero vector <0 0 0>", {});
			return false;
		}

		m_Position = startpos;

		if (config.FormationScale <= 0)
			m_FormationScale = s_AIPatrolSettings.FormationScale;
		else
			m_FormationScale = config.FormationScale;

		if (config.RespawnTime == -2)
			m_RespawnTime = s_AIPatrolSettings.RespawnTime;
		else
			m_RespawnTime = config.RespawnTime;

		if (config.DespawnTime < 0)
			m_DespawnTime = s_AIPatrolSettings.DespawnTime;
		else
			m_DespawnTime = config.DespawnTime;

		if (config.MinDistRadius < 0)
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
			EXError.Error(null, "MinDistRadius (" + m_MinimumRadius + ") should be smaller than MaxDistRadius (" + m_MaximumRadius + ")", {});
			float actualMax = m_MinimumRadius;
			m_MinimumRadius = m_MaximumRadius;
			m_MaximumRadius = actualMax;
		}

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

		m_MaxFlankingDistance = config.MaxFlankingDistance;
		if (m_MaxFlankingDistance <= 0)
			m_MaxFlankingDistance = s_AIPatrolSettings.MaxFlankingDistance;

		m_EnableFlankingOutsideCombat = config.EnableFlankingOutsideCombat;
		if (m_EnableFlankingOutsideCombat <= 0)
			m_EnableFlankingOutsideCombat = s_AIPatrolSettings.EnableFlankingOutsideCombat;

		if (config.Units && config.Units.Count())
			SetUnits(config.Units);

		m_CanSpawn = true;

		if (config.LootDropOnDeath)
		{
			ExpansionString path = EXPANSION_AI_LOOTDROPS_FOLDER + config.LootDropOnDeath;

			if (!path.EndsWithIgnoreCase(".json"))
				path += ".json";

			if (!s_LootDropsOnDeath.Find(path, m_LootDropOnDeath))
			{
				string errorMsg;
				if (!JsonFileLoader<array<ref ExpansionPrefab>>.LoadFile(path, m_LootDropOnDeath, errorMsg))
				{
					EXError.Error(this, string.Format("Couldn't load LootDropOnDeath '%1': %2", path, errorMsg), {});
				}
				else
				{
					foreach (ExpansionPrefab lootDropOnDeath: m_LootDropOnDeath)
						lootDropOnDeath.m_Name = config.LootDropOnDeath;

					s_LootDropsOnDeath[path] = m_LootDropOnDeath;
				}

			}
		}

		//! There are situations where no suitable waypoints could be generated (e.g. all waypoints in high water)
		//! In this case, the patrol will not autostart and thus be inactive
		if (autoStart && m_Waypoints.Count() > 0) Start();

		return true;
	}

	vector GetInitialSpawnPosition()
	{
		int startPosIndex;
		//! For object patrols, we always use random waypoint as startpoint, for patrols with fixed waypoints, only if random is set
		if (m_Config.ObjectClassName || m_Config.UseRandomWaypointAsStartPoint)
			startPosIndex = Math.RandomInt(0, m_Waypoints.Count());
		return m_Waypoints[startPosIndex];
	}

	static bool InitSettings()
	{
		if ( !s_AIPatrolSettings )
		{
			s_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

			LoadBalancing_Setup();
		}

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

		return eAIBase.Cast(g_Game.CreateObject(unit, pos));
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
		ai.m_eAI_HeadshotResistance = m_Config.HeadshotResistance;
		ai.eAI_SetSniperProneDistanceThreshold(m_Config.SniperProneDistanceThreshold);
		ai.eAI_SetLootingBehavior(m_Config.GetLootingBehaviour());
		ai.m_eAI_LootDropOnDeath = m_LootDropOnDeath;
		ai.m_eAI_DefaultStance = m_Config.GetDefaultStance();
		ai.m_eAI_DefaultLookAngle = m_Config.DefaultLookAngle;

		if (m_MaxFlankingDistance > 0)
			ai.m_eAI_MaxFlankingDistance = m_MaxFlankingDistance;

		if (m_EnableFlankingOutsideCombat > 0)
			ai.m_eAI_EnableFlankingOutsideCombat = true;
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

		Log(m_NameForLog + " bots were wiped out (spawn position " + m_Position + ", " + (m_NumberOfAI - m_Group.Count()) + "/" + m_NumberOfAI + " deceased)");

		m_Position = GetInitialSpawnPosition();  //! Reset spawn position for next spawn

		if (s_PatrolCount)
			UpdatePatrolCount(-1);

		return true;
	}

	bool CanSpawn()
	{
		if (m_Group)
			return false;

		if (!m_CanSpawn)
			return false;

		return CanStay(1);
	}

	bool CanStay(int delta)
	{
	#ifdef SERVER
		if (m_LoadBalancing && m_LoadBalancing != s_LoadBalancingGlobal)
		{
			if (m_LoadBalancing.MaxPatrols > -1 && m_PatrolCountTracker.m_PatrolCount + delta > m_LoadBalancing.MaxPatrols)
				return false;
		}

		if (s_LoadBalancingGlobal && s_LoadBalancingGlobal.MaxPatrols > -1 && s_PatrolCount + delta > s_LoadBalancingGlobal.MaxPatrols)
			return false;
	#endif

		return true;
	}

	override void LoadBalancing_Update()
	{
		m_LoadBalancing = s_LoadBalancing[m_Config.LoadBalancingCategory];

		if (m_LoadBalancing)
			m_PatrolCountTracker = m_LoadBalancing.m_PatrolCountTracker;

		if (m_Group)
			m_Group.m_Leave = !CanStay(0);

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, m_Config.Name + " LoadBalancing_Update category " + m_Config.LoadBalancingCategory + " " + m_LoadBalancing);
	#endif
	}

	static void LoadBalancing_Setup()
	{
		int playerCount = PlayerBase.Expansion_GetOnlinePlayersCount();

		s_LoadBalancingGlobal = null;

		foreach (string name, auto categories: s_AIPatrolSettings.LoadBalancingCategories)
		{
			s_LoadBalancing[name] = null;

			foreach (auto loadBalancing: categories)
			{
				if (playerCount < loadBalancing.MinPlayers || (loadBalancing.MaxPlayers > 0 && playerCount > loadBalancing.MaxPlayers))
					continue;

			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.AI, eAIDynamicPatrol, "LoadBalancing_Setup category " + name + " " + loadBalancing + " minPlayers " + loadBalancing.MinPlayers + " maxPlayers " +  loadBalancing.MaxPlayers + " maxPatrols " +  loadBalancing.MaxPatrols);
			#endif

				s_LoadBalancing[name] = loadBalancing;

				if (name == "Global")
					s_LoadBalancingGlobal = loadBalancing;

				break;
			}
		}
	}

	static void LoadBalancing_UpdateAll()
	{
		LoadBalancing_Setup();

		foreach (auto patrol: s_AllPatrols)
		{
			patrol.LoadBalancing_Update();
		}

		s_LoadBalancing_IsScheduled = false;
	}

	//! @note called everytime a player connects/disconnects
	static void LoadBalancing_Schedule()
	{
		if (s_LoadBalancing_IsScheduled)
			return;

		s_LoadBalancing_IsScheduled = true;
		
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LoadBalancing_UpdateAll, 10000, false);
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
		{
			m_NumberOfAI = m_Group.Count();
			m_Faction = m_Group.GetFaction();
			m_Group.m_CurrentWaypointIndex = m_WaypointIdx;

			SetNameForLog();

			Log("Loaded " + m_NumberOfAI + " persistent " + m_NameForLog + " bots at " + m_Position);
		}
		else
		{
			if (m_Config.NumberOfAI < 0)
			{
				m_NumberOfAI = Math.RandomIntInclusive(1, -m_Config.NumberOfAI);
			} else {
				m_NumberOfAI = m_Config.NumberOfAI;
			}

			m_Faction = eAIFaction.Create(m_Config.Faction);
			if (m_Faction == null) m_Faction = new eAIFactionCivilian();
			if (m_Config.Loadout == "")
				m_Config.Loadout = m_Faction.GetDefaultLoadout();

			SetNameForLog();

			Log("Spawning " + m_NumberOfAI + " " + m_NameForLog + " bots at " + m_Position);
		}

		m_WasGroupDestroyed = false;

		m_MovementSpeedLimit = m_Config.GetSpeed();
		m_MovementThreatSpeedLimit = m_Config.GetThreatSpeed();

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

		if (!loaded)
			m_Group.SetFaction(m_Faction);

		m_Formation = eAIFormation.Create(m_Config.Formation);
		if (m_Formation == null) m_Formation = new eAIFormationVee();
		m_Formation.SetScale(m_FormationScale);
		m_Formation.SetLooseness(m_Config.FormationLooseness);
		m_Group.SetFormation(m_Formation);

		m_WaypointBehaviour = m_Config.GetBehaviour();
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
				m_Group.m_CurrentWaypoint = waypoint;
				if (idx != 0 && Math.RandomIntInclusive(0, 1))
					m_Group.m_BackTracking = true;
			}
		}

		if (!loaded && m_NumberOfAI > 1)
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(SpawnAI_Deferred, 1);

		m_IsSpawned = true;

		UpdatePatrolCount(1);
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
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(SpawnAI_Deferred, i);
			else
				m_Group.SetWaypointBehaviour(m_WaypointBehaviour);  //! Only start moving after all AI spawned
		}
	}

	void Despawn(bool deferDespawnUntilLoosingAggro = false)
	{
		#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
		#endif

		if (!m_IsSpawned)
			return;

		m_IsSpawned = false;
		m_TimeSinceLastSpawn = 0;

		if (m_Group)
		{
			Log("Despawning " + m_Group.Count() + " " + m_NameForLog + " bots (spawn position " + m_Position + ", " + (m_NumberOfAI - m_Group.Count()) + "/" + m_NumberOfAI + " deceased)");

			if (m_Group.m_Persist && m_Group.Count() && m_Group.m_BaseName)
			{
				m_Group.Save(true);
				m_TimeSinceLastSpawn = m_RespawnTime;  //! Allow "respawn" instantly if persistent group wasn't killed
				m_Position = m_Group.GetFormationLeader().GetPosition();  //! Update spawn position for next spawn
			}

			m_Group.ClearAI(true, deferDespawnUntilLoosingAggro);
			m_Group = null;
		}
		else
		{
			Log("Despawning " + m_NameForLog + " patrol (spawn position " + m_Position + ")");
		}

		if (!m_WasGroupDestroyed && s_PatrolCount)
			UpdatePatrolCount(-1);
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
			if (m_IsSpawned && !m_WasGroupDestroyed)  //! Group is NULL but not killed so was deleted behind our back, need to do cleanup
				Despawn();

			m_TimeSinceLastSpawn += eAIPatrol.UPDATE_RATE_IN_SECONDS;
			//! https://feedback.bistudio.com/T173348
			if (!m_CanSpawn && m_RespawnTime > -1 && m_TimeSinceLastSpawn >= m_RespawnTime)
				m_CanSpawn = true;
		}

		if (!m_Group)
		{
			if (!m_Trigger && CanSpawn())
			{
				m_Trigger = eAIDynamicPatrolSphereTrigger.Cast(g_Game.CreateObjectEx("eAIDynamicPatrolSphereTrigger", m_Position, ECE_LOCAL));
				m_Trigger.eAI_SetParams(this, m_MaximumRadius, m_MinimumRadius);
			}
		}
		else
		{
			vector patrolPos = m_Position;
			DayZPlayerImplement leader = m_Group.GetFormationLeader();
			if (leader)
				patrolPos = leader.GetPosition();

			if ((m_WasGroupDestroyed && m_Group.DeceasedCount() == 0) || AvoidPlayer(patrolPos, m_DespawnRadius) || m_Group.m_ForcePatrolDespawn)
			{
				if (!m_WasGroupDestroyed)
					m_TimeSinceLastSpawn += eAIPatrol.UPDATE_RATE_IN_SECONDS;
				if (m_TimeSinceLastSpawn >= m_DespawnTime || m_Group.m_ForcePatrolDespawn)
					Despawn();
			}
		}
	}

	bool CanBeTriggeredBy(PlayerBase player)
	{
		if (!player.IsAlive())
			return false;

		//! Actual players can always trigger
		if (player.GetIdentity())
			return true;

		if (!m_Config.CanBeTriggeredByAI)
			return false;

		//! Determine if AI can trigger
		eAIGroup group = player.GetGroup();
		if (!group)
			return false;

		//! Can't trigger ourself or prevent from despawn
		if (group == m_Group)
			return false;

		eAIFaction faction = group.GetFaction();

		if (faction.IsInvincible())
			return false;

		if (faction.IsObserver())
			return false;

		if (faction.IsPassive())
			return false;

		return true;
	}

	bool AvoidPlayer(vector patrolPos, float radius)
	{
		if (m_Config.CanBeTriggeredByAI || !GetCEApi())
		{
			set<PlayerBase> players = PlayerBase.Expansion_GetInCircle(patrolPos, radius);

			foreach (auto player: players)
			{
				if (CanBeTriggeredBy(player))
					return false;
			}

			return true;
		}

		return GetCEApi().AvoidPlayer(patrolPos, radius);
	}

	private void UpdatePatrolCount(int delta)
	{
		s_PatrolCount += delta;
		Log("Global patrol count: " + s_PatrolCount);

		array<eAIBase> all = eAIBase.eAI_GetAll();
		int aliveCount;
		int deceasedCount;
		foreach (eAIBase ai: all)
		{
			if (ai.IsSetForDeletion())
				continue;

			if (ai.IsDamageDestroyed())
				deceasedCount += 1;
			else
				aliveCount += 1;
		}
		Log(string.Format("Global Expansion AI count: %1 alive, %2 deceased", aliveCount, deceasedCount));

		if (m_PatrolCountTracker && (!s_LoadBalancingGlobal || m_PatrolCountTracker != s_LoadBalancingGlobal.m_PatrolCountTracker))
		{
			m_PatrolCountTracker.m_PatrolCount += delta;
			Log(m_Config.LoadBalancingCategory + " category patrol count: " + m_PatrolCountTracker.m_PatrolCount);
		}

		if (s_LoadBalancingGlobal)
		{
			s_LoadBalancingGlobal.m_PatrolCountTracker.m_PatrolCount += delta;

			int patrolCount = s_LoadBalancingGlobal.m_PatrolCountTracker.m_PatrolCount;
			if (patrolCount != s_PatrolCount)
				CF.FormatError("Global category patrol count %1 doesn't match internal global patrol count %2", patrolCount.ToString(), s_PatrolCount.ToString());
		}
	}

	static void OnDebugAll()
	{
		Print(s_LoadBalancingGlobal);
		if (s_LoadBalancingGlobal)
		{
			Print(s_LoadBalancingGlobal.MinPlayers);
			Print(s_LoadBalancingGlobal.MaxPlayers);
			Print(s_LoadBalancingGlobal.MaxPatrols);
			Print(s_LoadBalancingGlobal.m_PatrolCountTracker.m_PatrolCount);
		}
	}

	override void Debug()
	{
		super.Debug();
		
		Print(m_Config.Name);
		Print(m_NameForLog);

		Print(m_Trigger);
		Print(m_Position);
		Print(m_MinimumRadius);
		Print(m_MaximumRadius);
		Print(m_DespawnRadius);

		Print(m_Group);
		if (m_Group)
		{
			bool canStay = CanStay(0);
			Print(canStay);
			Print(m_Group.m_Leave);
		}

		Print(m_TimeSinceLastSpawn);
		bool canSpawn = CanSpawn();
		Print(canSpawn);
		Print(m_CanSpawn);
		Print(m_IsSpawned);

		Print(m_Config.NumberOfAI);
		Print(m_NumberOfAI);
		Print(m_RespawnTime);
		Print(m_DespawnTime);
		Print(m_WasGroupDestroyed);

		Print(m_Config.LoadBalancingCategory);
		Print(m_LoadBalancing);
		if (m_LoadBalancing)
		{
			Print(m_LoadBalancing.MinPlayers);
			Print(m_LoadBalancing.MaxPlayers);
			Print(m_LoadBalancing.MaxPatrols);
			Print(m_LoadBalancing.m_PatrolCountTracker.m_PatrolCount);
		}
	}

	private void SetNameForLog()
	{
		m_NameForLog = m_Config.Name;
		if (m_NameForLog == string.Empty)
			m_NameForLog = m_Faction.GetName();
		else
			m_NameForLog = string.Format("%1 (%2)", m_NameForLog, m_Faction.GetName());
	}

	string GetNameForLog()
	{
		return m_NameForLog;
	}

	static void Log(ExpansionAIDynamicSpawnBase config, string msg)
	{
		Error("DEPRECATED, use ExpansionAISpawnBase::Log");

		if (config)
		{
			config.Log(msg);
		}
		else
		{
			auto settings = GetExpansionSettings().GetLog();
			if (settings.AIPatrol)
				settings.PrintLog("[AI Patrol] %1", msg);
		}
	}

	void Log(string msg)
	{
		auto settings = GetExpansionSettings().GetLog();
		if (m_Config && m_Config.ObjectClassName)
		{
			if (settings.AIObjectPatrol)
				settings.PrintLog("[AI Object Patrol %1] %2", m_ID.ToStringLen(5), msg);
		}
		else if (settings.AIPatrol)
		{
			settings.PrintLog("[AI Patrol %1] %2", m_ID.ToStringLen(5), msg);
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
