/**
 * InGameMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class InGameMenu
{
	protected static float 		DEAD_SCREEN_FADEIN_TIME = 3.0;					//fade starts as soon as deadscreen gets diplayed
	protected static float 		DEAD_SCREEN_IMAGE_FADEIN_TIME = 1.5;			//fade starts halfway through deadscreen fade in
	
	protected float				m_DeadScreenFadeInIncrement;
	protected float				m_DeadScreenFadeInLevel;
	protected float				m_DeadScreenImageFadeInIncrement;
	protected float				m_DeadScreenImageFadeInLevel;

	protected float 			m_Distance;
	protected float 			m_Playtime;
	protected float 			m_PlayersKilled;
	protected float 			m_InfectedKilled;
	protected float 			m_AnimalsKilled;
	protected float				m_LongestShot;
	
	protected TextWidget		m_DeadSourceVal;
	protected TextWidget		m_LongestShotVal;
	protected TextWidget		m_DistanceVal;
	protected TextWidget		m_AnimalsKilledVal;
	protected TextWidget		m_InfectedKilledVal;
	protected TextWidget		m_PlayersKilledVal;
	protected TextWidget		m_TimeSurvivedVal;
	protected ButtonWidget		m_DeadScreenStatsHideButton;
	protected TextWidget		m_DeadSceenStatsPanelTitle;
	protected Widget			m_DeadSceenStatsPanel;
	protected TextWidget		m_DeadSceenStatsButtonLable;
	protected ButtonWidget		m_DeadSceenStatsButton;
	protected Widget			m_DeadSceenStatsButtonPanel;
	protected ImageWidget 		m_DeadScreenImage;
	protected Widget 			m_DeadScreenRoot;
	protected Widget 			m_DeadScreen;

	protected float 			m_ShowAlpha;
	protected float 			m_TimerSlice;	
	protected bool 				m_WasGPSVisible;
	protected bool 				m_WasClockVisible;
	
	protected ImageWidget		m_Logo;
	protected WrapSpacerWidget	m_Licensing;
	
	protected Widget			m_NewsfeedPanel;
	protected ref ExpansionNewsfeed m_ExpansionNewsfeed;
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::Init - Start");
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
		
		if ( !GetExpansionSettings().GetGeneral().UseDeathScreenStatistics )
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::Init - End");
		#endif

		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Override SetGameVersion
	// ------------------------------------------------------------
	override protected void SetGameVersion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::SetGameVersion - Start");
		#endif
		TextWidget version_widget = TextWidget.Cast( layoutRoot.FindAnyWidget("version") );
		string version;
		GetGame().GetVersion( version );
		string expansion_version;	
		
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			version_widget.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		}
		else
		{
			version_widget.SetText( "DayZ SA #main_menu_version" + " " + version );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::SetGameVersion - End");
		#endif
	}

	// ------------------------------------------------------------
	// Override Update
	// ------------------------------------------------------------
	override void Update( float timeslice )
	{		
		if ( GetExpansionSettings().GetGeneral().UseDeathScreen && m_ExpansionShowDeadScreen )
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
	
	// ------------------------------------------------------------
	// Expansion GetValuesFromMonitor
	// ------------------------------------------------------------
	void GetValuesFromMonitor()
	{
		ExpansionMonitorModule monitor;
		if ( !Class.CastTo( monitor, GetModuleManager().GetModule( ExpansionMonitorModule ) ) )
			return;
		
		ExpansionSyncedPlayerStats player_stats = monitor.GetStats();
		if ( !player_stats )
			return;

		m_Distance = player_stats.m_Distance;
		m_Playtime = player_stats.m_Playtime;
		m_PlayersKilled = player_stats.m_PlayersKilled;
		m_InfectedKilled = player_stats.m_InfectedKilled;
		m_AnimalsKilled = player_stats.m_AnimalsKilled;
		m_LongestShot = player_stats.m_LongestShot;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTimeString
	// ------------------------------------------------------------
	private string GetTimeString( float total_time )
	{
		string time_string;

		if( total_time < 0 )
		{
			time_string =  "0 " + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_HOURS";
			return time_string;
		}
	
		int time_seconds = total_time; 									//convert total time to int
		
		int hours = time_seconds / 3600;
		if ( hours > 0 )
		{
			time_string += GetValueString( hours ) + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_HOURS";			//hours
		}
		
		time_string += " ";												//separator
		
		int minutes = ( time_seconds % 3600 ) / 60;
		time_string += GetValueString( minutes ) + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_MINUTES";			//minutes
		
		return time_string;
	}
	
	// ------------------------------------------------------------
	// Expansion GetDistanceString
	// ------------------------------------------------------------
	private string GetDistanceString( float total_distance, bool meters_only = false )
	{
		string distance_string;
		
		if( total_distance < 0 )
		{
			distance_string =  "0" + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_METERS";
			return distance_string;
		}
	
		int distance_meters = total_distance;
		
		int kilometers = distance_meters / 1000;
		if ( kilometers > 0 && !meters_only )
		{
			distance_string += GetValueString( kilometers ) + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_KM";				//kilometers
			distance_string += " ";												//separator
		}
		else
		{
			distance_string += GetValueString( distance_meters ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_METERS";	//meters
		}
		
		return distance_string;
	}
	
	// ------------------------------------------------------------
	// Expansion GetValueString
	// ------------------------------------------------------------
	private string GetValueString( float total_value )
	{
		if( total_value < 0 )
			return "0";
	
		int value = total_value;
		string out_string;
		
		if ( value >= 1000 )
		{
			string value_string = value.ToString();
			
			int count;		
			int first_length = value_string.Length() % 3;		//calculate position of the first separator
			if ( first_length > 0 )
			{
				count = 3 - first_length;
			}
			
			for ( int i = 0; i < value_string.Length(); ++i )
			{
				out_string += value_string.Get( i );
				count ++;
				
				if ( count >= 3 )
				{
					out_string += " ";			//separator
					count = 0;
				}
			}
		}
		else
		{
			out_string = value.ToString();
		}
		
		return out_string;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePlayerStatValues
	// ------------------------------------------------------------
	private void UpdatePlayerStatValues()
	{
		GetValuesFromMonitor();

		if ( GetGame().GetPlayer() && GetGame().GetPlayer().GetIdentity() )
		{
			string name = GetGame().GetPlayer().GetIdentity().GetName();
			ref StringLocaliser player_name = new StringLocaliser( "STR_EXPANSION_DEADSCREEN_STATS_TITLE", name );
			
			m_DeadSceenStatsPanelTitle.SetText( player_name.Format() );
			//m_DeadSourceVal.SetText( "" );
			m_LongestShotVal.SetText( GetDistanceString( m_LongestShot ) );
			m_DistanceVal.SetText( GetDistanceString( m_Distance ) );
			m_AnimalsKilledVal.SetText( GetValueString( m_AnimalsKilled ) );
			m_InfectedKilledVal.SetText( GetValueString( m_InfectedKilled ) );
			m_PlayersKilledVal.SetText( GetValueString( m_PlayersKilled ) );
			m_TimeSurvivedVal.SetText( GetTimeString( m_Playtime ) );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion DeadScreenShow
	// ------------------------------------------------------------
	void DeadScreenShow(float timeslice)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::DeadScreenShow - Start");
		#endif
		float new_deadscreen_val;
		if ( m_DeadScreenImageFadeInLevel != 1 )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("InGameMenu::DeadScreenShow - 1");
			#endif

			m_DeadScreenRoot.Show( true );
			
			#ifdef EXPANSIONEXPRINT
			EXPrint("InGameMenu::DeadScreenShow - 2");
			#endif
			
			new_deadscreen_val = m_DeadScreenFadeInLevel + m_DeadScreenFadeInIncrement * timeslice;
			if ( new_deadscreen_val < 1 )
				m_DeadScreenFadeInLevel = new_deadscreen_val;
			else
				m_DeadScreenFadeInLevel = 1;
			
			#ifdef EXPANSIONEXPRINT
			EXPrint("InGameMenu::DeadScreenShow - 3");
			#endif
			
			if ( m_DeadScreenFadeInLevel > 0.5 )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("InGameMenu::DeadScreenShow - 4");
				#endif
				float new_logo_val = m_DeadScreenImageFadeInLevel + m_DeadScreenImageFadeInIncrement * timeslice;
				if ( new_deadscreen_val < 1 )
					m_DeadScreenImageFadeInLevel = new_logo_val;
				else
					m_DeadScreenImageFadeInLevel = 1;
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("InGameMenu::DeadScreenShow - 5");
				#endif
			}
			
			#ifdef EXPANSIONEXPRINT
			EXPrint("InGameMenu::DeadScreenShow - 5");
			#endif
			
			m_DeadScreenRoot.SetAlpha( m_DeadScreenFadeInLevel );
			m_DeadScreen.SetAlpha( m_DeadScreenFadeInLevel );
			m_DeadScreenImage.SetAlpha( m_DeadScreenImageFadeInLevel );
			
			m_Logo.SetAlpha(-m_DeadScreenFadeInLevel);
			m_Licensing.SetAlpha(-m_DeadScreenFadeInLevel);
			
			if(m_ExpansionNewsfeed)
				m_ExpansionNewsfeed.ShowNewsfeed(false);
			
			#ifdef EXPANSIONEXPRINT
			EXPrint("InGameMenu::DeadScreenShow - 6");
			#endif
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::DeadScreenShow - End");
		#endif
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
}