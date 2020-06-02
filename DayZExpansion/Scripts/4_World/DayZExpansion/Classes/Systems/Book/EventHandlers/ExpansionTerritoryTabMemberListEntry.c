/**
 * ExpansionTerritoryTabMemberListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTerritoryTabMemberListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_MemberName;
	protected ButtonWidget m_EditMemberButton;
	protected ImageWidget m_EditMemberButtonIcon;
	protected ImageWidget m_MemberStateIcon;
	
	protected ExpansionTerritoryModule m_TerritoryModule;
	protected ref ExpansionTerritoryMember m_Member;
	protected ref ExpansionBookTabTerritory m_TerritoryMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritoryTabMemberListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionTerritoryTabMemberListEntry(Widget parent, ExpansionBookTabTerritory menu, ref ExpansionTerritoryMember member)
	{
		m_Root					= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/territory/expansion_territory_menu_member_list_entry.layout", parent ) );
		m_MemberName			= TextWidget.Cast( m_Root.FindAnyWidget( "member_name" ) );
		m_EditMemberButton		= ButtonWidget.Cast( m_Root.FindAnyWidget( "member_edit_button" ) );
		m_EditMemberButtonIcon	= ImageWidget.Cast( m_Root.FindAnyWidget( "member_edit_button_icon" ) );
		m_MemberStateIcon		= ImageWidget.Cast( m_Root.FindAnyWidget( "member_status_icon" ) );
		
		m_Member				= member;
		m_TerritoryMenu			= menu;
		
		m_TerritoryModule		= ExpansionTerritoryModule.Cast( GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		
		m_Root.SetHandler( this );
		
		SetMemberEntry();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritoryTabMemberListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionTerritoryTabMemberListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetMemberEntry
	// ------------------------------------------------------------
	void SetMemberEntry()
	{
		m_MemberName.SetText( m_Member.GetName() );
			
		if ( IsOnline( m_Member.GetID() ) )
		{
			m_MemberStateIcon.SetColor( ARGB( 255, 39, 174, 96 ) );
		}
		else
		{
			m_MemberStateIcon.SetColor( ARGB( 255, 192, 57, 43 ) );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoriesCOTMemberListEntry IsOnline
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
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( m_EditMemberButton && w == m_EditMemberButton )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionTerritoryTabMemberListEntry::OnClick - Clicked m_EditMemberButton");
			#endif
			
			if ( m_TerritoryMenu )
			{
				m_TerritoryMenu.SetMemberInfo( m_Member );
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
			m_EditMemberButtonIcon.SetColor( ARGB( 255, 255, 255, 255 ) );
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
			m_EditMemberButtonIcon.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}

		return false;
	}
}