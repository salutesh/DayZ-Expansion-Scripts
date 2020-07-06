/**
 * ExpansionSupply.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSupplySignal extends M18SmokeGrenade_Purple
{
	ExpansionAirdropLootContainers m_Settings;

	ExpansionAirdropContainerBase m_Container;

	ExpansionAirdropPlane m_Plane;

	int m_LootItemsSpawned;

	bool m_LootHasSpawned;

	void ExpansionSupplySignal()
	{
		SetEventMask( EntityEvent.SIMULATE );
	}

	void ~ExpansionSupplySignal()
	{
		if ( IsMissionHost() )
		{
			GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( OnUpdate );
		}
	}
	
	override void OnWorkStart()
	{
		if ( IsMissionHost() )
		{
			vector spawnPoint = Vector( 0, GetExpansionSettings().GetAirdrop().Height, 0 );
		
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

			m_Settings = GetExpansionSettings().GetAirdrop().Containers.GetRandomElement();

			m_Plane = ExpansionAirdropPlane.Cast( GetGame().CreateObject( "ExpansionAirdropPlane", spawnPoint, false, true, true ) );

			if ( m_Plane )
			{
				if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowPlayerAirdropStarted )
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop" ), new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_HEADING_TOWARDS_PLAYER" ), "set:expansion_notification_iconset image:icon_airdrop", COLOR_EXPANSION_NOTIFICATION_MISSION, 7 );
				}
					
				m_Plane.SetupPlane( Vector( GetPosition()[0], 0, GetPosition()[2] ), "", 100, GetExpansionSettings().GetAirdrop().Speed );
				m_Plane.m_Height = 750;

				GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( OnUpdate ); 
			}
		}

		super.OnWorkStart();
	}

	void OnUpdate( float delta )
	{
		//Message( GetPlayer(), "m_Plane " + m_Plane.GetPosition() );
		//Message( GetPlayer(), "m_Speed " + m_Plane.m_Speed );

		if ( m_Plane )
		{
			if ( m_Plane.CheckForDrop() && !m_Container )
			{
				m_LootHasSpawned = false;
 
				m_Container = m_Plane.CreateDrop( GetExpansionSettings().GetAirdrop().Containers.GetRandomElement().Container );
				m_Container.InitAirdrop( );

				if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowPlayerAirdropDropped )
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop" ), new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_SUPPLIES_DROPPED_PLAYER" ), "set:expansion_notification_iconset image:icon_airdrop", COLOR_EXPANSION_NOTIFICATION_MISSION, 7 );
				}
			}

			if ( m_Plane.IsWarningProximity() )
			{
				if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowPlayerAirdropClosingOn )
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop" ), new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_CLOSING_ON_PLAYER" ), "set:expansion_notification_iconset image:icon_airdrop", COLOR_EXPANSION_NOTIFICATION_MISSION, 7 );
				}
			}

			if ( m_Plane.CheckForRemove() )
			{
				RemovePlane();
			}
		}

		if ( m_Container )
		{
			if ( m_Container.m_ItemCount == 0 && !m_LootHasSpawned )
			{
				SpawnLoot( m_Container.GetPosition(), 30 );

				m_LootHasSpawned = true;
			}
		}
	}

	void SpawnLoot( vector centerPosition, float radius )
	{
		m_LootItemsSpawned = 0;
		while ( m_LootItemsSpawned < GetExpansionSettings().GetAirdrop().ItemCount )
		{
			int index = m_Settings.Loot.GetRandomIndex( );

			float chance = m_Settings.Loot.Get( index ).Chance;
			if ( Math.RandomFloat01() < chance )
			{
				m_LootItemsSpawned++;

				m_Container.AddItem( m_Settings.Loot.Get( index ) );
			}
		}	
	}

	void RemovePlane()
	{
		if ( !m_Plane )
			return;

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, 0, false, m_Plane );
	}
}