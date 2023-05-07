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
class ExpansionAirdropContainerBase: Container_Base
{
	const int EXPANSION_AIRDROP_RPC_ZSPAWN_PARTICLE = 120009009;
	
	bool m_FromSettings;
	protected bool m_HasLanded;
	protected bool m_IsLooted;
	float m_Expansion_FallSpeed = 3.0;
	float m_Expansion_WindImpactStrength;
	float m_Expansion_PreviousAltitude;
	float m_Expansion_SimulationTimeAccumulator;

	private int m_StartTime;
	
	//! Light
	ExpansionPointLight m_Light;
	protected bool m_LightOn = true;
	
	//! Particle
	Particle m_ParticleEfx;

	vector m_SpawnPosition;  //! @note position where container is spawned, not necessarily where it lands!

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif	

		RegisterNetSyncVariableBool("m_LightOn");
		RegisterNetSyncVariableBool("m_IsLooted");
		
		SetEventMask( EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.SIMULATE ); 

		SetAnimationPhase( "parachute", 1 );	
		
		m_FromSettings = true;
		
		UpdateLight();
		CreateSmoke();
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
			CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			EnableDynamicCCD( true );
			SetDynamicPhysicsLifeTime( -1 );
			
			m_StartTime = GetGame().GetTime();
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

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
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
	void InitAirdrop(  array < ref ExpansionLoot > Loot, TStringArray infected, int ItemCount, int infectedCount, float fallSpeed = 3.0, bool windImpact = 0.0 )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( IsMissionHost() )
		{
			ExpansionAirdropContainerManagers.Add( this, infected, infectedCount );

			ExpansionLootSpawner.SpawnLoot( this, Loot, ItemCount );

			if (fallSpeed <= 0)
				fallSpeed = 3.0;

			float totalWeight = GetWeightEx();
			float totalWeightKg = totalWeight * 0.001;

			m_Expansion_FallSpeed = fallSpeed * totalWeight / m_ConfigWeight;

			//! The higher the fall speed, the lesser the wind impact
			if (windImpact)
				m_Expansion_WindImpactStrength = ExpansionMath.LinearConversion(3.0, 6.0, m_Expansion_FallSpeed, 0.4, 0.2);

			EXLogPrint(this, "InitAirdrop - total weight (kg) " + totalWeightKg + " - nominal fall speed " + m_Expansion_FallSpeed + " m/s, wind impact strength " + m_Expansion_WindImpactStrength);

			dBodySetDamping(this, 0.0, 1.0);

			m_Expansion_PreviousAltitude = GetPosition()[1];
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
		}
	}

	override void EOnSimulate(IEntity other, float dt)
	{
		if (!GetGame().IsServer())
			return;

		if (!Expansion_CheckLanded())
		{
			vector windImpact;

			if ( m_Expansion_WindImpactStrength > 0.0 )
			{
				if ( GetGame() && GetGame().GetWeather() )
				{
					vector wind = GetGame().GetWeather().GetWind();

					windImpact[0] = wind[0] * m_Expansion_WindImpactStrength;
					windImpact[2] = wind[2] * m_Expansion_WindImpactStrength;
				}
			}

			vector velocity = Vector(windImpact[0], -m_Expansion_FallSpeed * 0.9093, windImpact[2]);

			SetVelocity(this, velocity);

			vector transform[4];
			GetTransform( transform );

		#ifdef DIAG
			m_Expansion_SimulationTimeAccumulator += dt;
			if (m_Expansion_SimulationTimeAccumulator >= 1.0)
			{
				m_Expansion_SimulationTimeAccumulator = 0.0;
				vector position = transform[3];
				float fallSpeed = m_Expansion_PreviousAltitude - position[1];
				EXTrace.Print(EXTrace.MISSIONS, this, "EOnSimulate - fall speed " + fallSpeed + " m/s");
				m_Expansion_PreviousAltitude = position[1];
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
				if (result.IsBush() && !result.IsDamageDestroyed())
					ExpansionWorld.CheckTreeContact(result, 7500, true);
			}

			return true;
		}

		return false;
	}

	void Expansion_SetHasLanded()
	{
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);

		m_HasLanded = true;

		ClearEventMask(EntityEvent.SIMULATE);

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

		if ( m_IsLooted )
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

				GetGame().ObjectDelete( m_ParticleEfx );
			}
		}
	}

	// ------------------------------------------------------------
	// IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return true;
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

		ExpansionLootSpawner.RemoveContainer(this);
	}

	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{		
		vector spawnPos;
		if ( rpc_type == EXPANSION_AIRDROP_RPC_ZSPAWN_PARTICLE)
		{
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return;
			
			if (!ctx.Read( spawnPos ))
				return;
			
			SpawnParticle( spawnPos );
		} 
		else
		{
			super.OnRPC( sender, rpc_type, ctx );
		}
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
