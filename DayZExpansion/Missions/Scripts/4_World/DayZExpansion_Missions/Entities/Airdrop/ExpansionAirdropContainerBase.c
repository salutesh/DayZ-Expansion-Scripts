/**
 * ExpansionAirdropContainerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAirdropContainerBase
 * @brief		
 **/
#ifdef SERVER
class ExpansionAirdropContainerBase: Container_Base
#else
class ExpansionAirdropContainerBase: House
#endif
{
#ifdef EXPANSION_MISSIONS_CONTAINER_CLIENT
	static bool s_Expansion_CreateContainerOnClient = true;
#else
	static bool s_Expansion_CreateContainerOnClient = false;
#endif

	static ref map<int, ExpansionAirdropContainerBase> s_Expansion_AirdropContainers = new map<int, ExpansionAirdropContainerBase>;
	static int s_Expansion_AirdropContainerNextID;

	int m_Expansion_AirdropContainerID = -1;

	string m_Expansion_ClientContainerType;

	protected bool m_Expansion_HasDiscardedParachute;
	protected bool m_Expansion_HasDynamicPhysics;
	protected bool m_Expansion_IsLooted;
	IEntity m_Expansion_LastContact;
	bool m_Expansion_InfectedSpawned;
	bool m_Expansion_ForceCollisionInit;
	Object m_Expansion_ForceCollisionInit_Object;
	float m_Expansion_FallSpeed = 4.5;
	float m_Expansion_FallSpeedAccumulator;
	float m_Expansion_FallTimeAccumulator;
	vector m_Expansion_WindImpact;
	float m_Expansion_WindImpactStrength = 0.15;
	float m_Expansion_SimulationTimeAccumulator;
	float m_Expansion_SimulationTimeAccumulatorDiag;
	int m_Expansion_SynchCount;
	bool m_Expansion_HideCargoWhileParachuteIsDeployed;

	private float m_Expansion_StartTime;

	bool m_Expansion_AirdropContainerFX_Enabled = true;
	bool m_Expansion_AirdropContainerFX_Enabled_Client = true;
	ref ExpansionAirdropContainerFX m_Expansion_AirdropContainerFX = new ExpansionAirdropContainerFX(this);

	vector m_Expansion_SpawnPosition;  //! @note position where container is spawned, not necessarily where it lands!
	vector m_Expansion_Position;
	vector m_Expansion_Center;
	float m_Expansion_Height;
	float m_Expansion_Radius;

	//! Used on MP client only
	vector m_Expansion_PositionSynch;
	vector m_Expansion_OrientationSynch;

	protected bool m_Expansion_IsUpdateEnabled;
	float m_Expansion_LastUpdateTickTime;

#ifdef EXPANSIONMODAI
#ifdef SERVER
	ref eAIDynamicPatrolSpawner<ExpansionAirdropContainerBase> m_eAI_DynamicPatrolSpawner;
#endif
#endif

	void ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif	
		
		//SetEventMask( EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.SIMULATE );

		if (GetGame().IsServer())
			Expansion_EnableUpdate();

		if (GetGame().IsServer())
		{
			m_Expansion_HideCargoWhileParachuteIsDeployed = GetExpansionSettings().GetAirdrop().HideCargoWhileParachuteIsDeployed;

		#ifdef EXPANSIONMODAI
		#ifdef SERVER
			m_eAI_DynamicPatrolSpawner = new eAIDynamicPatrolSpawner<ExpansionAirdropContainerBase>(this);
			m_eAI_DynamicPatrolSpawner.SetDeferDespawnUntilLoosingAggro(true);
		#endif
		#endif
		}
	}
	
	void Expansion_EnableUpdate()
	{
	#ifdef SERVER
		//SetEventMask( EntityEvent.SIMULATE );
		m_Expansion_LastUpdateTickTime = GetGame().GetTickTime();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_Update, 25, true);
	#else
		//! Client or SP
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(Expansion_OnUpdate);
	#endif

		m_Expansion_IsUpdateEnabled = true;
	}

	void Expansion_DisableUpdate()
	{
		EXTrace.Print(EXTrace.MISSIONS, this, "Expansion_DisableUpdate " + m_Expansion_IsUpdateEnabled);

		if (m_Expansion_IsUpdateEnabled)
		{
		#ifdef SERVER
			//ClearEventMask(EntityEvent.SIMULATE);
			if (GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_Update);
		#else
			if (GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM))
				GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_OnUpdate);
		#endif

			m_Expansion_IsUpdateEnabled = false;
		}
	}

	void ~ExpansionAirdropContainerBase()
	{
		if (!GetGame())
			return;

		Expansion_DisableUpdate();

		if (s_Expansion_AirdropContainers && m_Expansion_AirdropContainerID > -1)
			s_Expansion_AirdropContainers.Remove(m_Expansion_AirdropContainerID);
	}

	override void EEInit()
	{
		super.EEInit();

		//if (GetGame().IsServer())
		//{
			m_Expansion_SpawnPosition = GetPosition();
			m_Expansion_Position = m_Expansion_SpawnPosition;
			m_Expansion_PositionSynch = m_Expansion_SpawnPosition;

			vector minMax[2];
			if (GetCollisionBox(minMax))
			{
				m_Expansion_Center = Vector((minMax[0][0] + minMax[1][0]) * 0.5, (minMax[0][1] + minMax[1][1]) * 0.5, (minMax[0][2] + minMax[1][2]) * 0.5);
				m_Expansion_Height = minMax[1][1] - minMax[0][1];
				m_Expansion_Radius = vector.Distance(minMax[0], minMax[1]) * 0.5;
				EXTrace.Print(EXTrace.MISSIONS, this, "EEInit - center " + m_Expansion_Center + " height " + m_Expansion_Height + " radius " + m_Expansion_Radius);
			}
			else
			{
				EXError.Error(this, ConfigGetString("model") + " has no collision box!", {});
			}

		#ifndef SERVER
			//! Show parachute on client
			SetAnimationPhase( "parachute", 0 );
		#endif

			m_Expansion_OrientationSynch = Vector( GetOrientation()[0], 0, 0 );
			SetOrientation( m_Expansion_OrientationSynch );
			//CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			//EnableDynamicCCD( true );
			//SetDynamicPhysicsLifeTime( -1 );

			m_Expansion_StartTime = GetGame().GetTickTime();

			//dBodySetDamping(this, 0.0, 1.0);
		//}
	}

	//! EOnContact will only fire on server
	//! and only while entity physics are active (will also be set active if another active entity collides with this one, e.g. a vehicle)
	override void EOnContact(IEntity other, Contact extra)
	{
	#ifdef DIAG_DEVELOPER
		if (other != m_Expansion_LastContact)
		{
			EXTrace.Print(EXTrace.MISSIONS, this, "EOnContact other=" + ExpansionStatic.GetDebugInfo(other) + " isActive=" + dBodyIsActive(this) + " hasDynamicPhysics=" + m_Expansion_HasDynamicPhysics);
			m_Expansion_LastContact = other;
		}
	#endif

		ExpansionWorld.CheckTreeContact(other, 7500, true);

		m_Expansion_HasDynamicPhysics = true;

		if (!m_Expansion_HasDiscardedParachute)
		{
			Object obj;
			if (!Class.CastTo(obj, other) || Expansion_CanCollideWith(obj))
				Expansion_DiscardParachute();
		}
	}

	override void EEDelete( EntityAI parent )
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
#endif
		
		super.EEDelete(parent);

		if (m_Expansion_AirdropContainerFX)
			m_Expansion_AirdropContainerFX.Destroy();

		if ( IsMissionHost() )
		{
			ExpansionAirdropContainerManagers.DeferredCleanup();

			Expansion_SendDeleteContainerOnClient();

			if (m_Expansion_ForceCollisionInit_Object)
				m_Expansion_ForceCollisionInit_Object.Delete();
		}
	}

	override bool CanObstruct()
	{
		return false;
	}

	bool Expansion_HasLanded()
	{
		return m_Expansion_HasDiscardedParachute && !dBodyIsActive(this);
	}
	
	void Expansion_SetAirdropContainerID(int containerID)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MISSIONS, this, "Airdrop container ID " + containerID);
	#endif

		m_Expansion_AirdropContainerID = containerID;
		s_Expansion_AirdropContainers[containerID] = this;
	}

	void Expansion_InitAirdrop( array < ref ExpansionLoot > Loot, TStringArray infected, int ItemCount, int infectedCount, float fallSpeed = 4.5, float windImpact = 0.0 )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( IsMissionHost() )
		{
		#ifdef SERVER
			if (s_Expansion_CreateContainerOnClient)
			{
				//! Hide the server container on client
				SetAnimationPhase( "parachute", 1 );
				SetAnimationPhase( "camo", 1 );

				//! Disable server container FX on client (client container has its own, separate FX)
				m_Expansion_AirdropContainerFX_Enabled = false;
				SetSynchDirty();
			}
		#endif

			ExpansionAirdropContainerManagers.Add( this, infected, infectedCount );

			m_Expansion_ClientContainerType = GetType() + "_Client";

			ExpansionLootSpawner.SpawnLoot( this, Loot, ItemCount );

			if (fallSpeed <= 0)
				fallSpeed = 4.5;

			float totalWeight = GetWeightEx();
			float totalWeightKg = totalWeight * 0.001;

			m_Expansion_FallSpeed = fallSpeed * ExpansionMath.LinearConversion(m_ConfigWeight, m_ConfigWeight * 1.666666, totalWeight, 1.0, 1.111111);

			//! The higher the fall speed, the lesser the wind impact
			m_Expansion_WindImpactStrength = ExpansionMath.LinearConversion(3.0, 6.0, m_Expansion_FallSpeed, 0.2, 0.1) * windImpact;

			EXLogPrint(ToString() + " Expansion_InitAirdrop - total weight (kg) " + totalWeightKg + " - nominal fall speed " + m_Expansion_FallSpeed + " m/s, wind impact strength " + m_Expansion_WindImpactStrength);

			if (s_Expansion_CreateContainerOnClient)
			{
				Expansion_SetAirdropContainerID(s_Expansion_AirdropContainerNextID++);
				Expansion_SendCreateContainerOnClient();
			}
		}
	}

	void Expansion_InitAirdropClient(float fallSpeed, float windImpactStrength, bool fxEnabled = true, bool hasDiscardedParachute = false)
	{
		m_Expansion_FallSpeed = fallSpeed;
		m_Expansion_WindImpactStrength = windImpactStrength;

		EXLogPrint(ToString() + " Expansion_InitAirdropClient - nominal fall speed " + m_Expansion_FallSpeed + " m/s, wind impact strength " + m_Expansion_WindImpactStrength);

		m_Expansion_AirdropContainerFX.Enable(fxEnabled);

		m_Expansion_HasDiscardedParachute = hasDiscardedParachute;

		if (hasDiscardedParachute)
			SetAnimationPhase( "parachute", 1 );
	}

	void Expansion_SendCreateContainerOnClient(PlayerIdentity identity = null)
	{
		if (m_Expansion_AirdropContainerID == -1)
			return;

		auto rpc = ExpansionMissionModule.s_Instance.Expansion_CreateRPC("RPC_CreateAirdropContainer");
		rpc.Write(m_Expansion_AirdropContainerID);
		rpc.Write(m_Expansion_ClientContainerType);
		rpc.Write(GetPosition());
		rpc.Write(GetOrientation());
		rpc.Write(m_Expansion_FallSpeed);
		rpc.Write(m_Expansion_WindImpactStrength);
		rpc.Write(m_Expansion_AirdropContainerFX_Enabled_Client);
		rpc.Write(m_Expansion_HasDiscardedParachute);
		rpc.Expansion_Send(true, identity);
	}

	static void Expansion_SendCreateContainersOnClient(PlayerBase player)
	{
		if (!player)
			return;

		auto identity = player.GetIdentity();

		if (!identity)
			return;

		foreach (int id, ExpansionAirdropContainerBase container: s_Expansion_AirdropContainers)
		{
			container.Expansion_SendCreateContainerOnClient(identity);
		}
	}

	void Expansion_SendDeleteContainerOnClient(PlayerIdentity identity = null)
	{
		if (m_Expansion_AirdropContainerID == -1)
			return;

		if (!ExpansionMissionModule.s_Instance)  //! Can be NULL on server shutdown
			return;

		auto rpc = ExpansionMissionModule.s_Instance.Expansion_CreateRPC("RPC_DeleteAirdropContainer");
		rpc.Write(m_Expansion_AirdropContainerID);
		rpc.Expansion_Send(true, identity);
	}

	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		Expansion_CheckAirdrop();
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		Expansion_CheckAirdrop();
	}

	void Expansion_CheckAirdrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		if ( !m_Expansion_IsLooted && IsEmpty() )
		{
			m_Expansion_IsLooted = true;

			Expansion_DisableFX();

			ExpansionAirdropContainerManager manager = ExpansionAirdropContainerManagers.Find( this );
			if ( manager )
				manager.StopUpdateNoise();
		}
	}

