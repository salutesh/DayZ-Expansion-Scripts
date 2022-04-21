/**
 * MainMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	protected ref ExpansionNewsfeed m_ExpansionNewsfeed;
	protected ref ExpansionModsMenuSimple m_ExpModsSimple;
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "UpdateItemInfoLiquidType");
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
				
		//! Newsfeed
		m_NewsfeedPanel = Widget.Cast(layoutRoot.FindAnyWidget("NewsFeedPanel"));
		m_ExpansionNewsfeed = new ExpansionNewsfeed(m_NewsfeedPanel);
		
		#ifdef EDITOR
		TextWidget playLable = TextWidget.Cast(layoutRoot.FindAnyWidget("play_label"));
		playLable.SetText("#STR_EDITOR_OPEN_EDITOR");
		
		ImageWidget logo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
		logo.SetFlags(WidgetFlags.STRETCH);
		logo.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		logo.SetImage(0);
		m_NewsfeedPanel.Show(false);
		
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version + " #STR_EDITOR_MAIN_MENU_VERSION" + " " + GetEditor().Version);
		}
		#else
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		}
		else
		{
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version );
		}
		#endif
		
		GetGame().GetUIManager().ScreenFadeOut(0);

		SetFocus( null );
		
		Refresh();
		
		LoadMods();
		
		GetDayZGame().GetBacklit().MainMenu_OnShow();
	
		g_Game.SetLoadState( DayZLoadState.MAIN_MENU_CONTROLLER_SELECT );
				
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Override LoadMods
	// ------------------------------------------------------------
	override void LoadMods()
	{
		array<ref ModInfo> modArray = new array<ref ModInfo>;		
		GetGame().GetModInfos( modArray );

		if( modArray.Count() > 0 )
		{
			modArray.Remove( modArray.Count() - 1 );
			modArray.Invert();
		}
		
		if( m_ExpModsSimple )
			delete m_ExpModsSimple;
		if( m_ModsDetailed )
			delete m_ModsDetailed;
		
		m_ModdedWarning.Show( GetGame().GetModToBeReported() );
		
		if( modArray.Count() > 0 )
		{
			layoutRoot.FindAnyWidget("ModsSimple").Show( true );
			m_ModsTooltip = new ModsMenuTooltip(layoutRoot);

			m_ModsDetailed = new ModsMenuDetailed(modArray, layoutRoot.FindAnyWidget("ModsDetailed"), m_ModsTooltip, this);
			
			m_ExpModsSimple = new ExpansionModsMenuSimple(modArray, layoutRoot.FindAnyWidget("ModsSimple"), m_ModsDetailed);
		}
				
		if (m_ModsDetailed)
			m_ModsDetailed.SetMainMenu(this);
		
		if (m_ExpModsSimple) 
			m_ExpModsSimple.SetMainMenu(this);
	}
	
	// ------------------------------------------------------------
	// Override Refresh
	// ------------------------------------------------------------
	override void Refresh()
	{		
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "Refresh");
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
	}
	
	// ------------------------------------------------------------
	// Override OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{	
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "OnShow");
#endif
		
		SetFocus( null );
		OnChangeCharacter();
		LoadMods();
		HideModWarning();

		return;
	}
	
	// ------------------------------------------------------------
	// Expansion HideModWarning
	// ------------------------------------------------------------
	void HideModWarning()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "HideModWarning");
#endif
		
		// Hide moded game warning in the main menu
		if ( m_ModdedWarning )
			m_ModdedWarning.Show(false);
		
	}

	#ifndef EXPANSION_MAINMENU_NEW_DISABLE
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if ( w == m_CharacterRotationFrame )
		{
			if (m_CustomScene)
				m_CustomScene.CharacterRotationStart();		
			if (m_ScenePC)
				m_ScenePC.CharacterRotationStart();
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
		if (m_ScenePC)
			m_ScenePC.CharacterRotationStop();
		
		return false;
	}	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		int charID;
		if( w == m_Play && !GetExpansionClientSettings().StreamerMode)
		{
			string ip = "";
			string name = "";
			int port = 0;
			 
			if(m_CustomScene && !m_CustomScene.GetIntroCharacter().IsDefaultCharacter())
			{
				charID = m_CustomScene.GetIntroCharacter().GetCharacterID();
				m_CustomScene.GetIntroCharacter().GetLastPlayedServer(charID, ip, name, port);
				
				m_LastPlayedTooltipName.SetText( "#server_details_name " + name );
				m_LastPlayedTooltipIP.SetText( "#main_menu_IP " + ip );
				m_LastPlayedTooltipPort.SetText( "#main_menu_port " + port );
				
				m_LastPlayedTooltipTimer.FadeIn( m_LastPlayedTooltip, 0.3, true );
			}
			if(m_ScenePC && !m_ScenePC.GetIntroCharacter().IsDefaultCharacter())
			{
				charID = m_ScenePC.GetIntroCharacter().GetCharacterID();
				m_ScenePC.GetIntroCharacter().GetLastPlayedServer(charID, ip, name, port);
				
				m_LastPlayedTooltipName.SetText( "#server_details_name " + name );
				m_LastPlayedTooltipIP.SetText( "#main_menu_IP " + ip );
				m_LastPlayedTooltipPort.SetText( "#main_menu_port " + port );
				
				m_LastPlayedTooltipTimer.FadeIn( m_LastPlayedTooltip, 0.3, true );
			}
		}
		
		if( IsFocusable( w ) )
		{
			ColorHighlight( w );
			return true;
		}
		return false;
	}	

	override bool CanSaveDefaultCharacter()
	{
		//TODO - check if default character exists
		if (m_CustomScene && m_CustomScene.GetIntroCharacter() && m_CustomScene.GetIntroCharacter().GetCharacterID() == -1)
		{
			return true;
		}		
		if (m_ScenePC && m_ScenePC.GetIntroCharacter() && m_ScenePC.GetIntroCharacter().GetCharacterID() == -1)
		{
			return true;
		}
		return false;
	}	
	
	override void OpenMenuServerBrowser()
	{
		EnterScriptedMenu(MENU_SERVER_BROWSER);
		PlayerBase player;		
		//saves demounit for further use
		if (m_CustomScene && m_CustomScene.GetIntroCharacter() && m_CustomScene.GetIntroCharacter().GetCharacterID() == -1 )
		{
			player = m_CustomScene.GetIntroCharacter().GetCharacterObj();
			if(player && player.GetInventory() && player.GetInventory().FindAttachment(InventorySlots.BODY))
			{
				//todo - save default char here if none exists
				//m_CustomScene.GetIntroCharacter().SaveCharacterSetup();
			}
		}		
		if (m_ScenePC && m_ScenePC.GetIntroCharacter() && m_ScenePC.GetIntroCharacter().GetCharacterID() == -1 )
		{
			player = m_ScenePC.GetIntroCharacter().GetCharacterObj();
			if(player && player.GetInventory() && player.GetInventory().FindAttachment(InventorySlots.BODY))
			{
				//todo - save default char here if none exists
				//m_ScenePC.GetIntroCharacter().SaveCharacterSetup();
			}
		}
	}

	override void NextCharacter()
	{
		int charID;
		if ( m_CustomScene && m_CustomScene.GetIntroCharacter() )
		{
			charID = m_CustomScene.GetIntroCharacter().GetNextCharacterID();
			//m_CustomScene.GetIntroCharacter().SaveCharName();
			if( charID != m_CustomScene.GetIntroCharacter().GetCharacterID())
			{
				m_CustomScene.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}		
		if ( m_ScenePC && m_ScenePC.GetIntroCharacter() )
		{
			charID = m_ScenePC.GetIntroCharacter().GetNextCharacterID();
			//m_ScenePC.GetIntroCharacter().SaveCharName();
			if( charID != m_ScenePC.GetIntroCharacter().GetCharacterID())
			{
				m_ScenePC.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}
	}
	
	override void PreviousCharacter()
	{
		int charID;
		if ( m_CustomScene && m_CustomScene.GetIntroCharacter() )
		{
			//m_CustomScene.GetIntroCharacter().SaveCharName();
			charID = m_CustomScene.GetIntroCharacter().GetPrevCharacterID();
			if( charID != m_CustomScene.GetIntroCharacter().GetCharacterID())
			{
				m_CustomScene.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}		
		if ( m_ScenePC && m_ScenePC.GetIntroCharacter() )
		{
			//m_ScenePC.GetIntroCharacter().SaveCharName();
			charID = m_ScenePC.GetIntroCharacter().GetPrevCharacterID();
			if( charID != m_ScenePC.GetIntroCharacter().GetCharacterID())
			{
				m_ScenePC.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}
	}
	
	override void OnChangeCharacter(bool create_character = true)
	{
		int charID;
		Widget w;
		TextWidget text;
		if ( m_CustomScene && m_CustomScene.GetIntroCharacter() )
		{
			charID = m_CustomScene.GetIntroCharacter().GetCharacterID();
			if (create_character)
			{
				m_CustomScene.GetIntroCharacter().CreateNewCharacterById( charID );
			}
			m_PlayerName.SetText( m_CustomScene.GetIntroCharacter().GetCharacterNameById( charID ) );
			
			w = m_CustomizeCharacter.FindAnyWidget(m_CustomizeCharacter.GetName() + "_label");
			
			if ( w )
			{
				text = TextWidget.Cast( w );
				
				if( m_CustomScene.GetIntroCharacter().IsDefaultCharacter() )
				{
					text.SetText("#layout_main_menu_customize_char");
				}
				else
				{
					text.SetText("#layout_main_menu_rename");
				}
			}
			if (m_CustomScene.GetIntroCharacter().GetCharacterObj() )
			{
				if ( m_CustomScene.GetIntroCharacter().GetCharacterObj().IsMale() )
					m_CustomScene.GetIntroCharacter().SetCharacterGender(ECharGender.Male);
				else
					m_CustomScene.GetIntroCharacter().SetCharacterGender(ECharGender.Female);
			}
			
			//update character stats
			m_Stats.UpdateStats();
		}		
		if ( m_ScenePC && m_ScenePC.GetIntroCharacter() )
		{
			charID = m_ScenePC.GetIntroCharacter().GetCharacterID();
			if (create_character)
			{
				m_ScenePC.GetIntroCharacter().CreateNewCharacterById( charID );
			}
			m_PlayerName.SetText( m_ScenePC.GetIntroCharacter().GetCharacterNameById( charID ) );
			
			w = m_CustomizeCharacter.FindAnyWidget(m_CustomizeCharacter.GetName() + "_label");
			
			if ( w )
			{
				text = TextWidget.Cast( w );
				
				if( m_ScenePC.GetIntroCharacter().IsDefaultCharacter() )
				{
					text.SetText("#layout_main_menu_customize_char");
				}
				else
				{
					text.SetText("#layout_main_menu_rename");
				}
			}
			if (m_ScenePC.GetIntroCharacter().GetCharacterObj() )
			{
				if ( m_ScenePC.GetIntroCharacter().GetCharacterObj().IsMale() )
					m_ScenePC.GetIntroCharacter().SetCharacterGender(ECharGender.Male);
				else
					m_ScenePC.GetIntroCharacter().SetCharacterGender(ECharGender.Female);
			}
			
			//update character stats
			m_Stats.UpdateStats();
		}
		if (m_CustomScene)
		{
			m_CustomScene.SetAnim();
			m_CustomScene.CustomHandItem();
		};
	}

	override void ConnectLastSession()
	{
		string ip = "";
		string name = "";
		int port = 0;
		int charID;
			 
		if(m_CustomScene && !m_CustomScene.GetIntroCharacter().IsDefaultCharacter())
		{
			charID = m_CustomScene.GetIntroCharacter().GetCharacterID();
			m_CustomScene.GetIntroCharacter().GetLastPlayedServer(charID,ip,name,port);
		}		
		if(m_ScenePC && !m_ScenePC.GetIntroCharacter().IsDefaultCharacter())
		{
			charID = m_ScenePC.GetIntroCharacter().GetCharacterID();
			m_ScenePC.GetIntroCharacter().GetLastPlayedServer(charID,ip,name,port);
		}
		
		if( ip.Length() > 0 )
		{
			g_Game.ConnectFromServerBrowser( ip, port, "" );
		}
		else
		{
			OpenMenuServerBrowser();
		}
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
#ifndef EXPANSION_MAINMENU_NEW_DISABLE
modded class MainMenuStats
{
	override void UpdateStats()
	{
		PlayerBase player;
		MissionMainMenu mission_main_menu = MissionMainMenu.Cast( GetGame().GetMission() );
		
		#ifdef PLATFORM_WINDOWS
		if (mission_main_menu.GetIntroSceneExpansion())
			player = mission_main_menu.GetIntroSceneExpansion().GetIntroCharacter().GetCharacterObj();
		if (mission_main_menu.GetIntroScenePC())
			player = mission_main_menu.GetIntroScenePC().GetIntroCharacter().GetCharacterObj();
		#endif
		#ifdef PLATFORM_CONSOLE
			player = mission_main_menu.GetIntroScenePC().GetIntroCharacter().GetCharacterObj();
		#endif
		
		if ( player )
		{
			float stat_value;
			string stat_text;
			
			m_TimeSurvivedValue.SetText( ExpansionStatic.GetTimeString( player.StatGet( AnalyticsManagerServer.STAT_PLAYTIME ) ) );
			m_PlayersKilledValue.SetText( ExpansionStatic.GetValueString( player.StatGet( AnalyticsManagerServer.STAT_PLAYERS_KILLED ) ) );
			m_InfectedKilledValue.SetText( ExpansionStatic.GetValueString( player.StatGet( AnalyticsManagerServer.STAT_INFECTED_KILLED ) ) );
			m_DistanceTraveledValue.SetText( ExpansionStatic.GetDistanceString( player.StatGet( AnalyticsManagerServer.STAT_DISTANCE ) ) );
			m_LongRangeShotValue.SetText( ExpansionStatic.GetDistanceString( player.StatGet( AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT ), true ) );
		}
	}
	
	
}
#endif
