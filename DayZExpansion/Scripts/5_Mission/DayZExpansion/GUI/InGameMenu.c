/**
 * InGameMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
#ifndef CF_DebugUI
modded class InGameMenu
{
	protected static float 					DEAD_SCREEN_FADEIN_TIME = 3.0;					//fade starts as soon as deadscreen gets diplayed
	protected static float 					DEAD_SCREEN_IMAGE_FADEIN_TIME = 1.5;			//fade starts halfway through deadscreen fade in
	
	protected float								m_DeadScreenFadeInIncrement;
	protected float								m_DeadScreenFadeInLevel;
	protected float								m_DeadScreenImageFadeInIncrement;
	protected float								m_DeadScreenImageFadeInLevel;

	protected float 								m_Distance;
	protected float 								m_Playtime;
	protected float 								m_PlayersKilled;
	protected float 								m_InfectedKilled;
	protected float 								m_AnimalsKilled;
	protected float								m_LongestShot;
	
	protected TextWidget						m_DeadSourceVal;
	protected TextWidget						m_LongestShotVal;
	protected TextWidget						m_DistanceVal;
	protected TextWidget						m_AnimalsKilledVal;
	protected TextWidget						m_InfectedKilledVal;
	protected TextWidget						m_PlayersKilledVal;
	protected TextWidget						m_TimeSurvivedVal;
	protected ButtonWidget					m_DeadScreenStatsHideButton;
	protected TextWidget						m_DeadSceenStatsPanelTitle;
	protected Widget							m_DeadSceenStatsPanel;
	protected TextWidget						m_DeadSceenStatsButtonLable;
	protected ButtonWidget					m_DeadSceenStatsButton;
	protected Widget							m_DeadSceenStatsButtonPanel;
	protected ImageWidget 					m_DeadScreenImage;
	protected Widget 							m_DeadScreenRoot;
	protected Widget 							m_DeadScreen;

	protected float 								m_ShowAlpha;
	protected float 								m_TimerSlice;	
	protected bool 								m_WasGPSVisible;
	protected bool 								m_WasClockVisible;
	
	protected ImageWidget					m_Logo;
	protected WrapSpacerWidget			m_Licensing;
	
	protected Widget							m_NewsfeedPanel;
	protected ref ExpansionNewsfeed 	m_ExpansionNewsfeed;

	void InGameMenu()
	{
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitor;
		if ( CF_Modules<ExpansionMonitorModule>.Get(monitor))
			monitor.m_StatsInvoker.Insert(OnDataReceived);
#endif
	}

	void ~InGameMenu()
	{
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitor;
		if ( CF_Modules<ExpansionMonitorModule>.Get(monitor))
			monitor.m_StatsInvoker.Remove(OnDataReceived);
#endif
	}

	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "Init");
#endif

		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_ingamemenu.layout");
		m_RespawnButton 			= layoutRoot.FindAnyWidget( "respawn_button" );
		m_RestartDeadRandomButton	= layoutRoot.FindAnyWidget( "respawn_button_random" );
		m_RestartDeadCustomButton	= layoutRoot.FindAnyWidget( "respawn_button_custom" );
				
		m_ContinueButton	= layoutRoot.FindAnyWidget( "continuebtn" );
		m_ExitButton		= layoutRoot.FindAnyWidget( "exitbtn" );
		m_RestartButton		= layoutRoot.FindAnyWidget( "restartbtn" );
		m_OptionsButton		= layoutRoot.FindAnyWidget( "optionsbtn" );
		m_ModdedWarning		= TextWidget.Cast( layoutRoot.FindAnyWidget( "ModdedWarning" ) );
		m_HintPanel			= new UiHintPanel(layoutRoot.FindAnyWidget( "hint_frame" ));
		m_ServerInfoPanel 	= layoutRoot.FindAnyWidget( "server_info" );
		m_ServerIP 			= TextWidget.Cast( layoutRoot.FindAnyWidget( "server_ip" ) );
		m_ServerPort 		= TextWidget.Cast( layoutRoot.FindAnyWidget( "server_port" ) );
		m_ServerName 		= TextWidget.Cast( layoutRoot.FindAnyWidget( "server_name" ) );
		//m_FavoriteButton 	= layoutRoot.FindAnyWidget( "favorite_button" );
		m_FavoriteImage 	= layoutRoot.FindAnyWidget( "favorite_image" );
		m_UnfavoriteImage 	= layoutRoot.FindAnyWidget( "unfavorite_image" );
		m_CopyInfoButton 	= layoutRoot.FindAnyWidget( "copy_button" );
		
		if (GetGame().IsMultiplayer())
		{
			ButtonSetText(m_RestartButton, "#main_menu_respawn");
		}
		else
		{
			ButtonSetText(m_RestartButton, "#main_menu_restart");
		}	
		
	#ifdef BULDOZER		
		delete m_RestartButton;
	#endif
		
		//! Logo
		m_Logo = ImageWidget.Cast( layoutRoot.FindAnyWidget( "dayz_logo" ) );
		if ( m_Logo )
		{
			m_Logo.LoadImageFile( 0, "set:expansion_iconset image:logo_expansion_white" );
		}
		
		//! Licensing Panel
		m_Licensing = WrapSpacerWidget.Cast( layoutRoot.FindAnyWidget( "Licensing" ) );
		
		//! DeadScreen
		m_DeadScreenRoot					= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_dead_screen.layout", layoutRoot);
		
		m_DeadScreenRoot.SetAlpha( 0 );
		m_DeadScreenRoot.Show( false );
		
		m_DeadScreen						= Widget.Cast( m_DeadScreenRoot.FindAnyWidget( "dead_screen" ) );
		m_DeadScreen.SetAlpha(0);
		
		m_DeadScreenImage 					= ImageWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "dead_screen_image" ) );
		m_DeadScreenImage.SetAlpha(0);
		m_DeadScreenImage.LoadImageFile( 0, "DayZExpansion/GUI/textures/dead_screens/dead_screen.edds" );
		
		m_DeadSceenStatsButtonPanel			= Widget.Cast( m_DeadScreenRoot.FindAnyWidget( "ButtonPanel" ) );
		m_DeadSceenStatsButton				= ButtonWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "bStats" ) );
		m_DeadSceenStatsButtonLable			= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "bStatsLable" ) );	
		
		m_DeadSceenStatsPanel				= Widget.Cast( m_DeadScreenRoot.FindAnyWidget( "PlayerStatisticsPanel" ) );
		m_DeadSceenStatsPanelTitle			= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "Caption" ) );
		m_DeadSourceVal						= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "DeadSourceValue" ) );
		m_LongestShotVal					= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "LongRangeShotValue" ) );
		m_DistanceVal						= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "DistanceTraveledValue" ) );
		m_AnimalsKilledVal					= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "AnimalsKilledValue" ) );
		m_InfectedKilledVal					= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "InfectedKilledValue" ) );
		m_PlayersKilledVal					= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "PlayersKilledValue" ) );
		m_TimeSurvivedVal					= TextWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "TimeSurvivedValue" ) );
		m_DeadScreenStatsHideButton			= ButtonWidget.Cast( m_DeadScreenRoot.FindAnyWidget( "bHide" ) );
		
		m_DeadScreenFadeInIncrement	= 1 / DEAD_SCREEN_FADEIN_TIME;
		m_DeadScreenImageFadeInIncrement = 1 / DEAD_SCREEN_IMAGE_FADEIN_TIME;
		
		if ( !GetExpansionSettings().GetGeneral().UseDeathScreenStatistics || !GetValuesFromMonitor() )
			m_DeadSceenStatsButtonPanel.Show(false);
		
		HudShow( false );
		SetGameVersion();
		SetServerInfoVisibility( SetServerInfo() && g_Game.GetProfileOption( EDayZProfilesOptions.SERVERINFO_DISPLAY ) && !GetExpansionClientSettings().StreamerMode );
		m_ModdedWarning.Show( false );
		
		if (GetExpansionSettings().GetGeneral().UseNewsFeedInGameMenu)
		{
			//! Newsfeed
			m_NewsfeedPanel 	= Widget.Cast(layoutRoot.FindAnyWidget( "NewsFeedPanel" ));
			m_ExpansionNewsfeed = new ExpansionNewsfeed(m_NewsfeedPanel);
		}
		
		Refresh();

		return layoutRoot;
	}

	// ------------------------------------------------------------
	// Override Update
	// ------------------------------------------------------------
	override void Update( float timeslice )
	{		
		if ( GetExpansionSettings().GetGeneral().UseDeathScreen && g_ExpansionShowDeadScreen )
		{
			m_TimerSlice += timeslice;
			if (m_TimerSlice >= 0.01)
			{
				DeadScreenShow( timeslice );
				m_TimerSlice = 0;	
			}
		}
		
		super.Update( timeslice );
	}
	
	void OnDataReceived( ExpansionSyncedPlayerStats player_stats)
	{
		auto trace = EXTrace.Start(EXTrace.PLAYER_MONITOR, this, player_stats.m_PlainID);

		if (GetGame().GetPlayer().GetIdentity().GetPlainId() != player_stats.m_PlainID || !player_stats.m_HasRegisteredStats) return;

		if ( GetExpansionSettings().GetGeneral().UseDeathScreenStatistics && GetValuesFromMonitor() )
			m_DeadSceenStatsButtonPanel.Show(true);
	}

	// ------------------------------------------------------------
	// Expansion GetValuesFromMonitor
	// ------------------------------------------------------------
	bool GetValuesFromMonitor()
	{
/*
		m_Distance = GetGame().GetPlayer().StatGet(AnalyticsManagerServer.STAT_DISTANCE);
		m_Playtime = GetGame().GetPlayer().StatGet(AnalyticsManagerServer.STAT_PLAYTIME);
		m_PlayersKilled = GetGame().GetPlayer().StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED);
		m_InfectedKilled = GetGame().GetPlayer().StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED);
		m_AnimalsKilled = GetGame().GetPlayer().StatGet(AnalyticsManagerServer.EXP_STAT_ANIMALS_KILLED);
		m_LongestShot = GetGame().GetPlayer().StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT);
*/
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitor;
		if ( !CF_Modules<ExpansionMonitorModule>.Get(monitor))
			return false;
		
		ExpansionSyncedPlayerStats player_stats = monitor.GetClientStats();
		if ( !player_stats || !player_stats.m_HasRegisteredStats )
			return false;

		m_Distance = player_stats.m_Distance;
		m_Playtime = player_stats.m_Playtime;
		m_PlayersKilled = player_stats.m_PlayersKilled;
		m_InfectedKilled = player_stats.m_InfectedKilled;
		m_AnimalsKilled = player_stats.m_AnimalsKilled;
		m_LongestShot = player_stats.m_LongestShot;

		return true;
