modded class ExpansionWorld
{
	static float AI_UPDATE_INTERVAL = 0.05;  //! s
	static float AI_UPDATE_BUDGET_FACTOR = 0.3;  //! 0.3 = 30% of frame time budget can be spent on AI
	static float MAX_AI_UPDATE_TIME_MS = 5;  //! ms
	static int MAX_AI_UPDATE_BATCH = 20;
	static float SERVER_FPS_UPDATE_THRESH = 1000.0 / 60.0;  //! ms
	static float SERVER_STATS_SEND_THRESH = 1000.0;  //! ms

	static bool s_DebugMonitor_ShowServerStats;

	//! Shoryuken meme stuff
	static ref ExpansionSoundSet s_eAI_UahUahUahM_SoundSet = ExpansionSoundSet.Register("Expansion_AI_UahUahUahM_SoundSet");
	static ref ExpansionSoundSet s_eAI_Heavy_Punch_SoundSet = ExpansionSoundSet.Register("Expansion_AI_Heavy_Punch_SoundSet");
	
	int m_ServerStats_RPCID;

	private ref eAIRoadNetwork m_Network;
	private bool m_NetworkGenerate;
	private vector m_NetworkPosition;
	private float m_NetworkRadius;

	CF_DoublyLinkedNode_WeakRef<eAIBase> m_CurrentAliveAI;
	int m_LastAIUpdateTime;
	bool m_eAI_Thread;

	void ExpansionWorld()
	{
		m_Network = new eAIRoadNetwork();
		//m_Network.Init();

		if (g_Game.IsDedicatedServer())
		{
			auto settings = GetExpansionSettings().GetAILocation();

			if (settings.RoamingLocations.Count() == 0)
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GenerateRoamingLocations, 10000);

			CheckCreateExampleAILootDrop();
		}

		if (g_Game.IsServer())
			eAIBase.s_eAI_Alive.m_OnRemove.Insert(eAI_OnRemoveAI);

		m_RPCManager.RegisterServer("RPC_DebugMonitor_ShowServerStats");
		m_ServerStats_RPCID = m_RPCManager.RegisterClient("RPC_ServerStats");

		if (g_Game.ServerConfigGetInt("enableDebugMonitor") == 2)
			s_DebugMonitor_ShowServerStats = true;
	}

	void ~ExpansionWorld()
	{
		if (g_Game && g_Game.IsServer())
			eAIBase.s_eAI_Alive.m_OnRemove.Remove(eAI_OnRemoveAI);
	}

	override void OnStart()
	{
		super.OnStart();

		if (g_Game.IsServer())
		{
			m_eAI_Thread = true;
			//g_Game.GameScript.Call(this, "eAI_Thread", null);
		}
	}

	void RPC_DebugMonitor_ShowServerStats(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		ctx.Read(s_DebugMonitor_ShowServerStats);

		if (s_DebugMonitor_ShowServerStats)
			g_Game.SetDebugMonitorEnabled(2);
		else if (g_Game.ServerConfigGetInt("enableDebugMonitor") == 1)
			g_Game.SetDebugMonitorEnabled(1);
	}

	static int s_AI_Alive_Count;
	static float s_ServerFPS_Current;
	static float s_ServerFPS_Min;
	static float s_ServerFPS_Max;
	static float s_ServerFPS_Avg;
	static float s_AIUpdateDT_Current;
	static float s_AIUpdateDT_Avg;
	static float s_AIUpdateCost_Current;
	static float s_AIUpdateCost_Avg;
	static int s_AIUpdate_Current;
	static float s_AIUpdate_Avg;
	void RPC_ServerStats(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		s_DebugMonitor_ShowServerStats = true;
		g_Game.GetMission().CreateDebugMonitor();

		ctx.Read(eAIBase.s_eAI_FTO);
		ctx.Read(s_AI_Alive_Count);
		ctx.Read(eAIDynamicPatrol.s_PatrolCount);

		ctx.Read(s_ServerFPS_Current);
		ctx.Read(s_ServerFPS_Min);
		ctx.Read(s_ServerFPS_Max);
		ctx.Read(s_ServerFPS_Avg);

		ctx.Read(s_AIUpdateDT_Current);
		ctx.Read(s_AIUpdateDT_Avg);

		ctx.Read(s_AIUpdateCost_Current);
		ctx.Read(s_AIUpdateCost_Avg);

		ctx.Read(s_AIUpdate_Current);
		ctx.Read(s_AIUpdate_Avg);
	}

	void GenerateRoamingLocations()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		auto settings = GetExpansionSettings().GetAILocation();
		auto excludedAreas = settings.NoGoAreas;

		int worldSize = g_Game.GetWorld().GetWorldSize();
		vector min = Vector(0, -1000, 0);
		vector max = Vector(worldSize, 1000, worldSize);

		int amount = Math.Pow(worldSize / 1500.0, 2.0);

		array<EntityAI> candidates = {};
		DayZPlayerUtils.SceneGetEntitiesInBox(min, max, candidates, QueryFlags.STATIC | QueryFlags.ORIGIN_DISTANCE);

		EXPrint(this, "Found " + candidates.Count() + " static entities (buildings) on the map");

		int locIdx = settings.RoamingLocations.Count();
		EntityAI candidate;
		while (candidates.Count() > 0 && settings.RoamingLocations.Count() < amount)
		{
			int index = candidates.GetRandomIndex();
			candidate = candidates[index];
			candidates.Remove(index);

			if (!candidate.IsBuilding())
				continue;

			vector position = candidate.GetPosition();

			if (excludedAreas.IsPointInside(position))
				continue;

			string type = candidate.GetType();
			type.ToLower();
			if (ExpansionString.StartsWithAny(type, settings.ExcludedRoamingBuildings))
				continue;

			float radius = 100.0;
			string name = "Random_" + (locIdx + 1);
			string className = "Local_" + name;
			settings.RoamingLocations.Insert(new ExpansionAIRoamingLocation(position, radius, name, "Local", className, null, locIdx++));
		}

		if (settings.RoamingLocations.Count() > 0)
			settings.Save();

		EXPrint(this, "Selected " + settings.RoamingLocations.Count() + " random buildings as locations for AI roaming");
	}

	ref ExpansionRollingAverage m_AIUpdates_Avg = new ExpansionRollingAverage(500);
	int m_AIUpdates_Current;
	ref ExpansionRollingAverage m_AIUpdates_DTAvg = new ExpansionRollingAverage(500);
	float m_AIUpdates_DTCurrent;
	ref ExpansionRollingAverage m_AIUpdates_CostAvg = new ExpansionRollingAverage(500);
	float m_AIUpdates_CostCurrent;
	ref ExpansionRollingAverage m_ServerFPS_Current = new ExpansionRollingAverage(10);
	ref ExpansionRollingAverage m_ServerFPS_Min = new ExpansionRollingAverage(64);
	ref ExpansionRollingAverage m_ServerFPS_Max = new ExpansionRollingAverage(64);
	ref ExpansionRollingAverage m_ServerFPS_Avg = new ExpansionRollingAverage(64);
	int m_ServerFPS_LastUpdateTime;
	float m_ServerFPS_TimeSlice;
	int m_Server_Ticks;
	int m_ServerStats_LastSendCheckTime;

	//! @note when -limitFPS is used on commandline, the thread can still run at a higher rate (good)
