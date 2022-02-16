/**
 * ExpansionSpawnSelectionMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawnSelectionMenu extends ExpansionUIScriptedMenu
{
	ref array< ref ExpansionSpawnSelectionEntry > m_SpawnPointsEntrys;
	ref array< ref ExpansionSpawnSelectionMarker > m_MapMarkers;
	
	protected ref ExpansionRespawnHandlerModule m_RespawnModule;
	
	protected GridSpacerWidget m_ListGrid;
	protected ButtonWidget m_Confirm;
	protected TextWidget m_ConfirmLable;
	protected ButtonWidget m_Random;
	protected TextWidget m_RandomLable;
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
		m_ConfirmLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "ConfirmText" ) );
		m_Random = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "Random" ) );
		m_RandomLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "RandomText" ) );
		m_MapPanel = Widget.Cast( layoutRoot.FindAnyWidget( "MapPanel" ) );
		m_Map = MapWidget.Cast( layoutRoot.FindAnyWidget( "Map" ) );

		Clear();
		
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu FillList
	// ------------------------------------------------------------		
	void FillList(array< ref ExpansionSpawnLocation> SpawnLocations, int type = 0)
	{
		if ( !SpawnLocations )
			return;

		int PrimaryColor = ARGB(255, 255, 255, 255);
		int HoverColor 	 = ARGB(255, 255, 255, 255);

		//! You can use this switch case for modding purposes
		switch( type )
		{
			case 0: //! Normal
				PrimaryColor = ARGB(255, 211, 84, 0);
				HoverColor 	 = ARGB(200, 255, 255, 255);
			break;
			case 1: //! Territory
				PrimaryColor = ARGB(255, 0, 102, 204);
				HoverColor 	 = ARGB(200, 255, 255, 255);
			break;
		}
		
		foreach( ExpansionSpawnLocation currenLocation : SpawnLocations )
		{
			ExpansionSpawnSelectionEntry location_entry = new ExpansionSpawnSelectionEntry( m_ListGrid, currenLocation );
			m_SpawnPointsEntrys.Insert( location_entry );
			
			//! Create map marker for territory
			vector pos;
			pos = currenLocation.Positions[0];
			
			ExpansionSpawnSelectionMarker marker = new ExpansionSpawnSelectionMarker( m_MapPanel, m_Map, true);
			marker.SetIcon( ExpansionIcons.Get( 46 ) );
			marker.SetPosition( pos );
			marker.SetPrimaryColor( PrimaryColor );
			marker.SetHoverColour( HoverColor );
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
	void SetSpawnPoint( ExpansionSpawnLocation location, bool setmappos = true)
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

		GetGame().GetInput().ChangeGameFocus(1);
		SetFocus( layoutRoot );
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

		if ( GetGame().GetMission() )
		{
			GetGame().GetInput().ResetGameFocus();
			PPEffects.SetBlurMenu( 0.0 );
			
			m_Mission.GetHud().ShowHud( true );
			m_Mission.GetHud().ShowQuickBar( true );
			m_Mission.GetExpansionHud().ShowHud( true );
			
			Clear();
		}

		//! If menu gets closed via nonstandard means, choose last selected spawn point
		if (!m_RespawnModule.m_SpawnSelected)
			m_RespawnModule.SelectSpawn(m_SelectedSpawnPoint);
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
			m_Confirm.Enable(false);
			break;
		case m_Random:
			Print("ExpansionSpawnSelectionMenu::OnClick - m_Random");
			m_RespawnModule.SelectRandomSpawn();
			m_Random.Enable(false);
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
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		switch ( w )
		{
			case m_Confirm:
			{
				w.SetColor( ARGB( 255,255,255,255 ) );
				m_ConfirmLable.SetColor( ARGB( 255,0,0,0 ) );
				break;
			}
			
			case m_Random:
			{
				w.SetColor( ARGB( 255,255,255,255 ) );
				m_RandomLable.SetColor( ARGB( 255,0,0,0 ) );
				break;
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch ( w )
		{
			case m_Confirm:
			{
				w.SetColor( ARGB( 255,0,0,0 ) );
				m_ConfirmLable.SetColor( ARGB( 255,255,255,255 ) );
				break;
			}
			
			case m_Random:
			{
				w.SetColor( ARGB( 255,0,0,0 ) );
				m_RandomLable.SetColor( ARGB( 255,255,255,255 ) );
				break;
			}
		}
		
		return false;
	}
}