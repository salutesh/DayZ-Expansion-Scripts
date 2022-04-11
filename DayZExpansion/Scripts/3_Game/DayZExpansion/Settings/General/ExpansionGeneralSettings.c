/**
 * ExpansionGeneralSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionGeneralSettingsBase
 * @brief		General settings base class
 **/
class ExpansionGeneralSettingsBase: ExpansionSettingBase
{
	bool PlayerLocationNotifier;
	bool EnableGlobalChat;
	bool EnablePartyChat;
	bool EnableTransportChat;
	bool DisableShootToUnlock;
	bool EnableGravecross;
	bool GravecrossDeleteBody;
	int GravecrossTimeThreshold;
	ref ExpansionMapping Mapping;
	LampModeEnum EnableLamps;										//! 0 - Disable Street-Lights | 1 - Will use and spawn Generators for Street-Lights | 2 - Street-Lights always on
	bool EnableGenerators;
	bool EnableLighthouses;
	bool EnableHUDNightvisionOverlay;
	bool DisableMagicCrosshair;
	bool EnableAutoRun;
	bool UnlimitedStamina;
	bool UseDeathScreen;
	bool UseDeathScreenStatistics;
	bool UseNewsFeedInGameMenu;
}

/**@class		ExpansionGeneralSettings
 * @brief		General settings class
 **/
class ExpansionGeneralSettings: ExpansionGeneralSettingsBase
{
	static const int VERSION = 5;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionGeneralSettings()
	{
		Mapping = new ExpansionMapping;
	}

	// ------------------------------------------------------------
	void ~ExpansionGeneralSettings()
	{
		delete Mapping;
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnRecieve");
#endif

		ctx.Read(PlayerLocationNotifier);
		ctx.Read(EnableGlobalChat);
		ctx.Read(EnablePartyChat);
		ctx.Read(EnableTransportChat);
		ctx.Read(DisableShootToUnlock);
		ctx.Read(EnableGravecross);
		ctx.Read(GravecrossDeleteBody);
		ctx.Read(GravecrossTimeThreshold);
		ctx.Read(EnableLamps);
		ctx.Read(EnableGenerators);
		ctx.Read(EnableLighthouses);
		ctx.Read(EnableHUDNightvisionOverlay);
		ctx.Read(DisableMagicCrosshair);
		ctx.Read(EnableAutoRun);
		ctx.Read(UnlimitedStamina);
		ctx.Read(UseDeathScreen);
		ctx.Read(UseDeathScreenStatistics);
		ctx.Read(UseNewsFeedInGameMenu);

		m_IsLoaded = true;

		ExpansionSettings.SI_General.Invoke();
		
		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write(PlayerLocationNotifier);
		ctx.Write(EnableGlobalChat);
		ctx.Write(EnablePartyChat);
		ctx.Write(EnableTransportChat);
		ctx.Write(DisableShootToUnlock);
		ctx.Write(EnableGravecross);
		ctx.Write(GravecrossDeleteBody);
		ctx.Write(GravecrossTimeThreshold);
		//! Do not send mapping
		ctx.Write(EnableLamps);
		ctx.Write(EnableGenerators);
		ctx.Write(EnableLighthouses);
		ctx.Write(EnableHUDNightvisionOverlay);
		ctx.Write(DisableMagicCrosshair);
		ctx.Write(EnableAutoRun);
		ctx.Write(UnlimitedStamina);
		ctx.Write(UseDeathScreen);
		ctx.Write(UseDeathScreenStatistics);
		ctx.Write(UseNewsFeedInGameMenu);
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.General, true, identity );
		
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionGeneralSettings settings = ExpansionGeneralSettings.Cast( setting );
		if ( !settings )
			return false;

		CopyInternal( settings );
		
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionGeneralSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		ExpansionGeneralSettingsBase sb = s;
		CopyInternal( sb );
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionGeneralSettingsBase s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif
		
		PlayerLocationNotifier = s.PlayerLocationNotifier;
		EnableGlobalChat = s.EnableGlobalChat;
		EnablePartyChat = s.EnablePartyChat;
		EnableTransportChat = s.EnableTransportChat;
		EnableGravecross = s.EnableGravecross;
		DisableShootToUnlock = s.DisableShootToUnlock;
		GravecrossDeleteBody = s.GravecrossDeleteBody;
		GravecrossTimeThreshold = s.GravecrossTimeThreshold;
		Mapping.Copy( s.Mapping );
		EnableLamps = s.EnableLamps;
		EnableGenerators = s.EnableGenerators;
		EnableLighthouses = s.EnableLighthouses;
		EnableHUDNightvisionOverlay = s.EnableHUDNightvisionOverlay;
		DisableMagicCrosshair = s.DisableMagicCrosshair;
		EnableAutoRun = s.EnableAutoRun;
		UnlimitedStamina = s.UnlimitedStamina;
		UseDeathScreen = s.UseDeathScreen;
		UseDeathScreenStatistics = s.UseDeathScreenStatistics;
		UseNewsFeedInGameMenu = s.UseNewsFeedInGameMenu;
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;

		bool generalSettingsExist = FileExist(EXPANSION_GENERAL_SETTINGS);

		if (generalSettingsExist)
		{
			EXPrint("[ExpansionGeneralSettings] Load existing setting file:" + EXPANSION_GENERAL_SETTINGS);
			
			ExpansionGeneralSettings settingsDefault = new ExpansionGeneralSettings;
			settingsDefault.Defaults();

			ExpansionGeneralSettingsBase settingsBase;

			JsonFileLoader<ExpansionGeneralSettingsBase>.JsonLoadFile(EXPANSION_GENERAL_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionGeneralSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_GENERAL_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 0)
				{
					//! Placeholder
				}
				else
				{
					JsonFileLoader<ExpansionGeneralSettings>.JsonLoadFile(EXPANSION_GENERAL_SETTINGS, this);
				}

				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionGeneralSettings>.JsonLoadFile(EXPANSION_GENERAL_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionGeneralSettings] No existing setting file:" + EXPANSION_GENERAL_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		return generalSettingsExist;
	}
	
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionGeneralSettings] Saving settings");

		JsonFileLoader<ExpansionGeneralSettings>.JsonSaveFile( EXPANSION_GENERAL_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_General.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		PlayerLocationNotifier = true;
		
		EnableGlobalChat = true;
		EnablePartyChat = true;
		EnableTransportChat = true;
		DisableShootToUnlock = false;
		EnableGravecross = false;
		GravecrossDeleteBody = true;
		GravecrossTimeThreshold = 1200;
		
		Mapping.Defaults();

		EnableLamps = LampModeEnum.AlwaysOnEverywhere;
		EnableGenerators = false;
		EnableLighthouses = true;
		
		EnableHUDNightvisionOverlay = true;

		DisableMagicCrosshair = true;
		
		EnableAutoRun = true;
		UnlimitedStamina = false;
		
		UseDeathScreen = true;
		UseDeathScreenStatistics = true;
		
		UseNewsFeedInGameMenu = true;
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "General Settings";
	}
};
