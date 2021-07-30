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

class ExpansionMapMenu extends ExpansionUIScriptedMenu
{
	protected PlayerBase m_PlayerB;

	protected ref MapWidget m_MapWidget;

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
	private float m_Time;
	
	private bool m_IsEditingMarker = false;

	private bool m_IsShown;

	private bool m_DoUpdateMarkers;
	private int m_MaxMarkerUpdatesPerFrame = 3;  //! Max markers updated per frame for each marker type

	private ref array< string > m_PartyMarkersCheckArr;
	private int m_PartyMarkersUpdateIndex;
	private bool m_PartyMarkersUpdated;

	private ref array< string > m_PersonalMarkersCheckArr;
	private int m_PersonalMarkersUpdateIndex;
	private bool m_PersonalMarkersUpdated;

	private ref array< string > m_PlayerMarkersCheckArr;
	private int m_PlayerMarkersUpdateIndex;
	private bool m_PlayerMarkersUpdated;

	private ref array< string > m_ServerMarkersCheckArr;
	private int m_ServerMarkersUpdateIndex;
	private bool m_ServerMarkersUpdated;
	
	ItemMap m_Map;
	
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
		
		m_PartyMarkersUpdated = false;
		m_PersonalMarkersUpdated = false;
		m_PlayerMarkersUpdated = false;
		m_ServerMarkersUpdated = false;
		m_DoUpdateMarkers = true;
		
		if ( layoutRoot )
			return layoutRoot;
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map.layout");
		Class.CastTo( m_MapWidget, layoutRoot.FindAnyWidget( "Map" ) );
		
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		
		if (m_MapWidget)
		{
			float scale;
			vector map_pos;
			if( player && !player.GetLastMapInfo(scale,map_pos) )
			{
				string path = "CfgWorlds " + GetGame().GetWorldName();
				vector temp = GetGame().ConfigGetVector(path + " centerPosition");
				scale = 0.33;
				map_pos = Vector(temp[0],temp[2],temp[1]);
			}
			m_MapWidget.SetScale(scale);
			m_MapWidget.SetMapPos(map_pos);
		}
		
		if ( GetExpansionSettings().GetMap().ShowPlayerPosition == 1 || GetExpansionSettings().GetMap().ShowPlayerPosition == 2 )
		{
			ExpansionMapMarkerPlayerArrow player_Marker = new ExpansionMapMarkerPlayerArrow( layoutRoot, m_MapWidget );
			
			if ( Class.CastTo( player, g_Game.GetPlayer() ) )
			{
				if ( GetExpansionClientSettings().StreamerMode )
				{
					player_Marker.SetName( new StringLocaliser("#STR_EXPANSION_MAP_MARKER_PLAYERMARKER_STREAMERMODE") );
				} else {
					player_Marker.SetName( new StringLocaliser("#STR_EXPANSION_MAP_MARKER_PLAYERMARKER_STREAMERMODE") );

					//! Doesn't work I don't know why
					//player_Marker.SetName( new StringLocaliser("#STR_EXPANSION_MAP_MARKER_PLAYERMARKER", player.GetIdentityName() ) );
				}
			}
			if ( GetExpansionSettings().GetMap().ShowPlayerPosition == 2 )
			{
				if ( PlayerBase.Cast( GetGame().GetPlayer() ).HasItemCompass() )
				{
					player_Marker.Show();
				} else {
					player_Marker.Hide();
				}
			}
			
			m_Markers.Insert( player_Marker );
		}
		
		Widget marker_list = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/map/expansion_map_markerlist.layout", layoutRoot );
		marker_list.GetScript( m_MarkerList );
		
		m_MarkerList.Init( this );
		
		SetMapPosition();
		
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
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Refresh - Start");
		#endif
		
		super.Refresh();

		m_DoUpdateMarkers = true;
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Refresh - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateMarkers
	// ------------------------------------------------------------
	void UpdateMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkers - Start");
		#endif
		
		if ( !m_PersonalMarkersUpdated )
			UpdatePersonalMarkers();
		if ( !m_PartyMarkersUpdated )
			UpdatePartyMarkers();
		if ( !m_ServerMarkersUpdated )
			UpdateServerMarkers();
		if ( !m_PlayerMarkersUpdated )
			UpdatePlayerMarkers();
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePersonalMarkers
	// ------------------------------------------------------------
	private void UpdatePersonalMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePersonalMarkers - Start");
		#endif

