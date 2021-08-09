/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

	protected autoptr ExpansionDebugSettings m_SettingsDebug;
	protected autoptr ExpansionLogSettings m_SettingsLog;
	protected autoptr ExpansionSafeZoneSettings m_SettingsSafeZone;

	protected bool m_SettingsLoaded;
	protected bool m_Debug;
	protected ref TStringArray m_NetworkedSettings;
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	protected void OnServerInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnServerInit - Start");
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

		m_NetworkedSettings.Insert( "expansiondebugsettings" );
		m_NetworkedSettings.Insert( "expansionlogsettings" );
		
		m_SettingsLoaded = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	void Unload()
	{
		m_SettingsDebug.Unload();
		m_SettingsLog.Unload();
		m_SettingsSafeZone.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion LoadSetting
	// ------------------------------------------------------------
	void LoadSetting( ExpansionSettingBase setting )
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
		
		if ( !IsSettingLoaded( m_SettingsDebug, m_SettingsLoaded ) )
			return;
		
		if ( !IsSettingLoaded( m_SettingsLog, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsSafeZone, m_SettingsLoaded ) )
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
		m_SettingsDebug = new ExpansionDebugSettings;
		m_SettingsLog = new ExpansionLogSettings;
		m_SettingsSafeZone = new ExpansionSafeZoneSettings;

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

		m_SettingsDebug.Send( identity );
		m_SettingsLog.Send( identity );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSettings::SendSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
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
			
			case ExpansionSettingsRPC.Debug:
			{
				Expansion_Assert_False( m_SettingsDebug.OnRecieve( ctx ), "Failed reading Debug settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC m_SettingsDebug");
				#endif

				break;
			}
			
			case ExpansionSettingsRPC.Log:
			{
				Expansion_Assert_False( m_SettingsLog.OnRecieve( ctx ), "Failed reading Log settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC m_SettingsLog");
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::Save - Start");
		#endif

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsDebug.Save();
			m_SettingsLog.Save();
			m_SettingsSafeZone.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::Save - End");
		#endif
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