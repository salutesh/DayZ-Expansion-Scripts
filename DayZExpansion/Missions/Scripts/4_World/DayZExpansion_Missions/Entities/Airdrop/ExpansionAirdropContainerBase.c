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
class ExpansionAirdropContainerBase extends Container_Base
{
	const int EXPANSION_AIRDROP_RPC_ZSPAWN_PARTICLE = 120009009;
	
	bool m_FromSettings;
	protected bool m_HasLanded;
	protected bool m_IsLooted;
	protected bool m_HasWindImpact;

	private int m_StartTime;
	
	//! Light
	ExpansionPointLight m_Light;
	protected bool m_LightOn = true;
	
	//! Particle
	Particle m_ParticleEfx;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::ExpansionAirdropContainerBase - Start");
		#endif	

		RegisterNetSyncVariableBool("m_LightOn");
		RegisterNetSyncVariableBool("m_IsLooted");
		
		SetEventMask( EntityEvent.INIT ); 

		SetAnimationPhase( "parachute", 1 );	
		
		m_FromSettings = true;
		m_HasLanded = false;
		m_HasWindImpact = false;
		
		UpdateLight();
		CreateSmoke();

		if ( IsMissionHost() )
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( CheckAirdrop, 5000, true );
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::ExpansionAirdropContainerBase - End");
		#endif	
	}
	
	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::~ExpansionAirdropContainerBase - Start");
		#endif	
		
		DestroyLight();
		
		StopSmokeEffect();

		if ( IsMissionHost() )
			ExpansionAirdropContainerManagers.DeferredCleanup();
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::~ExpansionAirdropContainerBase - End");
		#endif
	}

	void SetWindImpact(bool state)
	{
		m_HasWindImpact = state;
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
		EXLogPrint("ExpansionAirdropContainerBase::LoadFromMission - Start");
		#endif	

		m_FromSettings = false;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::LoadFromMission - End");
		#endif	
	}
	
	// ------------------------------------------------------------
	// InitAirdrop
	// ------------------------------------------------------------
	void InitAirdrop(  array < ref ExpansionLoot > Loot, TStringArray infected, int ItemCount, int infectedCount )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::InitAirdrop - Start");
		#endif
		
		if ( IsMissionHost() )
		{				
			GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( this.OnUpdate );
	
			SetAnimationPhase( "parachute", 0 );
			SetOrientation( Vector( GetOrientation()[0], 0, 0 ) );
			SetDirection( GetDirection() );
			CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			EnableDynamicCCD( true );
			SetDynamicPhysicsLifeTime( -1 );
			
			m_StartTime = GetGame().GetTime();

			ExpansionAirdropContainerManagers.Add( this, infected, infectedCount );

			ExpansionLootSpawner.SpawnLoot( this, Loot, ItemCount );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::InitAirdrop - End");
		#endif
	}

	// ------------------------------------------------------------
	// CheckAirdrop
	// ------------------------------------------------------------
	void CheckAirdrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::CheckAirdrop - Start");
		#endif

		if ( IsMissionHost() && !m_IsLooted && GetNumberOfItems() == 0 )
		{
			m_IsLooted = true;

			ToggleLight();

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( CheckAirdrop );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::CheckAirdrop - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion OnUpdate
	// ------------------------------------------------------------
	void OnUpdate( float deltaTime )
	{		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint( "ExpansionAirdropContainerBase::OnUpdate - Start");
		#endif
		
		if ( !IsGround( 0.5 ) ) 
		{
			float mass = dBodyGetMass( this );

			if ( m_HasWindImpact )
			{
				if ( GetGame() && GetGame().GetWeather() )
				{
					vector m_wind = GetGame().GetWeather().GetWind();

					m_wind[0] = ( ( m_wind[0] + 0.1 ) * 2 ) / 100;
					m_wind[1] = 9.0;
					m_wind[2] = ( ( m_wind[2] + 0.1 ) * 2 ) / 100;
					
					dBodyApplyImpulse( this, mass * m_wind * deltaTime );
				}
			} else {
				dBodyApplyImpulse( this, "0 9.0 0" * mass * deltaTime );
			}
		} else if ( !m_HasLanded )
		{
			m_HasLanded = true;

			GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( this.OnUpdate );
	
	   		SetDynamicPhysicsLifeTime( ( GetGame().GetTime() - m_StartTime ) + 30 );

			//! Set parachute animation phase so parachute is hiden 
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
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint( "ExpansionAirdropContainerBase::OnUpdate - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion IsGround
	// Check distance to ground
	// ------------------------------------------------------------
	private bool IsGround( float height )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::IsGround - Start");
		#endif
		
		//! Ray input
		vector start = GetPosition();
		vector end = GetPosition() - Vector( 0, height, 0 );
		
		//! Ray output
		vector hit;
		vector hitpos;
		
		//! Ray hitindex output
		int hitindex;

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::IsGround - End and return height: " + DayZPhysics.RaycastRV( start, end, hitpos, hit, hitindex, NULL, NULL, this ).ToString());
		#endif
		
		//! Ray
		return DayZPhysics.RaycastRV( start, end, hitpos, hit, hitindex, NULL, NULL, this );
	}
	
	// ------------------------------------------------------------
	// Expansion OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::OnVariablesSynchronized - Start");
		#endif
		
		super.OnVariablesSynchronized();
		
		UpdateLight();

		if ( m_IsLooted )
			StopSmokeEffect();

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::OnVariablesSynchronized - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// CreateLight
	// Create Chemlight object on server side (light is on client side)
	// ------------------------------------------------------------
	protected void CreateLight()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint( "ExpansionAirdropContainerBase::CreateLight - Start" );
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
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint( "ExpansionAirdropContainerBase::CreateLight - End" );
		#endif
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
		EXLogPrint("ExpansionAirdropContainerBase::ToggleLight - Start");
		#endif
		
		m_LightOn = !m_LightOn;
		
		SetSynchDirty();
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::ToggleLight - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// UpdateLight
	// ------------------------------------------------------------
	void UpdateLight()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::UpdateLight - Start");
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
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::UpdateLight - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// DestroyLight
	// ------------------------------------------------------------
	protected void DestroyLight()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::DestroyLight - Start");
		#endif
		
		if ( m_Light )
			m_Light.Destroy();
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::DestroyLight - Start");
		#endif
	}
		
	// ------------------------------------------------------------
	// CreateSmoke
	// Create the particle effect on client side only!
	// ------------------------------------------------------------
	protected void CreateSmoke()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::CreateSmoke - Start");
		#endif
		
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() ) //! Client side
		{
			m_ParticleEfx = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, this, GetMemoryPointPos("light") );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::CreateSmoke - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// StopSmokeEffect
	// ------------------------------------------------------------
	protected void StopSmokeEffect()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::StopSmokeEffect - Start");
		#endif
		
		if ( IsMissionClient() )
		{	
			if ( m_ParticleEfx )
			{
				m_ParticleEfx.Stop();

				GetGame().ObjectDelete( m_ParticleEfx );
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::StopSmokeEffect - End");
		#endif
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
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "LongDeferredInit");
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
