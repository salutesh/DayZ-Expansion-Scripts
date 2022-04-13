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
class ExpansionCOTGroupsPlayersListEntry extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_PlayerName;
	private ButtonWidget m_EditButton;
	
	private ref ExpansionCOTGroupsMenu m_COTGroupsMenu;
	private ref ExpansionPlayerDataCollection m_Player;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTGroupsPlayersListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTGroupsPlayersListEntry(Widget parent, ExpansionCOTGroupsMenu menu, ExpansionPlayerDataCollection player)
	{
		m_Root	= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Groups/GUI/layouts/COT/groups/Players_List_Entry.layout", parent);
		m_PlayerName = TextWidget.Cast(m_Root.FindAnyWidget("player_name"));
		m_EditButton	= ButtonWidget.Cast(m_Root.FindAnyWidget("edit_button"));
		
		m_COTGroupsMenu	= menu;
		m_Player = player;
		
		m_Root.SetHandler( this );
		
		SetEntry();
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsPlayersListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTGroupsPlayersListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsPlayersListEntry SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		if (!m_Player)
			return;
		
		m_PlayerName.SetText(m_Player.Name);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsPlayersListEntry OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_COTGroupsMenu.SetPlayerInfo(m_Player);
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsPlayersListEntry OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_PlayerName.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsPlayersListEntry OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_EditButton && w == m_EditButton)
		{
			m_PlayerName.SetColor(ARGB(255, 255, 255, 255));
			return true;
		}

		return false;
	}
};
#endif
