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
	static ref ScriptInvoker SI_DamageSystem = new ScriptInvoker();

	protected bool m_GameInit = false;

	protected ref map<typename, ref ExpansionSettingBase> m_Settings = new map<typename, ref ExpansionSettingBase>;
	protected autoptr array<ExpansionSettingBase> m_SettingsOrdered = new array<ExpansionSettingBase>;
	protected ref map<typename, int> m_Warnings = new map<typename, int>;

	void ServerInit()
	{
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this);

		Init();

		foreach (ExpansionSettingBase setting: m_SettingsOrdered)
		{
			Load(setting);
		}
	}

	void Unload()
	{
		auto trace = EXTrace.StartStack(EXTrace.SETTINGS, this);

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
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this);

		Init(ExpansionDebugSettings);
		Init(ExpansionLogSettings);
		Init(ExpansionSafeZoneSettings);
		Init(ExpansionNotificationSettings);
		Init(ExpansionMonitoringSettings);
		Init(ExpansionDamageSystemSettings);
	}

	void Init(typename type)
	{
		if (!m_Settings.Contains(type))
		{
			auto setting = ExpansionSettingBase.Cast(type.Spawn());
			m_Settings.Insert(type, setting);
			m_SettingsOrdered.Insert(setting);
		}
	}

	void GameInit()
	{
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this);

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if ( IsMissionClient() )
			return;

		foreach (ExpansionSettingBase setting: m_SettingsOrdered)
		{
			setting.Send(identity);
		}
	}

	bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnRPC");
#endif

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.Debug:
			{
				Receive(ExpansionDebugSettings, ctx);
				return true;
			}
			
			case ExpansionSettingsRPC.Notification:
			{
				Receive(ExpansionNotificationSettings, ctx);
				return true;
			}
		}

		return false;
	}

	void Receive(typename type, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.SETTINGS, this, type.ToString());

		if (GetGame().IsDedicatedServer())
			return;

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		Init(type);
		Expansion_Assert_False(m_Settings[type].OnRecieve(ctx), "Failed reading " + type.ToString() );
	}

	void Save()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Save");
#endif

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
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
			if (checkLoaded && (GetGame().IsDedicatedServer() || !GetGame().GetMission() || (GetGame().GetMission().IsMissionGameplay() && GetGame().IsMultiplayer()) || (GetDayZGame().IsLoading() && !GetDayZGame().Expansion_IsMissionMainMenu())))
				WarnNotLoaded(type);

			if (!setting.IsUsingDefaults())
			{
				EXTrace.Print(true, type, "Using defaults");
				setting.Defaults();
				setting.SetIsUsingDefaults();
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
			string suffix;
			if (count)
				suffix = " (there have been " + count + " more suppressed warnings)";
			if (GetGame().IsServer())
				EXTrace.Print(true, null, "WARNING: Trying to access " + type.ToString() + " before it has been loaded!" + suffix);
			else
				EXTrace.Print(true, null, "WARNING: Trying to access " + type.ToString() + " before it has been received!" + suffix);
		#ifdef DIAG
			EXTrace.Print(true, null, "Dedicated server: " + GetGame().IsDedicatedServer());
			EXTrace.Print(true, null, "Mission: " + GetGame().GetMission());
			if (GetGame().GetMission())
				EXTrace.Print(true, null, "Is mission gameplay: " + GetGame().GetMission().IsMissionGameplay());
			EXTrace.Print(true, null, "Is multiplayer: " + GetGame().IsMultiplayer());
			EXTrace.Print(true, null, "Is loading: " + GetDayZGame().IsLoading());
			EXTrace.Print(true, null, "Is mission main menu: " + GetDayZGame().Expansion_IsMissionMainMenu());
		#endif
			EXTrace trace = EXTrace.StartStack(true, this);
			trace.SetStart(5);
		}
	}

	ExpansionDebugSettings GetDebug(bool checkLoaded = true)
	{
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