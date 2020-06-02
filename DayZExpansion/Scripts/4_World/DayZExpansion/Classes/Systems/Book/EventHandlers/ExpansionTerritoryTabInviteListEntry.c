/**
 * ExpansionTerritoryTabInviteListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTerritoryTabInviteListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_TerritoryName;
	protected ButtonWidget m_InviteButton;
	protected ImageWidget m_InviteButtonIcon;
	
	protected ExpansionTerritoryModule m_TerritoryModule;
	protected ref ExpansionTerritoryInvite m_Invite;
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritoryTabInviteListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionTerritoryTabInviteListEntry(Widget parent, ExpansionTerritoryInvite invite)
	{
		m_Root				= GridSpacerWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/group/expansion_group_menu_player_list_entry.layout", parent ) );
		m_TerritoryName		= TextWidget.Cast(m_Root.FindAnyWidget("player_name"));
		m_InviteButton		= ButtonWidget.Cast(m_Root.FindAnyWidget("player_invite_button"));
		m_InviteButtonIcon	= ImageWidget.Cast(m_Root.FindAnyWidget("player_invite_button_icon"));
		
		m_Invite			= invite;
		
		m_TerritoryModule	= ExpansionTerritoryModule.Cast( GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		
		m_Root.SetHandler(this);
		
		SetInviteEntry();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritoryTabInviteListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionTerritoryTabInviteListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPlayerEntry
	// ------------------------------------------------------------
	void SetInviteEntry()
	{
		if ( m_Invite )
		{
			m_TerritoryName.SetText( m_Invite.TerritoryName );
		}
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
			
			if ( m_Invite )
			{
				m_TerritoryModule.AcceptInvite( m_Invite.TerritoryID );
				return true;
			}
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
			m_InviteButtonIcon.SetColor( ARGB(255,255,255,255) );
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