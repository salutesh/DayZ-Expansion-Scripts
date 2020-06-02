/**
 * ExpansionPartyTabMemberListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyTabMemberListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_MemberName;
	protected ButtonWidget m_KickButton;
	protected ImageWidget m_KickButtonIcon;
	protected ButtonWidget m_PromoteButton;
	protected ImageWidget m_PromoteButtonIcon;
	protected ButtonWidget m_DemoteButton;
	protected ImageWidget m_DemoteButtonIcon;
	protected ImageWidget m_MemberStateIcon;
	
	protected ref ExpansionPartySaveFormatPlayer m_Player;
	protected ref ExpansionPartyModule m_PartyModule;
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyTabMemberListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionPartyTabMemberListEntry(Widget parent, ref ExpansionPartySaveFormatPlayer player)
	{
		m_Root					= GridSpacerWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/group/expansion_group_menu_member_list_entry.layout", parent ) );
		m_MemberName			= TextWidget.Cast(m_Root.FindAnyWidget("member_name"));
		m_KickButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("member_kick_button"));
		m_KickButtonIcon		= ImageWidget.Cast(m_Root.FindAnyWidget("member_kick_button_icon"));
		m_PromoteButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("member_promote_button"));
		m_PromoteButtonIcon		= ImageWidget.Cast(m_Root.FindAnyWidget("member_promote_button_icon"));
		m_DemoteButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("member_demote_button"));
		m_DemoteButtonIcon		= ImageWidget.Cast(m_Root.FindAnyWidget("member_demote_button_icon"));
		m_MemberStateIcon		= ImageWidget.Cast(m_Root.FindAnyWidget("member_status_icon"));
		
		m_Player				= player;

		m_PartyModule			= ExpansionPartyModule.Cast( GetModuleManager().GetModule( ExpansionPartyModule ) );
		
		m_Root.SetHandler(this);
		
		SetMemberEntry();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyTabMemberListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionPartyTabMemberListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetMemberEntry
	// ------------------------------------------------------------
	void SetMemberEntry()
	{
		if (m_Player)
		{
			string name = m_Player.Name;
			
			//! Set player name
			if (name != "")
				m_MemberName.SetText(name);
			else
				m_MemberName.SetText("");
			
			//! Set online state icon color
			if ( IsOnline( m_Player.UID ) )
			{
				m_MemberStateIcon.SetColor( ARGB( 255, 39, 174, 96 ) );
			}
			else
			{
				m_MemberStateIcon.SetColor( ARGB( 255, 192, 57, 43 ) );
			}
			
			//! Show buttons depending of promotion
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			string ownerUID = m_PartyModule.GetParty().GetOwnerUID();
			bool isOwner = ( player.GetIdentityUID() == ownerUID );
			
			//! We check if we are viewing ourself or viewing the owner of the group
			if (m_Player.UID == player.GetIdentityUID() || ownerUID == m_Player.UID)
			{
				m_PromoteButton.Show( false );
				m_DemoteButton.Show( false );
				m_KickButton.Show( false );
				return;
			}
			
			if ( m_Player.Promoted )
			{
				m_PromoteButton.Show( false );
				m_DemoteButton.Show( isOwner );
			}
			else
			{
				m_PromoteButton.Show( isOwner );
				m_DemoteButton.Show( false );
			}
			
			ref ExpansionPartySaveFormatPlayer myPlayer = m_PartyModule.GetParty().GetPlayer( player.GetIdentityUID() );
			if (!myPlayer) return;
			
			m_KickButton.Show( myPlayer.Promoted || isOwner );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabTerritory IsOnline
	// ------------------------------------------------------------
	bool IsOnline( string uid )
	{
		for ( int i = 0; i < ClientData.m_PlayerList.m_PlayerList.Count(); i++ )
		{
			ref SyncPlayer player = ClientData.m_PlayerList.m_PlayerList.Get( i );

			if ( player )
			{
				if ( player.m_RUID == uid )
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion OnKickButtonClick
	// ------------------------------------------------------------
	void OnKickButtonClick()
	{
		if (m_Player && m_PartyModule)
		{
			m_PartyModule.RemovePlayer( m_Player.UID );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnPromoteButtonClick
	// ------------------------------------------------------------
	void OnPromoteButtonClick()
	{
		if (m_Player && m_PartyModule)
		{
			m_PartyModule.PromotePlayer( m_Player.UID );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnPromoteButtonClick
	// ------------------------------------------------------------
	void OnDemoteButtonClick()
	{
		if (m_Player && m_PartyModule)
		{
			m_PartyModule.DemotePlayer( m_Player.UID );
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if (m_KickButton && w == m_KickButton)
		{
			OnKickButtonClick();
			return true;
		}
		
		if (m_PromoteButton && w == m_PromoteButton)
		{
			OnPromoteButtonClick();
			return true;
		}
		
		if (m_DemoteButton && w == m_DemoteButton)
		{
			OnDemoteButtonClick();
			return true;
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( m_KickButton && w == m_KickButton )
		{
			m_KickButtonIcon.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}
		
		if ( m_PromoteButton && w == m_PromoteButton )
		{
			m_PromoteButtonIcon.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}
		
		if ( m_DemoteButton && w == m_DemoteButton )
		{
			m_DemoteButtonIcon.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_KickButton && w == m_KickButton )
		{
			m_KickButtonIcon.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}
		
		if ( m_PromoteButton && w == m_PromoteButton )
		{
			m_PromoteButtonIcon.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}
		
		if ( m_DemoteButton && w == m_DemoteButton )
		{
			m_DemoteButtonIcon.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}

		return false;
	}
}