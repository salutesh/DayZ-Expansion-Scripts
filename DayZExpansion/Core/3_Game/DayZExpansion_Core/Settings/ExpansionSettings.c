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
	static ref ScriptInvoker SI_Debug = new ScriptInvoker();
	protected ref ExpansionDebugSettings m_SettingsDebug;
	protected bool m_SettingsLoaded;
	protected bool m_Debug;
	protected ref TStringArray m_NetworkedSettings;
	
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
		
		if ( FileExist( EXPANSION_SETTINGS_FOLDER ) && IsMissionClient() )
		{
			DeleteFile( EXPANSION_SETTINGS_FOLDER );
		}

		LoadSetting( m_SettingsDebug );

		m_NetworkedSettings.Insert( "expansiondebugsettings" );
		
		m_SettingsLoaded = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	void Unload()
	{
		m_SettingsDebug.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion LoadSetting
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
	// Expansion IsSettingsLoaded
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
			m_SettingsDebug.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionDebugSettings GetDebug
	// ------------------------------------------------------------
	ref ExpansionDebugSettings GetDebug()
	{
		return m_SettingsDebug;
	}
};

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