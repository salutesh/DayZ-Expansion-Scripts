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
	protected ref MainMenuExpansionNewsfeed		m_ExpansionNewsfeed;
	protected ImageWidget 						m_Logo;
	protected ImageWidget 						m_SettingsIcon;
	protected ImageWidget 						m_ExitIcon;
	protected ImageWidget 						m_TutorialIcon;
	protected Widget 							m_ExpansionLicenceRoot;
	protected Widget							m_LicencePanel;
	protected TextWidget						m_LicenceTitle;
	protected RichTextWidget   	 				m_LicenceText;
	protected ButtonWidget						m_AcceptButton;
	protected ButtonWidget						m_DisagreeButton;
	protected ButtonWidget						m_ViewLicenceButton;
	protected Widget							m_DialogPanel;
	protected TextWidget						m_DialogTitle;
	protected RichTextWidget					m_DialogText;
	protected ButtonWidget						m_DialogAcceptButton;
	protected bool								m_isLicenceVisible;
	protected GridSpacerWidget					m_NewsFeedRoot;
	protected Widget							m_ExpansionNewsFeedRoot;
	
	// ------------------------------------------------------------
	// MainMenu Constructor
	// ------------------------------------------------------------
	void MainMenu()
	{
	}

	// ------------------------------------------------------------
	// Override Widget Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Init - Start");
		#endif
		super.Init();
		
		m_Logo 						= ImageWidget.Cast( layoutRoot.FindAnyWidget( "dayz_logo" ) );
		m_SettingsIcon				= ImageWidget.Cast( layoutRoot.FindAnyWidget( "settings_button_image" ) );
		m_ExitIcon					= ImageWidget.Cast( layoutRoot.FindAnyWidget( "exit_button_image" ) );
		m_TutorialIcon				= ImageWidget.Cast( layoutRoot.FindAnyWidget( "tutorial_button_image" ) );
		
		m_Logo.LoadImageFile( 0, "set:expansion_iconset image:logo_expansion_white" );
		m_SettingsIcon.LoadImageFile( 0, "set:expansion_iconset image:icon_settings" );
		m_ExitIcon.LoadImageFile( 0, "set:expansion_iconset image:icon_exit" );
		m_TutorialIcon.LoadImageFile( 0, "set:expansion_notification_iconset image:icon_info" );
		
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
		
		//! Licence
		m_ExpansionLicenceRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_licence_dialog.layout", layoutRoot );
		m_ExpansionLicenceRoot.Show(false);
		
		if ( m_ExpansionLicenceRoot )
		{
			m_LicencePanel = Widget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "ExpansionLicenceDialog" ) );
			m_LicenceTitle = TextWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "LicenceDialogTitle" ) );
			m_LicenceText = RichTextWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "LicenceDialogText" ) );
			m_AcceptButton = ButtonWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "LicenceDialogAgreeButton" ) );
			m_DisagreeButton = ButtonWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "LicenceDialogDisagreeButton" ) );
			m_ViewLicenceButton	= ButtonWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "LicenceDialogLicenseButton" ) );
			
			m_DialogPanel = Widget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "ExpansionDialog" ) );
			m_DialogTitle = TextWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "DialogTitle" ) );
			m_DialogText = RichTextWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "DialogText" ) );
			m_DialogAcceptButton = ButtonWidget.Cast( m_ExpansionLicenceRoot.FindAnyWidget( "DialogAgreeButton" ) );
		}
		
		m_NewsFeedRoot = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "news_feed_root" ) );
		float feed_x = 0;
		float feed_y = 0;
		m_NewsFeedRoot.GetPos(feed_x, feed_y);
		
		//! Add Expansion newsfeed
		//m_ExpansionNewsFeedRoot	= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/newsfeed.layout", layoutRoot );
		//m_ExpansionNewsFeedRoot.SetPos(feed_x, feed_y);
		//m_ExpansionNewsfeed	= new MainMenuExpansionNewsfeed( m_ExpansionNewsFeedRoot.FindAnyWidget( "news_feed_root" ) );
		
		Refresh();
		
		GetDayZGame().GetBacklit().MainMenu_OnShow();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Init - End");
		#endif
		
		return layoutRoot;
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
	// Expansion CheckLicenze
	// ------------------------------------------------------------
	void CheckLicenze()
	{
		string hasSeen;
		if ( GetGame().GetProfileString( "ExpansionLicence", hasSeen ) )
		{
			if ( hasSeen == "" || hasSeen == "false" )
			{
				ShowLicence();
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion ShowLicence
	// ------------------------------------------------------------
	void ShowLicence()
	{
		m_ExpansionLicenceRoot.Show( true );
		
		string title = "DayZ Expansion - End User License Agreement";
		string text = "\nThis License Agreement (\"Agreement\") is made between you (the \"Licensee\") and DayZ Expansion (the \"Licensor\"). The terms of this Agreement apply to all current and future versions and updates of the DayZ Expansion Project (\"DayZ Expansion\").\n \n \nBy agreeing to this license, you agree to any and all terms listed below. The licensor reserves all rights not specifically granted and transferred to Licensee.\n \n \nLicensee understands, acknowledges and agrees with the following:\n \n- Licensor provides DayZ Expansion on an \"as is\" basis without warranty of any kind. Licensor neither guarantees the correct, error-free functioning of DayZ Expansion nor is Licensor responsible for any damage caused by the use of DayZ Expansion.\n \n- Licensee may not decompile, disassemble, reverse-engineer, modify or redistribute DayZ Expansion in any way. Doing this will result in a legal matter.\n \n- DayZ Expansion will automatically, without notice to Licensee, download and install updates from time to time.\n \n- DayZ Expansion may further report and store Licensee\'s game account name and identifier, in-game nickname, and system-related and hardware-related information including, but not limited to, device identifiers and hardware serial numbers.\n \n- Licensor values Licensee\'s privacy and does its utmost to protect it at all times. DayZ Expansion does not report any personally identifiable information or personal data except for any information/data specifically mentioned herein.\n \n- Licensor stores all information collected by DayZ Expansion on servers located in Europe. Licensor may share the information with its partners and/or affiliates.\n \n- Licensee acknowledges that the invasive nature of DayZ Expansion is necessary to meet its purpose and goal of preventing and detecting license breaches.\n \n- Licensor is allowed to terminate the license at any time for any reason and without notice to Licensee.\n \n \nThis License Agreement constitutes the entire agreement between Licensor and Licensee and supersedes any prior statements.";
	
		m_LicenceTitle.SetText(title);
		m_LicenceText.SetText(text);
	}
	
	// ------------------------------------------------------------
	// Expansion CloseLicence
	// ------------------------------------------------------------
	void CloseLicence(bool accepted)
	{			
		GetGame().SetProfileString( "ExpansionLicence", accepted.ToString() );
			
		if ( accepted )
		{
			m_ExpansionLicenceRoot.Show( false );
			m_ExpansionLicenceRoot.Unlink();
		} 
		else if ( !accepted )
		{
			m_LicencePanel.Show( false );
			m_DialogPanel.Show( true );
			OnDisagreeDialogShow();
		}
	}
		
	// ------------------------------------------------------------
	// Expansion Accept
	// ------------------------------------------------------------
	void Accept()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Accept - Start");
		#endif
		
		CloseLicence(true);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Accept - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Disagree
	// ------------------------------------------------------------
	void Disagree()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Disagree - Start");
		#endif
		
		CloseLicence(false);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::Disagree - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnDisagreeDialogShow
	// ------------------------------------------------------------
	void OnDisagreeDialogShow()
	{
		string title = "Programm Termination";
		string text = "The End User Agreemend has been declined. The programm will be terminated now..";
	
		m_DialogTitle.SetText(title);
		m_DialogText.SetText(text);
	}
	
	// ------------------------------------------------------------
	// Expansion Terminate
	// ------------------------------------------------------------
	void Terminate()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
	}
	
	// ------------------------------------------------------------
	// Expansion ViewLicence
	// ------------------------------------------------------------
	void ViewLicence()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::ViewLicence - Start");
		#endif
		
		GetGame().OpenURL( "https://www.dayzexpansion.com/" );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::ViewLicence - End");
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
		//ShowNewsfeed();
		HideModWarning();
		CheckLicenze();

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
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::OnClick - Start");
		#endif
		
		if( button == MouseState.LEFT )
		{
			if( w == m_AcceptButton )
			{
				m_LastFocusedButton = m_AcceptButton;
				Accept();
				return true;
			}
			else if ( w == m_DisagreeButton )
			{
				m_LastFocusedButton = m_DisagreeButton;
				Disagree();
				return true;
			}
			else if ( w == m_ViewLicenceButton )
			{
				m_LastFocusedButton = m_ViewLicenceButton;
				ViewLicence();
				return true;
			}
			else if (w == m_DialogAcceptButton)
			{
				Terminate();
				return true;
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MainMenu::OnClick - End");
		#endif
		return super.OnClick(w, x, y, button);;
	}
	
	// ------------------------------------------------------------
	// Override ShowNewsfeed
	// ------------------------------------------------------------
	override void ShowNewsfeed()
	{
		super.ShowNewsfeed();
		
		if (m_ExpansionNewsfeed)
			m_ExpansionNewsfeed.ShowNewsfeed();
	}
	
	// ------------------------------------------------------------
	// Override HideNewsfeed
	// ------------------------------------------------------------
	override void HideNewsfeed()
	{
		super.HideNewsfeed();

		if (m_ExpansionNewsfeed)
			m_ExpansionNewsfeed.HideNewsfeed();
	}
}