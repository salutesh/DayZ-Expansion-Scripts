/**
 * ExpansionBaseBuildingSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	DismantleFlagMode DismantleFlagRequireTools;					//! -1 = can't dismantle | 0 = doesn't need tools to dismantle | 1 = dismantle with tools
	bool DismantleOutsideTerritory;											//! When enabled, allows players to dismantle basebuilding without territory.
	bool DismantleInsideTerritory;											//! When enabled, allows players to dismantle basebuilding inside another territory.
	bool DismantleAnywhere;													//! When enabled, allows players to dismantle basebuilding anywhere and not only on the soft side.
	CodelockAttachMode CanAttachCodelock;								//! 0 = only on expansion basebuilding | 1 = also on tents (and exp) | 2 = also on fence (and exp) | 3 = also on tents and fence (and exp)
	bool CodelockActionsAnywhere;											//! If enabled, Allow the player to get code lock actions from anywhere on the target instead of the codelock selection.
	int CodeLockLength;															//! Lenght of pin code required for the code lock.
	bool DoDamageWhenEnterWrongCodeLock;							//! If enabled, deals the damage to the player when entering the wrong code lock.
	float DamageWhenEnterWrongCodeLock;							//! Amount of damage player takes when entering the wrong code in the code lock.
	bool RememberCode;														//! Remember code once entered correctly
	bool CanCraftTerritoryFlagKit;												//! When enabled this will allow the crafting of the territory flag kit.
	bool SimpleTerritory;															//! When enabled this will construct the full flag pole when a flag kit is deployed.
	bool AutomaticFlagOnCreation;											//! When enabled this will add the flag attachment to the flag pole once the territory pole has been constructed
	FlagMenuMode EnableFlagMenu;											//! When enabled this allow to use the flag menu on territory flags.
	bool GetTerritoryFlagKitAfterBuild;										//! When enabled this will spawn a territory flag kit next to the full constructed flag pole.
}

/**@class		ExpansionBaseBuildingSettings
 * @brief		Base buidling settings class
 **/
class ExpansionBaseBuildingSettings: ExpansionBaseBuildingSettingsBase
{
	static const int VERSION = 2;

	string BuildZoneRequiredCustomMessage;							//! Custom message for build zone required (optional, uses default message if not set)
	ref array < ref ExpansionBuildNoBuildZone > Zones;			//! "No build" or "build" zones
	bool ZonesAreNoBuildZones;											//! Whether the provided zones are "no build" zones (true = can not build in the zones) or "build" zones (false = can ONLY build in the zones)
	
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
		ref ExpansionBaseBuildingSettings thisSetting = this;

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
	private void CopyInternal( ref ExpansionBaseBuildingSettings s )
	{
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
	private void CopyInternal( ref ExpansionBaseBuildingSettingsBase s )
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
		DismantleFlagRequireTools = s.DismantleFlagRequireTools;
		DismantleOutsideTerritory = s.DismantleOutsideTerritory;
		DismantleInsideTerritory = s.DismantleInsideTerritory;
		DismantleAnywhere = s.DismantleAnywhere;
		
		CanAttachCodelock = s.CanAttachCodelock;
		CodelockActionsAnywhere = s.CodelockActionsAnywhere;
		CodeLockLength = s.CodeLockLength;
		DoDamageWhenEnterWrongCodeLock = s.DoDamageWhenEnterWrongCodeLock;
		DamageWhenEnterWrongCodeLock = s.DamageWhenEnterWrongCodeLock;
		RememberCode = s.RememberCode;
	
		CanCraftTerritoryFlagKit = s.CanCraftTerritoryFlagKit;
		SimpleTerritory = s.SimpleTerritory;
		AutomaticFlagOnCreation = s.AutomaticFlagOnCreation;
		EnableFlagMenu = s.EnableFlagMenu;
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
			ExpansionBaseBuildingSettings settingsDefault = new ExpansionBaseBuildingSettings;
			settingsDefault.Defaults();

			ExpansionBaseBuildingSettingsBase settingsBase;

			JsonFileLoader<ExpansionBaseBuildingSettingsBase>.JsonLoadFile(EXPANSION_BASE_BUILDING_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 2)
				{
					EXPrint("[ExpansionBaseBuildingSettings] Load - Converting v1 \"" + EXPANSION_BASE_BUILDING_SETTINGS + "\" to v" + VERSION);
					
					//! New with v2
					CopyInternal(settingsDefault);
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
		Print("[ExpansionBaseBuildingSettings] Loading default settings");

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

		ZonesAreNoBuildZones = true;

		CanCraftVanillaBasebuilding = false;
		CanCraftExpansionBasebuilding = true;
		DestroyFlagOnDismantle = true;
		DismantleFlagRequireTools = DismantleFlagMode.WithTools;
		DismantleOutsideTerritory = false;
		DismantleInsideTerritory = false;
		DismantleAnywhere = false;
		
		CanAttachCodelock = CodelockAttachMode.ExpansionOnly;
		CodelockActionsAnywhere = false;
		CodeLockLength = 4;
		DoDamageWhenEnterWrongCodeLock = true;
		DamageWhenEnterWrongCodeLock = 10.0;
		RememberCode = true;
	
		CanCraftTerritoryFlagKit = true;
		SimpleTerritory = true;
		AutomaticFlagOnCreation = true;
		EnableFlagMenu = FlagMenuMode.Enabled;
		GetTerritoryFlagKitAfterBuild = false;
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Base-Building Settings";
	}
};