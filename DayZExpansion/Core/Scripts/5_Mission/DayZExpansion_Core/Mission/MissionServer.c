/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	bool EXPANSION_CLASSNAME_DUMP = false;
	
	// ------------------------------------------------------------
	// MissionServer Constructor
	// ------------------------------------------------------------
	void MissionServer()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "MissionServer");
#endif
		
		GetExpansionSettings().GameInit();
	}

	// ------------------------------------------------------------
	// MissionServer Destructor
	// ------------------------------------------------------------
	void ~MissionServer()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "~MissionServer");
#endif
			
		DestroyNotificationSystem();
	}

	override void HandleBody(PlayerBase player)
	{
		//! Avoid exploit where you are just outside safezone, get shot uncon, fall backwards into safezone,
		//! then disconnect and reconnect to dupe your character
		//! (your other unconscious body will still be on the ground inside safezone due to having gained godmode from it)
		if (player.IsUnconscious() || player.IsRestrained())
			player.SetAllowDamage(true);

		super.HandleBody(player);
	}

	// ------------------------------------------------------------
	// OnMissionStart
	// ------------------------------------------------------------
	override void OnMissionStart()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnMissionStart");
#endif
		
		super.OnMissionStart();
		
		GetDayZExpansion().OnStart();	

		if (EXPANSION_CLASSNAME_DUMP)
		{
			DumpClassNameJSON();
			DumpClassNameJSON("Expansion");
		}

		bool loadTraderNPCs;

		#ifdef EXPANSIONMODMARKET
		loadTraderNPCs = GetExpansionSettings().GetMarket().MarketSystemEnabled;
		#endif

		ExpansionObjectSpawnTools.FindMissionFiles(true, loadTraderNPCs);
	}
	
	// ------------------------------------------------------------
	// OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnMissionLoaded");
#endif
		
		super.OnMissionLoaded();
		
		GetDayZExpansion().OnLoaded();

		GetModuleManager().OnSettingsUpdated();
	}
	
	// ------------------------------------------------------------
	// OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnMissionFinish");
#endif
		
		super.OnMissionFinish();

		//! Save settings on mission finish
		g_exGlobalSettings.Save();
	}
	
	// ------------------------------------------------------------
	// OnClientReconnectEvent
	// ------------------------------------------------------------
	override void OnClientReconnectEvent( PlayerIdentity identity, PlayerBase player )
	{
		super.OnClientReconnectEvent( identity, player );

		g_exGlobalSettings.Send( identity );
	}
	
	// ------------------------------------------------------------
	// InvokeOnConnect
	// ------------------------------------------------------------
	override void InvokeOnConnect( PlayerBase player, PlayerIdentity identity )
	{
		GetExpansionSettings().Send( identity );
		
		//! Leave this here. If someone complains about vehicle desync as pilot, ask them about server logs and exact timestamp, then use this to check whether the desyncing player had an identity on connect.
		if ( !player.GetIdentity() )
			EXPrint("WARNING: Player without identity connecting! " + player + " " + identity);

		PlayerBase.AddPlayer( player, identity );
		
		//! Do after, because some modules use PlayerIdentity setup inside AddPlayer of PlayerBase class
		super.InvokeOnConnect( player, identity );
	}

	// ------------------------------------------------------------
	// PlayerDisconnected
	// ------------------------------------------------------------
	override void PlayerDisconnected( PlayerBase player, PlayerIdentity identity, string uid )
	{
		PlayerBase.RemovePlayer( uid );
		
		super.PlayerDisconnected( player, identity, uid );
	}
	
	// ------------------------------------------------------------
	// DumpClassNameJSON
	// ------------------------------------------------------------
	void DumpClassNameJSON(string mustContain = "")
	{
		array<string> classNames();
		Param1<array<string>> dump = new Param1<array<string>>(classNames);
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgVehicles"); i++)
	    {
	        string name;
	        GetGame().ConfigGetChildName("CfgVehicles", i, name);
	 
	        if (name == string.Empty)
	            continue;
	        
			ExpansionString exName = new ExpansionString(name);
			if (exName.EndsWith("_Base"))
			{
				continue;
			}
			
			if (mustContain && !name.Contains(mustContain))
				continue;
		        
		    classNames.Insert(name);	
	    }
		
		JsonFileLoader<Param1<array<string>>>.JsonSaveFile("$profile:\\" + mustContain + "ClassNames.json", dump);
	}
};