/*
	override void EOnSimulate(IEntity other, float dt)
	{
		if (!GetGame().IsServer())
			return;

		Expansion_OnUpdate(dt);
	}
*/

	void Expansion_Update()
	{
		float time = GetGame().GetTickTime();
		float dt = time - m_Expansion_LastUpdateTickTime;

		Expansion_OnUpdate(dt);

		m_Expansion_LastUpdateTickTime = time;
	}

	void Expansion_OnUpdate(float dt)
	{
		vector position = GetPosition();

		vector transform[4];

		bool guaranteed;

		if (GetGame().IsClient())
		{
			//! MP client

			vector orientation = GetOrientation();

			if (m_Expansion_HasDiscardedParachute && Expansion_IsFinal(position, orientation))
			{
				SetPosition(m_Expansion_PositionSynch);
				SetOrientation(m_Expansion_OrientationSynch);
			}
			else
			{
				if (m_Expansion_HasDiscardedParachute)
					dt *= 2;

				SetPosition(vector.Lerp(position, m_Expansion_PositionSynch, dt * 2));
				SetOrientation(vector.Lerp(orientation, m_Expansion_OrientationSynch, dt * 2));
			}
		}
		else if (m_Expansion_HasDiscardedParachute)
		{
			if (!dBodyIsActive(this))
			{
				if (Expansion_CheckLanded())
				{
					if (m_Expansion_HasDynamicPhysics)
					{
						//EnableDynamicCCD(false);
						SetDynamicPhysicsLifeTime(0);

						m_Expansion_HasDynamicPhysics = false;
						m_Expansion_LastContact = null;

						//ClearEventMask(EntityEvent.SIMULATE);
						//Expansion_DisableUpdate();

						EXTrace.Print(EXTrace.MISSIONS, this, "Expansion_OnUpdate - inactive & has landed - updating pathgraph region");

						SetAffectPathgraph(false, true);
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);

						if (GetGame().IsDedicatedServer())
						{
							if (m_Expansion_SimulationTimeAccumulator > 0.0)
								Expansion_SynchContainerStateToClient();

							GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_SynchContainerStateToClient, 100, false, true, null);
						}
					}
				}
				else if (!m_Expansion_HasDynamicPhysics)
				{
					Expansion_CreateDynamicPhysics();
				}
			}
			else if (GetGame().IsDedicatedServer())
			{
				if (GetAnimationPhase("camo") < 1)
				{
					GetTransform( transform );
					MoveInTime( transform, dt );
				}

				m_Expansion_SimulationTimeAccumulator += dt;
				if (m_Expansion_SimulationTimeAccumulator >= 0.1)
				{
					m_Expansion_SimulationTimeAccumulator = 0.0;
					m_Expansion_SynchCount++;

					if (m_Expansion_SynchCount % 10 == 0)
						guaranteed = true;

					Expansion_SynchContainerStateToClient(guaranteed);
				}
			}
		}
		else if (!Expansion_CheckLanded())
		{
			//! Get current velocity
			//vector currentVelocity = GetVelocity(this);
			vector diff = position - m_Expansion_Position;
			vector currentVelocity = diff * (1.0 / dt);
			vector velocity = currentVelocity;

			if ( m_Expansion_WindImpactStrength > 0.0 )
			{
				if (GetGame().GetWeather())
				{
					vector wind = GetGame().GetWeather().GetWind();

					vector windImpact;
					windImpact[0] = wind[0] * m_Expansion_WindImpactStrength;
					windImpact[2] = wind[2] * m_Expansion_WindImpactStrength;

					//! Prevent infinite acceleration by subtracting current velocity
					windImpact = windImpact - Vector(velocity[0], 0.0, velocity[2]);

					m_Expansion_WindImpact[0] = Math.Lerp(m_Expansion_WindImpact[0], windImpact[0], dt);
					m_Expansion_WindImpact[2] = Math.Lerp(m_Expansion_WindImpact[2], windImpact[2], dt);

					//! Apply directional wind impact
					//dBodyApplyImpulse(this, m_Expansion_WindImpact * dt * 0.5);
					velocity[0] = velocity[0] + m_Expansion_WindImpact[0] * 0.5;
					velocity[2] = velocity[2] + m_Expansion_WindImpact[2] * 0.5;

					//! Get updated velocity
					//velocity = GetVelocity(this);

					//! Rotate to updated direction of movement
					vector dirNormalized = velocity.Normalized();
					vector ori = dirNormalized.VectorToAngles();
					ori[0] = Math.NormalizeAngle(ori[0] + 90.0);
					vector angles = GetAngles();
					float angleDiff = ExpansionMath.AngleDiff2(angles[1], ori[0]);
					angles[1] = Math.NormalizeAngle(angles[1] + angleDiff * m_Expansion_WindImpactStrength * dt);
					SetAngles(angles);
				}
			}

			//! Apply fall speed accounting for gravitational acceleration
			//! (effective fall speed will roughly match computed value from InitAirdrop)
			//velocity[1] = -m_Expansion_FallSpeed * 0.44;
			velocity[1] = -m_Expansion_FallSpeed;

			//SetVelocity(this, velocity);

			GetTransform( transform );
			transform[3] = transform[3] + velocity * dt;

		#ifdef DIAG_DEVELOPER
			m_Expansion_FallSpeedAccumulator += -diff[1];
			m_Expansion_FallTimeAccumulator += dt;
			m_Expansion_SimulationTimeAccumulatorDiag += dt;
			if (m_Expansion_SimulationTimeAccumulatorDiag >= 1.0)
			{
				m_Expansion_SimulationTimeAccumulatorDiag = 0.0;
				float fallSpeed = -currentVelocity[1];
				float fallSpeedAvg = m_Expansion_FallSpeedAccumulator / m_Expansion_FallTimeAccumulator;
				EXTrace.Print(EXTrace.MISSIONS, this, "Expansion_OnUpdate - dt " + dt + " - fall speed " + fallSpeed + " m/s (avg " + fallSpeedAvg + ") - velocity " + currentVelocity.Length());
				if (DayZPlayerImplement.s_Expansion_DebugObjects_Enabled)
				{
					EntityAI dbgEnt;
					if (Class.CastTo(dbgEnt, GetGame().CreateObjectEx("ExpansionDebugRodBig", transform[3], ECE_NOLIFETIME)))
					{
						dbgEnt.SetOrientation(GetOrientation());
						dbgEnt.SetLifetime(600);
					}
				}
			}
		#endif

			MoveInTime( transform, dt );

			m_Expansion_SimulationTimeAccumulator += dt;
			if (m_Expansion_SimulationTimeAccumulator >= 0.1)
			{
				m_Expansion_SimulationTimeAccumulator = 0.0;
				m_Expansion_SynchCount++;

				if (m_Expansion_SynchCount % 10 == 0)
					guaranteed = true;

				Expansion_SynchContainerStateToClient(guaranteed);
			}
		}
		else
		{
			Expansion_DiscardParachute();
		}

		m_Expansion_Position = position;
	}
	
	private bool Expansion_CheckLanded()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		//! Ray input
		vector start = ModelToWorld(m_Expansion_Center);
		start[1] = start[1] - m_Expansion_Height * 0.5;

		vector surfacePos = ExpansionStatic.GetSurfaceRoadPosition(start, RoadSurfaceDetection.CLOSEST);

		float altitude = start[1] - surfacePos[1];

		if (altitude <= 0.05)
			return true;

		if (altitude > 0.5)
		{
			ExpansionAirdropContainerManager manager;

			if (altitude <= 50 && !m_Expansion_ForceCollisionInit)
			{
				m_Expansion_ForceCollisionInit = true;

				//! Raycast or roadsurface will ignore map objects whose collision hasn't been initialized.
				//! Collision for map objects like rocks, structures etc only exists if there is a player or creature in a 100 m radius.
				//! The player/creature does not have to be alive (but note that some dead animals like chicken, hares and foxes will be
				//! replaced with a corpse item on death, so they won't work!).
				//! To force collision init for any map objects at the drop position, if we have a list of zombies, just start spawning them.
				//! If we don't have a list of zombies and no player is around, we spawn a random zombie on ground and kill it.
				
				manager = ExpansionAirdropContainerManagers.Find( this );
				if (manager && manager.InfectedCount > 0)
				{
					manager.SpawnSingleInfected(surfacePos);
				}
				else if (!ExpansionLootSpawner.IsPlayerNearby(this, 100))
				{
					Object obj = GetGame().CreateObjectEx(ExpansionStatic.GetWorkingZombieClasses().GetRandomElement(), surfacePos, ECE_PLACE_ON_SURFACE | ECE_INITAI);
					obj.SetHealth(0);
					m_Expansion_ForceCollisionInit_Object = obj;
				}
			}

			if (altitude <= 25 && !m_Expansion_InfectedSpawned)
			{
				m_Expansion_InfectedSpawned = true;
				
				manager = ExpansionAirdropContainerManagers.Find( this );
				if (manager)
				{
					manager.SpawnInfected();
				}
			}
		}

		vector end = start;
		
		//! Ray output
		vector hitpos;
		vector hitdir;
		int hitindex;
		set<Object> results = new set<Object>;
		
		if (DayZPhysics.RaycastRV( start, end, hitpos, hitdir, hitindex, results, NULL, this, false, false, ObjIntersectView, 0.5, CollisionFlags.ALLOBJECTS))
		{
			int count = results.Count();
			int collided;

			foreach (Object result: results)
			{
				//! Bushes do not have collision, so we deal with them here
				if ((result.IsBush() || result.IsTree()) && !result.IsDamageDestroyed())
					ExpansionWorld.CheckTreeContact(result, 7500, true);

				if (Expansion_CanCollideWith(result))
					collided++;
			}

			if (count == 0 || collided > 0)
				return true;
		}

		return false;
	}

	bool Expansion_CanCollideWith(Object obj)
	{
		if (obj.IsInherited(ExpansionAirdropPlaneBase) || (obj.IsItemBase() && !ExpansionStatic.CanObstruct(obj)) || obj.IsBush())
			return false;

		return true;
	}

	void Expansion_DiscardParachute()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
