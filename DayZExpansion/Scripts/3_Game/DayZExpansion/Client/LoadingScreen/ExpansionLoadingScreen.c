/**
 * ExpansionLoadingScreen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

	protected autoptr ExpansionLoadingScreenMessage m_Message;
	
	protected autoptr ExpansionLoadingScreenMessageCurl m_MessageRest;

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

		// Expansion Mod version check
		m_ExpansionVersionChecked = false;
		
		m_ExpansionRestApi = CreateRestApi();
		if ( m_ExpansionRestApi )
		{
			RestContext ctx = m_ExpansionRestApi.GetRestContext( EXPANSION_Rest_URL );

			string data = ctx.GET_now( "loadingtext" );
			string error;
			
			DestroyRestApi();

			if ( !m_Serializer.ReadFromString( m_MessageRest, data, error ) )
			{
				m_LoadingMessage.Show(false);
				m_LoadingMessageAuthor.Show(false);
				Error( error );
			}
		}
		else
		{
			m_LoadingMessage.Show(false);
			m_LoadingMessageAuthor.Show(false);
		}

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
			}
			else
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
		
		if ( m_MessageRest )
		{
			m_LoadingMessage.SetText( m_MessageRest.message );
			m_LoadingMessageAuthor.SetText( "Submitted by " + m_MessageRest.submitter );
		} 
		else
		{
			m_LoadingMessage.Show(false);
			m_LoadingMessageAuthor.Show(false);
		}
		
		// Set random loading screen image
		m_ImageBackground.LoadImageFile( 0, m_Backgrounds.GetRandomElement().Path );
		
		//! Version check
		if ( GetDayZGame() )
		{
			m_ExpansionRestApi = CreateRestApi();
			if ( m_ExpansionRestApi )
			{
				RestContext ctx = m_ExpansionRestApi.GetRestContext( EXPANSION_Rest_URL );
				string data_version = ctx.GET_now( "version" );
			}
			else
			{
				data_version = GetClientExpansionVersion();
			}
			
			m_ExpansionClientVersion = GetClientExpansionVersion();
			m_ExpansionCurrentVersion = data_version;
			
			GetDayZGame().SetExpansionClientVersion(m_ExpansionClientVersion);
			GetDayZGame().SetExpansionLatestVersion(m_ExpansionCurrentVersion);
			
			if (m_ExpansionCurrentVersion)
				m_ExpansionCurrentVersion.Replace("\"","");
			
			//! If Rest API times-out and client dont get the current mod version.
			if ( m_ExpansionCurrentVersion == "App Error" )
			{
				m_ExpansionVersionChecked = true;
			}
			
			string client_version_format = m_ExpansionClientVersion;
			string current_version_format = m_ExpansionCurrentVersion;
			int client_version = client_version_format.Replace(".", "");
			int current_version = current_version_format.Replace(".", "");
			
			//! If Rest API current version is older then loaded client version for some reason
			if ( client_version >= current_version )
			{
				m_ExpansionVersionChecked = true;
			}
			
			if ( m_ExpansionCurrentVersion == "App Error" )
			{
				m_ExpansionVersionChecked = true;
			}
			
			if ( !m_ExpansionVersionChecked )
			{
				m_ExpansionCurrentVersion.Replace("\"","");
				
				//! The version should always display in the scripts.log.
				Print("[Expansion Mod] Client Mod Version from Mod Config: " + m_ExpansionClientVersion);

				Print("[Expansion Mod] Current Mod Version from Rest API: " + m_ExpansionCurrentVersion);
				
				if ( m_ExpansionClientVersion != m_ExpansionCurrentVersion )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("[Expansion Mod] Mod Version outdated!");
					#endif

					m_IsExpansionOutdated = true;
				}
				else 
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("[Expansion Mod] Latest Mod Version Loaded!");
					#endif
					
					m_IsExpansionOutdated = false;
				}
			
				if ( m_IsExpansionOutdated )
				{
					string text = "#STG_EXPANSION_MOD_OUTDATED " + "(" + "#STR_EXPANSION_LOADED_MOD" + ":" + m_ExpansionClientVersion + ") - (" + "#STR_EXPANSION_LATEST_MOD" + ":" + m_ExpansionCurrentVersion + ").";
					m_ModOutdatedText.SetText( text );	
					m_ModOutdatedPanel.Show( m_IsExpansionOutdated );	
				}

				m_ExpansionVersionChecked = true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("LoadingScreen::Show - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetClientExpansionVersion
	// ------------------------------------------------------------
   	static string GetClientExpansionVersion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoadingScreen::GetClientExpansionVersion - Start");
		#endif

		ref array<ref ModInfo> mods = new array<ref ModInfo>;
		string version;
		
		GetDayZGame().GetModInfos( mods );
		for ( int i = 0; i < mods.Count(); ++i )
		{
			if ( mods[i].GetName().IndexOf( "DayZ" ) == 0 && mods[i].GetName().IndexOf( "Expansion" ) == 5 )
			{
				version = mods[i].GetVersion();
				break;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint( "LoadingScreen::GetClientExpansionVersion - Return: " + version );
		#endif

		return version;
	}
};