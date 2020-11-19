/**
 * ExpansionSpawnSelectionMenuOld.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
/**
 * ExpansionCOTTerritoriesMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*class ExpansionSpawnSelectionMenuOld extends UIScriptedMenu
{
	protected ButtonWidget m_Spawn0;
	protected Widget m_Spawn0Panel;
	protected TextWidget m_Spawn0Text;
	
	protected ButtonWidget m_Spawn1;
	protected Widget m_Spawn1Panel;
	protected TextWidget m_Spawn1Text;
	
	protected ButtonWidget m_Spawn2;
	protected Widget m_Spawn2Panel;
	protected TextWidget m_Spawn2Text;
	
	protected ButtonWidget m_Spawn3;
	protected Widget m_Spawn3Panel;
	protected TextWidget m_Spawn3Text;
	
	protected ButtonWidget m_Spawn4;
	protected Widget m_Spawn4Panel;
	protected TextWidget m_Spawn4Text;
	
	protected ButtonWidget m_Spawn5;
	protected Widget m_Spawn5Panel;
	protected TextWidget m_Spawn5Text;
	
	protected ButtonWidget m_Random;
	
	protected ref ExpansionRespawnHandlerModule m_RespawnModule;
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu Constructor
	// ------------------------------------------------------------
	void ExpansionSpawnSelectionMenu()
	{
		if ( !Class.CastTo( m_RespawnModule, GetModuleManager().GetModule( ExpansionRespawnHandlerModule ) ) )
			return;
	}
	
	// ------------------------------------------------------------
	// Override Widget Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSelectionMenu::Init - Start");
		#endif
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_spawn_selection.layout");

		m_Spawn0 = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "spawn0Button" ) );
		m_Spawn0Panel = Widget.Cast( layoutRoot.FindAnyWidget( "spawn0Panel" ) );
		m_Spawn0Text = TextWidget.Cast( layoutRoot.FindAnyWidget( "spawn0Text" ) );
		
		m_Spawn1 = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "spawn1Button" ) );
		m_Spawn1Panel = Widget.Cast( layoutRoot.FindAnyWidget( "spawn1Panel" ) );
		m_Spawn1Text = TextWidget.Cast( layoutRoot.FindAnyWidget( "spawn1Text" ) );
		
		m_Spawn2 = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "spawn2Button" ) );
		m_Spawn2Panel = Widget.Cast( layoutRoot.FindAnyWidget( "spawn2Panel" ) );
		m_Spawn2Text = TextWidget.Cast( layoutRoot.FindAnyWidget( "spawn2Text" ) );
		
		m_Spawn3 = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "spawn3Button" ) );
		m_Spawn3Panel = Widget.Cast( layoutRoot.FindAnyWidget( "spawn3Panel" ) );
		m_Spawn3Text = TextWidget.Cast( layoutRoot.FindAnyWidget( "spawn3Text" ) );
		
		m_Spawn4 = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "spawn4Button" ) );
		m_Spawn4Panel = Widget.Cast( layoutRoot.FindAnyWidget( "spawn4Panel" ) );
		m_Spawn4Text = TextWidget.Cast( layoutRoot.FindAnyWidget( "spawn4Text" ) );
		
		m_Spawn5 = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "spawn5Button" ) );
		m_Spawn5Panel = Widget.Cast( layoutRoot.FindAnyWidget( "spawn5Panel" ) );
		m_Spawn5Text = TextWidget.Cast( layoutRoot.FindAnyWidget( "spawn5Text" ) );
		
		m_Random = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "randomSpawn" ) );
		
		// Left side
		m_Spawn0Panel.SetRotation(0, 0, 5);
		m_Spawn1Panel.SetRotation(0, 0, -7);
		m_Spawn2Panel.SetRotation(0, 0, -7);
		// Right side
		m_Spawn3Panel.SetRotation(0, 0, -7);
		m_Spawn4Panel.SetRotation(0, 0, 5);
		m_Spawn5Panel.SetRotation(0, 0, 4);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSelectionMenu::Init - End");
		#endif
		return layoutRoot;
	}
	
	void ~ExpansionSpawnSelectionMenu()
	{

	}
	
	// ------------------------------------------------------------
	// Override Bool UseKeyboard
	// ------------------------------------------------------------
	override bool UseKeyboard() 
	{ 
		return false; 
	}

	// ------------------------------------------------------------
	// Override Bool UseMouse
	// ------------------------------------------------------------
	override bool UseMouse()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Override OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSelectionMenu::OnShow - Start");
		#endif
		super.OnShow();

		SetFocus( layoutRoot );
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		PPEffects.SetBlurMenu( 0.5 );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSelectionMenu::OnShow - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{				
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSelectionMenu::OnHide - Start");
		#endif
		super.OnHide();

		GetGame().GetMission().PlayerControlEnable(true);
		PPEffects.SetBlurMenu( 0.0 );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSelectionMenu::OnHide - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Bool OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w == m_Spawn0 )
		{
			Spawn( 0 );
			return true;
		}
		if ( w == m_Spawn1 )
		{
			Spawn( 1 );
			return true;
		}
		if ( w == m_Spawn2 )
		{
			Spawn( 2 );
			return true;
		}
		if ( w == m_Spawn3 )
		{
			Spawn( 3 );
			return true;
		}
		if ( w == m_Spawn4 )
		{
			Spawn( 4 );
			return true;
		}
		if ( w == m_Spawn5 )
		{
			Spawn( 5 );
			return true;
		}
		if ( w == m_Random )
		{
			Spawn( Math.RandomInt(0, 5) );
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override Update
	// ------------------------------------------------------------
	override void Update(float timeslice)
	{
		if ( layoutRoot.IsVisible() )
		{
			GetGame().GetUIManager().ShowUICursor( true );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Spawn
	// ------------------------------------------------------------
	void Spawn( int position )
	{
		m_RespawnModule.SelectSpawn( position );
		//GetRPCManager().SendRPC( "DayZExpansion", "SelectSpawnPosition", new Param1< int >( position ), true );
	}
}*/