#endif 
		
		m_Expansion_HasDiscardedParachute = true;

		Expansion_CreateDynamicPhysics();

		//! Set parachute animation phase so parachute is hidden
		SetAnimationPhase( "parachute", 1 );

		ExpansionAirdropContainerManager manager = ExpansionAirdropContainerManagers.Find( this );
		if ( manager )
		{
			manager.m_ContainerPosition = GetPosition();

			if ( GetExpansionSettings().GetAirdrop().ServerMarkerOnDropLocation )
				manager.CreateServerMarker(); //! Set server map marker on drop position

			if (!m_Expansion_InfectedSpawned)
				manager.SpawnInfected();
		}

	#ifdef EXPANSIONMODAI
		m_eAI_DynamicPatrolSpawner.Init();
	#endif

		if (GetLifetime() <= 0)
			EXPrint(ToString() + " WARNING: " + GetType() + " lifetime was already zero when it landed! Check MissionMaxTime (for mission airdrops) or expansion_types.xml (for player-called airdrops).");

		SetSynchDirty();
		Expansion_SynchContainerStateToClient();
	}
	
	void Expansion_CreateDynamicPhysics()
	{
		EXTrace.Print(EXTrace.MISSIONS, this, "Creating dynamic physics");

		CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );

		dBodySetDamping(this, 0.5, 0.5);

		SetDynamicPhysicsLifeTime( (GetGame().GetTickTime() - m_Expansion_StartTime) + 30 );

		m_Expansion_HasDynamicPhysics = true;
	}

	void Expansion_SynchContainerStateToClient(bool guaranteed = true, PlayerIdentity identity = null)
	{
		if (m_Expansion_AirdropContainerID == -1)
			return;

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MISSIONS, this, "Expansion_SynchContainerStateToClient guaranteed=" + guaranteed + " identity=" + identity);
	#endif

		auto rpc = ExpansionMissionModule.s_Instance.Expansion_CreateRPC("RPC_SynchContainerStateToClient");

		rpc.Write(m_Expansion_AirdropContainerID);

		vector position = GetPosition();
		rpc.Write(position[0]);
		rpc.Write(position[1]);
		rpc.Write(position[2]);

		vector orientation = GetOrientation();
		rpc.Write(orientation[0]);
		rpc.Write(orientation[1]);
		rpc.Write(orientation[2]);

		rpc.Write(m_Expansion_AirdropContainerFX_Enabled_Client);
		rpc.Write(m_Expansion_HasDiscardedParachute);

		if (guaranteed || identity)
			rpc.Expansion_Send(guaranteed, identity);
		else
			PlayerBase.Expansion_SendNear(rpc, position, 2100, null, guaranteed);
	}

	bool Expansion_ReadState(ParamsReadContext ctx)
	{
		float x;
		if (!ctx.Read(x))
			return false;

		float y;
		if (!ctx.Read(y))
			return false;

		float z;
		if (!ctx.Read(z))
			return false;

		m_Expansion_PositionSynch = Vector(x, y, z);

		float yaw;
		if (!ctx.Read(yaw))
			return false;

		float pitch;
		if (!ctx.Read(pitch))
			return false;

		float roll;
		if (!ctx.Read(roll))
			return false;

		m_Expansion_OrientationSynch = Vector(yaw, pitch, roll);

		bool fxEnabled;
		if (!ctx.Read(fxEnabled))
			return false;

		m_Expansion_AirdropContainerFX.Enable(fxEnabled);

		bool hasDiscardedParachute;
		if (!ctx.Read(hasDiscardedParachute))
			return false;

		if (hasDiscardedParachute && !m_Expansion_HasDiscardedParachute)
		{
			m_Expansion_HasDiscardedParachute = true;
			SetAnimationPhase( "parachute", 1 );
		}

		return true;
	}

	bool Expansion_IsFinal(vector position, vector orientation)
	{
		if (vector.DistanceSq(position, m_Expansion_PositionSynch) > 0.0001)
			return false;
			
		if (Math.AbsFloat(orientation[0] - m_Expansion_OrientationSynch[0]) > 0.1)
			return false;

		if (Math.AbsFloat(orientation[1] - m_Expansion_OrientationSynch[1]) > 0.1)
			return false;

		if (Math.AbsFloat(orientation[2] - m_Expansion_OrientationSynch[2]) > 0.1)
			return false;

		return true;
	}

	void Expansion_DisableFX()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		if (m_Expansion_AirdropContainerFX_Enabled)
		{
			m_Expansion_AirdropContainerFX_Enabled = false;
			SetSynchDirty();
		}
		else
		{
			m_Expansion_AirdropContainerFX_Enabled_Client = false;
			Expansion_SynchContainerStateToClient();
		}
	}
};

