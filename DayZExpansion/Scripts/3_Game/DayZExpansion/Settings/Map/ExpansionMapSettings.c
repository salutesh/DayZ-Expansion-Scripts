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
	bool CanCreateMarker;				// Allow player to create markers
	bool ShowPlayerPosition;			// Allow player to see his position on the map
	bool CanCreatePartyMarkers;			// Allow player to create party markers
	bool ShowMapStats;
	bool CanCreate3DMarker;				// Allow player to create 3D markers
	bool ShowPartyMembersMapMarkers;	// Allow player to see his teammates position on the map
	bool ShowServerMarkers;				// Show server markers
	bool CanOpenMapWithKeyBinding;

	bool ShowNameOnServerMarkers;
	bool ShowDistanceOnServerMarkers;
	
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
		
		if ( !ctx.Read( EnableMap ) )
			return false;
		if ( !ctx.Read( UseMapOnMapItem ) )
			return false;
		if ( !ctx.Read( CanCreateMarker ) )
			return false;
		if ( !ctx.Read( ShowPlayerPosition ) )
			return false;
		if ( !ctx.Read( CanCreatePartyMarkers ) )
			return false;
		if ( !ctx.Read( ShowMapStats ) )
			return false;
		if ( !ctx.Read( CanCreate3DMarker ) )
			return false;
		if ( !ctx.Read( ShowPartyMembersMapMarkers ) )
			return false;
		if ( !ctx.Read( ShowServerMarkers ) )
			return false;
		if ( !ctx.Read( CanOpenMapWithKeyBinding ) )
			return false;
		if ( !ctx.Read( ShowNameOnServerMarkers ) )
			return false;
		if ( !ctx.Read( ShowDistanceOnServerMarkers ) )
			return false;

		int count = 0;
		int index = 0;
		int removeIndex = 0;
		string uid = "";
		if ( !ctx.Read( count ) )
			return false;
			
		ExpansionMarkerData marker;
		array< string > checkArr = ServerMarkersMap.GetKeyArray();
		for ( index = 0; index < count; ++index )
		{
			if ( !ctx.Read( uid ) )
				return false;

			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			marker = ServerMarkersMap.Get( uid );
			if ( !marker )
			{
				marker = ExpansionMarkerData.Create( ExpansionMapMarkerType.SERVER, uid );
				ServerMarkersMap.Insert( uid, marker );
				ServerMarkers.Insert( marker );
			}

			if ( !marker.OnRecieve( ctx ) )
				return false;
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			marker = ServerMarkersMap.Get( checkArr[index] );
			if ( marker )
			{
				removeIndex = ServerMarkers.Find( marker );
				if ( removeIndex != -1 )
					ServerMarkers.Remove( removeIndex );

				ServerMarkersMap.Remove( checkArr[index] );
				delete marker;
			}
		}

		m_IsLoaded = true;

		ExpansionSettings.SI_Map.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		int count = 0;
		int index = 0;
		int removeIndex = 0;
		string uid = "";

		ctx.Write( EnableMap );
		ctx.Write( UseMapOnMapItem );
		ctx.Write( CanCreateMarker );
		ctx.Write( ShowPlayerPosition );
		ctx.Write( CanCreatePartyMarkers );
		ctx.Write( ShowMapStats );
		ctx.Write( CanCreate3DMarker );
		ctx.Write( ShowPartyMembersMapMarkers );
		ctx.Write( ShowServerMarkers );
		ctx.Write( CanOpenMapWithKeyBinding );
		ctx.Write( ShowNameOnServerMarkers );
		ctx.Write( ShowDistanceOnServerMarkers );
		
		count = ServerMarkers.Count();
		ctx.Write( count );
		for ( index = 0; index < count; ++index )
		{
			ctx.Write( ServerMarkers[index].GetUID() );
			ServerMarkers[index].OnSend( ctx );
		}
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
		CanCreatePartyMarkers = s.CanCreatePartyMarkers;
		ShowMapStats = s.ShowMapStats;
		CanCreate3DMarker = s.CanCreate3DMarker;
		ShowPartyMembersMapMarkers = s.ShowPartyMembersMapMarkers;
		ShowServerMarkers = s.ShowServerMarkers;
		CanOpenMapWithKeyBinding = s.CanOpenMapWithKeyBinding;
		ShowNameOnServerMarkers = s.ShowNameOnServerMarkers;
		ShowDistanceOnServerMarkers = s.ShowDistanceOnServerMarkers;
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
		CanCreatePartyMarkers = true;
		ShowMapStats = true;
		CanCreate3DMarker = true;
		
		ShowPartyMembersMapMarkers = true;
		ShowServerMarkers = true;

		CanOpenMapWithKeyBinding = true;
		
		ShowNameOnServerMarkers = true;
		ShowDistanceOnServerMarkers = true;
		
		//! Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		//! Vanilla Maps
		if ( world_name.Contains( "chernarusplus" ) )
		{
			//! All 3D server markers - Chernarus
			//ServerMarkers.Insert( new ExpansionMarkerData( "Hero Trader-Zone", 		"Trader",		Vector( 11844.678711, 0, 12466.84082 ), 	ARGB( 255, 46, 204, 113 ), 	false, /*3D marker*/true ) ); 	// Color Emerald green: #2ecc71 
			//ServerMarkers.Insert( new ExpansionMarkerData( "Bandit Trader-Zone", 		"Trader",		Vector( 1127.144531, 0, 2419.871582 ), 	ARGB( 255, 231, 76, 60 ), 	false, /*3D marker*/true ) ); 	// Color Alizarin red: #e74c3c	
			//ServerMarkers.Insert( new ExpansionMarkerData( "Boat Trader", 				"Boat",			Vector( 14355.744141, 0, 13231.0507813 ), ARGB( 255, 52, 152, 219 ), 	false, /*3D marker*/true ) );	// Color Peter River blue: #3498db
			//ServerMarkers.Insert( new ExpansionMarkerData( "Boat Trader", 				"Boat",			Vector( 1755.128906, 0, 2027.907837 ), 	ARGB( 255, 52, 152, 219 ), 	false, /*3D marker*/true ) );	// Color Peter River blue: #3498db
			//ServerMarkers.Insert( new ExpansionMarkerData( "Aircraft Trader", 			"Helicopter",	Vector( 4971.664063, 0, 2438.656494 ), 	ARGB( 255, 52, 73, 94 ), 	false, /*3D marker*/true ) );	// Color Wet Asphalt gray: #34495e
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