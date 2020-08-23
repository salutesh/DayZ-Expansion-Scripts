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
	bool EnableGravecross;
	bool GravecrossDeleteBody;
	int GravecrossTimeThreshold;
	ref ExpansionMapping Mapping;

	int EnableLamps;										//! 0 - Disable Street-Lights | 1 - Will use and spawn Generators for Street-Lights | 2 - Street-Lights always on
	bool EnableGenerators;
	bool EnableLighthouses;

	bool EnableHUDGPS;
	bool NeedGPSItemForKeyBinding;
	bool NeedMapItemForKeyBinding;

	bool EnableHUDNightvisionOverlay;
	bool DisableMagicCrosshair;
	bool EnablePlayerTags;
	int PlayerTagViewRange;
	
	bool EnablePlayerList;

	bool EnableAutoRun;
	bool UnlimitedStamina;
	
	bool UseDeadScreen;
	bool UseDeadScreenStatistics;
	
	bool UseNeewsFeedInGameMenu;
	
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
			Error("Failed to read param");
			return false;
		}

		CopyInternal( setting );

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
		EnableGravecross = s.EnableGravecross;
		GravecrossDeleteBody = s.GravecrossDeleteBody;
		GravecrossTimeThreshold = s.GravecrossTimeThreshold;
		Mapping.Copy( s.Mapping );
		EnableLamps = s.EnableLamps;
		EnableGenerators = s.EnableGenerators;
		EnableLighthouses = s.EnableLighthouses;
		EnableHUDGPS = s.EnableHUDGPS;
		NeedGPSItemForKeyBinding = s.NeedGPSItemForKeyBinding;
		NeedMapItemForKeyBinding = s.NeedMapItemForKeyBinding;
		EnableHUDNightvisionOverlay = s.EnableHUDNightvisionOverlay;
		DisableMagicCrosshair = s.DisableMagicCrosshair;
		EnablePlayerTags = s.EnablePlayerTags;
		PlayerTagViewRange = s.PlayerTagViewRange;
		EnablePlayerList = s.EnablePlayerList;
		EnableAutoRun = s.EnableAutoRun;
		UnlimitedStamina = s.UnlimitedStamina;
		UseDeadScreen = s.UseDeadScreen;
		UseDeadScreenStatistics = s.UseDeadScreenStatistics;
		UseNeewsFeedInGameMenu = s.UseNeewsFeedInGameMenu;
		
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
		EnableGravecross = false;
		GravecrossDeleteBody = true;
		GravecrossTimeThreshold = 1200;
		
		Mapping.Defaults();

		EnableLamps = 3;						//! 0 - Disable Street-Lights | 1 - Will use and spawn Generators for Street-Lights | 2 - Street-Lights always on | 3 - Street-Lights always on everywhere
		EnableGenerators = false;
		EnableLighthouses = true;

		EnableHUDGPS = true;

		#ifdef EXPANSION_SETTINGS_MAP_ITEM_DEFAULT_DISABLE
		NeedGPSItemForKeyBinding = false;
		NeedMapItemForKeyBinding = false;
		#else
		NeedGPSItemForKeyBinding = true;
		NeedMapItemForKeyBinding = true;
		#endif
		
		EnableHUDNightvisionOverlay = true;

		DisableMagicCrosshair = true;
		EnablePlayerTags = true;
		PlayerTagViewRange = 5;

		EnablePlayerList = true;

		EnableAutoRun = true;
		UnlimitedStamina = false;
		
		UseDeadScreen = false;
		UseDeadScreenStatistics = false;
		
		UseNeewsFeedInGameMenu = true;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionGeneralSettings] Default settings loaded!");
		#endif
	}
}