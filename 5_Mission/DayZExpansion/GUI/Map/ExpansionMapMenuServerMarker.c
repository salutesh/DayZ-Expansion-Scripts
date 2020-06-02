/**
 * ExpansionMapMenuServerMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenuServerMarker extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_Name;
	protected ImageWidget m_Icon;
	protected ButtonWidget m_MarkerButton;
	protected MapWidget m_MapWidget;
	protected vector m_MarkerPos;
	protected string m_MarkerName;
	protected string m_MarkerIcon;
	protected int m_MarkerColor;
	protected int m_MarkerAlpha;

	protected ref Timer	m_MarkerUpdateTimer;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuServerMarker Constructor
	// ------------------------------------------------------------
	void ExpansionMapMenuServerMarker(Widget parent, MapWidget mapwidget, vector position, string name, int color, string icon)
	{
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map_marker.layout", parent) );

		m_Name			= TextWidget.Cast( m_Root.FindAnyWidget("marker_name") );
		m_Icon			= ImageWidget.Cast( m_Root.FindAnyWidget("marker_icon") );

		m_MapWidget		= mapwidget;
		m_MarkerPos		= position;
		m_MarkerName	= name;
		m_MarkerIcon	= icon;
		m_MarkerColor	= color;

		m_Root.SetHandler(this);

		SetMarker();

		//! Update Timer
		m_MarkerUpdateTimer = new Timer();
		m_MarkerUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuPositionArrow Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionMapMenuServerMarker()
	{
		delete m_Root;

		m_MarkerUpdateTimer.Stop();
	}
	
	// ------------------------------------------------------------
	// Expansion SetMarker
	// ------------------------------------------------------------
	void SetMarker()
	{
		string icon = m_MarkerIcon;
		string name = m_MarkerName;
		int color = m_MarkerColor;
		vector position = m_MarkerPos;

		if (icon != "")
		{
			m_Icon.LoadImageFile(0, icon);
			m_Icon.SetColor(color);
		}

		if (name != "")
		{
			m_Name.SetText(name);
			m_Name.SetColor(color);
		}

		if (position)
		{
			vector mapPos = m_MapWidget.MapToScreen( position );
			m_Root.SetPos(mapPos[0], mapPos[1], true);
		}
	}
	
	//============================================
	// Expansion GetMarkerName
	//============================================
	string GetMarkerName()
	{
		return m_MarkerName;
	}

	// ------------------------------------------------------------
	// Expansion SetMarkerAlpha
	// ------------------------------------------------------------
	void SetMarkerAlpha(int alpha)
	{
		m_MarkerAlpha = alpha;
	}

	//============================================
	// Expansion Update
	//============================================
	void Update( float timeslice )
	{
		int color = m_MarkerColor;
		int alpha;
		
		if (!m_MarkerAlpha)
		{
			alpha = 255;
		}
		else
		{
			alpha = m_MarkerAlpha;
		}

		if (alpha < 5)
		{
			alpha = 5;
		}

		int currentalpha;
		int red;
		int green;
		int blue;

		GetARGB(color, currentalpha, red, green, blue);
		
		m_MarkerColor = ARGB(alpha, red, green, blue);
		
		m_Icon.SetColor(m_MarkerColor);
		m_Name.SetColor(m_MarkerColor);

		vector position = m_MarkerPos;
		if (position)
		{
			vector mapPos = m_MapWidget.MapToScreen( position );
			m_Root.SetPos(mapPos[0], mapPos[1], true);
		}
	}
}