		int removeIndex = 0;
		int count = m_MarkerModule.GetData().PersonalCount();
		int index = m_PersonalMarkersUpdateIndex;
		int end = Math.Min( index + m_MaxMarkerUpdatesPerFrame, count );
		
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

		if ( index == 0 )
			m_PersonalMarkersCheckArr = m_PersonalMarkers.GetKeyArray();

		for ( ; index < end; ++index )
		{
			uid = m_MarkerModule.GetData().PersonalGet( index ).GetUID();
			removeIndex = m_PersonalMarkersCheckArr.Find( uid );
			if ( removeIndex != -1 )
				m_PersonalMarkersCheckArr.Remove( removeIndex );

			marker = m_PersonalMarkers.Get( uid );
			if ( !marker )
			{
				marker = new ExpansionMapMarker( layoutRoot, m_MapWidget, false );
				marker.Init();
				m_PersonalMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
			}
			
			marker.SetMarkerData( m_MarkerModule.GetData().PersonalGet( index ) );
			marker.SetMapMenu( this );
			
			if ( !m_MarkerList.HasPersonalEntry( marker ) )
				m_MarkerList.AddPersonalEntry( marker );
			
			m_PersonalMarkersUpdateIndex++;
		}
		
		if ( index == count )
		{
			m_PersonalMarkersUpdateIndex = 0;

			for ( index = 0; index < m_PersonalMarkersCheckArr.Count(); ++index )
			{
				marker = m_PersonalMarkers.Get( m_PersonalMarkersCheckArr[index] );
				if ( marker && !marker.GetMarkerData() )
				{
					removeIndex = m_Markers.Find( marker );
					if ( removeIndex != -1 )
						m_Markers.Remove( removeIndex );

					m_PersonalMarkers.Remove( m_PersonalMarkersCheckArr[index] );
					m_MarkerList.RemovePersonalEntry(marker);
					delete marker;
				}
			}

			m_PersonalMarkersUpdated = true;
		}
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePersonalMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePartyMarkers
	// ------------------------------------------------------------
	private void UpdatePartyMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - Start");
		#endif
		
		int removeIndex = 0;
		int index = m_PartyMarkersUpdateIndex;

		ExpansionMapMarker marker = NULL;
		string uid = "";
		if ( !m_PartyModule || !m_PartyModule.HasParty() )
		{
			foreach ( string uuid, ExpansionMapMarker mmarker : m_PartyMarkers )
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

			m_PartyMarkersUpdated = true;
		
			#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
			EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - No Party - End");
			#endif

			return;
		}

		array< ref ExpansionMarkerData > markers = m_PartyModule.GetParty().GetAllMarkers();

		if ( index == 0 )
			m_PartyMarkersCheckArr = m_PartyMarkers.GetKeyArray();

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

		int count = markers.Count();
		int end = Math.Min( index + m_MaxMarkerUpdatesPerFrame, count );

		for ( ; index < end; ++index )
		{
			uid = markers[index].GetUID();
			removeIndex = m_PartyMarkersCheckArr.Find( uid );
			if ( removeIndex != -1 )
				m_PartyMarkersCheckArr.Remove( removeIndex );

			marker = m_PartyMarkers.Get( uid );
			if ( !marker )
			{
				marker = new ExpansionMapMarker( layoutRoot, m_MapWidget, false );
				marker.Init();
				m_PartyMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
			}

			marker.SetMarkerData( markers[index] );
			marker.SetMapMenu( this );
			
			if ( !m_MarkerList.HasPartyEntry( marker ) )
				m_MarkerList.AddPartyEntry( marker );

			m_PartyMarkersUpdateIndex++;
		}

		if ( index == count )
		{
			m_PartyMarkersUpdateIndex = 0;

			for ( index = 0; index < m_PartyMarkersCheckArr.Count(); ++index )
			{
				marker = m_PartyMarkers.Get( m_PartyMarkersCheckArr[index] );
				if ( marker && !marker.GetMarkerData() )
				{
					removeIndex = m_Markers.Find( marker );
					if ( removeIndex != -1 )
						m_Markers.Remove( removeIndex );

					m_PartyMarkers.Remove( m_PartyMarkersCheckArr[index] );
					m_MarkerList.RemovePartyEntry(marker);
					delete marker;
				}
			}

			m_PartyMarkersUpdated = true;
		}
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateServerMarkers
	// ------------------------------------------------------------
	private void UpdateServerMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - Start");
		#endif

