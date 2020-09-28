/**
 * ExpansionAirdropContainerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	ExpansionMarkerData m_ServerMarker;
	
	bool m_FromSettings;
	bool m_LootHasSpawned;
	bool m_IsLooted;
	
	int m_ItemsCount;
	int m_Infected;

	int m_ItemCount;

	ExpansionAirdropLoot m_LootTier;

	private int m_StartTime;
	
	//! Light
	ExpansionPointLight m_Light;
	protected bool m_LightOn;
	
	//! Particle
	Particle m_ParticleEfx;
	
	protected AIGroup m_AIGroup;
	
	protected ExpansionMarkerModule m_MarkerModule;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionAirdropContainerBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::ExpansionAirdropContainerBase - Start");
		#endif	

		RegisterNetSyncVariableBool("m_LightOn");
		
		SetEventMask( EntityEvent.INIT ); 

		SetAnimationPhase( "parachute", 1 );	
		
		m_FromSettings = true;
		m_LootHasSpawned = false;
		
		if ( !Class.CastTo(m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) ) )
			return;
		
		CreateSmoke();
		ToggleLight();

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.CheckAirdrop, 5000, true );
		
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
		
		//RemoveServerMarker();
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::~ExpansionAirdropContainerBase - End");
		#endif	
	}
	
	// ------------------------------------------------------------
	// StopUpdateQue
	// ------------------------------------------------------------
	void StopUpdateQue()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( this.OnUpdate );
	}
	
	// ------------------------------------------------------------
	// LoadFromMission
	// ------------------------------------------------------------
	void LoadFromMission( ref Class mission )
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
	void InitAirdrop()
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
			SetLifetimeMax( 1.0 ); // shouldn't be required, why is this here? - jacob
			
			m_StartTime = GetGame().GetTime();
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::InitAirdrop - End");
		#endif
	}

	// ------------------------------------------------------------
	// InitAirdrop
	// ------------------------------------------------------------
	void CheckAirdrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::CheckAirdrop - Start");
		#endif
				
		array< EntityAI > items = new array< EntityAI >;
		GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		if ( items.Count() < m_ItemCount )
		{
			if ( !m_IsLooted && IsMissionHost() )
			{
				ToggleLight();

				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.CheckAirdrop );
				
				m_IsLooted = true;
			} 

			if ( m_IsLooted && IsMissionClient() )
			{				
				StopSmokeEffect();
			}
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
			this.dBodyApplyImpulse( this, "0 9.0 0" * mass * deltaTime );
		} else if ( !m_LootHasSpawned )
		{
	   		SetDynamicPhysicsLifeTime( ( GetGame().GetTime() - m_StartTime ) + 30 );

			//! Set parachute animation phase so parachute is hiden 
			SetAnimationPhase( "parachute", 1 );

			if ( GetExpansionSettings().GetAirdrop().ServerMarkerOnDropLocation && IsMissionHost())
			{
				//! Set server map marker on drop position
				CreateServerMarker();
			}
			
			m_LootHasSpawned = true;
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint( "ExpansionAirdropContainerBase::OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion AddItem
	// ------------------------------------------------------------
	void AddItem( ref ExpansionAirdropLootAttachments className )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::AddItem - Start");
		#endif
		
		ItemBase item = ItemBase.Cast( GetInventory().CreateInInventory( className.Name ) ); 

		if ( className.Attachments != NULL )
		{
			for ( int i; i < className.Attachments.Count(); i++ )
			{
				if ( item )
				{
					item.GetInventory().CreateInInventory( className.Attachments.Get( i ) );
				}
			}
		}

		array< EntityAI > items = new array< EntityAI >;
		GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		m_ItemCount = items.Count();
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::AddItem - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CreateServerMarker
	// ------------------------------------------------------------
	void CreateServerMarker()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::CreateServerMarker - Start");
		#endif
		
		string markerName = "#STR_EXPANSION_AIRDROP_SYSTEM_TITLE";
		if ( GetExpansionSettings().GetAirdrop().ShowAirdropTypeOnMarker )
			markerName = "[" + this.GetDisplayName() + "] " + markerName;
		
		m_ServerMarker = m_MarkerModule.CreateServerMarker( markerName, "Airdrop", this.GetPosition(), ARGB(255, 235, 59, 90), GetExpansionSettings().GetAirdrop().Server3DMarkerOnDropLocation );
	
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::CreateServerMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveServerMarker
	// ------------------------------------------------------------
	void RemoveServerMarker()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::RemoveServerMarker - Start");
		#endif
		
		if (!m_ServerMarker)
		{
			#ifdef EXPANSION_MISSION_EVENT_DEBUG
			EXLogPrint("ExpansionAirdropContainerBase::RemoveServerMarker - m_ServerMarker is NULL");
			#endif
			return;
		}
		
		if ( m_ServerMarker )
		{
			#ifdef EXPANSION_MISSION_EVENT_DEBUG
			EXLogPrint("ExpansionAirdropContainerBase::RemoveServerMarker - Remove server marker with UID: " + m_ServerMarker.GetUID());
			#endif
			m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		}
			
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::RemoveServerMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// SpawnInfected
	// ------------------------------------------------------------
	protected void SpawnInfected( vector centerPosition, float innerRadius, float spawnRadius )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::SpawnInfected - Start");
		#endif
		
		m_AIGroup = GetGame().GetWorld().GetAIWorld().CreateGroup( "ExpansionInfectedPatrolGroupBeh" );
		array<ref BehaviourGroupInfectedPackWaypointParams> waypointParams = new array<ref BehaviourGroupInfectedPackWaypointParams>;

		waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 5, 0 ), 2.0 ) );
		waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 20, 5 ), 20.0 ) );
		waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 45, 35 ), 50.0 ) );
		waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 30, 25 ), 20.0 ) );
		waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 5, 0 ), 2.0 ) );

		BehaviourGroupInfectedPack bgip;
		Class.CastTo( bgip, m_AIGroup.GetBehaviour() );
		if ( bgip )
		{
			bgip.SetWaypoints( waypointParams, 0, true, false );
		}

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::SpawnInfected - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// SampleSpawnPosition
	// ------------------------------------------------------------
	protected vector SampleSpawnPosition( vector position, float maxRadius, float innerRadius )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::SampleSpawnPosition - Start");
		#endif
		
		float a = Math.RandomFloatInclusive( 0.0, 1.0 ) * Math.PI2;
		float r = maxRadius * Math.RandomFloatInclusive( innerRadius / maxRadius, 1 );

		float spawnX = r * Math.Cos( a );
		float spawnZ = r * Math.Sin( a );

		vector nPosition = "0 0 0";

		nPosition[0] = position[0] + spawnX;
		nPosition[2] = position[2] + spawnZ;
		nPosition[1] = GetGame().SurfaceY( nPosition[0], nPosition[2] );

		AIWorld aiWorld = GetGame().GetWorld().GetAIWorld();

		PGFilter filter = new PGFilter();
		filter.SetFlags( PGPolyFlags.NONE, PGPolyFlags.NONE, PGPolyFlags.NONE );
		filter.SetCost( PGAreaType.TERRAIN, 10 );

		aiWorld.SampleNavmeshPosition( nPosition, maxRadius, filter, nPosition );

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::SampleSpawnPosition - End and return nPosition: " + nPosition.ToString());
		#endif
		
		return nPosition;
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
	// Expansion IsPlayerNearby
	// Check if player is nearby container
	// ------------------------------------------------------------
	bool IsPlayerNearby(float radius)
	{		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::IsPlayerNearby - Start");
		#endif
		
		vector pos = this.GetPosition();
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);
		float distance;
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			Man player = players[i];
			if (!player) continue;
			
			distance = vector.Distance( pos, player.GetPosition() );
			
			if ( distance <= radius )
			{
				#ifdef EXPANSION_MISSION_EVENT_DEBUG
				EXLogPrint("ExpansionAirdropContainerBase::IsPlayerNearby - End and return true");
				#endif
				return true;
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::IsPlayerNearby - End and return false");
		#endif
		
		return false;
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
		
		UpdateLight();
		
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
	// CanUseConstruction
	// ------------------------------------------------------------
	override bool CanUseConstruction()
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
	// CanReceiveItemIntoCargo
	// ------------------------------------------------------------
	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		if ( GetHealthLevel() == GameConstants.STATE_RUINED )
			return false;

		return super.CanReceiveItemIntoCargo( item );
	}
	
	// ------------------------------------------------------------
	// AfterStoreLoad
	// ------------------------------------------------------------
	//! Called when entity is being loaded from DB or Storage.
	// This will remove the saved containers after they got loaded from CE.
	override void AfterStoreLoad()
	{
		array< EntityAI > items = new array< EntityAI >;
		this.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );
		
		for ( int i = 0; i < items.Count(); ++i )
		{
			GetGame().ObjectDelete(items[i]);
		}
		
		GetGame().ObjectDelete( this );
	}	
};