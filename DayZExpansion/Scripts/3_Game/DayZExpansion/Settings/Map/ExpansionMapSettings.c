/**
 * ExpansionMapSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionEventCategorys
 * @brief		Map settings class
 **/
class ExpansionMapSettings: ExpansionSettingBase
{
	bool EnableMap;
	bool UseMapOnMapItem;
	bool CanCreateMarker;
    bool ShowPlayerPosition;
	bool PartyMarkers;
	bool ShowMapStats;
	bool CanCreate3DMarker;
	bool ShowPartyMembersMapMarkers;
	bool ShowServerMarkers;
	bool CanOpenMapWithKeyBinding;

	ref array< ref ExpansionMapMarker > ServerMarkers;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	void ExpansionMapSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::ExpansionMapSettings - Start");
		#endif
		
		ServerMarkers = new array< ref ExpansionMapMarker >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::ExpansionMapSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::HandleRPC - Start");
		#endif
		
		ExpansionMapSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionMapSettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Map.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::HandleRPC - End");
		#endif
    }

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionMapSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.Map, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
    override bool Copy( ExpansionSettingBase setting )
    {
		ExpansionMapSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionMapSettings s )
	{
		ServerMarkers.Clear();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 1 s : " + s);
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 2 s.ServerMarkers.Count() : " + s.ServerMarkers.Count());
		#endif
		for (int i = 0; i < s.ServerMarkers.Count(); i++)
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 2." + (i+1) + " s.ServerMarkers[" + i + "] : " + s.ServerMarkers[i]);
		#endif
			ServerMarkers.Insert( s.ServerMarkers[i] );
		}

		EnableMap = s.EnableMap;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 3 s.EnableMap : " + s.EnableMap);
		#endif
		UseMapOnMapItem = s.UseMapOnMapItem;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 4 s.UseMapOnMapItem : " + s.UseMapOnMapItem);
		#endif
		CanCreateMarker = s.CanCreateMarker;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 5 s.CanCreateMarker : " + s.CanCreateMarker);
		#endif
		ShowPlayerPosition = s.ShowPlayerPosition;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 6 s.ShowPlayerPosition : " + s.ShowPlayerPosition);
		#endif
		PartyMarkers = s.PartyMarkers;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 7 s.PartyMarkers : " + s.PartyMarkers);
		#endif
		ShowMapStats = s.ShowMapStats;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 8 s.ShowMapStats : " + s.ShowMapStats);
		#endif
		CanCreate3DMarker = s.CanCreate3DMarker;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 9 s.CanCreate3DMarker : " + s.CanCreate3DMarker);
		#endif
		ShowPartyMembersMapMarkers = s.ShowPartyMembersMapMarkers;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 10 s.ShowPartyMembersMapMarkers : " + s.ShowPartyMembersMapMarkers);
		#endif
		ShowServerMarkers = s.ShowServerMarkers;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 11 s.ShowServerMarkers : " + s.ShowServerMarkers);
		#endif
		CanOpenMapWithKeyBinding = s.CanOpenMapWithKeyBinding;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::CopyInternal 12 s.CanOpenMapWithKeyBinding : " + s.CanOpenMapWithKeyBinding);
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
		EXPrint("ExpansionMapSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_MAP_SETTINGS ) )
		{
			JsonFileLoader<ExpansionMapSettings>.JsonLoadFile( EXPANSION_MAP_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionMapSettings::Load - End");
			#endif

			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::Load - End");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::Save - Start");
		#endif
		
		JsonFileLoader<ExpansionMapSettings>.JsonSaveFile( EXPANSION_MAP_SETTINGS, this );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::Save - End");
		#endif

		return true;
	}

	// ------------------------------------------------------------
    override void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionMapSettings] Loading default settings..");
		#endif
		
		EnableMap = true;
		UseMapOnMapItem = true;
        CanCreateMarker = true;
        ShowPlayerPosition = true;
        PartyMarkers = true;
		ShowMapStats = true;
		CanCreate3DMarker = true;
		
		ShowPartyMembersMapMarkers = true;
		ShowServerMarkers = true;

		CanOpenMapWithKeyBinding = true;
		
		// Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		// Vanilla Maps
        if ( world_name.Contains( "chernarusplus" ) )
        {
			//All 3D server markers - Chernarus
			//ServerMarkers.Insert( new ExpansionMapMarker( "Hero Trader-Zone", 		6, Vector( 11844.678711, 0, 12466.84082 ), 	ARGB( 255, 46, 204, 113 ), 	false, /*3D marker*/true ) ); 	// Color Emerald green: #2ecc71 
			//ServerMarkers.Insert( new ExpansionMapMarker( "Bandit Trader-Zone", 		6, Vector( 1127.144531, 0, 2419.871582 ), 	ARGB( 255, 231, 76, 60 ), 	false, /*3D marker*/true ) ); 	// Color Alizarin red: #e74c3c	
			//ServerMarkers.Insert( new ExpansionMapMarker( "Boat Trader", 			12, Vector( 14355.744141, 0, 13231.0507813 ), ARGB( 255, 52, 152, 219 ), 	false, /*3D marker*/true ) );	// Color Peter River blue: #3498db
			//ServerMarkers.Insert( new ExpansionMapMarker( "Boat Trader", 			12, Vector( 1755.128906, 0, 2027.907837 ), 	ARGB( 255, 52, 152, 219 ), 	false, /*3D marker*/true ) );	// Color Peter River blue: #3498db
			//ServerMarkers.Insert( new ExpansionMapMarker( "Aircraft Trader", 		10, Vector( 4971.664063, 0, 2438.656494 ), 	ARGB( 255, 52, 73, 94 ), 	false, /*3D marker*/true ) );	// Color Wet Asphalt gray: #34495e
		} 
		else if ( world_name.Contains( "enoch" ) )
        {
			//! TODO: Livonia Server Markers
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionMapSettings] Default settings loaded!");
		#endif
	}
}