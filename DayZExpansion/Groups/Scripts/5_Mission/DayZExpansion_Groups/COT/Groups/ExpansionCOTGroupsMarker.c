/**
 * ExpansionCOTGroupsMapMarker.c
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
class ExpansionCOTGroupsMapMarker extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_Name;
	private ImageWidget m_Icon;
	private ButtonWidget m_MarkerButton;
	private MapWidget m_MapWidget;
	private ref Timer m_MarkerUpdateTimer;
	
	private ref ExpansionMarkerData m_Marker;
	private ExpansionCOTGroupsMenu m_COTGroupsMenu;

	private vector m_MarkerPos;
		
	// ------------------------------------------------------------
	// Expansion ExpansionCOTGroupsMapMarker Constructor
	// ------------------------------------------------------------
	void ExpansionCOTGroupsMapMarker(Widget parent, MapWidget mapwidget, ExpansionMarkerData marker, ExpansionCOTGroupsMenu menu)
	{
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Groups/GUI/layouts/COT/groups/Groups_Marker.layout", parent);
		m_Name = TextWidget.Cast(m_Root.FindAnyWidget("marker_name"));
		m_Icon	= ImageWidget.Cast(m_Root.FindAnyWidget("marker_icon"));
		m_MarkerButton	= ButtonWidget.Cast(m_Root.FindAnyWidget("marker_button"));

		m_MapWidget = mapwidget;
				
		m_Marker = marker;
		m_COTGroupsMenu = menu;
		
		SetMarker();
		
		m_Root.SetHandler(this);
		RunUpdateTimer();
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMapMarker Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionCOTGroupsMapMarker()
	{
		StopUpdateTimer();
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMapMarker SetMarker
	// ------------------------------------------------------------	
	void SetMarker()
	{
		if (!m_Marker)
			return;
		
		m_MarkerPos = m_Marker.GetPosition();
		
		m_Name.SetText(m_Marker.GetName());
		m_Name.SetColor(m_Marker.GetColor());
		m_Icon.LoadImageFile(0, m_Marker.GetIcon());
		m_Icon.SetColor(m_Marker.GetColor());
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMapMarker Update
	// ------------------------------------------------------------
	void Update(float timeslice)
	{
		if (!m_MapWidget)
			return;

		vector mapPos = m_MapWidget.MapToScreen(m_MarkerPos);

		float x, y;
		m_Root.GetParent().GetScreenPos(x, y);
		m_Root.SetPos(mapPos[0] - x, mapPos[1] - y, true);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMapMarker OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
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
	// ExpansionCOTGroupsMapMarker OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_MarkerButton && w == m_MarkerButton)
		{
			RunUpdateTimer();
			
			m_Icon.SetColor(m_Marker.GetColor());
			m_Name.SetColor(m_Marker.GetColor());
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMapMarker OnClick
	// ------------------------------------------------------------
	/*override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_MarkerButton && w == m_MarkerButton)
		{
			//! Do nothing for now.
			return true;
		}

		return false;
	}*/
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMapMarker SetPosition
	// ------------------------------------------------------------
	void SetPosition(float x, float y)
	{
		m_Root.SetPos(x, y, true);
		m_MarkerPos = m_MapWidget.ScreenToMap(Vector(x, y, 0));
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMapMarker GetMarkerPos
	// ------------------------------------------------------------	
	vector GetMarkerPos()
	{
		return m_MarkerPos;
	}
	
	// ------------------------------------------------------------
	// Expansion HideMarker
	// ------------------------------------------------------------
	void HideMarker()
	{
		m_Root.Show(false);
	}
	
	// ------------------------------------------------------------
	// Expansion ShowMarker
	// ------------------------------------------------------------
	void ShowMarker()
	{
		m_Root.Show(true);
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
	// ExpansionCOTGroupsMapMarker RunUpdateTimer
	// ------------------------------------------------------------
	void RunUpdateTimer()
	{
		if (!m_MarkerUpdateTimer)
		{
			m_MarkerUpdateTimer = new Timer(CALL_CATEGORY_GUI);
			if (!m_MarkerUpdateTimer.IsRunning())
			{
				m_MarkerUpdateTimer.Run(0.01, this, "Update", NULL, true); // Call Update all 0.01 seconds
			}
		}
	}
};
#endif