		if ( !m_MarkerModule )
		{
			m_ServerMarkersUpdated = true;
		
			#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
			EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - No Marker Module - End");
			#endif

			return;
		}

		int removeIndex = 0;
		int count = m_MarkerModule.GetData().ServerCount();
		int index = m_ServerMarkersUpdateIndex;
		int end = Math.Min( index + m_MaxMarkerUpdatesPerFrame, count );

		ExpansionMapMarker marker = NULL;
		string uid = "";
		
		if ( index == 0 )
			m_ServerMarkersCheckArr = m_ServerMarkers.GetKeyArray();

		for ( ; index < end; ++index )
		{
			uid = m_MarkerModule.GetData().ServerGet( index ).GetUID();
			removeIndex = m_ServerMarkersCheckArr.Find( uid );
			if ( removeIndex != -1 )
				m_ServerMarkersCheckArr.Remove( removeIndex );

			marker = m_ServerMarkers.Get( uid );
			if ( !marker )
			{
				marker = new ExpansionMapMarkerServer( layoutRoot, m_MapWidget, false );
				marker.Init();
				m_ServerMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
			}

			marker.SetMarkerData( m_MarkerModule.GetData().ServerGet( index ) );
			marker.SetMapMenu( this );

			if ( !m_MarkerList.HasServerEntry( marker ) )
				m_MarkerList.AddServerEntry( marker );
			
			m_ServerMarkersUpdateIndex++;
		}
		
		if ( index == count )
		{
			m_ServerMarkersUpdateIndex = 0;

			for ( index = 0; index < m_ServerMarkersCheckArr.Count(); ++index )
			{
				marker = m_ServerMarkers.Get( m_ServerMarkersCheckArr[index] );
				if ( marker && !marker.GetMarkerData() )
				{
					removeIndex = m_Markers.Find( marker );
					if ( removeIndex != -1 )
						m_Markers.Remove( removeIndex );

					m_ServerMarkers.Remove( m_ServerMarkersCheckArr[index] );
					m_MarkerList.RemoveServerEntry(marker);
					delete marker;
				}
			}

			m_ServerMarkersUpdated = true;
		}
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdatePlayerMarkers
	// ------------------------------------------------------------
	private void UpdatePlayerMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - Start");
		#endif
		
		int removeIndex = 0;
		int index = m_PlayerMarkersUpdateIndex;

		ExpansionMapMarker marker = NULL;
		string uid = "";
		if ( !m_PartyModule || !m_PartyModule.HasParty() )
		{
			foreach ( string uuid, ExpansionMapMarker mmarker : m_PlayerMarkers )
			{
				if ( mmarker )
				{
					removeIndex = m_Markers.Find( mmarker );
					if ( removeIndex != -1 )
						m_Markers.Remove( removeIndex );

					m_PlayerMarkers.Set( uuid, NULL );
					m_MarkerList.RemoveMemberEntry( mmarker );
					delete mmarker;
				}
			}

			m_PlayerMarkers.Clear();

			m_PlayerMarkersUpdated = true;

			#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
			EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - No Party - End");
			#endif

			return;
		}

		PlayerBase localPlayer = PlayerBase.Cast( GetGame().GetPlayer() );
		string localUid = "";
		if ( localPlayer )
			localUid = localPlayer.GetIdentityUID();

		array< ref ExpansionPartyPlayerData > players = m_PartyModule.GetParty().GetPlayers();

		if ( index == 0 )
			m_PlayerMarkersCheckArr = m_PlayerMarkers.GetKeyArray();

		int count = players.Count();
		int end = Math.Min( index + m_MaxMarkerUpdatesPerFrame, count );

		for ( ; index < end; ++index )
		{
			uid = players[index].UID;
			removeIndex = m_PlayerMarkersCheckArr.Find( uid );
			if ( removeIndex != -1 )
				m_PlayerMarkersCheckArr.Remove( removeIndex );

			if ( uid == localUid )
				continue;

			marker = ExpansionMapMarkerPlayer.Cast( m_PlayerMarkers.Get( uid ) );
			if ( !marker )
			{
				marker = new ExpansionMapMarkerPlayer( layoutRoot, m_MapWidget, false );
				marker.Init();
				m_PlayerMarkers.Insert( uid, marker );
				m_Markers.Insert( marker );
			}

			marker.SetMarkerData( players[index].Marker );
			marker.SetMapMenu( this );

			if ( !m_MarkerList.HasMemberEntry( marker ) )
				m_MarkerList.AddMemberEntry( marker );
			
			m_PlayerMarkersUpdateIndex++;
		}
		
