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
	bool m_FromSettings;
	bool m_LootHasSpawned;

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
	
	int m_ServerMarker;
	string m_Location;
	bool m_IsLooted;
	bool m_Cleanup;
	
	protected ExpansionMapMarkerModule m_MarkerModule;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionAirdropContainerBase()
	{
		RegisterNetSyncVariableBool("m_LightOn");
		
		SetEventMask( EntityEvent.INIT ); 

		SetAnimationPhase( "parachute", 1 );	
		
		m_FromSettings = true;
		m_LootHasSpawned = false;
		m_IsLooted = false;
		m_Cleanup = false;
		
		m_ServerMarker = -1;
		m_MarkerModule = ExpansionMapMarkerModule.Cast( GetModuleManager().GetModule( ExpansionMapMarkerModule ) );
		
		CreateSmoke();
		ToggleLight();

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.CheckAirdrop, 5000, true );
		//! GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ClearAirdrop, 60000, false );
	}
	
	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionAirdropContainerBase()
	{
	}
	
	// ------------------------------------------------------------
	// ClearAirdrop
	// ------------------------------------------------------------
	void ClearAirdrop()
	{
		array< EntityAI > items = new array< EntityAI >;

		if ( GetGame().IsServer() ) 
		{		
			GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

			GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( this.OnUpdate );
				
			//RemoveServerMarker();

			foreach ( EntityAI item : items )
			{
				GetGame().ObjectDelete( item );
			}
		}
		else if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			StopSmokeEffect();
		}
		
		GetGame().ObjectDelete( this );
	}
	
	// ------------------------------------------------------------
	// EEDelete
	//! Not sure if this gets executed on object deletion
	// ------------------------------------------------------------
	override void EEDelete(EntityAI parent)
	{		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionAirdropContainerBase::EEDelete - Start");
		#endif	

		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( this.OnUpdate );
			
		//RemoveServerMarker();
		
		DestroyLight();
		
		StopSmokeEffect();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionAirdropContainerBase::EEDelete - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// LoadFromMission
	// ------------------------------------------------------------
	void LoadFromMission( ref Class mission )
	{
		m_FromSettings = false;
	}
	
	// ------------------------------------------------------------
	// InitAirdrop
	// ------------------------------------------------------------
	void InitAirdrop(string location)
	{
		if ( !location )
			return;	
		
		if ( GetGame() && GetGame().IsMultiplayer() || !GetGame().IsClient() )
		{				
			GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( this.OnUpdate );
	
			m_Location = location;
			
			SetAnimationPhase( "parachute", 0 );
			SetOrientation( Vector( GetOrientation()[0], 0, 0 ) );
			SetDirection( GetDirection() );
			CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			EnableDynamicCCD( true );
			SetDynamicPhysicsLifeTime( -1 );
        	
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.ClearAirdrop );
			
			m_StartTime = GetGame().GetTime();
		}
	}

	// ------------------------------------------------------------
	// InitAirdrop
	// ------------------------------------------------------------
	void CheckAirdrop()
	{
		array< EntityAI > items = new array< EntityAI >;
		GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		if ( items.Count() < m_ItemCount )
		{
			if ( !m_IsLooted && GetGame().IsServer() )
			{
				//! Delete light source
				ToggleLight();
				
				//! Delete drop server map and 3D marker
				//RemoveServerMarker();
				
				//! Announce player who hase claimed aidrop
				//! GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", "AIRDROP" ), new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_LOOTED", nearbyPlayer.GetIdentityName(), m_Location ), "set:expansion_notification_iconset image:icon_airdrop", COLOR_EXPANSION_NOTIFICATION_MISSION, 7 );
			
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ClearAirdrop, 3600000, false, this ); //! Remove crate after 15 minutes

				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.CheckAirdrop );
				
				m_IsLooted = true;
			} 
			else if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{				
				//! Stop smoke
				StopSmokeEffect();

				m_IsLooted = true;
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion OnUpdate
	// ------------------------------------------------------------
	void OnUpdate( float deltaTime )
	{		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionAirdropContainerBase::OnUpdate - Start");
		#endif
		
		if ( !IsGround( 0.5 ) ) 
		{		
			float mass = dBodyGetMass( this );
			dBodyApplyImpulse( this, "0 9.0 0" * mass * deltaTime );
		} 
		else if ( !m_LootHasSpawned )
		{
       		SetDynamicPhysicsLifeTime( ( GetGame().GetTime() - m_StartTime ) + 30 );

			//! Set parachute animation phase so parachute is hiden 
			SetAnimationPhase( "parachute", 1 );

			//! Create server map and 3D marker on crate pos
			//CreateServerMarker( m_Location );
			
			m_LootHasSpawned = true;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionAirdropContainerBase::OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion AddItem
	// ------------------------------------------------------------
	void AddItem( ref ExpansionAirdropLootAttachments className )
	{
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

		m_ItemCount++;
	}
	
	// ------------------------------------------------------------
	// Expansion CreateServerMarker
	// ------------------------------------------------------------
	void CreateServerMarker(string name)
	{
		if ( GetGame() && GetGame().IsMultiplayer() || !GetGame().IsClient() )
		{
			m_ServerMarker = m_MarkerModule.AddServerMarker( "[" + name + "] " + GetDisplayName(), 1, GetPosition(), ARGB(255, 235, 59, 90), true );

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionAirdropContainerBase::CreateServerMarker - m_ServerMarker index is: " + m_ServerMarker );
			#endif
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveServerMarker
	// ------------------------------------------------------------
	void RemoveServerMarker()
	{	
		if ( GetGame() && GetGame().IsMultiplayer() || !GetGame().IsClient() )
		{
			if ( m_ServerMarker != -1 )
			{
				m_MarkerModule.RemoveServerMarker( m_ServerMarker );
				
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionAirdropContainerBase::RemoveServerMarker - m_ServerMarker index is: " + m_ServerMarker );
				#endif
			}
		}
	}
	
	// ------------------------------------------------------------
	// SpawnInfected
	// ------------------------------------------------------------
	protected void SpawnInfected( vector centerPosition, float innerRadius, float spawnRadius )
	{
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

		// SpawnInfectedRemaining( centerPosition, innerRadius, spawnRadius, m_Settings.Infected );
	}
	
	// ------------------------------------------------------------
	// SampleSpawnPosition
	// ------------------------------------------------------------
	protected vector SampleSpawnPosition( vector position, float maxRadius, float innerRadius )
	{
		float a = Math.RandomFloatInclusive( 0.0, 1.0 ) * Math.PI2;
        float r = maxRadius * Math.RandomFloatInclusive( innerRadius / maxRadius, 1 );

		float spawnX = r * Math.Cos( a );
		float spawnZ = r * Math.Sin( a );

		vector nPosition = "0 0 0";

		nPosition[0] = position[0] + spawnX;
		nPosition[2] = position[2] + spawnZ;
		nPosition[1] = GetGame().SurfaceY( nPosition[0], nPosition[2] );

		AIWorld aiWorld = GetGame().GetWorld().GetAIWorld();
		aiWorld.SampleNavmeshPosition( nPosition, maxRadius, nPosition );

		return nPosition;
	}

	// ------------------------------------------------------------
	// Expansion IsGround
	// Check distance to ground
	// ------------------------------------------------------------
	private bool IsGround( float height )
	{
		//! Ray input
		vector start = GetPosition();
		vector end = GetPosition() - Vector( 0, height, 0 );
		
		//! Ray output
		vector hit;
		vector hitpos;
		
		//! Ray hitindex output
		int hitindex;

		//! Ray
		return DayZPhysics.RaycastRV( start, end, hitpos, hit, hitindex, NULL, NULL, this );
	}	
	
	// ------------------------------------------------------------
	// Expansion IsPlayerNearby
	// Check if player is nearby container
	// ------------------------------------------------------------
	private bool IsPlayerNearby(float radius)
	{		
		vector pos = GetPosition();
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);
		float distance;
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			Man player = players[i];
			if (!player) continue;
			
			distance = vector.Distance( pos, player.GetPosition() );
			
			if ( distance <= radius )
				return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		UpdateLight();
	}
	
	// ------------------------------------------------------------
	// CreateLight
	// Create Chemlight object on server side (light is on client side)
	// ------------------------------------------------------------
	protected void CreateLight()
	{
		#ifdef EXPANSIONEXLOGPRINT
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
		
		#ifdef EXPANSIONEXLOGPRINT
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
		m_LightOn = !m_LightOn;
		
		SetSynchDirty();
		
		UpdateLight();
	}
	
	// ------------------------------------------------------------
	// UpdateLight
	// ------------------------------------------------------------
	void UpdateLight()
	{
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionAirdropContainerBase::DestroyLight - Start");
		#endif
		
		if ( m_Light )
			m_Light.Destroy();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionAirdropContainerBase::DestroyLight - Start");
		#endif
	}
		
	// ------------------------------------------------------------
	// CreateSmoke
	// Create the particle effect on client side only!
	// ------------------------------------------------------------
	protected void CreateSmoke()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionAirdropContainerBase::CreateSmoke - Start");
		#endif
		
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() ) //! Client side
		{
			m_ParticleEfx = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, this, GetMemoryPointPos("light") );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionAirdropContainerBase::CreateSmoke - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// StopSmokeEffect
	// ------------------------------------------------------------
	protected void StopSmokeEffect()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionAirdropContainerBase::StopSmokeEffect - Start");
		#endif
		
		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() ) //! Client side
		{	
			if ( m_ParticleEfx )
			{
				m_ParticleEfx.Stop();

				GetGame().ObjectDelete( m_ParticleEfx );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
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
}