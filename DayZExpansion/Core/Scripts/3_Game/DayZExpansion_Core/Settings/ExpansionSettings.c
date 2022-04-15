/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSettings
{
	static ref ScriptInvoker SI_Debug = new ScriptInvoker();
	static ref ScriptInvoker SI_Log = new ScriptInvoker();
	static ref ScriptInvoker SI_SafeZone = new ScriptInvoker();
	static ref ScriptInvoker SI_Notification = new ScriptInvoker();

	protected autoptr ExpansionDebugSettings m_SettingsDebug;
	protected autoptr ExpansionLogSettings m_SettingsLog;
	protected autoptr ExpansionSafeZoneSettings m_SettingsSafeZone;
	protected autoptr ExpansionNotificationSettings m_SettingsNotification;

	protected autoptr ExpansionMonitoringSettings m_SettingsMonitoring;

	protected bool m_GameInit = false;

	protected bool m_SettingsLoaded;
	protected bool m_Debug;
	protected ref TStringArray m_NetworkedSettings;
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	protected void OnServerInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnServerInit");
#endif

		if ( FileExist( EXPANSION_SETTINGS_FOLDER ) && IsMissionClient() )
		{
			DeleteFile( EXPANSION_SETTINGS_FOLDER );
		}
		
		if ( FileExist( EXPANSION_LOG_FOLDER ) && IsMissionClient() )
		{
			DeleteFile( EXPANSION_LOG_FOLDER );
		}

		LoadSetting( m_SettingsDebug );
		LoadSetting( m_SettingsLog);
		LoadSetting( m_SettingsSafeZone );
		LoadSetting( m_SettingsNotification );

		LoadSetting( m_SettingsMonitoring );

		m_NetworkedSettings.Insert( "expansiondebugsettings" );
		m_NetworkedSettings.Insert( "expansionlogsettings" );
		m_NetworkedSettings.Insert( "expansionnotificationsettings" );
		
		m_SettingsLoaded = true;
	}

	// ------------------------------------------------------------
	void Unload()
	{
		m_SettingsDebug.Unload();
		m_SettingsLog.Unload();
		m_SettingsSafeZone.Unload();
		m_SettingsNotification.Unload();

		m_SettingsMonitoring.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion LoadSetting
	// ------------------------------------------------------------
	void LoadSetting( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "LoadSetting");
#endif

		if ( setting.Load() )
		{
			CF_Log.Info( "ExpansionSettings: Successfully Loaded " + setting.ClassName() );
		} else
		{
			CF_Log.Info( "ExpansionSettings: Unsuccessful load, using defaults for " + setting.ClassName() );
		}
	}

	// ------------------------------------------------------------
	// Expansion OnClientInit
	// ------------------------------------------------------------
	protected void OnClientInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnClientInit");
#endif

		m_SettingsLoaded = false;
	}
	
	// ------------------------------------------------------------
	// Expansion IsSettingsLoaded
	// ------------------------------------------------------------
	bool IsSettingsLoaded()
	{
		return m_SettingsLoaded;
	}
	
	// ------------------------------------------------------------
	// Expansion IsSettingLoaded
	// ------------------------------------------------------------
	bool IsSettingLoaded( ExpansionSettingBase setting, out bool settingsLoaded )
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "CheckSettingsLoaded");
#endif

		if ( !IsMissionClient() )
		{
			m_SettingsLoaded = true;

			return;
		}

		if ( m_SettingsLoaded )
			return;
		
		if ( !IsSettingLoaded( m_SettingsDebug, m_SettingsLoaded ) )
			return;
		
		if ( !IsSettingLoaded( m_SettingsLog, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsSafeZone, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsNotification, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsMonitoring, m_SettingsLoaded ) )
			return;

		m_SettingsLoaded = true;
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	void Init()
	{
		m_SettingsDebug = new ExpansionDebugSettings;
		m_SettingsLog = new ExpansionLogSettings;
		m_SettingsSafeZone = new ExpansionSafeZoneSettings;
		m_SettingsNotification = new ExpansionNotificationSettings;

		m_SettingsMonitoring = new ExpansionMonitoringSettings;

		m_NetworkedSettings = new TStringArray;
	}

	void GameInit()
	{
		if (m_GameInit)
		{
			Unload();

			m_GameInit = false;
		}

		if ( GetGame().IsMissionMainMenu() )
			return;

		if ( IsMissionHost() )  //! Server and offline mode
			OnServerInit();
		else
			OnClientInit();

		m_GameInit = true;
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	void Send( notnull PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if ( IsMissionClient() )
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( m_NetworkedSettings );
		rpc.Send( NULL, ExpansionSettingsRPC.ListToLoad, true, identity );

		m_SettingsDebug.Send( identity );
		m_SettingsLog.Send( identity );
		m_SettingsNotification.Send( identity );
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnRPC");
#endif

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.ListToLoad:
			{
				RPC_ListToLoad( ctx, sender, target );

				return true;
			}
			
			case ExpansionSettingsRPC.Debug:
			{
				Expansion_Assert_False( m_SettingsDebug.OnRecieve( ctx ), "Failed reading Debug settings" );

				return true;
			}
			
			case ExpansionSettingsRPC.Log:
			{
				Expansion_Assert_False( m_SettingsLog.OnRecieve( ctx ), "Failed reading Log settings" );

				return true;
			}
			
			case ExpansionSettingsRPC.Notification:
			{
				Expansion_Assert_False( m_SettingsNotification.OnRecieve( ctx ), "Failed reading Notification settings" );

				return true;
			}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_ListToLoad
	// Called on client
	// ------------------------------------------------------------
	private void RPC_ListToLoad( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Save");
#endif

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsDebug.Save();
			m_SettingsLog.Save();
			m_SettingsSafeZone.Save();
			m_SettingsNotification.Save();
			m_SettingsMonitoring.Save();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionDebugSettings GetDebug
	// ------------------------------------------------------------
	ExpansionDebugSettings GetDebug()
	{
		return m_SettingsDebug;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionLogSettings GetLog
	// ------------------------------------------------------------
	ExpansionLogSettings GetLog()
	{
		return m_SettingsLog;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSafeZoneSettings GetSafeZone
	// ------------------------------------------------------------
	ExpansionSafeZoneSettings GetSafeZone()
	{
		return m_SettingsSafeZone;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionNotificationSettings GetNotification
	// ------------------------------------------------------------
	ExpansionNotificationSettings GetNotification()
	{
		return m_SettingsNotification;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionNotificationSettings GetNotification
	// ------------------------------------------------------------
	ExpansionMonitoringSettings GetMonitoring()
	{
		return m_SettingsMonitoring;
	}
};

static ref ExpansionSettings g_exGlobalSettings;

static ExpansionSettings GetExpansionSettings()
{
	if ( g_exGlobalSettings == NULL )
	{
		g_exGlobalSettings = new ExpansionSettings();

		g_exGlobalSettings.Init();
	}

	return g_exGlobalSettings;
}