/**
 * ExpansionCOTGroupsListEntry.c
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
class ExpansionCOTGroupsListEntry: ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_GroupName;
	private TextWidget m_GroupID;
	private ButtonWidget m_EditButton;
	
	private ref ExpansionCOTGroupsMenu m_COTGroupsMenu;
	private ref ExpansionPartyData m_PartyData;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTGroupsListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTGroupsListEntry(Widget parent, ExpansionCOTGroupsMenu menu, ExpansionPartyData partyData)
	{
		m_Root	= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Groups/GUI/layouts/COT/groups/Groups_List_Entry.layout", parent);
		m_GroupName = TextWidget.Cast( m_Root.FindAnyWidget("group_name"));
		m_GroupID = TextWidget.Cast( m_Root.FindAnyWidget("group_id"));
		m_EditButton	= ButtonWidget.Cast( m_Root.FindAnyWidget("edit_button"));
		
		m_COTGroupsMenu	= menu;
		m_PartyData = partyData;
		
		m_Root.SetHandler( this );
		
		SetEntry();
	}
	
	void ~ExpansionCOTGroupsListEntry()
	{
		if (g_Game && m_Root)
			m_Root.Unlink();
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsListEntry SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		m_GroupName.SetText(m_PartyData.GetPartyName() + " - ");
		m_GroupID.SetText("ID: " + m_PartyData.GetPartyID().ToString());
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsListEntry OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_COTGroupsMenu.SetGroupInfo(m_PartyData);
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
			m_GroupName.SetColor(ARGB(255, 0, 0, 0));
			m_GroupID.SetColor(ARGB(255, 0, 0, 0));
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsListEntry OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_GroupName.SetColor(ARGB(255, 255, 255, 255));
			m_GroupID.SetColor(ARGB(255, 255, 255, 255));
		}

		return false;
	}
};
#endif
