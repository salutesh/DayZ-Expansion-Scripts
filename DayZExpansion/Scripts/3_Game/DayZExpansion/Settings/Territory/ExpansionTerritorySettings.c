/**
 * ExpansionTerritorySettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionTerritorySettingsBase
 * @brief		Territory settings base class
 **/
class ExpansionTerritorySettingsBase: ExpansionSettingBase
{
	bool EnableTerritories;					//! If enabled, use the expansion territory system
	bool UseWholeMapForInviteList; 		//! Use it if you want whole map available in invite list, instead only nearby players.
	float TerritorySize;						//! The radius of a territory in meters.
	float TerritoryPerimeterSize			//! The radius who prevent territories to overlap
	int MaxMembersInTerritory; 			//! If <= 0, unlimited territory size.
	int MaxTerritoryPerPlayer;				//! If <= 0, unlimited territory number.
}

/**@class		ExpansionTerritorySettings
 * @brief		Spawn settings class
 **/
class ExpansionTerritorySettings: ExpansionTerritorySettingsBase
{
	static const int VERSION = 0;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::OnRecieve - Start");
		#endif
		
		ExpansionTerritorySettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionTerritorySettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );
		
		m_IsLoaded = true;

		ExpansionSettings.SI_Territory.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::OnRecieve - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::OnSend - Start");
		#endif
		
		ExpansionTerritorySettings thisSetting = this;

		ctx.Write( thisSetting );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::OnSend - End");
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
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Territory, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Copy - Start");
		#endif
		
		ExpansionTerritorySettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Copy - End");
		#endif
		
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionTerritorySettings s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::CopyInternal - Start");
		#endif
		
		//!Nothing to do here yet
		
		ExpansionTerritorySettingsBase sb = s;
		CopyInternal( sb );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::CopyInternal - End");
		#endif
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionTerritorySettingsBase s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::CopyInternal - Start");
		#endif
		
		EnableTerritories = s.EnableTerritories;
		UseWholeMapForInviteList = s.UseWholeMapForInviteList;
		TerritorySize = s.TerritorySize;
		TerritoryPerimeterSize = s.TerritoryPerimeterSize;
		MaxMembersInTerritory = s.MaxMembersInTerritory;
		MaxTerritoryPerPlayer = s.MaxTerritoryPerPlayer;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::CopyInternal - End");
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
		EXPrint("ExpansionTerritorySettings::Load - Start");
		#endif

		m_IsLoaded = true;

		bool save;

		bool territorySettingsExist = FileExist(EXPANSION_TERRITORY_SETTINGS);

		if (territorySettingsExist)
		{
			EXPrint("[ExpansionTerritorySettings] Load existing setting file:" + EXPANSION_TERRITORY_SETTINGS);
			
			ExpansionTerritorySettings settingsDefault = new ExpansionTerritorySettings;
			settingsDefault.Defaults();

			ExpansionTerritorySettingsBase settingsBase;

			JsonFileLoader<ExpansionTerritorySettingsBase>.JsonLoadFile(EXPANSION_TERRITORY_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 2)
				{
					EXPrint("[ExpansionTerritorySettings] Load - Converting v1 \"" + EXPANSION_TERRITORY_SETTINGS + "\" to v" + VERSION);
					//!Nothing to do here yet
				}
				
				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionTerritorySettings>.JsonLoadFile(EXPANSION_TERRITORY_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionTerritorySettings] No existing setting file:" + EXPANSION_TERRITORY_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionTerritorySettings::Load - End");
		#endif
		
		return territorySettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionTerritorySettings] Saving settings");
		
		JsonFileLoader<ExpansionTerritorySettings>.JsonSaveFile( EXPANSION_TERRITORY_SETTINGS, this );
		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		EnableTerritories = true;
		UseWholeMapForInviteList = false;
		TerritorySize = 150.0;
		TerritoryPerimeterSize = 150.0;
		MaxMembersInTerritory = 10;
		MaxTerritoryPerPlayer = 1;
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Territory Settings";
	}
};