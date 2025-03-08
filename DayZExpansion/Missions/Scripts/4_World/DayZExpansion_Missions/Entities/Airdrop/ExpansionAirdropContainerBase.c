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
class ExpansionAirdropContainerBase: Container_Base
{
	static int s_Expansion_SpawnParticle_RPCID;

	bool m_FromSettings;
	protected bool m_HasLanded;
	protected bool m_IsLooted;
	float m_Expansion_FallSpeed = 4.5;
	float m_Expansion_WindImpactStrength = 0.15;
	float m_Expansion_PreviousAltitude;
	float m_Expansion_SimulationTimeAccumulator;
	bool m_Expansion_HideCargoWhileParachuteIsDeployed;

	private int m_StartTime;
	
	//! Light
	ExpansionPointLight m_Light;
	protected bool m_LightOn = true;
	
	//! Particle
	Particle m_ParticleEfx;

	vector m_SpawnPosition;  //! @note position where container is spawned, not necessarily where it lands!

	float m_Expansion_LastUpdateTickTime;

#ifdef EXPANSIONMODAI
	ref eAIDynamicPatrolSpawner<ExpansionAirdropContainerBase> m_eAI_DynamicPatrolSpawner;
#endif

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif	

		RegisterNetSyncVariableBool("m_LightOn");
		RegisterNetSyncVariableBool("m_HasLanded");
		RegisterNetSyncVariableBool("m_IsLooted");
		RegisterNetSyncVariableBool("m_Expansion_HideCargoWhileParachuteIsDeployed");
		
		//SetEventMask( EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.SIMULATE );

