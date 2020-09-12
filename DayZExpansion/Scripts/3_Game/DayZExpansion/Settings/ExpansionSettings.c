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

class ExpansionSettings
{
	static ref ScriptInvoker SI_Airdrop = new ScriptInvoker();
	static ref ScriptInvoker SI_Book = new ScriptInvoker();
	static ref ScriptInvoker SI_BaseBuilding = new ScriptInvoker();
	static ref ScriptInvoker SI_General = new ScriptInvoker();
	static ref ScriptInvoker SI_Map = new ScriptInvoker();
	static ref ScriptInvoker SI_Market = new ScriptInvoker();
	static ref ScriptInvoker SI_Mission = new ScriptInvoker();
	static ref ScriptInvoker SI_Notification = new ScriptInvoker();
	static ref ScriptInvoker SI_Party = new ScriptInvoker();
	static ref ScriptInvoker SI_Raid = new ScriptInvoker();
	static ref ScriptInvoker SI_SafeZone = new ScriptInvoker();
	static ref ScriptInvoker SI_Spawn = new ScriptInvoker();
	static ref ScriptInvoker SI_Territory = new ScriptInvoker();
	static ref ScriptInvoker SI_Vehicle = new ScriptInvoker();
	static ref ScriptInvoker SI_Debug = new ScriptInvoker();
	
	protected ref ExpansionAirdropSettings m_SettingsAirdrop;
	protected ref ExpansionBookSettings m_SettingsBook;
	protected ref ExpansionBaseBuildingSettings m_SettingsBaseBuilding;
	protected ref ExpansionGeneralSettings m_SettingsGeneral;
	protected ref ExpansionMapSettings m_SettingsMap;
	protected ref ExpansionMarketSettings m_SettingsMarket;
	protected ref ExpansionMissionSettings m_SettingsMission;
	protected ref ExpansionNotificationSettings m_SettingsNotification;
	protected ref ExpansionPartySettings m_SettingsParty;
	protected ref ExpansionSafeZoneSettings m_SettingsSafeZone;
	protected ref ExpansionSpawnSettings m_SettingsSpawn;
	protected ref ExpansionRaidSettings m_SettingsRaid;
	protected ref ExpansionTerritorySettings m_SettingsTerritory;
	protected ref ExpansionVehicleSettings m_SettingsVehicle;
	protected ref ExpansionDebugSettings m_SettingsDebug;
	
	protected bool m_SettingsLoaded;
	
	protected bool m_Debug;

	protected ref TStringArray m_NetworkedSettings;
	protected bool m_SettingsToLoadInitialSent;
	
