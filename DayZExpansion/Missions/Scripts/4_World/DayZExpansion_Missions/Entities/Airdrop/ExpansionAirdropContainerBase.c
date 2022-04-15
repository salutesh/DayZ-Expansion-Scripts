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
	void InitAirdrop(  array < ref ExpansionAirdropLoot > Loot, TStringArray infected, int ItemCount, int infectedCount )
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

			SpawnLoot( Loot, ItemCount );
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
					
					this.dBodyApplyImpulse( this, mass * m_wind * deltaTime ); //! Null pointer with deltaTime
				}
			} else {
				this.dBodyApplyImpulse( this, "0 9.0 0" * mass * deltaTime );
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
	// Expansion AddItem
	// ------------------------------------------------------------
	void AddItem(  ExpansionAirdropLoot loot )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::AddItem - Start");
		#endif

		string className = loot.Name;
		
		TStringArray attachments = loot.Attachments;

		if ( loot.Variants && loot.Variants.Count() > 0 )
		{
			array< float > chances = new array< float >;

			int count = loot.Variants.Count();
			float chance;
			float chancesSum;

			for ( int j = 0; j < count; ++j )
			{
				chance = loot.Variants[j].Chance;
				chances.Insert( chance );
				chancesSum += chance;
			}

			//! Determine chance for parent item
			if ( chancesSum < 1.0 )
			{
				//! Chances are treated as actual chances here, i.e. total sum is 1.0
				chance = 1.0 - chancesSum;
			} else
			{
				//! Just give parent item a 1.0 chance
				chance = 1.0;
			}

			chances.Insert( chance );

			int index = ExpansionStatic.GetWeightedRandom( chances );

			if ( index > -1 && index < count )
			{
				className = loot.Variants[index].Name;
				if ( loot.Variants[index].Attachments && loot.Variants[index].Attachments.Count() > 0 )
					attachments = loot.Variants[index].Attachments;
			}
		}

		Spawn( className, this, loot.QuantityPercent, attachments ); 
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::AddItem - End");
		#endif
	}

	void Spawn( string className, ItemBase parent, int quantityPercent, TStringArray attachments )
	{
		ItemBase item = ItemBase.Cast( parent.ExpansionCreateInInventory( className ) );

		if ( item )
		{
			Magazine_Base mag;

			float quantity;
			float quantity01;

			if (quantityPercent > 0)
			{
				quantity01 = quantityPercent / 100;
			}
			else if (quantityPercent == -2)
			{
				CEItemProfile profile = item.GetEconomyProfile();

				if (profile)
				{
					float quantityMin01 = profile.GetQuantityMin();
					float quantityMax01 = profile.GetQuantityMax();

					if (quantityMin01 >= 0 && quantityMax01 > 0)
						quantity01 = Math.RandomFloatInclusive( quantityMin01, quantityMax01 );
					else
						quantityPercent = -1;
				}
			}

			if ( Class.CastTo( mag, item ) )
			{
				//! Ammo/magazines

				if (quantityPercent == -1)
					quantity = mag.GetAmmoMax();
				else
					quantity = Math.Round( mag.GetAmmoMax() * quantity01 );

				if (!quantity && item.IsAmmoPile())
					quantity = 1;

				mag.ServerSetAmmoCount( quantity );
			}
			else if ( item.HasQuantity() )
			{
				if (quantityPercent == -1)
					quantity = item.GetQuantityInit();
				else
					quantity = Math.Round( item.GetQuantityMax() * quantity01 );

				item.SetQuantity( quantity );
			}

			if ( attachments )
			{
				//! Yes this is needed, otherwise you get NULL pointers if there is more than one attachment :-(
				TStringArray attachmentsTmp();
				attachmentsTmp.Copy(attachments);

				foreach ( string attachment: attachmentsTmp )
				{
					Spawn( attachment, item, quantityPercent, NULL );
				}
			}
		}
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

		RemoveContainer();
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveContainer
	// ------------------------------------------------------------
	void RemoveContainer()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::RemoveContainer - Start");
		#endif
		
		array< EntityAI > items = new array< EntityAI >;
		this.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );
		
		for ( int i = 0; i < items.Count(); ++i )
		{
			GetGame().ObjectDelete(items[i]);
		}
		
		GetGame().ObjectDelete( this );
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::RemoveContainer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SpawnLoot
	// ------------------------------------------------------------
	void SpawnLoot(  array < ref ExpansionAirdropLoot > Loot, int ItemCount )
	{
		array< float > chances = new array< float >;
		array< int > max = new array< int >;

		for ( int i = 0; i < Loot.Count(); ++i )
		{
			chances.Insert( Loot[i].Chance );

			max.Insert( Loot[i].Max );
		}

		int LootItemsSpawned = 0;
		while ( LootItemsSpawned < ItemCount )
		{
			//! Chances are treated as weights here, otherwise it wouldn't make sense as we always want a fixed number of items
			int index = ExpansionStatic.GetWeightedRandom( chances );

			if ( index > -1 )
			{
				LootItemsSpawned++;

				AddItem( Loot.Get( index ) );

				if ( max[index] > 0 )
					max[index] = max[index] - 1;

				if ( max[index] == 0 )
					chances[index] = 0;
			} else
			{
				Print("ExpansionAirdropContainerBase::SpawnLoot couldn't select a loot item to spawn (all chances zero?) - items spawned : " + LootItemsSpawned);
				break;
			}
		}
	}

	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		vector spawnPos;
		if ( rpc_type == EXPANSION_AIRDROP_RPC_ZSPAWN_PARTICLE && ctx.Read( spawnPos ) )
		{
			SpawnParticle( spawnPos );
		} else
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
