/**
 * ExpansionMonitorModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	private ref map<string, ref ExpansionSyncedPlayerStats> m_Stats;
	private ref map<string, ref ExpansionSyncedPlayerStates> m_States;
	
	private float m_UpdateQueueTimer;
	private int m_CurrentPlayerTick;
	
	//Client only
	static ref ExpansionSyncedPlayerStats m_ClientStats;
	static ref ExpansionSyncedPlayerStates m_ClientStates;
	
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
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			m_Stats = new map<string, ref ExpansionSyncedPlayerStats>;
			m_States = new map<string, ref ExpansionSyncedPlayerStates>;
			m_UpdateQueueTimer = 0;
		}

		m_ClientStats = new ExpansionSyncedPlayerStats;
		m_ClientStates = new ExpansionSyncedPlayerStates;
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
	#ifdef CF_BUGFIX_REF
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	#else
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	#endif
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnRPC - Start");
		#endif

		switch ( rpc_type )
		{
		case ExpansionMonitorRPC.SyncStats:
			m_ClientStats.OnRecieve(ctx);
			break;
		case ExpansionMonitorRPC.SyncStates:
			m_ClientStates.OnRecieve(ctx);
			break;
		case ExpansionMonitorRPC.SendMessage:
			RPC_SendMessage(ctx);
			break;
		case ExpansionMonitorRPC.RequestPlayerStats:
			RPC_RequestPlayerStats(ctx, sender);
			break;
		case ExpansionMonitorRPC.SendPlayerStats:
			RPC_SendPlayerStats(ctx);
			break;
		case ExpansionMonitorRPC.RequestPlayerStates:
			RPC_RequestPlayerStates(ctx, sender);
			break;
		case ExpansionMonitorRPC.SendPlayerStates:
			RPC_SendPlayerStates(ctx);
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

		if (GetExpansionSettings().GetNotification().ShowPlayerJoinServer && identity)
		{			
			if (GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.NOTIFICATION) 
			{
				ExpansionNotification("STR_EXPANSION_PLAYER_JOINED_TITLE", new StringLocaliser("STR_EXPANSION_PLAYER_JOINED_TEXT", identity.GetName()), EXPANSION_NOTIFICATION_ICON_PERSONA).Info();
			} 
			else if (GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.CHAT)
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
	override void OnInvokeConnect(PlayerBase player, PlayerIdentity identity)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnInvokeConnect - Start");
		#endif

		if (!player)
			return;
		
		if (!identity)
			return;
		
		string playerID = identity.GetId();
		AddPlayerStats(player, playerID);
		AddPlayerStates(player, playerID);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnInvokeConnect - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnInvokeDisconnect
	// Called on server
	// ------------------------------------------------------------
	override void OnInvokeDisconnect(PlayerBase player)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnInvokeDisconnect - Start" );
		#endif

		if (GetExpansionSettings().GetNotification().ShowPlayerLeftServer && player.GetIdentity()) 
		{			
			if (GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.NOTIFICATION) 
			{
				ExpansionNotification("STR_EXPANSION_PLAYER_LEFT_TITLE", new StringLocaliser("STR_EXPANSION_PLAYER_LEFT_TEXT", player.GetIdentity().GetName()), EXPANSION_NOTIFICATION_ICON_PERSONA).Info();
			} 
			else if (GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.CHAT)
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
	override void OnClientLogout(PlayerBase player, PlayerIdentity identity, int logoutTime, bool authFailed)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnClientLogout - Start" );
		#endif

		if (!identity)
			return;
		
		string playerID = identity.GetId();
		RemovePlayerStats(playerID);
		RemovePlayerStates(playerID);

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionMonitorModule::OnClientLogout - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate( timeslice );
		
		if (IsMissionClient())
			return;
		
		m_UpdateQueueTimer += timeslice;
		if ( m_UpdateQueueTimer >= UPDATE_TICK_TIME )
		{
			if (m_Stats.Count() > 0)
			{
				for (int i = 0; i < UPDATE_PLAYERS_PER_TICK; ++i) 
				{
					if (m_CurrentPlayerTick >= m_Stats.Count())
					{
						m_CurrentPlayerTick = 0;
					}
					
					string currentPlayerID = m_Stats.GetKey(m_CurrentPlayerTick);
					if (currentPlayerID == "")
					{
						Error("ExpansionMonitorModule::OnUpdate - Player ID is empty!");
						continue;
					}
					
					UpdateStats(currentPlayerID);
					UpdateStates(currentPlayerID);
					
					m_CurrentPlayerTick++;

					if (m_CurrentPlayerTick == m_Stats.Count())
					{
						break;
					}
				}
			}
			else
			{
				m_CurrentPlayerTick = 0;
			}
			
			m_UpdateQueueTimer = 0.0;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule UpdateStats
	// ------------------------------------------------------------	
	private void UpdateStats(string playerID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		ExpansionSyncedPlayerStats stats = m_Stats.Get(playerID);
		if (!stats)
		{
			Error("ExpansionMonitorModule::UpdateStats - Could not get player stats!");
			return;
		}
		
		PlayerBase player = PlayerBase.GetPlayerByUID(playerID);
		if (!player)
		{
			//! Don't error here, player will be NULL if dead
			return;
		}
			
		stats.m_Health = CalcHealth(player);
		stats.m_Blood = CalcBlood(player);
		stats.m_Water = CalcWater(player);
		stats.m_Energy = CalcEnergy(player);
		stats.m_Stamina = CalcStamina(player);
		stats.m_Distance = player.StatGet("dist");
		stats.m_Playtime = player.StatGet("playtime");
		stats.m_PlayersKilled = player.StatGet("players_killed" );
		stats.m_InfectedKilled = player.StatGet("infected_killed");
		stats.m_AnimalsKilled = player.StatGet("animals_killed");
		stats.m_LongestShot = player.StatGet("longest_survivor_hit");
		stats.m_Weight = CalcWeight( player );
				
		ScriptRPC rpc = new ScriptRPC();
		stats.OnSend(rpc);
		rpc.Send(NULL, ExpansionMonitorRPC.SyncStats, false, player.GetIdentity());
	}

	// ------------------------------------------------------------
	// ExpansionMonitorModule UpdateStates
	// ------------------------------------------------------------		
	private void UpdateStates(string playerID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		ExpansionSyncedPlayerStates states = m_States.Get(playerID);
		if (!states)
		{
			Error("ExpansionMonitorModule::UpdateStates - Could not get player stats!");
			return;
		}
		
		PlayerBase player = PlayerBase.GetPlayerByUID(playerID);
		if (!player)
		{
			//! Don't error here, player will be NULL if dead
			return;
		}
			
		states.m_Bones = BonesState(player);
		states.m_Sick = IsBrianSick(player);
		states.m_Cholera = HasCholera(player);
		states.m_Influenza = HasInfluenza(player);
		states.m_Salmonella = HasSalmonella(player);
		states.m_Poison = HasFoodPoison(player);
		states.m_Infection= HasWoundInfection(player);
		states.m_Cuts = HasCuts(player);
		states.m_Stance = GetPlayerStance(player);
		
		ScriptRPC rpc = new ScriptRPC();
		states.OnSend(rpc);
		rpc.Send(NULL, ExpansionMonitorRPC.SyncStates, false, player.GetIdentity());
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule AddPlayerStats
	// Called on server
	// ------------------------------------------------------------
	void AddPlayerStats(PlayerBase player, string playerID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - Start");
		#endif
		
		EXPrint("ExpansionMonitorModule::AddPlayerStats - Add player stats for with ID: " + playerID);
		
		if (!m_Stats.Contains(playerID)) //! RemovePlayerStats would never be called if AddPlayerStats was called twice (logout cancelled or reconnected in time)
		{
			m_Stats.Insert(playerID, new ExpansionSyncedPlayerStats);
			UpdateStats(playerID);
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule AddPlayerStates
	// Called on server
	// ------------------------------------------------------------
	void AddPlayerStates(PlayerBase player, string playerID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - Start");
		#endif
		
		EXPrint("ExpansionMonitorModule::AddPlayerStats - Add player states for player with ID: " + playerID);

		if (!m_States.Contains(playerID)) //! RemovePlayerStats would never be called if AddPlayerStats was called twice (logout cancelled or reconnected in time)
		{
			m_States.Insert(playerID, new ExpansionSyncedPlayerStates);
			UpdateStates(playerID);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RemovePlayerStats
	// Called on server
	// ------------------------------------------------------------
	void RemovePlayerStats(string playerID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStats - Start");
		#endif
		
		ExpansionSyncedPlayerStats stat;
		if (m_Stats.Find(playerID, stat))
			m_Stats.Remove(playerID);

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RemovePlayerStates
	// Called on server
	// ------------------------------------------------------------
	void RemovePlayerStates(string playerID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStates - Start");
		#endif
		
		ExpansionSyncedPlayerStates state;
		if (m_States.Find(playerID, state))
			m_States.Remove(playerID);

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStates - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule ServerChatMessage
	// Called on server
	// ------------------------------------------------------------
	void ServerChatMessage(StringLocaliser tag, StringLocaliser message)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::Message - Start");
		#endif

		ScriptRPC message_rpc = new ScriptRPC();
		message_rpc.Write(tag);
		message_rpc.Write(message);
		message_rpc.Send(null, ExpansionMonitorRPC.SendMessage, true);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::Message - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_SendMessage
	// Called on all Clients
	// ------------------------------------------------------------
	private void RPC_SendMessage(ParamsReadContext ctx)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RPC_SendMessage - Start");
		#endif
		
		if (!GetGame().IsClient())
			return;

		StringLocaliser tag;
		if (!ctx.Read(tag))
			return;
		
		StringLocaliser message;
		if (!ctx.Read(message))
			return;
		
		GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(ExpansionChatChannels.CCSystem, "", tag.Format() + " - " + message.Format(), ""));

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RPC_SendMessage - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcHealth
	// Called on server
	// ------------------------------------------------------------
	private int CalcHealth(PlayerBase player)
	{
		return Calc(player.GetHealth("","Health"), player.GetMaxHealth("GlobalHealth", "Health"));
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcBlood
	// Called on server
	// ------------------------------------------------------------
	private int CalcBlood(PlayerBase player)
	{
		return Calc(player.GetHealth("", "Blood") - 2500,  2500);
	}

	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcWater
	// Called on server
	// ------------------------------------------------------------
	private int CalcWater(PlayerBase player)
	{
		return CalcStat(player.GetStatWater());
	}

	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcEnergy
	// Called on server
	// ------------------------------------------------------------
	private int CalcEnergy(PlayerBase player)
	{
		return CalcStat(player.GetStatEnergy());
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcStamina
	// Called on server
	// ------------------------------------------------------------
	private int CalcStamina(PlayerBase player)
	{
		return CalcStat(player.GetStatStamina());
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule Calc
	// Called on server
	// ------------------------------------------------------------
	private int Calc(float value, float max)
	{
		return (Math.Round(value) * 100) / max;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcStat
	// Called on server
	// ------------------------------------------------------------
	private int CalcStat(PlayerStat<float> stat)
	{
		return Calc(stat.Get(), stat.GetMax());
	}
		
	// ------------------------------------------------------------
	// ExpansionMonitorModule CalcWeight
	// Called on server
	// ------------------------------------------------------------
	private int CalcWeight(PlayerBase player)
	{
		int weight;
		weight = player.GetWeight();
		
		return weight;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule BonesState
	// Called on server
	// ------------------------------------------------------------
	private int BonesState(PlayerBase player)
	{
		int bonesState;
		bonesState = player.m_BrokenLegState;
		
		return bonesState;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule IsBrianSick
	// Called on server
	// ------------------------------------------------------------
	private bool IsBrianSick(PlayerBase player)
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
	private bool HasCholera(PlayerBase player)
	{
		if (player.GetSingleAgentCount(eAgents.CHOLERA) >= CholeraMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasInfluenza
	// Called on server
	// ------------------------------------------------------------
	private bool HasInfluenza(PlayerBase player)
	{
		if (player.GetSingleAgentCount(eAgents.INFLUENZA) >= InfluenzaMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasSalmonella
	// Called on server
	// ------------------------------------------------------------
	private bool HasSalmonella(PlayerBase player)
	{
		if (player.GetSingleAgentCount(eAgents.SALMONELLA) >= SalmonellaMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasFoodPoison
	// Called on server
	// ------------------------------------------------------------
	private bool HasFoodPoison(PlayerBase player)
	{
		if (player.GetSingleAgentCount(eAgents.FOOD_POISON) >= PoisoningMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasWoundInfection
	// Called on server
	// ------------------------------------------------------------
	private bool HasWoundInfection(PlayerBase player)
	{
		if (player.GetSingleAgentCount(eAgents.WOUND_AGENT) >= WoundInfectionMdfr.AGENT_THRESHOLD_ACTIVATE)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule HasCuts
	// Called on server
	// ------------------------------------------------------------
	private int HasCuts(PlayerBase player)
	{
		int cuts;
		cuts = player.GetBleedingSourceCount();
		if (player.IsBleeding() && cuts > 0)
			return cuts;
		
		return 0;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetPlayerStance
	// Called on server
	// ------------------------------------------------------------	
	private int GetPlayerStance(PlayerBase player)
	{
		int stance = 0;
		CarScript car;
		
		if (!Class.CastTo(car, player.GetParent()))
		{
			if (player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH) || player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDCROUCH))
			{
				stance = 2;
			}
			else if (player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE) || player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE))
			{
				stance = 3;
			}
			else
			{
				stance = 1;
			}
		}
		else
		{
			ExpansionHelicopterScript heli;
			ExpansionBoatScript boat;
			if (Class.CastTo(heli, car))
			{
				stance = 5;
			}
			else if (Class.CastTo(boat, car))
			{
				stance = 6;
			}
			else
			{
				stance = 4;
			}
		}
		
		return stance;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetClientStats
	// Called on client
	// ------------------------------------------------------------ 
	ExpansionSyncedPlayerStats GetClientStats()
	{
		return m_ClientStats;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetClientStates
	// Called on client
	// ------------------------------------------------------------ 
	ExpansionSyncedPlayerStates GetClientStates()
	{
		return m_ClientStates;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RequestPlayerStats
	// Called from client
	// ------------------------------------------------------------
	void RequestPlayerStats(string playerID)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPlayerStats - Start");
		#endif
	
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.RequestPlayerStats, true);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_RequestPlayerStats
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPlayerStats(ParamsReadContext ctx, PlayerIdentity sender)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RPC_RequestPlayerStats - Start");
		#endif
		
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		string playerID;
		if (!ctx.Read(playerID))
			return;
		
		ExpansionSyncedPlayerStats playerStats = GetPlayerStatsByID(playerID);
		if (!playerStats)
		{
			Error("ExpansionMonitorModule::RPC_RequestPlayerStats - Could not get player stats for player wirth id: " + playerID);
			return;
		}
		
		SendPlayerStats(playerStats, playerID, sender);

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RPC_RequestPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetPlayerStatsByID
	// Called on server
	// ------------------------------------------------------------
	ExpansionSyncedPlayerStats GetPlayerStatsByID(string playerID)
	{
		ExpansionSyncedPlayerStats playerStats;
		m_Stats.Find(playerID, playerStats);
		return playerStats;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SendPlayerStats
	// Called on server
	// ------------------------------------------------------------
	void SendPlayerStats(ExpansionSyncedPlayerStats playerStats, string playerID, PlayerIdentity ident)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::SendPlayerStats - Start");
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerStats);
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.SendPlayerStats, true, ident);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::SendPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SendPlayerStats
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerStats(ParamsReadContext ctx)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RPC_SendPlayerStats - Start");
		#endif
		
		if (!GetGame().IsClient())
			return;
		
		ExpansionSyncedPlayerStats playerStats;
		if (!ctx.Read(playerStats))
			return;
		
		string playerID;
		if (!ctx.Read(playerID))
			return;

		playerStats.m_ID = playerID;

		m_StatsInvoker.Invoke(playerStats);
		m_ClientStats = playerStats;
			
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RPC_SendPlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RequestPlayerStates
	// Called from client
	// ------------------------------------------------------------
	void RequestPlayerStates(string playerID)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPlayerStates - Start");
		#endif

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.RequestPlayerStates, true);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPlayerStates - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_RequestPlayerStates
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPlayerStates(ParamsReadContext ctx, PlayerIdentity sender)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RPC_RequestPlayerStates - Start");
		#endif
		
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		string playerID;
		if (!ctx.Read(playerID))
			return;
		
		ExpansionSyncedPlayerStates playerStates = GetPlayerStatesByID(playerID);
		if (!playerStates)
		{
			Error("ExpansionMonitorModule::RPC_RequestPlayerStates - Could not get player states for player with id: " + playerID);
			return;
		}
		
		SendPlayerStates(playerStates, playerID, sender);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_RequestPlayerStates - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetPlayerStatesByID
	// Called on server
	// ------------------------------------------------------------
	ExpansionSyncedPlayerStates GetPlayerStatesByID(string playerID)
	{
		ExpansionSyncedPlayerStates playerStates;
		m_States.Find(playerID, playerStates);
		return playerStates;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SendPlayerStates
	// Called on server
	// ------------------------------------------------------------
	void SendPlayerStates(ExpansionSyncedPlayerStates playerStates, string playerID, PlayerIdentity ident)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::SendPlayerStates - Start" );
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerStates);
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.SendPlayerStates, true, ident);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::SendPlayerStates - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_SendPlayerStates
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerStates(ParamsReadContext ctx)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendPlayerStates - Start" );
		#endif
		
		if (!GetGame().IsClient())
			return;
		
		ExpansionSyncedPlayerStates playerStates;
		if (!ctx.Read(playerStates))
			return;

		string playerID;
		if (!ctx.Read(playerID))
			return;
		
		playerStates.m_ID = playerID;
		
		m_StatesInvoker.Invoke(playerStates);
		m_ClientStates = playerStates;
				
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMonitorModule::RPC_SendPlayerStates - End" );
		#endif
	}	
}