/**
 * ExpansionCOTTerritoriesMemberListEntry.c
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
class ExpansionCOTTerritoriesMemberListEntry extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_MemberName;
	private TextWidget m_MemberID;
	private ButtonWidget m_EditMemberButton;
	private ImageWidget m_MemberStateIcon;
	
	private ref ExpansionTerritoryMember m_Member;
	private ref ExpansionCOTTerritoriesMenu m_TerritoryCOTMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesMemberListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesMemberListEntry(Widget parent, ExpansionCOTTerritoriesMenu menu, ExpansionTerritoryMember member)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::ExpansionCOTTerritoriesMemberListEntry - Start");
		#endif
		
		m_Root					= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/BaseBuilding/GUI/layouts/COT/territories/Territories_MemberList_Entry_New.layout", parent ) );
		m_MemberName			= TextWidget.Cast( m_Root.FindAnyWidget( "member_name" ) );
		m_MemberID				= TextWidget.Cast( m_Root.FindAnyWidget( "owner_id" ) );
		m_EditMemberButton		= ButtonWidget.Cast( m_Root.FindAnyWidget( "member_entry" ) );
		m_MemberStateIcon		= ImageWidget.Cast( m_Root.FindAnyWidget( "member_status_icon" ) );
	
		m_Member				= member;
		m_TerritoryCOTMenu		= menu;
		
		m_Root.SetHandler( this );
		
		SetMemberEntry();
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::ExpansionCOTTerritoriesMemberListEntry - End");
		#endif
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
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::SetMemberEntry - Start");
		#endif
		
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
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::SetMemberEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoriesCOTMemberListEntry IsOnline
	// ------------------------------------------------------------
	bool IsOnline( string uid )
	{
		foreach(  SyncPlayer player : ClientData.m_PlayerList.m_PlayerList )
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
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::SetMemberEntry - Start");
		#endif
		
		if ( m_EditMemberButton && w == m_EditMemberButton )
		{			
			if ( m_TerritoryCOTMenu )
			{
				m_TerritoryCOTMenu.SetTerritoryMemberInfo( m_Member );
			}
			
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::SetMemberEntry - m_EditMemberButton");
			#endif
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::SetMemberEntry - End");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::OnMouseEnter - Start");
		#endif
		
		if ( m_EditMemberButton && w == m_EditMemberButton )
		{
			m_MemberName.SetColor( ARGB( 255, 0, 0, 0 ) );
			m_MemberID.SetColor( ARGB( 255, 0, 0, 0 ) );
			
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::OnMouseEnter - m_EditMemberButton");
			#endif
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::OnMouseEnter - End");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::OnMouseLeave - Start");
		#endif
		
		if ( m_EditMemberButton && w == m_EditMemberButton )
		{
			m_MemberName.SetColor( ARGB( 255, 255, 255, 255 ) );
			m_MemberID.SetColor( ARGB( 255, 255, 255, 255 ) );
			
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::OnMouseLeave - m_EditMemberButton");
			#endif
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMemberListEntry::OnMouseLeave - End");
		#endif

		return false;
	}
};
#endif