/*
	void eAI_Thread()
	{
		EXPrint(this, "+eAI_Thread");

		while (m_eAI_Thread)
		{
*/
	override void eAI_OnUpdate(bool doSim, float timeslice)
	{
			float costTotal = 0;

			if (eAIBase.s_eAI_FTO > 0)
			{
				int aiCount = eAIBase.s_eAI_Alive.m_Count;

				if (aiCount > 0)
				{
					float serverFPSAvg = m_ServerFPS_Avg.Get();
					float f = ExpansionMath.LinearConversion(30.0, 35.0, serverFPSAvg, 5.0, 1.0);
					float updateInterval = AI_UPDATE_INTERVAL * f;  //! Increase update interval if server FPS goes below 35
					float thresh = updateInterval / aiCount;
					int updateTime = g_Game.GetTime();

					if (m_LastAIUpdateTime == 0)
						m_LastAIUpdateTime = updateTime;

					float elapsed = (updateTime - m_LastAIUpdateTime) * 0.001;

					if (elapsed >= thresh)
					{
						m_LastAIUpdateTime = updateTime;

						int simulationPrecision = 0;

						float costAvg = m_AIUpdates_CostAvg.Get();  //! ms
						float t = Math.Max(costAvg, 0.01);  //! ms
						float budget = Math.Min((1000.0 / serverFPSAvg) * AI_UPDATE_BUDGET_FACTOR, MAX_AI_UPDATE_TIME_MS);  //! ms
						float maxUpdateTime = ExpansionMath.LinearConversion(30.0, 35.0, serverFPSAvg, t * 2, budget);  //! ms
						int aiMax = Math.Min(Math.Ceil(maxUpdateTime / t), Math.Min(aiCount, MAX_AI_UPDATE_BATCH));
						int i = 0;
						int n = 0;
						float updateIntervalCurrent;
						while (i < aiCount)
						{
							if (!m_CurrentAliveAI)
							{
								m_CurrentAliveAI = eAIBase.s_eAI_Alive.m_Head;
								if (!m_CurrentAliveAI)
									break;
							}

							eAIBase ai = m_CurrentAliveAI.m_Value;
							m_CurrentAliveAI = m_CurrentAliveAI.m_Next;

							if (ai.m_eAI_LastUpdateTime == 0)
								ai.m_eAI_LastUpdateTime = updateTime;

							if (eAIBase.s_eAI_FTO >= 2 && ai.m_eAI_PlayersWithinVisibilityDistanceLimit.Count() == 0)
								updateIntervalCurrent = AI_UPDATE_INTERVAL * 5;
							else
								updateIntervalCurrent = updateInterval;

							float pDt = (updateTime - ai.m_eAI_LastUpdateTime) * 0.001;  //! s
							if (pDt >= updateIntervalCurrent)
							{
								int tickCount = TickCount(0);
								bool isDisabled = ai.GetIsSimulationDisabled();
								if (!isDisabled)
								{
									ai.eAI_OnUpdate(pDt);
									ai.eAI_OnWeaponAimUpdate();
								}
								else
								{
									EntityAI entityInHands = ai.eAI_SetHasProjectileWeaponInHands();
									ai.eAI_Targeting(pDt, entityInHands, false);
								}
								ai.eAI_UpdateFSM(pDt, simulationPrecision);
								if (!isDisabled)
								{
									ai.m_eAI_CommandMove.AvoidObstacles(pDt);
									ai.eAI_OnMovementUpdate(pDt);
								}
								updateTime = g_Game.GetTime();
								ai.m_eAI_LastUpdateTime = updateTime;
								float cost = TickCount(tickCount) * 0.0001;  //! ms
								costTotal += cost;

								m_AIUpdates_CostAvg.Add(cost);

								if (s_DebugMonitor_ShowServerStats)
								{
									m_AIUpdates_DTAvg.Add(pDt / updateIntervalCurrent * f);
									m_AIUpdates_DTCurrent = pDt / updateIntervalCurrent * f;
									m_AIUpdates_CostCurrent = cost;
								}

								++n;

								if (n >= aiMax || costTotal >= maxUpdateTime)
									break;
							}

							++i;
						}

						if (s_DebugMonitor_ShowServerStats && n > 0)
						{
							m_AIUpdates_Avg.Add(n);
							m_AIUpdates_Current = n;
						}
					}
				}
			}
/*
			//! We took <costTotal> ms, so we give back at least <costTotal> ms
			Sleep(Math.Max(costTotal, 1));
		}

		EXPrint(this, "-eAI_Thread");
	}

	override void eAI_OnUpdate(bool doSim, float timeslice)
	{
*/
		int time = g_Game.GetTime();

		m_ServerFPS_TimeSlice = time - m_ServerFPS_LastUpdateTime;
		++m_Server_Ticks;

		if (m_ServerFPS_TimeSlice > SERVER_FPS_UPDATE_THRESH)
		{
			float currentFPS = (1000.0 * m_Server_Ticks) / m_ServerFPS_TimeSlice;

			m_ServerFPS_Avg.Add(currentFPS);

			m_ServerFPS_LastUpdateTime = time;
			m_Server_Ticks = 0;

			if (s_DebugMonitor_ShowServerStats)
			{
				m_ServerFPS_Current.Add(currentFPS);
				m_ServerFPS_Min.Add(m_ServerFPS_Avg.GetMin());
				m_ServerFPS_Max.Add(m_ServerFPS_Avg.GetMax());
				m_ServerFPS_Avg.SetRollingMinMax(true);

				if (time - m_ServerStats_LastSendCheckTime > SERVER_STATS_SEND_THRESH)
				{
					m_ServerStats_LastSendCheckTime = time;

					set<string> connectedAdmins = ExpansionAISettings.Get().GetConnectedAdmins();

				#ifndef SERVER
					//! Offline/SP
					if (connectedAdmins.Count() == 0)
						connectedAdmins.Insert("OFFLINE");
				#endif

					if (connectedAdmins.Count() > 0)
					{
						auto rpc = m_RPCManager.CreateRPC(m_ServerStats_RPCID);

						rpc.Write(eAIBase.s_eAI_FTO);
						rpc.Write(eAIBase.s_eAI_Alive.m_Count);
						rpc.Write(eAIDynamicPatrol.s_PatrolCount);

						rpc.Write(m_ServerFPS_Current.Get());
						rpc.Write(m_ServerFPS_Min.Get());
						rpc.Write(m_ServerFPS_Max.Get());
						rpc.Write(m_ServerFPS_Avg.Get());

						rpc.Write(m_AIUpdates_DTCurrent * AI_UPDATE_INTERVAL * 1000);
						rpc.Write(m_AIUpdates_DTAvg.Get() * AI_UPDATE_INTERVAL * 1000);

						rpc.Write(m_AIUpdates_CostCurrent);
						rpc.Write(m_AIUpdates_CostAvg.Get());

						rpc.Write(m_AIUpdates_Current);
						rpc.Write(m_AIUpdates_Avg.Get());

						foreach (string uid: connectedAdmins)
						{
						#ifdef SERVER
							PlayerBase admin = PlayerBase.GetPlayerByUID(uid);
							if (!admin)
								continue;
							PlayerIdentity identity = admin.GetIdentity();
							if (identity)
								rpc.Expansion_Send(true, identity);
						#else
							//! Offline/SP
							rpc.Expansion_Send(true);
						#endif
						}
					}
				}
			}
			else
			{
				m_ServerFPS_Avg.SetRollingMinMax(false);
			}
		}
	}

	void eAI_OnRemoveAI(CF_DoublyLinkedNode_WeakRef<eAIBase> node)
	{
		if (node == m_CurrentAliveAI)
			m_CurrentAliveAI = node.m_Next;
	}

	override void OnFinish()
	{
		super.OnFinish();

		if (m_eAI_Thread)
			DbgAIUpdate();

		m_eAI_Thread = false;
	}

	void DbgAIUpdate()
	{
		Print(m_AIUpdates_Current);

		float aiUpdatesMin;
		float aiUpdatesMax;
		m_AIUpdates_Avg.FindMinMax(aiUpdatesMin, aiUpdatesMax);
		Print(aiUpdatesMin);
		Print(aiUpdatesMax);

		float aiUpdatesAvg = m_AIUpdates_Avg.Get();
		Print(aiUpdatesAvg);

		Print(m_AIUpdates_DTCurrent);

		float aiUpdatesDTMin;
		float aiUpdatesDTMax;
		m_AIUpdates_DTAvg.FindMinMax(aiUpdatesDTMin, aiUpdatesDTMax);
		Print(aiUpdatesDTMin);
		Print(aiUpdatesDTMax);

		float aiUpdatesDTAvg = m_AIUpdates_DTAvg.Get();
		Print(aiUpdatesDTAvg);
	}

