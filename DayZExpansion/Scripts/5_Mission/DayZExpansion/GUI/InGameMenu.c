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
	protected ImageWidget 		m_DeadScreenImage;
	protected Widget 			m_DeadScreenRoot;
	protected Widget 			m_DeadScreen;
	protected float 			m_ShowAlpha;
	protected float 			m_TimerSlice;	
	protected bool 				m_WasGPSVisible;
	protected bool 				m_WasClockVisible;
	protected ImageWidget		m_Logo;
	protected WrapSpacerWidget	m_Licensing;
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::Init - Start");
		#endif
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_ingamemenu.layout");
				
		m_ContinueButton	= layoutRoot.FindAnyWidget( "continuebtn" );
		m_ExitButton		= layoutRoot.FindAnyWidget( "exitbtn" );
		m_RestartButton		= layoutRoot.FindAnyWidget( "restartbtn" );
		m_RestartDeadButton	= layoutRoot.FindAnyWidget( "restartdeadbtn" );
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
		
		m_DeadScreenFadeInIncrement	= 1 / DEAD_SCREEN_FADEIN_TIME;
		m_DeadScreenImageFadeInIncrement = 1 / DEAD_SCREEN_IMAGE_FADEIN_TIME;
		
		HudShow( false );
		SetGameVersion();
		SetServerInfoVisibility(SetServerInfo() && g_Game.GetProfileOption( EDayZProfilesOptions.SERVERINFO_DISPLAY ) && !GetExpansionClientSettings().StreamerMode );
		m_ModdedWarning.Show( g_Game.ReportModded() );
		
		Refresh();
		
		GetExpansionClientSettings().Load();
		PPEffects.UpdateSaturation();
		PPEffects.UpdateVignette();

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
		if ( m_ExpansionShowDeadScreen )
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
			m_Logo.Show( false );
			m_Licensing.Show( false );
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
			
			#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::DeadScreenShow - 6");
		#endif
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("InGameMenu::DeadScreenShow - End");
		#endif
	}
}