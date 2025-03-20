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
#ifdef EXPANSIONMODAI
[eAIRegisterDynamicPatrolSpawner(ExpansionAirdropContainerBase)]
#endif
#ifdef SERVER
class ExpansionAirdropContainerBase: Container_Base
#else
class ExpansionAirdropContainerBase: House
#endif
{
	static ref map<int, ExpansionAirdropContainerBase> s_Expansion_AirdropContainers = new map<int, ExpansionAirdropContainerBase>;
	static int s_Expansion_AirdropContainerNextID;

	int m_Expansion_AirdropContainerID;

	string m_Expansion_ClientContainerType;

	ref array<ref ExpansionLoot> m_Expansion_AirdropLoot;
	int m_Expansion_ItemCount;

	bool m_FromSettings;
	protected bool m_HasLanded;
	protected bool m_IsLooted;
	float m_Expansion_FallSpeed = 4.5;
	vector m_Expansion_WindImpact;
	float m_Expansion_WindImpactStrength = 0.15;
	float m_Expansion_SimulationTimeAccumulator;
	float m_Expansion_SimulationTimeAccumulatorDiag;
	int m_Expansion_SynchCount;
	bool m_Expansion_HideCargoWhileParachuteIsDeployed;

	private int m_StartTime;
	
	//! Light
	ExpansionPointLight m_Light;
	protected bool m_LightOn = true;
	
	//! Particle
	Particle m_ParticleEfx;

	vector m_SpawnPosition;  //! @note position where container is spawned, not necessarily where it lands!
	vector m_Expansion_Position;

	//! MP client only
	vector m_Expansion_PositionSynch;
	vector m_Expansion_OrientationSynch;

	protected bool m_Expansion_IsUpdateEnabled;
	float m_Expansion_LastUpdateTickTime;

#ifdef EXPANSIONMODAI
#ifdef SERVER
	ref eAIDynamicPatrolSpawner<ExpansionAirdropContainerBase> m_eAI_DynamicPatrolSpawner;
#endif
#endif

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif	

		//RegisterNetSyncVariableBool("m_LightOn");
		//RegisterNetSyncVariableBool("m_IsLooted");
		
		//SetEventMask( EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.SIMULATE );

		if (GetGame().IsServer())
			Expansion_EnableUpdate();
		
		m_FromSettings = true;

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

		if (s_Expansion_AirdropContainers)
			s_Expansion_AirdropContainers.Remove(m_Expansion_AirdropContainerID);
	}

	override void EEInit()
	{
		super.EEInit();

		//if (GetGame().IsServer())
		//{
			m_SpawnPosition = GetPosition();
			m_Expansion_Position = m_SpawnPosition;
			m_Expansion_PositionSynch = m_SpawnPosition;

		#ifndef SERVER
			//! Show parachute on client
			SetAnimationPhase( "parachute", 0 );
		#endif

			m_Expansion_OrientationSynch = Vector( GetOrientation()[0], 0, 0 );
			SetOrientation( m_Expansion_OrientationSynch );
			//CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			//EnableDynamicCCD( true );
			//SetDynamicPhysicsLifeTime( -1 );
			
			m_StartTime = GetGame().GetTime();

			//dBodySetDamping(this, 0.0, 1.0);
		//}
	}

	//! EOnContact will only fire on server
	override void EOnContact(IEntity other, Contact extra)
	{
		ExpansionWorld.CheckTreeContact(other, 7500, true);

		Object obj;
		if (!m_HasLanded && Class.CastTo(obj, other))
		{
			if (obj.IsBuilding() || obj.IsPlainObject() || obj.IsRock() || obj.IsScenery() || obj.IsTree())
				Expansion_SetHasLanded();
		}
	}

	override void EEDelete( EntityAI parent )
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
#endif
		
		DestroyLight();
		
		StopSmokeEffect();

		if ( IsMissionHost() )
		{
			ExpansionAirdropContainerManagers.DeferredCleanup();

			Expansion_SendDeleteContainerOnClient();
		}
	}

	override bool CanObstruct()
	{
		return false;
	}

	bool HasLanded()
	{
		return m_HasLanded;
	}
	
	// ------------------------------------------------------------
	// LoadFromMission
	// ------------------------------------------------------------
	void LoadFromMission(  Class mission )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif	

		m_FromSettings = false;
	}
	
	void Expansion_SetAirdropContainerID(int containerID)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MISSIONS, this, "Airdrop container ID " + containerID);
	#endif

		m_Expansion_AirdropContainerID = containerID;
		s_Expansion_AirdropContainers[containerID] = this;
	}

	// ------------------------------------------------------------
	// InitAirdrop
	// ------------------------------------------------------------
	void InitAirdrop( string clientContainerType, array < ref ExpansionLoot > Loot, TStringArray infected, int ItemCount, int infectedCount, float fallSpeed = 4.5, float windImpact = 0.0 )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( IsMissionHost() )
		{
		#ifdef SERVER
			//! Hide the server container on client
			SetAnimationPhase( "parachute", 1 );
			SetAnimationPhase( "camo", 1 );
		#endif

			ExpansionAirdropContainerManagers.Add( this, infected, infectedCount );

			m_Expansion_ClientContainerType = clientContainerType;

			if (!m_Expansion_HideCargoWhileParachuteIsDeployed)
			{
				ExpansionLootSpawner.SpawnLoot( this, Loot, ItemCount );
			}
			else
			{
				m_Expansion_AirdropLoot = Loot;
				m_Expansion_ItemCount = ItemCount;
			}

			if (fallSpeed <= 0)
				fallSpeed = 4.5;

			float totalWeight = GetWeightEx();
			float totalWeightKg = totalWeight * 0.001;

			m_Expansion_FallSpeed = fallSpeed * ExpansionMath.LinearConversion(m_ConfigWeight, m_ConfigWeight * 1.666666, totalWeight, 1.0, 1.111111);

			//! The higher the fall speed, the lesser the wind impact
			m_Expansion_WindImpactStrength = ExpansionMath.LinearConversion(3.0, 6.0, m_Expansion_FallSpeed, 0.2, 0.1) * windImpact;

			EXLogPrint(ToString() + " InitAirdrop - total weight (kg) " + totalWeightKg + " - nominal fall speed " + m_Expansion_FallSpeed + " m/s, wind impact strength " + m_Expansion_WindImpactStrength);

			Expansion_SetAirdropContainerID(s_Expansion_AirdropContainerNextID++);
			Expansion_SendCreateContainerOnClient();
		}
	}

	void InitAirdropClient(float fallSpeed, float windImpactStrength)
	{
		m_Expansion_FallSpeed = fallSpeed;
		m_Expansion_WindImpactStrength = windImpactStrength;

		EXLogPrint(ToString() + " InitAirdropClient - nominal fall speed " + m_Expansion_FallSpeed + " m/s, wind impact strength " + m_Expansion_WindImpactStrength);

		UpdateLight();
		CreateSmoke();
	}

	void Expansion_SendCreateContainerOnClient(PlayerIdentity identity = null)
	{
		auto rpc = ExpansionMissionModule.s_Instance.Expansion_CreateRPC("RPC_CreateAirdropContainer");
		rpc.Write(m_Expansion_AirdropContainerID);
		rpc.Write(m_Expansion_ClientContainerType);
		rpc.Write(GetPosition());
		rpc.Write(GetOrientation());
		rpc.Write(m_Expansion_FallSpeed);
		rpc.Write(m_Expansion_WindImpactStrength);
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
		if (!ExpansionMissionModule.s_Instance)  //! Can be NULL on server shutdown
			return;

		auto rpc = ExpansionMissionModule.s_Instance.Expansion_CreateRPC("RPC_DeleteAirdropContainer");
		rpc.Write(m_Expansion_AirdropContainerID);
		rpc.Expansion_Send(true, identity);
	}

	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		CheckAirdrop();
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		CheckAirdrop();
	}

	// ------------------------------------------------------------
	// CheckAirdrop
	// ------------------------------------------------------------
	void CheckAirdrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		if ( IsMissionHost() && !m_IsLooted && IsEmpty() )
		{
			m_IsLooted = true;

			ToggleLight();

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

		//! Get current velocity
		//vector velocity = GetVelocity(this);
		vector velocity = position - m_Expansion_Position;

		m_Expansion_Position = position;

		bool guaranteed;

		if (GetGame().IsClient())
		{
			//! MP client

			vector orientation = GetOrientation();

			if (m_HasLanded && Expansion_IsFinal(position, orientation))
			{
				SetPosition(m_Expansion_PositionSynch);
				SetOrientation(m_Expansion_OrientationSynch);

				Expansion_DisableUpdate();
			}
			else
			{
				if (m_HasLanded)
					dt *= 2;

				SetPosition(vector.Lerp(position, m_Expansion_PositionSynch, dt * 2));
				SetOrientation(vector.Lerp(orientation, m_Expansion_OrientationSynch, dt * 2));
			}
		}
		else if (m_HasLanded)
		{
			if (velocity.LengthSq() < 0.0001 && dBodyGetAngularVelocity(this).LengthSq() < 0.0001)
			{
				//EnableDynamicCCD(false);
				SetDynamicPhysicsLifeTime(0);

				//ClearEventMask(EntityEvent.SIMULATE);
				Expansion_DisableUpdate();

				if (m_Expansion_HideCargoWhileParachuteIsDeployed && m_Expansion_AirdropLoot)
					ExpansionLootSpawner.SpawnLoot( this, m_Expansion_AirdropLoot, m_Expansion_ItemCount );

				EXTrace.Print(EXTrace.MISSIONS, this, "EOnSimulate - stopped - updating pathgraph region");

				SetAffectPathgraph(false, true);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);

				if (GetGame().IsDedicatedServer())
				{
					if (m_Expansion_SimulationTimeAccumulator > 0.0)
						Expansion_SynchContainerStateToClient();

					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_SynchContainerStateToClient, 100, false, true, null);
				}
			}
			else if (GetGame().IsDedicatedServer())
			{
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

			vector transform[4];
			GetTransform( transform );
			transform[3] = transform[3] + velocity * dt;

		#ifdef DIAG_DEVELOPER
			m_Expansion_SimulationTimeAccumulatorDiag += dt;
			if (m_Expansion_SimulationTimeAccumulatorDiag >= 1.0)
			{
				m_Expansion_SimulationTimeAccumulatorDiag = 0.0;
				float fallSpeed = -velocity[1];
				EXTrace.Print(EXTrace.MISSIONS, this, "EOnSimulate - dt " + dt + " - fall speed " + fallSpeed + " m/s");
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
			Expansion_SetHasLanded();
		}
	}
	
	private bool Expansion_CheckLanded()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		//! Ray input
		vector start = GetPosition();
		vector end = GetPosition();
		
		//! Ray output
		vector hit;
		vector hitpos;
		int hitindex;
		set<Object> results = new set<Object>;
		
		if (DayZPhysics.RaycastRV( start, end, hitpos, hit, hitindex, results, NULL, this, false, false, ObjIntersectFire, 0.5, CollisionFlags.ALLOBJECTS))
		{
			foreach (Object result: results)
			{
				//! Bushes do not have collision, so we deal with them here
				if ((result.IsBush() || result.IsTree()) && !result.IsDamageDestroyed())
					ExpansionWorld.CheckTreeContact(result, 7500, true);

				//! We consider anything that can obstruct as colliding as well as rocks, trees and vehicles
				if (ExpansionStatic.CanObstruct(result) || result.IsRock() || result.IsTree() || result.IsTransport())
					return true;
			}
		}

		if (start[1] <= ExpansionStatic.GetSurfaceRoadPosition(start, RoadSurfaceDetection.CLOSEST)[1] + 0.05)
			return true;

		return false;
	}

	void Expansion_SetHasLanded()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