/*
	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);

		if (m_NetworkGenerate)
		{
			m_NetworkGenerate = false;

			m_Network.NotifyGenerate(m_NetworkPosition, m_NetworkRadius);
		}
		
		if (g_Game.GetInput().LocalPress("UAFire"))
		{			
			vector pos = g_Game.GetCurrentCameraPosition();
			float radius = 500;
			
			m_Network.DS_Destroy();
			
			GenerateRoadNetwork(pos, radius);
		}
	}
*/

	void GenerateRoadNetwork(vector position, float radius)
	{
		m_NetworkGenerate = true;
		m_NetworkPosition = position;
		m_NetworkRadius = radius;
	}

	eAIRoadNetwork GetRoadNetwork()
	{
		return m_Network;
	}

	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
				  vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	{
		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		if (g_Game.IsServer())
		{
			Weapon_Base weapon;
			if (Class.CastTo(weapon, source))
				weapon.eAI_FirearmEffects(directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);
		}
	}
	/**
	 * @brief fix firearm FX for redirected AI dmg
	 */
	override void FixAIFirearmFX(inout Object source, inout Object directHit, inout int componentIndex, inout string surface, inout vector pos, inout vector surfNormal, inout vector exitPos, inout vector inSpeed, inout vector outSpeed, inout bool isWater, inout bool deflected, inout string ammoType)
	{
		if (!directHit)
			return;

		bool isClient = g_Game.IsClient();

		//! source is NULL on client
		if (isClient)
			source = m_FirearmFXSource;

		EntityAI entity;
		eAIBase ai;

		if (source && Class.CastTo(entity, source) && Class.CastTo(ai, entity.GetHierarchyRoot()))
		{
			Object hitObject;

			if (isClient)
				hitObject = m_FirearmFXHitObject;
			else
				hitObject = ai.m_eAI_HitObject;

			if (hitObject && directHit != hitObject)
			{
				if (directHit.IsDayZCreature() || directHit.IsItemBase() || directHit.IsMan() || directHit.IsTransport())
				{
					pos = directHit.WorldToModel(pos);
					exitPos = directHit.WorldToModel(exitPos);

					directHit = hitObject;

					pos = directHit.ModelToWorld(pos);
					exitPos = directHit.ModelToWorld(exitPos);
				}
			}
		}

		if (isClient)
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ResetFirearmFXSource, 3, false, source);
	}

	void ResetFirearmFXSource(Object source)
	{
		if (m_FirearmFXSource == source)
			m_FirearmFXSource = null;
	}

	void CheckCreateExampleAILootDrop()
	{
		if (FileExist(EXPANSION_AI_LOOTDROPS_FOLDER))
			return;

		ExpansionStatic.MakeDirectoryRecursive(EXPANSION_AI_LOOTDROPS_FOLDER);

		string path = EXPANSION_AI_LOOTDROPS_FOLDER + "Example.json";

		if (FileExist(path))
			return;

		auto defaultLootDrop = new array<ref ExpansionPrefab>;
		
		ExpansionPrefab prefab;

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_545x39";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_545x39Tracer";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x39";
		prefab.Chance = 0.25;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x39Tracer";
		prefab.Chance = 0.25;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_556x45";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_556x45Tracer";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x54";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x54Tracer";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_308Win";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_308WinTracer";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "FirstAidKit";
		prefab.Chance = 0.33;
		prefab.SetHealth(0.7, 1.0);
		ExpansionPrefabObject obj;
		obj = prefab.BeginCargo("BandageDressing");
			obj.Chance = 1.0;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		obj = prefab.BeginCargo("BandageDressing");
			obj.Chance = 0.4;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		obj = prefab.BeginCargo("IodineTincture");
			obj.Chance = 0.25;
			obj.SetHealth(0.7, 1.0);
			obj.SetQuantity(0.2, 1.0);
			obj.End();
		obj = prefab.BeginCargo("Morphine");
			obj.Chance = 0.25;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		obj = prefab.BeginCargo("Epinephrine");
			obj.Chance = 0.25;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		defaultLootDrop.Insert(prefab);

		string errorMsg;
		if (!JsonFileLoader<array<ref ExpansionPrefab>>.SaveFile(path, defaultLootDrop, errorMsg))
			EXError.Error(this, string.Format("Couldn't save '%1': %2", path, errorMsg), {});
	}
};
