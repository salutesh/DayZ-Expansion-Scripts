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
	
	int ShowPlayerPosition;				//! 0 = Player position is not visible | 1 = Player position is visible | 2 = Player position is visible if the player have a compass
	bool ShowMapStats;					//! Show XYZ positions of markers

	bool NeedPenItemForCreateMarker;	//! Require Pen Item to create markers
	bool NeedGPSItemForCreateMarker;	//! Require GPS Item to create markers
	bool CanCreateMarker;				//! Allow player to create markers
	bool CanCreate3DMarker;				//! Allow player to create 3D markers
	bool CanOpenMapWithKeyBinding;		//! Allow player to use a keybind to open the map
	bool ShowDistanceOnPersonalMarkers;	//! Show the distance of personal markers
	
	bool EnableHUDGPS;					//! Enable HUD GPS (bottom right corner after holding N)
	bool NeedGPSItemForKeyBinding;		//! Require GPS Item to use the keybind
	bool NeedMapItemForKeyBinding;		//! Require Map Item to use the keybind
	
	bool EnableServerMarkers;			//! Show server markers
	bool ShowNameOnServerMarkers;		//! Show the name of server markers
	bool ShowDistanceOnServerMarkers;	//! Show the distance of server markers
	
	//! WARNING, Do not send over ExpansionMapSettings as a variable, use OnSend.
	//! Failure will result in ServerMarkers incurring a memory leak on the client.
	ref array< ref ExpansionMarkerData > ServerMarkers;
	
	[NonSerialized()]
	private ref map< string, ExpansionMarkerData > ServerMarkersMap;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	void ExpansionMapSettings()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMapSettings::ExpansionMapSettings - Start");
		#endif
		
		ServerMarkers = new array< ref ExpansionMarkerData >;
		ServerMarkersMap = new map< string, ExpansionMarkerData >;
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
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
	
	// ------------------------------------------------------------
	bool RemoveServerMarker( string uid )
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMapSettings::RemoveServerMarker - Start");
		#endif
		
		ref ExpansionMarkerData server_markerMap = ServerMarkersMap.Get( uid );
		if ( server_markerMap )
		{
			ServerMarkersMap.Remove( uid );
		
			for ( int index = 0; index < ServerMarkers.Count(); ++index )
			{
				ref ExpansionMarkerData server_marker = ServerMarkers[index];
				if ( server_marker.GetUID() == uid )
				{
					ServerMarkers.Remove( index );
				}
			}
			
			delete server_markerMap;
			
			return true;
		}
		
		return false;
	}

	// ------------------------------------------------------------
	// Client OnReceive
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMapSettings::OnRecieve - Start");
		#endif

		if ( !ctx.Read( EnableMap ) )
			return false;
		if ( !ctx.Read( UseMapOnMapItem ) )
			return false;
		if ( !ctx.Read( ShowPlayerPosition ) )
			return false;
		if ( !ctx.Read( ShowMapStats ) )
			return false;
		if ( !ctx.Read( NeedPenItemForCreateMarker ) )
			return false;
		if ( !ctx.Read( NeedGPSItemForCreateMarker ) )
			return false;
		if ( !ctx.Read( CanCreateMarker ) )
			return false;
		if ( !ctx.Read( CanCreate3DMarker ) )
			return false;
		if ( !ctx.Read( ShowDistanceOnPersonalMarkers ) )
			return false;
		if ( !ctx.Read( CanOpenMapWithKeyBinding ) )
			return false;
		if ( !ctx.Read( EnableHUDGPS ) )
			return false;
		if ( !ctx.Read( NeedGPSItemForKeyBinding ) )
			return false;
		if ( !ctx.Read( NeedMapItemForKeyBinding ) )
			return false;
		if ( !ctx.Read( EnableServerMarkers ) )
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

		// Remove server markers that have been deleted on server
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			RemoveServerMarker( checkArr[index] );
		}

		m_IsLoaded = true;

		ExpansionSettings.SI_Map.Invoke();
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMapSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write( EnableMap );
		ctx.Write( UseMapOnMapItem );
		ctx.Write( ShowPlayerPosition );
		ctx.Write( ShowMapStats );
		ctx.Write( NeedPenItemForCreateMarker );
		ctx.Write( NeedGPSItemForCreateMarker );
		ctx.Write( CanCreateMarker );
		ctx.Write( CanCreate3DMarker );
		ctx.Write( ShowDistanceOnPersonalMarkers );		
		ctx.Write( CanOpenMapWithKeyBinding );
		ctx.Write( EnableHUDGPS );
		ctx.Write( NeedGPSItemForKeyBinding );
		ctx.Write( NeedMapItemForKeyBinding );
		ctx.Write( EnableServerMarkers );
		ctx.Write( ShowNameOnServerMarkers );
		ctx.Write( ShowDistanceOnServerMarkers );

		int count = 0;
		int index = 0;
		string uid = "";
		
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
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMapSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Map, true, identity );
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
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
		/*
		if ( copyServerMarkers )
		{
			ServerMarkers.Clear();
			Print("CopyInternal::s.ServerMarkers"+s.ServerMarkers.Count());

			for (int i = 0; i < s.ServerMarkers.Count(); i++)
			{
				ServerMarkers.Insert( s.ServerMarkers[i] );
			}
		}
		*/

		EnableMap = s.EnableMap;
		UseMapOnMapItem = s.UseMapOnMapItem;
		NeedPenItemForCreateMarker = s.NeedPenItemForCreateMarker;
		NeedGPSItemForCreateMarker = s.NeedGPSItemForCreateMarker;
		CanCreateMarker = s.CanCreateMarker;
		ShowPlayerPosition = s.ShowPlayerPosition;
		ShowMapStats = s.ShowMapStats;
		CanCreate3DMarker = s.CanCreate3DMarker;
		ShowDistanceOnPersonalMarkers = s.ShowDistanceOnPersonalMarkers;
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
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMapSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_MAP_SETTINGS ) )
		{
			Print("[ExpansionMapSettings] Loading settings");

			JsonFileLoader<ExpansionMapSettings>.JsonLoadFile( EXPANSION_MAP_SETTINGS, this );

			#ifdef EXPANSION_MARKER_MODULE_DEBUG
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

		#ifdef EXPANSION_MARKER_MODULE_DEBUG
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
		NeedPenItemForCreateMarker = false;
		NeedGPSItemForCreateMarker = false;
		ShowPlayerPosition = 1;
		ShowMapStats = true;
		CanCreateMarker = true;
		CanCreate3DMarker = true;
		ShowDistanceOnPersonalMarkers = true;
		
		EnableServerMarkers = true;

		CanOpenMapWithKeyBinding = true;
		
		ShowNameOnServerMarkers = true;
		ShowDistanceOnServerMarkers = true;
	
		EnableHUDGPS = true;

		NeedGPSItemForKeyBinding = true;
		NeedMapItemForKeyBinding = false;

		int index = 0;
		
		for ( index = 0; index < ServerMarkers.Count(); ++index )
			if ( ServerMarkers[index] )
				ServerMarkersMap.Insert( ServerMarkers[index].GetUID(), ServerMarkers[index] );

		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("[ExpansionMapSettings] Default settings loaded!");
		#endif
	}
	
	override string SettingName()
	{
		return "Map Settings";
	}
};