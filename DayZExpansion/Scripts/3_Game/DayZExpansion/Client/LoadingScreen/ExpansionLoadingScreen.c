/**
 * ExpansionLoadingScreen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class LoadingScreen
{
	protected RichTextWidget m_LoadingMessage;
	protected RichTextWidget m_LoadingMessageAuthor;
	
	protected autoptr ExpansionLoadingScreenMessageData m_MessageRest;

	protected autoptr array< ref ExpansionLoadingScreenMessageData > m_MessageJson;
	protected autoptr ExpansionArray< ref ExpansionLoadingScreenMessageData > m_Expansion_MessageJson;

	protected autoptr array< ref ExpansionLoadingScreenBackground > m_Backgrounds;

	protected RestApi m_ExpansionRestApi;

	static float s_Expansion_LoadingTime = -1;
	static float s_Expansion_LoadingTimeStamp = -1;

	bool m_UseExpansionLoadingScreen;

	// ------------------------------------------------------------
	// LoadingScreen Constructor
	// ------------------------------------------------------------
	void LoadingScreen(DayZGame game)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "LoadingScreen");
#endif

		m_Backgrounds = ExpansionLoadingScreenBackgrounds.Get();
		JsonFileLoader< array< ref ExpansionLoadingScreenMessageData > >.JsonLoadFile( "DayZExpansion/Scripts/Data/LoadingMessages.json", m_MessageJson );

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

		string tmp;
		m_ProgressText = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("ProgressText"));
		
		//! Game object does not exist yet during parts of the loading process
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "Show");
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
		
		//EXPrint(ToString() + "::Show");
		m_UseExpansionLoadingScreen = true;
		UpdateLoadingBackground();
	}

	void UpdateLoadingBackground()
	{
		float loadingTime = s_Expansion_LoadingTime;
		float tickTime = m_DayZGame.GetTickTime();

		if (s_Expansion_LoadingTimeStamp < 0)
			s_Expansion_LoadingTime = 0;
		else
			s_Expansion_LoadingTime += tickTime - s_Expansion_LoadingTimeStamp;

		s_Expansion_LoadingTimeStamp = tickTime;

		//! Show each loading message and screen at least five seconds
		if (loadingTime > -1 && s_Expansion_LoadingTime < 5)
			return;

		//EXPrint(ToString() + "::UpdateLoadingBackground - " + s_Expansion_LoadingTime);
		s_Expansion_LoadingTime = 0;

		if ( m_MessageJson )
		{
			if ( !m_Expansion_MessageJson )
				m_Expansion_MessageJson = new ExpansionArray< ref ExpansionLoadingScreenMessageData >;

			if ( !m_Expansion_MessageJson.Count() )
			{
				foreach ( ExpansionLoadingScreenMessageData data: m_MessageJson )
				{
					m_Expansion_MessageJson.Insert(data);
				}
			}

			m_MessageRest = m_Expansion_MessageJson.GetQuasiRandomElementAvoidRepetition();

			m_LoadingMessage.SetText( m_MessageRest.message );
			m_LoadingMessageAuthor.SetText( "#STR_EXPANSION_LOADING_MSG_SUBMITTEDBY" + " " + m_MessageRest.submitter );
		}
		else
		{
			m_LoadingMessage.Show(false);
			m_LoadingMessageAuthor.Show(false);
		}
		
		string world_name = ExpansionStatic.GetCanonicalWorldName(m_DayZGame);
		
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
			m_ImageBackground.LoadImageFile( 0, backgrounds.GetRandomPath() );
	}

	override void OnUpdate( float timeslice )
	{
		super.OnUpdate(timeslice);

		if (m_UseExpansionLoadingScreen && IsLoading())
			UpdateLoadingBackground();
	}
};