		if (GetGame().IsServer())
		{
			m_Expansion_LastUpdateTickTime = GetGame().GetTickTime();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_Update, 25, true);
		}

		SetAnimationPhase( "parachute", 1 );	
		
		m_FromSettings = true;
		
		UpdateLight();
		CreateSmoke();

		if (GetGame().IsServer())
		{
			m_Expansion_HideCargoWhileParachuteIsDeployed = GetExpansionSettings().GetAirdrop().HideCargoWhileParachuteIsDeployed;

		#ifdef EXPANSIONMODAI
			m_eAI_DynamicPatrolSpawner = new eAIDynamicPatrolSpawner<ExpansionAirdropContainerBase>(this);
			m_eAI_DynamicPatrolSpawner.SetDeferDespawnUntilLoosingAggro(true);
		#endif
		}

		if (!m_Expansion_RPCManager)
			m_Expansion_RPCManager = new ExpansionRPCManager(this, ItemBase);
		if (!s_Expansion_SpawnParticle_RPCID)
			s_Expansion_SpawnParticle_RPCID = m_Expansion_RPCManager.RegisterClient("RPC_SpawnParticle");
	}
	
	void ~ExpansionAirdropContainerBase()
	{
		if (GetGame() && GetGame().IsServer() && GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_Update);
	}

	override void EEInit()
	{
		super.EEInit();

		if (GetGame().IsServer())
		{
			m_SpawnPosition = GetPosition();
	
			SetAnimationPhase( "parachute", 0 );
			SetOrientation( Vector( GetOrientation()[0], 0, 0 ) );
			SetDirection( GetDirection() );
			//CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			//EnableDynamicCCD( true );
			//SetDynamicPhysicsLifeTime( -1 );
			
			m_StartTime = GetGame().GetTime();

			//dBodySetDamping(this, 0.0, 1.0);

			m_Expansion_PreviousAltitude = GetPosition()[1];
		}
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
			ExpansionAirdropContainerManagers.DeferredCleanup();
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
	
	// ------------------------------------------------------------
	// InitAirdrop
	// ------------------------------------------------------------
	void InitAirdrop(  array < ref ExpansionLoot > Loot, TStringArray infected, int ItemCount, int infectedCount, float fallSpeed = 4.5, float windImpact = 0.0 )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( IsMissionHost() )
		{
			ExpansionAirdropContainerManagers.Add( this, infected, infectedCount );

			ExpansionLootSpawner.SpawnLoot( this, Loot, ItemCount );

			if (fallSpeed <= 0)
				fallSpeed = 4.5;

			float totalWeight = GetWeightEx();
			float totalWeightKg = totalWeight * 0.001;

			m_Expansion_FallSpeed = fallSpeed * ExpansionMath.LinearConversion(m_ConfigWeight, m_ConfigWeight * 1.666666, totalWeight, 1.0, 1.111111);

			//! The higher the fall speed, the lesser the wind impact
			if (windImpact)
				m_Expansion_WindImpactStrength = ExpansionMath.LinearConversion(3.0, 6.0, m_Expansion_FallSpeed, 0.2, 0.1) * windImpact;

			EXLogPrint(ToString() + " InitAirdrop - total weight (kg) " + totalWeightKg + " - nominal fall speed " + m_Expansion_FallSpeed + " m/s, wind impact strength " + m_Expansion_WindImpactStrength);
		}
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
		//! Get current velocity
		vector velocity = GetVelocity(this);

		if (m_HasLanded)
		{
			if (velocity.LengthSq() < 0.0001 && dBodyGetAngularVelocity(this).LengthSq() < 0.0001)
			{
				//EnableDynamicCCD(false);
				//SetDynamicPhysicsLifeTime(0);

				//ClearEventMask(EntityEvent.SIMULATE);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_Update);

				EXTrace.Print(EXTrace.MISSIONS, this, "EOnSimulate - stopped - updating pathgraph region");

				SetAffectPathgraph(false, true);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);
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

					//! Apply directional wind impact
					//dBodyApplyImpulse(this, windImpact * dt * 0.5);
					velocity[0] = velocity[0] + windImpact[0] * dt * 0.5;
					velocity[2] = velocity[2] + windImpact[2] * dt * 0.5;

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
			velocity[1] = -m_Expansion_FallSpeed * dt;

			//SetVelocity(this, velocity);

			vector transform[4];
			GetTransform( transform );
			transform[3] = transform[3] + velocity;

		#ifdef DIAG_DEVELOPER
			m_Expansion_SimulationTimeAccumulator += dt;
			if (m_Expansion_SimulationTimeAccumulator >= 1.0)
			{
				m_Expansion_SimulationTimeAccumulator = 0.0;
				vector position = transform[3];
				float fallSpeed = m_Expansion_PreviousAltitude - position[1];
				EXTrace.Print(EXTrace.MISSIONS, this, "EOnSimulate - dt " + dt + " - fall speed " + fallSpeed + " m/s");
				m_Expansion_PreviousAltitude = position[1];
				if (DayZPlayerImplement.s_Expansion_DebugObjects_Enabled)
					GetGame().CreateObjectEx("ExpansionDebugRodBig", position, ECE_NOLIFETIME);
			}
		#endif

			MoveInTime( transform, dt );
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
		
		if (DayZPhysics.RaycastRV( start, end, hitpos, hit, hitindex, results, NULL, this, false, false, ObjIntersectView, 0.5, CollisionFlags.ALLOBJECTS))
		{
			foreach (Object result: results)
			{
				//! Bushes do not have collision, so we deal with them here
				if ((result.IsBush() || result.IsTree()) && !result.IsDamageDestroyed())
					ExpansionWorld.CheckTreeContact(result, 7500, true);
			}

			//return true;
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

		//dBodySetDamping(this, 0.5, 0.5);

		//SetDynamicPhysicsLifeTime( ( GetGame().GetTime() - m_StartTime ) + 30 );

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
		
		UpdateLight();

		if ( !m_LightOn )
			StopSmokeEffect();
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
		
		SetSynchDirty();
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

	// ------------------------------------------------------------
	// IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return m_HasLanded || !m_Expansion_HideCargoWhileParachuteIsDeployed;
	}

	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// AfterStoreLoad
	// ------------------------------------------------------------
	//! Called when entity is being loaded from DB or Storage.
	// This will remove the saved containers after they got loaded from CE.
	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif

		super.AfterStoreLoad();

		GetGame().ObjectDelete(this);
	}

	void RPC_SpawnParticle(PlayerIdentity sender, ParamsReadContext ctx)
	{
		vector spawnPos;
		if (!ctx.Read( spawnPos ))
			return;
		
		SpawnParticle( spawnPos );
	}

	protected void SpawnParticle( vector spawnPos )
	{
		//! Play spawn sound
		SEffectManager.PlaySound( "Expansion_Airdrop_ZSpawn_SoundSet", spawnPos );

		//! Create dirt particle
		Particle particle = Particle.PlayInWorld( ParticleList.IMPACT_DIRT_RICOCHET, spawnPos );

		particle.ScaleParticleParam( EmitorParam.SIZE, 10 );
		particle.ScaleParticleParam( EmitorParam.BIRTH_RATE, 5 );
		particle.ScaleParticleParam( EmitorParam.BIRTH_RATE_RND, 5 );
		particle.ScaleParticleParam( EmitorParam.LIFETIME, 5 );
		particle.ScaleParticleParam( EmitorParam.LIFETIME_RND, 5 );
	}
};
