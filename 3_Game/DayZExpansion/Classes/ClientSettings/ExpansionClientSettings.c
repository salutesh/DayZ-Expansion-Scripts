/**
 * ExpansionClientSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionClientSettings
 * @brief		Expansion Main Client Settings class
 **/
 
class ExpansionClientSettings
{
	[NonSerialized()]
	autoptr ScriptInvoker SI_UpdateSetting = new ScriptInvoker();
	
	[NonSerialized()]
	autoptr array< ref ExpansionClientSettingCategory > m_Categories;

	[NonSerialized()]
	private ExpansionClientSettingCategory m_CurrentCategory;

	// Vehicles
	bool UseCameraLock;
	bool UseInvertedMouseControl;

	bool UseHelicopterMouseControl;
	bool UsePlaneMouseControl;

	// Video
	float DrawDistance;
	bool ColorGrading;
	bool ColorVignette;
	bool CastLightShadows;

	// Mapping
	bool Show3DMarkers;
	bool Show3DClientMarkers;
	bool Show3DPlayerMarkers;
	bool Show3DPartyMarkers;
	bool Show3DGlobalMarkers;

	bool Show2DMarkers;
	bool Show2DClientMarkers;
	bool Show2DPlayerMarkers;
	bool Show2DPartyMarkers;
	bool Show2DGlobalMarkers;

	// Notifications
	bool ShowNotifications;
	bool NotificationSound;
	bool NotificationJoin;
	bool NotificationLeave;
	// int NotificationSoundType;

	// Streamer mode
	bool StreamerMode;
	bool ShowPINCode;

	// HUD Settings
	bool HUDShowClientClock;
	bool HUDShowClientHumanity;
	float EarplugLevel;
	float RedColorHUDOnTopOfHeadOfPlayers;
	float GreenColorHUDOnTopOfHeadOfPlayers;
	float BlueColorHUDOnTopOfHeadOfPlayers;

	void ExpansionClientSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::ExpansionClientSettings - Start");
		#endif
		
		m_Categories = new array< ref ExpansionClientSettingCategory >;

		Defaults();

		Init();

