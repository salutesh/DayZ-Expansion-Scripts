/**
 * ExpansionCOTGroupsIconListEntry.c
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
class ExpansionCOTGroupsIconListEntry extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private ImageWidget m_Icon;
	private TextWidget m_IconName;
	private ButtonWidget m_EntryButton;
	
	private ExpansionIcon m_IconData;
	private ExpansionCOTGroupsMapMarker m_Marker;
	private int m_Color;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTGroupsIconListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTGroupsIconListEntry(Widget parent, ExpansionIcon icon, ExpansionCOTGroupsMapMarker marker)
	{
		m_Root	= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Groups/GUI/layouts/COT/groups/Icon_List_Entry.layout", parent);
		m_Icon = ImageWidget.Cast(m_Root.FindAnyWidget("entry_icon"));
		m_IconName = TextWidget.Cast(m_Root.FindAnyWidget("icon_name"));
		m_EntryButton = ButtonWidget.Cast(m_Root.FindAnyWidget("entry_button"));
		
		m_IconData = icon;
		m_Marker = marker;
				
		m_Root.SetHandler(this);
		
		SetEntry();
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTGroupsIconListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		m_Icon.LoadImageFile(0, m_IconData.Path);
		m_IconName.SetText(m_IconData.Name);
		
		m_Icon.SetColor(m_Marker.GetMarkerData().GetColor());
		m_IconName.SetColor(m_Marker.GetMarkerData().GetColor());
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry SetColor
	// ------------------------------------------------------------
	void SetColor(int color)
	{
		m_Color = color;
		
		m_IconName.SetColor(m_Color);
		m_Icon.SetColor(m_Color);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry UpdateSelection
	// ------------------------------------------------------------	
	void UpdateSelection(ExpansionIcon icon)
	{
		UpdateSelection(icon.Name);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry UpdateSelection
	// ------------------------------------------------------------	
	void UpdateSelection(string icon)
	{
		if (icon == m_IconData.Name)
			m_Root.SetColor(ARGB(100, 200, 0, 0));
		else
			m_Root.SetColor(ARGB(100, 0, 0, 0));
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_EntryButton && w == m_EntryButton)
		{
			m_Marker.SetIcon(m_IconData);
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_EntryButton && w == m_EntryButton)
		{
			m_IconName.SetColor(ARGB(255, 0, 0, 0));
			m_Icon.SetColor(ARGB(255, 0, 0, 0));
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsIconListEntry OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_EntryButton && w == m_EntryButton)
		{
			m_IconName.SetColor(m_Color);
			m_Icon.SetColor(m_Color);
		}

		return false;
	}
};
#endif
#endif
