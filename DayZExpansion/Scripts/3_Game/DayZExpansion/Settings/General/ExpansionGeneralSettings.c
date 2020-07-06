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
	bool DisableDamagedHeliSpin;
	
	
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
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::HandleRPC - Start");
		#endif
		
		ExpansionGeneralSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("Failed to read param");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_General.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::HandleRPC - End");
		#endif
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
		
		ref ExpansionGeneralSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
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
		DisableDamagedHeliSpin = s.DisableDamagedHeliSpin;
		
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Save - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionGeneralSettings>.JsonSaveFile( EXPANSION_GENERAL_SETTINGS, this );
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionGeneralSettings::Save - Settings saved!");
			#endif

			return true;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGeneralSettings::Save - End");
		#endif
		return false;
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionGeneralSettings] Loading default settings..");
		#endif
		
		PlayerLocationNotifier = true;
		
		EnableGlobalChat = true;

		Mapping.Defaults();

		EnableLamps = 3;						//! 0 - Disable Street-Lights | 1 - Will use and spawn Generators for Street-Lights | 2 - Street-Lights always on | 3 - Street-Lights always on everywhere
		EnableGenerators = false;
		EnableLighthouses = true;
		
		EnableHUDGPS = true;
		NeedGPSItemForKeyBinding = true;
		NeedMapItemForKeyBinding = false;
		
		EnableHUDNightvisionOverlay = true;


		EnablePlayerTags = true;
		PlayerTagViewRange = 5;
		DisableDamagedHeliSpin = true;

		EnablePlayerList = true;

		EnableAutoRun = true;
		UnlimitedStamina = false;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionGeneralSettings] Default settings loaded!");
		#endif
	}
}