		Load();

		Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::ExpansionClientSettings - End");
		#endif
	}

	void Load()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Load - Start");
		#endif
		
		JsonFileLoader< ExpansionClientSettings >.JsonLoadFile( EXPANSION_CLIENT_SETTINGS, this );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Load - End");
		#endif
	}

	void Save()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Load - Start");
		#endif
		
		JsonFileLoader< ExpansionClientSettings >.JsonSaveFile( EXPANSION_CLIENT_SETTINGS, this );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Load - End");
		#endif
	}

	void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Defaults - Start");
		#endif
		
		DrawDistance = 1600.0;
		ColorGrading = true;
		ColorVignette = false;
		CastLightShadows = false;

		Show3DMarkers = true;
		Show3DClientMarkers = true;
		Show3DPlayerMarkers = true;
		Show3DPartyMarkers = true;
		Show3DGlobalMarkers = true;

		Show2DMarkers = true;
		Show2DClientMarkers = true;
		Show2DPlayerMarkers = true;
		Show2DPartyMarkers = true;
		Show2DGlobalMarkers = true;
		
		ShowNotifications = true;
		NotificationSound = true;
		NotificationJoin = true;
		NotificationLeave = true;

		StreamerMode = true;
		ShowPINCode = true;

		HUDShowClientClock = false;
		HUDShowClientHumanity = false;
		EarplugLevel = 0.05;
		RedColorHUDOnTopOfHeadOfPlayers = 255;
		GreenColorHUDOnTopOfHeadOfPlayers = 180;
		BlueColorHUDOnTopOfHeadOfPlayers = 24;

		UseCameraLock = false;
		UseInvertedMouseControl = true;
		
		UseHelicopterMouseControl = false;

		// UsePlaneMouseControlInverted = true;
		//  UsePlaneMouseControl = false;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Defaults - End");
		#endif
	}

	void Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Init - Start");
		#endif

		CreateCategory( "VideoSettings", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO" );
		
		//! Option to change camera view distance 
		// CreateSlider( "DrawDistance", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_DRAWDISTANCE", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_DRAW_DISTANCE_DESC", 1600, 3000 );
		//! Option to toggle color grading
		CreateToggle( "ColorGrading", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_COLORGRADING", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_COLORGRADING_DESC" );
		//! Option to toggle color vignette
		CreateToggle( "ColorVignette", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_COLORVIGNETTE", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_COLORVIGNETTE_DESC" );
		//! Option to toggle light shadows
		CreateToggle( "CastLightShadows", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_LIGHTSHADOWS", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_LIGHTSHADOWS_DESC" );
		
		CreateCategory( "3DMapMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D" );

		//! Option to toggle view of all 3D Map-Markers
		CreateToggle( "Show3DMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DMARKERS_DESC" );
		//! Option to toggle view of all personal 3D Map-Markers
		CreateToggle( "Show3DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Player-Markers 
		CreateToggle( "Show3DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Map-Markers
		CreateToggle( "Show3DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS_DESC" );
		//! Option to toggle view of all 3D Global Server-Markers
		CreateToggle( "Show3DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS_DESC" );
		
		CreateCategory( "2DMapMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D" );
		
		//! Option to toggle view of all 2D Map-Markers
		CreateToggle( "Show2DMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DMARKERS_DESC" );
		//! Option to toggle view of all personal 2D Map-Markers
		CreateToggle( "Show2DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DCLIENTMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DCLIENTMARKERS_DESC" );
		//! Option to toggle view of all 2D Party Player-Markers
		CreateToggle( "Show2DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPLAYERMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPLAYERMARKERS_DESC" );
		//! Option to toggle view of all 2D Party Map-Markers
		CreateToggle( "Show2DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPARTYMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPARTYMARKERS_DESC" );
		//! Option to toggle view of all 2D Global Server-Markers
		CreateToggle( "Show2DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DGLOBALMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DGLOBALMARKERS_DESC" );
	
		CreateCategory( "Notifications", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS" );

		//! Option to toggle notification sounds
		CreateToggle( "ShowNotifications", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_DISPLAY", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_DISPLAY_DESC" );
		//! Option to toggle notification sounds
		CreateToggle( "NotificationSound", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND_DESC" );
		//! Option to toggle display of player join notifications
		CreateToggle( "NotificationJoin", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_JOIN", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_JOIN_DESC" );
		//! Option to toggle display of player left notifications
		CreateToggle( "NotificationLeave", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_LEAVE", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_LEAVE_DESC" );
		
		CreateCategory( "StreamerMode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE" );
		
		//! Option to toggle streamer mode
		CreateToggle( "StreamerMode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_OPTION", "#STR_EXPANSION_SETTINGS_STREAMER_MODE", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_OPTION_DESC" );
		//! Option to toggle display of pins and passwords
		CreateToggle( "ShowPINCode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_SHOW_PIN_CODE", "#STR_EXPANSION_SETTINGS_STREAMER_MODE", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_SHOW_PIN_CODE_DESC" );
		
		//CreateCategory( "HUD", "#STR_EXPANSION_SETTINGS_HUD" );

		//! Option to toggle the display of the clock in the ingame hud
		//CreateToggle( "HUDShowClientClock", "#STR_EXPANSION_SETTINGS_HUD_SHOW_CLOCK", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_SHOW_CLOCK_DESC" );
		//! Option to toggle the display of the humanity idicator in the ingame hud
		//CreateToggle( "HUDShowClientHumanity", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY_DESC" );

		//! Option to change ear plug level 
		// CreateSlider( "EarplugLevel", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY_DESC", 0, 0.2 );
	
		//Color slider for party member on top of player head
		// CreateSlider( "RedColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY_DESC" );
		// CreateSlider( "GreenColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY_DESC" );
		// CreateSlider( "BlueColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_SHOW_HUMANITY_DESC" );

		CreateCategory( "Vehicles", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES" );
		
		//! Option to toggle the vehicle camera
		CreateToggle( "UseCameraLock", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK_DESC" );
		CreateToggle( "UseInvertedMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_INVERTED", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_DESC_INVERTED" );
		CreateToggle( "UseHelicopterMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL_DESC" );
		//CreateToggle( "UsePlaneMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL_DESC" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::Init - End");
		#endif
	}

	private void CreateCategory( string name, string displayName )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::CreateCategory - Start");
		#endif
		
		ExpansionClientSettingCategory category = new ref ExpansionClientSettingCategory;

		category.m_Name = name;
		category.m_DisplayName = displayName;

		m_Categories.Insert( category );
		m_CurrentCategory = category;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::CreateCategory - End");
		#endif
	}

	private void CreateToggle( string variable, string name, string detailLabel, string detailContent )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::CreateToggle - Start");
		#endif
		
		ExpansionClientSettingToggle setting = new ref ExpansionClientSettingToggle;

		//name.Replace( "#", "" );
		//detailLabel.Replace( "#", "" );
		//detailContent.Replace( "#", "" );

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::CreateToggle - End");
		#endif
	}

	private void CreateSlider( string variable, string name, string detailLabel, string detailContent, float min = 0.0, float max = 1.0 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::CreateToggle - Start");
		#endif
		
		ExpansionClientSettingSlider setting = new ref ExpansionClientSettingSlider;

		//name.Replace( "#", "" );
		//detailLabel.Replace( "#", "" );
		//detailContent.Replace( "#", "" );

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;
		setting.m_Min = min;
		setting.m_Max = max;

		m_CurrentCategory.m_Settings.Insert( setting );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::CreateToggle - End");
		#endif
	}
}

static ref ExpansionClientSettings g_ExClientSettings;

ref ExpansionClientSettings GetExpansionClientSettings()
{
	#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettings::GetExpansionClientSettings - Start");
		#endif
	
	if ( !g_ExClientSettings )
	{
		g_ExClientSettings = new ExpansionClientSettings;
	}

	#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionClientSettings::GetExpansionClientSettings - Return: " + g_ExClientSettings.ToString() );
		#endif
	return g_ExClientSettings;
}