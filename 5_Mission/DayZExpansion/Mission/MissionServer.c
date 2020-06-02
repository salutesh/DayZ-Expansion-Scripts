/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionServer
 * @brief		
 **/
modded class MissionServer
{
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void MissionServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - Start");
		#endif

		//! CreateExpansionLogger();
		CreateDayZExpansion();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~MissionServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - Start");
		#endif
		
		DestroyDayZExpansion();
		DestroyNotificationSystem();
		//! DestroyExpansionLogger();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnMissionStart
	// ------------------------------------------------------------
	override void OnMissionStart()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionStart - Start");
		#endif
		
		super.OnMissionStart();
		
		GetDayZExpansion().OnMissionStart();	

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionStart - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionLoaded - Start");
		#endif
		
		super.OnMissionLoaded();
		
		GetDayZExpansion().OnMissionLoaded();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionFinish - Start");
		#endif
	   
		super.OnMissionFinish();

		//! Save settings on mission finish
		g_exGlobalSettings.Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionFinish - End");
		#endif
	}

	// ------------------------------------------------------------
	override void OnClientReadyEvent( PlayerIdentity identity, PlayerBase player )
	{
		super.OnClientReadyEvent( identity, player );

		if ( GetExpansionSettings().GetGeneral().ShowPlayerJoinServer ) 
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PLAYER_JOINED_TITLE" ), new StringLocaliser( "STR_EXPANSION_PLAYER_JOINED_TEXT", identity.GetName() ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 5 );
		}
	}
	
	// ------------------------------------------------------------
	override void OnClientReconnectEvent( PlayerIdentity identity, PlayerBase player )
	{
		super.OnClientReconnectEvent( identity, player );

		g_exGlobalSettings.Send( identity );
	}	
	
	// ------------------------------------------------------------
	override void InvokeOnConnect( PlayerBase player, PlayerIdentity identity )
	{
		g_exGlobalSettings.Send( identity );
		
		PlayerBase.AddPlayer( player );
		
		//! Do after, because some modules use PlayerIdentity setup inside AddPlayer of PlayerBase class
		super.InvokeOnConnect( player, identity );
	}

	override void InvokeOnDisconnect( PlayerBase player )
	{
		//! TODO: Move into a module
		if ( GetExpansionSettings().GetGeneral().ShowPlayerLeftServer && player.GetIdentity() ) 
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PLAYER_LEFT_TITLE" ), new StringLocaliser( "STR_EXPANSION_PLAYER_LEFT_TEXT", player.GetIdentity().GetName() ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 5 );
		}

		super.InvokeOnDisconnect( player );
	}

	// ------------------------------------------------------------
	override void PlayerDisconnected( PlayerBase player, PlayerIdentity identity, string uid )
	{
		PlayerBase.RemovePlayer( uid );
		
		super.PlayerDisconnected( player, identity, uid );
	}

	// ------------------------------------------------------------
	// Override EquipCharacter
	// ------------------------------------------------------------
	override void EquipCharacter()
	{
		EntityAI item;
		EntityAI item2;
		EntityAI item3;
		
		//! Creates clothes from DayZIntroScene's m_demoUnit
		if ( m_top != -1 && m_bottom != -1 && m_shoes != -1 && m_skin != -1 )
		{
			item = m_player.GetInventory().CreateInInventory( topsArray.Get( m_top ) );
			item2 = m_player.GetInventory().CreateInInventory( pantsArray.Get( m_bottom ) );
			item3 = m_player.GetInventory().CreateInInventory( shoesArray.Get( m_shoes ) );
			
			StartingEquipSetup(m_player, true);
		}
		//! Creates random starting clothes - fallback
		else
		{
			item = m_player.GetInventory().CreateInInventory( topsArray.GetRandomElement() );
			item2 = m_player.GetInventory().CreateInInventory( pantsArray.GetRandomElement() );
			item3 = m_player.GetInventory().CreateInInventory( shoesArray.GetRandomElement() );
			
			StartingEquipSetup(m_player, false);
		}
	}

	void SetStartingGear(PlayerBase player)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::SetStartingGear - Start");
		#endif

		int i;
		
		if ( GetExpansionSettings() && GetExpansionSettings().GetSpawn() )
		{
			ref ExpansionStartingGear gear = GetExpansionSettings().GetSpawn().StartingGear;
			
			if ( gear.UsingUpperGear && gear.UpperGear )
			{
				EntityAI itemTop = player.FindAttachmentBySlotName("Body");
				
				string classNameTop = itemTop.ClassName();
				if ( classNameTop != "" )
				{
					GetGame().ObjectDelete( itemTop );
					itemTop = player.GetInventory().CreateInInventory( classNameTop );
				}
				
				if ( itemTop )
				{
					for ( i = 0; i < gear.UpperGear.Count(); i++ )
					{
						itemTop.GetInventory().CreateInInventory( gear.UpperGear[i] );
					}
				}
			}
			
			if ( gear.UsingPantsGear && gear.PantsGear )
			{
				EntityAI itemPants = player.FindAttachmentBySlotName("Pants");
				
				string classNamePants = itemPants.ClassName();
				if ( classNamePants != "" )
				{
					GetGame().ObjectDelete( itemTop );
					itemPants = player.GetInventory().CreateInInventory( classNamePants );
				}
				
				if ( itemPants )
				{					
					for ( i = 0; i < gear.PantsGear.Count(); i++ )
					{
						itemPants.GetInventory().CreateInInventory( gear.PantsGear[i] );
					}
				}
			}
			
			if ( gear.UsingBackpackGear && gear.BackpackGear )
			{
				EntityAI itemBag = player.GetInventory().CreateInInventory( gear.SpawnBackpacks.GetRandomElement() );
				if ( itemBag )
				{					
					for ( i = 0; i < gear.BackpackGear.Count(); i++ )
					{
						itemBag.GetInventory().CreateInInventory( gear.BackpackGear[i] );
					}
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::SetStartingGear - End");
		#endif
	}
}
