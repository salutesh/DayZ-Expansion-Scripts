/**
 * ExpansionCOTVehiclesMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
class ExpansionCOTVehiclesMapMarker extends ScriptedWidgetEventHandler
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
	
	protected float m_OffsetX;
	protected float m_OffsetY;

	protected ref ExpansionVehicleMetaData m_Vehicle;
	protected ref ExpansionCOTVehiclesMenu m_COTVehicleMenu;
	
	protected ref Timer m_MarkerUpdateTimer;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTVehiclesMapMarker Constructor
	// ------------------------------------------------------------
	void ExpansionCOTVehiclesMapMarker(Widget parent, MapWidget mapwidget, vector pos, int color, string icon, ExpansionVehicleMetaData vehicle, ExpansionCOTVehiclesMenu menu)
	{
		m_Root 				= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Vehicles/GUI/layouts/COT/vehicles/Vehicles_Marker.layout", parent);

		m_Name				= TextWidget.Cast( m_Root.FindAnyWidget("marker_name") );
		m_Icon				= ImageWidget.Cast( m_Root.FindAnyWidget("marker_icon") );
		m_MarkerButton		= ButtonWidget.Cast( m_Root.FindAnyWidget("marker_button") );
		m_MarkerDragging 	= ButtonWidget.Cast( m_Root.FindAnyWidget("marker_icon_panel") );

		m_MapWidget			= mapwidget;
		m_MarkerPos			= pos;
		m_MarkerIcon		= icon;
		m_MarkerColor		= color;
		m_Vehicle 			= vehicle;
		m_COTVehicleMenu 	= menu;
		
		GetGame().ConfigGetText( "cfgVehicles " + m_Vehicle.m_ClassName + " displayName", m_MarkerName );
		
		if (m_MarkerIcon)
			m_Icon.LoadImageFile(0, m_MarkerIcon);
		
		m_Icon.SetColor(m_MarkerColor);
		m_Name.SetText(m_MarkerName);
		m_Name.SetColor(m_MarkerColor);
		
		m_Root.SetHandler(this);
		
		RunUpdateTimer();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTVehiclesMapMarker Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionCOTVehiclesMapMarker()
	{
		StopUpdateTimer();
		
		delete m_Root;
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
	void Update(float timeslice)
	{
		if ( !m_MapWidget )
			return;

		if ( !m_MarkerPos )
			return;

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
		if (m_MarkerButton && w == m_MarkerButton)
		{
			StopUpdateTimer();
			
			m_Icon.SetColor(ARGB(255,255,255,255));
			m_Name.SetColor(ARGB(255,255,255,255));
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if (m_MarkerButton && w == m_MarkerButton)
		{
			RunUpdateTimer();
			
			m_Icon.SetColor(m_MarkerColor);
			m_Name.SetColor(m_MarkerColor);
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
			m_COTVehicleMenu.SetVehicleInfo( m_Vehicle );
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPosition
	// ------------------------------------------------------------
	void SetPosition( float x, float y )
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
		if (m_MarkerUpdateTimer && m_MarkerUpdateTimer.IsRunning())
		{
			m_MarkerUpdateTimer.Stop();
			m_MarkerUpdateTimer = NULL;
			
			delete m_MarkerUpdateTimer;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RunUpdateTimer
	// ------------------------------------------------------------
	void RunUpdateTimer()
	{
		if (!m_MarkerUpdateTimer)
		{
			m_MarkerUpdateTimer = new Timer( CALL_CATEGORY_GUI );
			if (!m_MarkerUpdateTimer.IsRunning())
			{
				m_MarkerUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
			}
		}
	}
};
#endif