		if ( index == count )
		{
			m_PlayerMarkersUpdateIndex = 0;

			for ( index = 0; index < m_PlayerMarkersCheckArr.Count(); ++index )
			{
				marker = ExpansionMapMarkerPlayer.Cast( m_PlayerMarkers.Get( m_PlayerMarkersCheckArr[index] ) );
				if ( marker && !marker.GetMarkerData() )
				{
					removeIndex = m_Markers.Find( marker );
					if ( removeIndex != -1 )
						m_Markers.Remove( removeIndex );

					m_PlayerMarkers.Remove( m_PlayerMarkersCheckArr[index] );
					m_MarkerList.RemoveMemberEntry(marker);
					delete marker;
				}
			}

			m_PlayerMarkersUpdated = true;
		}
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - End");
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
	bool SetSelectedMarker(  ExpansionMarkerData data )
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

		m_SelectedMarker = NULL;
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CancelCurrentSelectedMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteMarker
	// ------------------------------------------------------------	
	void DeleteMarker(  ExpansionMapMarker marker )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteMarker - Start");
		#endif
		
		if ( marker == NULL )
			return;

		if ( marker.GetMarkerData() != NULL )
		{
			string uid = marker.GetMarkerData().GetUID();
			
			if ( m_PersonalMarkers.Get( uid ) == marker )
			{
				m_MarkerModule.GetData().PersonalRemove( uid );
				m_DoUpdateMarkers = true;

				return;
			}

			if ( m_PartyMarkers.Get( uid ) == marker )
			{
				m_MarkerModule.GetData().PartyRemove( uid );

				return;
			}

			return;
		}

		m_DeletingMarkers.Insert( marker );
		m_DoUpdateMarkers = true;
		
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

	void DeletePreviewMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeletePreviewMarker - Start");
		#endif

		DeleteMarker( m_SelectedMarker );

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

			m_DeletingMarkers.Remove( i );

			i--;
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeletePreviewMarker - End");
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

			#ifdef EXPANSION_MAP_MENU_DEBUG
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current player position is: " + player_pos.ToString());
			#endif

			map_pos = Vector(0,0,0);

			//! only do this if the server want to show player pos (if not, we don't want to open the map on player pos)
			if ( GetExpansionSettings().GetMap().ShowPlayerPosition )
			{
				camera_pos = GetGame().GetCurrentCameraPosition();

				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current camera position is: " + camera_pos.ToString());
				#endif
				
				map_pos = camera_pos;

				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMenu::SetMapPosition:: Map Position is: " + map_pos.ToString());
				#endif
			}
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
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMapPosition - Start");
		#endif
		
		float scale;
		vector map_pos;
		
		scale = m_MapWidget.GetScale();
		map_pos = m_MapWidget.GetMapPos();
		
		PlayerBase m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if (m_Player)
			m_Player.SetLastMapInfo(scale, map_pos);
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
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
					PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

					if ( GetExpansionSettings().GetMap().NeedPenItemForCreateMarker && player.HasItemPen() )
					{
						#ifdef EXPANSION_MAP_MENU_DEBUG
						EXLogPrint("ExpansionMapMenu::OnDoubleClick - player has pen: " + player.HasItemPen().ToString() );
						#endif

						GetGame().GetMousePos( mouse_x, mouse_y );
						CreateNewMarker( mouse_x, mouse_y );
					}
					else if ( GetExpansionSettings().GetMap().NeedGPSItemForCreateMarker && player.HasItemGPS() )
					{
						#ifdef EXPANSION_MAP_MENU_DEBUG
						EXLogPrint("ExpansionMapMenu::OnDoubleClick - player has gps: " + player.HasItemGPS().ToString() );
						#endif

						GetGame().GetMousePos( mouse_x, mouse_y );
						CreateNewMarker( mouse_x, mouse_y );
					}
					else if ( !GetExpansionSettings().GetMap().NeedGPSItemForCreateMarker && !GetExpansionSettings().GetMap().NeedPenItemForCreateMarker )
					{
						GetGame().GetMousePos( mouse_x, mouse_y );
						CreateNewMarker( mouse_x, mouse_y );
					}

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
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnShow - Start");
		#endif

