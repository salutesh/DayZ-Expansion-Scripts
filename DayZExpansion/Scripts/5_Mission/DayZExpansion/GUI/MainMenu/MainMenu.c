/**
 * MainMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MainMenu
{
	#ifndef EXPANSION_MAINMENU_NEW_DISABLE
	protected DayZIntroSceneExpansion 			m_CustomScene;
	#endif
	
	protected Widget m_ModsInfoPanel;
	protected Widget m_NewsfeedPanel;
	ref ExpansionNewsfeed m_ExpansionNewsfeed;
	
	// ------------------------------------------------------------
	// MainMenu Constructor
	// ------------------------------------------------------------
	void MainMenu()
	{
		#ifndef EXPANSION_MAINMENU_NEW_DISABLE
		m_Mission					= MissionMainMenu.Cast( GetGame().GetMission() );
		m_CustomScene				= m_Mission.GetIntroSceneExpansion();
		#endif
	}
	
	// ------------------------------------------------------------
	// Override MainMenu Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Init - Start");
		#endif
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/gui/layouts/expansion_main_menu.layout" );
		
		m_Play						= layoutRoot.FindAnyWidget( "play" );
		m_ChooseServer				= layoutRoot.FindAnyWidget( "choose_server" );
		m_CustomizeCharacter		= layoutRoot.FindAnyWidget( "customize_character" );
		m_PlayVideo					= layoutRoot.FindAnyWidget( "play_video" );
		m_Tutorials					= layoutRoot.FindAnyWidget( "tutorials" );
		m_TutorialButton			= layoutRoot.FindAnyWidget( "tutorial_button" );
		m_MessageButton				= layoutRoot.FindAnyWidget( "message_button" );
		m_SettingsButton			= layoutRoot.FindAnyWidget( "settings_button" );
		m_Exit						= layoutRoot.FindAnyWidget( "exit_button" );
		m_PrevCharacter				= layoutRoot.FindAnyWidget( "prev_character" );
		m_NextCharacter				= layoutRoot.FindAnyWidget( "next_character" );

		m_Version					= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_ModdedWarning				= TextWidget.Cast( layoutRoot.FindAnyWidget( "ModdedWarning" ) );
		m_CharacterRotationFrame	= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		
		m_LastPlayedTooltip			= layoutRoot.FindAnyWidget( "last_server_info" );
		m_LastPlayedTooltip.Show(false);
		m_LastPlayedTooltipLabel	= m_LastPlayedTooltip.FindAnyWidget( "last_server_info_label" );
		m_LastPlayedTooltipName 	= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_name" ) );
		m_LastPlayedTooltipIP		= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_ip" ) );
		m_LastPlayedTooltipPort		= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_port" ) );
		
		m_LastPlayedTooltipTimer	= new WidgetFadeTimer();
		
		m_Stats						= new MainMenuStats( layoutRoot.FindAnyWidget( "character_stats_root" ) );
		
		m_Mission					= MissionMainMenu.Cast( GetGame().GetMission() );
		
		m_LastFocusedButton = 		m_Play;

		m_ScenePC					= m_Mission.GetIntroScenePC();
		
		if( m_ScenePC )
		{
			m_ScenePC.ResetIntroCamera();
		}
		
		m_PlayVideo.Show( false );
		
		m_PlayerName				= TextWidget.Cast( layoutRoot.FindAnyWidget("character_name_text") );
		
		//! Set Version
		string version;
		GetGame().GetVersion( version );
		string expansion_version;	
		
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		}
		else
		{
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version );
		}
		
		//m_ModsInfoPanel = Widget.Cast(layoutRoot.FindAnyWidget("ModsDetailed"));
		
		//! Newsfeed
		m_NewsfeedPanel = Widget.Cast(layoutRoot.FindAnyWidget("NewsFeedPanel"));
		m_ExpansionNewsfeed = new ExpansionNewsfeed(m_NewsfeedPanel);
		
		GetGame().GetUIManager().ScreenFadeOut(0);

		SetFocus( null );
		
		Refresh();
		
		LoadMods();
		
		GetDayZGame().GetBacklit().MainMenu_OnShow();
	
		g_Game.SetLoadState( DayZLoadState.MAIN_MENU_CONTROLLER_SELECT );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Init - End");
		#endif
		
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Override LoadMods
	// ------------------------------------------------------------
	override void LoadMods()
	{
		super.LoadMods();
				
		if (m_ModsDetailed)
			m_ModsDetailed.SetMainMenu(this);
		
		if (m_ModsSimple) 
			m_ModsSimple.SetMainMenu(this);
	}
	
	// ------------------------------------------------------------
	// Override Refresh
	// ------------------------------------------------------------
	override void Refresh()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Refresh - Start");
		#endif
		
		string version;
		GetGame().GetVersion( version );
		string expansion_version;	
		
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		}
		else
		{
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version );
		}
		
		HideModWarning();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Refresh - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::OnShow - Start");
		#endif
		
		SetFocus( null );
		OnChangeCharacter();
		LoadMods();
		HideModWarning();

		ExpansionBook book = ExpansionBook.Cast( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) );
		if ( book && book.IsVisible() )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MainMenu::OnShow - Book menu is still opened in main menu. Force closeing!");
			#endif

			book.Close();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::OnShow - End");
		#endif
		return;
	}
	
	// ------------------------------------------------------------
	// Expansion HideModWarning
	// ------------------------------------------------------------
	void HideModWarning()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::HideModWarning - Start");
		#endif
		
		// Hide moded game warning in the main menu
		m_ModdedWarning.Show(false);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::HideModWarning - End");
		#endif
	}
	#ifndef EXPANSION_MAINMENU_NEW_DISABLE
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if ( w == m_CharacterRotationFrame )
		{
			if (m_CustomScene)
				m_CustomScene.CharacterRotationStart();
			return true;
		}
		else if ( w == m_Play )
		{
			m_LastFocusedButton = m_Play;
			Play();
			return true;
		}
		else if ( w == m_ChooseServer )
		{
			m_LastFocusedButton = m_ChooseServer;
			OpenMenuServerBrowser();
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if (m_CustomScene)
			m_CustomScene.CharacterRotationStop();
		return false;
	}	
	#endif
	
	// ------------------------------------------------------------
	// ShowNewsfeed
	// ------------------------------------------------------------
	void ShowNewsfeed(bool state)
	{
		m_NewsfeedPanel.Show(state);
		m_ExpansionNewsfeed.ShowNewsfeed(state);
	}
}