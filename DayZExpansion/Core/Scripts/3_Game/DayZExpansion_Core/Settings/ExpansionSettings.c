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
	static ref ScriptInvoker SI_Core = new ScriptInvoker();
	static ref ScriptInvoker SI_Debug = SI_Core;  //! Legacy compat
	static ref ScriptInvoker SI_Log = new ScriptInvoker();
	static ref ScriptInvoker SI_SafeZone = new ScriptInvoker();
	static ref ScriptInvoker SI_Notification = new ScriptInvoker();
	static ref ScriptInvoker SI_DamageSystem = new ScriptInvoker();

	protected bool m_GameInit = false;

	protected ref map<typename, ref ExpansionSettingBase> m_Settings = new map<typename, ref ExpansionSettingBase>;
	protected autoptr array<ExpansionSettingBase> m_SettingsOrdered = new array<ExpansionSettingBase>;
	protected ref map<typename, int> m_Warnings = new map<typename, int>;

	ref ExpansionRPCManager m_Expansion_RPCManager = new ExpansionRPCManager(this);
	protected ref map<typename, int> m_RPCIDs = new map<typename, int>;

	void ServerInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this);
#endif
		
		Init();

		foreach (ExpansionSettingBase setting: m_SettingsOrdered)
		{
			Load(setting);
		}
	}

	void Unload()
	{
#ifdef EXTRACE
		auto trace = EXTrace.StartStack(EXTrace.SETTINGS, this);
#endif
		
		foreach (ExpansionSettingBase setting: m_SettingsOrdered)
		{
			setting.Unload();
		}
	}

	void Load(ExpansionSettingBase setting)
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

	bool IsLoaded(typename type)
	{
		ExpansionSettingBase setting;
		if (m_Settings.Find(type, setting))
		{
			if (setting.IsLoaded())
				return true;
		}

		return false;
	}

	void Init()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this);
#endif
		
		Init(ExpansionCoreSettings, true);
		Init(ExpansionLogSettings);
		Init(ExpansionSafeZoneSettings);
		Init(ExpansionNotificationSettings, true);
		Init(ExpansionMonitoringSettings, true);
		Init(ExpansionDamageSystemSettings);
	}

	void RegisterClientRPC(typename type)
	{
		string name = type.ToString();
		name = name.Substring(9, name.Length() - 9);  //! Strip "Expansion"
		m_RPCIDs[type] = m_Expansion_RPCManager.RegisterClient("RPC_" + name);
	}

	ExpansionScriptRPC CreateRPC(typename type)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Profile(EXTrace.SETTINGS, this);
#endif
		
		return m_Expansion_RPCManager.CreateRPC(m_RPCIDs[type]);
	}

	ExpansionScriptRPC CreateRPC(string fn)
	{
		return m_Expansion_RPCManager.CreateRPC(fn);
	}

	ExpansionBitStreamReader GetReader(ParamsReadContext ctx)
	{
		return m_Expansion_RPCManager.GetReader(ctx);
	}

	void Init(typename type, bool registerClientRPC = false)
	{
		if (!m_Settings.Contains(type))
		{
			auto setting = ExpansionSettingBase.Cast(type.Spawn());
			m_Settings.Insert(type, setting);
			m_SettingsOrdered.Insert(setting);

			if (registerClientRPC)
				RegisterClientRPC(type);

			OnInit(type);
		}
	}

	void OnInit(typename type)
	{
	}

	void GameInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this);
#endif
		
		if (m_GameInit)
		{
			Unload();

			m_GameInit = false;
		}
		else
		{
			m_GameInit = true;
		}

		Init();
	}

	void Send( notnull PlayerIdentity identity )
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this);
#endif
		
		if ( IsMissionClient() )
			return;

		foreach (ExpansionSettingBase setting: m_SettingsOrdered)
		{
			setting.Send(identity);
		}
	}

	void RPC_CoreSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionCoreSettings, ctx);
	}

	void RPC_MonitoringSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionMonitoringSettings, ctx);
	}

	void RPC_NotificationSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionNotificationSettings, ctx);
	}

	void Receive(typename type, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this, type.ToString());
#endif
		
		if (g_Game.IsDedicatedServer())
			return;

		Init(type);
		Expansion_Assert_False(m_Settings[type].OnRecieve(ctx), "Failed reading " + type.ToString() );
	}

	void Save()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Save");
