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
	protected Widget m_Root;
	protected TextWidget m_Name;
	protected ImageWidget m_Icon;
	protected ButtonWidget m_MarkerButton;
	protected Widget m_MarkerDragging;
	protected MapWidget m_MapWidget;
	protected vector m_MarkerPos;
	protected string m_MarkerName;
	protected string m_MarkerIcon;
	protected int m_MarkerColor;
	protected int m_MarkerAlpha;
	protected ref Timer m_MarkerUpdateTimer;
	
	protected float m_OffsetX;
	protected float m_OffsetY;
	
	protected bool m_IsTempMarker;
	protected ref ExpansionTerritory m_Territory;
	protected ref ExpansionCOTTerritoriesMenu m_COTTerritoryMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMapMarker Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesMapMarker(Widget parent, MapWidget mapwidget, vector position, string name, int color, string icon, ExpansionTerritory territory, ExpansionCOTTerritoriesMenu menu)
	{
		m_Root 			= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map_marker.layout", parent);

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
		
		m_Root.SetHandler(this);
		
		Init();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMapMarker Init
	// ------------------------------------------------------------
	void Init()
	{			
		//! Update Timer
		m_MarkerUpdateTimer = new Timer();
		m_MarkerUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMapMarker Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionCOTTerritoriesMapMarker()
	{
		m_Root.Unlink();
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteMarker
	// ------------------------------------------------------------
	void DeleteMarker()
	{
		m_MarkerUpdateTimer.Stop();
		
		m_Root.Unlink();
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeColor
	// ------------------------------------------------------------
	void ChangeColor(int color)
	{
		m_MarkerColor = color;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangePositon
	// ------------------------------------------------------------
	void ChangePositon(vector position)
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
		if (m_MarkerIcon)
			m_Icon.LoadImageFile(0, m_MarkerIcon);
		
		if (m_MarkerColor)
			m_Icon.SetColor(m_MarkerColor);
			m_Name.SetColor(m_MarkerColor);
		
		if (m_MarkerName)
			m_Name.SetText(m_MarkerName);
		
		if (m_MarkerPos)
		{
			vector mapPos = m_MapWidget.MapToScreen(m_MarkerPos);
			m_Root.SetPos(mapPos[0], mapPos[1], true);
		}
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if (m_MarkerButton && w == m_MarkerButton && !m_IsTempMarker)
		{
			if (m_MarkerUpdateTimer)
				m_MarkerUpdateTimer.Stop();

			m_Icon.SetColor(ARGB(255,255,255,255));
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		int color = m_MarkerColor;
		if (m_MarkerButton && w == m_MarkerButton)
		{
			if (m_MarkerUpdateTimer)
			{
				m_MarkerUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
			}

			m_Icon.SetColor(color);
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( m_MarkerButton && w == m_MarkerButton )
		{
			m_COTTerritoryMenu.SetTerritoryInfo( m_Territory );
			return true;
		}

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
}