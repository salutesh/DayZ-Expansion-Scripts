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

/**@class		ExpansionGeneralSettings
 * @brief		General settings class
 **/
class ExpansionGeneralSettings: ExpansionSettingBase
{
	static const int VERSION = 14;

	bool DisableShootToUnlock;
	bool EnableGravecross;
	bool GravecrossDeleteBody;
	float GravecrossTimeThreshold;
	float GravecrossSpawnTimeDelay;
	ref ExpansionMapping Mapping;
	LampModeEnum EnableLamps;										//! 0 - Disable Street-Lights | 1 - Will use and spawn Generators for Street-Lights | 2 - Street-Lights always on
	bool EnableGenerators;
	bool EnableLighthouses;
	bool EnableHUDNightvisionOverlay;
	bool DisableMagicCrosshair;
	bool EnableAutoRun;
	bool UseDeathScreen;
	bool UseDeathScreenStatistics;
	bool UseExpansionMainMenuLogo;
	bool UseExpansionMainMenuIcons;
	bool UseExpansionMainMenuIntroScene;
	bool UseNewsFeedInGameMenu;

	bool UseHUDColors;
	ref ExpansionHudIndicatorColors HUDColors;
	
	bool EnableEarPlugs;
	string InGameMenuLogoPath;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionGeneralSettings()
	{
		HUDColors = new ExpansionHudIndicatorColors;
		Mapping = new ExpansionMapping;
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnRecieve");
#endif

		GetDayZGame().GetExpansionGame().ReadRemovedWorldObjects(ctx);

		int interiorCount;
		if (!ctx.Read(interiorCount))
			Error("Couldn't read interior count");
		Mapping.BuildingInteriors = interiorCount;
		if (Mapping.Interiors.Count())
			EXPrint(this, "Clearing previous interiors: " + Mapping.Interiors.Count());
		Mapping.Interiors.Clear();
		while (interiorCount)
		{
			string interior;
			if (!ctx.Read(interior))
				Error("Couldn't read interior");
			Mapping.Interiors.Insert(interior);
			interiorCount--;
		}

		if (!ctx.Read(Mapping.BuildingIvys))
			Error("Couldn't read BuildingIvys");

		if (!ctx.Read(EnableLamps))
			Error("Couldn't read EnableLamps");
		if (!ctx.Read(EnableGenerators))
			Error("Couldn't read EnableGenerators");
		if (!ctx.Read(EnableLighthouses))
			Error("Couldn't read EnableLighthouses");
		if (!ctx.Read(EnableHUDNightvisionOverlay))
			Error("Couldn't read EnableHUDNightvisionOverlay");
		if (!ctx.Read(DisableMagicCrosshair))
			Error("Couldn't read DisableMagicCrosshair");
		if (!ctx.Read(EnableAutoRun))
			Error("Couldn't read EnableAutoRun");
		if (!ctx.Read(UseDeathScreen))
			Error("Couldn't read UseDeathScreen");
		if (!ctx.Read(UseDeathScreenStatistics))
			Error("Couldn't read UseDeathScreenStatistics");
		if (!ctx.Read(UseExpansionMainMenuLogo))
			Error("Couldn't read UseExpansionMainMenuLogo");
		if (!ctx.Read(UseExpansionMainMenuIcons))
			Error("Couldn't read UseExpansionMainMenuIcons");
		if (!ctx.Read(UseExpansionMainMenuIntroScene))
			Error("Couldn't read UseExpansionMainMenuIntroScene");
		if (!ctx.Read(UseNewsFeedInGameMenu))
			Error("Couldn't read UseNewsFeedInGameMenu");

		if (!ctx.Read(UseHUDColors))
			Error("Couldn't read UseHUDColors");
		if (UseHUDColors)
			HUDColors.OnReceive(ctx);
		HUDColors.Update();
		
		if (!ctx.Read(EnableEarPlugs))
			Error("Couldn't read EnableEarPlugs");
		if (!ctx.Read(InGameMenuLogoPath))
			Error("Couldn't read InGameMenuLogoPath");

		m_IsLoaded = true;

		ExpansionSettings.SI_General.Invoke();
		
		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		GetDayZGame().GetExpansionGame().WriteRemovedWorldObjects(ctx);

		//! Do not send mapping, but do send interiors if enabled as interiors w/o collision are created only clientside
		if (Mapping.BuildingInteriors)
		{
			ctx.Write(Mapping.Interiors.Count());
			foreach (string interior: Mapping.Interiors)
			{
				ctx.Write(interior);
			}
		}
		else
		{
			ctx.Write(0);
		}

		//! Ivys are created only clientside
		ctx.Write(Mapping.BuildingIvys);

		ctx.Write(EnableLamps);
		ctx.Write(EnableGenerators);
		ctx.Write(EnableLighthouses);
		ctx.Write(EnableHUDNightvisionOverlay);
		ctx.Write(DisableMagicCrosshair);
		ctx.Write(EnableAutoRun);
		ctx.Write(UseDeathScreen);
		ctx.Write(UseDeathScreenStatistics);
		ctx.Write(UseExpansionMainMenuLogo);
		ctx.Write(UseExpansionMainMenuIcons);
		ctx.Write(UseExpansionMainMenuIntroScene);
		ctx.Write(UseNewsFeedInGameMenu);

		ctx.Write(UseHUDColors);
		if (UseHUDColors)
			HUDColors.OnSend(ctx);
		
		ctx.Write(EnableEarPlugs);
		ctx.Write(InGameMenuLogoPath);
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
		
		auto rpc = CreateRPC();
		OnSend( rpc );
		rpc.Expansion_Send(true, identity);
		
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
		
		EnableGravecross = s.EnableGravecross;
		DisableShootToUnlock = s.DisableShootToUnlock;
		GravecrossDeleteBody = s.GravecrossDeleteBody;
		GravecrossTimeThreshold = s.GravecrossTimeThreshold;
		GravecrossSpawnTimeDelay = s.GravecrossSpawnTimeDelay;
		Mapping.Copy( s.Mapping );
		EnableLamps = s.EnableLamps;
		EnableGenerators = s.EnableGenerators;
		EnableLighthouses = s.EnableLighthouses;
		EnableHUDNightvisionOverlay = s.EnableHUDNightvisionOverlay;
		DisableMagicCrosshair = s.DisableMagicCrosshair;
		EnableAutoRun = s.EnableAutoRun;
		UseDeathScreen = s.UseDeathScreen;
		UseDeathScreenStatistics = s.UseDeathScreenStatistics;
		UseNewsFeedInGameMenu = s.UseNewsFeedInGameMenu;
		HUDColors = s.HUDColors;
		EnableEarPlugs = s.EnableEarPlugs;
		InGameMenuLogoPath = s.InGameMenuLogoPath;
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

			JsonFileLoader<ExpansionGeneralSettings>.JsonLoadFile(EXPANSION_GENERAL_SETTINGS, this);

			HUDColors.Update();

			if (m_Version < VERSION)
			{
				EXPrint("[ExpansionGeneralSettings] Load - Converting v" + m_Version + " \"" + EXPANSION_GENERAL_SETTINGS + "\" to v" + VERSION);

				if (m_Version < 7)
				{
					//! Remove buildings without custom interiors from existing settings
					Mapping.Interiors.RemoveItem( "Land_City_School" );
					Mapping.Interiors.RemoveItem( "Land_Village_Pub" );
					Mapping.Interiors.RemoveItem( "Land_House_1B01_Pub" );
					Mapping.Interiors.RemoveItem( "Land_House_1W03" );
					Mapping.Interiors.RemoveItem( "Land_House_1W04" );
					Mapping.Interiors.RemoveItem( "Land_House_1W05" );
					Mapping.Interiors.RemoveItem( "Land_House_1W08" );
					Mapping.Interiors.RemoveItem( "Land_House_1W09" );
					Mapping.Interiors.RemoveItem( "Land_House_1W10" );
					Mapping.Interiors.RemoveItem( "Land_House_1W12" );
					Mapping.Interiors.RemoveItem( "Land_House_1W03_Brown" );
					Mapping.Interiors.RemoveItem( "Land_House_1W04_Yellow" );
					Mapping.Interiors.RemoveItem( "Land_House_1W05_Yellow" );
					Mapping.Interiors.RemoveItem( "Land_House_1W08_Brown" );
					Mapping.Interiors.RemoveItem( "Land_House_1W09_Yellow" );
					Mapping.Interiors.RemoveItem( "Land_House_1W10_Brown" );
					Mapping.Interiors.RemoveItem( "Land_House_1W12_Brown" );
					Mapping.Interiors.RemoveItem( "Land_House_2W04" );
					Mapping.Interiors.RemoveItem( "Land_House_2W03_Brown" );
					Mapping.Interiors.RemoveItem( "Land_House_2W04_Yellow" );
					Mapping.Interiors.RemoveItem( "Land_Lighthouse" );
					Mapping.Interiors.RemoveItem( "Land_Power_Station" );
				}

				if (m_Version < 8)
				{
					HUDColors = settingsDefault.HUDColors;
				}
				
				if (m_Version < 9)
				{
					EnableEarPlugs = settingsDefault.EnableEarPlugs;
					InGameMenuLogoPath = settingsDefault.InGameMenuLogoPath;
				}

				if (m_Version < 10)
				{
					GravecrossSpawnTimeDelay = settingsDefault.GravecrossSpawnTimeDelay;
				}

				if (m_Version < 11)
				{
					UseExpansionMainMenuLogo = settingsDefault.UseExpansionMainMenuLogo;
					UseExpansionMainMenuIcons = settingsDefault.UseExpansionMainMenuIcons;
					UseExpansionMainMenuIntroScene = settingsDefault.UseExpansionMainMenuIntroScene;
				}

				if (m_Version < 14)
				{
					foreach (string name, int color: HUDColors.m_Colors)
					{
						if (color != settingsDefault.HUDColors.m_Colors[name])
						{
							EXPrint(this, "Using custom HUD colors");
							UseHUDColors = true;
							break;
						}
					}
				}

				m_Version = VERSION;
				save = true;
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
		
		DisableShootToUnlock = false;
		EnableGravecross = false;
		GravecrossDeleteBody = true;
		GravecrossTimeThreshold = 1200;
		GravecrossSpawnTimeDelay = 180;
		
		Mapping.Defaults();

		EnableLamps = LampModeEnum.AlwaysOnEverywhere;
		EnableGenerators = false;
		EnableLighthouses = true;
		
		EnableHUDNightvisionOverlay = true;

		DisableMagicCrosshair = true;
		
		EnableAutoRun = true;
		
		UseDeathScreen = true;
		UseDeathScreenStatistics = true;
		
		UseExpansionMainMenuLogo = true;
		UseExpansionMainMenuIcons = true;
		UseExpansionMainMenuIntroScene = true;
		UseNewsFeedInGameMenu = true;

		UseHUDColors = false;
		HUDColors.Update();
		
		EnableEarPlugs = true;
		InGameMenuLogoPath = "set:expansion_iconset image:logo_expansion_white";
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "General Settings";
	}
};