#endif

		if ( IsMissionHost() && g_Game.IsMultiplayer() )
		{
			foreach (ExpansionSettingBase setting: m_SettingsOrdered)
			{
				setting.Save();
			}
		}
	}
	
	ExpansionSettingBase Get(typename type, bool checkLoaded = true)
	{
		auto setting = m_Settings[type];
		if (setting && !setting.IsLoaded())
		{
			if (checkLoaded && (g_Game.IsDedicatedServer() || !g_Game.GetMission() || (g_Game.GetMission().IsMissionGameplay() && g_Game.IsMultiplayer()) || (GetDayZGame().IsLoading() && !GetDayZGame().Expansion_IsMissionMainMenu() && !GetDayZGame().Expansion_IsMissionSinglePlayer())))
				WarnNotLoaded(type);

			if (!setting.IsUsingDefaults() && (g_Game.IsDedicatedServer() || GetDayZGame().Expansion_IsMissionSinglePlayer()))
			{
				//! IMPORTANT: Only use defaults if dedicated server or SP!
				EXTrace.Print(true, type, "Using defaults");
				setting.Defaults();
				setting.SetIsUsingDefaults();
				EnScript.SetClassVar(setting, "m_IsLoaded", 0, true);
			}
		}

		return setting;
	}

	private void WarnNotLoaded(typename type)
	{
		int count;
		if (!m_Warnings.Find(type, count))
			m_Warnings.Insert(type, 1);
		else
			m_Warnings[type] = ++count;

		if (count % 10 == 0)
		{
			string msg = "Trying to access " + type.ToString() + " before it has been ";

			if (g_Game.IsDedicatedServer() || GetDayZGame().Expansion_IsMissionSinglePlayer())
				msg += "loaded!";
			else
				msg += "received!";

			if (count)
			{
				EXError.Warn(this, "WARNING: " + msg + " (there have been " + count + " more suppressed warnings)");
			}
			else
			{
				Error("ERROR: " + msg);
			}

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(true, null, "Dedicated server: " + g_Game.IsDedicatedServer());
			EXTrace.Print(true, null, "Is mission singleplayer: " + GetDayZGame().Expansion_IsMissionSinglePlayer());
			EXTrace.Print(true, null, "Is multiplayer: " + g_Game.IsMultiplayer());
			EXTrace.Print(true, null, "Is loading: " + GetDayZGame().IsLoading());
			EXTrace.Print(true, null, "Mission: " + g_Game.GetMission());
			if (g_Game.GetMission())
				EXTrace.Print(true, null, "Is mission gameplay: " + g_Game.GetMission().IsMissionGameplay());
			EXTrace.Print(true, null, "Is mission main menu: " + GetDayZGame().Expansion_IsMissionMainMenu());
		#endif
		}
	}

	ExpansionCoreSettings GetCore(bool checkLoaded = true)
	{
		return ExpansionCoreSettings.Cast(Get(ExpansionCoreSettings, checkLoaded));
	}

	ExpansionDebugSettings GetDebug(bool checkLoaded = true)
	{
		EXError.ErrorOnce(this, "DEPRECATED, use GetExpansionSettings().GetCore() instead");
		return ExpansionDebugSettings.Cast(Get(ExpansionDebugSettings, checkLoaded));
	}

	ExpansionLogSettings GetLog(bool checkLoaded = true)
	{
		return ExpansionLogSettings.Cast(Get(ExpansionLogSettings, checkLoaded));
	}

	ExpansionSafeZoneSettings GetSafeZone(bool checkLoaded = true)
	{
		return ExpansionSafeZoneSettings.Cast(Get(ExpansionSafeZoneSettings, checkLoaded));
	}

	ExpansionNotificationSettings GetNotification(bool checkLoaded = true)
	{
		return ExpansionNotificationSettings.Cast(Get(ExpansionNotificationSettings, checkLoaded));
	}

	ExpansionMonitoringSettings GetMonitoring(bool checkLoaded = true)
	{
		return ExpansionMonitoringSettings.Cast(Get(ExpansionMonitoringSettings, checkLoaded));
	}

	ExpansionDamageSystemSettings GetDamageSystem(bool checkLoaded = true)
	{
		return ExpansionDamageSystemSettings.Cast(Get(ExpansionDamageSystemSettings, checkLoaded));
	}
};

static ref ExpansionSettings g_exGlobalSettings;

static ExpansionSettings GetExpansionSettings()
{
	if ( g_exGlobalSettings == NULL )
	{
		g_exGlobalSettings = new ExpansionSettings();
#ifdef SERVER
		g_exGlobalSettings.ServerInit();
#else
		g_exGlobalSettings.GameInit();
#endif
	}

	return g_exGlobalSettings;
}