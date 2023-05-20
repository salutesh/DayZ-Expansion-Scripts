/**
 * ExpansionClientSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

	[NonSerialized()]
	bool m_ShouldShowHUDCategory = false;

	// ================= Vehicles Settings =================
	bool UseCameraLock;
	bool UseInvertedMouseControl;

	bool UseHelicopterMouseControl;
	float HelicopterMouseVerticalSensitivity;
	float HelicopterMouseHorizontalSensitivity;
	bool UsePlaneMouseControl;
	bool TurnOffAutoHoverDuringFlight;

	// ================= Video Settings =================
	bool CastLightShadows;

	// ================= Navigation Settings =================
	bool Show3DClientMarkers;
	bool Show3DPlayerMarkers;
	bool Show3DPartyMarkers;
	bool Show3DGlobalMarkers;

	bool Show3DPartyMemberIcon;

	bool Show2DClientMarkers;
	bool Show2DPlayerMarkers;
	bool Show2DPartyMarkers;
	bool Show2DGlobalMarkers;
	
	bool ShowPartyMemberHUD;

	ExpansionClientUIMemberMarkerType MemberMarkerType;
	ExpansionClientUIMarkerSize MarkerSize;
	ExpansionClientUIPlayerArrowColor PlayerArrowColor;
	bool ShowMemberNameMarker;
	bool ShowMemberDistanceMarker;
	bool ForceColorMemberMarker;
	float AlphaColorHUDOnTopOfHeadOfPlayers;
	float RedColorHUDOnTopOfHeadOfPlayers;
	float GreenColorHUDOnTopOfHeadOfPlayers;
	float BlueColorHUDOnTopOfHeadOfPlayers;
	float AlphaColorLookAtMinimum;
	bool DefaultMarkerLockState;

	// ================= Notification Settings =================
	bool ShowNotifications;
	bool NotificationSound;
	bool NotificationSoundLeaveJoin;
	bool NotificationJoin;
	bool NotificationLeave;
	// int NotificationSoundType;

	// ================= Codelock Settings =================
	bool StreamerMode;
	bool ShowPINCode;

	// ================= HUD Settings =================
	float EarplugLevel;

	// Chat Settings
	bool HUDChatToggle;
	ExpansionClientUIChatSize HUDChatSize;
	float HUDChatMessageTimeThreshold;
	float HUDChatFadeOut;
	ExpansionClientUIChatChannel DefaultChatChannel;

	bool ShowNameQuickMarkers;
	bool ShowDistanceQuickMarkers;

	bool ShowMapMarkerList;

	bool EnableGPSBasedOnVehicleSpeed;

	float VehicleCameraHeight;
	float VehicleCameraDistance;
	float VehicleCameraOffsetY;

	float VehicleResyncTimeout;
	bool ShowDesyncInvulnerabilityNotifications;

	bool MarketMenuCategoriesState;
	bool MarketMenuSkipConfirmations;
	bool MarketMenuFilterPurchasableState;
	bool MarketMenuFilterSellableState;
	bool MarketMenuDisableSuccessNotifications;

	ref array<string> MutedPlayers;

	bool ShowUnderRoofIndicator;

	// -----------------------------------------------------------
	// ExpansionClientSettings Constructor
	// -----------------------------------------------------------
	void ExpansionClientSettings()
	{
		auto trace = EXTrace.StartStack(EXTrace.CLIENT_SETTINGS, this);

		#ifdef EXPANSIONMOD
		m_ShouldShowHUDCategory = true;
		#endif
		#ifdef EXPANSIONMODNAVIGATION
		m_ShouldShowHUDCategory = true;
		#endif
		#ifdef EXPANSIONMODCHAT
		m_ShouldShowHUDCategory = true;
		#endif

		m_Categories = new array< ref ExpansionClientSettingCategory >;
		MutedPlayers = new array<string>;

		Init();
		Defaults();

		if ( !FileExist( EXPANSION_CLIENT_SETTINGS ) || !Load() )
		{
			Print( "Creating client settings." );
			Save();
		}
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings OnRead
	// -----------------------------------------------------------
	private bool OnRead( ParamsReadContext ctx, int version, out bool settingsRepaired = false )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(EXTrace.CLIENT_SETTINGS, this, "OnRead").Add(ctx).Add(version);
#endif

		// Vehicles
		if ( !ctx.Read( UseCameraLock ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read UseCameraLock!");
			return false;
		}
		if ( !ctx.Read( UseInvertedMouseControl ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read UseInvertedMouseControl!");
			return false;
		}
		if ( !ctx.Read( UseHelicopterMouseControl ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read UseHelicopterMouseControl!");
			return false;
		}
		if ( !ctx.Read( UsePlaneMouseControl ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read UsePlaneMouseControl!");
			return false;
		}

		// Video
		if (version < 36)
		{
			//! These three were removed in version 36
			float drawDistance;
			if ( !ctx.Read( drawDistance ) )
			{
				EXPrint(ToString() + "::OnRead - ERROR: Couldn't read drawDistance!");
				return false;
			}
			bool colorGrading;
			if ( !ctx.Read( colorGrading ) )
			{
				EXPrint(ToString() + "::OnRead - ERROR: Couldn't read colorGrading!");
				return false;
			}
			bool colorVignette;
			if ( !ctx.Read( colorVignette ) )
			{
				EXPrint(ToString() + "::OnRead - ERROR: Couldn't read colorVignette!");
				return false;
			}
		}

		if ( !ctx.Read( CastLightShadows ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read CastLightShadows!");
			return false;
		}

		// 3D Markers
		if ( !ctx.Read( Show3DClientMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show3DClientMarkers!");
			return false;
		}
		if ( !ctx.Read( Show3DPlayerMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show3DPlayerMarkers!");
			return false;
		}
		if ( !ctx.Read( Show3DPartyMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show3DPartyMarkers!");
			return false;
		}
		if ( !ctx.Read( Show3DGlobalMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show3DGlobalMarkers!");
			return false;
		}

		// 2D Markers
		if ( !ctx.Read( Show2DClientMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show2DClientMarkers!");
			return false;
		}
		if ( !ctx.Read( Show2DPlayerMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show2DPlayerMarkers!");
			return false;
		}
		if ( !ctx.Read( Show2DPartyMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show2DPartyMarkers!");
			return false;
		}
		if ( !ctx.Read( Show2DGlobalMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show2DGlobalMarkers!");
			return false;
		}

		// Notifications
		if ( !ctx.Read( ShowNotifications ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowNotifications!");
			return false;
		}
		if ( !ctx.Read( NotificationSound ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read NotificationSound!");
			return false;
		}
		if ( !ctx.Read( NotificationSoundLeaveJoin ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read NotificationSoundLeaveJoin!");
			return false;
		}
		if ( !ctx.Read( NotificationJoin ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read NotificationJoin!");
			return false;
		}
		if ( !ctx.Read( NotificationLeave ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read NotificationLeave!");
			return false;
		}

		// Streamer mode
		if ( !ctx.Read( StreamerMode ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read StreamerMode!");
			return false;
		}
		if ( !ctx.Read( ShowPINCode ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowPINCode!");
			return false;
		}

		// HUD Settings
		if ( !ctx.Read( EarplugLevel ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read EarplugLevel!");
			return false;
		}
		if ( !ctx.Read( AlphaColorHUDOnTopOfHeadOfPlayers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read AlphaColorHUDOnTopOfHeadOfPlayers!");
			return false;
		}
		if ( !ctx.Read( RedColorHUDOnTopOfHeadOfPlayers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read RedColorHUDOnTopOfHeadOfPlayers!");
			return false;
		}
		if ( !ctx.Read( GreenColorHUDOnTopOfHeadOfPlayers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read GreenColorHUDOnTopOfHeadOfPlayers!");
			return false;
		}
		if ( !ctx.Read( BlueColorHUDOnTopOfHeadOfPlayers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read BlueColorHUDOnTopOfHeadOfPlayers!");
			return false;
		}
		if ( !ctx.Read( AlphaColorLookAtMinimum ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read AlphaColorLookAtMinimum!");
			return false;
		}

		if ( version < 7 )
			return true;

		// Chat settings
		if ( !ctx.Read( HUDChatSize ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatSize!");
			return false;
		}
		if ( !ctx.Read( HUDChatFadeOut ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatFadeOut!");
			return false;
		}

		if ( version < 8 )
			return true;

		// more markers settings
		if ( !ctx.Read( MemberMarkerType ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MemberMarkerType!");
			return false;
		}
		if ( !ctx.Read( MarkerSize ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MarkerSize!");
			return false;
		}
		if ( !ctx.Read( ShowMemberNameMarker ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowMemberNameMarker!");
			return false;
		}
		if ( !ctx.Read( ShowMemberDistanceMarker ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowMemberDistanceMarker!");
			return false;
		}
		if ( !ctx.Read( ForceColorMemberMarker ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ForceColorMemberMarker!");
			return false;
		}

		if ( version < 9 )
			return true;

		if ( !ctx.Read( ShowNameQuickMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowNameQuickMarkers!");
			return false;
		}

		if ( !ctx.Read( ShowDistanceQuickMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowDistanceQuickMarkers!");
			return false;
		}

		if ( version < 13 )
			return true;

		if ( !ctx.Read( HelicopterMouseVerticalSensitivity ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HelicopterMouseVerticalSensitivity!");
			return false;
		}

		if ( !ctx.Read( HelicopterMouseHorizontalSensitivity ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HelicopterMouseHorizontalSensitivity!");
			return false;
		}

		if ( version < 18 )
			return true;

		if ( !ctx.Read( ShowMapMarkerList ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowMapMarkerList!");
			return false;
		}

		if ( version < 26 )
			return true;

		if ( !ctx.Read( VehicleCameraHeight ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read VehicleCameraHeight!");
			return false;
		}

		if ( !ctx.Read( VehicleCameraDistance ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read VehicleCameraDistance!");
			return false;
		}

		if ( !ctx.Read( VehicleCameraOffsetY ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read VehicleCameraOffsetY!");
			return false;
		}

		if ( version < 29 )
			return true;

		if ( !ctx.Read( MarketMenuCategoriesState ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MarketMenuCategoriesState!");
			return false;
		}

		if ( !ctx.Read( MarketMenuSkipConfirmations ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MarketMenuSkipConfirmations!");
			return false;
		}

		if ( !ctx.Read( MarketMenuFilterPurchasableState ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MarketMenuFilterPurchasableState!");
			return false;
		}

		if ( !ctx.Read( MarketMenuFilterSellableState ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MarketMenuFilterSellableState!");
			return false;
		}

		if ( version < 38 )
			return true;

		if ( !ctx.Read( MarketMenuDisableSuccessNotifications ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MarketMenuDisableSuccessNotifications!");
			return false;
		}

		if ( version < 39 )
			return true;

		if ( !ctx.Read( DefaultMarkerLockState ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read DefaultMarkerLockState!");
			return false;
		}

		if ( !ctx.Read( TurnOffAutoHoverDuringFlight ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read TurnOffAutoHoverDuringFlight!");
			return false;
		}

		if ( version < 40 )
			return true;

		if ( !ctx.Read( Show3DPartyMemberIcon ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read Show3DPartyMemberIcon!");
			return false;
		}

		if ( version < 41 )
			return true;

		if ( !ctx.Read( DefaultChatChannel ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read DefaultChatChannel!");
			return false;
		}

		if ( version < 42 )
			return true;

		if ( version >= 43 )
		{
			int mutedPlayersCount;
			if ( !ctx.Read( mutedPlayersCount ) )
			{
				EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MutedPlayers count!");
				return false;
			}

			MutedPlayers.Clear();
			for (int i = 0; i < mutedPlayersCount; i++)
			{
				string muted;
				if ( !ctx.Read( muted ) )
				{
					EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MutedPlayer string!");
					return false;
				}

				if (MutedPlayers.Find(muted) == -1)
				{
					MutedPlayers.Insert( muted );
				}
				else
				{
					settingsRepaired = true;
					if (mutedPlayersCount > 100)
						return true;  //! Bail
				}
			}
		}

		if ( !ctx.Read( VehicleResyncTimeout ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read VehicleResyncTimeout!");
			return false;
		}

		if ( !ctx.Read( ShowDesyncInvulnerabilityNotifications ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowDesyncInvulnerabilityNotifications!");
			return false;
		}

		if (version < 44)
			return true;

		if ( !ctx.Read( ShowUnderRoofIndicator ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowUnderRoofIndicator!");
			return false;
		}

		if (version < 45)
			return true;

		if ( !ctx.Read( ShowPartyMemberHUD ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowPartyMemberHUD!");
			return false;
		}

		if ( !ctx.Read( PlayerArrowColor ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowPartyMemberHUD!");
			return false;
		}

		if (version < 46)
			return true;

		if ( !ctx.Read( EnableGPSBasedOnVehicleSpeed ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read EnableGPSBasedOnVehicleSpeed!");
			return false;
		}

		if (version < 47)
			return true;

		if ( !ctx.Read( HUDChatToggle ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatToggle!");
			return false;
		}

		if ( !ctx.Read( HUDChatMessageTimeThreshold ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatToggle!");
			return false;
		}

		if (version < 48)
			HUDChatFadeOut /= 10.0;

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings OnSave
	// -----------------------------------------------------------
	private void OnSave( ParamsWriteContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(EXTrace.CLIENT_SETTINGS, this, "OnSave").Add(ctx);
#endif

		// Vehicles
		ctx.Write( UseCameraLock );
		ctx.Write( UseInvertedMouseControl );

		ctx.Write( UseHelicopterMouseControl );
		ctx.Write( UsePlaneMouseControl );

		// Video
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

		// Chat settings
		ctx.Write( HUDChatSize );
		ctx.Write( HUDChatFadeOut );

		// More markers settings
		ctx.Write( MemberMarkerType );
		ctx.Write( MarkerSize );
		ctx.Write( ShowMemberNameMarker );
		ctx.Write( ShowMemberDistanceMarker );
		ctx.Write( ForceColorMemberMarker );

		ctx.Write( ShowNameQuickMarkers );
		ctx.Write( ShowDistanceQuickMarkers );

		ctx.Write( HelicopterMouseVerticalSensitivity );
		ctx.Write( HelicopterMouseHorizontalSensitivity );

		ctx.Write( ShowMapMarkerList );

		ctx.Write( VehicleCameraHeight );
		ctx.Write( VehicleCameraDistance );
		ctx.Write( VehicleCameraOffsetY );

		ctx.Write( MarketMenuCategoriesState );
		ctx.Write( MarketMenuSkipConfirmations );
		ctx.Write( MarketMenuFilterPurchasableState );
		ctx.Write( MarketMenuFilterSellableState );

		ctx.Write( MarketMenuDisableSuccessNotifications );

		ctx.Write( DefaultMarkerLockState );

		ctx.Write( TurnOffAutoHoverDuringFlight );

		ctx.Write( Show3DPartyMemberIcon );

		ctx.Write( DefaultChatChannel );

		int mutedPlayersCount = MutedPlayers.Count();
		ctx.Write( mutedPlayersCount );
		foreach (string muted: MutedPlayers)
		{
			ctx.Write( muted );
		}

		ctx.Write( VehicleResyncTimeout );
		ctx.Write( ShowDesyncInvulnerabilityNotifications );

		ctx.Write( ShowUnderRoofIndicator );

		ctx.Write( ShowPartyMemberHUD );
		ctx.Write( PlayerArrowColor );

		ctx.Write( EnableGPSBasedOnVehicleSpeed );

		ctx.Write( HUDChatToggle );
		ctx.Write( HUDChatMessageTimeThreshold );
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings Load
	// -----------------------------------------------------------
	bool Load()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "Load");
#endif

		FileSerializer file = new FileSerializer;

		if ( file.Open( EXPANSION_CLIENT_SETTINGS, FileMode.READ ) )
		{
			int version;
			if ( !file.Read( version ) )
			{
				EXPrint(ToString() + "::Load - ERROR: Could not read version!");
				file.Close();
				return false;
			}

			EXPrint("Loading Expansion client settings version " + version);

			bool settingsRepaired;
			if ( !OnRead( file, version, settingsRepaired ) )
			{
				EXPrint(ToString() + "::Load - ERROR: Loading client settings failed!");
				file.Close();
				return false;
			}

			file.Close();

			if (settingsRepaired)
				Save();
		} else
		{
			EXPrint(ToString() + "::Load - ERROR: Could not open client settings file \"" + EXPANSION_CLIENT_SETTINGS + "\"!");
			return false;
		}

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings Save
	// -----------------------------------------------------------
	void Save()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "Save");
#endif

		FileSerializer file = new FileSerializer;
		if ( file.Open( EXPANSION_CLIENT_SETTINGS, FileMode.WRITE ) )
		{
			EXPrint("Saving Expansion client settings version " + EXPANSION_VERSION_CLIENT_SETTING_SAVE);
			file.Write( EXPANSION_VERSION_CLIENT_SETTING_SAVE );

			OnSave( file );

			file.Close();
		}
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings Defaults
	// -----------------------------------------------------------
	void Defaults()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "Defaults");
#endif

		CastLightShadows = true;

		Show3DClientMarkers = true;
		Show3DPlayerMarkers = true;
		Show3DPartyMarkers = true;
		Show3DGlobalMarkers = true;

		Show2DClientMarkers = true;
		Show2DPlayerMarkers = true;
		Show2DPartyMarkers = true;
		Show2DGlobalMarkers = true;

		ShowPartyMemberHUD = true;

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

		UseHelicopterMouseControl = false;
		HelicopterMouseVerticalSensitivity = 1.0;
		HelicopterMouseHorizontalSensitivity = 1.0;

		HUDChatToggle = true;
		HUDChatMessageTimeThreshold = 10.0;
		HUDChatSize = ExpansionClientUIChatSize.MEDIUM;
		HUDChatFadeOut = 1.0;

		MemberMarkerType = ExpansionClientUIMemberMarkerType.PERSON;
		MarkerSize = ExpansionClientUIMarkerSize.MEDIUM;
		PlayerArrowColor = ExpansionClientUIPlayerArrowColor.GREY;

		ShowMemberNameMarker = true;
		ShowMemberDistanceMarker = true;
		ForceColorMemberMarker = false;

		// UsePlaneMouseControlInverted = true;
		// UsePlaneMouseControl = false;

		ShowNameQuickMarkers = true;
		ShowDistanceQuickMarkers = true;

		ShowMapMarkerList = true;

		EnableGPSBasedOnVehicleSpeed = false;

		VehicleCameraHeight = 1.0;
		VehicleCameraDistance = 1.0;

		MarketMenuCategoriesState = false;
		MarketMenuSkipConfirmations = false;
		MarketMenuFilterPurchasableState = false;
		MarketMenuFilterSellableState = false;
		MarketMenuDisableSuccessNotifications = false;

		DefaultMarkerLockState = false;
		TurnOffAutoHoverDuringFlight = true;
		Show3DPartyMemberIcon = true;

		DefaultChatChannel = ExpansionClientUIChatChannel.DIRECT;

		VehicleResyncTimeout = 5.0;
		ShowDesyncInvulnerabilityNotifications = false;
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings Init
	// -----------------------------------------------------------
	void Init()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "Init");
#endif

		CreateCategory( "VideoSettings", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO" );

		//! Option to toggle light shadows
		CreateToggle( "CastLightShadows", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_LIGHTSHADOWS", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_LIGHTSHADOWS_DESC" );

	#ifdef EXPANSIONMODNAVIGATION
		CreateCategory( "3DMapMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D" );

		//! Option to toggle view of all personal 3D Map-Markers
		CreateToggle( "Show3DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Player-Markers
		CreateToggle( "Show3DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Map-Markers
		CreateToggle( "Show3DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS_DESC" );
		//! Option to toggle view of all 3D Global Server-Markers
		CreateToggle( "Show3DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS_DESC" );
	#ifdef EXPANSIONMODGROUPS
		//! Option to show/hide the icon that gets displayed next to party member markers
		CreateToggle( "Show3DPartyMemberIcon", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMEMBERICON", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMEMBERICON_DESC" );
	#endif
	#endif

	#ifdef EXPANSIONMODNAVIGATION
		CreateCategory( "2DMapMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D" );

		//! Option to toggle view of all personal 2D Map-Markers
		CreateToggle( "Show2DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DCLIENTMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DCLIENTMARKERS_DESC" );
		//! Option to toggle view of all 2D party Player-Markers
		CreateToggle( "Show2DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPLAYERMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPLAYERMARKERS_DESC" );
		//! Option to toggle view of all 2D party Map-Markers
		CreateToggle( "Show2DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPARTYMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPARTYMARKERS_DESC" );
		//! Option to toggle view of all 2D global Server-Markers
		CreateToggle( "Show2DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DGLOBALMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DGLOBALMARKERS_DESC" );
		//! Option to set default marker lock state for new created map markers.
		CreateToggle( "DefaultMarkerLockState", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_DEFAULTMARKERLOCK", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_DEFAULTMARKERLOCK_DESC" );
	#endif

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
	if ( m_ShouldShowHUDCategory )
		CreateCategory( "HUD", "#STR_EXPANSION_SETTINGS_HUD" );
	#ifdef EXPANSIONMOD	
		//! Option to change ear plug level 
		CreateSlider( "EarplugLevel", "#STR_EXPANSION_SETTINGS_HUD_EARPLUG_LEVEL", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_EARPLUG_LEVEL_DESC", 0.0, 1.0, 0.05 );
	#endif

	#ifdef EXPANSIONMODGROUPS
		//! Option to show/hide the party hud on client side
		CreateToggle( "ShowPartyMemberHUD", "#STR_EXPANSION_SETTINGS_CLIENT_PARTY_SHOWPARTYMEMBERHUD", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_CLIENT_PARTY_SHOWPARTYMEMBERHUD_DESC" );
	#endif

	#ifdef EXPANSIONMODNAVIGATION
		//! Option to use the desired party member marker
		//CreateEnum( "MemberMarkerType", ExpansionClientUIMemberMarkerType, "MemberMarkerType", "MemberMarkerType", "MemberMarkerType" );
		//! Option to set the 3D marker size
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

		CreateToggle( "ShowMapMarkerList", "#STR_EXPANSION_SETTINGS_MAPMENULIST_STATE_PREFERENCE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_MAPMENULIST_STATE_PREFERENCE_DESC" );

		CreateToggle( "EnableGPSBasedOnVehicleSpeed", "#STR_EXPANSION_SETTINGS_GPS_AUTO_UPDATE_STATE_PREFERENCE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_GPS_AUTO_UPDATE_STATE_PREFERENCE_DESC" );

		//! Option to set the 3D marker size
		CreateEnum( "PlayerArrowColor", ExpansionClientUIPlayerArrowColor, "#STR_EXPANSION_SETTINGS_PLAYERARROW_COLOR", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_PLAYERARROW_COLOR_DESC" );
		//Color slider for party member on top of player head
		//CreateSlider( "AlphaColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_ALPHA_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_ALPHA_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
		//CreateSlider( "RedColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_RED_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_RED_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
		//CreateSlider( "GreenColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_GREEN_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_GREEN_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
		//CreateSlider( "BlueColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_BLUE_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_BLUE_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
	
		CreateSlider( "AlphaColorLookAtMinimum", "#STR_EXPANSION_SETTINGS_HUD_3D_MARKER_POINTING", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_3D_MARKER_POINTING_DESC", 0.0, 255.0, 5.0 );
	#endif

	#ifdef EXPANSIONMODCHAT
		CreateToggle( "HUDChatToggle", "#STR_EXPANSION_SETTINGS_HUD_CHAT_TOGGLE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_TOGGLE_DESC" );
		CreateEnum( "HUDChatSize", ExpansionClientUIChatSize, "#STR_EXPANSION_SETTINGS_HUD_CHAT_SIZE", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SIZE_DESC" );
		CreateSlider( "HUDChatMessageTimeThreshold", "#STR_EXPANSION_SETTINGS_HUD_CHAT_MESSAGE_TIME_THRESHOLD", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_MESSAGE_TIME_THRESHOLD_DESC", 1.0, 20.0, 0.25 );
		CreateSlider( "HUDChatFadeOut", "#STR_EXPANSION_SETTINGS_HUD_CHAT_FADEOUT", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_FADEOUT_DESC", 0.0, 6.0, 0.1 );
		CreateEnum( "DefaultChatChannel", ExpansionClientUIChatChannel, "#STR_EXPANSION_SETTINGS_HUD_CHAT_CHANNEL", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_CHANNEL_DESC" );
	#endif

	#ifdef EXPANSIONMODHARDLINE
		CreateToggle( "ShowUnderRoofIndicator", "#STR_EXPANSION_HUD_SHOW_ROOF_INDICATOR", "#STR_EXPANSION_SETTINGS_HUD", "#STR_EXPANSION_HUD_SHOW_ROOF_INDICATOR_DESC" );
	#endif

	#ifdef EXPANSIONMODVEHICLE
		CreateCategory( "Vehicles", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES" );

		//! Option to toggle the vehicle camera
		//CreateToggle( "UseCameraLock", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK_DESC" );
		CreateToggle( "UseInvertedMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_INVERTED", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_DESC_INVERTED" );
		CreateToggle( "UseHelicopterMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL_DESC" );
		CreateSlider( "HelicopterMouseVerticalSensitivity", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_VERTICAL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_VERTICAL_DESC", 0.1, 3.0, 0.1 );
		CreateSlider( "HelicopterMouseHorizontalSensitivity", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_HORIZONTAL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_HORIZONTAL_DESC", 0.1, 3.0, 0.1 );
		CreateToggle( "TurnOffAutoHoverDuringFlight", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_TURN_OFF_AUTOHOVER_DURING_FLIGHT", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_TURN_OFF_AUTOHOVER_DURING_FLIGHT_DESC" );

		//! Option to change vehicle camera height
		CreateSlider( "VehicleCameraHeight", "VEHICLE CAMERA HEIGHT", "VEHICLE CAMERA HEIGHT", "", 0.5, 10.0, 0.5 );
		CreateSlider( "VehicleCameraDistance", "VEHICLE CAMERA DISTANCE", "VEHICLE CAMERA DISTANCE", "", 0.5, 5.0, 0.5 );
		//CreateSlider( "VehicleCameraOffsetY", "VEHICLE CAMERA OFFSET VERTICAL", "VEHICLE CAMERA OFFSET VERTICAL", "", -10.0, 5.0, 0.5 );
		
		//CreateToggle( "UsePlaneMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL_DESC" );

		//! @note Lower values for VehicleResyncTimeout allow for faster resync, but may cause more stutter in crowded areas of the map,
		//! higher values may avoid stutter, but can result in more extreme vehicle position changes in case of reaching the timeout or prevent resync altogether.
		//! A vehicle that fails to resync will remain in that state for the affected client until the player leaves the vehicle's network bubble.
		CreateSlider( "VehicleResyncTimeout", "VEHICLE RESYNC TIMEOUT", "VEHICLE RESYNC TIMEOUT", "How long after desync and no resync has occurred vehicle physics updates are halted and position is restored to last known from server.", 1.0, 10.0, 0.5 );
		CreateToggle( "ShowDesyncInvulnerabilityNotifications", "INVULNERABILITY NOTIFICATIONS", "SHOW DESYNC INVULNERABILITY NOTIFICATIONS", "Show desync invulnerability notifications (after the fact)." );
	#endif

	#ifdef EXPANSIONMODMARKET
		CreateCategory( "MarketMenu", "MARKET MENU" );
		CreateToggle( "MarketMenuCategoriesState", "MARKET CATEGORIES TOGGLE STATE", "MarketMenu", "Automatically expand all categories when opening trader menu (WARNING: Performance hit!)" );
		CreateToggle( "MarketMenuSkipConfirmations", "SKIP ALL MENU CONFIRMATIONS", "MarketMenu", "Skip all confirmations for buying/selling." );
		CreateToggle( "MarketMenuFilterPurchasableState", "PURCHASABLES FILTER STATE", "MarketMenu", "Show only puchasable items by default." );
		CreateToggle( "MarketMenuFilterSellableState", "SELLABLES FILTER STATE", "MarketMenu", "Show only sellable items by default." );
		CreateToggle( "MarketMenuDisableSuccessNotifications", "DISABLE SUCCESS NOTIFICATIONS", "MarketMenu", "Disable notifications for successful purchases and sales." );
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings OnSettingsUpdated
	// -----------------------------------------------------------
	void OnSettingsUpdated(typename type, ExpansionSettingSerializationBase setting)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "OnSettingsUpdated");
#endif

		GetExpansionClientSettings().SI_UpdateSetting.Invoke();
	}

	private void CreateCategory( string name, string displayName )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateCategory");
#endif

		ExpansionClientSettingCategory category = new ExpansionClientSettingCategory;

		category.m_Name = name;
		category.m_DisplayName = displayName;

		m_Categories.Insert( category );
		m_CurrentCategory = category;
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings CreateToggle
	// -----------------------------------------------------------
	private void CreateToggle( string variable, string name, string detailLabel, string detailContent )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateToggle");
#endif

		ExpansionSettingSerializationToggle setting = new ExpansionSettingSerializationToggle;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings CreateSlider
	// -----------------------------------------------------------
	private void CreateSlider( string variable, string name, string detailLabel, string detailContent, float min, float max, float step = 0 )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateSlider");
#endif

		ExpansionSettingSerializationSlider setting = new ExpansionSettingSerializationSlider;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;
		setting.m_Min = min;
		setting.m_Max = max;
		setting.m_Step = step;

		m_CurrentCategory.m_Settings.Insert( setting );
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings CreateInt
	// -----------------------------------------------------------
	//! TODO: Not working.
	private void CreateInt( string variable, string name, string detailLabel, string detailContent )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateInt");
#endif

		ExpansionSettingSerializationInt setting = new ExpansionSettingSerializationInt;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings CreateEnum
	// ----------------------------------------------------------
	private void CreateEnum( string variable, typename enm, string name, string detailLabel, string detailContent )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateEnum");
#endif

		ExpansionSettingSerializationEnum setting = new ExpansionSettingSerializationEnum;

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
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings CreateEnum
	// ----------------------------------------------------------
	private void CreateString( string variable, string name, string detailLabel, string detailContent )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateString");
#endif

		ExpansionSettingSerializationString setting = new ExpansionSettingSerializationString;

		setting.m_Variable = variable;
		setting.m_Name = name;
		setting.m_Instance = this;
		setting.m_DetailLabel = detailLabel;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
	}
}

static ref ExpansionClientSettings g_ExClientSettings;

static ExpansionClientSettings GetExpansionClientSettings()
{
#ifdef EXPANSIONTRACE
	auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, "GetExpansionClientSettings");
#endif

	if (GetGame().IsDedicatedServer())
		return null;

	if ( !g_ExClientSettings )
	{
		g_ExClientSettings = new ExpansionClientSettings;
	}

	return g_ExClientSettings;
}
