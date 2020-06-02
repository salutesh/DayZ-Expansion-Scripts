/**
 * ExpansionCOTTerritoriesMemberListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoriesMemberListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_MemberName;
	protected TextWidget m_MemberID;
	protected ButtonWidget m_EditMemberButton;
	protected ImageWidget m_MemberStateIcon;
	
	protected ref ExpansionTerritoryMember m_Member;
	protected ref ExpansionCOTTerritoriesMenu m_TerritoryCOTMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMemberListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesMemberListEntry(Widget parent, ExpansionCOTTerritoriesMenu menu, ExpansionTerritoryMember member)
	{
		m_Root					= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/COT/territories/Territories_MemberList_Entry.layout", parent ) );
		m_MemberName			= TextWidget.Cast( m_Root.FindAnyWidget( "member_name" ) );
		m_MemberID				= TextWidget.Cast( m_Root.FindAnyWidget( "owner_id" ) );
		m_EditMemberButton		= ButtonWidget.Cast( m_Root.FindAnyWidget( "member_edit_button" ) );
		m_MemberStateIcon		= ImageWidget.Cast( m_Root.FindAnyWidget( "member_status_icon" ) );
	
		m_Member				= member;
		m_TerritoryCOTMenu		= menu;
		
		m_Root.SetHandler( this );
		
		SetMemberEntry();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMemberListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTTerritoriesMemberListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetMemberEntry
	// ------------------------------------------------------------
	void SetMemberEntry()
	{
		if ( m_Member )
		{
			m_MemberName.SetText( m_Member.GetName() );
			m_MemberID.SetText( m_Member.GetID() );
			
			if ( IsOnline( m_Member.GetID() ) )
			{
				m_MemberStateIcon.SetColor( ARGB( 255, 39, 174, 96 ) );
			}
			else
			{
				m_MemberStateIcon.SetColor( ARGB( 255, 192, 57, 43 ) );
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoriesCOTMemberListEntry IsOnline
	// ------------------------------------------------------------
	bool IsOnline( string uid )
	{
		foreach( ref SyncPlayer player : ClientData.m_PlayerList.m_PlayerList )
		{
			if ( player.m_RUID == uid )
			{
				return true;
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( m_EditMemberButton && w == m_EditMemberButton )
		{			
			if ( m_TerritoryCOTMenu )
			{
				m_TerritoryCOTMenu.SetTerritoryMemberInfo( m_Member );
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
		if ( m_EditMemberButton && w == m_EditMemberButton )
		{
			m_MemberName.SetColor( ARGB( 255, 0, 0, 0 ) );
			m_MemberID.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_EditMemberButton && w == m_EditMemberButton )
		{
			m_MemberName.SetColor( ARGB( 255, 255, 255, 255 ) );
			m_MemberID.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}

		return false;
	}
}