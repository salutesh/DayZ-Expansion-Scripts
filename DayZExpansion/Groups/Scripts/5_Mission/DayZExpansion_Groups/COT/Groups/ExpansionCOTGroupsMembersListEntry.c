/**
 * ExpansionCOTGroupsMembersListEntry.c
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
class ExpansionCOTGroupsMembersListEntry extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_MemberName;
	private ButtonWidget m_EditButton;
	private ImageWidget m_OnlineIcon;
	
	private ExpansionCOTGroupsMenu m_COTGroupsMenu;
	private ExpansionPartyPlayerData m_PlayerData;
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMembersListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTGroupsMembersListEntry(Widget parent, ExpansionCOTGroupsMenu menu, ExpansionPartyPlayerData playerData)
	{
		m_Root	= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Groups/GUI/layouts/COT/groups/Members_List_Entry.layout", parent);
		m_MemberName = TextWidget.Cast(m_Root.FindAnyWidget("member_name"));
		m_EditButton	= ButtonWidget.Cast(m_Root.FindAnyWidget("edit_button"));
		m_OnlineIcon = ImageWidget.Cast(m_Root.FindAnyWidget("member_status_icon"));	
		
		m_COTGroupsMenu	= menu;
		m_PlayerData = playerData;
		
		m_Root.SetHandler( this );
		
		SetEntry();
	}
	
	void ~ExpansionCOTGroupsMembersListEntry()
	{
		if (g_Game && m_Root)
			m_Root.Unlink();
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMembersListEntry SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		m_MemberName.SetText(m_PlayerData.GetName());
		
		int color;
		if (PlayerBase.Expansion_IsOnline(m_PlayerData.GetID()))
			color =  ARGB(255, 39, 174, 96);
		else
			color = ARGB(255, 192, 57, 43);
		
		m_OnlineIcon.SetColor(color);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMembersListEntry OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_COTGroupsMenu.SetMemberInfo(m_PlayerData);
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMembersListEntry OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_MemberName.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMembersListEntry OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_EditButton && w == m_EditButton )
		{
			m_MemberName.SetColor(ARGB(255, 255, 255, 255));
			return true;
		}

		return false;
	}
};
#endif