		m_IsShown = true;

		super.OnShow();

		m_OpenMapTime = 0;
		
		ToggleGPS();

		SetFocus(layoutRoot);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		PPEffects.SetBlurMenu(0.5);
		
		//Show();

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnShow - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnHide - Start");
		#endif

		if ( !m_IsShown )
			return;

		m_IsShown = false;

		super.OnHide();
		
		m_MarkerList.HideTooltips();

		ToggleGPS();

		m_MarkerModule.SaveLocalServerMarkers();

		if ( GetGame().GetMission() )
			GetGame().GetMission().PlayerControlEnable(true);
		
		PPEffects.SetBlurMenu( 0.0 );

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnHide - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Override Update
	// ------------------------------------------------------------	
	override void Update( float timeslice )
	{		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Update - Start");
		#endif

		m_OpenMapTime += timeslice;
		m_Time += timeslice;
		
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) )
		{
			CloseMapMenu();
			return;
		}
		
		if ( GetGame().GetInput().LocalPress( "UAExpansionMapToggle", false ) && m_OpenMapTime > 0.10 && !IsEditingMarker() )
		{
			CloseMapMenu();
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
		
		if ( m_DoUpdateMarkers )
		{
			UpdateMarkers();
		
			if ( m_PartyMarkersUpdated && m_PersonalMarkersUpdated && m_PlayerMarkersUpdated && m_ServerMarkersUpdated )
			{
				m_PartyMarkersUpdated = false;
				m_PersonalMarkersUpdated = false;
				m_PlayerMarkersUpdated = false;
				m_ServerMarkersUpdated = false;
				m_DoUpdateMarkers = false;
			}
		}

		if ( m_MarkerList.IsListVisible() && m_Time >= 0.25 )
		{
			UpdateMarkerList();
			m_Time = 0;
		}
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Update - End");
		#endif
	}
	
	private void UpdateMarkerList()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkerList - Start");
		#endif
		
		int j;
		ExpansionMapMarkerListEntry entry;
		for ( j = 0; j < m_MarkerList.GetPersonalEntrys().Count(); ++j )
		{			
			entry = m_MarkerList.GetPersonalEntrys()[j];
			if (entry)
				entry.Update();
		}
		
		for ( j = 0; j < m_MarkerList.GetPartyEntrys().Count(); ++j )
		{
			entry = m_MarkerList.GetPartyEntrys()[j];
			if (entry)
				entry.Update();
		}
		
		for ( j = 0; j < m_MarkerList.GetMemberEntrys().Count(); ++j )
		{
			entry = m_MarkerList.GetMemberEntrys()[j];			
			if (entry)
				entry.Update();
		}
		
		for ( j = 0; j < m_MarkerList.GetServerEntrys().Count(); ++j )
		{
			entry = m_MarkerList.GetServerEntrys()[j];			
			if (entry)
				entry.Update();
		}
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkerList - End");
		#endif
	}
	
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
	
	// ------------------------------------------------------------
	// Expansion IsEditingMarker
	// ------------------------------------------------------------
	override void InitMapItem(EntityAI item)
	{
		Print("ExpansionMapMenu::InitMapItem - Start");
		
		super.InitMapItem(item);
		
		m_Map = ItemMap.Cast(item);
		
		Print("ExpansionMapMenu::InitMapItem - End");
	}
	
	void CloseMapMenu(bool destroy = false)
	{
		Print("ExpansionMapMenu::CloseMapMenu - Start");
		
		if (m_Map)	
		{
			Print("ExpansionMapMenu::CloseMapMenu - Step 1");
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			if (player)
			{
				Print("ExpansionMapMenu::CloseMapMenu - Step 2");
				player.SetMapClosingSyncSet(false); //map is closing, server needs to be notified - once
			}
		}
		
		if (destroy)
			Close();
		else
			GetGame().GetUIManager().HideScriptedMenu( this );  //! Don't close, we do not want to have to redraw all the markers next time we open it
		
		Print("ExpansionMapMenu::CloseMapMenu - End");
	}
};