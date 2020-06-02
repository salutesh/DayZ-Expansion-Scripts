/**
 * ExpansionPartyTabPlayerListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyTabPlayerListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_TextPlayerName;
	protected ButtonWidget m_InviteButton;
	protected ImageWidget m_InviteButtonIcon;
	
	protected string m_PlayerID;
	protected string m_PlayerName;
	protected ref ExpansionPartyModule m_PartyModule;
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyTabPlayerListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionPartyTabPlayerListEntry(Widget parent, string name, string id)
	{
		m_Root				= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/group/expansion_group_menu_player_list_entry.layout", parent ) );
		m_TextPlayerName		= TextWidget.Cast(m_Root.FindAnyWidget("player_name"));
		m_InviteButton		= ButtonWidget.Cast(m_Root.FindAnyWidget("player_invite_button"));
		m_InviteButtonIcon	= ImageWidget.Cast(m_Root.FindAnyWidget("player_invite_button_icon"));
		
		m_PlayerID			= id;
		m_PlayerName 		= name;

		m_PartyModule		= ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
		
		m_Root.SetHandler(this);
		
		SetPlayerEntry();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyTabPlayerListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionPartyTabPlayerListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPlayerEntry
	// ------------------------------------------------------------
	void SetPlayerEntry()
	{
		m_TextPlayerName.SetText( m_PlayerName );
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( m_InviteButton && w == m_InviteButton )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGroupMenuPlayerListEntry::OnClick - Clicked m_InviteButton");
			#endif
			
			m_PartyModule.InvitePlayer( m_PlayerID );
			
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( m_InviteButton && w == m_InviteButton )
		{
			m_InviteButtonIcon.SetColor(ARGB(255,255,255,255));
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_InviteButton && w == m_InviteButton )
		{
			m_InviteButtonIcon.SetColor(ARGB(255,0,0,0));
			return true;
		}

		return false;
	}
}