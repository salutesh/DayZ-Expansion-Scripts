/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - Start");
		#endif

		GetExpansionSettings().GameInit();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// MissionServer Destructor
	// ------------------------------------------------------------
	void ~MissionServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - Start");
		#endif
				
		DestroyNotificationSystem();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - End");
		#endif
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionStart - Start");
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
		
		GetDayZExpansion().OnLoaded();

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
		ExpansionClassNameDump newDump = new ExpansionClassNameDump;
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgVehicles"); i++)
	    {
	        string name;
	        GetGame().ConfigGetChildName("CfgVehicles", i, name);
	 
	        if (name == string.Empty)
	            continue;
	        
			Print(name);
			ExpansionString exName = new ExpansionString(name);
			if (exName.EndsWith("_Base"))
			{
				Print("We hit a _Base");
				continue;
			}
			
			if (mustContain && !name.Contains(mustContain))
				continue;
		        
		    newDump.ClassNames.Insert(name);	
	    }
		
		JsonFileLoader<ExpansionClassNameDump>.JsonSaveFile("$profile:\\" + mustContain + "ClassNames.json", newDump);
		delete newDump;
	}
};

class ExpansionClassNameDump
{
	ref array<string> ClassNames = new array<string>;
};
