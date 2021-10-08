/**
 * ExpansionBaseBuildingSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBaseBuildingSettingsBase
 * @brief		Base buidling settings base class
 **/
class ExpansionBaseBuildingSettingsBase: ExpansionSettingBase
{
	bool CanBuildAnywhere;														//! If enabled, allows players to build anywhere.
	bool AllowBuildingWithoutATerritory; 									//! 0 - You can build without territory, but nobody else can build near your territory. | 1 - You can only build if you are in your own territory.
	autoptr TStringArray DeployableOutsideATerritory;				//! List of items that you can place everywhere but not in enemy territory, only works when AllowBuildingWithoutATerritory is set to 1.
	autoptr TStringArray DeployableInsideAEnemyTerritory;		//! List of items that are allowed to place in enemy territory, only works when AllowBuildingWithoutATerritory is set to 1 or 0.
	bool CanCraftVanillaBasebuilding;										//! When enabled, allows players to craft Vanilla basebuilding.
	bool CanCraftExpansionBasebuilding;									//! When enabled, allows players to craft Expansion basebuilding.
	bool DestroyFlagOnDismantle;												//! When enabled, destroys both flag pole and the flag kit on dismantle.
	bool DismantleOutsideTerritory;											//! When enabled, allows players to dismantle basebuilding without territory.
	bool DismantleInsideTerritory;											//! When enabled, allows players to dismantle basebuilding inside another territory.
	bool DismantleAnywhere;													//! When enabled, allows players to dismantle basebuilding anywhere and not only on the soft side.
	bool CodelockActionsAnywhere;											//! If enabled, Allow the player to get code lock actions from anywhere on the target instead of the codelock selection.
	int CodeLockLength;															//! Lenght of pin code required for the code lock.
	bool DoDamageWhenEnterWrongCodeLock;							//! If enabled, deals the damage to the player when entering the wrong code lock.
	float DamageWhenEnterWrongCodeLock;							//! Amount of damage player takes when entering the wrong code in the code lock.
	bool RememberCode;														//! Remember code once entered correctly
	bool CanCraftTerritoryFlagKit;												//! When enabled this will allow the crafting of the territory flag kit.
	bool SimpleTerritory;															//! When enabled this will construct the full flag pole when a flag kit is deployed.
	bool AutomaticFlagOnCreation;											//! When enabled this will add the flag attachment to the flag pole once the territory pole has been constructed
	bool GetTerritoryFlagKitAfterBuild;										//! When enabled this will spawn a territory flag kit next to the full constructed flag pole.
}

/**@class		ExpansionBaseBuildingSettingsBaseV2
 * @brief		Base buidling settings base v2 class
 **/
class ExpansionBaseBuildingSettingsBaseV2: ExpansionBaseBuildingSettingsBase
{
	string BuildZoneRequiredCustomMessage;							//! Custom message for build zone required (optional, uses default message if not set)
	ref array < ref ExpansionBuildNoBuildZone > Zones;			//! "No build" or "build" zones
	bool ZonesAreNoBuildZones;											//! Whether the provided zones are "no build" zones (true = can not build in the zones) or "build" zones (false = can ONLY build in the zones)
}

/**@class		ExpansionBaseBuildingSettingsV2
 * @brief		Base buidling settings v2 class
 **/
class ExpansionBaseBuildingSettingsV2: ExpansionBaseBuildingSettingsBaseV2
{
	int DismantleFlagRequireTools;					//! -1 = only territory members, no tools needed  | 0 = anyone, no tools needed | 1 = anyone, only with tools
	int CanAttachCodelock;								//! 0 = only on Exp doors/gates | 1 = Exp doors/gates + vanilla tents | 2 = Exp doors/gates + vanilla fence (also works for BBP) | 3 = Exp doors/gates + vanilla fences (also works for BBP) & tents
	int EnableFlagMenu;											//! When enabled this allow to use the flag menu on territory flags.
}

/**@class		ExpansionBaseBuildingSettings
 * @brief		Base buidling settings class
 **/
class ExpansionBaseBuildingSettings: ExpansionBaseBuildingSettingsBaseV2
{
	static const int VERSION = 3;

