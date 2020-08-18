/**
 * ExpansionPartyTabInvitesListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyTabInvitesListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_GroupName;
	protected ButtonWidget m_AcceptButton;
	protected ImageWidget m_AcceptButtonIcon;
	protected ButtonWidget m_DeclineButton;
	protected ImageWidget m_DeclineButtonIcon;
	
	protected Man m_Player;
	protected ref ExpansionPartyModule m_PartyModule;
	protected ref ExpansionPartyInviteData m_Invite;
	protected ref ExpansionBookTabParty m_GroupMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyTabInvitesListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionPartyTabInvitesListEntry(Widget parent, ExpansionPartyInviteData invite)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyTabInvitesListEntry::ExpansionPartyTabInvitesListEntry - Start");
		#endif
	
		m_Root					= GridSpacerWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/group/expansion_group_menu_group_list_entry.layout", parent ) );
		m_GroupName				= TextWidget.Cast(m_Root.FindAnyWidget("group_name"));
		m_AcceptButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("group_accept_button"));
		m_AcceptButtonIcon		= ImageWidget.Cast(m_Root.FindAnyWidget("group_accept_button_icon"));
		m_DeclineButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("group_decline_button"));
		m_DeclineButtonIcon		= ImageWidget.Cast(m_Root.FindAnyWidget("group_decline_button_icon"));

		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast( GetModuleManager().GetModule( ExpansionPartyModule ) );
		
		m_Player = GetGame().GetPlayer();
		
		m_Invite = invite;
		
		m_Root.SetHandler(this);
		
		SetEntry();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyTabInvitesListEntry::ExpansionPartyTabInvitesListEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyTabInvitesListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionPartyTabInvitesListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetEntry
	// ------------------+------------------------------------------
	void SetEntry()
	{
		m_GroupName.SetText( m_Invite.partyName );
	}
	
	// ------------------------------------------------------------
	// Expansion OnAcceptInviteButtonClick
	// ------------------------------------------------------------
	void OnAcceptInviteButtonClick()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyTabInvitesListEntry::OnAcceptInviteButtonClick - Start");
		#endif
		
		if ( m_PartyModule && m_Invite )
		{
			m_PartyModule.AcceptInvite( m_Invite.partyID );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyTabInvitesListEntry::OnAcceptInviteButtonClick - End");
		#endif
	}
	
		
	// ------------------------------------------------------------
	// Expansion OnDeclineButtonClick
	// ------------------------------------------------------------
	void OnDeclineButtonClick()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyTabInvitesListEntry::OnDeclineButtonClick - Start");
		#endif
		
		if ( m_PartyModule && m_Invite )
		{
			m_PartyModule.DeclineInvite( m_Invite.partyID );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyTabInvitesListEntry::OnDeclineButtonClick - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetInvite
	// ------------------------------------------------------------
	ref ExpansionPartyInviteData GetInvite()
	{
		return m_Invite;
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_AcceptButton && w == m_AcceptButton)
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGroupMenuInvitesListEntry::OnClick - Clicked m_AcceptButton");
			#endif

			OnAcceptInviteButtonClick();
			
			return true;
		}
		
		if (m_DeclineButton && w == m_DeclineButton)
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGroupMenuInvitesListEntry::OnClick - Clicked m_DeclineButton");
			#endif

			OnDeclineButtonClick();
			
			return true;
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( m_AcceptButton && w == m_AcceptButton )
		{
			m_AcceptButtonIcon.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}
		
		if ( m_DeclineButton && w == m_DeclineButton )
		{
			m_DeclineButtonIcon.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_AcceptButton && w == m_AcceptButton )
		{
			m_AcceptButtonIcon.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}
		
		if ( m_DeclineButton && w == m_DeclineButton )
		{
			m_DeclineButtonIcon.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}

		return false;
	}
}