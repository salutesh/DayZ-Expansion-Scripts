/**
 * ExpansionLoadingScreen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class LoadingScreen
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;

	protected RichTextWidget m_LoadingMessage;
	protected RichTextWidget m_LoadingMessageAuthor;
	
	protected autoptr ExpansionLoadingScreenMessageData m_MessageRest;

	protected autoptr array< ref ExpansionLoadingScreenMessageData > m_MessageJson;

	protected autoptr array< ref ExpansionLoadingScreenBackground > m_Backgrounds;
	
	protected Widget m_ModOutdatedPanel;
	protected ImageWidget m_ModOutdatedImage;
	protected TextWidget m_ModOutdatedText;

	protected bool m_ExpansionVersionChecked;
	protected bool m_IsExpansionOutdated;
	
	string m_ExpansionClientVersion;
	string m_ExpansionCurrentVersion;
	
	protected RestApi m_ExpansionRestApi;

	// ------------------------------------------------------------
	// LoadingScreen Constructor
	// ------------------------------------------------------------
	void LoadingScreen(DayZGame game)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoadingScreen::LoadingScreen - Start");
		#endif

		JsonFileLoader< ref array< ref ExpansionLoadingScreenBackground > >.JsonLoadFile( "DayZExpansion/Scripts/Data/LoadingImages.json", m_Backgrounds );
		JsonFileLoader< ref array< ref ExpansionLoadingScreenMessageData > >.JsonLoadFile( "DayZExpansion/Scripts/Data/LoadingMessages.json", m_MessageJson );

		if (m_WidgetRoot)
		{
			m_WidgetRoot.Unlink();
		}

		m_DayZGame = game;

		m_WidgetRoot = game.GetLoadingWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/expansion_loading.layout");

		Class.CastTo(m_ImageLogoMid, m_WidgetRoot.FindAnyWidget("ImageLogoMid"));
		Class.CastTo(m_ImageLogoCorner, m_WidgetRoot.FindAnyWidget("ImageLogoCorner"));

		Class.CastTo(m_TextWidgetTitle, m_WidgetRoot.FindAnyWidget("TextWidget"));
		Class.CastTo(m_ImageWidgetBackground, m_WidgetRoot.FindAnyWidget("ImageBackground"));
		Class.CastTo(m_ImageLoadingIcon, m_WidgetRoot.FindAnyWidget("ImageLoadingIcon"));
		Class.CastTo(m_ModdedWarning, m_WidgetRoot.FindAnyWidget("ModdedWarning"));
		Class.CastTo(m_LoadingMessage, m_WidgetRoot.FindAnyWidget("LoadingMessage"));
		Class.CastTo(m_LoadingMessageAuthor, m_WidgetRoot.FindAnyWidget("LoadingAuthor"));

		m_ImageBackground = ImageWidget.Cast( m_WidgetRoot.FindAnyWidget("ImageBackground") );
		m_ProgressLoading = ProgressBarWidget.Cast( m_WidgetRoot.FindAnyWidget("LoadingBar") );

		m_ImageLogoMid.LoadImageFile( 0, "set:expansion_iconset image:logo_expansion_white" );
		m_ImageLogoCorner.LoadImageFile( 0, "set:expansion_iconset image:logo_expansion_white" );

		m_ModOutdatedPanel = Widget.Cast( m_WidgetRoot.FindAnyWidget("ModVersionPanel") );
		Class.CastTo(m_ModOutdatedImage, m_WidgetRoot.FindAnyWidget("ModOutdatedImage"));
		Class.CastTo(m_ModOutdatedText, m_WidgetRoot.FindAnyWidget("ModOutdatedText"));

		string tmp;
		m_ProgressText = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("ProgressText"));
		
		if (GetGame())
		{
			m_ProgressText.Show( GetGame().CommandlineGetParam("loadingTest", tmp) );
		}

		// Show/Hide modded game warning
		m_ModdedWarning.Show( g_Game.ReportModded() );

		// Show logo image
		m_ImageLogoMid.Show(true);
		m_ImageLogoCorner.Show(false);

		// Show background image
		m_ImageWidgetBackground.Show( true );
		m_Counter = 0;

		// Lighten up your desktop
		game.GetBacklit().LoadingAnim();

		// Set loadingbar progress
		if ( m_ProgressLoading )
			ProgressAsync.SetProgressData(m_ProgressLoading);
		if ( m_ImageBackground )
			ProgressAsync.SetUserData(m_ImageBackground);

		#ifdef EXPANSIONEXPRINT
		EXPrint("LoadingScreen::LoadingScreen - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// LoadingScreen Destructor
	// ------------------------------------------------------------
	void ~LoadingScreen()
	{
		if(m_ExpansionRestApi)
			DestroyRestApi();
	}

	// ------------------------------------------------------------
	// Override Show
	// ------------------------------------------------------------
	override void Show()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoadingScreen::Show - Start");
		#endif
		
		m_ProgressText.SetText("");
		m_ProgressLoading.SetCurrent( 0.0 );
		//m_ImageBackground.SetMaskProgress( 0.0 );
		
		if ( !m_WidgetRoot.IsVisible() )
		{
			if ( m_DayZGame.GetUIManager().IsDialogVisible() )
			{
				m_DayZGame.GetUIManager().HideDialog();
			}
			
			if ( m_DayZGame.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU )
			{
				m_ImageLogoMid.Show(false);
				m_ImageLogoCorner.Show(false);				
				m_ImageWidgetBackground.Show( true );		
				m_ModdedWarning.Show(false);
			} else
			{
				m_ImageLogoMid.Show(true);
				m_ImageLogoCorner.Show(false);
				m_ImageWidgetBackground.Show( true );
			}
			
			m_WidgetRoot.Show( true );
			m_TextWidgetTitle.SetText( "" );
		}

		if ( m_ProgressLoading )
			ProgressAsync.SetProgressData( m_ProgressLoading );
		if ( m_ImageBackground )
			ProgressAsync.SetUserData( m_ImageBackground );
		
		if ( m_MessageJson )
		{
			m_MessageRest = m_MessageJson.GetRandomElement();

			m_LoadingMessage.SetText( m_MessageRest.message );
			m_LoadingMessageAuthor.SetText( "#STR_EXPANSION_LOADING_MSG_SUBMITTEDBY" + " " + m_MessageRest.submitter );
		}
		else
		{
			m_LoadingMessage.Show(false);
			m_LoadingMessageAuthor.Show(false);
		}

		string world_name = "default";

		if ( GetGame() )
			GetGame().GetWorldName(world_name);

		world_name.ToLower();
		
		if ( world_name.IndexOf("gloom") == world_name.Length() - 5 )
		{
			world_name = world_name.Substring(0, world_name.Length() - 5);
		}
		
		ExpansionLoadingScreenBackground backgrounds = m_Backgrounds[0];

		for ( int i = 0; i < m_Backgrounds.Count(); ++i )
		{
			if ( world_name == m_Backgrounds[i].MapName )
			{
				backgrounds = m_Backgrounds[i];
				break;
			}
		}
		
		if (backgrounds)
			m_ImageBackground.LoadImageFile( 0, backgrounds.Path.GetRandomElement() );

		#ifdef EXPANSIONEXPRINT
		EXPrint("LoadingScreen::Show - End");
		#endif
	}
};