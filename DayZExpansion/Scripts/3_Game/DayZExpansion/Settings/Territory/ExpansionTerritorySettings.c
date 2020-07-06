/**
 * ExpansionTerritorySettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionTerritorySettings
 * @brief		Spawn settings class
 **/
class ExpansionTerritorySettings: ExpansionSettingBase
{
	bool EnableTerritories;				//! If enabled, use the expansion territory system
	bool UseWholeMapForInviteList; 		//! Use it if you want whole map available in invite list, instead only nearby players.
	float TerritorySize;				//! The radius of a territory in meters.
	float TerritoryPerimterSize			//! The radius who prevent territories to overlap
	int MaxMembersInTerritory; 			//! If <= 0, unlimited territory size.
	int MaxTerritoryPerPlayer;			//! If <= 0, unlimited territory number.
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionTerritorySettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::ExpansionTerritorySettings - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::ExpansionTerritorySettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::HandleRPC - Start");
		#endif
		
		ExpansionTerritorySettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionTerritorySettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );
		
		m_IsLoaded = true;

		ExpansionSettings.SI_Territory.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::HandleRPC - End");
		#endif
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionTerritorySettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.Territory, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionTerritorySettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionTerritorySettings s )
	{
		EnableTerritories = s.EnableTerritories;
		UseWholeMapForInviteList = s.UseWholeMapForInviteList;
		TerritorySize = s.TerritorySize;
		TerritoryPerimterSize = s.TerritoryPerimterSize;
		MaxMembersInTerritory = s.MaxMembersInTerritory;
		MaxTerritoryPerPlayer = s.MaxTerritoryPerPlayer;
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
		EXPrint("ExpansionTerritorySettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_TERRITORY_SETTINGS ) )
		{
			JsonFileLoader<ExpansionTerritorySettings>.JsonLoadFile( EXPANSION_TERRITORY_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Load - End");
		#endif
			return true;
		}
		
		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Load - End");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionTerritorySettings] Saving settings");
		#endif
		
		JsonFileLoader<ExpansionTerritorySettings>.JsonSaveFile( EXPANSION_TERRITORY_SETTINGS, this );
		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Defaults - Start");
		#endif
		
		EnableTerritories = true;
		UseWholeMapForInviteList = false;
		TerritorySize = 150.0;
		TerritoryPerimterSize = 150.0;
		MaxMembersInTerritory = 10;
		MaxTerritoryPerPlayer = 1;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Defaults - End");
		#endif
	}
}