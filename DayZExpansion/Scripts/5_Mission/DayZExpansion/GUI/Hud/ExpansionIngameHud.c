/**
 * ExpansionIngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGPSUI extends ScriptedWidgetEventHandler
{	
	protected Widget m_Root;
	
	// ------------------------------------------------------------
	// Expansion ExpansionGPSUI Constructor
	// ------------------------------------------------------------
	void ExpansionGPSUI(Widget parent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPSUI::ExpansionGPSUI - Start");
		#endif
	
		m_Root					= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/hud/expansion_gps.layout", parent ) );
		
		m_Root.SetHandler(this);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPSUI::ExpansionGPSUI - End");
		#endif
	}
	
		
	// ------------------------------------------------------------
	// Expansion ExpansionPartyTabInvitesListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionGPSUI()
	{
		delete m_Root;
	}
}


class ExpansionIngameHud
{
	//! HUD UI META
	protected Widget										m_WgtRoot;
	protected ref ExpansionIngameHudEventHandler 			m_ExpansionEventHandler;
	
	//! CONDITIONS
	protected bool											m_ExpansionHudState;
	protected bool											m_ExpansionHudDebuggerState;
	protected bool 											m_ExpansionHudGPSState;
	protected bool											m_ExpansionHudGPSMapState;
	protected bool											m_ExpansionHudGPSMapStatsState;
	protected bool											m_ExpansionHudNVState;
	protected bool											m_ExpansionClockState;
	protected bool											m_ExpansionEarplugState;
	protected bool 											m_ExpansionGPSSetting;
	protected bool											m_ExpansionGPSPosSetting;
	protected bool											m_ExpansionNVSetting;
	protected bool											m_ClientClockShow;
	
	//! GPS
	protected Widget										m_GPSPanel;
	protected Widget										m_MapStatsPanel;
	protected Widget 										m_MapPosPanel;
	protected TextWidget 									m_PlayerPosVal;
	protected Widget 										m_PlayerPosPanel;
	protected TextWidget									m_PlayerALTVal;
	protected Widget 										m_PlayerALTPanel;
	protected TextWidget 									m_PlayerDirVal;
	protected Widget 										m_PlayerDirPanel;
	
	protected Widget										m_GPSMapPanel;
	protected Widget										m_MapFrame;
	protected MapWidget										m_MapWidget;
	protected ref ExpansionGPSPositionArrow 				m_MapPositionArrow;
	
	protected float											m_GPSMapScale = 0.1;
	
	//! NIGHTVISION
	protected Widget 										m_NVPanel;
	protected ImageWidget 									m_NVOverlayImage;
	//! CLOCK
	protected Widget										m_ClockPanel;
	protected TextWidget									m_Time;
	//! EARPLUG
	protected ImageWidget 									m_EarPlugIcon;

	//! DEBUGER
	protected MultilineTextWidget							m_ExpansionDebug;

	//! MISC
	protected WrapSpacerWidget 								m_RightHUDPanel;
	
	protected ref ExpansionGPSUI							m_GPSUI;
	
	protected ref array<ref ExpansionMapMenuMarker> 		m_MapMarkers;
	protected ref array<ref ExpansionMapMarker> 			m_MapSavedMarkers;
	
	protected ref array<ref ExpansionMapMenuMarker> 		m_MapPartyMarkers;
	protected ref array<ref ExpansionMapMarker> 			m_MapSavedPartyMarkers;

	protected ref array<ref ExpansionMapMenuServerMarker> 	m_MapServerMarkers;
	protected ref array<ref ExpansionMapMarker> 			m_MapSavedServerMarkers;

	protected ref array<ref ExpansionMapMenuPlayerMarker> 	m_MapPartyPlayerMarkers;
	
	//! MARKER MODULES
	protected ref ExpansionMapMarkerModule 					m_MarkerModule;
	protected ref ExpansionPartyModule 						m_PartyModule;

	//============================================
	// ExpansionIngameHud Constructor
	//============================================
	void ExpansionIngameHud()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::ExpansionIngameHud Start");
		#endif
		
		m_ExpansionEarplugState = false;
		
		if (!m_MarkerModule)
			m_MarkerModule = ExpansionMapMarkerModule.Cast(GetModuleManager().GetModule(ExpansionMapMarkerModule));
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
		
		m_MapMarkers = new array<ref ExpansionMapMenuMarker>;
		m_MapSavedMarkers = new array<ref ExpansionMapMarker>;
		m_MapPartyMarkers = new array<ref ExpansionMapMenuMarker>;
		m_MapSavedPartyMarkers = new array<ref ExpansionMapMarker>;
		m_MapServerMarkers = new array<ref ExpansionMapMenuServerMarker>;
		m_MapSavedServerMarkers = new array<ref ExpansionMapMarker>;
		m_MapPartyPlayerMarkers = new array<ref ExpansionMapMenuPlayerMarker>;
		
		GetExpansionClientSettings().SI_UpdateSetting.Insert( RefreshExpansionHudVisibility );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::ExpansionIngameHud End");
		#endif
	}

	//============================================
	// ~ExpansionIngameHud Destructor
	//============================================
	void ~ExpansionIngameHud()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::~ExpansionIngameHud Start");
		#endif
		
		GetExpansionClientSettings().SI_UpdateSetting.Remove( RefreshExpansionHudVisibility );

		delete m_WgtRoot;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::~ExpansionIngameHud End");
		#endif
	}
	
	//============================================
	// Expansion Init
	//============================================
	void Init( Widget hud_panel_widget )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::Init Start");
		#endif
		m_WgtRoot = hud_panel_widget;
		m_WgtRoot.Show( true );		
		
		/*m_RightHUDPanel 						= WrapSpacerWidget.Cast( m_WgtRoot.FindAnyWidget("RightHUDPanel") );
		if ( !m_GPSUI )
			m_GPSUI = new ExpansionGPSUI( m_RightHUDPanel );*/
		
		//! GPS
		m_GPSPanel								= Widget.Cast( m_WgtRoot.FindAnyWidget("GPSPanel") );
		m_GPSPanel.Show( false );
		if ( m_GPSPanel )
		{
			m_MapStatsPanel							= Widget.Cast( m_GPSPanel.FindAnyWidget("MapStatsPanel") );
			m_MapPosPanel 							= Widget.Cast( m_GPSPanel.FindAnyWidget("cursor_position") );
			m_PlayerPosVal 							= TextWidget.Cast( m_GPSPanel.FindAnyWidget("player_position_value") );
			m_PlayerPosPanel 						= Widget.Cast( m_GPSPanel.FindAnyWidget("player_position") );
			m_PlayerALTVal 							= TextWidget.Cast( m_GPSPanel.FindAnyWidget("player_alt_value") );
			m_PlayerALTPanel 						= Widget.Cast( m_GPSPanel.FindAnyWidget("player_alt") );
			m_PlayerDirVal							= TextWidget.Cast( m_GPSPanel.FindAnyWidget("player_direction_value") );
			m_PlayerDirPanel 						= Widget.Cast( m_GPSPanel.FindAnyWidget("player_direction") );
			
			m_GPSMapPanel							= Widget.Cast( m_GPSPanel.FindAnyWidget("GPSMapPanel") );
			m_MapFrame								= Widget.Cast( m_GPSPanel.FindAnyWidget("GPSMapFrame") );
			m_MapWidget 							= MapWidget.Cast( m_GPSPanel.FindAnyWidget("Map") );
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "ExpansionIngameHud::Init - m_MapFrame: " + m_MapFrame.ToString() );
			#endif
			
			m_MapPositionArrow = new ExpansionGPSPositionArrow( m_WgtRoot, this );
			m_MapPositionArrow.Show(false);
		}
		
		//! NIGHTVISION OVERLAY
		m_NVPanel 								= Widget.Cast( m_WgtRoot.FindAnyWidget("NVPanel") );
		if (m_NVPanel)
		{
			m_NVOverlayImage 						= ImageWidget.Cast( m_NVPanel.FindAnyWidget("NVOverlay") );
		}
				
		//! CLOCK
		m_ClockPanel							= Widget.Cast( m_WgtRoot.FindAnyWidget("TimePanel") );
		m_ClockPanel.Show( false );
		if (m_ClockPanel)
		{
			m_Time									= TextWidget.Cast( m_WgtRoot.FindAnyWidget("Time") );
		}
		
		m_EarPlugIcon 							= ImageWidget.Cast( m_WgtRoot.FindAnyWidget("EarPlug_Icon") );
		
		Class.CastTo( m_ExpansionDebug, m_WgtRoot.FindAnyWidget( "ExpansionDebugger" ) );
		
		//! SET UI EVENT HANDLER
		m_ExpansionEventHandler = new ExpansionIngameHudEventHandler( this );
		m_WgtRoot.SetHandler( m_ExpansionEventHandler );
		
		m_ExpansionHudState = g_Game.GetProfileOption( EDayZProfilesOptions.HUD );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHud::Init End");
		#endif
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
	}
	
	// ------------------------------------------------------------
	// Expansion LoadPersonnalMarkers
	// Events load personnal stored markers on gpsmap
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
				ExpansionMapMenuMarker mapMarker = new ExpansionMapMenuMarker( m_MapFrame, m_MapWidget, currentMarker.GetMarkerPosition(), " " + currentMarker.GetMarkerText(), currentMarker.GetMarkerColor(), ExpansionMarkerIcons.GetMarkerPath(currentMarker.GetIconIndex()), currentMarker );
				m_MapMarkers.Insert(mapMarker);
				m_MapSavedMarkers.Insert(currentMarker);
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

		if ( GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().ShowServerMarkers )
		{
			ExpansionMapMarker currentMarker;
			ExpansionMapMenuServerMarker mapMarker;
	
			for ( int i = 0; i < m_MarkerModule.ServerCount(); ++i )
			{
				currentMarker = m_MarkerModule.GetServerMarker( i );
				if (currentMarker)
				{
					mapMarker = new ExpansionMapMenuServerMarker( m_MapFrame, m_MapWidget, currentMarker.GetMarkerPosition(), " " + currentMarker.GetMarkerText(), currentMarker.GetMarkerColor(), ExpansionMarkerIcons.GetMarkerPath(currentMarker.GetIconIndex()) );
					
					m_MapServerMarkers.Insert(mapMarker);
					m_MapSavedServerMarkers.Insert(currentMarker);
				}
			}
		}
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
						ExpansionMapMenuMarker mapMarker = new ExpansionMapMenuMarker( m_MapFrame, m_MapWidget, currentMarker.GetMarkerPosition(), " " + currentMarker.GetMarkerText(), currentMarker.GetMarkerColor(), ExpansionMarkerIcons.GetMarkerPath(currentMarker.GetIconIndex()), currentMarker );
						m_MapPartyMarkers.Insert( mapMarker );
						m_MapSavedPartyMarkers.Insert( currentMarker );
					}
				}
			}
			
			// Party member Markers
			if ( GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().ShowPartyMembersMapMarkers )
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
									ExpansionMapMenuPlayerMarker playerMarker = new ExpansionMapMenuPlayerMarker( m_MapFrame, m_MapWidget, player );
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
	
	//============================================
	// Expansion UpdateExpansionDebugText
	//============================================
	void UpdateExpansionDebugText()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateExpansionDebugText Start");
		#endif
		
		array< ExpansionDebuggerBlock > blocks = new array< ExpansionDebuggerBlock >;
		ExpansionDebugger.GetEnabledBlocks( blocks );

		array< string > text = new array< string >();
		for ( int i = 0; i < blocks.Count(); i++ )
		{
			for ( int j = 0; j < blocks[i]._readyBuffer.Count(); j++ )
			{
				text.Insert( blocks[i]._readyBuffer[j] );
			}
		}

		string txt = "";
		for ( i = 0; i < text.Count(); i++ )
		{
			txt = txt + " \n\r " + text[i];
		}

		for ( int k = i; k < 256; k++ )
		{
			txt = txt + " \n\r ";
		}

		if ( m_ExpansionDebug )
			m_ExpansionDebug.SetText( txt );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateExpansionDebugText End");
		#endif
	}
	
	//============================================
	// Expansion Show
	//============================================
	void Show( bool show )
	{
		m_WgtRoot.Show( show );
	}
	
	//============================================
	// Expansion Update
	//============================================
	void Update( float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::Update Start");
		#endif

		RefreshExpansionHudVisibility();

		UpdateExpansionDebugText();
		
		if ( m_GPSMapPanel.IsVisible() )
		{
			for (int i = 0; i < m_MapMarkers.Count(); ++i)
			{
				if (m_MapMarkers[i])
					m_MapMarkers[i].Update( timeslice );
			}
			
			for (int j = 0; j < m_MapPartyMarkers.Count(); ++j)
			{
				if (m_MapPartyMarkers[j])
					m_MapPartyMarkers[j].Update( timeslice );
			}
			
			for (int k = 0; k < m_MapPartyPlayerMarkers.Count(); ++k)
			{
				if (m_MapPartyPlayerMarkers[k])
					m_MapPartyPlayerMarkers[k].Update( timeslice );
			}
		}
		
		if (m_ExpansionEventHandler)
			m_ExpansionEventHandler.Update(timeslice);

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::Update End");
		#endif
	}
	
	//============================================
	// Expansion RefreshExpansionHudVisibility
	//============================================
	void RefreshExpansionHudVisibility()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::RefreshExpansionHudVisibility Start");
		#endif
		
		m_ExpansionGPSSetting = GetExpansionSettings().GetGeneral().EnableHUDGPS;
		m_ExpansionGPSPosSetting = GetExpansionSettings().GetMap().ShowPlayerPosition;
		m_ExpansionNVSetting = GetExpansionSettings().GetGeneral().EnableHUDNightvisionOverlay; 
		
		
		if ( m_ExpansionDebug )
			m_ExpansionDebug.Show( m_ExpansionHudState && m_ExpansionHudDebuggerState );
		
		if ( m_GPSPanel )
		{
			m_GPSPanel.Show( m_ExpansionHudState && m_ExpansionHudGPSState && m_ExpansionGPSSetting );
			m_MapPositionArrow.Show( m_ExpansionHudState && m_ExpansionHudGPSState && m_ExpansionGPSSetting && m_ExpansionGPSPosSetting );
			if ( m_GPSPanel.IsVisible() )
			{
				UpdateGPS();
			}
		}
		
		if ( m_GPSMapPanel )
			m_GPSMapPanel.Show( m_ExpansionHudState && m_ExpansionHudGPSState && m_ExpansionHudGPSMapState && m_ExpansionGPSSetting );
		
		if ( m_MapPositionArrow )
			m_MapPositionArrow.Show( m_ExpansionHudState && m_ExpansionHudGPSState && m_ExpansionHudGPSMapState && m_ExpansionGPSSetting );
		
		if ( m_MapStatsPanel )
			m_MapStatsPanel.Show( m_ExpansionHudState && m_ExpansionHudGPSState && m_ExpansionHudGPSMapStatsState && m_ExpansionGPSSetting );
		
		if ( m_NVPanel )
			m_NVPanel.Show( m_ExpansionHudNVState && m_ExpansionNVSetting );
		
		if ( m_ClockPanel )
		{
			m_ClockPanel.Show( m_ExpansionHudState && m_ExpansionClockState && m_ClientClockShow );
			if ( m_ClockPanel.IsVisible() )
			{
				UpdateTime();
			}
		}
		
		if ( m_EarPlugIcon )
			m_EarPlugIcon.Show( m_ExpansionHudState && m_ExpansionEarplugState );	

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::RefreshExpansionHudVisibility End");
		#endif
	}
	
	//============================================
	// Expansion IsInitialized
	//============================================
	bool IsInitialized()
	{
		if ( m_WgtRoot == NULL || !m_WgtRoot )
		{
			return false;
		}
		
		return true;
	}
	
	//============================================
	// Expansion OnClick
	//============================================
	bool OnClick( Widget w, int x, int y, int button )
	{
		return false;
	}
	
	//============================================
	// Expansion OnMouseWheel
	//============================================
	bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		return false;
	}
	
	//============================================
	// Expansion UpdateGPS
	//============================================
	void UpdateGPS()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateGPS Start");
		#endif
		if ( m_MapStatsPanel.IsVisible() )
		{
			UpdateGPSMapStats();
		}
		
		if ( m_GPSMapPanel.IsVisible() )
		{
			UpdateGPSMap();
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateGPS End");
		#endif
	}
	
	//============================================
	// Expansion UpdateGPSMapStats
	//============================================
	void UpdateGPSMapStats()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateGPSMapStats Start");
		#endif
		vector m_cameraPosition, m_cameraDirection;
		float yaw, normalizedAngle;
		int dir, alt;
		PlayerBase player;
		
		if ( Class.CastTo(player, GetGame().GetPlayer()) )
		{
			m_cameraPosition = GetGame().GetCurrentCameraPosition();
			m_cameraDirection = GetGame().GetCurrentCameraDirection().VectorToAngles();
			yaw = m_cameraDirection[0];
			normalizedAngle = Math.NormalizeAngle(yaw);
			dir = Math.Round(normalizedAngle);
			alt = Math.Round(m_cameraPosition[1]);
			
			m_PlayerPosVal.SetText( Math.Round(m_cameraPosition[0]).ToString() + " : " + Math.Round(m_cameraPosition[2]).ToString() );
			m_PlayerDirVal.SetText( dir.ToString() );
			m_PlayerALTVal.SetText( alt.ToString() );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateGPSMapStats End");
		#endif
	}
	
	//============================================
	// Expansion UpdateGPSMap
	//============================================
	void UpdateGPSMap()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHud::UpdateGPSMap Start");
		#endif

		float scale;
		vector map_pos, camera_pos;
		float camera_x, camera_y;
		int deviation_x, deviation_y;
		int multiplier;
		float shift_x, shift_y;
		string worldName;
		
		if ( GetGame().GetPlayer() && GetGame().GetCurrentCameraPosition() )
		{
			GetGame().GetWorldName(worldName);
			worldName.ToLower();
		
			camera_pos = GetGame().GetCurrentCameraPosition();
			camera_x = camera_pos[0];
			camera_y = camera_pos[2];
			scale = GetCurrentGPSMapScale();
			
			multiplier = Math.Round(scale * 10);
			
			if ( worldName.Contains("chernarus") )	// CHERNARUS
			{
				if( scale >= 0.1 )
				{	
					shift_x = 642.5;
					shift_y = 485.5;
				}			
			} 
			else if ( worldName.Contains("enoch") )	// LIVONIA
			{
				if( scale >= 0.1 )
				{	
					shift_x = 545.0;
					shift_y = 412.5;
				}		
			}
			
			camera_x = camera_x + (shift_x * multiplier);
			camera_y = camera_y - (shift_y * multiplier);
			map_pos = Vector(camera_x, 0, camera_y);
			
			m_MapWidget.SetScale( scale );
			m_MapWidget.SetMapPos(map_pos);
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHud::UpdateGPSMap End");
		#endif
	}
	
	//============================================
	// Expansion UpdateTime
	//! Updates clock world time
	//============================================
	void UpdateTime()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateTime Start");
		#endif
		int year, month, day, hour, minute;
		string hours, minutes;
		GetGame().GetWorld().GetDate( year, month, day, hour, minute );
		
		if (hour < 10)
			hours = "0" + hour.ToString();
		else
			hours = hour.ToString();
		
		if (minute < 10)
			minutes = "0" + minute.ToString();
		else
			minutes = minute.ToString();
		
		m_Time.SetText( hours + ":" + minutes );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::UpdateTime End");
		#endif
	}
	
	//============================================
	// Expansion ShowHud
	//============================================
	void ShowHud( bool show )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::ShowHud Start");
		#endif
		m_ExpansionHudState = show;
		RefreshExpansionHudVisibility();
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHud::ShowHud End");
		#endif
	}
	
	//============================================
	// Expansion Bool GetExpansionHudState
	//============================================
	bool GetHudState()
	{		
		return m_ExpansionHudState;
	}
	
	//============================================
	// Expansion Bool ShowDebugger
	//============================================
	void ShowDebugger( bool show )
	{
		m_ExpansionHudDebuggerState = show;
		RefreshExpansionHudVisibility();
	}
	
	//============================================
	// Expansion GetDebuggerState
	//============================================
	bool GetDebuggerState()
	{
		return m_ExpansionHudDebuggerState;
	}
	
	//============================================
	// Expansion ShowGPS
	//============================================
	void ShowGPS( bool show )
	{
		m_ExpansionHudGPSState = show;
		
		RefreshExpansionHudVisibility();
		
		LoadMarkers();
	}
	
	//============================================
	// Expansion GetGPSState
	//============================================
	bool GetGPSState()
	{
		return m_ExpansionHudGPSState;
	}
	
	//============================================
	// Expansion GetGPSMapState
	//============================================
	bool GetGPSMapState()
	{
		return m_ExpansionHudGPSMapState;
	}
	
	//============================================
	// Expansion ShowGPSMap
	//============================================
	void ShowGPSMap(bool show)
	{
		m_ExpansionHudGPSMapState = show;
		RefreshExpansionHudVisibility();
	}
	
	//============================================
	// Expansion GetGPSMapStatsState
	//============================================
	bool GetGPSMapStatsState()
	{
		return m_ExpansionHudGPSMapStatsState;
	}
	
	//============================================
	// Expansion ShowGPSMapStats
	//============================================
	void ShowGPSMapStats(bool show)
	{
		m_ExpansionHudGPSMapStatsState = show;
		RefreshExpansionHudVisibility();
	}
	
	//============================================
	// Expansion ShowNV
	//============================================
	void ShowNV( bool show )
	{
		m_ExpansionHudNVState = show;
		RefreshExpansionHudVisibility();
	}
	
	//============================================
	// Expansion GetNVState
	//============================================
	bool GetNVState()
	{
		return m_ExpansionHudNVState;
	}

	//============================================
	// Expansion ShowClock
	//============================================
	void ShowClock( bool show )
	{
		m_ExpansionClockState = show;
		RefreshExpansionHudVisibility();
	}
	
	//============================================
	// Expansion ClockToggle
	//============================================
	void ClockToggle(bool show)
	{
		m_ClientClockShow = show;
		RefreshExpansionHudVisibility();
	}
	
	//============================================
	// Expansion GetClockState
	//============================================
	bool GetClockState()
	{
		return m_ExpansionClockState;
	}
	
	//============================================
	// Expansion GetEarplugsState
	//============================================
	bool GetEarplugsState()
	{
		return m_ExpansionEarplugState;
	}
	
	//============================================
	// Expansion OnResizeScreen
	//============================================
	void OnResizeScreen()
	{
		float x, y;
		m_WgtRoot.GetScreenSize( x, y );
		m_WgtRoot.Update();
	}

	// ------------------------------------------------------------
	// Expansion ToggleEarplugs
	// ------------------------------------------------------------
	void ToggleEarplugs()
	{
		m_ExpansionEarplugState = !m_ExpansionEarplugState;
		
		if ( m_ExpansionEarplugState )
		{
			GetGame().GetSoundScene().SetSoundVolume( GetExpansionClientSettings().EarplugLevel, 1 );
		}
		else
		{
			GetGame().GetSoundScene().SetSoundVolume( g_Game.m_volume_sound, 1 );
		}

		RefreshExpansionHudVisibility();
	}

	// ------------------------------------------------------------
	// Expansion ToggleEarplugs
	// ------------------------------------------------------------
	void UpdateEarplugs()
	{
		GetGame().GetSoundScene().SetSoundVolume( GetExpansionClientSettings().EarplugLevel, 1 );
		
		RefreshExpansionHudVisibility();
	}
	
	// ------------------------------------------------------------
	// Expansion GetCurrentGPSMapScale
	// ------------------------------------------------------------
	float GetCurrentGPSMapScale()
	{
		return m_GPSMapScale;
	}
	
	// ------------------------------------------------------------
	// Expansion SetGPSMapScale
	// ------------------------------------------------------------
	void SetGPSMapScale(float scale)
	{	
		m_GPSMapScale = scale;
		UpdateGPSMap();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHud::SetGPSMapScale:: m_GPSMapScale set to: " + m_GPSMapScale.ToString());
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetGPSArrow
	// ------------------------------------------------------------
	ExpansionGPSPositionArrow GetGPSArrow()
	{
		return m_MapPositionArrow;
	}
}
