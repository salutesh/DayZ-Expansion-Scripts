/**
 * ExpansionMissionEventAirdrop.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMissionEventAirdrop extends ExpansionMissionEventBase
{
	bool ShowNotification;
	
	float Height;
	float Speed;

	string Container;
	
	ref ExpansionAirdropLocation DropLocation;

	ref array < ref ExpansionAirdropLootAttachments > Loot;
	ref TStringArray Infected;
	int ItemCount;
	int InfectedCount;

	[NonSerialized()]
	ExpansionAirdropPlane m_Plane;

	[NonSerialized()]
	ExpansionAirdropContainerBase m_Container;

	[NonSerialized()]
	autoptr array< Object > m_Infected;

	[NonSerialized()]
	AIGroup m_AIGroup;
	
	[NonSerialized()]
	bool m_LootHasSpawned;

	[NonSerialized()]
	int m_LootItemsSpawned;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventAirdrop
	// ------------------------------------------------------------
	void ExpansionMissionEventAirdrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::ExpansionMissionEventAirdrop - Start");
		#endif
		
		m_EventName = "Airdrop";
		m_Infected = new array< Object >;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::ExpansionMissionEventAirdrop - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnStart
	// ------------------------------------------------------------
	// handle mission start
	override void Event_OnStart()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnStart - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			vector spawnPoint = Vector( 0, Height, 0 );
			
			switch( Math.RandomInt(0, 4) ) {
				case 0: {
					spawnPoint[0] = 0.0;
					spawnPoint[2] = Math.RandomFloat( 0.0, 15360.0 );
					break;
				}
				case 1: {
					spawnPoint[0] = 15360.0;
					spawnPoint[2] = Math.RandomFloat( 0.0, 15360.0 );
					break;
				}
				case 2: {
					spawnPoint[0] = Math.RandomFloat( 0.0, 15360.0 );
					spawnPoint[2] = 0.0;
					break;
				}
				case 3: {
					spawnPoint[0] = Math.RandomFloat( 0.0, 15360.0 );
					spawnPoint[2] = 15360.0;
					break;
				}
			}
	
			m_Plane = ExpansionAirdropPlane.Cast( GetGame().CreateObjectEx("ExpansionAirdropPlane", spawnPoint, ECE_AIRBORNE|ECE_CREATEPHYSICS) );
			
			if ( m_Plane )
			{
				if (ShowNotification || GetExpansionSettings().GetNotification().ShowAirdropStarted)
				{
					CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_HEADING_TOWARDS", DropLocation.Name ), "set:expansion_notification_iconset image:icon_airdrop", 7 );
				}
				
				m_Plane.SetupPlane( Vector( DropLocation.x, 0, DropLocation.z ), DropLocation.Name, DropLocation.Radius, Speed );
				m_Plane.m_Height = Height;
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnStart - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnEnd
	// ------------------------------------------------------------
	// handle mission cleanup
	override void Event_OnEnd()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnEnd - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			//! Remove the plane
			RemovePlane();
	
			//! After mission ends check all 60 seconds if a player is nearby the airdrop crate and if not delete the container and the zeds
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.CleanupCheck, 60, true );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnEnd - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveContainer
	// ------------------------------------------------------------
	void RemoveContainer()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::RemoveContainer - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			if ( !m_Container )
				return;
			
			array< EntityAI > items = new array< EntityAI >;
			m_Container.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );
			
			for ( int i = 0; i < items.Count(); ++i )
			{
				GetGame().ObjectDelete(items[i]);
			}
			
			//m_Container.RemoveServerMarker();
			GetGame().ObjectDelete( m_Container );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerBase::RemoveContainer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CleanupCheck
	// ------------------------------------------------------------
	void CleanupCheck()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::CleanupCheck - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			if ( !m_Container )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.CleanupCheck );
				return;
			}
			
			//! Check if a player is nearby the container in a 1500 meter radius
			if ( !IsPlayerNearby(100) )
			{	
				m_Container.RemoveServerMarker();				
				RemoveContainer();
				
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.CleanupCheck );
				
				while ( m_Infected.Count() > 0 )
				{
					int index = m_Infected.Count() - 1;
		
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, index * 5, false, m_Infected[ index ] );
		
					m_Infected.Remove( index );
				}
				
				if (ShowNotification /*|| GetExpansionSettings().GetNotification().ShowAirdropEnded*/)
				{
					CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_ENDED", DropLocation.Name ), "set:expansion_notification_iconset image:icon_airdrop", 7 );
				}
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::CleanupCheck - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion IsPlayerNearby
	// Check if player is nearby container
	// ------------------------------------------------------------
	bool IsPlayerNearby(float radius)
	{		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::IsPlayerNearby - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			vector pos = m_Container.GetPosition();
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
					EXLogPrint("ExpansionMissionEventAirdrop::IsPlayerNearby - End and return true");
					#endif
					return true;
				}
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::IsPlayerNearby - End and return false");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion GetValueString
	// ------------------------------------------------------------
	protected string GetValueString( float total_value )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::GetValueString - Start");
		#endif
		
		if ( total_value < 0 )
			return "0";
	
		int value = total_value;
		string out_string;
		
		if ( value >= 1000 )
		{
			string value_string = value.ToString();
			
			int count;		
			int first_length = value_string.Length() % 3;		//calculate position of the first separator
			if ( first_length > 0 )
			{
				count = 3 - first_length;
			}
			
			for ( int i = 0; i < value_string.Length(); ++i )
			{
				out_string += value_string.Get( i );
				count ++;
				
				if ( count >= 3 )
				{
					out_string += " ";			//separator
					count = 0;
				}
			}
		}
		else
		{
			out_string = value.ToString();
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::GetValueString - End and return out_string: " + out_string);
		#endif
		
		return out_string;
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnUpdate
	// ------------------------------------------------------------
	// update tick for the mission
	override void Event_OnUpdate( float delta )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnUpdate - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			if ( m_Plane )
			{			
				if ( m_Plane.CheckForDrop() && !m_Container )
				{
					m_LootHasSpawned = false;
	
					m_Container = m_Plane.CreateDrop( Container );
					
					if (!m_Container)
					{
						Error( "ExpansionMissionEventAirdrop::Event_OnUpdate - ERROR: Could not create container object!" );
						return;
					}
					
					if (m_Container)
					{
						m_Container.InitAirdrop( );
						m_Container.SetLifetimeMax( 10800 );
					
						if (ShowNotification || GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowAirdropDropped)
							CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_SUPPLIES_DROPPED", DropLocation.Name ), "set:expansion_notification_iconset image:icon_airdrop", 10 );
					}
				}
	
				if ( m_Plane.IsWarningProximity() )
				{
					if (ShowNotification || GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowAirdropClosingOn)
						CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_CLOSING_ON", DropLocation.Name ), "set:expansion_notification_iconset image:icon_airdrop", 7 );	
				}
	
				if ( m_Plane.CheckForRemove() )
				{
					RemovePlane( );
				}
			}
	
			if ( m_Container )
			{
				if ( m_Container.m_ItemCount == 0 && !m_LootHasSpawned )
				{
					SpawnLoot( m_Container.GetPosition(), 30 );
	
					SpawnInfected( m_Container.GetPosition(), 50 );
	
					m_CurrentMissionTime = 0;
	
					m_LootHasSpawned = true;
				}
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RemovePlane
	// ------------------------------------------------------------
	void RemovePlane()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::RemovePlane - Start");
		#endif
	
		if ( IsMissionHost() )
		{	
			if ( !m_Plane )
				return;
	
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, 0, false, m_Plane );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::RemovePlane - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnLoadMission
	// ------------------------------------------------------------
	protected override void OnLoadMission()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnLoadMission - Start");
		#endif

		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionMissionEventAirdrop>.JsonLoadFile( m_FileName, this );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnLoadMission - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnSaveMission
	// ------------------------------------------------------------
	protected override void OnSaveMission()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnSaveMission - Start");
		#endif

		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionMissionEventAirdrop>.JsonSaveFile( m_FileName, this );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnSaveMission - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion MaxDefaultMissions
	// ------------------------------------------------------------
	override int MaxDefaultMissions()
	{
		return 52;
	}
	
	// ------------------------------------------------------------
	// Expansion OnDefaultMission
	// ------------------------------------------------------------
	protected override string OnDefaultMission( int index )
	{
		Enabled = true;

		Weight = 1 + ( ( index + 1 ) * 5 );
		MissionMaxTime = 1200; // 20 minutes

		Speed = 25.0;
		Height = 750.0;
		
		ShowNotification = true;
		
		//! Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		//! Vanilla Maps
		if ( world_name.Contains( "chernarusplus" ) )
		{
			return ExpansionMissionAirdropChernarus( index );
		} else if ( world_name.Contains( "enoch" ) )
		{
			return ExpansionMissionAirdropLivonia( index );
		} else if ( world_name.Contains( "deerisle" ) )
		{
			return ExpansionMissionAirdropDeerIsle( index );
		}
		
		return "Airdrop_Template";
	}
		
	// ------------------------------------------------------------
	// Expansion DefaultGeneralLoot
	// ------------------------------------------------------------
	private void DefaultGeneralLoot()
	{
		TStringArray ak74u_1 = { "AKS74U_Bttstck" };

		TStringArray akm_1 = { "AK_WoodBttstck", "AK_WoodHndgrd" };

		TStringArray sks_1 = { "PUScopeOptic" };
		TStringArray sks_2 = { "SKS_Bayonet" };

		TStringArray ump_1 = { "UniversalLight" };

		TStringArray mosin_1 = { "PUScopeOptic" };

		TStringArray b95_1 = { "HuntingOptic" };

		TStringArray cz527_1 = { "HuntingOptic" };

		TStringArray cz75_1 = { "TLRLight" };

		TStringArray fnx1_1 = { "TLRLight" };
		
		TStringArray kedr_1 = { "PistolSuppressor" };  

		TStringArray winchester70_1 = { "HuntingOptic" };

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };
		TStringArray visor = { "DirtBikeHelmet_Visor" };
		
		TStringArray firstaidkit_1 = { "BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing" };
		TStringArray firstaidkit_2 = { "BandageDressing","SalineBagIV" };
		TStringArray firstaidkit_3 = { "CharcoalTablets","Morphine" };
		TStringArray firstaidkit_4 = { "Epinephrine","StartKitIV" };
		TStringArray firstaidkit_5 = { "Morphine","SalineBagIV" };
		TStringArray firstaidkit_6 = { "PainkillerTablets","Epinephrine" };
		TStringArray firstaidkit_7 = { "TetracyclineAntiBiotics","Morphine" };
		TStringArray firstaidkit_8 = { "BandageDressing","VitaminBottle" };

		Container = "ExpansionAirdropContainer";

		Loot = {
			new ExpansionAirdropLootAttachments( "AKS74U", ak74u_1, 0.08 ),
			
			new ExpansionAirdropLootAttachments( "AKM", akm_1, 0.05 ),

			new ExpansionAirdropLootAttachments( "SKS", sks_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "SKS", sks_2, 0.1  ),
			new ExpansionAirdropLootAttachments( "SKS", NULL, 0.3 ),

			new ExpansionAirdropLootAttachments( "UMP45", ump_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "UMP45", NULL, 0.15 ),
 
			new ExpansionAirdropLootAttachments( "Mosin9130", mosin_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "Mosin9130", NULL, 0.3 ),
 
			new ExpansionAirdropLootAttachments( "B95", b95_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "B95", NULL, 0.2 ),

			new ExpansionAirdropLootAttachments( "CZ527", cz527_1, 0.3 ),
			new ExpansionAirdropLootAttachments( "CZ527", NULL, 0.2 ),

			new ExpansionAirdropLootAttachments( "CZ75", cz75_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "CZ75", NULL, 0.1 ),

			new ExpansionAirdropLootAttachments( "FNX45", fnx1_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "FNX45", NULL, 0.3 ),

			new ExpansionAirdropLootAttachments( "Expansion_Kedr", kedr_1, 0.2 ),
			new ExpansionAirdropLootAttachments( "Expansion_Kedr", NULL, 0.3 ),

   			new ExpansionAirdropLootAttachments( "Mp133Shotgun", NULL, 0.8 ),

			new ExpansionAirdropLootAttachments( "Winchester70", winchester70_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "Winchester70", NULL, 0.4 ),
				
			new ExpansionAirdropLootAttachments( "Expansion_DT11", NULL, 0.3 ),
			
			new ExpansionAirdropLootAttachments( "Binoculars", NULL, 0.3 ),
			new ExpansionAirdropLootAttachments( "ChernarusMap", NULL, 0.3 ),
			new ExpansionAirdropLootAttachments( "Rangefinder", battery, 0.05 ),
			new ExpansionAirdropLootAttachments( "ExpansionGPS", NULL, 0.05 ),
			
			new ExpansionAirdropLootAttachments( "BoxCerealCrunchin", NULL, 0.05 ),
			new ExpansionAirdropLootAttachments( "PeachesCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "BakedBeansCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "SpaghettiCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "SardinesCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "TunaCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "WaterBottle", NULL, 0.5 ),
			
			new ExpansionAirdropLootAttachments( "CanOpener", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "KitchenKnife", NULL, 0.3 ),
			
			new ExpansionAirdropLootAttachments( "BallisticHelmet_UN", NULL, 0.08 ),
			new ExpansionAirdropLootAttachments( "DirtBikeHelmet_Chernarus", visor, 0.3 ),
			
			new ExpansionAirdropLootAttachments( "SewingKit", NULL, 0.25 ),
			new ExpansionAirdropLootAttachments( "LeatherSewingKit", NULL, 0.25 ),
			new ExpansionAirdropLootAttachments( "WeaponCleaningKit", NULL, 0.05 ),
			new ExpansionAirdropLootAttachments( "Lockpick", NULL, 0.05 ),
			
			new ExpansionAirdropLootAttachments( "GhillieAtt_Mossy", NULL, 0.05 ),

			new ExpansionAirdropLootAttachments( "Mag_Expansion_Kedr_20Rnd", NULL, 0.8 ),
			new ExpansionAirdropLootAttachments( "Mag_CZ527_5rnd", NULL, 0.9 ),
			new ExpansionAirdropLootAttachments( "Mag_CZ75_15Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Mag_FNX45_15Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Mag_UMP_25Rnd", NULL, 0.5 ),

			new ExpansionAirdropLootAttachments( "AmmoBox_9x39_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_9x19_25Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39Tracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39_20Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_45ACP_25Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_308WinTracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_308Win_20Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_12gaSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_12gaRubberSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_12gaPellets_10Rnd", NULL, 1 ),
			
			new ExpansionAirdropLootAttachments( "Ammo_9x39", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "Ammo_762x39Tracer", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "Ammo_762x39", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_45ACP", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_308WinTracer", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "Ammo_308Win", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_12gaSlug", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_12gaRubberSlug", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_12gaPellets", NULL,  0.5 ),
			
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_1 , 0.03),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_2 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_3 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_4 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_5 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_6 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_7 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_8 , 0.05),
		};

		Infected = {
			"ZmbM_HermitSkinny_Base",
			"ZmbM_HermitSkinny_Beige",
			"ZmbM_HermitSkinny_Black",
			"ZmbM_HermitSkinny_Green",
			"ZmbM_HermitSkinny_Red",
			"ZmbM_FarmerFat_Base",
			"ZmbM_FarmerFat_Beige",
			"ZmbM_FarmerFat_Blue",
			"ZmbM_FarmerFat_Brown",
			"ZmbM_FarmerFat_Green",
			"ZmbF_CitizenANormal_Base",
			"ZmbF_CitizenANormal_Beige",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Blue",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Grey",
			"ZmbF_CitizenANormal_Red",
			"ZmbM_CitizenBFat_Blue",
			"ZmbM_CitizenBFat_Red",
			"ZmbM_CitizenBFat_Green",
			"ZmbF_CitizenBSkinny",
			"ZmbM_FishermanOld_Blue",
			"ZmbM_FishermanOld_Green",
			"ZmbM_FishermanOld_Grey",
			"ZmbM_FishermanOld_Red",
			"ZmbM_JournalistSkinny",
			"ZmbF_JournalistNormal_Blue",
			"ZmbF_JournalistNormal_Green",
			"ZmbF_JournalistNormal_Red",
			"ZmbF_JournalistNormal_White",
			"ZmbM_HiderSkinny_Blue",
			"ZmbM_HiderSkinny_Green",
			"ZmbM_HiderSkinny_Yellow",
			"ZmbF_HiderSkinny_Blue",
			"ZmbF_HiderSkinny_Grey",
			"ZmbF_HiderSkinny_Green",
			"ZmbF_HiderSkinny_Red",
			"ZmbF_SurvivorNormal_Blue",
			"ZmbF_SurvivorNormal_Orange",
			"ZmbF_SurvivorNormal_Red",
			"ZmbF_SurvivorNormal_White",
			"ZmbM_CommercialPilotOld_Blue",
			"ZmbM_CommercialPilotOld_Olive",
			"ZmbM_CommercialPilotOld_Brown",
			"ZmbM_CommercialPilotOld_Grey",
			"ZmbM_JoggerSkinny_Blue",
			"ZmbM_JoggerSkinny_Green",
			"ZmbM_JoggerSkinny_Red",
			"ZmbF_JoggerSkinny_Blue",
			"ZmbF_JoggerSkinny_Brown",
			"ZmbF_JoggerSkinny_Green",
			"ZmbF_JoggerSkinny_Red",
			"ZmbM_MotorBikerFat_Beige",
			"ZmbM_MotorBikerFat_Black",
			"ZmbM_MotorBikerFat_Blue",
			"ZmbM_VillagerOld_Blue",
			"ZmbM_VillagerOld_Green",
			"ZmbM_VillagerOld_White",
			"ZmbM_SkaterYoung_Blue",
			"ZmbM_SkaterYoung_Brown",
			"ZmbM_SkaterYoung_Green",
			"ZmbM_SkaterYoung_Grey",
			"ZmbF_SkaterYoung_Brown",
			"ZmbF_SkaterYoung_Striped",
			"ZmbF_SkaterYoung_Violet",
			"ZmbM_OffshoreWorker_Green",
			"ZmbM_OffshoreWorker_Orange",
			"ZmbM_OffshoreWorker_Red",
			"ZmbM_OffshoreWorker_Yellow",
			"ZmbM_Jacket_beige",
			"ZmbM_Jacket_black",
			"ZmbM_Jacket_blue",
			"ZmbM_Jacket_bluechecks",
			"ZmbM_Jacket_brown",
			"ZmbM_Jacket_green",
			"ZmbM_Jacket_khaki",
			"ZmbM_Jacket_magenta",
			"ZmbM_Jacket_stripes",
			"ZmbF_ShortSkirt_beige",
			"ZmbF_ShortSkirt_black",
			"ZmbF_ShortSkirt_brown",
			"ZmbF_ShortSkirt_green",
			"ZmbF_ShortSkirt_grey",
			"ZmbF_ShortSkirt_checks",
			"ZmbF_ShortSkirt_red",
			"ZmbF_ShortSkirt_stripes",
			"ZmbF_ShortSkirt_white",
			"ZmbF_ShortSkirt_yellow",
			"ZmbF_VillagerOld_Blue",
			"ZmbF_VillagerOld_Green",
			"ZmbF_VillagerOld_Red",
			"ZmbF_VillagerOld_White",
			"ZmbF_MilkMaidOld_Beige",
			"ZmbF_MilkMaidOld_Black",
			"ZmbF_MilkMaidOld_Green",
			"ZmbF_MilkMaidOld_Grey",
		};

		ItemCount = 30;
		InfectedCount = 25;
	}
		
	// ------------------------------------------------------------
	// Expansion DefaultMedicalLoot
	// ------------------------------------------------------------
	private void DefaultMedicalLoot()
	{
		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };
		
		TStringArray firstaidkit_1 = { "BandageDressing","BandageDressing" };
		TStringArray firstaidkit_2 = { "BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing" };
		TStringArray firstaidkit_3 = { "Morphine","BandageDressing" };
		TStringArray firstaidkit_4 = { "BandageDressing","SalineBagIV" };
		TStringArray firstaidkit_5 = { "PainkillerTablets","DisinfectantAlcohol" };
		TStringArray firstaidkit_6 = { "CharcoalTablets","Morphine" };
		TStringArray firstaidkit_7 = { "Epinephrine","StartKitIV" };
		TStringArray firstaidkit_8 = { "TetracyclineAntiBiotics","VitaminBottle" };
		TStringArray firstaidkit_9 = { "Morphine","SalineBagIV" };
		TStringArray firstaidkit_10 = { "PainkillerTablets","Epinephrine" };
		TStringArray firstaidkit_11 = { "TetracyclineAntiBiotics","Morphine" };
		TStringArray firstaidkit_12 = { "VitaminBottle","SurgicalGloves_Blue" };
		TStringArray firstaidkit_13 = { "BandageDressing","VitaminBottle" };
		TStringArray firstaidkit_14 = { "CharcoalTablets","DisinfectantAlcohol" };
		
		Container = "ExpansionAirdropContainer_Medical";

		Loot = {
			new ExpansionAirdropLootAttachments( "BandageDressing", NULL , 0.15),

			new ExpansionAirdropLootAttachments( "Morphine", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Epinephrine", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "TacticalBaconCan", NULL , 0.05),

			new ExpansionAirdropLootAttachments( "PainkillerTablets", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "CharcoalTablets", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "TetracyclineAntiBiotics", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "DisinfectantSpray", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "DisinfectantAlcohol", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "VitaminBottle", NULL , 0.2),

			new ExpansionAirdropLootAttachments( "StartKitIV", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "SalineBagIV", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "SalineBag", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "BloodBagEmpty", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "BloodBagIV", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "BloodTestKit", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "BloodTestKit", NULL , 0.2),

			new ExpansionAirdropLootAttachments( "MedicalScrubsHat_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsHat_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsHat_Green", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsPants_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsPants_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsPants_Green", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsShirt_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsShirt_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsShirt_Green", NULL , 0.08),

			new ExpansionAirdropLootAttachments( "NioshFaceMask", NULL , 0.08),

			new ExpansionAirdropLootAttachments( "SurgicalGloves_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "SurgicalGloves_LightBlue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "SurgicalGloves_Green", NULL , 0.08),		

			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_1 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_2 , 0.1),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_3 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_4 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_5 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_6 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_7 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_8 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_9 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_10 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_11 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_12 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_13 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_14 , 0.18),
		};

		Infected = {
			"ZmbM_HermitSkinny_Base",
			"ZmbM_HermitSkinny_Beige",
			"ZmbM_HermitSkinny_Black",
			"ZmbM_HermitSkinny_Green",
			"ZmbM_HermitSkinny_Red",
			"ZmbM_FarmerFat_Base",
			"ZmbM_FarmerFat_Beige",
			"ZmbM_FarmerFat_Blue",
			"ZmbM_FarmerFat_Brown",
			"ZmbM_FarmerFat_Green",
			"ZmbF_CitizenANormal_Base",
			"ZmbF_CitizenANormal_Beige",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Blue",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Grey",
			"ZmbF_CitizenANormal_Red",
			"ZmbM_CitizenBFat_Blue",
			"ZmbM_CitizenBFat_Red",
			"ZmbM_CitizenBFat_Green",
			"ZmbF_CitizenBSkinny",
			"ZmbM_FishermanOld_Blue",
			"ZmbM_FishermanOld_Green",
			"ZmbM_FishermanOld_Grey",
			"ZmbM_FishermanOld_Red",
			"ZmbM_JournalistSkinny",
			"ZmbF_JournalistNormal_Blue",
			"ZmbF_JournalistNormal_Green",
			"ZmbF_JournalistNormal_Red",
			"ZmbF_JournalistNormal_White",
			"ZmbM_HiderSkinny_Blue",
			"ZmbM_HiderSkinny_Green",
			"ZmbM_HiderSkinny_Yellow",
			"ZmbF_HiderSkinny_Blue",
			"ZmbF_HiderSkinny_Grey",
			"ZmbF_HiderSkinny_Green",
			"ZmbF_HiderSkinny_Red",
			"ZmbF_SurvivorNormal_Blue",
			"ZmbF_SurvivorNormal_Orange",
			"ZmbF_SurvivorNormal_Red",
			"ZmbF_SurvivorNormal_White",
			"ZmbM_CommercialPilotOld_Blue",
			"ZmbM_CommercialPilotOld_Olive",
			"ZmbM_CommercialPilotOld_Brown",
			"ZmbM_CommercialPilotOld_Grey",
			"ZmbM_JoggerSkinny_Blue",
			"ZmbM_JoggerSkinny_Green",
			"ZmbM_JoggerSkinny_Red",
			"ZmbF_JoggerSkinny_Blue",
			"ZmbF_JoggerSkinny_Brown",
			"ZmbF_JoggerSkinny_Green",
			"ZmbF_JoggerSkinny_Red",
			"ZmbM_MotorBikerFat_Beige",
			"ZmbM_MotorBikerFat_Black",
			"ZmbM_MotorBikerFat_Blue",
			"ZmbM_VillagerOld_Blue",
			"ZmbM_VillagerOld_Green",
			"ZmbM_VillagerOld_White",
			"ZmbM_SkaterYoung_Blue",
			"ZmbM_SkaterYoung_Brown",
			"ZmbM_SkaterYoung_Green",
			"ZmbM_SkaterYoung_Grey",
			"ZmbF_SkaterYoung_Brown",
			"ZmbF_SkaterYoung_Striped",
			"ZmbF_SkaterYoung_Violet",
			"ZmbM_OffshoreWorker_Green",
			"ZmbM_OffshoreWorker_Orange",
			"ZmbM_OffshoreWorker_Red",
			"ZmbM_OffshoreWorker_Yellow",
			"ZmbM_Jacket_beige",
			"ZmbM_Jacket_black",
			"ZmbM_Jacket_blue",
			"ZmbM_Jacket_bluechecks",
			"ZmbM_Jacket_brown",
			"ZmbM_Jacket_green",
			"ZmbM_Jacket_khaki",
			"ZmbM_Jacket_magenta",
			"ZmbM_Jacket_stripes",
			"ZmbF_ShortSkirt_beige",
			"ZmbF_ShortSkirt_black",
			"ZmbF_ShortSkirt_brown",
			"ZmbF_ShortSkirt_green",
			"ZmbF_ShortSkirt_grey",
			"ZmbF_ShortSkirt_checks",
			"ZmbF_ShortSkirt_red",
			"ZmbF_ShortSkirt_stripes",
			"ZmbF_ShortSkirt_white",
			"ZmbF_ShortSkirt_yellow",
			"ZmbF_VillagerOld_Blue",
			"ZmbF_VillagerOld_Green",
			"ZmbF_VillagerOld_Red",
			"ZmbF_VillagerOld_White",
			"ZmbF_MilkMaidOld_Beige",
			"ZmbF_MilkMaidOld_Black",
			"ZmbF_MilkMaidOld_Green",
			"ZmbF_MilkMaidOld_Grey",
			"ZmbF_DoctorSkinny",
			"ZmbF_NurseFat",
			"ZmbM_DoctorFat",
			"ZmbF_PatientOld",
			"ZmbM_PatientSkinny",
		};

		ItemCount = 25;
		InfectedCount = 15;
	}
		
	// ------------------------------------------------------------
	// Expansion DefaultBasebuildingLoot
	// ------------------------------------------------------------
	private void DefaultBasebuildingLoot()
	{
		Container = "ExpansionAirdropContainer_Basebuilding";

		Loot = {
			new ExpansionAirdropLootAttachments( "NailBox", NULL , 0.8),
			new ExpansionAirdropLootAttachments( "DuctTape", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "MetalPlate", NULL , 0.25),
			new ExpansionAirdropLootAttachments( "WoodenPlank", NULL , 0.5),
			new ExpansionAirdropLootAttachments( "Hammer", NULL , 0.4),
			new ExpansionAirdropLootAttachments( "Shovel", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Pliers", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "WoodAxe", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Crowbar", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "Hacksaw", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "Handsaw", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "SledgeHammer", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Hatchet", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionCodeLock", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "BarbedWire", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "MetalWire", NULL , 0.2),

			new ExpansionAirdropLootAttachments( "ExpansionHescoKit", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "ExpansionBarbedWireKit", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "ExpansionCamoBoxKit", NULL , 0.18),
			new ExpansionAirdropLootAttachments( "ExpansionGunrack", NULL , 0.05),

			new ExpansionAirdropLootAttachments( "ExpansionCone", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignDanger", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignDanger2", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignDanger3", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignRoadBarrier", NULL , 0.15),

			new ExpansionAirdropLootAttachments( "ExpansionSafeMini", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSafeMedium", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "ExpansionSafeLarge", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "ExpansionCamoTentKit", NULL , 0.25),
			new ExpansionAirdropLootAttachments( "MediumTent", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "LargeTent", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "CarTent", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "CamoNet", NULL , 0.3),
		};

		Infected = {
			"ZmbM_HermitSkinny_Base",
			"ZmbM_HermitSkinny_Beige",
			"ZmbM_HermitSkinny_Black",
			"ZmbM_HermitSkinny_Green",
			"ZmbM_HermitSkinny_Red",
			"ZmbM_FarmerFat_Base",
			"ZmbM_FarmerFat_Beige",
			"ZmbM_FarmerFat_Blue",
			"ZmbM_FarmerFat_Brown",
			"ZmbM_FarmerFat_Green",
			"ZmbF_CitizenANormal_Base",
			"ZmbF_CitizenANormal_Beige",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Blue",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Grey",
			"ZmbF_CitizenANormal_Red",
			"ZmbM_CitizenBFat_Blue",
			"ZmbM_CitizenBFat_Red",
			"ZmbM_CitizenBFat_Green",
			"ZmbF_CitizenBSkinny",
			"ZmbM_FishermanOld_Blue",
			"ZmbM_FishermanOld_Green",
			"ZmbM_FishermanOld_Grey",
			"ZmbM_FishermanOld_Red",
			"ZmbM_JournalistSkinny",
			"ZmbF_JournalistNormal_Blue",
			"ZmbF_JournalistNormal_Green",
			"ZmbF_JournalistNormal_Red",
			"ZmbF_JournalistNormal_White",
			"ZmbM_HiderSkinny_Blue",
			"ZmbM_HiderSkinny_Green",
			"ZmbM_HiderSkinny_Yellow",
			"ZmbF_HiderSkinny_Blue",
			"ZmbF_HiderSkinny_Grey",
			"ZmbF_HiderSkinny_Green",
			"ZmbF_HiderSkinny_Red",
			"ZmbF_SurvivorNormal_Blue",
			"ZmbF_SurvivorNormal_Orange",
			"ZmbF_SurvivorNormal_Red",
			"ZmbF_SurvivorNormal_White",
			"ZmbM_CommercialPilotOld_Blue",
			"ZmbM_CommercialPilotOld_Olive",
			"ZmbM_CommercialPilotOld_Brown",
			"ZmbM_CommercialPilotOld_Grey",
			"ZmbM_JoggerSkinny_Blue",
			"ZmbM_JoggerSkinny_Green",
			"ZmbM_JoggerSkinny_Red",
			"ZmbF_JoggerSkinny_Blue",
			"ZmbF_JoggerSkinny_Brown",
			"ZmbF_JoggerSkinny_Green",
			"ZmbF_JoggerSkinny_Red",
			"ZmbM_MotorBikerFat_Beige",
			"ZmbM_MotorBikerFat_Black",
			"ZmbM_MotorBikerFat_Blue",
			"ZmbM_VillagerOld_Blue",
			"ZmbM_VillagerOld_Green",
			"ZmbM_VillagerOld_White",
			"ZmbM_SkaterYoung_Blue",
			"ZmbM_SkaterYoung_Brown",
			"ZmbM_SkaterYoung_Green",
			"ZmbM_SkaterYoung_Grey",
			"ZmbF_SkaterYoung_Brown",
			"ZmbF_SkaterYoung_Striped",
			"ZmbF_SkaterYoung_Violet",
			"ZmbM_OffshoreWorker_Green",
			"ZmbM_OffshoreWorker_Orange",
			"ZmbM_OffshoreWorker_Red",
			"ZmbM_OffshoreWorker_Yellow",
			"ZmbM_Jacket_beige",
			"ZmbM_Jacket_black",
			"ZmbM_Jacket_blue",
			"ZmbM_Jacket_bluechecks",
			"ZmbM_Jacket_brown",
			"ZmbM_Jacket_green",
			"ZmbM_Jacket_khaki",
			"ZmbM_Jacket_magenta",
			"ZmbM_Jacket_stripes",
			"ZmbF_ShortSkirt_beige",
			"ZmbF_ShortSkirt_black",
			"ZmbF_ShortSkirt_brown",
			"ZmbF_ShortSkirt_green",
			"ZmbF_ShortSkirt_grey",
			"ZmbF_ShortSkirt_checks",
			"ZmbF_ShortSkirt_red",
			"ZmbF_ShortSkirt_stripes",
			"ZmbF_ShortSkirt_white",
			"ZmbF_ShortSkirt_yellow",
			"ZmbF_VillagerOld_Blue",
			"ZmbF_VillagerOld_Green",
			"ZmbF_VillagerOld_Red",
			"ZmbF_VillagerOld_White",
			"ZmbF_MilkMaidOld_Beige",
			"ZmbF_MilkMaidOld_Black",
			"ZmbF_MilkMaidOld_Green",
			"ZmbF_MilkMaidOld_Grey",
			"ZmbF_BlueCollarFat_Blue",
			"ZmbF_BlueCollarFat_Green",
			"ZmbF_BlueCollarFat_Red",
			"ZmbF_MechanicNormal_Beige",
			"ZmbF_MechanicNormal_Green",
			"ZmbF_MechanicNormal_Grey",
			"ZmbF_MechanicNormal_Orange",
			"ZmbM_MechanicSkinny_Blue",
			"ZmbM_MechanicSkinny_Grey",
			"ZmbM_MechanicSkinny_Green",
			"ZmbM_MechanicSkinny_Red",
			"ZmbM_ConstrWorkerNormal_Beige",
			"ZmbM_ConstrWorkerNormal_Black",
			"ZmbM_ConstrWorkerNormal_Green",
			"ZmbM_ConstrWorkerNormal_Grey",
			"ZmbM_HeavyIndustryWorker",
		};

		ItemCount = 50;
		InfectedCount = 10;
	}
		
	// ------------------------------------------------------------
	// Expansion DefaultMilitaryLoot
	// ------------------------------------------------------------
	private void DefaultMilitaryLoot()
	{
		TStringArray ak101_1 = { "AK_PlasticBttstck", "AK_PlasticHndgrd" };

		TStringArray ak74_1 = { "AK74_WoodBttstck", "AK74_Hndgrd" };
		TStringArray ak74_2 = { "AK_FoldingBttstck", "AK74_Hndgrd" };
		TStringArray ak74_3 = { "AK_FoldingBttstck", "AK74_Hndgrd", "AK_Bayonet" };
		TStringArray ak74_4 = { "KobraOptic", "AK_WoodHndgrd", "AK74_WoodBttstck" };

		TStringArray ak74u_1 = { "AK74_WoodBttstck"};
		TStringArray ak74u_2 = { "AKS74U_Bttstck" };

		TStringArray akm_1 = { "AK_FoldingBttstck", "AK_RailHndgrd" };
		TStringArray akm_2 = { "AK_WoodBttstck", "AK_WoodHndgrd", "KobraOptic" };
		TStringArray akm_3 = { "AK_PlasticBttstck", "AK_RailHndgrd"};
		TStringArray akm_4 = { "AK74_WoodBttstck", "AK74_Hndgrd"};
		TStringArray akm_5 = { "AK_PlasticBttstck", "AK_RailHndgrd", "UniversalLight"};

		TStringArray m16_1 = { "ACOGOptic" };
		TStringArray m16_2 = { "UniversalLight" };

		TStringArray m4a1_1 = { "M4_CarryHandleOptic", "M4_OEBttstck", "M4_PlasticHndgrd" };
		TStringArray m4a1_2 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd" };
		TStringArray m4a1_3 = { "ACOGOptic", "M4_MPBttstck", "M4_MPHndgrd" };
		TStringArray m4a1_4 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd", "UniversalLight"};

		TStringArray fal_1 = { "Fal_OeBttstck" };
		TStringArray fal_2 = { "Fal_FoldingBttstck" };

		TStringArray svd_1 = { "PSO1Optic"};

		TStringArray saiga_1 = { "Saiga_Bttstck", "KobraOptic" };
		TStringArray saiga_2 = { "Saiga_Bttstck" };

		TStringArray colt1911_1 = { "TLRLight" };

		TStringArray glock19_1 = { "TLRLight" };

		TStringArray benelim4 = { "UniversalLight" };

		TStringArray winchester70_1 = { "HuntingOptic" };

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };

		Container = "ExpansionAirdropContainer_Military";

		Loot = {
			new ExpansionAirdropLootAttachments( "AK101", ak101_1 , 0.12),

			new ExpansionAirdropLootAttachments( "AK74", ak74_1 , 0.135),
			new ExpansionAirdropLootAttachments( "AK74", ak74_2 , 0.115),
			new ExpansionAirdropLootAttachments( "AK74", ak74_3 , 0.125),
			new ExpansionAirdropLootAttachments( "AK74", ak74_4 , 0.12),

			new ExpansionAirdropLootAttachments( "AKS74U", ak74u_1 , 0.25),
			new ExpansionAirdropLootAttachments( "AKS74U", ak74u_2 , 0.3),
			
			new ExpansionAirdropLootAttachments( "AKM", akm_1 , 0.25),
			new ExpansionAirdropLootAttachments( "AKM", akm_2 , 0.26),
			new ExpansionAirdropLootAttachments( "AKM", akm_3 , 0.26),
			new ExpansionAirdropLootAttachments( "AKM", akm_4 , 0.27),
			new ExpansionAirdropLootAttachments( "AKM", akm_5 , 0.26),

			new ExpansionAirdropLootAttachments( "Expansion_M16", m16_1 , 0.14),
			new ExpansionAirdropLootAttachments( "Expansion_M16", m16_2 , 0.13),
			new ExpansionAirdropLootAttachments( "Expansion_M16", NULL , 0.18),

			new ExpansionAirdropLootAttachments( "M4A1", m4a1_1 , 0.14),
			new ExpansionAirdropLootAttachments( "M4A1", m4a1_2 , 0.13),
			new ExpansionAirdropLootAttachments( "M4A1", m4a1_3 , 0.14),
			new ExpansionAirdropLootAttachments( "M4A1", m4a1_4 , 0.18),

			new ExpansionAirdropLootAttachments( "FAL", fal_1 , 0.12),
			new ExpansionAirdropLootAttachments( "FAL", fal_2 , 0.12),

			new ExpansionAirdropLootAttachments( "SVD", svd_1 , 0.1),
			new ExpansionAirdropLootAttachments( "SVD", NULL , 0.18),

			new ExpansionAirdropLootAttachments( "Saiga", saiga_1 , 0.10),
			new ExpansionAirdropLootAttachments( "Saiga", saiga_2 , 0.12),

			new ExpansionAirdropLootAttachments( "Expansion_M79", NULL , 0.1 ),

			new ExpansionAirdropLootAttachments( "ExpansionRPG7", NULL , 0.1 ),
			new ExpansionAirdropLootAttachments( "ExpansionLAW", NULL , 0.1 ),

			new ExpansionAirdropLootAttachments( "AK_RailHndgrd", NULL , 0.1 ),
			new ExpansionAirdropLootAttachments( "AK_Bayonet", NULL , 0.1 ),
			new ExpansionAirdropLootAttachments( "KobraOptic", NULL , 0.08 ),
			new ExpansionAirdropLootAttachments( "UniversalLight", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "KobraOptic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "UniversalLight", battery , 0.08),
			new ExpansionAirdropLootAttachments( "ACOGOptic", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "M4_RisHndgrd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M9A1_Bayonet", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mosin_Bayonet", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PUScopeOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "TLRLight", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "TLRLight", battery , 0.08),
			new ExpansionAirdropLootAttachments( "SKS_Bayonet", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "M68Optic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M4_T3NRDSOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "FNP45_MRDSOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "ExpansionReflexMRSOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PSO1Optic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PSO11Optic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M68Optic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "M4_T3NRDSOptic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "ExpansionReflexMRSOptic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "PSO1Optic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "PSO11Optic", battery , 0.08),

			new ExpansionAirdropLootAttachments( "RGD5Grenade", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "M67Grenade", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Red", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Green", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Yellow", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Purple", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_White", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "RDG2SmokeGrenade_Black", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "RDG2SmokeGrenade_White", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "LandMineTrap", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "Mag_AK101_30Rnd", NULL , 0.10),
			new ExpansionAirdropLootAttachments( "Mag_AK74_30Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_AKM_30Rnd", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "Mag_AKM_Drum75Rnd", NULL , 0.06),
			new ExpansionAirdropLootAttachments( "Mag_AKM_Palm30Rnd", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Mag_CMAG_20Rnd", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "Mag_CMAG_30Rnd", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Mag_CMAG_40Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_FAL_20Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_STANAGCoupled_30Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Mag_STANAG_30Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_SVD_10Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Mag_Saiga_5Rnd", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Mag_Saiga_8Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_Saiga_Drum20Rnd", NULL , 0.08),

			new ExpansionAirdropLootAttachments( "AmmoBox_762x54Tracer_20Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x54_20Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39_20Rnd", NULL , 0.09),
			new ExpansionAirdropLootAttachments( "AmmoBox_556x45Tracer_20Rnd", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "AmmoBox_556x45_20Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "AmmoBox_545x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionAirdropLootAttachments( "AmmoBox_545x39_20Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_762x54Tracer", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_762x54", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Ammo_762x39Tracer", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Ammo_762x39", NULL , 0.12),
			new ExpansionAirdropLootAttachments( "Ammo_556x45Tracer", NULL , 0.07),
			new ExpansionAirdropLootAttachments( "Ammo_556x45", NULL , 0.11),
			new ExpansionAirdropLootAttachments( "Ammo_545x39Tracer", NULL , 0.07),
			new ExpansionAirdropLootAttachments( "Ammo_545x39", NULL , 0.10),

			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Red", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Green", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Yellow", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Purple", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Teargas", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_White", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Red", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Green", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Yellow", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Purple", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Teargas", NULL , 0.02),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_HE", NULL , 0.7),

			new ExpansionAirdropLootAttachments( "ExpansionAmmoRPG", NULL , 0.05),

			new ExpansionAirdropLootAttachments( "UKAssVest_Black", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "UKAssVest_Camo", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "UKAssVest_Khaki", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "UKAssVest_Olive", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "HighCapacityVest_Black", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "HighCapacityVest_Olive", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "PlateCarrierVest", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "PlateCarrierHolster", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PlateCarrierPouches", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PlateCarrierVest", vest , 0.05),

			new ExpansionAirdropLootAttachments( "TacticalBaconCan", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "Bear_Pink", NULL , 0.1),
			
			new ExpansionAirdropLootAttachments( "GhillieAtt_Mossy", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "GhillieHood_Mossy", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "GhillieBushrag_Mossy", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "GhillieSuit_Mossy", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "ExpansionSatchel", NULL , 0.01),
		};

		Infected = {	
			"ZmbM_usSoldier_normal_Woodland",
			"ZmbM_SoldierNormal",
			"ZmbM_usSoldier_normal_Desert",
			"ZmbM_PatrolNormal_PaulRev",
			"ZmbM_PatrolNormal_Autumn",
			"ZmbM_PatrolNormal_Flat",
			"ZmbM_PatrolNormal_Summer",
		};

		ItemCount = 50;
		InfectedCount = 50;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMappingChernarus
	// ------------------------------------------------------------
	string ExpansionMissionAirdropChernarus(int index)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropChernarus::Defaults - Start");
		#endif

		Enabled = true;

		Weight = 1 + ( ( index + 1 ) * 5 );
		MissionMaxTime = 1200; // 20 minutes

		Speed = 25.0;
		Height = 750.0;
		
		ShowNotification = true;

		int idx = (int) Math.Floor( index / 4.0 );
		int lootIdx = index - ( idx * 4 );

		float chanceLower = 0.1;

		switch ( lootIdx )
		{
		default:
		case 0:
			MissionName = "General";
			DefaultGeneralLoot();
			break;
		case 1:
			MissionName = "Medical";
			DefaultMedicalLoot();
			break;
		case 2:
			MissionName = "Basebuilding";
			DefaultBasebuildingLoot();
			break;
		case 3:
			MissionName = "Military";
			DefaultMilitaryLoot();
			break;
		}

		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 4807, 9812, 100, "NWAF" );
			chanceLower = 0.25;
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 12159, 12583, 100, "NEAF" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 11464, 8908, 100, "Berezino" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 5043, 2505, 100, "Balota" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 2351, 5393, 100, "Zelenogorsk" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 2036, 7491, 100, "Myshkinko" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 11125, 14040, 100, "Novodmitrovsk" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 6128, 2497, 100, "Chernogorsk" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 9371, 2229, 100, "Elektrozavodsk" );
			break;
		case 9:
			DropLocation = new ExpansionAirdropLocation( 13452, 3112, 100, "Skalisty Island" );
			break;
		case 10:
			DropLocation = new ExpansionAirdropLocation( 2700, 6193, 100, "Sosnovka" );
			break;
		case 11:
			DropLocation = new ExpansionAirdropLocation( 7436, 7720, 100, "Novy Sobor" );
			break;
		case 12:
			DropLocation = new ExpansionAirdropLocation( 5823, 7764, 100, "Stary Sobor" );
			break;
		}

		for ( int i = 0; i < Loot.Count(); ++i )
		{
			Loot[i].Chance *= chanceLower;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropChernarus::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMappingChernarus
	// ------------------------------------------------------------
	string ExpansionMissionAirdropLivonia(int index)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropLivonia::Defaults - Start");
		#endif

		Enabled = true;

		Weight = 1 + ( ( index + 1 ) * 5 );
		MissionMaxTime = 1200; // 20 minutes

		Speed = 25.0;
		Height = 750.0;
		
		ShowNotification = true;

		int idx = (int) Math.Floor( index / 4.0 );
		int lootIdx = index - ( idx * 4 );

		float chanceLower = 0.1;

		switch ( lootIdx )
		{
		default:
		case 0:
			MissionName = "General";
			DefaultGeneralLoot();
			break;
		case 1:
			MissionName = "Medical";
			DefaultMedicalLoot();
			break;
		case 2:
			MissionName = "Basebuilding";
			DefaultBasebuildingLoot();
			break;
		case 3:
			MissionName = "Military";
			DefaultMilitaryLoot();
			break;
		}

		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 0" );
			chanceLower = 0.25;
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 1" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 2" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 3" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 4" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 5" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 6" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 7" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 8" );
			break;
		case 9:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 9" );
			break;
		case 10:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 10" );
			break;
		case 11:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 11" );
			break;
		case 12:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "TODO 12" );
			break;
		}

		for ( int i = 0; i < Loot.Count(); ++i )
		{
			Loot[i].Chance *= chanceLower;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropLivonia::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMappingChernarus
	// ------------------------------------------------------------
	string ExpansionMissionAirdropDeerIsle(int index)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropDeerIsle::Defaults - Start");
		#endif

		Enabled = true;

		Weight = 1 + ( ( index + 1 ) * 5 );
		MissionMaxTime = 300; // 5 minutes

		Speed = 25.0;
		Height = 750.0;
		
		ShowNotification = true;

		int idx = (int) Math.Floor( index / 4.0 );
		int lootIdx = index - ( idx * 4 );

		float chanceLower = 0.1;

		switch ( lootIdx )
		{
		default:
		case 0:
			MissionName = "General";
			DefaultGeneralLoot();
			break;
		case 1:
			MissionName = "Medical";
			DefaultMedicalLoot();
			break;
		case 2:
			MissionName = "Basebuilding";
			DefaultBasebuildingLoot();
			break;
		case 3:
			MissionName = "Military";
			DefaultMilitaryLoot();
			break;
		}

		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 6055, 13429, 100, "Portland" );
			chanceLower = 0.25;
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 7911, 8864, 100, "Mountainville" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 4627, 7507, 100, "Sandy Bay" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 1849, 8830, 100, "Old Town" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 3614, 6360, 100, "Westbrook" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 6117, 7004, 100, "Oldfield" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 6924, 5473, 100, "Warren Cove" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 4522, 4370, 100, "Georgstown" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 4539, 2317, 100, "Hazelands" );
			break;
		case 9:
			DropLocation = new ExpansionAirdropLocation( 4658, 1414, 100, "Greenville" );
			break;
		case 10:
			DropLocation = new ExpansionAirdropLocation( 6973, 1701, 100, "Stonington" );
			break;
		case 11:
			DropLocation = new ExpansionAirdropLocation( 7630, 3717, 100, "Kushville" );
			break;
		case 12:
			DropLocation = new ExpansionAirdropLocation( 10179, 4722, 100, "Oceanville" );
			break;
		}

		for ( int i = 0; i < Loot.Count(); ++i )
		{
			Loot[i].Chance *= chanceLower;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropDeerIsle::Defaults - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion SpawnLoot
	// ------------------------------------------------------------
	protected void SpawnLoot( vector centerPosition, float radius )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnLoot - Start");
		#endif

		m_LootItemsSpawned = 0;
		while ( m_LootItemsSpawned < ItemCount )
		{
			int index = Loot.GetRandomIndex( );

			float chance = Loot.Get( index ).Chance;
			if ( Math.RandomFloat01() < chance )
			{
				m_LootItemsSpawned++;

				m_Container.AddItem( Loot.Get( index ) );
			}
		}	

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnLoot - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion SpawnInfected
	// ------------------------------------------------------------
	protected void SpawnInfected( vector centerPosition, /*float innerRadius, */ float spawnRadius )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnInfected - Start");
		#endif
		
		if ( GetGame().IsServer() )
		{
			for ( int z = 0; z < InfectedCount; z++ ) 
			{
				Object obj = GetGame().CreateObject( Infected.GetRandomElement(), Vector( m_Container.GetPosition()[0] + Math.RandomFloat( -spawnRadius, spawnRadius ), 0, m_Container.GetPosition()[2] + Math.RandomFloat( -spawnRadius, spawnRadius ) ), false, true );
	
				m_Infected.Insert( obj );
			}
	
			/*
			m_AIGroup = GetGame().GetWorld().GetAIWorld().CreateGroup( "ExpansionInfectedPatrolGroupBeh" );
			array<ref BehaviourGroupInfectedPackWaypointParams> waypointParams = new array<ref BehaviourGroupInfectedPackWaypointParams>;
	
			waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( centerPosition, 20.0 ) );
			//waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 20, 5 ), 20.0 ) );
			//waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 45, 35 ), 50.0 ) );
			//waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 30, 25 ), 20.0 ) );
			//waypointParams.Insert( new BehaviourGroupInfectedPackWaypointParams( SampleSpawnPosition( centerPosition, 5, 0 ), 2.0 ) );
	
			BehaviourGroupInfectedPack bgip;
			Class.CastTo( bgip, m_AIGroup.GetBehaviour() );
			if ( bgip )
			{
				bgip.SetWaypoints( waypointParams, 0, true, false );
			}
			*/
			// SpawnInfectedRemaining( centerPosition, innerRadius, spawnRadius, Infected );
	
			// SpawnInfectedRemaining_NOPATHING( centerPosition, innerRadius, spawnRadius, Infected );
		}

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnInfected - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion SpawnInfectedRemaining_NOPATHING
	// ------------------------------------------------------------
	protected void SpawnInfectedRemaining_NOPATHING( vector centerPosition, float innerRadius, float spawnRadius, int remaining )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnInfectedRemaining_NOPATHING - Start");
		#endif
		
		if ( GetGame().IsServer() )
		{
			vector spawnPosition = SampleSpawnPosition( centerPosition, spawnRadius, innerRadius );
	
			Object obj = GetGame().CreateObject( CustomExpansionWorkingZombieClasses().GetRandomElement(), spawnPosition, false, false, true );
	
			DayZCreatureAI creature;
			Class.CastTo( creature, obj );
			if ( creature )
			{
				creature.InitAIAgent( m_AIGroup );
	
				m_Infected.Insert( obj );
			}
			
			if ( remaining > 0 )
			{
				// SpawnInfectedRemaining( centerPosition, innerRadius, spawnRadius, remaining - 1 );
				// GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( this.SpawnInfectedRemaining_NOPATHING, centerPosition, innerRadius, spawnRadius, remaining - 1 );
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnInfectedRemaining_NOPATHING - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SpawnInfectedRemaining
	// ------------------------------------------------------------
	private void SpawnInfectedRemaining( vector centerPosition, float innerRadius, float spawnRadius, int remaining )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnInfectedRemaining - Start");
		#endif
		
		if ( GetGame().IsServer() )
		{
			if ( remaining <= 0 )
				return;
	
			vector spawnPosition = SampleSpawnPosition( centerPosition, spawnRadius, innerRadius );
	
			Object obj = GetGame().CreateObject( CustomExpansionWorkingZombieClasses().GetRandomElement(), spawnPosition, false, false, true );
	
			DayZCreatureAI creature;
			Class.CastTo( creature, obj );
			if ( creature )
			{
				creature.InitAIAgent( m_AIGroup );
	
				m_Infected.Insert( obj );
			}
	
			// SpawnInfectedRemaining( centerPosition, innerRadius, spawnRadius, remaining - 1 );
			// GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( this.SpawnInfectedRemaining, centerPosition, innerRadius, spawnRadius, remaining - 1 );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnInfectedRemaining - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SampleSpawnPosition
	// ------------------------------------------------------------
	private vector SampleSpawnPosition( vector position, float maxRadius, float innerRadius )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SampleSpawnPosition - Start");
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
		EXLogPrint("ExpansionMissionEventAirdrop::SampleSpawnPosition - End and return nPosition: " + nPosition.ToString() );
		#endif
		
		return nPosition;
	}
};