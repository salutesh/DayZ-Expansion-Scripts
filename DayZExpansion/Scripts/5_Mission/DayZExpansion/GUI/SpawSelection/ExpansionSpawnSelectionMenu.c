/**
 * ExpansionSpawnSelectionMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawnSelectionMenu extends UIScriptedMenu
{
	ref array< ref ExpansionSpawnSelectionEntry > m_SpawnPointsEntrys;
	ref array< ref ExpansionSpawnSelectionMarker > m_MapMarkers;
	
	protected ref ExpansionRespawnHandlerModule m_RespawnModule;
	
	protected GridSpacerWidget m_ListGrid;
	protected ButtonWidget m_Confirm;
	protected ButtonWidget m_Random;
	protected Widget m_MapPanel;
	protected MapWidget m_Map;
	
	vector m_SelectedSpawnPoint = Vector(0,0,0);
	
	protected MissionGameplay m_Mission;
	
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu Constructor
	// ------------------------------------------------------------
	void ExpansionSpawnSelectionMenu()
	{
		m_SpawnPointsEntrys = new array< ref ExpansionSpawnSelectionEntry >;
		m_MapMarkers =  new array<ref ExpansionSpawnSelectionMarker>;
		
		Class.CastTo( m_RespawnModule, GetModuleManager().GetModule( ExpansionRespawnHandlerModule ) );
		Class.CastTo( m_Mission, MissionGameplay.Cast( GetGame().GetMission() ) );
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu Destructor
	// ------------------------------------------------------------
	void ~ExpansionSpawnSelectionMenu()
	{
		Clear();
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu Widget Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_spawn_selection_new.layout");
		
		m_ListGrid = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "ListGrid" ) );
		m_Confirm = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "Confirm" ) );
		m_Random = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "Random" ) );
		m_MapPanel = Widget.Cast( layoutRoot.FindAnyWidget( "MapPanel" ) );
		m_Map = MapWidget.Cast( layoutRoot.FindAnyWidget( "Map" ) );
				
		FillList();
		
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu FillList
	// ------------------------------------------------------------		
	void FillList()
	{
		Clear();
		
		foreach( ExpansionSpawnLocation currenLocation : GetExpansionSettings().GetSpawn().SpawnLocations )
		{
			ExpansionSpawnSelectionEntry location_entry = new ExpansionSpawnSelectionEntry( m_ListGrid, currenLocation );
			m_SpawnPointsEntrys.Insert( location_entry );
			
			//! Create map marker for territory
			vector pos;
			pos = currenLocation.Positions[0];
			
			ExpansionSpawnSelectionMarker marker = new ExpansionSpawnSelectionMarker( m_MapPanel, m_Map, true);
			marker.SetIcon( ExpansionIcons.Get( 46 ) );
			marker.SetPosition( pos );
			marker.SetPrimaryColor( ARGB( 255, 211, 84, 0 ) );
			marker.SetHoverColour( ARGB( 200, 255, 255, 255 ) );
			marker.SetName( currenLocation.Name );
			marker.SetLocation( currenLocation );
			
			m_MapMarkers.Insert( marker );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu ClearList
	// ------------------------------------------------------------	
	void Clear()
	{		
		m_SpawnPointsEntrys.Clear();
		m_MapMarkers.Clear();
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu OnShow
	// ------------------------------------------------------------
	void SetSpawnPoint(ref ExpansionSpawnLocation location, bool setmappos = true)
	{
		Print("ExpansionSpawnSelectionMenu::SetSpawnPoint - Start");
		
		m_SelectedSpawnPoint = location.Positions.GetRandomElement();
		
		if ( setmappos )
		{			
			vector mapPos = location.Positions[0];
			
			m_Map.SetScale( 0.2 );
			m_Map.SetMapPos( Vector( mapPos[0] + 900.0, 0, mapPos[2] - 600.0 ) );
		}
		
		Print("ExpansionSpawnSelectionMenu::SetSpawnPoint - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		super.OnShow();

		SetFocus( layoutRoot );
		GetGame().GetMission().PlayerControlDisable( INPUT_EXCLUDE_ALL );
		PPEffects.SetBlurMenu( 0.5 );
		
		m_Mission.GetHud().ShowHud( false );
		m_Mission.GetHud().ShowQuickBar( false );
		m_Mission.GetExpansionHud().ShowHud( false );
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();

		GetGame().GetMission().PlayerControlEnable( true );
		PPEffects.SetBlurMenu( 0.0 );
		
		m_Mission.GetHud().ShowHud( true );
		m_Mission.GetHud().ShowQuickBar( true );
		m_Mission.GetExpansionHud().ShowHud( true );
		
		Clear();
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("ExpansionSpawnSelectionMenu::OnClick - Start");

		switch ( w )
		{
		case m_Confirm:
			Print("ExpansionSpawnSelectionMenu::OnClick - m_Confirm");
			if ( m_SelectedSpawnPoint != Vector( 0, 0, 0 ) )
				m_RespawnModule.SelectSpawn( m_SelectedSpawnPoint );
			break;
		case m_Random:
			Print("ExpansionSpawnSelectionMenu::OnClick - m_Random");
			m_RespawnModule.SelectRandomSpawn();
			break;
		}
		
		Print("ExpansionSpawnSelectionMenu::OnClick - End");
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu UseKeyboard
	// ------------------------------------------------------------
	override bool UseKeyboard() 
	{ 
		return false; 
	}

	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu UseMouse
	// ------------------------------------------------------------
	override bool UseMouse()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu Update
	// ------------------------------------------------------------
	override void Update(float timeslice) 
	{
		for ( int i = 0; i < m_MapMarkers.Count(); i++ )
		{
			m_MapMarkers[i].Update(0.5);
		}
	}
}