	ExpansionCodelockAttachMode CodelockAttachMode;						//! 0 = only on Exp doors/gates | 1 = Exp doors/gates + vanilla fences (also works for BBP) | 2 = Exp doors/gates + vanilla fence (also works for BBP) & tents | 3 = Exp doors/gates + vanilla tents
	ExpansionDismantleFlagMode DismantleFlagMode;					//! -1 = only territory members, no tools needed  | 0 = anyone, no tools needed | 1 = anyone, only with tools
	ExpansionFlagMenuMode FlagMenuMode;											//! 0 = no flag menu | 1 = disabled | 2 = no flag choice
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionBaseBuildingSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::ExpansionBaseBuildingSettings - Start");
		#endif
		
		DeployableOutsideATerritory = new TStringArray;
		DeployableInsideAEnemyTerritory = new TStringArray;

		Zones =  new array< ref ExpansionBuildNoBuildZone >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::ExpansionBaseBuildingSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::OnRecieve - Start");
		#endif
		
		ExpansionBaseBuildingSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionBaseBuildingSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );
		
		m_IsLoaded = true;

		ExpansionSettings.SI_BaseBuilding.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionBaseBuildingSettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.BaseBuilding, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::Send - End and return");
		#endif

		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionBaseBuildingSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionBaseBuildingSettings s )
	{
		DismantleFlagMode = s.DismantleFlagMode;
		CodelockAttachMode = s.CodelockAttachMode;
		FlagMenuMode = s.FlagMenuMode;

		BuildZoneRequiredCustomMessage = s.BuildZoneRequiredCustomMessage;
		
		Zones.Clear();
		for (int i = 0; i < s.Zones.Count(); i++)
		{
			Zones.Insert( s.Zones[i] );
		}
		
		ZonesAreNoBuildZones = s.ZonesAreNoBuildZones;
		
		ExpansionBaseBuildingSettingsBase sb = s;
		CopyInternal( sb );
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionBaseBuildingSettingsBase s )
	{
		CanBuildAnywhere = s.CanBuildAnywhere;
		AllowBuildingWithoutATerritory = s.AllowBuildingWithoutATerritory;
		
		DeployableOutsideATerritory.Clear();
		for (int i = 0; i < s.DeployableOutsideATerritory.Count(); i++)
		{
			DeployableOutsideATerritory.Insert( s.DeployableOutsideATerritory[i] );
		}
		
		DeployableInsideAEnemyTerritory.Clear();
		for (i = 0; i < s.DeployableInsideAEnemyTerritory.Count(); i++)
		{
			DeployableInsideAEnemyTerritory.Insert( s.DeployableInsideAEnemyTerritory[i] );
		}

		CanCraftVanillaBasebuilding = s.CanCraftVanillaBasebuilding;
		CanCraftExpansionBasebuilding = s.CanCraftExpansionBasebuilding;
		DestroyFlagOnDismantle = s.DestroyFlagOnDismantle;
		DismantleOutsideTerritory = s.DismantleOutsideTerritory;
		DismantleInsideTerritory = s.DismantleInsideTerritory;
		DismantleAnywhere = s.DismantleAnywhere;
		
		CodelockActionsAnywhere = s.CodelockActionsAnywhere;
		CodeLockLength = s.CodeLockLength;
		DoDamageWhenEnterWrongCodeLock = s.DoDamageWhenEnterWrongCodeLock;
		DamageWhenEnterWrongCodeLock = s.DamageWhenEnterWrongCodeLock;
		RememberCode = s.RememberCode;
	
		CanCraftTerritoryFlagKit = s.CanCraftTerritoryFlagKit;
		SimpleTerritory = s.SimpleTerritory;
		AutomaticFlagOnCreation = s.AutomaticFlagOnCreation;
		GetTerritoryFlagKitAfterBuild = s.GetTerritoryFlagKitAfterBuild;
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
		EXPrint("ExpansionBaseBuildingSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		bool save;

		bool baseBuildingSettingsExist = FileExist(EXPANSION_BASE_BUILDING_SETTINGS);

		if (baseBuildingSettingsExist)
		{
			EXPrint("[ExpansionBaseBuildingSettings] Load existing setting file:" + EXPANSION_BASE_BUILDING_SETTINGS);
			
			ExpansionBaseBuildingSettings settingsDefault = new ExpansionBaseBuildingSettings;
			settingsDefault.Defaults();

			ExpansionBaseBuildingSettingsBase settingsBase;

			JsonFileLoader<ExpansionBaseBuildingSettingsBase>.JsonLoadFile(EXPANSION_BASE_BUILDING_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionBaseBuildingSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_BASE_BUILDING_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 2)
				{
					//! New with v2
					CopyInternal(settingsDefault);
				}

				if (settingsBase.m_Version < 3)
				{
					ExpansionBaseBuildingSettingsV2 settings_v2;
					JsonFileLoader<ExpansionBaseBuildingSettingsV2>.JsonLoadFile(EXPANSION_BASE_BUILDING_SETTINGS, settings_v2);

					DismantleFlagMode = settings_v2.DismantleFlagRequireTools;

					if (settings_v2.CanAttachCodelock == 1)
						CodelockAttachMode = ExpansionCodelockAttachMode.ExpansionAndTents;
					else if (settings_v2.CanAttachCodelock == 2)
						CodelockAttachMode = ExpansionCodelockAttachMode.ExpansionAndFence;
					else if (settings_v2.CanAttachCodelock == 3)
						CodelockAttachMode = ExpansionCodelockAttachMode.ExpansionAndFenceAndTents;
					else
						CodelockAttachMode = ExpansionCodelockAttachMode.ExpansionOnly;

					FlagMenuMode = settings_v2.EnableFlagMenu;

					BuildZoneRequiredCustomMessage = settings_v2.BuildZoneRequiredCustomMessage;
					Zones = settings_v2.Zones;
					ZonesAreNoBuildZones = settings_v2.ZonesAreNoBuildZones;
				}
				else
				{
					JsonFileLoader<ExpansionBaseBuildingSettings>.JsonLoadFile(EXPANSION_BASE_BUILDING_SETTINGS, this);
				}

				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionBaseBuildingSettings>.JsonLoadFile(EXPANSION_BASE_BUILDING_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionBaseBuildingSettings] No existing setting file:" + EXPANSION_BASE_BUILDING_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::Load - End - Loaded: " + baseBuildingSettingsExist);
		#endif
		
		return baseBuildingSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionBaseBuildingSettings] Saving settings");
		
		JsonFileLoader<ExpansionBaseBuildingSettings>.JsonSaveFile( EXPANSION_BASE_BUILDING_SETTINGS, this );
		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		CanBuildAnywhere = true;
		AllowBuildingWithoutATerritory = true;

		DeployableOutsideATerritory.Insert("ExpansionSatchel");
		DeployableOutsideATerritory.Insert("Fireplace");
		DeployableOutsideATerritory.Insert("TerritoryFlagKit");
		DeployableOutsideATerritory.Insert("TerritoryFlag");
		DeployableOutsideATerritory.Insert("MediumTent");
		DeployableOutsideATerritory.Insert("LargeTent");
		DeployableOutsideATerritory.Insert("CarTent");
		DeployableOutsideATerritory.Insert("PartyTent");
		DeployableOutsideATerritory.Insert("ExpansionCamoTentKit");
		DeployableOutsideATerritory.Insert("ExpansionCamoBoxKit");
		DeployableOutsideATerritory.Insert("ShelterKit");
		DeployableOutsideATerritory.Insert("BearTrap");
		DeployableOutsideATerritory.Insert("ExpansionSafeLarge");
		DeployableOutsideATerritory.Insert("ExpansionSafeMedium");
		DeployableOutsideATerritory.Insert("ExpansionSafeSmall");
		DeployableOutsideATerritory.Insert("SeaChest");
		DeployableOutsideATerritory.Insert("GardenPlot");
		
		DeployableInsideAEnemyTerritory.Insert("ExpansionSatchel");
		DeployableInsideAEnemyTerritory.Insert("LandMineTrap");
		DeployableInsideAEnemyTerritory.Insert("BearTrap");
		DeployableInsideAEnemyTerritory.Insert("FishNetTrap");
		DeployableInsideAEnemyTerritory.Insert("RabbitSnareTrap");
		DeployableInsideAEnemyTerritory.Insert("SmallFishTrap");
		DeployableInsideAEnemyTerritory.Insert("TripwireTrap");

		ZonesAreNoBuildZones = true;

		CanCraftVanillaBasebuilding = false;
		CanCraftExpansionBasebuilding = true;
		DestroyFlagOnDismantle = true;
		DismantleFlagMode = ExpansionDismantleFlagMode.AnyoneWithTools;
		DismantleOutsideTerritory = false;
		DismantleInsideTerritory = false;
		DismantleAnywhere = false;
		
		CodelockAttachMode = ExpansionCodelockAttachMode.ExpansionAndFence;  //! Will also allow BBP if installed
		CodelockActionsAnywhere = false;
		CodeLockLength = 4;
		DoDamageWhenEnterWrongCodeLock = true;
		DamageWhenEnterWrongCodeLock = 10.0;
		RememberCode = true;
	
		CanCraftTerritoryFlagKit = true;
		SimpleTerritory = true;
		AutomaticFlagOnCreation = true;
		FlagMenuMode = ExpansionFlagMenuMode.Enabled;
		GetTerritoryFlagKitAfterBuild = false;
		
	#ifdef EXPANSIONMODMARKET
		
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		if ( world_name.IndexOf("gloom") == world_name.Length() - 5 )
		{
			world_name = world_name.Substring(0, world_name.Length() - 5);
		}
		
		switch ( world_name )
		{
			case "chernarusplus":
			{
				DefaultChernarusNonBuildingZones();
				break;
			}
			case "namalsk":
			{
				DefaultNamalskNonBuildingZones();
				break;
			}
			case "takistanplus":
			{
				DefaultTakistanNonBuildingZones();
				break;
			}
		}
	#endif 
	}
	
	TStringArray GetNoBuildZoneWhitelist()
	{
		return {"Fireplace", "LandMineTrap", "BearTrap", "FishNetTrap", "RabbitSnareTrap", "SmallFishTrap", "TripwireTrap"};
	}
	
	// ------------------------------------------------------------	
	void DefaultChernarusNonBuildingZones()
	{
		

		//! Krasnostav Trader Camp
		ExpansionBuildNoBuildZone zone = new ExpansionBuildNoBuildZone("Krasnostav Trader Camp", "11882 143 12466", 1000.0, GetNoBuildZoneWhitelist(), true, "");
		Zones.Insert(zone);
		
		//! Green Mountain Trader Camp
		zone = new ExpansionBuildNoBuildZone("Green Mountain Trader Camp", "3728.27 403 6003.6", 1000.0, GetNoBuildZoneWhitelist(), true, "");
		Zones.Insert(zone);
		
		//! Kamenka Trader Camp
		zone = new ExpansionBuildNoBuildZone("Kamenka Trader Camp", "1143.14 6.9 2423.27", 1000.0, GetNoBuildZoneWhitelist(), true, "");
		Zones.Insert(zone);
	}
	
	// ------------------------------------------------------------	
	void DefaultNamalskNonBuildingZones()
	{
		//! Airsrip
		ExpansionBuildNoBuildZone zone = new ExpansionBuildNoBuildZone("Airstrip Trader", "6305 26 9521", 400.0, GetNoBuildZoneWhitelist(), true, "");
		Zones.Insert(zone);
		
		//! Jalovisko Trader Camp
		zone = new ExpansionBuildNoBuildZone("Jalovisko Trader Camp", "8583.67 29 10515", 200.0, GetNoBuildZoneWhitelist(), true, "");
		Zones.Insert(zone);
		
		//!  Tara Harbor
		zone = new ExpansionBuildNoBuildZone("Tara Harbor Boat Trader", "8043.45 10 7593.45", 200.0, GetNoBuildZoneWhitelist(), true, "");
		Zones.Insert(zone);
	}
	
	// ------------------------------------------------------------	
	void DefaultTakistanNonBuildingZones()
	{
		//! Marastar Oasis Trader Camp
		ExpansionBuildNoBuildZone zone = new ExpansionBuildNoBuildZone("Marastar Trader Camp", "4611.26 4.6 12334.0", 1000.0, GetNoBuildZoneWhitelist(), true, "");
		Zones.Insert(zone);
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Base-Building Settings";
	}
};