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
	float HelicopterMouseVerticalSensitivity;
	float HelicopterMouseHorizontalSensitivity;
	bool UsePlaneMouseControl;

	// Video
	float DrawDistance;
	bool ColorGrading;
	bool ColorVignette;
	bool CastLightShadows;

	// Mapping
	bool Show3DClientMarkers;
	bool Show3DPlayerMarkers;
	bool Show3DPartyMarkers;
	bool Show3DGlobalMarkers;

	bool Show2DClientMarkers;
	bool Show2DPlayerMarkers;
	bool Show2DPartyMarkers;
	bool Show2DGlobalMarkers;

	// Notifications
	bool ShowNotifications;
	bool NotificationSound;
	bool NotificationSoundLeaveJoin;
	bool NotificationJoin;
	bool NotificationLeave;
	// int NotificationSoundType;

	// Streamer mode
	bool StreamerMode;
	bool ShowPINCode;

	// HUD Settings
	float EarplugLevel;

	// Markers Settings
	ExpansionClientUIMemberMarkerType MemberMarkerType;
	ExpansionClientUIMarkerSize MarkerSize;
	bool ShowMemberNameMarker;
	bool ShowMemberDistanceMarker;
	bool ForceColorMemberMarker;
	float AlphaColorHUDOnTopOfHeadOfPlayers;
	float RedColorHUDOnTopOfHeadOfPlayers;
	float GreenColorHUDOnTopOfHeadOfPlayers;
	float BlueColorHUDOnTopOfHeadOfPlayers;
	float AlphaColorLookAtMinimum;
	
	// Chat Settings
	ExpansionClientUIChatSize HUDChatSize;
	float HUDChatFadeOut;
	
	bool ShowNameQuickMarkers;
	bool ShowDistanceQuickMarkers;
	
	// -----------------------------------------------------------
	// ExpansionClientSettings Constructor
	// -----------------------------------------------------------
	void ExpansionClientSettings()
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::ExpansionClientSettings - Start");
		#endif
		
		m_Categories = new array< ref ExpansionClientSettingCategory >;
		
		Init();

		if ( !FileExist( EXPANSION_CLIENT_SETTINGS ) || !Load() )
		{
			Print( "Creating client settings." );
			Defaults();
			Save();
		}
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::ExpansionClientSettings - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings OnRead
	// -----------------------------------------------------------
	private bool OnRead( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::OnRead - Start");
		#endif
		
		// Vehicles
		if ( !ctx.Read( UseCameraLock ) )
			return false;
		if ( !ctx.Read( UseInvertedMouseControl ) )
			return false;
		if ( !ctx.Read( UseHelicopterMouseControl ) )
			return false;
		if ( !ctx.Read( UsePlaneMouseControl ) )
			return false;
		
		// Video
		if ( !ctx.Read( DrawDistance ) )
			return false;
		if ( !ctx.Read( ColorGrading ) )
			return false;
		if ( !ctx.Read( ColorVignette ) )
			return false;
		if ( !ctx.Read( CastLightShadows ) )
			return false;
		
		// 3D Markers
		if ( !ctx.Read( Show3DClientMarkers ) )
			return false;
		if ( !ctx.Read( Show3DPlayerMarkers ) )
			return false;
		if ( !ctx.Read( Show3DPartyMarkers ) )
			return false;
		if ( !ctx.Read( Show3DGlobalMarkers ) )
			return false;
		
		// 2D Markers
		if ( !ctx.Read( Show2DClientMarkers ) )
			return false;
		if ( !ctx.Read( Show2DPlayerMarkers ) )
			return false;
		if ( !ctx.Read( Show2DPartyMarkers ) )
			return false;
		if ( !ctx.Read( Show2DGlobalMarkers ) )
			return false;
		
		// Notifications
		if ( !ctx.Read( ShowNotifications ) )
			return false;
		if ( !ctx.Read( NotificationSound ) )
			return false;
		if ( !ctx.Read( NotificationSoundLeaveJoin ) )
			return false;
		if ( !ctx.Read( NotificationJoin ) )
			return false;
		if ( !ctx.Read( NotificationLeave ) )
			return false;
		
		// Streamer mode
		if ( !ctx.Read( StreamerMode ) )
			return false;
		if ( !ctx.Read( ShowPINCode ) )
			return false;
		
		// HUD Settings
		if ( !ctx.Read( EarplugLevel ) )
			return false;
		if ( !ctx.Read( AlphaColorHUDOnTopOfHeadOfPlayers ) )
			return false;
		if ( !ctx.Read( RedColorHUDOnTopOfHeadOfPlayers ) )
			return false;
		if ( !ctx.Read( GreenColorHUDOnTopOfHeadOfPlayers ) )
			return false;
		if ( !ctx.Read( BlueColorHUDOnTopOfHeadOfPlayers ) )
			return false;
		if ( !ctx.Read( AlphaColorLookAtMinimum ) )
			return false;
		
		if ( version < 7 )
			return false;

		// Chat settings
		if ( !ctx.Read( HUDChatSize ) )
			return false;
		if ( !ctx.Read( HUDChatFadeOut ) )
			return false;
		
		if ( version < 8 )
			return false;

		// more markers settings
		if ( !ctx.Read( MemberMarkerType ) )
			return false;
		if ( !ctx.Read( MarkerSize ) )
			return false;
		if ( !ctx.Read( ShowMemberNameMarker ) )
			return false;
		if ( !ctx.Read( ShowMemberDistanceMarker ) )
			return false;
		if ( !ctx.Read( ForceColorMemberMarker ) )
			return false;
		
		if ( version < 9 )
			return false;
		
		if ( !ctx.Read( ShowNameQuickMarkers ) )
			return false;
		
		if ( !ctx.Read( ShowDistanceQuickMarkers ) )
			return false;
		
		if ( version < 13 )
			return false;
		
		if ( !ctx.Read( HelicopterMouseVerticalSensitivity ) )
			return false;
		
		if ( !ctx.Read( HelicopterMouseHorizontalSensitivity ) )
			return false;

		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::OnRead - End and return");
		#endif
		
		return true;
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings OnSave
	// -----------------------------------------------------------
	private void OnSave( ParamsWriteContext ctx, int version )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::OnSave - Start");
		#endif
		
		// Vehicles
		ctx.Write( UseCameraLock );
		ctx.Write( UseInvertedMouseControl );

		ctx.Write( UseHelicopterMouseControl );
		ctx.Write( UsePlaneMouseControl );

		// Video
		ctx.Write( DrawDistance );
		ctx.Write( ColorGrading );
		ctx.Write( ColorVignette );
		ctx.Write( CastLightShadows );

		// 3D Markers
		ctx.Write( Show3DClientMarkers );
		ctx.Write( Show3DPlayerMarkers );
		ctx.Write( Show3DPartyMarkers );
		ctx.Write( Show3DGlobalMarkers );

		// 2D Markers
		ctx.Write( Show2DClientMarkers );
		ctx.Write( Show2DPlayerMarkers );
		ctx.Write( Show2DPartyMarkers );
		ctx.Write( Show2DGlobalMarkers );

		// Notifications
		ctx.Write( ShowNotifications );
		ctx.Write( NotificationSound );
		ctx.Write( NotificationSoundLeaveJoin );
		ctx.Write( NotificationJoin );
		ctx.Write( NotificationLeave );

		// Streamer mode
		ctx.Write( StreamerMode );
		ctx.Write( ShowPINCode );

		// HUD Settings
		ctx.Write( EarplugLevel );
		ctx.Write( AlphaColorHUDOnTopOfHeadOfPlayers );
		ctx.Write( RedColorHUDOnTopOfHeadOfPlayers );
		ctx.Write( GreenColorHUDOnTopOfHeadOfPlayers );
		ctx.Write( BlueColorHUDOnTopOfHeadOfPlayers );
		ctx.Write( AlphaColorLookAtMinimum );

		if ( version < 7 )
			return;
		
		// Chat settings
		ctx.Write( HUDChatSize );
		ctx.Write( HUDChatFadeOut );

		if ( version < 8 )
			return;

		// More markers settings
		ctx.Write( MemberMarkerType );
		ctx.Write( MarkerSize );
		ctx.Write( ShowMemberNameMarker );
		ctx.Write( ShowMemberDistanceMarker );
		ctx.Write( ForceColorMemberMarker );
		
		if ( version < 9 )
			return;
		
		ctx.Write( ShowNameQuickMarkers );
		ctx.Write( ShowDistanceQuickMarkers );

		
		if ( version < 13 )
			return;

		ctx.Write( HelicopterMouseVerticalSensitivity );
		ctx.Write( HelicopterMouseHorizontalSensitivity );
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::OnSave - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings Load
	// -----------------------------------------------------------
	bool Load()
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Load - Start");
		#endif

		FileSerializer file = new FileSerializer;
		
		if ( file.Open( EXPANSION_CLIENT_SETTINGS, FileMode.READ ) )
		{
			int version;
			if ( !file.Read( version ) )
			{
				file.Close();
				return false;
			}

			if ( !OnRead( file, version ) )
			{
				file.Close();
				return false;
			}
			
			file.Close();
		} else
		{
			return false;
		}
		
		return true;
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Load - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings Save
	// -----------------------------------------------------------
	void Save()
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Load - Start");
		#endif
			
		FileSerializer file = new FileSerializer;
		if ( file.Open( EXPANSION_CLIENT_SETTINGS, FileMode.WRITE ) )
		{
			int version = EXPANSION_VERSION_CLIENT_SETTING_SAVE;
			file.Write( version );
			
			OnSave( file, version );
			
			file.Close();
		}
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Load - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings Defaults
	// -----------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Defaults - Start");
		#endif
		
		DrawDistance = 1600.0;
		ColorGrading = true;
		ColorVignette = true;
		CastLightShadows = true;

		Show3DClientMarkers = true;
		Show3DPlayerMarkers = true;
		Show3DPartyMarkers = true;
		Show3DGlobalMarkers = true;

		Show2DClientMarkers = true;
		Show2DPlayerMarkers = true;
		Show2DPartyMarkers = true;
		Show2DGlobalMarkers = true;
		
		ShowNotifications = true;
		NotificationSound = true;
		NotificationJoin = true;
		NotificationLeave = true;

		StreamerMode = false;
		ShowPINCode = true;

		EarplugLevel = 0.05;
		AlphaColorHUDOnTopOfHeadOfPlayers = 255;
		RedColorHUDOnTopOfHeadOfPlayers = 255;
		GreenColorHUDOnTopOfHeadOfPlayers = 180;
		BlueColorHUDOnTopOfHeadOfPlayers = 24;
		AlphaColorLookAtMinimum = 80;

		UseCameraLock = false;
		UseInvertedMouseControl = false;
		
		UseHelicopterMouseControl = true;
		HelicopterMouseVerticalSensitivity = 1.0;
		HelicopterMouseHorizontalSensitivity = 1.0;

		HUDChatSize = ExpansionClientUIChatSize.SMALL;
		HUDChatFadeOut = 10;

		MemberMarkerType = ExpansionClientUIMemberMarkerType.PERSON;
		MarkerSize = ExpansionClientUIMarkerSize.MEDIUM;
		ShowMemberNameMarker = true;
		ShowMemberDistanceMarker = true;
		ForceColorMemberMarker = false;

		// UsePlaneMouseControlInverted = true;
		// UsePlaneMouseControl = false;
		
		ShowNameQuickMarkers = true;
		ShowDistanceQuickMarkers = true;
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Defaults - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings Init
	// -----------------------------------------------------------
	void Init()
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Init - Start");
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

		//! Option to toggle view of all personal 3D Map-Markers
		CreateToggle( "Show3DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Player-Markers 
		CreateToggle( "Show3DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Map-Markers
		CreateToggle( "Show3DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS_DESC" );
		//! Option to toggle view of all 3D Global Server-Markers
		CreateToggle( "Show3DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS_DESC" );
		
		CreateCategory( "2DMapMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D" );
		
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
		//! Option to toggle notification sounds
		CreateToggle( "NotificationSoundLeaveJoin", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND_LEAVE_JOIN", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND_LEAVE_JOIN_DESC" );
		//! Option to toggle display of player join notifications
		CreateToggle( "NotificationJoin", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_JOIN", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_JOIN_DESC" );
		//! Option to toggle display of player left notifications
		CreateToggle( "NotificationLeave", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_LEAVE", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_LEAVE_DESC" );
		
		CreateCategory( "StreamerMode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE" );
		
		//! Option to toggle streamer mode
		CreateToggle( "StreamerMode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_OPTION", "#STR_EXPANSION_SETTINGS_STREAMER_MODE", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_OPTION_DESC" );
		//! Option to toggle display of pins and passwords
		CreateToggle( "ShowPINCode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_SHOW_PIN_CODE", "#STR_EXPANSION_SETTINGS_STREAMER_MODE", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_SHOW_PIN_CODE_DESC" );
		
		CreateCategory( "HUD", "#STR_EXPANSION_SETTINGS_HUD" );

		//! Option to change ear plug level 
		CreateSlider( "EarplugLevel", "#STR_EXPANSION_SETTINGS_HUD_EARPLUG_LEVEL", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_EARPLUG_LEVEL_DESC", 0.0, 1.0 );
	
		//! Option to use the desired party member marker
		//CreateEnum( "MemberMarkerType", ExpansionClientUIMemberMarkerType, "MemberMarkerType", "MemberMarkerType", "MemberMarkerType" );
		//! Option to use the desired party member color instead of a randomized color
		CreateEnum( "MarkerSize", ExpansionClientUIMarkerSize, "#STR_EXPANSION_SETTINGS_MARKER_SIZE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_MARKER_SIZE_DESC" );
		//! Option to toggle party Member name under their marker
		CreateToggle( "ShowMemberNameMarker", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_NAME", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_NAME_DESC" );
		//! Option to toggle party Member distance under their marker
		CreateToggle( "ShowMemberDistanceMarker", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_DISTANCE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_DISTANCE_DESC" );
		//! Option to use the desired party member color instead of a randomized color
		//CreateToggle( "ForceColorMemberMarker", "ForceColorMemberMarker", "ForceColorMemberMarker", "ForceColorMemberMarker" );

		//! Option to toggle name under quick markers
		CreateToggle( "ShowNameQuickMarkers", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_NAME", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_NAME_DESC" );
		//! Option to toggle distance under quick markers
		CreateToggle( "ShowDistanceQuickMarkers", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_DISTANCE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_DISTANCE_DESC" );

		//Color slider for party member on top of player head
		//CreateSlider( "AlphaColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_ALPHA_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_ALPHA_HEAD_PLAYER_DESC", 0.0, 255.0 );
		//CreateSlider( "RedColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_RED_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_RED_HEAD_PLAYER_DESC", 0.0, 255.0 );
		//CreateSlider( "GreenColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_GREEN_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_GREEN_HEAD_PLAYER_DESC", 0.0, 255.0 );
		//CreateSlider( "BlueColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_BLUE_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_BLUE_HEAD_PLAYER_DESC", 0.0, 255.0 );

		CreateSlider( "AlphaColorLookAtMinimum", "#STR_EXPANSION_SETTINGS_HUD_3D_MARKER_POINTING", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_3D_MARKER_POINTING_DESC", 0.0, 255.0 );
		
		CreateEnum( "HUDChatSize", ExpansionClientUIChatSize, "#STR_EXPANSION_SETTINGS_HUD_CHAT_SIZE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SIZE_DESC" );
		CreateSlider( "HUDChatFadeOut", "#STR_EXPANSION_SETTINGS_HUD_CHAT_FADEOUT", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_FADEOUT_DESC", 0.0, 60.0 );
		
		CreateCategory( "Vehicles", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES" );
		
		//! Option to toggle the vehicle camera
		//CreateToggle( "UseCameraLock", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK_DESC" );
		CreateToggle( "UseInvertedMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_INVERTED", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_DESC_INVERTED" );
		CreateToggle( "UseHelicopterMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL_DESC" );
		CreateSlider( "HelicopterMouseVerticalSensitivity", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_VERTICAL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_VERTICAL_DESC", 0.1, 3.0 );
		CreateSlider( "HelicopterMouseHorizontalSensitivity", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_HORIZONTAL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_HORIZONTAL_DESC", 0.1, 3.0 );
		
		//CreateToggle( "UsePlaneMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL_DESC" );

		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::Init - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings OnSettingsUpdated
	// -----------------------------------------------------------
	void OnSettingsUpdated( typename type, ExpansionSettingSerializationBase setting )
	{
		GetExpansionClientSettings().SI_UpdateSetting.Invoke();
	}

	private void CreateCategory( string name, string displayName )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateCategory - Start");
		#endif
		
		ExpansionClientSettingCategory category = new ref ExpansionClientSettingCategory;

		category.m_Name = name;
		category.m_DisplayName = displayName;

		m_Categories.Insert( category );
		m_CurrentCategory = category;
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateCategory - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings CreateToggle
	// -----------------------------------------------------------
	private void CreateToggle( string variable, string name, string detailLabel, string detailContent )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateToggle - Start");
		#endif
		
		ExpansionSettingSerializationToggle setting = new ref ExpansionSettingSerializationToggle;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateToggle - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings CreateSlider
	// -----------------------------------------------------------
	private void CreateSlider( string variable, string name, string detailLabel, string detailContent, float min = 0.0, float max = 1.0 )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateSlider - Start");
		#endif
		
		ExpansionSettingSerializationSlider setting = new ref ExpansionSettingSerializationSlider;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;
		setting.m_Min = min;
		setting.m_Max = max;

		m_CurrentCategory.m_Settings.Insert( setting );
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateSlider - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings CreateInt
	// -----------------------------------------------------------
	//! TODO: Not working.
	private void CreateInt( string variable, string name, string detailLabel, string detailContent )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateInt - Start");
		#endif
		
		ExpansionSettingSerializationInt setting = new ref ExpansionSettingSerializationInt;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateInt - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// ExpansionClientSettings CreateEnum
	// ----------------------------------------------------------
	private void CreateEnum( string variable, typename enm, string name, string detailLabel, string detailContent )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateEnum - Start");
		#endif
		
		ExpansionSettingSerializationEnum setting = new ref ExpansionSettingSerializationEnum;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;

		for ( int j = 0; j < enm.GetVariableCount(); ++j )
		{
			if ( enm.GetVariableType( j ) == int )
			{
				setting.m_Values.Insert( enm.GetVariableName( j ) );
			}
		}

		m_CurrentCategory.m_Settings.Insert( setting );
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint("ExpansionClientSettings::CreateEnum - End");
		#endif
	}
}

static ref ExpansionClientSettings g_ExClientSettings;

ref ExpansionClientSettings GetExpansionClientSettings()
{
	#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
	EXLogPrint("ExpansionClientSettings::GetExpansionClientSettings - Start");
	#endif
	
	if ( !g_ExClientSettings )
	{
		g_ExClientSettings = new ExpansionClientSettings;
	}

	#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
	EXLogPrint( "ExpansionClientSettings::GetExpansionClientSettings - Return: " + g_ExClientSettings.ToString() );
	#endif
	return g_ExClientSettings;
}