/**
 * ExpansionMonitorModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMonitorModule
 * @brief		This class handle expansion monitor module
 **/

#ifdef EXPANSIONMONITORMODULE
[CF_RegisterModule(ExpansionMonitorModule)]
class ExpansionMonitorModule: CF_ModuleWorld
{
	private const float UPDATE_TICK_TIME = 1.0; // refreshes 100 players every ten seconds
	private const int UPDATE_PLAYERS_PER_TICK = 10;

	//Server only
	private ref map<string, ref ExpansionSyncedPlayerStats> m_Stats;
	private ref map<string, ref ExpansionSyncedPlayerStates> m_States;
	private ref TStringArray m_PlayerIDs;
	
	private float m_UpdateQueueTimer;
	private int m_CurrentPlayerTick;
	
	//Client only
	static ref ExpansionSyncedPlayerStats m_ClientStats;
	static ref ExpansionSyncedPlayerStates m_ClientStates;
	
	static ref ScriptInvoker m_StatsInvoker = new ScriptInvoker();
	static ref ScriptInvoker m_StatesInvoker = new ScriptInvoker();

	vector m_LastDeathPos;
	
	// Server & Client
	private ref map<string, vector> m_PlayerDeathPositions;
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "OnInit");
#endif

		super.OnInit();

		EnableClientDisconnect();
		EnableClientReady();
		EnableClientRespawn();
		EnableInvokeConnect();
		EnableInvokeDisconnect();
		EnableRPC();
		EnableUpdate();

		//EXPrint(ToString() + "::OnInit - GetGame() " + GetGame());  // OK
		//EXPrint(ToString() + "::OnInit - GetGame().IsServer() " + GetGame().IsServer());  // true on client during init
		//EXPrint(ToString() + "::OnInit - GetGame().IsMultiplayer() " + GetGame().IsMultiplayer());  // false on client during init

		m_Stats = new map<string, ref ExpansionSyncedPlayerStats>;
		m_States = new map<string, ref ExpansionSyncedPlayerStates>;

		m_PlayerIDs = new TStringArray();

		m_ClientStats = new ExpansionSyncedPlayerStats;
		m_ClientStates = new ExpansionSyncedPlayerStates;
		
		m_PlayerDeathPositions = new map<string, vector>;
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
	override void OnRPC(Class sender, CF_EventArgs args)
	{
		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
		case ExpansionMonitorRPC.SyncStats:
			m_ClientStats.OnRecieve(rpc.Context);
			break;
		case ExpansionMonitorRPC.SyncStates:
			m_ClientStates.OnRecieve(rpc.Context);
			break;
		case ExpansionMonitorRPC.SendMessage:
			RPC_SendMessage(rpc.Context);
			break;
		case ExpansionMonitorRPC.RequestPlayerStats:
			RPC_RequestPlayerStats(rpc.Context, rpc.Sender);
			break;
		case ExpansionMonitorRPC.SendPlayerStats:
			RPC_SendPlayerStats(rpc.Context);
			break;
		case ExpansionMonitorRPC.RequestPlayerStates:
			RPC_RequestPlayerStates(rpc.Context, rpc.Sender);
			break;
		case ExpansionMonitorRPC.SendPlayerStates:
			RPC_SendPlayerStates(rpc.Context);
			break;
			case ExpansionMonitorRPC.SyncLastDeathPos:
			RPC_SyncLastDeathPos(rpc.Context, rpc.Sender);
			break;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnClientReady
	// ------------------------------------------------------------
	override void OnClientReady(Class sender, CF_EventArgs args)
	{
		super.OnClientReady(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (GetExpansionSettings().GetNotification().ShowPlayerJoinServer && cArgs.Identity)
		{			
			if (GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.NOTIFICATION) 
			{
				ExpansionNotification("STR_EXPANSION_PLAYER_JOINED_TITLE", new StringLocaliser("STR_EXPANSION_PLAYER_JOINED_TEXT", cArgs.Identity.GetName()), EXPANSION_NOTIFICATION_ICON_PERSONA).Info();
			} 
			else if (GetExpansionSettings().GetNotification().JoinMessageType == ExpansionAnnouncementType.CHAT)
			{
				ServerChatMessage(new StringLocaliser("STR_EXPANSION_PLAYER_JOINED_TITLE"), new StringLocaliser("STR_EXPANSION_PLAYER_JOINED_TEXT", cArgs.Identity.GetName()));
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnInvokeConnect
	// ------------------------------------------------------------
	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "OnInvokeConnect");
#endif

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (!cArgs.Player)
			return;
		
		string playerID = cArgs.Player.GetIdentity().GetId();
		AddPlayerStats(playerID);
		AddPlayerStates(playerID);

		if (m_PlayerIDs.Find(playerID) == -1)
			m_PlayerIDs.Insert(playerID);
		
		SyncLastDeathPos(cArgs.Player);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnInvokeDisconnect
	// Called on server
	// ------------------------------------------------------------
	override void OnInvokeDisconnect(Class sender, CF_EventArgs args)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "OnInvokeDisconnect");
#endif

		super.OnInvokeDisconnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (GetExpansionSettings().GetNotification().ShowPlayerLeftServer && cArgs.Player.GetIdentity()) 
		{			
			if (GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.NOTIFICATION) 
			{
				ExpansionNotification("STR_EXPANSION_PLAYER_LEFT_TITLE", new StringLocaliser("STR_EXPANSION_PLAYER_LEFT_TEXT", cArgs.Player.GetIdentity().GetName()), EXPANSION_NOTIFICATION_ICON_PERSONA).Info();
			} 
			else if (GetExpansionSettings().GetNotification().LeftMessageType == ExpansionAnnouncementType.CHAT)
			{
				ServerChatMessage(new StringLocaliser("STR_EXPANSION_PLAYER_LEFT_TITLE"), new StringLocaliser("STR_EXPANSION_PLAYER_LEFT_TEXT", cArgs.Player.GetIdentity().GetName()));
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnClientDisconnect
	// Called on server
	// ------------------------------------------------------------
	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "OnClientDisconnect");
#endif

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		if (!cArgs.Identity)
			return;
		
		string playerID = cArgs.Identity.GetId();
		RemovePlayerStats(playerID);
		RemovePlayerStates(playerID);
		m_PlayerIDs.RemoveItem(playerID);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate(Class sender, CF_EventArgs args)
	{
		super.OnUpdate(sender, args);
		
		if (IsMissionClient() || !GetExpansionSettings().GetMonitoring().Enabled)
			return;

		auto update = CF_EventUpdateArgs.Cast(args);
		
		m_UpdateQueueTimer += update.DeltaTime;
		if ( m_UpdateQueueTimer >= UPDATE_TICK_TIME )
		{
			if (m_PlayerIDs.Count() > 0)
			{
				for (int i = 0; i < UPDATE_PLAYERS_PER_TICK; ++i) 
				{
					if (m_CurrentPlayerTick >= m_PlayerIDs.Count())
					{
						m_CurrentPlayerTick = 0;
					}
					
					string currentPlayerID = m_PlayerIDs[m_CurrentPlayerTick];
					if (currentPlayerID == "")
					{
						Error("ExpansionMonitorModule::OnUpdate - Player ID is empty!");
						continue;
					}
					
					UpdateStats(currentPlayerID);
					UpdateStates(currentPlayerID);
					
					m_CurrentPlayerTick++;

					if (m_CurrentPlayerTick == m_PlayerIDs.Count())
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
			Error("ExpansionMonitorModule::UpdateStats - Could not get player stats for player wirth id: " + playerID);
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
			Error("ExpansionMonitorModule::UpdateStates - Could not get player states for player with id: " + playerID);
			return;
		}
		
		PlayerBase player = PlayerBase.GetPlayerByUID(playerID);
		if (!player)
		{
			//! Don't error here, player will be NULL if dead
			return;
		}
			
		states.m_Bones = BonesState(player);
		states.m_Sick = IsBrainSick(player);
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
	void AddPlayerStats(string playerID)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "AddPlayerStats");
#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
				
		if (!m_Stats.Contains(playerID)) //! RemovePlayerStats would never be called if AddPlayerStats was called twice (logout cancelled or reconnected in time)
		{
			m_Stats.Insert(playerID, new ExpansionSyncedPlayerStats);
			UpdateStats(playerID);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule AddPlayerStates
	// Called on server
	// ------------------------------------------------------------
	void AddPlayerStates(string playerID)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "AddPlayerStates");
#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		if (!m_States.Contains(playerID)) //! RemovePlayerStats would never be called if AddPlayerStats was called twice (logout cancelled or reconnected in time)
		{
			m_States.Insert(playerID, new ExpansionSyncedPlayerStates);
			UpdateStates(playerID);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RemovePlayerStats
	// Called on server
	// ------------------------------------------------------------
	void RemovePlayerStats(string playerID)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RemovePlayerStats");
#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		ExpansionSyncedPlayerStats stat;
		if (m_Stats.Find(playerID, stat))
			m_Stats.Remove(playerID);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RemovePlayerStates
	// Called on server
	// ------------------------------------------------------------
	void RemovePlayerStates(string playerID)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RemovePlayerStates");
#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		ExpansionSyncedPlayerStates state;
		if (m_States.Find(playerID, state))
			m_States.Remove(playerID);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule ServerChatMessage
	// Called on server
	// ------------------------------------------------------------
	void ServerChatMessage(StringLocaliser tag, StringLocaliser message)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "ServerChatMessage");
#endif

		ScriptRPC message_rpc = new ScriptRPC();
		message_rpc.Write(tag);
		message_rpc.Write(message);
		message_rpc.Send(null, ExpansionMonitorRPC.SendMessage, true);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_SendMessage
	// Called on all Clients
	// ------------------------------------------------------------
	private void RPC_SendMessage(ParamsReadContext ctx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RPC_SendMessage");
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
		eBrokenLegs state = player.GetBrokenLegs();
		if (state == eBrokenLegs.NO_BROKEN_LEGS)
		{
			bonesState = 0;
		}
		else if (state == eBrokenLegs.BROKEN_LEGS)
		{
			bonesState = 1;
		}
		else if (state == eBrokenLegs.BROKEN_LEGS_SPLINT)
		{
			bonesState = 2;
		}
		
		return bonesState;
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule IsBrainSick
	// Called on server
	// ------------------------------------------------------------
	private bool IsBrainSick(PlayerBase player)
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
			if ((player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDERECT) || player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_ERECT)) && !player.IsUnconscious())
			{
				stance = 1;
			}
			else if ((player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH) || player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDCROUCH)) && !player.IsUnconscious())
			{
				stance = 2;
			}
			else if ((player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE) || player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE)) && !player.IsUnconscious())
			{
				stance = 3;
			}
			else if (!player.IsAlive())
			{
				stance = 7;
			}
			else if (player.IsUnconscious())
			{
				stance = 8;
			}
		}
		else
		{
	#ifdef EXPANSIONMODVEHICLE
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
	#endif
				stance = 4;
	#ifdef EXPANSIONMODVEHICLE
			}
	#endif
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RequestPlayerStats");
#endif

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.RequestPlayerStats, true);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_RequestPlayerStats
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPlayerStats(ParamsReadContext ctx, PlayerIdentity sender)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RPC_RequestPlayerStats");
#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		string playerID;
		if (!ctx.Read(playerID))
			return;
		
		ExpansionSyncedPlayerStats playerStats = GetPlayerStatsByID(playerID);
		if (!playerStats)
		{
			return;
		}
		
		SendPlayerStats(playerStats, playerID, sender);
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "SendPlayerStats");
#endif

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerStats);
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.SendPlayerStats, true, ident);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SendPlayerStats
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerStats(ParamsReadContext ctx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RPC_SendPlayerStats");
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
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RequestPlayerStates
	// Called from client
	// ------------------------------------------------------------
	void RequestPlayerStates(string playerID)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RequestPlayerStates");
#endif

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.RequestPlayerStates, true);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_RequestPlayerStates
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPlayerStates(ParamsReadContext ctx, PlayerIdentity sender)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RPC_RequestPlayerStates");
#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		string playerID;
		if (!ctx.Read(playerID))
			return;
		
		ExpansionSyncedPlayerStates playerStates = GetPlayerStatesByID(playerID);
		if (!playerStates)
		{
			return;
		}
		
		SendPlayerStates(playerStates, playerID, sender);
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "SendPlayerStates");
#endif

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerStates);
		rpc.Write(playerID);
		rpc.Send(null, ExpansionMonitorRPC.SendPlayerStates, true, ident);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_SendPlayerStates
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerStates(ParamsReadContext ctx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "RPC_SendPlayerStates");
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
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule AddLastPlayerDeathPos
	// Called on server
	// ------------------------------------------------------------
	void AddLastPlayerDeathPos(string playerUID, vector pos)
	{
		if (m_PlayerDeathPositions.Get(playerUID))
			m_PlayerDeathPositions.Set(playerUID, pos);
		else
			m_PlayerDeathPositions.Insert(playerUID, pos);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetLastDeathPosServer
	// Called on server
	// ------------------------------------------------------------
	vector GetLastDeathPosServer(string playerUID)
	{
		return m_PlayerDeathPositions.Get(playerUID);
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule SyncLastDeathPos
	// Called on server
	// ------------------------------------------------------------
	void SyncLastDeathPos(PlayerBase player)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER_MONITOR, this, "SyncLastDeathPos");
#endif
			
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		vector pos = GetLastDeathPosServer(player.GetIdentity().GetId());
		if (pos == vector.Zero)
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(pos);
		rpc.Send(null, ExpansionMonitorRPC.SyncLastDeathPos, true, player.GetIdentity());
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule RPC_SyncLastDeathPos
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SyncLastDeathPos(ParamsReadContext ctx, PlayerIdentity ident)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.PLAYER_MONITOR, this, "RPC_SyncLastDeathPos").Add(ident).Add(ctx);
#endif
		
		if (!GetGame().IsClient())
			return;
		
		if (!ctx.Read(m_LastDeathPos))
		{
			Error(ToString() + "::RPC_SyncLastDeathPos - Could not read last death position!");
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMonitorModule GetLastDeathPosClient
	// Called on client
	// ------------------------------------------------------------
	vector GetLastDeathPosClient()
	{
		return m_LastDeathPos;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule OnClientRespawn
	// ------------------------------------------------------------	
	override void OnClientRespawn(Class sender, CF_EventArgs args)
	{
		super.OnClientRespawn(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (!cArgs.Player)
			return;
		
		SyncLastDeathPos(cArgs.Player);
	}
};
#endif