#endif 
		
		m_HasLanded = true;

		CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );

		dBodySetDamping(this, 0.5, 0.5);

		SetDynamicPhysicsLifeTime( ( GetGame().GetTime() - m_StartTime ) + 30 );

		//! Set parachute animation phase so parachute is hidden
		SetAnimationPhase( "parachute", 1 );
		
		ExpansionAirdropContainerManager manager = ExpansionAirdropContainerManagers.Find( this );
		if ( manager )
		{
			manager.m_ContainerPosition = GetPosition();

			if ( GetExpansionSettings().GetAirdrop().ServerMarkerOnDropLocation )
				manager.CreateServerMarker(); //! Set server map marker on drop position

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

	// ------------------------------------------------------------
	// Expansion OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		super.OnVariablesSynchronized();
		
		//UpdateLight();

		//if ( !m_LightOn )
			//StopSmokeEffect();
	}
	
	void Expansion_SynchContainerStateToClient(bool guaranteed = true, PlayerIdentity identity = null)
	{
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

		rpc.Write(m_LightOn);
		rpc.Write(m_HasLanded);

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

		if (!ctx.Read(m_LightOn))
			return false;

		bool hasLanded;
		if (!ctx.Read(hasLanded))
			return false;

		if (hasLanded && !m_HasLanded)
		{
			m_HasLanded = true;
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

	// ------------------------------------------------------------
	// CreateLight
	// Create Chemlight object on server side (light is on client side)
	// ------------------------------------------------------------
	protected void CreateLight()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
				
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() ) //! Client side
		{
			if ( !m_Light ) 
			{
				vector container_pos = GetPosition();
				
				m_Light = CreateAirdropLight();
				
				m_Light.SetDiffuseColor( 1, 0.1, 0.1 );
				m_Light.SetRadiusTo( 20 );
				m_Light.SetBrightnessTo( 0.6 );
				m_Light.SetFlareVisible( true );
				m_Light.AttachOnObject( this, GetMemoryPointPos("light") );
			}
		}
	}
	
	// ------------------------------------------------------------
	// CreateAirdropLight
	// Return default ExpansionPointLight object
	// ------------------------------------------------------------
	ExpansionPointLight CreateAirdropLight()
	{
		return ExpansionPointLight.Cast( ScriptedLightBase.CreateLight(ExpansionPointLight) );
	}
	
	// ------------------------------------------------------------
	// ToggleLight
	// ------------------------------------------------------------
	void ToggleLight()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		m_LightOn = !m_LightOn;
		
		Expansion_SynchContainerStateToClient();
	}
	
	// ------------------------------------------------------------
	// UpdateLight
	// ------------------------------------------------------------
	void UpdateLight()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() ) // Client side
		{
			if ( m_LightOn )
			{
				if ( !m_Light )
				{
					m_Light = CreateAirdropLight();
				
					m_Light.SetDiffuseColor( 1, 0.1, 0.1 );
					m_Light.SetRadiusTo( 20 );
					m_Light.SetBrightnessTo( 0.6 );
					m_Light.SetFlareVisible( true );
					m_Light.AttachOnObject( this, GetMemoryPointPos("light") );
				}
			}
			else
			{		
				if ( m_Light )
				{
					m_Light.FadeOut();
					m_Light = null;
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// DestroyLight
	// ------------------------------------------------------------
	protected void DestroyLight()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( m_Light )
			m_Light.Destroy();
	}
		
	// ------------------------------------------------------------
	// CreateSmoke
	// Create the particle effect on client side only!
	// ------------------------------------------------------------
	protected void CreateSmoke()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() ) //! Client side
		{
			m_ParticleEfx = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, this, GetMemoryPointPos("light") );
		}
	}
	
	// ------------------------------------------------------------
	// StopSmokeEffect
	// ------------------------------------------------------------
	protected void StopSmokeEffect()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( IsMissionClient() )
		{	
			if ( m_ParticleEfx )
			{
				m_ParticleEfx.Stop();

				m_ParticleEfx = null;
			}
		}
	}
};

#ifdef SERVER
class ExpansionAirdropContainerBase_Server: ExpansionAirdropContainerBase
{
	void ExpansionAirdropContainerBase_Server()
	{
		RegisterNetSyncVariableBool("m_HasLanded");
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
	protected bool m_HasLanded;
	bool m_Expansion_HideCargoWhileParachuteIsDeployed;

	void ExpansionAirdropContainerBase_Server()
	{
		RegisterNetSyncVariableBool("m_HasLanded");
		RegisterNetSyncVariableBool("m_Expansion_HideCargoWhileParachuteIsDeployed");
	}

	override bool IsInventoryVisible()
	{
		return m_HasLanded || !m_Expansion_HideCargoWhileParachuteIsDeployed;
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
};
class ExpansionAirdropContainerBase_Client: ExpansionAirdropContainerBase
{
};
#endif
