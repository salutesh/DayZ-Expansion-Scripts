/**
 * ExpansionCOTTerritoriesMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoriesMapMarker extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private Widget m_MarkerFrame;
	private TextWidget m_Name;
	private ImageWidget m_Icon;
	private ButtonWidget m_MarkerButton;
	private Widget m_MarkerDragging;
	private MapWidget m_MapWidget;
	private vector m_MarkerPos;
	private string m_MarkerName;
	private string m_MarkerIcon;
	private int m_MarkerColor;
	private int m_MarkerAlpha;
	private ref Timer m_MarkerUpdateTimer;
	
	private float m_OffsetX;
	private float m_OffsetY;
	
	private bool m_IsTempMarker;
	private ref ExpansionTerritory m_Territory;
	private ref ExpansionCOTTerritoriesMenu m_COTTerritoryMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMapMarker Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesMapMarker(Widget parent, MapWidget mapwidget, vector position, string name, int color, string icon, ref ExpansionTerritory territory, ExpansionCOTTerritoriesMenu menu)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::ExpansionCOTTerritoriesMapMarker - Start");
		#endif
		
		m_Root 			= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map_marker.layout", parent);

		m_MarkerFrame	= Widget.Cast( m_Root.FindAnyWidget("marker_frame") );
		m_Name			= TextWidget.Cast( m_Root.FindAnyWidget("marker_name") );
		m_Icon			= ImageWidget.Cast( m_Root.FindAnyWidget("marker_icon") );
		m_MarkerButton	= ButtonWidget.Cast( m_Root.FindAnyWidget("marker_button") );
		m_MarkerDragging = ButtonWidget.Cast( m_Root.FindAnyWidget("marker_icon_panel") );

		m_MapWidget		= mapwidget;
		m_MarkerPos		= position;
		m_MarkerName	= name;
		m_MarkerIcon	= icon;
		m_MarkerColor	= color;
		m_Territory 	= territory;
		m_COTTerritoryMenu = menu;
		
		m_MarkerFrame.SetAlpha(0);
		m_Icon.LoadImageFile(0, m_MarkerIcon);
		m_Icon.SetColor(m_MarkerColor);
		m_Name.SetColor(m_MarkerColor);
		m_Name.SetText(m_MarkerName);
		
		m_Root.SetHandler(this);
		
		RunUpdateTimer();
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::ExpansionCOTTerritoriesMapMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMapMarker Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionCOTTerritoriesMapMarker()
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::~ExpansionCOTTerritoriesMapMarker - Start");
		#endif
		
		StopUpdateTimer();
		
		delete m_Root;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::~ExpansionCOTTerritoriesMapMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeColor
	// ------------------------------------------------------------
	void ChangeColor(int color)
	{
		m_MarkerColor = color;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPosition
	// ------------------------------------------------------------
	void SetPosition(vector position)
	{
		m_MarkerPos = position;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeIcon
	// ------------------------------------------------------------
	void ChangeIcon(string icon)
	{
		m_MarkerIcon = icon;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeName
	// ------------------------------------------------------------
	void ChangeName(string name)
	{
		m_MarkerName = name;
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarkerName
	// ------------------------------------------------------------
	string GetMarkerName()
	{
		return m_MarkerName;
	}

	// ------------------------------------------------------------
	// Expansion Update
	// ------------------------------------------------------------
	void Update( float timeslice )
	{				
		vector mapPos = m_MapWidget.MapToScreen( m_MarkerPos );

		float x;
		float y;
		m_Root.GetParent().GetScreenPos( x, y );

		m_Root.SetPos( mapPos[0] - x, mapPos[1] - y, true );
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnMouseEnter - Start");
		#endif
		
		if (m_MarkerButton && w == m_MarkerButton && !m_IsTempMarker)
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnMouseEnter - m_MarkerButton");
			#endif
			
			StopUpdateTimer();

			m_Icon.SetColor(ARGB(255,255,255,255));
			m_Name.SetColor(ARGB(255,255,255,255));
			
			return true;
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnMouseEnter - End");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnMouseLeave - Start");
		#endif
		
		int color = m_MarkerColor;
		if (m_MarkerButton && w == m_MarkerButton)
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnMouseLeave - m_MarkerButton");
			#endif
			
			RunUpdateTimer();

			m_Icon.SetColor(color);
			m_Name.SetColor(color);
			
			return true;
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnMouseLeave - End");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnClick - Start");
		#endif
		
		if ( m_MarkerButton && w == m_MarkerButton )
		{
			m_COTTerritoryMenu.SetTerritoryInfo( m_Territory );
			
			return true;
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::OnClick - End");
		#endif

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPosition
	// ------------------------------------------------------------
	void SetPosition( int x, int y )
	{
		m_Root.SetPos( x, y, true );
		m_MarkerPos = m_MapWidget.ScreenToMap( Vector( x, y, 0 ) );
	}
	
	// ------------------------------------------------------------
	// Expansion HideMarker
	// ------------------------------------------------------------
	void HideMarker()
	{
		m_Root.Show( false );
	}
	
	// ------------------------------------------------------------
	// Expansion ShowMarker
	// ------------------------------------------------------------
	void ShowMarker()
	{
		m_Root.Show( true );
	}
	
	// ------------------------------------------------------------
	// Expansion StopUpdateTimer
	// ------------------------------------------------------------
	void StopUpdateTimer()
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::StopUpdateTimer - Start");
		#endif
		
		if (m_MarkerUpdateTimer && m_MarkerUpdateTimer.IsRunning())
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesMapMarker::StopUpdateTimer - Stop Update Timer");
			#endif
			m_MarkerUpdateTimer.Stop();
			m_MarkerUpdateTimer = NULL;
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::StopUpdateTimer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RunUpdateTimer
	// ------------------------------------------------------------
	void RunUpdateTimer()
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::RunUpdateTimer - Start");
		#endif
		
		if (!m_MarkerUpdateTimer)
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesMapMarker::StopUpdateTimer - Run Update Timer");
			#endif
			m_MarkerUpdateTimer = new Timer( CALL_CATEGORY_GUI );
			if (!m_MarkerUpdateTimer.IsRunning())
			{
				m_MarkerUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
			}
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::RunUpdateTimer - End");
		#endif
	}
}