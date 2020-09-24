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

/**@class		ExpansionBaseBuildingSettings
 * @brief		Spawn settings class
 **/
class ExpansionBaseBuildingSettings: ExpansionSettingBase
{
	bool CanBuildAnywhere;									//! If enabled, allows players to build anywhere.
	bool AllowBuildingWithoutATerritory; 					//! 0 - You can build without territory, but nobody else can build near your territory. | 1 - You can only build if you are in your own territory.
	
	autoptr TStringArray DeployableOutsideATerritory;		//! List of items that you can place everywhere but not in enemy territory, only works when AllowBuildingWithoutATerritory is set to 1.
	autoptr TStringArray DeployableInsideAEnemyTerritory;	//! List of items that are allowed to place in enemy territory, only works when AllowBuildingWithoutATerritory is set to 1 or 0.
	
	bool CanCraftVanillaBasebuilding;						//! When enabled, allows players to craft Vanilla basebuilding.
	bool CanCraftExpansionBasebuilding;						//! When enabled, allows players to craft Expansion basebuilding.

	bool DestroyFlagOnDismantle;							//! When enabled, destroys both flag pole and the flag kit on dismantle.
	int DismantleFlagRequireTools;							//! -1 = can't dismantle | 0 = doesn't need tools to dismantle | 1 = dismantle with tools
	bool DismantleOutsideTerritory;							//! When enabled, allows players to dismantle basebuilding without territory.
	bool DismantleAnywhere;									//! When enabled, allows players to dismantle basebuilding anywhere and not only on the soft side.
	
	int CodeLockLength;										//! Lenght of pin code required for the code lock.
	bool DoDamageWhenEnterWrongCodeLock;					//! If enabled, deals the damage to the player when entering the wrong code lock.
	float DamageWhenEnterWrongCodeLock;						//! Amount of damage player takes when entering the wrong code in the code lock.

	bool CanCraftTerritoryFlagKit;							//! When enabled this will allow the crafting of the territory flag kit.
	bool SimpleTerritory;									//! When enabled this will construct the full flag pole when a flag kit is deployed.
	bool AutomaticFlagOnCreation;							//! When enabled this will add the flag attachment to the flag pole once the territory pole has been constructed
	bool EnableFlagMenu;									//! When enabled this allow to use the flag menu on territory flags.
	bool GetTerritoryFlagKitAfterBuild;						//! When enabled this will spawn a territory flag kit next to the full constructed flag pole.

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

		return false;
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
		DismantleAnywhere = s.DismantleAnywhere;
		
		CodeLockLength = s.CodeLockLength;
		DoDamageWhenEnterWrongCodeLock = s.DoDamageWhenEnterWrongCodeLock;
		DamageWhenEnterWrongCodeLock = s.DamageWhenEnterWrongCodeLock;
	
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

		if ( FileExist( EXPANSION_BASE_BUILDING_SETTINGS ) )
		{
			JsonFileLoader<ExpansionBaseBuildingSettings>.JsonLoadFile( EXPANSION_BASE_BUILDING_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionBaseBuildingSettings::Load - End");
			#endif

			return true;
		}
		
		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::Load - End");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionBaseBuildingSettings] Saving settings");
		#endif
		
		JsonFileLoader<ExpansionBaseBuildingSettings>.JsonSaveFile( EXPANSION_BASE_BUILDING_SETTINGS, this );
		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::Defaults - Start");
		#endif

		CanBuildAnywhere = true;
		AllowBuildingWithoutATerritory = true;

		DeployableOutsideATerritory.Insert("ExpansionSatchel");
		DeployableOutsideATerritory.Insert("Fireplace");
		DeployableOutsideATerritory.Insert("TerritoryFlagKit");
		
		DeployableInsideAEnemyTerritory.Insert("ExpansionSatchel");
		DeployableInsideAEnemyTerritory.Insert("LandMineTrap");
		DeployableInsideAEnemyTerritory.Insert("BearTrap");

		CanCraftVanillaBasebuilding = false;
		CanCraftExpansionBasebuilding = true;
		DestroyFlagOnDismantle = true;
		DismantleFlagRequireTools = 1;
		DismantleOutsideTerritory = false;
		DismantleAnywhere = false;
		
		CodeLockLength = 4;
		DoDamageWhenEnterWrongCodeLock = true;
		DamageWhenEnterWrongCodeLock = 10.0;
	
		CanCraftTerritoryFlagKit = true;
		SimpleTerritory = true;
		AutomaticFlagOnCreation = true;
		EnableFlagMenu = true;
		GetTerritoryFlagKitAfterBuild = false;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::Defaults - End");
		#endif
	}
}