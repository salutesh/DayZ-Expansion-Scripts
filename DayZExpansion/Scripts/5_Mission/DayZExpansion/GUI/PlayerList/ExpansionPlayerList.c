/**
 * ExpansionPlayerList.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPlayerListEntry extends ScriptedWidgetEventHandler
{	
	protected Widget m_Root;
	protected TextWidget m_NameWidget;
	
	protected string m_PlayerName;
	
	// ------------------------------------------------------------
	// ExpansionPlayerListEntry Constructor
	// ------------------------------------------------------------	
	void ExpansionPlayerListEntry(Widget parent, string name)
	{
		m_Root	= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/player_list/expansion_player_list_entry.layout", parent ) );
		m_NameWidget = TextWidget.Cast( m_Root.FindAnyWidget("player_name") );
		
		m_PlayerName = name;
		
		m_Root.SetHandler(this);
		
		SetEntry();
	}
	
	// ------------------------------------------------------------
	// ExpansionPlayerListEntry Destructor
	// ------------------------------------------------------------	
	void ~ExpansionPlayerListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionPlayerListEntry SetEntry
	// ------------------------------------------------------------
	protected void SetEntry()
	{
		m_NameWidget.SetText(m_PlayerName);
	}
}

class ExpansionPlayerList extends UIScriptedMenu
{
	protected GridSpacerWidget m_PlayerListGrid;
	protected TextWidget m_PlayerTotal;
	protected ref array<ref ExpansionPlayerListEntry> m_PlayerList;
	protected float m_OpenListTime = 0;
	
	// ------------------------------------------------------------
	// ExpansionPlayerList Constructor
	// ------------------------------------------------------------	
	void ExpansionPlayerList()
	{			
		if ( !m_PlayerList )
			m_PlayerList = new array<ref ExpansionPlayerListEntry>;
	}
	
	// ------------------------------------------------------------
	// ExpansionPlayerList Destructor
	// ------------------------------------------------------------	
	void ~ExpansionPlayerList()
	{
	}

	// ------------------------------------------------------------
	// Expansion Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/player_list/expansion_player_list.layout");
	
		m_PlayerListGrid = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget("ListGrid") );
		m_PlayerTotal = TextWidget.Cast( layoutRoot.FindAnyWidget("TotalPlayersVal") );
		
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion Override Init
	// ------------------------------------------------------------
	protected void CreatePlayerList()
	{				
		if ( m_PlayerList && m_PlayerList.Count() > 0 )
		{
			while (m_PlayerList.Count() > 0)
			{
				int idx = m_PlayerList.Count() - 1;
				
				delete m_PlayerList[idx];
				m_PlayerList.Remove(idx);
			}
			
			m_PlayerList.Clear();
		}
		
		for ( int i = 0; i < ClientData.m_PlayerList.m_PlayerList.Count(); i++ )
		{
			ref SyncPlayer player = ClientData.m_PlayerList.m_PlayerList.Get( i );

			if ( player )
			{
				ExpansionPlayerListEntry entry = new ExpansionPlayerListEntry( m_PlayerListGrid, player.m_PlayerName );
				m_PlayerList.Insert( entry );
			}
		}
		
		if ( m_PlayerList.Count() > 0 )
			m_PlayerTotal.SetText( m_PlayerList.Count().ToString() );
	}
	
	// ------------------------------------------------------------
	// Expansion Override OnShow
	// ------------------------------------------------------------	
	override void OnShow()
	{
		super.OnShow();

		m_OpenListTime = 0;
		
		SetFocus(layoutRoot);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		PPEffects.SetBlurMenu(0.5);
		
		CreatePlayerList();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater( this.CreatePlayerList, 3000, true );
		
		Show();
	}
	
	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();
	
		GetGame().GetMission().PlayerControlEnable(true);
		
		PPEffects.SetBlurMenu( 0.0 );
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove( this.CreatePlayerList );
	}
	
	// ------------------------------------------------------------
	// Expansion Show
	// ------------------------------------------------------------	
	void Show()
	{
		if (layoutRoot)
			layoutRoot.Show(true);
	}
	
	// ------------------------------------------------------------
	// Expansion Hide
	// ------------------------------------------------------------	
	void Hide()
	{
		if (layoutRoot)
			layoutRoot.Show(false);
	}
	
	// ------------------------------------------------------------
	// Expansion Override Update
	// ------------------------------------------------------------	
	override void Update( float timeslice )
	{		
		m_OpenListTime += timeslice;
		
		Input input = GetGame().GetInput();
		if ( input.LocalPress( "UAUIBack", false ) )
		{		
			Hide();	
			Close();

			return;
		}
		
		if ( input.LocalPress( "UAExpansionPlayerListToggle", false ) && m_OpenListTime > 0.75 )
		{		
			Hide();	
			Close();

			return;
		}
		
		super.Update( timeslice );
	}
}