#ifdef EXPANSIONMODAI
[eAIRegisterDynamicPatrolSpawner(ExpansionAirdropContainerBase_Server)]
#endif
#ifdef SERVER
class ExpansionAirdropContainerBase_Server: ExpansionAirdropContainerBase
{
	void ExpansionAirdropContainerBase_Server()
	{
		RegisterNetSyncVariableBool("m_Expansion_AirdropContainerFX_Enabled");
		RegisterNetSyncVariableBool("m_Expansion_HasDiscardedParachute");
		RegisterNetSyncVariableBool("m_Expansion_HideCargoWhileParachuteIsDeployed");
	
		SetEventMask( EntityEvent.INIT | EntityEvent.CONTACT );
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		GetGame().ObjectDelete(this);
	}
};
#else
class ExpansionAirdropContainerBase_Server: Container_Base
{
	bool m_Expansion_AirdropContainerFX_Enabled = true;
	ref ExpansionAirdropContainerFX m_Expansion_AirdropContainerFX = new ExpansionAirdropContainerFX(this);
	protected bool m_Expansion_HasDiscardedParachute;
	bool m_Expansion_HideCargoWhileParachuteIsDeployed;

	void ExpansionAirdropContainerBase_Server()
	{
		RegisterNetSyncVariableBool("m_Expansion_AirdropContainerFX_Enabled");
		RegisterNetSyncVariableBool("m_Expansion_HasDiscardedParachute");
		RegisterNetSyncVariableBool("m_Expansion_HideCargoWhileParachuteIsDeployed");
	}

	override bool IsInventoryVisible()
	{
		return m_Expansion_HasDiscardedParachute || !m_Expansion_HideCargoWhileParachuteIsDeployed;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override void SetActions()
	{
	}

	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		super.OnVariablesSynchronized();
		
		m_Expansion_AirdropContainerFX.Enable(m_Expansion_AirdropContainerFX_Enabled);
	}
};
class ExpansionAirdropContainerBase_Client: ExpansionAirdropContainerBase
{
};
#endif
