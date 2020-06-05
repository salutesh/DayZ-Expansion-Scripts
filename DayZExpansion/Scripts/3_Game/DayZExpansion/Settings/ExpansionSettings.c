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
	static ref ScriptInvoker SI_Book = new ScriptInvoker();
	static ref ScriptInvoker SI_General = new ScriptInvoker();
	static ref ScriptInvoker SI_Map = new ScriptInvoker();
	static ref ScriptInvoker SI_Market = new ScriptInvoker();
	static ref ScriptInvoker SI_Mission = new ScriptInvoker();
	static ref ScriptInvoker SI_Party = new ScriptInvoker();
	static ref ScriptInvoker SI_SafeZone = new ScriptInvoker();
	static ref ScriptInvoker SI_Spawn = new ScriptInvoker();
	static ref ScriptInvoker SI_BaseBuilding = new ScriptInvoker();
	static ref ScriptInvoker SI_Territory = new ScriptInvoker();
	
    protected ref ExpansionBookSettings m_SettingsBook;
	protected ref ExpansionGeneralSettings m_SettingsGeneral;
    protected ref ExpansionMapSettings m_SettingsMap;
	protected ref ExpansionMarketSettings m_SettingsMarket;
	protected ref ExpansionMissionSettings m_SettingsMission;
    protected ref ExpansionPartySettings m_SettingsParty;
	protected ref ExpansionSafeZoneSettings m_SettingsSafeZone;
	protected ref ExpansionSpawnSettings m_SettingsSpawn;
	protected ref ExpansionBaseBuildingSettings m_SettingsBaseBuilding;
	protected ref ExpansionTerritorySettings m_SettingsTerritory;
	
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
		if ( m_SettingsBook ) 
			delete m_SettingsBook;

		if ( m_SettingsGeneral ) 
			delete m_SettingsGeneral;

    	if ( m_SettingsMap ) 
			delete m_SettingsMap;

		if ( m_SettingsMarket ) 
			delete m_SettingsMarket;

		if ( m_SettingsMission ) 
			delete m_SettingsMission;

    	if ( m_SettingsParty ) 
			delete m_SettingsParty;

		if ( m_SettingsSafeZone ) 
			delete m_SettingsSafeZone;

		if ( m_SettingsSpawn ) 
			delete m_SettingsSpawn;

		if ( m_SettingsBaseBuilding ) 
			delete m_SettingsBaseBuilding;

		if ( m_SettingsTerritory ) 
			delete m_SettingsTerritory;
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

		LoadSetting( m_SettingsBook );
		LoadSetting( m_SettingsGeneral );
		LoadSetting( m_SettingsMap );
		#ifdef EXPANSION_MARKET
		LoadSetting( m_SettingsMarket );
		#endif
		LoadSetting( m_SettingsMission );
		LoadSetting( m_SettingsParty );
		LoadSetting( m_SettingsSafeZone );
		LoadSetting( m_SettingsSpawn );
		LoadSetting( m_SettingsBaseBuilding );
		LoadSetting( m_SettingsTerritory );

		// m_NetworkedSettings.Insert( "expansionairdropsettings" );
		m_NetworkedSettings.Insert( "expansionbooksettings" );
		m_NetworkedSettings.Insert( "expansiongeneralsettings" );
		m_NetworkedSettings.Insert( "expansionmapsettings" );
		#ifdef EXPANSION_MARKET
		m_NetworkedSettings.Insert( "expansionmarketsettings" );
		#endif
		// m_NetworkedSettings.Insert( "expansionmissionsettings" );
		m_NetworkedSettings.Insert( "expansionpartysettings" );
		// m_NetworkedSettings.Insert( "expansionsafezonesettings" );
		m_NetworkedSettings.Insert( "expansionspawnsettings" );
		m_NetworkedSettings.Insert( "expansionbasebuildingsettings" );
		m_NetworkedSettings.Insert( "expansionterritorysettings" );

		m_SettingsLoaded = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	void Unload()
	{
		m_SettingsBook.Unload();
		m_SettingsGeneral.Unload();
		m_SettingsMap.Unload();
		m_SettingsMarket.Unload();
		m_SettingsMission.Unload();
		m_SettingsParty.Unload();
		m_SettingsSafeZone.Unload();
		m_SettingsSpawn.Unload();
		m_SettingsBaseBuilding.Unload();
		m_SettingsTerritory.Unload();
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
		
		if ( !IsSettingLoaded( m_SettingsBook, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsGeneral, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsMap, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsMarket, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsMission, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsParty, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsSafeZone, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsSpawn, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsBaseBuilding, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsTerritory, m_SettingsLoaded ) )
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
		m_SettingsBook = new ExpansionBookSettings;
		m_SettingsGeneral = new ExpansionGeneralSettings;
		m_SettingsMap = new ExpansionMapSettings;
		m_SettingsMarket = new ExpansionMarketSettings;
		m_SettingsMission = new ExpansionMissionSettings;
		m_SettingsParty = new ExpansionPartySettings;
		m_SettingsSafeZone = new ExpansionSafeZoneSettings;
		m_SettingsSpawn = new ExpansionSpawnSettings;
		m_SettingsBaseBuilding = new ExpansionBaseBuildingSettings;
		m_SettingsTerritory = new ExpansionTerritorySettings;
		
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

		m_SettingsBook.Send( identity );
		m_SettingsGeneral.Send( identity );
		m_SettingsMap.Send( identity );
		#ifdef EXPANSION_MARKET
		m_SettingsMarket.Send( identity );
		#endif
		m_SettingsParty.Send( identity );
		m_SettingsSpawn.Send( identity );
		m_SettingsBaseBuilding.Send( identity );
		m_SettingsTerritory.Send( identity );

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
			
			case ExpansionSettingsRPC.Book:
			{
				m_SettingsBook.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Book");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.General:
			{
				m_SettingsGeneral.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_General");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Map:
			{
				m_SettingsMap.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Map");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Market:
			{
				m_SettingsMarket.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Market");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Party:
			{
				m_SettingsParty.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Party");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Spawn:
			{
				m_SettingsSpawn.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Spawn");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.BaseBuilding:
			{
				m_SettingsBaseBuilding.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_BaseBuilding");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Territory:
			{
				m_SettingsTerritory.HandleRPC( ctx );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_BaseBuilding");
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
			m_SettingsBook.Save();
			m_SettingsGeneral.Save();
			m_SettingsMap.Save();
			m_SettingsMarket.Save();
			m_SettingsMission.Save();
			m_SettingsParty.Save();
			m_SettingsSafeZone.Save();
			m_SettingsSpawn.Save();
			m_SettingsBaseBuilding.Save();
			m_SettingsTerritory.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketSettings GetMarket
	// ------------------------------------------------------------
	ref ExpansionMarketSettings GetMarket()
	{
		return m_SettingsMarket;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionPartySettings GetParty
	// ------------------------------------------------------------
	ref ExpansionPartySettings GetParty()
	{
		return m_SettingsParty;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionBookSettings GetBook
	// ------------------------------------------------------------
	ref ExpansionBookSettings GetBook()
	{
		return m_SettingsBook;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMapSettings GetMap
	// ------------------------------------------------------------
	ref ExpansionMapSettings GetMap()
	{
		return m_SettingsMap;
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
	// Expansion ExpansionPartySettings GetBaseBuilding
	// ------------------------------------------------------------
	ref ExpansionBaseBuildingSettings GetBaseBuilding()
	{
		return m_SettingsBaseBuilding;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartySettings GetTerritory
	// ------------------------------------------------------------
	ref ExpansionTerritorySettings GetTerritory()
	{
		return m_SettingsTerritory;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMissionSettings GetMission
	// ------------------------------------------------------------
	ref ExpansionMissionSettings GetMission()
	{
		return m_SettingsMission;
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