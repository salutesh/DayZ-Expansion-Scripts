/**
 * ExpansionMonitorModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMonitorModule
 * @brief		This class handle expansion monitor module
 **/

class ExpansionMonitorModule: JMModuleBase
{
	private const float UPDATE_TICK_TIME = 0.1; // refreshes 100 players every two seconds
	private const int UPDATE_PLAYERS_PER_TICK = 5;

	//Server only
	private ref map< string, ref ExpansionSyncedPlayerStats > m_Stats;

	private float m_UpdateQueueTimer;
	private int m_CurrentPlayerTick;
	
	//Client only
	private ref ExpansionSyncedPlayerStats m_ClientStat;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMonitorModule
	// ------------------------------------------------------------
	void ExpansionMonitorModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::ExpansionMonitorModule - Start");
		#endif
		
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::ExpansionMonitorModule - End");
		#endif
	}

	override void OnInit()
	{
		if ( IsMissionHost() )
		{
			m_Stats = new map< string, ref ExpansionSyncedPlayerStats >();
			m_UpdateQueueTimer = 0;
		}

		m_ClientStat = new ExpansionSyncedPlayerStats();
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionMonitorRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionMonitorRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Override OnRPC
	// Called on client
	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnRPC - Start");
		#endif

		switch ( rpc_type )
		{
		case ExpansionMonitorRPC.SyncStats:
			m_ClientStat.OnRecieve( ctx );
			break;
		case ExpansionMonitorRPC.SendMessage:
			RPC_SendMessage( ctx );
			break;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnClientReady
	// ------------------------------------------------------------
	override void OnClientReady( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPartyModule::OnClientReady - Start" );
		#endif

		if ( GetExpansionSettings().GetNotification().ShowPlayerJoinServer && identity )
		{
			ref StringLocaliser title = new StringLocaliser( "STR_EXPANSION_PLAYER_JOINED_TITLE" );
			ref StringLocaliser text = new StringLocaliser( "STR_EXPANSION_PLAYER_JOINED_TEXT", identity.GetName() );
			
			if ( GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.NOTIFICATION ) 
			{
				GetNotificationSystem().CreateNotification( title, text, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 5 );
			} else if ( GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.CHAT )
			{
				ServerChatMessage( title, text );
			}
		}

		//ServerChatMessage( new StringLocaliser( "STR_EXPANSION_PLAYER_JOINED_TITLE" ), new StringLocaliser( "Expansion Version is = " + LoadingScreen.GetClientExpansionVersion() ) );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartyModule::OnClientReady - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnInvokeConnect
	// ------------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPartyModule::OnInvokeConnect - Start" );
		#endif

		Print("ExpansionMonitorModule::OnInvokeConnect player : " + player);

		if ( player )
		{
			AddPlayerStats( player, identity );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartyModule::OnInvokeConnect - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnInvokeDisconnect
	// Called on server
	// ------------------------------------------------------------
	override void OnInvokeDisconnect( PlayerBase player )
	{
		if ( GetExpansionSettings().GetNotification().ShowPlayerLeftServer && player.GetIdentity() ) 
		{
			ref StringLocaliser title = new StringLocaliser( "STR_EXPANSION_PLAYER_LEFT_TITLE" );
			ref StringLocaliser text = new StringLocaliser( "STR_EXPANSION_PLAYER_LEFT_TEXT", player.GetIdentity().GetName() );
			
			if ( GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.NOTIFICATION ) 
			{
				GetNotificationSystem().CreateNotification( title, text, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 5 );
			} else if ( GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.CHAT )
			{
				ServerChatMessage( title, text );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClientLogout
	// Called on server
	// ------------------------------------------------------------
	override void OnClientLogout( PlayerBase player, PlayerIdentity identity, int logoutTime, bool authFailed )
	{
		RemovePlayerStats( identity );
	}
	
	// ------------------------------------------------------------
	// Override OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		if ( !IsMissionHost() )
			return;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnUpdate - Start");
		#endif

		m_UpdateQueueTimer += timeslice;
		if ( m_UpdateQueueTimer >= UPDATE_TICK_TIME )
		{
			if ( m_Stats.Count() > 0 )
			{
				for ( int i = 0; i < UPDATE_PLAYERS_PER_TICK; ++i ) 
				{
					if ( m_CurrentPlayerTick >= m_Stats.Count() )
					{
						m_CurrentPlayerTick = 0;
					}
	
					ref ExpansionSyncedPlayerStats stat = m_Stats.GetElement( m_CurrentPlayerTick );
	
					PlayerBase active_player = PlayerBase.GetPlayerByUID( stat.m_PlayerUID );
					
					if ( active_player && active_player.GetIdentity() )
					{
						stat.m_Health = CalcHealth( active_player );
						stat.m_Blood = CalcBlood( active_player );
						stat.m_Water = CalcWater( active_player );
						stat.m_Energy = CalcEnergy( active_player );
						stat.m_Stamina = CalcStamina( active_player );
						
						stat.m_Distance = active_player.StatGet( "dist" );
						stat.m_Playtime = active_player.StatGet( "playtime" );
						stat.m_PlayersKilled = active_player.StatGet( "players_killed" );
						stat.m_InfectedKilled = active_player.StatGet( "infected_killed" );
						stat.m_AnimalsKilled = active_player.StatGet( "animals_killed" );
						stat.m_LongestShot = active_player.StatGet( "longest_survivor_hit" );
						
						stat.m_Weight = CalcWeight( active_player );
						
						ScriptRPC rpc = new ScriptRPC();
						stat.OnSend( rpc );
						rpc.Send( NULL, ExpansionMonitorRPC.SyncStats, false, active_player.GetIdentity() );
					}
	
					m_CurrentPlayerTick++;
				}
			} else
			{
				m_CurrentPlayerTick = 0;
			}
			
			m_UpdateQueueTimer = 0.0;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override AddPlayerStats
	// Called on server
	// ------------------------------------------------------------
	void AddPlayerStats( PlayerBase player, PlayerIdentity identity )
	{
		if ( !IsMissionHost() )
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - Start");
		#endif
		
		if ( !m_Stats.Contains( identity.GetId() ) ) //! RemovePlayerStats would never be called if AddPlayerStats was called twice (logout cancelled or reconnected in time)
			m_Stats.Insert( identity.GetId(), new ExpansionSyncedPlayerStats );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RemovePlayerStats
	// Called on server
	// ------------------------------------------------------------
	void RemovePlayerStats(PlayerIdentity identity)
	{
		if ( !IsMissionHost() )
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStats - Start");
		#endif
		
		ExpansionSyncedPlayerStats stat;
		if ( m_Stats.Find( identity.GetId(), stat ) )
			m_Stats.Remove( identity.GetId() );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ServerChatMessage
	// Called on server
	// ------------------------------------------------------------
	void ServerChatMessage( ref StringLocaliser tag, ref StringLocaliser message)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionKillFeedModule::Message - Start" );
		#endif

		ScriptRPC message_rpc = new ScriptRPC();
		message_rpc.Write( tag );
		message_rpc.Write( message );
		message_rpc.Send( null, ExpansionMonitorRPC.SendMessage, true );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionKillFeedModule::Message - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_SendMessage
	// Called on all Clients
	// ------------------------------------------------------------
	private void RPC_SendMessage( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendMessage - Start" );
		#endif
		
		if ( !IsMissionClient() )
			return;

		ref StringLocaliser tag;
		if ( !ctx.Read( tag ) )
			return;
		
		ref StringLocaliser message;
		if ( !ctx.Read( message ) )
			return;
		
		GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( ExpansionChatChannels.CCSystem, "", tag.Format() + " - " + message.Format(), "" ) );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendMessage - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CalcHealth
	// Called on server
	// ------------------------------------------------------------
	protected int CalcHealth( PlayerBase player )
	{
		int health;
		int max_health = player.GetMaxHealth("GlobalHealth", "Health");
		
		health = Math.Round( player.GetHealth( "","Health" ) );
		health = ( ( health * 100 ) / max_health );
		
		return health;
	}
	
	// ------------------------------------------------------------
	// Expansion CalcBlood
	// Called on server
	// ------------------------------------------------------------
	protected int CalcBlood( PlayerBase player )
	{
		int blood;
		int max_blood = player.GetMaxHealth("GlobalHealth", "Blood");
		
		blood = Math.Round( player.GetHealth("", "Blood") );
		blood = ( ( blood * 100 ) / max_blood );
		
		return blood;
	}
	
	// ------------------------------------------------------------
	// Expansion CalcWater
	// Called on server
	// ------------------------------------------------------------
	protected int CalcWater( PlayerBase player )
	{
		int water;
		int max_water = 5000;
		
		water = Math.Round( player.GetStatWater().Get() );
		water = ( ( water * 100 ) / max_water );
		
		return water;
	}
	
	// ------------------------------------------------------------
	// Expansion CalcEnergy
	// Called on server
	// ------------------------------------------------------------
	protected int CalcEnergy( PlayerBase player )
	{
		int energy;
		int max_energy = 20000;
		
		energy = Math.Round( player.GetStatEnergy().Get() );
		energy = ( ( energy * 100 ) / max_energy );
		
		return energy;
	}
	
	// ------------------------------------------------------------
	// Expansion CalcStamina
	// Called on server
	// ------------------------------------------------------------
	protected int CalcStamina( PlayerBase player )
	{
		int stamina;
		int max_stamina = GameConstants.STAMINA_MAX;
		
		stamina = Math.Round( player.GetStatStamina().Get() );
		stamina = ( ( stamina * 100 ) / max_stamina );
		
		return stamina;
	}
		
	// ------------------------------------------------------------
	// Expansion CalcWeight
	// Called on server
	// ------------------------------------------------------------
	protected int CalcWeight( PlayerBase player )
	{
		int weight;
		
		weight = player.GetWeight();
		
		return weight;
	}
	
	// ------------------------------------------------------------
	// Expansion GetStats
	// Called on client
	// ------------------------------------------------------------ 
	ExpansionSyncedPlayerStats GetStats()
	{
		return m_ClientStat;
	}
}