/**
 * ExpansionCOTGroupsMarkerListEntry.c
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
#ifdef EXPANSIONMODNAVIGATION
class ExpansionCOTGroupsMarkerListEntry extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private ImageWidget m_MarkerIcon;
	private TextWidget m_MarkerName;
	private ButtonWidget m_EditButton;
	
	private ExpansionCOTGroupsMapMarker m_Marker;
	private ExpansionCOTGroupsMenu m_COTGroupsMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTGroupsMarkerListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTGroupsMarkerListEntry(Widget parent, ExpansionCOTGroupsMapMarker marker, ExpansionCOTGroupsMenu menu)
	{
		m_Root	= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Groups/GUI/layouts/COT/groups/Markers_List_Entry.layout", parent);
		m_MarkerIcon = ImageWidget.Cast(m_Root.FindAnyWidget("marker_icon"));
		m_MarkerName = TextWidget.Cast(m_Root.FindAnyWidget("marker_name"));
		m_EditButton = ButtonWidget.Cast(m_Root.FindAnyWidget("edit_button"));
		
		m_Marker = marker;
		m_COTGroupsMenu	= menu;
		
		m_Root.SetHandler( this );
		
		SetEntry();
	}
	
	void ~ExpansionCOTGroupsMarkerListEntry()
	{
		if (g_Game && m_Root)
			m_Root.Unlink();
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMarkerListEntry SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		if (!m_Marker)
			return;
		
		m_MarkerIcon.LoadImageFile(0, m_Marker.GetMarkerData().GetIcon());
		m_MarkerIcon.SetColor(m_Marker.GetMarkerData().GetColor());
		m_MarkerName.SetText(m_Marker.GetMarkerData().GetName());
		m_MarkerName.SetColor(m_Marker.GetMarkerData().GetColor());
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMarkerListEntry SetIcon
	// ------------------------------------------------------------	
	void SetIcon(string path)
	{
		m_MarkerIcon.LoadImageFile(0, path);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMarkerListEntry SetColor
	// ------------------------------------------------------------
	void SetColor(int color)
	{
		m_MarkerIcon.SetColor(color);
		m_MarkerName.SetColor(color);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMarkerListEntry OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_EditButton && w == m_EditButton && m_Marker)
		{
			m_Marker.FocusOnMarker();
			m_COTGroupsMenu.EditMarker(m_Marker);
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsListEntry OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_MarkerName.SetColor(ARGB(255, 0, 0, 0));
			m_MarkerIcon.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsListEntry OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_EditButton && w == m_EditButton && m_Marker)
		{
			m_MarkerName.SetColor(m_Marker.GetMarkerData().GetColor());
			m_MarkerIcon.SetColor(m_Marker.GetMarkerData().GetColor());
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsListEntry GetMarker
	// ------------------------------------------------------------		
	ExpansionCOTGroupsMapMarker GetMarker()
	{
		return m_Marker;
	}
};
#endif
#endif