	// ------------------------------------------------------------
	// ExpansionSettings Constructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ExpansionSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::ExpansionSettings - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::ExpansionSettings - End");
		#endif
	}
	
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

		if ( m_SettingsMarket ) 
			delete m_SettingsMarket;

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

		if ( m_SettingsVehicle ) 
			delete m_SettingsVehicle;
		
		if ( m_SettingsDebug ) 
			delete m_SettingsDebug;
	}
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	protected void OnServerInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnServerInit - Start");
		#endif
		
		m_Debug = false;
		
		if ( !FileExist( EXPANSION_SETTINGS_FOLDER ) && !GetGame().IsClient() )
		{
			MakeDirectory( EXPANSION_SETTINGS_FOLDER );
		}

		LoadSetting( m_SettingsAirdrop );
		LoadSetting( m_SettingsBook );
		LoadSetting( m_SettingsBaseBuilding );
		LoadSetting( m_SettingsGeneral );
		LoadSetting( m_SettingsMap );
		#ifndef EXPANSION_MARKET_DISABLE
		LoadSetting( m_SettingsMarket );
		#endif
		LoadSetting( m_SettingsMission );
		LoadSetting( m_SettingsNotification );
		LoadSetting( m_SettingsParty );
		LoadSetting( m_SettingsRaid );
		LoadSetting( m_SettingsSafeZone );
		LoadSetting( m_SettingsSpawn );
		LoadSetting( m_SettingsTerritory );
		LoadSetting( m_SettingsVehicle );
		LoadSetting( m_SettingsDebug );

		//m_NetworkedSettings.Insert( "expansionairdropsettings" );
		m_NetworkedSettings.Insert( "expansionbooksettings" );
		m_NetworkedSettings.Insert( "expansionbasebuildingsettings" );
		m_NetworkedSettings.Insert( "expansiongeneralsettings" );
		m_NetworkedSettings.Insert( "expansionmapsettings" );
		#ifndef EXPANSION_MARKET_DISABLE
		m_NetworkedSettings.Insert( "expansionmarketsettings" );
		#endif
		// m_NetworkedSettings.Insert( "expansionmissionsettings" );
		m_NetworkedSettings.Insert( "expansionnotificationsettings" );
		m_NetworkedSettings.Insert( "expansionpartysettings" );
		m_NetworkedSettings.Insert( "expansionraidsettings" );
		// m_NetworkedSettings.Insert( "expansionsafezonesettings" );
		m_NetworkedSettings.Insert( "expansionspawnsettings" );
		m_NetworkedSettings.Insert( "expansionterritorysettings" );
		m_NetworkedSettings.Insert( "expansionvehiclesettings" );
		m_NetworkedSettings.Insert( "expansiondebugsettings" );
		
		m_SettingsLoaded = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	void Unload()
	{
		m_SettingsAirdrop.Unload();
		m_SettingsBook.Unload();
		m_SettingsBaseBuilding.Unload();
		m_SettingsGeneral.Unload();
		m_SettingsMap.Unload();
		m_SettingsMarket.Unload();
		m_SettingsMission.Unload();
		m_SettingsNotification.Unload();
		m_SettingsRaid.Unload();
		m_SettingsParty.Unload();
		m_SettingsSafeZone.Unload();
		m_SettingsSpawn.Unload();
		m_SettingsTerritory.Unload();
		m_SettingsVehicle.Unload();
		m_SettingsDebug.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	void LoadSetting( ref ExpansionSettingBase setting )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::LoadSetting - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionSettings: Loading " + setting.ClassName() );
		#endif
		
		if ( setting.Load() )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "ExpansionSettings: Successfully Loaded " + setting.ClassName() );
			#endif
		} else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "ExpansionSettings: Unsuccessful load, using defaults for " + setting.ClassName() );
			#endif
			
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::LoadSetting - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion OnClientInit
	// ------------------------------------------------------------
	protected void OnClientInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnClientInit - Start");
		#endif

		m_SettingsLoaded = false;

		if ( IsMissionOffline() )
		{
			OnServerInit();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnClientInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CheckSettingsLoaded
	// ------------------------------------------------------------
	bool IsSettingsLoaded()
	{
		return m_SettingsLoaded;
	}
	
	// ------------------------------------------------------------
	// Expansion IsSettingLoaded
	// ------------------------------------------------------------
	bool IsSettingLoaded( ref ExpansionSettingBase setting, out bool settingsLoaded )
	{
		string name = setting.ClassName();
		name.ToLower();

		if ( m_NetworkedSettings.Find( name ) > -1 )
		{
			if ( !setting.IsLoaded() )
			{
				settingsLoaded = false;

				return false;
			}
			
			return true;
		}

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion CheckSettingsLoaded
	// Called on Client
	// ------------------------------------------------------------
	protected void CheckSettingsLoaded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::CheckSettingsLoaded - Start");
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

		if ( !IsSettingLoaded( m_SettingsMarket, m_SettingsLoaded ) )
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

		if ( !IsSettingLoaded( m_SettingsVehicle, m_SettingsLoaded ) )
			return;
		
		if ( !IsSettingLoaded( m_SettingsDebug, m_SettingsLoaded ) )
			return;

		m_SettingsLoaded = true;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::CheckSettingsLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	void Init()
	{
		m_SettingsAirdrop = new ExpansionAirdropSettings;
		m_SettingsBook = new ExpansionBookSettings;
		m_SettingsBaseBuilding = new ExpansionBaseBuildingSettings;
		m_SettingsGeneral = new ExpansionGeneralSettings;
		m_SettingsMap = new ExpansionMapSettings;
		m_SettingsMarket = new ExpansionMarketSettings;
		m_SettingsMission = new ExpansionMissionSettings;
		m_SettingsNotification = new ExpansionNotificationSettings;
		m_SettingsRaid = new ExpansionRaidSettings;
		m_SettingsParty = new ExpansionPartySettings;
		m_SettingsSafeZone = new ExpansionSafeZoneSettings;
		m_SettingsSpawn = new ExpansionSpawnSettings;
		m_SettingsTerritory = new ExpansionTerritorySettings;
		m_SettingsVehicle = new ExpansionVehicleSettings;
		m_SettingsDebug = new ExpansionDebugSettings;

		m_NetworkedSettings = new TStringArray;

		if ( IsMissionClient() )
		{
			OnClientInit();
		} else if ( IsMissionHost() )
		{			
			OnServerInit();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	void Send( notnull PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionSettings::SendSettings - Start identity : " + identity );
		#endif

		if ( IsMissionClient() )
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( m_NetworkedSettings );
		rpc.Send( NULL, ExpansionSettingsRPC.ListToLoad, true, identity );

		m_SettingsAirdrop.Send( identity );
		m_SettingsBook.Send( identity );
		m_SettingsBaseBuilding.Send( identity );
		m_SettingsGeneral.Send( identity );
		m_SettingsMap.Send( identity );
		#ifndef EXPANSION_MARKET_DISABLE
		m_SettingsMarket.Send( identity );
		#endif
		m_SettingsNotification.Send( identity );
		m_SettingsRaid.Send( identity );
		m_SettingsParty.Send( identity );
		m_SettingsSpawn.Send( identity );
		m_SettingsTerritory.Send( identity );
		m_SettingsVehicle.Send( identity );
		m_SettingsDebug.Send( identity );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSettings::SendSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnRPC - Start");
		#endif

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.ListToLoad:
			{
				RPC_ListToLoad( ctx, sender, target );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_ListToLoad");
				#endif

				return true;
			}
			
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
			
			case ExpansionSettingsRPC.Market:
			{
				Expansion_Assert_False( m_SettingsMarket.OnRecieve( ctx ), "Failed reading Market settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Market");
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
			
			case ExpansionSettingsRPC.Vehicle:
			{
				Expansion_Assert_False( m_SettingsVehicle.OnRecieve( ctx ), "Failed reading Vehicle settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Vehicle");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Debug:
			{
				Expansion_Assert_False( m_SettingsDebug.OnRecieve( ctx ), "Failed reading Debug settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC m_SettingsDebug");
				#endif

				break;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnRPC - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_ListToLoad
	// Called on client
	// ------------------------------------------------------------
	private void RPC_ListToLoad( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		TStringArray listToLoads = new TStringArray;
		if ( !ctx.Read( listToLoads ) )
			return;

		m_NetworkedSettings = listToLoads;
	}
	
	// ------------------------------------------------------------
	// Expansion Save
	// Called on server
	// ------------------------------------------------------------
	void Save()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::Save - Start");
		#endif

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsAirdrop.Save();
			m_SettingsBook.Save();
			m_SettingsBaseBuilding.Save();
			m_SettingsGeneral.Save();
			m_SettingsMap.Save();
			m_SettingsMarket.Save();
			m_SettingsMission.Save();
			m_SettingsNotification.Save();
			m_SettingsRaid.Save();
			m_SettingsParty.Save();
			m_SettingsSafeZone.Save();
			m_SettingsSpawn.Save();
			m_SettingsTerritory.Save();
			m_SettingsVehicle.Save();
			m_SettingsDebug.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionVehicleSettings GetAirdrop
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
	// Expansion ExpansionMarketSettings GetMarket
	// ------------------------------------------------------------
	ref ExpansionMarketSettings GetMarket()
	{
		return m_SettingsMarket;
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
	// Expansion ExpansionPartySettings GetRaid
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
	// Expansion ExpansionPartySettings GetParty
	// ------------------------------------------------------------
	ref ExpansionSpawnSettings GetSpawn()
	{
		return m_SettingsSpawn;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartySettings GetTerritory
	// ------------------------------------------------------------
	ref ExpansionTerritorySettings GetTerritory()
	{
		return m_SettingsTerritory;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionVehicleSettings GetVehicle
	// ------------------------------------------------------------
	ref ExpansionVehicleSettings GetVehicle()
	{
		return m_SettingsVehicle;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionNotificationSettings GetNotification
	// ------------------------------------------------------------
	ref ExpansionNotificationSettings GetNotification()
	{
		return m_SettingsNotification;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionDebugSettings GetMap
	// ------------------------------------------------------------
	ref ExpansionDebugSettings GetDebug()
	{
		return m_SettingsDebug;
	}
}

static ref ExpansionSettings g_exGlobalSettings;

static ref ExpansionSettings GetExpansionSettings()
{
	if ( g_exGlobalSettings == NULL )
	{
		g_exGlobalSettings = new ExpansionSettings();

		g_exGlobalSettings.Init();
	}

	return g_exGlobalSettings;
}