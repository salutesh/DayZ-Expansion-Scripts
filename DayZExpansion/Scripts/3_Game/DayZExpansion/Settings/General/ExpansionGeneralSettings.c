/**
 * ExpansionGeneralSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	int EnableLamps;										//! 0 - Disable Street-Lights | 1 - Will use and spawn Generators for Street-Lights | 2 - Street-Lights always on
	bool EnableGenerators;
	bool EnableLighthouses;
	bool EnableHUDNightvisionOverlay;
	bool DisableMagicCrosshair;
	bool EnablePlayerTags;
	int PlayerTagViewRange;
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
	static const int VERSION = 2;
	
	int SystemChatColor;			//! Added with v2
	int AdminChatColor;			//! Added with v2
	int GlobalChatColor;			//! Added with v2
	int DirectChatColor;			//! Added with v2
	int TransportChatColor;		//! Added with v2
	int PartyChatColor;				//! Added with v2
	int TransmitterChatColor;	//! Added with v2
	
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::OnRecieve - Start");
		#endif
		
		ExpansionGeneralSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionGeneralSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		UpdateChatColors();

		m_IsLoaded = true;

		ExpansionSettings.SI_General.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionGeneralSettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.General, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Copy - Start");
		#endif
		
		ExpansionGeneralSettings settings = ExpansionGeneralSettings.Cast( setting );
		if ( !settings )
			return false;

		CopyInternal( settings );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Send - End and return");
		#endif
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionGeneralSettings s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::CopyInternal - Start");
		#endif
		
		//! Added with v2
		SystemChatColor = s.SystemChatColor;
		AdminChatColor = s.AdminChatColor;
		GlobalChatColor = s.GlobalChatColor;
		DirectChatColor = s.DirectChatColor;
		TransportChatColor = s.TransportChatColor;
		PartyChatColor = s.PartyChatColor;
		TransmitterChatColor = s.TransmitterChatColor;
		
		ExpansionGeneralSettingsBase sb = s;
		CopyInternal( sb );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::CopyInternal - End");
		#endif
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionGeneralSettingsBase s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::CopyInternal - Start");
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
		EnablePlayerTags = s.EnablePlayerTags;
		PlayerTagViewRange = s.PlayerTagViewRange;
		EnableAutoRun = s.EnableAutoRun;
		UnlimitedStamina = s.UnlimitedStamina;
		UseDeathScreen = s.UseDeathScreen;
		UseDeathScreenStatistics = s.UseDeathScreenStatistics;
		UseNewsFeedInGameMenu = s.UseNewsFeedInGameMenu;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::CopyInternal - End");
		#endif
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		bool save;

		bool generalSettingsExist = FileExist(EXPANSION_GENERAL_SETTINGS);

		if (generalSettingsExist)
		{
			ExpansionGeneralSettings settingsDefault = new ExpansionGeneralSettings;
			settingsDefault.Defaults();

			ExpansionGeneralSettingsBase settingsBase;

			JsonFileLoader<ExpansionGeneralSettingsBase>.JsonLoadFile(EXPANSION_GENERAL_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 2)
				{
					EXPrint("[ExpansionGeneralSettings] Load - Converting v1 \"" + EXPANSION_GENERAL_SETTINGS + "\" to v" + VERSION);
					
					//! Added with v2
					SystemChatColor = settingsDefault.SystemChatColor;
					AdminChatColor = settingsDefault.AdminChatColor;
					GlobalChatColor = settingsDefault.GlobalChatColor;
					DirectChatColor = settingsDefault.DirectChatColor;
					TransportChatColor = settingsDefault.TransportChatColor;
					PartyChatColor = settingsDefault.PartyChatColor;
					TransmitterChatColor = settingsDefault.TransmitterChatColor;
					
					UpdateChatColors();
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
			Defaults();
			UpdateChatColors();
			save = true;
		}
		
		if (save)
			Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Load - End - Loaded: " + bookSettingsExist);
		#endif
		
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
		Print("[ExpansionGeneralSettings] Loading default settings");
		
		PlayerLocationNotifier = true;
		
		EnableGlobalChat = true;
		EnablePartyChat = true;
		EnableTransportChat = true;
		DisableShootToUnlock = false;
		EnableGravecross = false;
		GravecrossDeleteBody = true;
		GravecrossTimeThreshold = 1200;
		
		Mapping.Defaults();

		EnableLamps = 3;						//! 0 - Disable Street-Lights | 1 - Will use and spawn Generators for Street-Lights | 2 - Street-Lights always on | 3 - Street-Lights always on everywhere
		EnableGenerators = false;
		EnableLighthouses = true;
		
		EnableHUDNightvisionOverlay = true;

		DisableMagicCrosshair = true;
		EnablePlayerTags = true;
		PlayerTagViewRange = 5;

		EnableAutoRun = true;
		UnlimitedStamina = false;
		
		UseDeathScreen = true;
		UseDeathScreenStatistics = true;
		
		UseNewsFeedInGameMenu = true;
		
		UpdateChatColors();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionGeneralSettings] Default settings loaded!");
		#endif
	}

	void UpdateChatColors()
	{
		//! Make sure none of the colors are zero (all transparent)
		if (!SystemChatColor)
			SystemChatColor = ARGB(255, 186, 69, 186);
		if (!AdminChatColor)
			AdminChatColor = ARGB(255, 192, 57, 43);
		if (!GlobalChatColor)
			GlobalChatColor = ARGB(255, 88, 195, 247);
		if (!DirectChatColor)
			DirectChatColor = ARGB(255, 255, 255, 255);
		if (!TransportChatColor)
			TransportChatColor = ARGB(255, 255, 206, 9);
		if (!PartyChatColor)
			PartyChatColor = ARGB(255, 10, 250, 122);
		if (!TransmitterChatColor)
			TransmitterChatColor = ARGB(255, 249, 255, 73);
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "General Settings";
	}
};