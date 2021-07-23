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
	private const float UPDATE_TICK_TIME = 1.0; // refreshes 100 players every ten seconds
	private const int UPDATE_PLAYERS_PER_TICK = 10;

	//Server only
	private ref map< string, ref ExpansionSyncedPlayerStats > m_Stats;
	private ref map< string, ref ExpansionSyncedPlayerStates > m_States;
	
	private float m_UpdateQueueTimer;
	private int m_CurrentPlayerTick;
	
	//Client only
	private ref ExpansionSyncedPlayerStats m_ClientStat;
	private ref ExpansionSyncedPlayerStates m_ClientStates;
	
	static ref ScriptInvoker m_StatsInvoker = new ScriptInvoker();
	static ref ScriptInvoker m_StatesInvoker = new ScriptInvoker();
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule Constructor
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
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		if ( IsMissionHost() )
		{
			m_Stats = new map< string, ref ExpansionSyncedPlayerStats >();
			m_States = new map< string, ref ExpansionSyncedPlayerStates >();
			
			m_UpdateQueueTimer = 0;
		}

		m_ClientStat = new ExpansionSyncedPlayerStats();
		m_ClientStates = new ExpansionSyncedPlayerStates();
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionMonitorRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionMonitorRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnRPC
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
			m_ClientStates.OnRecieve( ctx );
			break;
		case ExpansionMonitorRPC.SendMessage:
			RPC_SendMessage( ctx );
			break;
		case ExpansionMonitorRPC.RequestPlayerStats:
			RPC_RequestPlayerStats( ctx, sender );
			break;
		case ExpansionMonitorRPC.SendPlayerStats:
			RPC_SendPlayerStats( ctx );
			break;
		case ExpansionMonitorRPC.RequestPlayerStates:
			RPC_RequestPlayerStates( ctx, sender );
			break;
		case ExpansionMonitorRPC.SendPlayerStates:
			RPC_SendPlayerStates( ctx );
			break;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnClientReady
	// ------------------------------------------------------------
	override void OnClientReady( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnClientReady - Start" );
		#endif

		if ( GetExpansionSettings().GetNotification().ShowPlayerJoinServer && identity )
		{			
			if ( GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.NOTIFICATION ) 
			{
				ExpansionNotification("STR_EXPANSION_PLAYER_JOINED_TITLE", new StringLocaliser("STR_EXPANSION_PLAYER_JOINED_TEXT", identity.GetName()), EXPANSION_NOTIFICATION_ICON_PERSONA).Info();
			} else if ( GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.CHAT )
			{
				ServerChatMessage(new StringLocaliser("STR_EXPANSION_PLAYER_JOINED_TITLE"), new StringLocaliser("STR_EXPANSION_PLAYER_JOINED_TEXT", identity.GetName()));
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnClientReady - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnInvokeConnect
	// ------------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnInvokeConnect - Start" );
		EXPrint( "ExpansionMonitorModule::OnInvokeConnect player : " + player );
		#endif

		if ( player )
		{
			AddPlayerStats( player, identity );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnInvokeConnect - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnInvokeDisconnect
	// Called on server
	// ------------------------------------------------------------
	override void OnInvokeDisconnect( PlayerBase player )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnInvokeDisconnect - Start" );
		#endif

		if ( GetExpansionSettings().GetNotification().ShowPlayerLeftServer && player.GetIdentity() ) 
		{			
			if ( GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.NOTIFICATION ) 
			{
				ExpansionNotification("STR_EXPANSION_PLAYER_LEFT_TITLE", new StringLocaliser("STR_EXPANSION_PLAYER_LEFT_TEXT", player.GetIdentity().GetName()), EXPANSION_NOTIFICATION_ICON_PERSONA).Info();
			} else if ( GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.CHAT )
			{
				ServerChatMessage(new StringLocaliser("STR_EXPANSION_PLAYER_LEFT_TITLE"), new StringLocaliser("STR_EXPANSION_PLAYER_LEFT_TEXT", player.GetIdentity().GetName()));
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnInvokeDisconnect - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnClientLogout
	// Called on server
	// ------------------------------------------------------------
	override void OnClientLogout( PlayerBase player, PlayerIdentity identity, int logoutTime, bool authFailed )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnClientLogout - Start" );
		#endif

		RemovePlayerStats( identity );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnClientLogout - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnUpdate
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
					ref ExpansionSyncedPlayerStates states = m_States.GetElement( m_CurrentPlayerTick );
					
					PlayerBase active_player = PlayerBase.GetPlayerByUID( m_Stats.GetKey( m_CurrentPlayerTick ) );
					
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
						
						states.m_Bones = BonesState( active_player );
						states.m_Sick = IsBrianSick( active_player );
						states.m_Cholera = HasCholera( active_player );
						states.m_Influenza = HasInfluenza( active_player );
						states.m_Salmonella = HasSalmonella( active_player );
						states.m_Poison = HasFoodPoison( active_player );
						states.m_Infection= HasWoundInfection( active_player );
						
						ScriptRPC rpc = new ScriptRPC();
						stat.OnSend( rpc );
						states.OnSend( rpc );
						rpc.Send( NULL, ExpansionMonitorRPC.SyncStats, false, active_player.GetIdentity() );
					}
	
					m_CurrentPlayerTick++;

					if ( m_CurrentPlayerTick == m_Stats.Count() )
					{
						break;
					}
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
	// ExpansionMonitorModule AddPlayerStats
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
		
		if ( !m_States.Contains( identity.GetId() ) ) //! RemovePlayerStats would never be called if AddPlayerStats was called twice (logout cancelled or reconnected in time)
			m_States.Insert( identity.GetId(), new ExpansionSyncedPlayerStates );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RemovePlayerStats
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
		
		ExpansionSyncedPlayerStates state;
		if ( m_States.Find( identity.GetId(), state ) )
			m_States.Remove( identity.GetId() );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule ServerChatMessage
	// Called on server
	// ------------------------------------------------------------
	void ServerChatMessage( ref StringLocaliser tag, ref StringLocaliser message)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::Message - Start" );
		#endif

		ScriptRPC message_rpc = new ScriptRPC();
		message_rpc.Write( tag );
		message_rpc.Write( message );
		message_rpc.Send( null, ExpansionMonitorRPC.SendMessage, true );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::Message - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_SendMessage
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
	// ExpansionMonitorModule CalcHealth
	// Called on server
	// ------------------------------------------------------------
	protected int CalcHealth( PlayerBase player )
	{
		return Calc( player.GetHealth( "","Health" ), player.GetMaxHealth("GlobalHealth", "Health") );
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcBlood
	// Called on server
	// ------------------------------------------------------------
	protected int CalcBlood( PlayerBase player )
	{
		return Calc( player.GetHealth("", "Blood") - 2500,  2500);
	}

	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcWater
	// Called on server
	// ------------------------------------------------------------
	protected int CalcWater( PlayerBase player )
	{
		return CalcStat( player.GetStatWater() );
	}

	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcEnergy
	// Called on server
	// ------------------------------------------------------------
	protected int CalcEnergy( PlayerBase player )
	{
		return CalcStat( player.GetStatEnergy() );
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcStamina
	// Called on server
	// ------------------------------------------------------------
	protected int CalcStamina( PlayerBase player )
	{
		return CalcStat( player.GetStatStamina() );
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule Calc
	// Called on server
	// ------------------------------------------------------------
	protected int Calc( float value, float max )
	{
		return ( Math.Round( value ) * 100 ) / max;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcStat
	// Called on server
	// ------------------------------------------------------------
	protected int CalcStat( PlayerStat< float > stat )
	{
		return Calc( stat.Get(), stat.GetMax() );
	}
		
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcWeight
	// Called on server
	// ------------------------------------------------------------
	protected int CalcWeight( PlayerBase player )
	{
		int weight;
		
		weight = player.GetWeight();
		
		return weight;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule BonesState
	// Called on server
	// ------------------------------------------------------------
	protected int BonesState( PlayerBase player )
	{
		int bonesState;
		
		bonesState = player.m_BrokenLegState;
		
		return bonesState;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule IsBrianSick
	// Called on server
	// ------------------------------------------------------------
	protected bool IsBrianSick( PlayerBase player )
	{
		bool brainSick = false;
		
		if (player.GetSingleAgentCount(eAgents.BRAIN) >= BrainDiseaseMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasCholera
	// Called on server
	// ------------------------------------------------------------
	protected bool HasCholera( PlayerBase player )
	{
		if (player.GetSingleAgentCount(eAgents.CHOLERA) >= CholeraMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasInfluenza
	// Called on server
	// ------------------------------------------------------------
	protected bool HasInfluenza( PlayerBase player )
	{
		if (player.GetSingleAgentCount(eAgents.INFLUENZA) >= InfluenzaMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasSalmonella
	// Called on server
	// ------------------------------------------------------------
	protected bool HasSalmonella( PlayerBase player )
	{
		if (player.GetSingleAgentCount(eAgents.SALMONELLA) >= SalmonellaMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasFoodPoison
	// Called on server
	// ------------------------------------------------------------
	protected bool HasFoodPoison( PlayerBase player )
	{
		if (player.GetSingleAgentCount(eAgents.FOOD_POISON) >= PoisoningMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasWoundInfection
	// Called on server
	// ------------------------------------------------------------
	protected bool HasWoundInfection( PlayerBase player )
	{
		if (player.GetSingleAgentCount(eAgents.WOUND_AGENT) >= WoundInfectionMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetStats
	// Called on client
	// ------------------------------------------------------------ 
	ExpansionSyncedPlayerStats GetStats()
	{
		return m_ClientStat;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetStates
	// Called on client
	// ------------------------------------------------------------ 
	ExpansionSyncedPlayerStates GetStates()
	{
		return m_ClientStates;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RequestPlayerStats
	// Called from client
	// ------------------------------------------------------------
	void RequestPlayerStats(string player_id)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RequestPlayerStats - Start" );
		#endif

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( player_id );
		rpc.Send( null, ExpansionMonitorRPC.RequestPlayerStats, true );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RequestPlayerStats - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_RequestPlayerStats
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPlayerStats( ref ParamsReadContext ctx, PlayerIdentity sender )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_RequestPlayerStats - Start" );
		#endif
		
		if (IsMissionClient())
			return;

		string player_id;
		if (!ctx.Read(player_id))
			return;
		
		ref ExpansionSyncedPlayerStats playerStats = GetPlayerStatsByID(player_id);
		if (playerStats)
		{
			SendPlayerStats(playerStats, sender);
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_RequestPlayerStats - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetPlayerStatsByID
	// Called on server
	// ------------------------------------------------------------
	ref ExpansionSyncedPlayerStats GetPlayerStatsByID(string player_id)
	{
		ref ExpansionSyncedPlayerStats currentStats = NULL;
		ref ExpansionSyncedPlayerStats playerStats = NULL;
		
		if (m_Stats.Find(player_id, currentStats))
		{
			playerStats = currentStats;
		}
		
		return playerStats;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SendPlayerStats
	// Called on server
	// ------------------------------------------------------------
	void SendPlayerStats(ref ExpansionSyncedPlayerStats player_stats, PlayerIdentity ident)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::SendPlayerStats - Start" );
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( player_stats );
		rpc.Send( null, ExpansionMonitorRPC.SendPlayerStats, true, ident );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::SendPlayerStats - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SendPlayerStats
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerStats( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendPlayerStats - Start" );
		#endif
		
		if (!IsMissionClient())
			return;
		
		ref ExpansionSyncedPlayerStats player_stats;
		if (!ctx.Read(player_stats))
			return;

		m_StatsInvoker.Invoke(player_stats);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendPlayerStats - End" );
		#endif
	}
	
	//-----------------------------------------------------------------------------------------------
	// ------------------------------------------------------------
	// ExpansionMonitorModule RequestPlayerStates
	// Called from client
	// ------------------------------------------------------------
	void RequestPlayerStates(string player_id)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RequestPlayerStates - Start" );
		#endif

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( player_id );
		rpc.Send( null, ExpansionMonitorRPC.RequestPlayerStates, true );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RequestPlayerStates - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_RequestPlayerStates
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPlayerStates( ref ParamsReadContext ctx, PlayerIdentity sender )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_RequestPlayerStates - Start" );
		#endif
		
		if (IsMissionClient())
			return;

		string player_id;
		if (!ctx.Read(player_id))
			return;
		
		ref ExpansionSyncedPlayerStates playerStates = GetPlayerStatesByID(player_id);
		if (playerStates)
		{
			SendPlayerStates(playerStates, sender);
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_RequestPlayerStates - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetPlayerStatesByID
	// Called on server
	// ------------------------------------------------------------
	ref ExpansionSyncedPlayerStates GetPlayerStatesByID(string player_id)
	{
		ref ExpansionSyncedPlayerStates currentStates = NULL;
		ref ExpansionSyncedPlayerStates playerStates = NULL;
		
		if (m_States.Find(player_id, currentStates))
		{
			playerStates = currentStates;
		}
		
		return playerStates;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SendPlayerStates
	// Called on server
	// ------------------------------------------------------------
	void SendPlayerStates(ref ExpansionSyncedPlayerStates player_states, PlayerIdentity ident)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::SendPlayerStates - Start" );
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( player_states );
		rpc.Send( null, ExpansionMonitorRPC.SendPlayerStates, true, ident );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::SendPlayerStates - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_SendPlayerStates
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerStates( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendPlayerStates - Start" );
		#endif
		
		if (!IsMissionClient())
			return;
		
		ref ExpansionSyncedPlayerStates player_states;
		if (!ctx.Read(player_states))
			return;

		m_StatesInvoker.Invoke(player_states);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendPlayerStates - End" );
		#endif
	}	
}