#endif

		return false;
	}

	// ------------------------------------------------------------
	// Expansion UpdatePlayerStatValues
	// ------------------------------------------------------------
	private void UpdatePlayerStatValues()
	{
		if ( GetGame().GetPlayer() && GetGame().GetPlayer().GetIdentity() )
		{
			string name = GetGame().GetPlayer().GetIdentity().GetName();
			StringLocaliser player_name = new StringLocaliser( "STR_EXPANSION_DEADSCREEN_STATS_TITLE", name );
			
			m_DeadSceenStatsPanelTitle.SetText( player_name.Format() );
			//m_DeadSourceVal.SetText( "" );
			m_LongestShotVal.SetText( ExpansionStatic.GetDistanceString( m_LongestShot ) );
			m_DistanceVal.SetText( ExpansionStatic.GetDistanceString( m_Distance ) );
			m_AnimalsKilledVal.SetText( ExpansionStatic.GetValueString( m_AnimalsKilled ) );
			m_InfectedKilledVal.SetText( ExpansionStatic.GetValueString( m_InfectedKilled ) );
			m_PlayersKilledVal.SetText( ExpansionStatic.GetValueString( m_PlayersKilled ) );
			m_TimeSurvivedVal.SetText( ExpansionStatic.GetTimeString( m_Playtime ) );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion DeadScreenShow
	// ------------------------------------------------------------
	void DeadScreenShow(float timeslice)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.UI, this, "DeadScreenShow").Add(timeslice);
#endif

		float new_deadscreen_val;
		if ( m_DeadScreenImageFadeInLevel == 1 ) return;

			m_DeadScreenRoot.Show( true );
			
			new_deadscreen_val = m_DeadScreenFadeInLevel + m_DeadScreenFadeInIncrement * timeslice;
			if ( new_deadscreen_val < 1 )
				m_DeadScreenFadeInLevel = new_deadscreen_val;
			else
				m_DeadScreenFadeInLevel = 1;
			
			if ( m_DeadScreenFadeInLevel > 0.5 )
			{
				float new_logo_val = m_DeadScreenImageFadeInLevel + m_DeadScreenImageFadeInIncrement * timeslice;
				if ( new_deadscreen_val < 1 )
					m_DeadScreenImageFadeInLevel = new_logo_val;
				else
					m_DeadScreenImageFadeInLevel = 1;
			}
			
			
			m_DeadScreenRoot.SetAlpha( m_DeadScreenFadeInLevel );
			m_DeadScreen.SetAlpha( m_DeadScreenFadeInLevel );
			m_DeadScreenImage.SetAlpha( m_DeadScreenImageFadeInLevel );
			
			m_Logo.SetAlpha(-m_DeadScreenFadeInLevel);
			m_Licensing.SetAlpha(-m_DeadScreenFadeInLevel);
			
			if(m_ExpansionNewsfeed)
				m_ExpansionNewsfeed.ShowNewsfeed(false);
	}
	
	// ------------------------------------------------------------
	// Expansion OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_DeadSceenStatsButton )
		{
			UpdatePlayerStatValues();
			ShowPlayerStats();
		} else if ( w == m_DeadScreenStatsHideButton )
		{
			HidePlayerStats();
		}

		return false;
	}
		
	// ------------------------------------------------------------
	// Expansion ShowPlayerStats
	// ------------------------------------------------------------
	private void ShowPlayerStats()
	{
		m_DeadSceenStatsPanel.Show( true );
		m_DeadSceenStatsButtonPanel.Show( false );
	}
	
	// ------------------------------------------------------------
	// Expansion HidePlayerStats
	// ------------------------------------------------------------
	private void HidePlayerStats()
	{
		m_DeadSceenStatsPanel.Show( false );
		m_DeadSceenStatsButtonPanel.Show( true );
	}
};
#endif
