/**
 * ExpansionMapMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenu extends UIScriptedMenu
{
	protected PlayerBase m_PlayerB;

	protected MapWidget m_MapWidget;

	protected int COLOR_EXPANSION_MARKER_PLAYER_POSITION = ARGB( 255, 255, 180, 24 );

	protected ref array< ref ExpansionMapWidgetBase > m_Markers;

	protected ref map< string, ExpansionMapMarker > m_PersonalMarkers;
	protected ref map< string, ExpansionMapMarker > m_PartyMarkers;
	protected ref map< string, ExpansionMapMarker > m_ServerMarkers;
	protected ref map< string, ExpansionMapMarker > m_PlayerMarkers;
	
	private ref set< ExpansionMapMarker > m_DeletingMarkers;

	private ExpansionMapMarker m_SelectedMarker;

	protected ExpansionMarkerModule m_MarkerModule;
	protected ExpansionPartyModule m_PartyModule;
	
	private bool m_GpsWasOpen = false;

	private ref ExpansionMapMarkerList m_MarkerList;
	
	private float m_OpenMapTime = 0;
	
	private bool m_IsEditingMarker = false;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenu Constructor
	// ------------------------------------------------------------	
	void ExpansionMapMenu()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::ExpansionMapMenu - Start");
		#endif
		
		m_Markers = new array<ref ExpansionMapWidgetBase>();

		m_PersonalMarkers = new map< string, ExpansionMapMarker >();
		m_PartyMarkers = new map< string, ExpansionMapMarker >();
		m_ServerMarkers = new map< string, ExpansionMapMarker >();
		m_PlayerMarkers = new map< string, ExpansionMapMarker >();

		m_DeletingMarkers = new set< ExpansionMapMarker >();

		Class.CastTo( m_PlayerB, GetGame().GetPlayer() );

		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );
		Class.CastTo( m_PartyModule, GetModuleManager().GetModule( ExpansionPartyModule ) );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::ExpansionMapMenu - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenu Destructor
	// ------------------------------------------------------------	
	void ~ExpansionMapMenu()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::~ExpansionMapMenu - Start");
		#endif
		
		delete m_Markers;

		delete m_PersonalMarkers;
		delete m_PartyMarkers;
		delete m_ServerMarkers;
		delete m_PlayerMarkers;

		delete m_DeletingMarkers;
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::~ExpansionMapMenu - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Init - Start");
		#endif
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map.layout");
		Class.CastTo( m_MapWidget, layoutRoot.FindAnyWidget( "Map" ) );
		
		if ( GetExpansionSettings().GetMap().ShowPlayerPosition )
		{
			ExpansionMapMarkerPlayerArrow player_Marker = new ExpansionMapMarkerPlayerArrow( layoutRoot, m_MapWidget );
			
			PlayerBase player;
			if ( Class.CastTo( player, GetGame().GetPlayer() ) )
			{
				if ( GetExpansionClientSettings().StreamerMode )
				{
					player_Marker.SetName( "(YOU)" );
				} else 
				{
					player_Marker.SetName( "(YOU) " + player.GetIdentityName() );
				}
			}
			
			m_Markers.Insert( player_Marker );
		}
		
		GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/map/expansion_map_markerlist.layout", layoutRoot ).GetScript( m_MarkerList );

		m_MarkerList.Init( this );
		
		SetMapPosition();

		UpdateMarkers();
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Init - End and return layoutRoot: " + layoutRoot.ToString());
		#endif
		
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion Override Refresh
	// ------------------------------------------------------------
	override void Refresh()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Refresh - Start");
		#endif
		
		super.Refresh();

		UpdateMarkers();
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Refresh - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateMarkers
	// ------------------------------------------------------------
	void UpdateMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkers - Start");
		#endif
		
		UpdatePersonalMarkers();
		UpdatePartyMarkers();
		UpdateServerMarkers();
		UpdatePlayerMarkers();

		for ( int i = 0; i < m_DeletingMarkers.Count(); ++i )
		{
			ExpansionMapMarker marker = m_DeletingMarkers[i];
			if ( marker == NULL )
				continue;

			if ( marker.GetMarkerData() != NULL )
			{
				if ( marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PERSONAL )
					continue;

				if ( marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PARTY )
					continue;
			}

			int index = m_Markers.Find( marker );
			if ( index != -1 )
				m_Markers.Remove( index );

			delete marker;

			m_DeletingMarkers.Remove( i );

			i--;
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePersonalMarkers
	// ------------------------------------------------------------
	private void UpdatePersonalMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePersonalMarkers - Start");
		#endif
		
		array< string > checkArr = m_PersonalMarkers.GetKeyArray();

		int removeIndex = 0;
		int count = m_MarkerModule.GetData().PersonalCount();
		int index = 0;
		
		ExpansionMapMarker marker = NULL;
		string uid = "";

		for ( int i = 0; i < m_DeletingMarkers.Count(); ++i )
		{
			marker = m_DeletingMarkers[i];
			if ( marker && marker.GetMarkerData() != NULL )
			{
				if ( marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PERSONAL )
				{
					int idx2 = m_Markers.Find( marker );
					if ( idx2 != -1 )
						m_Markers.Remove( idx2 );
					delete marker;

					m_DeletingMarkers.Remove( i );
					
					i--;
				}
			}
		}

		for ( index = 0; index < count; ++index )
		{
			uid = m_MarkerModule.GetData().PersonalGet( index ).GetUID();
			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			marker = m_PersonalMarkers.Get( uid );
			if ( !marker )
			{
				marker = new ExpansionMapMarker( layoutRoot, m_MapWidget, false );
				marker.SetMarkerData( m_MarkerModule.GetData().PersonalGet( index ) );
				marker.SetMapMenu( this );
				marker.Init();
				m_PersonalMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
				m_MarkerList.AddPersonalEntry(marker);
			}

			marker.SetMarkerData( m_MarkerModule.GetData().PersonalGet( index ) );
			marker.SetMapMenu( this );
			
			
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			marker = m_PersonalMarkers.Get( checkArr[index] );
			if ( marker )
			{
				removeIndex = m_Markers.Find( marker );
				if ( removeIndex != -1 )
					m_Markers.Remove( removeIndex );

				m_PersonalMarkers.Remove( checkArr[index] );
				m_MarkerList.RemovePersonalEntry(marker);
				delete marker;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePersonalMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePartyMarkers
	// ------------------------------------------------------------
	private void UpdatePartyMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - Start");
		#endif
		
		int removeIndex = 0;
		int count = m_PartyMarkers.Count();
		int index = 0;

		ExpansionMapMarker marker = NULL;
		string uid = "";
		if ( !m_PartyModule || !m_PartyModule.HasParty() )
		{
			foreach ( string uuid, ref ExpansionMapMarker mmarker : m_PartyMarkers )
			{
				if ( mmarker )
				{
					removeIndex = m_Markers.Find( mmarker );
					if ( removeIndex != -1 )
						m_Markers.Remove( removeIndex );

					m_PartyMarkers.Set( uuid, NULL );
					m_MarkerList.RemovePartyEntry( mmarker );
					delete mmarker;
				}
			}

			m_PartyMarkers.Clear();

			return;
		}

		array< ref ExpansionMarkerData > markers = m_PartyModule.GetParty().GetAllMarkers();
		array< string > checkArr = m_PartyMarkers.GetKeyArray();

		for ( int i = 0; i < m_DeletingMarkers.Count(); ++i )
		{
			marker = m_DeletingMarkers[i];
			if ( marker && marker.GetMarkerData() != NULL )
			{
				if ( marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PARTY )
				{
					int idx2 = m_Markers.Find( marker );
					if ( idx2 != -1 )
						m_Markers.Remove( idx2 );
					
					delete marker;

					m_DeletingMarkers.Remove( i );
					
					i--;
				}
			}
		}

		count = markers.Count();

		for ( index = 0; index < count; ++index )
		{
			uid = markers[index].GetUID();
			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			marker = m_PartyMarkers.Get( uid );
			if ( !marker )
			{
				marker = new ExpansionMapMarker( layoutRoot, m_MapWidget, false );
				marker.SetMarkerData( markers[index] );
				marker.SetMapMenu( this );
				marker.Init();
				m_PartyMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
				m_MarkerList.AddPartyEntry(marker);
			}

			marker.SetMarkerData( markers[index] );
			marker.SetMapMenu( this );
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			marker = m_PartyMarkers.Get( checkArr[index] );
			if ( marker )
			{
				removeIndex = m_Markers.Find( marker );
				if ( removeIndex != -1 )
					m_Markers.Remove( removeIndex );

				m_PartyMarkers.Remove( checkArr[index] );
				m_MarkerList.RemovePartyEntry(marker);
				delete marker;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateServerMarkers
	// ------------------------------------------------------------
	private void UpdateServerMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - Start");
		#endif
		
		if ( !m_MarkerModule )
			return;
		
		array< string > checkArr = m_ServerMarkers.GetKeyArray();

		int removeIndex = 0;
		int count = m_MarkerModule.GetData().ServerCount();
		int index = 0;
		ExpansionMapMarker marker = NULL;
		string uid = "";

		for ( index = 0; index < count; ++index )
		{
			uid = m_MarkerModule.GetData().ServerGet( index ).GetUID();
			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			marker = m_ServerMarkers.Get( uid );
			if ( !marker )
			{
				marker = new ExpansionMapMarkerServer( layoutRoot, m_MapWidget, false );
				marker.SetMarkerData( m_MarkerModule.GetData().ServerGet( index ) );
				marker.SetMapMenu( this );
				marker.Init();
				m_ServerMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
				m_MarkerList.AddServerEntry(marker);
			}

			marker.SetMarkerData( m_MarkerModule.GetData().ServerGet( index ) );
			marker.SetMapMenu( this );
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			marker = m_ServerMarkers.Get( checkArr[index] );
			if ( marker )
			{
				removeIndex = m_Markers.Find( marker );
				if ( removeIndex != -1 )
					m_Markers.Remove( removeIndex );

				m_ServerMarkers.Remove( checkArr[index] );
				m_MarkerList.RemoveServerEntry(marker);
				delete marker;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdatePlayerMarkers
	// ------------------------------------------------------------
	private void UpdatePlayerMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - Start");
		#endif
		
		int removeIndex = 0;
		int count = m_PlayerMarkers.Count();
		int index = 0;

		ExpansionMapMarker marker = NULL;
		string uid = "";
		if ( !m_PartyModule || !m_PartyModule.HasParty() )
		{
			foreach ( string uuid, ref ExpansionMapMarker mmarker : m_PlayerMarkers )
			{
				if ( mmarker )
				{
					removeIndex = m_Markers.Find( mmarker );
					if ( removeIndex != -1 )
						m_Markers.Remove( removeIndex );

					m_PlayerMarkers.Set( uuid, NULL );
					m_MarkerList.RemovePartyEntry( mmarker );
					delete mmarker;
				}
			}

			m_PlayerMarkers.Clear();

			return;
		}

		PlayerBase localPlayer = PlayerBase.Cast( GetGame().GetPlayer() );
		string localUid = "";
		if ( localPlayer )
			localUid = localPlayer.GetIdentityUID();

		array< ref ExpansionPartyPlayerData > players = m_PartyModule.GetParty().GetPlayers();
		array< string > checkArr = m_PlayerMarkers.GetKeyArray();

		count = players.Count();

		for ( index = 0; index < count; ++index )
		{
			uid = players[index].UID;
			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			if ( uid == localUid )
				continue;

			marker = ExpansionMapMarkerPlayer.Cast( m_PlayerMarkers.Get( uid ) );
			if ( !marker )
			{
				marker = new ExpansionMapMarkerPlayer( layoutRoot, m_MapWidget, false );
				marker.SetMarkerData( players[index].Marker );
				marker.SetMapMenu( this );
				marker.Init();
				m_PlayerMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
				m_MarkerList.AddMemberEntry(marker);
			}

			marker.SetMarkerData( players[index].Marker );
			marker.SetMapMenu( this );
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			marker = ExpansionMapMarkerPlayer.Cast( m_PlayerMarkers.Get( checkArr[index] ) );
			if ( marker )
			{
				removeIndex = m_Markers.Find( marker );
				if ( removeIndex != -1 )
					m_Markers.Remove( removeIndex );

				m_PlayerMarkers.Remove( checkArr[index] );
				m_MarkerList.RemoveMemberEntry(marker);
				delete marker;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Show
	// ------------------------------------------------------------	
	void Show()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Show - Start");
		#endif
		
		if (layoutRoot)
			layoutRoot.Show(true);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Show - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Hide
	// ------------------------------------------------------------	
	void Hide()
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Hide - Start");
		#endif
		
		if (layoutRoot)
			layoutRoot.Show(false);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Hide - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CreateNewMarker
	// ------------------------------------------------------------	
	void CreateNewMarker( int x, int y )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CreateNewMarker - Start");
		#endif
		
		ExpansionMapMarker marker = new ExpansionMapMarker( layoutRoot, m_MapWidget, false );
		marker.Init();

		marker.SetIcon( ExpansionIcons.Get( 0 ) );
		marker.SetCreation( true );
		marker.SetPosition( x, y );
		marker.SetMapMenu( this );

		marker.ShowEditPanel();

		m_SelectedMarker = marker;

		m_Markers.Insert( marker );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CreateNewMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetSelectedMarker
	// ------------------------------------------------------------
	ExpansionMapMarker GetSelectedMarker()
	{
		return m_SelectedMarker;
	}

	bool HasSelectedMarker()
	{
		return m_SelectedMarker != NULL;
	}
	
	// ------------------------------------------------------------
	// Expansion SetSelectedMarker
	// ------------------------------------------------------------
	bool SetSelectedMarker( ref ExpansionMarkerData data )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetSelectedMarker - Start");
		#endif
		
		if ( m_SelectedMarker != NULL )
		{
			CancelCurrentSelectedMarker();
		}
		
		for ( int i = 0; i < m_Markers.Count(); ++i )
		{
			ExpansionMapMarker marker;
			if ( Class.CastTo( marker, m_Markers[i] ) && marker.GetMarkerData() == data )
			{
				m_SelectedMarker = marker;

				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMenu::SetSelectedMarker - return true for m_SelectedMarker: " + m_SelectedMarker.ToString());
				#endif
				
				return true;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetSelectedMarker - return false");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	// Expansion CompleteCreationNewMarker
	// ------------------------------------------------------------
	void CompleteCreationNewMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CompleteCreationNewMarker - Start");
		#endif
		
		if ( m_SelectedMarker == NULL )
			return;

		if ( !m_MarkerModule.CreateMarker( m_SelectedMarker.GetMarkerData() ) )
			return;

		m_DeletingMarkers.Insert( m_SelectedMarker );

		PlayDrawSound();
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CompleteCreationNewMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateSelectedMarker
	// ------------------------------------------------------------
	void UpdateSelectedMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateSelectedMarker - Start");
		#endif
		
		if ( m_SelectedMarker == NULL )
			return;

		if ( !m_MarkerModule.UpdateMarker( m_SelectedMarker.GetMarkerData() ) )
			return;
		
		m_SelectedMarker = NULL;

		PlayDrawSound();
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateSelectedMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion CancelCurrentSelectedMarker
	// ------------------------------------------------------------
	/**
	 * Cancels what ever is currently happening to the selected marker
	 * If the selected marker is the creation of a new marker, then it deletes it
	 * Otherwise it chucks out the new data
	 */
	void CancelCurrentSelectedMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CancelCurrentSelectedMarker - Start");
		#endif
		
		if ( m_SelectedMarker == NULL )
			return;

		m_SelectedMarker.CloseEditPanel();

		if ( m_SelectedMarker.GetMarkerData() )
		{
			string uid = m_SelectedMarker.GetMarkerData().GetUID();
			if ( m_PersonalMarkers.Get( uid ) == m_SelectedMarker || m_PartyMarkers.Get( uid ) == m_SelectedMarker )
			{
				m_SelectedMarker = NULL;
				return;
			}
		}

		int index = m_Markers.Find( m_SelectedMarker );
		if ( index != -1 )
			m_Markers.Remove( index );

		delete m_SelectedMarker;
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CancelCurrentSelectedMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteMarker
	// ------------------------------------------------------------	
	void DeleteMarker( ref ExpansionMapMarker marker )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteMarker - Start");
		#endif
		
		if ( marker == NULL )
			return;

		if ( marker.GetMarkerData() != NULL )
		{
			string uid = marker.GetMarkerData().GetUID();

			m_MarkerList.RemoveEntry( marker );
			
			if ( m_PersonalMarkers.Get( uid ) == marker )
			{
				m_MarkerModule.GetData().PersonalRemove( uid );

				return;
			}

			if ( m_PartyMarkers.Get( uid ) == marker )
			{
				m_MarkerModule.GetData().PartyRemove( uid );

				return;
			}
			
			if ( m_ServerMarkers.Get( uid ) == marker )
			{
				m_MarkerModule.GetData().PersonalRemove( uid );

				return;
			}

			return;
		}

		m_DeletingMarkers.Insert( marker );

		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Call( Refresh );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion DeleteSelectedMarker
	// ------------------------------------------------------------	
	void DeleteSelectedMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteSelectedMarker - Start");
		#endif

		DeleteMarker( m_SelectedMarker );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteSelectedMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetMapMenuRoot
	// ------------------------------------------------------------	
	Widget GetMapMenuRoot()
	{
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion GetCurrentMapPos
	// ------------------------------------------------------------	
	vector GetCurrentMapPos()
	{
		vector cameraPos;
		vector mapPos;

		cameraPos = GetGame().GetCurrentCameraPosition();
		mapPos = m_MapWidget.MapToScreen(cameraPos);

		return mapPos;
	}
	
	// ------------------------------------------------------------
	// Expansion GetCurrentMapDir
	// ------------------------------------------------------------	
	int GetCurrentMapDir()
	{
		vector cameraOri;
		float yaw;
		float normalizedAngle;
		int dir;

		cameraOri = GetGame().GetCurrentCameraDirection().VectorToAngles();
		yaw = cameraOri[0];
		normalizedAngle = Math.NormalizeAngle(yaw);
		dir = Math.Round(normalizedAngle);

		return dir;
	}

	// ------------------------------------------------------------
	// Expansion ToggleGPS
	// ------------------------------------------------------------
	void ToggleGPS()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::ToggleGPS - Start");
		#endif
		
		MissionGameplay missiongameplay = MissionGameplay.Cast( GetGame().GetMission() );
		if ( missiongameplay )
		{
			ExpansionIngameHud expansionhud = ExpansionIngameHud.Cast( missiongameplay.GetExpansionHud() );
			if ( expansionhud )
			{
				if ( expansionhud.GetGPSState() )
				{
					expansionhud.ShowGPS(false);
					m_GpsWasOpen = true;
				}
				else if ( m_GpsWasOpen && !expansionhud.GetGPSState() )
				{
					expansionhud.ShowGPS(true);
					m_GpsWasOpen = false;
				}
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::ToggleGPS - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetMap
	// ------------------------------------------------------------	
	MapWidget GetMap()
	{
		return m_MapWidget;
	}

	// ------------------------------------------------------------
	// Expansion RemoveMarker
	// Events when remove marker with delete key
	// ------------------------------------------------------------	
	void RemoveMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::RemoveMarker - Start");
		#endif
		
		for ( int i = 0; i < m_Markers.Count(); ++i )
		{
			ExpansionMapMarker marker;
			if ( Class.CastTo( marker, m_Markers[i] ) && marker.IsMouseHovering() && marker.CanEdit() )
			{
				DeleteMarker( marker );

				return;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::RemoveMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SetMapPosition
	// ------------------------------------------------------------	
	void SetMapPosition()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetMapPosition - Start");
		#endif
		
		float scale;
		vector map_pos;
		vector player_pos;
		vector camera_pos;
		
		if(!m_PlayerB)
			m_PlayerB = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if(m_PlayerB && !m_PlayerB.GetLastMapInfo(scale,map_pos))
		{
			//! Lower number zooms in / Higher number zooms out
			scale = 0.10; // Float between 0-1 ?!
			
			#ifdef EXPANSION_MAP_MENU_DEBUG
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current scale is: " + scale.ToString());
			#endif

			player_pos = m_PlayerB.GetPosition();
			camera_pos = GetGame().GetCurrentCameraPosition();

			#ifdef EXPANSION_MAP_MENU_DEBUG
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current player position is: " + player_pos.ToString());
			#endif

			#ifdef EXPANSION_MAP_MENU_DEBUG
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current camera position is: " + camera_pos.ToString());
			#endif
			
			map_pos = camera_pos;

			#ifdef EXPANSION_MAP_MENU_DEBUG
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Map Position is: " + map_pos.ToString());
			#endif
		}

		m_MapWidget.SetScale(scale);
		m_MapWidget.SetMapPos(map_pos);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetMapPosition:: SetMapPos with values: " + map_pos.ToString());
		#endif
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetMapPosition:: GetMapPos is: " + m_MapWidget.GetMapPos().ToString());
		#endif
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetMapPosition - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateMapPosition
	// ------------------------------------------------------------	
	void UpdateMapPosition()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMapPosition - Start");
		#endif
		
		float scale;
		vector map_pos;
		
		scale = m_MapWidget.GetScale();
		map_pos = m_MapWidget.GetMapPos();
		
		PlayerBase m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if (m_Player)
			m_Player.SetLastMapInfo(scale, map_pos);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMapPosition - End");
		#endif
	}

	// ------------------------------------------------------------
	// Override OnDoubleClick
	// ------------------------------------------------------------
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{
		if ( button == MouseState.LEFT )
		{
			if ( w == m_MapWidget )
			{
				if ( HasSelectedMarker() )
				{
					CancelCurrentSelectedMarker();

					return true;
				} else if ( GetExpansionSettings().GetMap().CanCreateMarker )
				{
					int mouse_x;
					int mouse_y;

					GetGame().GetMousePos( mouse_x, mouse_y );
					CreateNewMarker( mouse_x, mouse_y );

					//! This works but not sure if we should do this, doesn't feel right.
					//m_SelectedMarker.FocusOnMarker( false );

					return true;
				}
			}
		}
		
		return super.OnDoubleClick( w, x, y, button );
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		return super.OnClick(w, x, y, button);
	}
	
	// ------------------------------------------------------------
	// Expansion Override OnShow
	// ------------------------------------------------------------	
	override void OnShow()
	{
		super.OnShow();

		m_OpenMapTime = 0;
		
		ToggleGPS();

		SetFocus(layoutRoot);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		PPEffects.SetBlurMenu(0.5);
		
		Show();
	}
	
	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();
		
		ToggleGPS();

		m_MarkerModule.SaveLocalServerMarkers();
		
		//m_MarkerList.HideTooltips();

		GetGame().GetMission().PlayerControlEnable(true);
		
		PPEffects.SetBlurMenu( 0.0 );
	}

	// ------------------------------------------------------------
	// Expansion Override Update
	// ------------------------------------------------------------	
	override void Update( float timeslice )
	{		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Update - Start");
		#endif

		m_OpenMapTime += timeslice;
		
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) )
		{
			Hide();
			Close();

			return;
		}
		
		if ( GetGame().GetInput().LocalPress( "UAExpansionMapToggle", false ) && m_OpenMapTime > 0.10 && !IsEditingMarker() )
		{
			Hide();
			Close();
			return;
		}
		
		if ( GetGame().GetInput().LocalPress( "UAExpansionMapDeleteMarker", false ) )
		{
			RemoveMarker();
		}
		
		UpdateMapPosition();
		
		if ( layoutRoot.IsVisible() )
		{
			for ( int i = 0; i < m_Markers.Count(); ++i )
			{
				if ( m_Markers[i] )
				{
					m_Markers[i].Update( timeslice );
				}
			}
		}
		
		/*if ( m_MarkerList.IsListVisible() ) 
			UpdateMarkerList();*/
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Update - End");
		#endif
	}
	
	/*private void UpdateMarkerList()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkerList - Start");
		#endif
		
		int j;
		ref ExpansionMapMarkerListEntry entry;
		for ( j = 0; j < m_MarkerList.GetPersonalEntrys().Count(); ++j )
		{			
			entry = m_MarkerList.GetPersonalEntrys()[j];
			if ( entry.GetMarker() == NULL || entry.GetData() == NULL )
			{
				delete entry;
			}
			
			entry.Update();
		}
		
		for ( j = 0; j < m_MarkerList.GetPartyEntrys().Count(); ++j )
		{
			entry = m_MarkerList.GetPartyEntrys()[j];
			if ( entry.GetMarker() == NULL || entry.GetData() == NULL )
			{
				delete entry;
			}
			
			entry.Update();
		}
		
		for ( j = 0; j < m_MarkerList.GetMemberEntrys().Count(); ++j )
		{
			entry = m_MarkerList.GetMemberEntrys()[j];
			if ( entry.GetMarker() == NULL || entry.GetData() == NULL )
			{
				delete entry;
			}
			
			entry.Update();
		}
		
		for ( j = 0; j < m_MarkerList.GetServerEntrys().Count(); ++j )
		{
			entry = m_MarkerList.GetServerEntrys()[j];
			if ( entry.GetMarker() == NULL || entry.GetData() == NULL )
			{
				delete entry;
			}
			
			entry.Update();
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkerList - End");
		#endif
	}*/
	
	// ------------------------------------------------------------
	// Expansion PlayDrawSound
	// ------------------------------------------------------------	
	void PlayDrawSound()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			SEffectManager.PlaySound("Expansion_Draws_SoundSet", m_PlayerB.GetPosition());
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetIsEditingMarker
	// ------------------------------------------------------------	
	void SetIsEditingMarker( bool state )
	{
		m_IsEditingMarker = state;
	}
	
	// ------------------------------------------------------------
	// Expansion IsEditingMarker
	// ------------------------------------------------------------	
	bool IsEditingMarker()
	{
		return m_IsEditingMarker;
	}
}