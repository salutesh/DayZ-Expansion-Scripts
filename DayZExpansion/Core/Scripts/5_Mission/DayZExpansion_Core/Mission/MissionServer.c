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
		//! Avoid exploit where players that are given (temporary) godmode for any reason (e.g. through a 3rd party mod,
		//! since Expansion no longer uses player godmode for anything),
		//! can disconnect while uncon or restrained and reconnect to dupe their character and all their gear
		//! (the other unconscious body will still be on the ground due to the game not being able to kill it)
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
	}
	
	// ------------------------------------------------------------
	// OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnMissionLoaded");
#endif
		
		ExpansionGlobalID.s_IsMissionLoaded = true;

		super.OnMissionLoaded();
		
		GetDayZExpansion().OnLoaded();

		CF_ModuleCoreManager.OnSettingsChanged(this, CF_EventArgs.Empty);

		string exitAfter;
		if (GetCLIParam("exitAfter", exitAfter))
		{
			EXPrint(ToString() + " Command line parameter exitAfter found - exiting after " + exitAfter + " seconds");
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(g_Game.RequestExit, exitAfter.ToInt() * 1000, false, 0);
		}
	}
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();

		GetDayZExpansion().OnFinish();
	}
	
	/**
	 * Order of player-related mission events
	 * 
	 * On connect (new character):
	 * 1) OnClientPrepare
	 * 3) OnClientNew (calls CreateCharacter/EquipCharacter in vanilla)
	 * 3) InvokeOnConnect
	 * 
	 * On connect (existing character):
	 * 1) OnClientPrepare
	 * 2) OnClientReady
	 * 3) InvokeOnConnect
	 * 
	 * On respawn:
	 * 1) OnClientRespawn
	 * 2) OnClientPrepare
	 * 3) OnClientNew (calls CreateCharacter/EquipCharacter in vanilla)
	 * 4) InvokeOnConnect
	 * 
	 * On logout:
	 * 1) OnClientDisconnectedEvent (calls OnClientLogout for CF modules)
	 * 2) OnEvent LogoutCancelEventTypeID (if player cancels logout countdown, calls OnClientLogoutCancelled for CF modules)
	 * 
	 * On disconnect (if logout countdown runs out or player early disconnects):
	 * 1) PlayerDisconnected (calls OnClientDisconnect for CF modules)
	 * 2) InvokeOnDisconnect (called by PlayerDisconnected, only if player character not yet deleted)
	 */
#ifdef DIAG_DEVELOPER
	override void OnEvent(EventType eventTypeId, Param params) 
	{
#ifdef EXTRACE_DIAG
		EXTrace trace;
		if (EXTrace.PLAYER)
		{
			string eventTypeIdString;

			switch (eventTypeId)
			{
				case ClientPrepareEventTypeID:
					eventTypeIdString = "ClientPrepareEventTypeID";
					break;
				case ClientNewEventTypeID:
					eventTypeIdString = "ClientNewEventTypeID";
					break;
				case ClientReadyEventTypeID:
					eventTypeIdString = "ClientReadyEventTypeID";
					break;
				case ClientRespawnEventTypeID:
					eventTypeIdString = "ClientRespawnEventTypeID";
					break;
				case ClientReconnectEventTypeID:
					eventTypeIdString = "ClientReconnectEventTypeID";
					break;
				case ClientDisconnectedEventTypeID:
					eventTypeIdString = "ClientDisconnectedEventTypeID";
					break;
				case LogoutCancelEventTypeID:
					eventTypeIdString = "LogoutCancelEventTypeID";
					break;
			}

			if (eventTypeIdString)
				trace = EXTrace.Start(true, this, eventTypeIdString);
		}
#endif

		super.OnEvent(eventTypeId, params);
	}
#endif

	override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
	{
		super.OnClientRespawnEvent(identity, player);

		SyncEvents.s_Expansion_RespawningUIDs.Insert(identity.GetId(), true);
	}

	override void OnClientReadyEvent( PlayerIdentity identity, PlayerBase player )
	{
		super.OnClientReadyEvent( identity, player );

//! DON'T remove! Update this for the next version of DayZ when necessary.
#ifdef DAYZ_1_27
		string preferredVersion = "1.28";
		string version;
		GetDayZGame().GetVersion(version);
		string warningText = "This server is running an unsupported version of the game (" + version + ").";
		if (g_Game.ConfigIsExisting("CfgMods DZ_Expansion_Animations_Preload"))
			warningText += " Some player animations MAY not work correctly.";
		warningText += " Preferably, please use DayZ " + preferredVersion + " (Experimental or stable when available).";
		ExpansionNotification("WARNING", warningText, "Error", COLOR_EXPANSION_NOTIFICATION_ORANGE, 20).Create(identity);
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
		string uid = identity.GetId();
		bool isRespawn = SyncEvents.s_Expansion_RespawningUIDs[uid];

		//! Need to send settings BEFORE super so that modules come after
		if (!isRespawn)
		{
			//! Only send settings if this is not a respawn
		#ifdef EXPANSION_SETTINGS_DELAY
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_SendSettings, 20000, false, player);
		#else
			GetExpansionSettings().Send( identity );
		#endif
		}

		PlayerBase.Expansion_AddPlayer( player, identity );
		
		//! Do after, because some modules use PlayerIdentity setup inside AddPlayer of PlayerBase class
		super.InvokeOnConnect( player, identity );

		PlayerBase.s_Expansion_SI_OnPlayerConnected.Invoke(player, identity);

		if (isRespawn)
		{
			EXTrace.Print(EXTrace.PLAYER, this, "Client respawn");
			//! Next call directly after InvokeOnConnect is SyncEvents.SendPlayerList which will reset s_Expansion_IsClientRespawn
			SyncEvents.s_Expansion_IsClientRespawn = true;
			//! IMPORTANT: Remove respawning UID only AFTER super call so that modules still see it!
			SyncEvents.s_Expansion_RespawningUIDs.Remove(uid);
		}
	}
	
	void Expansion_SendSettings(PlayerBase player)
	{
		GetExpansionSettings().Send(player.GetIdentity());
	}

	// ------------------------------------------------------------
	// PlayerDisconnected
	// ------------------------------------------------------------
	override void PlayerDisconnected( PlayerBase player, PlayerIdentity identity, string uid )
	{
		PlayerBase.RemovePlayer( uid );
		
		super.PlayerDisconnected( player, identity, uid );

		PlayerBase.Expansion_RemovePlayerPlainID(uid);

		if (SyncEvents.s_Expansion_RespawningUIDs[uid])
		{
			EXTrace.Print(EXTrace.PLAYER, this, "Client disconnected before respawn finished");
			SyncEvents.s_Expansion_RespawningUIDs.Remove(uid);
		}
	}
	
	// ------------------------------------------------------------
	// DumpClassNameJSON
	// ------------------------------------------------------------
	void DumpClassNameJSON(string mustContain = "")
	{
		array<string> classNames = {};
		Param1<array<string>> dump = new Param1<array<string>>(classNames);
		
		for (int i = 0; i < g_Game.ConfigGetChildrenCount("CfgVehicles"); i++)
	    {
	        string name;
	        g_Game.ConfigGetChildName("CfgVehicles", i, name);
	 
	        if (name == string.Empty)
	            continue;
	        
			ExpansionString exName = name;
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
