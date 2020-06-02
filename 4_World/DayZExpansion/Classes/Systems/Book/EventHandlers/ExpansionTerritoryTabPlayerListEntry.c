/**
 * ExpansionTerritoryTabPlayerListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTerritoryTabPlayerListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_PlayerNameText;
	protected ButtonWidget m_InviteButton;
	protected ImageWidget m_InviteButtonIcon;
	
	protected string m_PlayerID;
	protected string m_PlayerName;
	protected ExpansionTerritoryModule m_TerritoryModule;
	protected ExpansionTerritoryFlag m_Flag;
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritoryTabPlayerListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionTerritoryTabPlayerListEntry(Widget parent, string playerID, string playerName, ExpansionTerritoryFlag flag)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryTabPlayerListEntry::ExpansionTerritoryTabPlayerListEntry - Start");
		#endif
		
		if (!flag) return;
		
		m_Root				= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/territory/expansion_territory_menu_player_list_entry.layout", parent ) );
		m_PlayerNameText	= TextWidget.Cast( m_Root.FindAnyWidget( "player_name" ) );
		m_InviteButton		= ButtonWidget.Cast( m_Root.FindAnyWidget( "player_invite_button" ) );
		m_InviteButtonIcon	= ImageWidget.Cast( m_Root.FindAnyWidget( "player_invite_button_icon" ) );
		
		m_PlayerID			= playerID;
		m_PlayerName		= playerName;
		
		m_Flag = flag;
		
		m_TerritoryModule	= ExpansionTerritoryModule.Cast( GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		
		m_Root.SetHandler(this);
		
		SetPlayerEntry();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryTabPlayerListEntry::ExpansionTerritoryTabPlayerListEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritoryTabPlayerListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionTerritoryTabPlayerListEntry()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryTabPlayerListEntry::~ExpansionTerritoryTabPlayerListEntry - Start");
		#endif
		
		delete m_Root;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGroupMenuPlayerListEntry::~ExpansionTerritoryTabPlayerListEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetPlayerEntry
	// ------------------------------------------------------------
	void SetPlayerEntry()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGroupMenuPlayerListEntry::SetPlayerEntry - Start");
		#endif

		m_PlayerNameText.SetText( m_PlayerName );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGroupMenuPlayerListEntry::SetPlayerEntry - End");
		#endif
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

			m_TerritoryModule.RequestInvitePlayer( m_PlayerID, m_Flag );
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
			m_InviteButtonIcon.SetColor( ARGB( 255, 255, 255, 255 ) );
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
			m_InviteButtonIcon.SetColor( ARGB( 255, 0, 0, 0 ) );
			return true;
		}

		return false;
	}
}