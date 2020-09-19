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
	bool EnableMap;						//! Enable Expansion Map
	bool UseMapOnMapItem;				//! Use Expansion Map On Map Item
	
	bool ShowPlayerPosition;			//! Allow player to see his position on the map
	bool ShowMapStats;					//! Show XYZ positions of markers

	bool CanCreateMarker;				//! Allow player to create markers
	bool CanCreate3DMarker;				//! Allow player to create 3D markers
	bool CanOpenMapWithKeyBinding;		//! Allow player to use a keybind to open the map
	
	bool EnableHUDGPS;					//! Enable HUD GPS (bottom right corner after holding N)
	bool NeedGPSItemForKeyBinding;		//! Require GPS Item to use the keybind
	bool NeedMapItemForKeyBinding;		//! Require Map Item to use the keybind
	
	bool EnableServerMarkers;			//! Show server markers
	bool ShowNameOnServerMarkers;		//! Show the name of server markers
	bool ShowDistanceOnServerMarkers;	//! Show the distance of  server markers
	
	ref array< ref ExpansionMarkerData > ServerMarkers;
	
	[NonSerialized()]
	private ref map< string, ExpansionMarkerData > ServerMarkersMap;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	void ExpansionMapSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::ExpansionMapSettings - Start");
		#endif
		
		ServerMarkers = new array< ref ExpansionMarkerData >;
		ServerMarkersMap = new map< string, ExpansionMarkerData >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::ExpansionMapSettings - End");
		#endif
	}
	
	void ~ExpansionMapSettings()
	{
		delete ServerMarkers;
		delete ServerMarkersMap;
	}

	bool AddServerMarker( ref ExpansionMarkerData marker )
	{
		if ( marker.GetUID() == "" )
		{
			string newUid = "" + Math.RandomInt( 0, int.MAX );
			while ( ServerMarkersMap.Get( newUid ) )
				newUid = "" + Math.RandomInt( 0, int.MAX );
			
			marker.SetUID( newUid );
		} else if ( ServerMarkersMap.Get( marker.GetUID() ) )
			return false;

		ServerMarkers.Insert( marker );
		ServerMarkersMap.Insert( marker.GetUID(), marker );

		return true;
	}

	bool RemoveServerMarker( string markerName )
	{
		ExpansionMarkerData marker = ServerMarkersMap.Get( markerName );
		if ( marker )
		{
			int removeIndex = ServerMarkers.Find( marker );
			if ( removeIndex != -1 )
				ServerMarkers.Remove( removeIndex );

			ServerMarkersMap.Remove( markerName );
			delete marker;
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::OnRecieve - Start");
		#endif

		ExpansionMapSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionMapSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );


		m_IsLoaded = true;

		ExpansionSettings.SI_Map.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ref ExpansionMapSettings thisSetting = this;

		ctx.Write( thisSetting );
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
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
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
	private void CopyInternal( ref ExpansionMapSettings s, bool copyServerMarkers = true )
	{
		if ( copyServerMarkers )
		{
			ServerMarkers.Clear();

			for (int i = 0; i < s.ServerMarkers.Count(); i++)
			{
				ServerMarkers.Insert( s.ServerMarkers[i] );
			}
		}

		EnableMap = s.EnableMap;
		UseMapOnMapItem = s.UseMapOnMapItem;
		CanCreateMarker = s.CanCreateMarker;
		ShowPlayerPosition = s.ShowPlayerPosition;
		ShowMapStats = s.ShowMapStats;
		CanCreate3DMarker = s.CanCreate3DMarker;
		EnableServerMarkers = s.EnableServerMarkers;
		CanOpenMapWithKeyBinding = s.CanOpenMapWithKeyBinding;
		ShowNameOnServerMarkers = s.ShowNameOnServerMarkers;
		ShowDistanceOnServerMarkers = s.ShowDistanceOnServerMarkers;
		EnableHUDGPS = s.EnableHUDGPS;
		NeedGPSItemForKeyBinding = s.NeedGPSItemForKeyBinding;
		NeedMapItemForKeyBinding = s.NeedMapItemForKeyBinding;
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
			Print("[ExpansionMapSettings] Loading settings");

			JsonFileLoader<ExpansionMapSettings>.JsonLoadFile( EXPANSION_MAP_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionMapSettings::Load - End");
			#endif

			int index = 0;

			for ( index = 0; index < ServerMarkers.Count(); ++index )
				if ( ServerMarkers[index] )
					ServerMarkersMap.Insert( ServerMarkers[index].GetUID(), ServerMarkers[index] );

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
		Print("[ExpansionMapSettings] Saving settings");
		
		JsonFileLoader<ExpansionMapSettings>.JsonSaveFile( EXPANSION_MAP_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		Print("[ExpansionMapSettings] Loading default settings");
		
		EnableMap = true;
		UseMapOnMapItem = true;
		CanCreateMarker = true;
		ShowPlayerPosition = true;
		ShowMapStats = true;
		CanCreate3DMarker = true;
		
		EnableServerMarkers = true;

		CanOpenMapWithKeyBinding = true;
		
		ShowNameOnServerMarkers = true;
		ShowDistanceOnServerMarkers = true;
	
		EnableHUDGPS = true;

		#ifdef EXPANSION_SETTINGS_MAP_ITEM_DEFAULT_DISABLE
		NeedGPSItemForKeyBinding = false;
		NeedMapItemForKeyBinding = false;
		#else
		NeedGPSItemForKeyBinding = true;
		NeedMapItemForKeyBinding = true;
		#endif
		
		//! Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		//! Vanilla Maps
		if ( world_name.Contains( "chernarusplus" ) )
		{
			//! All 3D server markers - Chernarus
			//ServerMarkers.Insert( new ExpansionMarkerData( "Hero Trader-Zone", 		"Trader",		Vector( 11844.678711, 0, 12466.84082 ), 	ARGB( 255, 46, 204, 113 ), 	false, /*3D marker*/true ) ); 	// Color Emerald green: #2ecc71 
			//ServerMarkers.Insert( new ExpansionMarkerData( "Bandit Trader-Zone", 		"Trader",		Vector( 1127.144531, 0, 2419.871582 ), 		ARGB( 255, 231, 76, 60 ), 	false, /*3D marker*/true ) ); 	// Color Alizarin red: #e74c3c	
			//ServerMarkers.Insert( new ExpansionMarkerData( "Boat Trader", 			"Boat",			Vector( 14355.744141, 0, 13231.0507813 ), 	ARGB( 255, 52, 152, 219 ), 	false, /*3D marker*/true ) );	// Color Peter River blue: #3498db
			//ServerMarkers.Insert( new ExpansionMarkerData( "Boat Trader", 			"Boat",			Vector( 1755.128906, 0, 2027.907837 ), 		ARGB( 255, 52, 152, 219 ), 	false, /*3D marker*/true ) );	// Color Peter River blue: #3498db
			//ServerMarkers.Insert( new ExpansionMarkerData( "Aircraft Trader", 		"Helicopter",	Vector( 4971.664063, 0, 2438.656494 ), 		ARGB( 255, 52, 73, 94 ), 	false, /*3D marker*/true ) );	// Color Wet Asphalt gray: #34495e
		} else if ( world_name.Contains( "enoch" ) )
		{
			//! TODO: Livonia Server Markers
		} else if ( world_name.Contains( "deerisle" ) )
		{
			//! TODO: DeerIsle Server Markers
		}

		int index = 0;
		
		for ( index = 0; index < ServerMarkers.Count(); ++index )
			if ( ServerMarkers[index] )
				ServerMarkersMap.Insert( ServerMarkers[index].GetUID(), ServerMarkers[index] );

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionMapSettings] Default settings loaded!");
		#endif
	}
}