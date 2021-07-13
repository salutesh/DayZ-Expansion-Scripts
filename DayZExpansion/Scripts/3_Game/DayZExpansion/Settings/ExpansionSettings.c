/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_Airdrop = new ScriptInvoker();
	static ref ScriptInvoker SI_Book = new ScriptInvoker();
	static ref ScriptInvoker SI_BaseBuilding = new ScriptInvoker();
	static ref ScriptInvoker SI_General = new ScriptInvoker();
	static ref ScriptInvoker SI_Map = new ScriptInvoker();
	static ref ScriptInvoker SI_Mission = new ScriptInvoker();
	static ref ScriptInvoker SI_Notification = new ScriptInvoker();
	static ref ScriptInvoker SI_Party = new ScriptInvoker();
	static ref ScriptInvoker SI_Raid = new ScriptInvoker();
	static ref ScriptInvoker SI_SafeZone = new ScriptInvoker();
	static ref ScriptInvoker SI_Spawn = new ScriptInvoker();
	static ref ScriptInvoker SI_Territory = new ScriptInvoker();
	static ref ScriptInvoker SI_PlayerList = new ScriptInvoker();
	
	protected ref ExpansionAirdropSettings m_SettingsAirdrop;
	protected ref ExpansionBookSettings m_SettingsBook;
	protected ref ExpansionBaseBuildingSettings m_SettingsBaseBuilding;
	protected ref ExpansionGeneralSettings m_SettingsGeneral;
	protected ref ExpansionMapSettings m_SettingsMap;
	protected ref ExpansionMissionSettings m_SettingsMission;
	protected ref ExpansionNotificationSettings m_SettingsNotification;
	protected ref ExpansionPartySettings m_SettingsParty;
	protected ref ExpansionSafeZoneSettings m_SettingsSafeZone;
	protected ref ExpansionSpawnSettings m_SettingsSpawn;
	protected ref ExpansionRaidSettings m_SettingsRaid;
	protected ref ExpansionTerritorySettings m_SettingsTerritory;
	protected ref ExpansionPlayerListSettings m_SettingsPlayerList;
	
	// ------------------------------------------------------------
	// ExpansionSettings Destructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ~ExpansionSettings()
	{
		if ( m_SettingsAirdrop ) 
			delete m_SettingsAirdrop;

		if ( m_SettingsBook ) 
			delete m_SettingsBook;

		if ( m_SettingsBaseBuilding ) 
			delete m_SettingsBaseBuilding;

		if ( m_SettingsGeneral ) 
			delete m_SettingsGeneral;

		if ( m_SettingsMap ) 
			delete m_SettingsMap;

		if ( m_SettingsMission ) 
			delete m_SettingsMission;

		if ( m_SettingsNotification ) 
			delete m_SettingsNotification;

		if ( m_SettingsParty ) 
			delete m_SettingsParty;

		if ( m_SettingsRaid ) 
			delete m_SettingsRaid;

		if ( m_SettingsSafeZone ) 
			delete m_SettingsSafeZone;

		if ( m_SettingsSpawn ) 
			delete m_SettingsSpawn;

		if ( m_SettingsTerritory ) 
			delete m_SettingsTerritory;
		
		if ( m_SettingsPlayerList)
			delete m_SettingsPlayerList;
	}
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::OnServerInit - Start");
		#endif

		LoadSetting( m_SettingsAirdrop );
		LoadSetting( m_SettingsBook );
		LoadSetting( m_SettingsBaseBuilding );
		LoadSetting( m_SettingsGeneral );
		LoadSetting( m_SettingsMap );
		LoadSetting( m_SettingsMission );
		LoadSetting( m_SettingsNotification );
		LoadSetting( m_SettingsParty );
		LoadSetting( m_SettingsRaid );
		LoadSetting( m_SettingsSafeZone );
		LoadSetting( m_SettingsSpawn );
		LoadSetting( m_SettingsTerritory );
		LoadSetting( m_SettingsPlayerList);

		//m_NetworkedSettings.Insert( "expansionairdropsettings" );
		m_NetworkedSettings.Insert( "expansionbooksettings" );
		m_NetworkedSettings.Insert( "expansionbasebuildingsettings" );
		m_NetworkedSettings.Insert( "expansiongeneralsettings" );
		m_NetworkedSettings.Insert( "expansionmapsettings" );
		// m_NetworkedSettings.Insert( "expansionmissionsettings" );
		m_NetworkedSettings.Insert( "expansionnotificationsettings" );
		m_NetworkedSettings.Insert( "expansionpartysettings" );
		m_NetworkedSettings.Insert( "expansionraidsettings" );
		// m_NetworkedSettings.Insert( "expansionsafezonesettings" );
		m_NetworkedSettings.Insert( "expansionspawnsettings" );
		m_NetworkedSettings.Insert( "expansionterritorysettings" );
		m_NetworkedSettings.Insert( "expansionplayerlistsettings" );
		
		super.OnServerInit();

		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsAirdrop.Unload();
		m_SettingsBook.Unload();
		m_SettingsBaseBuilding.Unload();
		m_SettingsGeneral.Unload();
		m_SettingsMap.Unload();
		m_SettingsMission.Unload();
		m_SettingsNotification.Unload();
		m_SettingsRaid.Unload();
		m_SettingsParty.Unload();
		m_SettingsSafeZone.Unload();
		m_SettingsSpawn.Unload();
		m_SettingsTerritory.Unload();
		m_SettingsPlayerList.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion CheckSettingsLoaded
	// Called on Client
	// ------------------------------------------------------------
	override protected void CheckSettingsLoaded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::CheckSettingsLoaded - Start");
		#endif

		if ( !IsMissionClient() )
		{
			m_SettingsLoaded = true;

			return;
		}

		if ( m_SettingsLoaded )
			return;
		
		if ( !IsSettingLoaded( m_SettingsAirdrop, m_SettingsLoaded ) )
			return;
		
		if ( !IsSettingLoaded( m_SettingsBook, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsBaseBuilding, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsGeneral, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsMap, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsMission, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsNotification, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsRaid, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsParty, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsSafeZone, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsSpawn, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsTerritory, m_SettingsLoaded ) )
			return;
		
		if ( !IsSettingLoaded( m_SettingsPlayerList, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::CheckSettingsLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsAirdrop = new ExpansionAirdropSettings;
		m_SettingsBook = new ExpansionBookSettings;
		m_SettingsBaseBuilding = new ExpansionBaseBuildingSettings;
		m_SettingsGeneral = new ExpansionGeneralSettings;
		m_SettingsMap = new ExpansionMapSettings;
		m_SettingsMission = new ExpansionMissionSettings;
		m_SettingsNotification = new ExpansionNotificationSettings;
		m_SettingsRaid = new ExpansionRaidSettings;
		m_SettingsParty = new ExpansionPartySettings;
		m_SettingsSafeZone = new ExpansionSafeZoneSettings;
		m_SettingsSpawn = new ExpansionSpawnSettings;
		m_SettingsTerritory = new ExpansionTerritorySettings;
		m_SettingsPlayerList = new ExpansionPlayerListSettings;

		super.Init();
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	override void Send( notnull PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[MAIN] ExpansionSettings::SendSettings - Start identity : " + identity );
		#endif

		if ( IsMissionClient() )
			return;

		super.Send( identity );

		m_SettingsAirdrop.Send( identity );
		m_SettingsBook.Send( identity );
		m_SettingsBaseBuilding.Send( identity );
		m_SettingsGeneral.Send( identity );
		m_SettingsMap.Send( identity );
		m_SettingsNotification.Send( identity );
		m_SettingsRaid.Send( identity );
		m_SettingsParty.Send( identity );
		m_SettingsSpawn.Send( identity );
		m_SettingsTerritory.Send( identity );
		m_SettingsPlayerList.Send( identity );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[MAIN] ExpansionSettings::SendSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::OnRPC - Start");
		#endif

		if ( !super.OnRPC( sender, target, rpc_type, ctx ) )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.AirDrop:
			{
				Expansion_Assert_False( m_SettingsAirdrop.OnRecieve( ctx ), "Failed reading AirDrop settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_AirDrop");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Book:
			{
				Expansion_Assert_False( m_SettingsBook.OnRecieve( ctx ), "Failed reading Book settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Book");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.BaseBuilding:
			{
				Expansion_Assert_False( m_SettingsBaseBuilding.OnRecieve( ctx ), "Failed reading BaseBuilding settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_BaseBuilding");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.General:
			{
				Expansion_Assert_False( m_SettingsGeneral.OnRecieve( ctx ), "Failed reading General settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_General");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Map:
			{
				Expansion_Assert_False( m_SettingsMap.OnRecieve( ctx ), "Failed reading Map settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Map");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Notification:
			{
				Expansion_Assert_False( m_SettingsNotification.OnRecieve( ctx ), "Failed reading Notification settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Notification");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Raid:
			{
				Expansion_Assert_False( m_SettingsRaid.OnRecieve( ctx ), "Failed reading Raid settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Raid");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Party:
			{
				Expansion_Assert_False( m_SettingsParty.OnRecieve( ctx ), "Failed reading Party settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Party");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Spawn:
			{
				Expansion_Assert_False( m_SettingsSpawn.OnRecieve( ctx ), "Failed reading Spawn settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Spawn");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Territory:
			{
				Expansion_Assert_False( m_SettingsTerritory.OnRecieve( ctx ), "Failed reading Territory settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Territory");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.PlayerList:
			{
				Expansion_Assert_False( m_SettingsPlayerList.OnRecieve( ctx ), "Failed reading PlayerList settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC m_SettingsPlayerList");
				#endif

				break;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::OnRPC - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion Save
	// Called on server
	// ------------------------------------------------------------
	override void Save()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::Save - Start");
		#endif

		super.Save();

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsAirdrop.Save();
			m_SettingsBook.Save();
			m_SettingsBaseBuilding.Save();
			m_SettingsGeneral.Save();
			m_SettingsMap.Save();
			m_SettingsMission.Save();
			m_SettingsNotification.Save();
			m_SettingsRaid.Save();
			m_SettingsParty.Save();
			m_SettingsSafeZone.Save();
			m_SettingsSpawn.Save();
			m_SettingsTerritory.Save();
			m_SettingsPlayerList.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[MAIN] ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionAirdropSettings GetAirdrop
	// ------------------------------------------------------------
	ref ExpansionAirdropSettings GetAirdrop()
	{
		return m_SettingsAirdrop;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionBookSettings GetBook
	// ------------------------------------------------------------
	ref ExpansionBookSettings GetBook()
	{
		return m_SettingsBook;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartySettings GetBaseBuilding
	// ------------------------------------------------------------
	ref ExpansionBaseBuildingSettings GetBaseBuilding()
	{
		return m_SettingsBaseBuilding;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMapSettings GetMap
	// ------------------------------------------------------------
	ref ExpansionMapSettings GetMap()
	{
		return m_SettingsMap;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMissionSettings GetMission
	// ------------------------------------------------------------
	ref ExpansionMissionSettings GetMission()
	{
		return m_SettingsMission;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionRaidSettings GetRaid
	// ------------------------------------------------------------
	ref ExpansionRaidSettings GetRaid()
	{
		return m_SettingsRaid;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionPartySettings GetParty
	// ------------------------------------------------------------
	ref ExpansionPartySettings GetParty()
	{
		return m_SettingsParty;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionGeneralSettings GetGeneral
	// ------------------------------------------------------------
	ref ExpansionGeneralSettings GetGeneral()
	{
		return m_SettingsGeneral;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionSafeZoneSettings GetSafeZone
	// ------------------------------------------------------------
	ref ExpansionSafeZoneSettings GetSafeZone()
	{
		return m_SettingsSafeZone;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnSettings GetParty
	// ------------------------------------------------------------
	ref ExpansionSpawnSettings GetSpawn()
	{
		return m_SettingsSpawn;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritorySettings GetTerritory
	// ------------------------------------------------------------
	ref ExpansionTerritorySettings GetTerritory()
	{
		return m_SettingsTerritory;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionNotificationSettings GetNotification
	// ------------------------------------------------------------
	ref ExpansionNotificationSettings GetNotification()
	{
		return m_SettingsNotification;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPlayerListSettings GetPlayerList
	// ------------------------------------------------------------
	ref ExpansionPlayerListSettings GetPlayerList()
	{
		return m_SettingsPlayerList;
	}
}
