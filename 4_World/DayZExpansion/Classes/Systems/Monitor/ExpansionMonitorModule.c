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
	//Server only
	protected ref map< string, ref ExpansionSyncedPlayerStats > m_Stats;
	protected float m_MonitorTimer;
	
	//Client only
	protected ref ExpansionSyncedPlayerStats m_PlayerStats;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMonitorModule
	// ------------------------------------------------------------
	void ExpansionMonitorModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::ExpansionMonitorModule - Start");
		#endif
		
		if (IsMissionHost())
		{
			m_Stats = new map< string, ref ExpansionSyncedPlayerStats>;
			m_MonitorTimer = 0;
		}
		else
		{
			m_PlayerStats = null;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::ExpansionMonitorModule - End");
		#endif
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
			RPC_SyncPlayerStats( ctx, sender, target );
			break;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnRPC - End");
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
			SyncStats( identity );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartyModule::OnInvokeConnect - End");
		#endif
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnUpdate - Start");
		#endif

		m_MonitorTimer += timeslice;
		//! Update and sync values every 5 seconds
		if ( m_MonitorTimer >= 5.0 )
		{
			UpdatePlayerStats();
			
			m_MonitorTimer = 0;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override AddPlayerStats
	// Called on server
	// ------------------------------------------------------------
	void AddPlayerStats(PlayerBase player, PlayerIdentity identity)
	{		
		if ( !IsMissionHost() ) return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::AddPlayerStats - Start");
		#endif
		
		ref ExpansionSyncedPlayerStats player_stats = new ExpansionSyncedPlayerStats;
		
		player_stats.m_PlayerUID = identity.GetId();
		player_stats.m_Health = CalcHealth(player);
		player_stats.m_Blood = CalcBlood(player);
		player_stats.m_Water = CalcWater(player);
		player_stats.m_Energy = CalcEnergy(player);
		player_stats.m_Stamina = CalcStamina(player);
		//! player_stats.m_Karma = player.GetKarma();
		
		player_stats.m_Distance = player.StatGet("dist");
		player_stats.m_Playtime = player.StatGet("playtime");
		player_stats.m_PlayersKilled = player.StatGet("players_killed");
		player_stats.m_InfectedKilled = player.StatGet("infected_killed");
		player_stats.m_AnimalsKilled = player.StatGet("animals_killed");
		player_stats.m_LongestShot = player.StatGet("longest_survivor_hit");
		
		player_stats.m_Weight = CalcWeight(player);
		
		m_Stats.Insert(player_stats.m_PlayerUID, player_stats);

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
		if ( !IsMissionHost() ) return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStats - Start");
		#endif
		
		m_Stats.Remove(identity.GetId());

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::RemovePlayerStats - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePlayerStats
	// ------------------------------------------------------------
	void UpdatePlayerStats()
	{
		if ( !IsMissionHost() ) return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::UpdatePlayerStats - Start");
		#endif
			
		for (int i = 0; i < m_Stats.Count(); ++i) 
		{
			ref ExpansionSyncedPlayerStats current_stats = m_Stats.GetElement(i);

			PlayerBase active_player = PlayerBase.GetPlayerByUID( current_stats.m_PlayerUID );
			
			if ( !active_player || !active_player.GetIdentity() )
				continue;
			
			current_stats.m_Health = CalcHealth(active_player);
			current_stats.m_Blood = CalcBlood(active_player);
			current_stats.m_Water = CalcWater(active_player);
			current_stats.m_Energy = CalcEnergy(active_player);
			current_stats.m_Stamina = CalcStamina(active_player);
			//! current_stats.m_Karma = active_player.GetKarma();
			
			current_stats.m_Distance = active_player.StatGet("dist");
			current_stats.m_Playtime = active_player.StatGet("playtime");
			current_stats.m_PlayersKilled = active_player.StatGet("players_killed");
			current_stats.m_InfectedKilled = active_player.StatGet("infected_killed");
			current_stats.m_AnimalsKilled = active_player.StatGet("animals_killed");
			current_stats.m_LongestShot = active_player.StatGet("longest_survivor_hit");
			
			current_stats.m_Weight = CalcWeight(active_player);
			
			SyncStats( active_player.GetIdentity(), current_stats );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::UpdatePlayerStats - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion SyncStats
	// Called on server
	// ------------------------------------------------------------
	void SyncStats( PlayerIdentity identity )
	{
		ExpansionSyncedPlayerStats stats = m_Stats.Get( identity.GetId() );
				
		if ( !IsMissionHost() || !stats )
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::SyncStats - Start");
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( stats );
		rpc.Send( NULL, ExpansionMonitorRPC.SyncStats, false, identity );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::SyncStats - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SyncStats
	// Called on server
	// ------------------------------------------------------------
	void SyncStats( PlayerIdentity identity, ref ExpansionSyncedPlayerStats stats )
	{		
		if ( !IsMissionHost() || !stats )
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::SyncStats - Start");
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( stats );
		rpc.Send( NULL, ExpansionMonitorRPC.SyncStats, false, identity );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMonitorModule::SyncStats - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion RPC_SyncPlayerStats
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SyncPlayerStats( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !ctx.Read( m_PlayerStats ) )
			return;
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
        return m_PlayerStats;
    }
}