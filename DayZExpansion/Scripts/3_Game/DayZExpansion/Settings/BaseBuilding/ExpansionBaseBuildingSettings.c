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
	bool CanCraftVanillaBasebuilding;
	bool CanCraftExpansionBasebuilding;
	bool CanCraftTerritoryFlag;
	bool DestroyFlagOnDismantle;
	bool CanDismantleFlag;
	bool DismantleOutsideTerritory;

	bool CanRaidSafes;

	float ExplosionTime;

	float ExplosionDamageMultiplier;
	float ProjectileDamageMultiplier;	


	float SafeExplosionDamageMultiplier;
	float SafeProjectileDamageMultiplier;
	
	int TerritoryMode; //! 0 Means that you can build without territory, but nobody else can build near your territory || 1 Means that you can only build if you are in your territory
	
	int CodeLockLength;
	
	//For these 2, it's the item in the hand. sometimes it's the kit, but not the item itself, for example flag, is flagkit instead 
	autoptr TStringArray AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory;	// Only used when TerritoryMode == 1
	autoptr TStringArray AllowedItemsToPlaceInEnemyTerritory;	// Used when TerritoryMode == 1 OR == 0
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionBaseBuildingSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::ExpansionBaseBuildingSettings - Start");
		#endif
		
		AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory = new TStringArray;
		AllowedItemsToPlaceInEnemyTerritory = new TStringArray;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::ExpansionBaseBuildingSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::HandleRPC - Start");
		#endif
		
		ExpansionBaseBuildingSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionBaseBuildingSettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );
		
		m_IsLoaded = true;

		ExpansionSettings.SI_BaseBuilding.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::HandleRPC - End");
		#endif
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
		
		ref ExpansionBaseBuildingSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
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
		CanCraftVanillaBasebuilding = s.CanCraftVanillaBasebuilding;
		CanCraftExpansionBasebuilding = s.CanCraftExpansionBasebuilding;
		CanCraftTerritoryFlag = s.CanCraftTerritoryFlag;
		DestroyFlagOnDismantle = s.DestroyFlagOnDismantle;
		CanDismantleFlag = s.CanDismantleFlag;
		DismantleOutsideTerritory = s.DismantleOutsideTerritory;

		CanRaidSafes = s.CanRaidSafes;

		ExplosionTime = s.ExplosionTime;

		ExplosionDamageMultiplier = s.ExplosionDamageMultiplier;
		ProjectileDamageMultiplier = s.ProjectileDamageMultiplier;		
		
		SafeExplosionDamageMultiplier = s.ExplosionDamageMultiplier;
		SafeProjectileDamageMultiplier = s.ProjectileDamageMultiplier;
		TerritoryMode = s.TerritoryMode;
		CodeLockLength = s.CodeLockLength;
		
		AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory.Clear();
		for (int i = 0; i < s.AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory.Count(); i++)
		{
			AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory.Insert( s.AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory[i] );
		}
		
		AllowedItemsToPlaceInEnemyTerritory.Clear();
		for (i = 0; i < s.AllowedItemsToPlaceInEnemyTerritory.Count(); i++)
		{
			AllowedItemsToPlaceInEnemyTerritory.Insert( s.AllowedItemsToPlaceInEnemyTerritory[i] );
		}
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

		CanCraftVanillaBasebuilding = false;
		CanCraftExpansionBasebuilding = true;
		CanCraftTerritoryFlag = true;
		DestroyFlagOnDismantle = true;
		CanDismantleFlag = true;
		DismantleOutsideTerritory = false;

		CanRaidSafes = true;
		
		ExplosionTime = 60;

		ExplosionDamageMultiplier = 50;
		ProjectileDamageMultiplier = 1;		
		
		SafeExplosionDamageMultiplier = 17;
		SafeProjectileDamageMultiplier = 1;
		TerritoryMode = 0;
		CodeLockLength = 4;
		
		AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory.Insert("ExpansionExplosiveBase");
		AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory.Insert("Fireplace");
		AllowedItemsToPlaceEveryWhereButNotInEnnemyTerritory.Insert("ExpansionFlagKitBase");
		
		AllowedItemsToPlaceInEnemyTerritory.Insert("ExpansionExplosiveBase");
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuildingSettings::Defaults - End");
		#endif
	}
}