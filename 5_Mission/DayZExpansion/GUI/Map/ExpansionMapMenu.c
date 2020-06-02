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
	protected PlayerBase m_Player;

	protected MapWidget m_MapWidget;
	protected vector m_PositionCreateMarker;
	protected int COLOR_EXPANSION_MARKER_PLAYER_POSITION = ARGB( 255, 255, 180, 24 );
	
	protected ref ExpansionMapMenuMarkerWindow m_MarkerWindow;
	protected ref ExpansionMapMenuPositionArrow m_MapPositionArrow;

	protected ref array<ref ExpansionMapMenuMarker> m_MapMarkers;
	protected ref array<ref ExpansionMapMarker> m_MapSavedMarkers;
	
	protected ref array<ref ExpansionMapMenuMarker> m_MapPartyMarkers;
	protected ref array<ref ExpansionMapMarker> m_MapSavedPartyMarkers;

	protected ref array<ref ExpansionMapMenuServerMarker> m_MapServerMarkers;
	protected ref array<ref ExpansionMapMarker> m_MapSavedServerMarkers;

	protected ref array<ref ExpansionMapMenuPlayerMarker> m_MapPartyPlayerMarkers;
	
	protected bool m_IsEditingMarker;

	protected ref ExpansionMapMarkerModule m_MarkerModule;
	protected ref Expansion3DMarkerModule m_3DMarkerModule;
	protected ref ExpansionPartyModule m_PartyModule;
	
	ref ExpansionMapMenuMarker m_TempMarker;
	
	private bool m_GpsWasOpen = false;
	
	private float m_OpenMapTime = 0;

	// ------------------------------------------------------------
	// Expansion ExpansionMapMenu Constructor
	// ------------------------------------------------------------	
	void ExpansionMapMenu()
	{
		if (!m_MarkerModule)
			m_MarkerModule = ExpansionMapMarkerModule.Cast(GetModuleManager().GetModule(ExpansionMapMarkerModule));
		
		if (!m_3DMarkerModule)
			m_3DMarkerModule = Expansion3DMarkerModule.Cast(GetModuleManager().GetModule(Expansion3DMarkerModule));
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
	}
	
	void ~ExpansionMapMenu()
	{
		
	}

	// ------------------------------------------------------------
	// Expansion Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map.layout");
		m_MapWidget = MapWidget.Cast(layoutRoot.FindAnyWidget("Map"));
		m_MapMarkers = new array<ref ExpansionMapMenuMarker>;
		m_MapSavedMarkers = new array<ref ExpansionMapMarker>;
		m_MapPartyMarkers = new array<ref ExpansionMapMenuMarker>;
		m_MapSavedPartyMarkers = new array<ref ExpansionMapMarker>;
		m_MapServerMarkers = new array<ref ExpansionMapMenuServerMarker>;
		m_MapSavedServerMarkers = new array<ref ExpansionMapMarker>;
		m_MapPartyPlayerMarkers = new array<ref ExpansionMapMenuPlayerMarker>;

		if (!m_3DMarkerModule)
			m_3DMarkerModule = Expansion3DMarkerModule.Cast(GetModuleManager().GetModule(Expansion3DMarkerModule));
		
		LoadMapExtentions();

		if (m_MapWidget)
			SetMapPosition();

		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion Show
	// ------------------------------------------------------------	
	void Show()
	{
		if (layoutRoot)
			layoutRoot.Show(true);
	}
	
	// ------------------------------------------------------------
	// Expansion Hide
	// ------------------------------------------------------------	
	void Hide()
	{
		if (layoutRoot)
			layoutRoot.Show(false);
	}
	
	// ------------------------------------------------------------
	// Expansion Override LoadMapExtentions
	// Loads additional map extentions and event handler
	// ------------------------------------------------------------	
	void LoadMapExtentions()
	{
		// Marker creation Window
		if (GetExpansionSettings() && GetExpansionSettings().GetMap().CanCreateMarker)
		{
			if (!m_MarkerWindow)	
			{			
				m_MarkerWindow = new ExpansionMapMenuMarkerWindow(layoutRoot, this, m_MapWidget);
				m_MarkerWindow.ShowMarkerWindow(false);
			}
		}
		
		// Map player position Arrow
		if (GetExpansionSettings() && GetExpansionSettings().GetMap().ShowPlayerPosition)
		{
			if (!m_MapPositionArrow)
			{
				m_MapPositionArrow = new ExpansionMapMenuPositionArrow(layoutRoot, this);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion LoadMarkers
	// Events when loading all existing (saved) map markers
	// ------------------------------------------------------------	
	void LoadMarkers()
	{
		if ( !GetExpansionClientSettings().Show2DMarkers )
			return;
		
		LoadPersonnalMarkers();
		LoadServerMarkers();
		LoadPartyMarkers();
		m_3DMarkerModule.RefreshMarkers();
	}

	// ------------------------------------------------------------
	// Expansion LoadPersonnalMarkers
	// Events load personnal stored markers on map
	// ------------------------------------------------------------
	void LoadPersonnalMarkers()
	{
		if ( !GetExpansionClientSettings().Show2DClientMarkers )
			return;

		m_MapMarkers.Clear();
		m_MapSavedMarkers.Clear();
		
		if (m_MarkerModule && m_MarkerModule.GetMarkers() && m_MarkerModule.GetMarkers().Count() >= 0)
		{
			for (int i = 0; i < m_MarkerModule.GetMarkers().Count(); ++i)
			{
				ExpansionMapMarker currentMarker = m_MarkerModule.GetMarker(i);
				ExpansionMapMenuMarker mapMarker = new ExpansionMapMenuMarker(layoutRoot, m_MapWidget, currentMarker.GetMarkerPosition(), " " + currentMarker.GetMarkerText(), currentMarker.GetMarkerColor(), ExpansionMarkerIcons.GetMarkerPath(currentMarker.GetIconIndex()), currentMarker);
				m_MapMarkers.Insert(mapMarker);
				m_MapSavedMarkers.Insert(currentMarker);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion AddPersonalMarker
	// Events when adding personal map marker on map
	// Adds a map marker and saved maker entry
	// ------------------------------------------------------------	
	void AddPersonalMarker(string name, int icon, vector pos, int color, bool is3D)
	{
		ExpansionMapMarker newMarker = m_MarkerModule.CreateClientMarker(name, icon, pos, color, false, is3D);
		ExpansionMapMenuMarker mapMarker = new ExpansionMapMenuMarker(layoutRoot, m_MapWidget, pos, name, color, ExpansionMarkerIcons.GetMarkerPath(icon), newMarker);
		m_MapMarkers.Insert(mapMarker);
		m_MapSavedMarkers.Insert(newMarker);
		
		LoadMarkers();
	}
	
	// ------------------------------------------------------------
	// Expansion CreateTempMarker
	// ------------------------------------------------------------	
	void CreateTempMarker(vector pos)
	{
		if (m_TempMarker)
			m_TempMarker = null;
		
		ExpansionMapMenuMarker tempMarker = new ExpansionMapMenuMarker(layoutRoot, m_MapWidget, pos, "NEW MARKER", ARGB(255, 0, 0, 0), ExpansionMarkerIcons.GetMarkerPath(1), NULL);
		m_TempMarker = tempMarker;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTempMarker
	// ------------------------------------------------------------
	ExpansionMapMenuMarker GetTempMarker()
	{
		return m_TempMarker;
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveTempMarker
	// ------------------------------------------------------------	
	void RemoveTempMarker()
	{
		if (!m_TempMarker || m_TempMarker == NULL)
			return;
		
		m_TempMarker.DeleteMarker();
		m_TempMarker = null;
	}

	// ------------------------------------------------------------
	// Expansion RemovePersonalMarker
	// Events when removing personal map marker on map
	// ------------------------------------------------------------	
	void RemovePersonalMarker(string name)
	{
		int index;
		string markerName;
		string markerName2;

		if ( name != "")
		{
			markerName = name;
			markerName.ToLower();

			if (m_MapMarkers && m_MapMarkers.Count() >= 0)
			{
				//! Delete map marker
				for (int i = 0; i < m_MapMarkers.Count(); ++i)
				{
					ExpansionMapMenuMarker mapmarker = m_MapMarkers.Get(i);
					if (mapmarker)
					{
						markerName2 = mapmarker.GetMarkerName();
						markerName2.ToLower();
		
						if ( markerName == markerName2 )
						{
							index = m_MapMarkers.Find(mapmarker);
		
							if ( index > -1 )
							{
								delete m_MapMarkers[index];
								m_MapMarkers.Remove(index);
							}
						}
					}
				}
			}
			
			if (m_MarkerModule && m_MarkerModule.GetMarkers() && m_MarkerModule.GetMarkers().Count() >= 0)
			{
				//! Delete saved marker entry
				for (int j = 0; j < m_MarkerModule.GetMarkers().Count(); ++j)
				{
					ExpansionMapMarker savedmarker = m_MarkerModule.GetMarker(j);
					if (savedmarker)
					{
						markerName2 = savedmarker.GetMarkerText();
						markerName2.ToLower();
		
						if ( markerName == markerName2 )
						{
							m_MarkerModule.RemoveMarker(savedmarker);
							index = m_MapSavedMarkers.Find(savedmarker);
		
							if ( index > -1 )
							{
								delete m_MapSavedMarkers[index];
								m_MapSavedMarkers.Remove(index);
							}
						}
					}
				}
			}
			
			LoadMarkers();
		}

		if (!m_IsEditingMarker)	
			GetNotificationSystem().CreateNotification(new StringLocaliser("STR_EXPANSION_MARKER_SYSTEM_TITLE"), new StringLocaliser("STR_EXPANSION_MARKER_SYSTEM_REMOVED_MARKER"), EXPANSION_NOTIFICATION_ICON_CHECK, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5 );
	}

	// ------------------------------------------------------------
	// Expansion LoadPartyMarkers
	// Events load party markers on map
	// ------------------------------------------------------------	
	void LoadPartyMarkers()
	{
		m_MapPartyMarkers.Clear();
		m_MapSavedPartyMarkers.Clear();
		m_MapPartyPlayerMarkers.Clear();
		
		if (m_PartyModule.HasParty())
		{
			array<ref ExpansionMapMarker> markers = m_PartyModule.GetParty().GetAllMarkers();
			
			if (markers.Count() > 0)
			{
				for (int i = 0; i < markers.Count(); ++i)
				{
					ExpansionMapMarker currentMarker = markers.Get(i);
					if (currentMarker)
					{
						ExpansionMapMenuMarker mapMarker = new ExpansionMapMenuMarker( layoutRoot, m_MapWidget, currentMarker.GetMarkerPosition(), " " + currentMarker.GetMarkerText(), currentMarker.GetMarkerColor(), ExpansionMarkerIcons.GetMarkerPath(currentMarker.GetIconIndex()), currentMarker );
						m_MapPartyMarkers.Insert( mapMarker );
						m_MapSavedPartyMarkers.Insert( currentMarker );
					}
				}
			}
			
			// Party member Markers
			if ( GetExpansionSettings() && GetExpansionSettings().GetMap().ShowPartyMembersMapMarkers )
			{
				PlayerBase m_Player = PlayerBase.Cast( GetGame().GetPlayer() );		

				if ( m_Player )
				{
					ref array< ref ExpansionPartySaveFormatPlayer > players = m_PartyModule.GetParty().GetPlayers();
					if (players)
					{
						for (int j = 0; j < players.Count(); ++j)
						{
							ref ExpansionPartySaveFormatPlayer currPlayer = players[j];
							if (!currPlayer) continue;
							
							if ( currPlayer.UID != m_Player.GetIdentityUID() )
							{
								PlayerBase player = PlayerBase.GetPlayerByUID(currPlayer.UID);
								if (player)
								{
									ExpansionMapMenuPlayerMarker playerMarker = new ExpansionMapMenuPlayerMarker(layoutRoot, m_MapWidget, player );
									m_MapPartyPlayerMarkers.Insert(playerMarker);
								}
							}
						}
					}
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion AddPartyMarker
	// Events when creating new party map marker on map
	// Adds a map marker and saved maker entry
	// ------------------------------------------------------------	
	void AddPartyMarker(string name, int icon, vector pos, int color, bool is3D)
	{
		ExpansionMapMarker newMarker = m_MarkerModule.CreateClientMarker(name, icon, pos, color, true, is3D);
		ExpansionMapMenuMarker mapMarker = new ExpansionMapMenuMarker(layoutRoot, m_MapWidget, pos, name, color, ExpansionMarkerIcons.GetMarkerPath(icon), newMarker);
		m_MapPartyMarkers.Insert(mapMarker);
		m_MapSavedPartyMarkers.Insert(newMarker);
		
		LoadMarkers();
	}

	// ------------------------------------------------------------
	// Expansion RemovePartyMarker
	// Events when removing party map marker on map
	// ------------------------------------------------------------	
	void RemovePartyMarker(string name)
	{
		int index;
		string markerName;
		string markerName2;
		ExpansionMapMarker currentMarker;
		ExpansionMapMarker removeMarker;

		if ( name != "")
		{
			markerName = name;
			markerName.ToLower();

			//! Delete map marker
			for ( int i = 0; i < m_MapPartyMarkers.Count(); ++i )
			{
				ExpansionMapMenuMarker marker = m_MapPartyMarkers.Get(i);
				if (marker)
				{
					markerName2 = marker.GetMarkerName();
					markerName2.ToLower();
	
					if ( markerName == markerName2 )
					{
						index = m_MapPartyMarkers.Find(marker);
	
						if ( index > -1 )
						{
							delete m_MapPartyMarkers[index];
							m_MapPartyMarkers.Remove(index);
						}
					}
				}
			}
			
			//! Search saved party marker entry
			if ( m_PartyModule.HasParty() )
			{
				array<ref ExpansionMapMarker> markers = m_PartyModule.GetParty().GetAllMarkers();
				
				if (markers.Count() > 0)
				{
					for (int k = 0; k < markers.Count(); ++k)
					{
						currentMarker = markers.Get(k);
						markerName2 = currentMarker.GetMarkerText();
						markerName2.ToLower();
	
						if (markerName == markerName2)
						{
							removeMarker = currentMarker;
						}
					}
				}
			}

			//! Delete saved party marker entry
			if (removeMarker)
			{
				if (removeMarker.IsPartyMarker())
				{
					ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));

					if (partyModule)
					{
						partyModule.DeleteMarker( removeMarker.GetMarkerText() );
						index = m_MapSavedPartyMarkers.Find(removeMarker);

						if ( index > -1 )
						{
							delete m_MapSavedPartyMarkers[index];
							m_MapSavedPartyMarkers.Remove(index);
									
							Refresh();
						}
					}
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion LoadServerMarkers
	// Events load server markers on map
	// ------------------------------------------------------------	
	void LoadServerMarkers()
	{
		m_MapServerMarkers.Clear();
		m_MapSavedServerMarkers.Clear();

		if ( !m_MarkerModule || m_MarkerModule.ServerCount() < 1 )
			return;
		
		if ( !GetExpansionClientSettings().Show2DGlobalMarkers )
			return;

		if ( GetExpansionSettings() && GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().ShowServerMarkers )
		{
			ExpansionMapMarker currentMarker;
			ExpansionMapMenuServerMarker mapMarker;
	
			for ( int i = 0; i < m_MarkerModule.ServerCount(); ++i )
			{
				currentMarker = m_MarkerModule.GetServerMarker( i );
				if (currentMarker)
				{
					mapMarker = new ExpansionMapMenuServerMarker(layoutRoot, m_MapWidget, currentMarker.GetMarkerPosition(), " " + currentMarker.GetMarkerText(), currentMarker.GetMarkerColor(), ExpansionMarkerIcons.GetMarkerPath(currentMarker.GetIconIndex()));
					
					m_MapServerMarkers.Insert(mapMarker);
					m_MapSavedServerMarkers.Insert(currentMarker);
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion AddServerMarker
	// Events when creating new server map marker on map
	// ------------------------------------------------------------	
	void AddServerMarker(string name, int icon, vector pos, int color, bool is3D)
	{	
		int index = m_MarkerModule.AddServerMarker(name, icon, pos, color, is3D);

		if (index > -1)
		{
			ExpansionMapMarker newMarker = ExpansionMapMarker.Cast(m_MarkerModule.GetServerMarker(index));
			ExpansionMapMenuServerMarker mapMarker = new ExpansionMapMenuServerMarker(layoutRoot, m_MapWidget, pos, name, color, ExpansionMarkerIcons.GetMarkerPath(icon));
			m_MapServerMarkers.Insert(mapMarker);
			m_MapSavedServerMarkers.Insert(newMarker);
			
			LoadMarkers();
		}
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
	}
	
	// ------------------------------------------------------------
	// Expansion GetCurrentMarkerPosition
	// ------------------------------------------------------------	
	vector GetCurrentMarkerPosition()
	{
		return m_PositionCreateMarker;
	}
	
	// ------------------------------------------------------------
	// Expansion SetCurrentMarkerPosition
	// ------------------------------------------------------------
	void SetCurrentMarkerPosition(vector pos)
	{
		m_PositionCreateMarker = pos;
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
		float minimumDistance = 1000000000;
		float distance;
		const float distanceFromCursor = 75;
		int mouse_x;
		int mouse_y;
		GetGame().GetMousePos( mouse_x, mouse_y );
		vector positionCursor = m_MapWidget.ScreenToMap(Vector( mouse_x, mouse_y, 0 ));
		ExpansionMapMarker currentMarker;
		ExpansionMapMarker removeMarker;
		string removeMarkerName;

		//! Check if marker is personal marker
		for (int i = 0; i < m_MarkerModule.Count(); ++i)
		{
			currentMarker = m_MarkerModule.GetMarker(i);

			distance = vector.Distance( currentMarker.GetMarkerPosition(), positionCursor );
			if ( distance < distanceFromCursor && distance < minimumDistance )
			{
				removeMarker = currentMarker;
				minimumDistance = distance;
			}
		}

		//! Delete personal marker
		if (removeMarker)
		{
			removeMarkerName = removeMarker.GetMarkerText();
			RemovePersonalMarker(removeMarkerName);
		}

		// Check if marker is party marker
		if (m_PartyModule.HasParty())
		{
			ref array<ref ExpansionMapMarker> markers = m_PartyModule.GetParty().GetAllMarkers();
			if (markers.Count() > 0)
			{
				for (int j = 0; j < markers.Count(); ++j)
				{
					currentMarker = markers.Get(j);
	
					if (distance < distanceFromCursor && distance < minimumDistance)
					{
						removeMarker = currentMarker;
						minimumDistance = distance;
					}
				}
			}
			
			//! Delete party marker
			if (removeMarker && m_PartyModule.HasParty())
			{
				if (removeMarker.IsPartyMarker())
				{
					ExpansionPartyModule module;
					if (Class.CastTo( module, GetModuleManager().GetModule(ExpansionPartyModule)))
					{
						removeMarkerName = removeMarker.GetMarkerText();
						RemovePartyMarker(removeMarkerName);
					}
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion SetEditingMarker
	// ------------------------------------------------------------	
	void SetEditingMarker(bool state)
	{
		m_IsEditingMarker = state;
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarkerWindow
	// ------------------------------------------------------------	
	ExpansionMapMenuMarkerWindow GetMarkerWindow()
	{
		return m_MarkerWindow;
	}

	// ------------------------------------------------------------
	// Expansion GetPlayerPositionArrow
	// ------------------------------------------------------------	
	ref ExpansionMapMenuPositionArrow GetPlayerPositionArrow()
	{
		if (!m_MapPositionArrow)
		{
			m_MapPositionArrow = new ExpansionMapMenuPositionArrow(layoutRoot, this);
		}
			
		return m_MapPositionArrow;
	}
	
	// ------------------------------------------------------------
	// Expansion SetMapPosition
	// ------------------------------------------------------------	
	void SetMapPosition()
	{
		float scale;
		vector map_pos;
		vector player_pos;
		vector camera_pos;
		
		PlayerBase m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		if(m_Player && !m_Player.GetLastMapInfo(scale,map_pos))
		{
			//! Lower number zooms in / Higher number zooms out
			scale = 0.10; // Float between 0-1 ?!
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current scale is: " + scale.ToString());
			#endif

			player_pos = m_Player.GetPosition();
			camera_pos = GetGame().GetCurrentCameraPosition();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current player position is: " + player_pos.ToString());
			#endif

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Current camera position is: " + camera_pos.ToString());
			#endif
			
			map_pos = camera_pos;

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenu::SetMapPosition:: Map Position is: " + map_pos.ToString());
			#endif
		}

		m_MapWidget.SetScale(scale);
		m_MapWidget.SetMapPos(map_pos);

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapMenu::SetMapPosition:: SetMapPos with values: " + map_pos.ToString());
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapMenu::SetMapPosition:: GetMapPos is: " + m_MapWidget.GetMapPos().ToString());
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateMapPosition
	// ------------------------------------------------------------	
	void UpdateMapPosition()
	{
		float scale;
		vector map_pos;
		
		scale = m_MapWidget.GetScale();
		map_pos = m_MapWidget.GetMapPos();
		
		PlayerBase m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if (m_Player)
			m_Player.SetLastMapInfo(scale, map_pos);
	}

	// ------------------------------------------------------------
	// Override OnDoubleClick
	// ------------------------------------------------------------
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{		
		bool superbool = super.OnDoubleClick( w, x, y, button );
		
		int mouse_x;
		int mouse_y;

		if ( button == MouseState.LEFT && !m_MarkerWindow.GetMarkerWindowState() )
		{
			if ( w == m_MapWidget && GetExpansionSettings().GetMap().CanCreateMarker )
			{		
				GetGame().GetMousePos( mouse_x, mouse_y );
				m_PositionCreateMarker = m_MapWidget.ScreenToMap( Vector( mouse_x, mouse_y, 0 ) );
				m_MarkerWindow.ShowMarkerWindow( true );
				m_MarkerWindow.ShowMapStatWindow( true );
				
				if ( !m_TempMarker || m_TempMarker == NULL )
				{
					CreateTempMarker( m_PositionCreateMarker );
				}
				
				return true;
			}
		}
		else if ( button == MouseState.LEFT && m_MarkerWindow.GetMarkerWindowState() )
		{
			if ( w == m_MapWidget && GetExpansionSettings().GetMap().CanCreateMarker )
			{
				GetGame().GetMousePos (mouse_x, mouse_y );
				m_PositionCreateMarker = m_MapWidget.ScreenToMap( Vector( mouse_x, mouse_y, 0 ) );

				if ( m_TempMarker )
				{
					m_TempMarker.ChangePositon(m_PositionCreateMarker);
				}
				
				if ( m_MarkerWindow.GetCurrentMapMarker() )
				{
					m_MarkerWindow.GetCurrentMapMarker().ChangePositon( m_PositionCreateMarker );
				}
				
				return true;
			}
		}

		if (button == MouseState.LEFT && !GetMarkerWindow().GetArrowColorPanelState())
		{
			if ((w == GetPlayerPositionArrow().GetArrowWidget()) && GetExpansionSettings().GetMap().ShowPlayerPosition)
			{
				GetMarkerWindow().ShowArrowColorPanel(true);
				
				return true;
			}
		}
		
		return superbool;
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

		LoadMarkers();
		
		GetMarkerWindow().LoadArrowColor();
		
		Show();
	}
	
	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();
		
		ToggleGPS();

		GetGame().GetMission().PlayerControlEnable(true);
		
		PPEffects.SetBlurMenu( 0.0 );

		if (m_IsEditingMarker)
			m_MarkerWindow.CancelMarkerEdit();
		else
			m_MarkerWindow.ClearChages();
	}

	// ------------------------------------------------------------
	// Expansion Override Update
	// ------------------------------------------------------------	
	override void Update( float timeslice )
	{		
		m_OpenMapTime += timeslice;
		
		UpdateMapPosition();

		if( GetGame().GetInput().LocalPress( "UAUIBack", false ) )
		{
			Hide();
			Close();
		}		
		
		if( GetGame().GetInput().LocalPress( "UAExpansionMapToggle", false ) && m_OpenMapTime > 0.75 && !m_MarkerWindow.GetMarkerWindowState() )
		{
			Hide();
			Close();
		}
		
		if( GetGame().GetInput().LocalPress( "UAExpansionMapDeleteMarker", false ) )
		{
			RemoveMarker();
		}
		
		if( layoutRoot.IsVisible() )
		{
			m_MarkerWindow.ShowPartyMarkerOption( m_PartyModule.HasParty() );
			
			for (int i = 0; i < m_MapMarkers.Count(); ++i)
			{
				if (m_MapMarkers[i]) m_MapMarkers[i].Update( timeslice );
			}
			
			for (int j = 0; j < m_MapPartyMarkers.Count(); ++j)
			{
				if (m_MapPartyMarkers[j]) m_MapPartyMarkers[j].Update( timeslice );
			}
			
			for (int k = 0; k < m_MapPartyPlayerMarkers.Count(); ++k)
			{
				if (m_MapPartyPlayerMarkers[k]) m_MapPartyPlayerMarkers[k].Update( timeslice );
			}
			
			m_MapPositionArrow.Update( timeslice );
		}
	}
}