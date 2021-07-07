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

/**@class		ExpansionGeneralSettings
 * @brief		General settings class
 **/
class ExpansionGeneralSettings: ExpansionSettingBase
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
	
	int SystemChatColor;
	int AdminChatColor;
	int GlobalChatColor;
	int DirectChatColor;
	int TransportChatColor;
	int PartyChatColor;
	int TransmitterChatColor;
	
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
		ref ExpansionGeneralSettings thisSetting = this;

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
		
		ref ExpansionGeneralSettings settings = ExpansionGeneralSettings.Cast( setting );
		if ( !settings )
			return false;

		CopyInternal( settings );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Send - End and return");
		#endif
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionGeneralSettings s )
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
		SystemChatColor = s.SystemChatColor;
		AdminChatColor = s.AdminChatColor;
		GlobalChatColor = s.GlobalChatColor;
		DirectChatColor = s.DirectChatColor;
		TransportChatColor = s.TransportChatColor;
		PartyChatColor = s.PartyChatColor;
		TransmitterChatColor = s.TransmitterChatColor;
		
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
		
		if ( FileExist( EXPANSION_GENERAL_SETTINGS ) )
		{
			Print("[ExpansionGeneralSettings] Loading settings");

			JsonFileLoader<ExpansionGeneralSettings>.JsonLoadFile( EXPANSION_GENERAL_SETTINGS, this );
			
			UpdateChatColors();
	
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionGeneralSettings::Load - End - Loaded");
			#endif

			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Load - End - Not Loaded");
		#endif
		return false;
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
	
	override string SettingName()
	{
		return "General Settings";
	}
};