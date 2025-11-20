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
	bool OpenMapOnPlayerPos;
	
	float MaxDistance3DClientMarkers;
	float MaxDistance3DPlayerMarkers;
	float MaxDistance3DPartyMarkers;
	float MaxDistance3DGlobalMarkers;

	bool ShowMarkerTextColor;

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
	bool AutoOpenLockMenuAfterPlacing;

	// ================= Misc =================
	bool EnableLiquidTypeColors;

	// ================= HUD Settings =================
	float EarplugLevel;

	// Chat Settings
	bool HUDChatToggle;
	ExpansionClientUIChatSize HUDChatSize;
	int HUDChatOutlineSize;
	float HUDChatOutlineOpacity;
	int HUDChatShadowSize;
	float HUDChatShadowOpacity;
	float HUDChatShadowOffsetX;
	float HUDChatShadowOffsetY;
	float HUDChatMessageTimeThreshold;
	float HUDChatFadeOut;
	ExpansionClientUIChatChannel DefaultChatChannel;
	int HUDChatMessagesHistoryLimit;

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
	bool RarityColorToggle;

	bool QuestHUDVisibility = true;
	ref map<int, bool> QuestVisibilityStates;

	float ViewDistance;
	float ObjectViewDistance;

	// -----------------------------------------------------------
	// ExpansionClientSettings Constructor
	// -----------------------------------------------------------
	void ExpansionClientSettings()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.StartStack(EXTrace.CLIENT_SETTINGS, this);
#endif

	#ifdef EXPANSIONMODNAVIGATION
		m_ShouldShowHUDCategory = true;
	#endif
	#ifdef EXPANSIONMODGROUPS
		m_ShouldShowHUDCategory = true;
	#endif
	#ifdef EXPANSIONMODHARDLINE
		m_ShouldShowHUDCategory = true;
	#endif

		m_Categories = new array< ref ExpansionClientSettingCategory >;
		MutedPlayers = new array<string>;
	
		QuestHUDVisibility = true;
		QuestVisibilityStates = new map<int, bool>;

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
		
		if (version < 49)
			return true;		
		
		if ( !ctx.Read( RarityColorToggle ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read RarityColorToggle!");
			return false;
		}
		
		if ( version < 50 )
			return true;

		//! Quest HUD visibility		
		if ( !ctx.Read( QuestHUDVisibility ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read QuestHUDVisibility!");
			return false;
		}
		
		int questVisStates;
		if ( !ctx.Read( questVisStates ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read questVisStates!");
			return false;
		}
		
		if (!QuestVisibilityStates)
			QuestVisibilityStates = new map<int, bool>;

		for (int j = 0; j < questVisStates; j++)
		{
			int questID;
			if ( !ctx.Read( questID ) )
			{
				EXPrint(ToString() + "::OnRead - ERROR: Couldn't read questID!");
				return false;
			}
			
			bool visState;
			if ( !ctx.Read( visState ) )
			{
				EXPrint(ToString() + "::OnRead - ERROR: Couldn't read visState!");
				return false;
			}
			
			EXPrint(ToString() + "::OnRead - Add quest visibity state: " + questID + " | " + visState);
			QuestVisibilityStates[questID] = visState;
		}
		
		if ( version < 51 )
		{
			EnableLiquidTypeColors = true;
			return true;
		}

		if ( !ctx.Read( EnableLiquidTypeColors ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read EnableLiquidTypeColors!");
			return false;
		}
		
		if ( version < 52 )
		{
			return true;
		}

		if ( !ctx.Read( AutoOpenLockMenuAfterPlacing ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read AutoOpenLockMenuAfterPlacing!");
			return false;
		}
		
		if ( version < 53 )
		{
			return true;
		}

		if ( !ctx.Read( ViewDistance ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ViewDistance!");
			return false;
		}

		if ( !ctx.Read( ObjectViewDistance ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ObjectViewDistance!");
			return false;
		}
		
		if ( version < 54 )
		{
			return true;
		}

		if ( !ctx.Read( MaxDistance3DClientMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MaxDistance3DClientMarkers!");
			return false;
		}

		if ( !ctx.Read( MaxDistance3DPlayerMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MaxDistance3DPlayerMarkers!");
			return false;
		}

		if ( !ctx.Read( MaxDistance3DPartyMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MaxDistance3DPartyMarkers!");
			return false;
		}

		if ( !ctx.Read( MaxDistance3DGlobalMarkers ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read MaxDistance3DGlobalMarkers!");
			return false;
		}

		if ( !ctx.Read( ShowMarkerTextColor ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read ShowMarkerTextColor!");
			return false;
		}
		
		if ( !ctx.Read( OpenMapOnPlayerPos ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read OpenMapOnPlayerPos!");
			return false;
		}
		
		if ( version < 55 )
		{
			return true;
		}
		
		if ( !ctx.Read( HUDChatMessagesHistoryLimit ) )
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatMessagesHistoryLimit!");
			return false;
		}
		
		if (version < 56)
			return true;

		if (!ctx.Read(HUDChatOutlineSize))
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatOutlineSize!");
			return false;
		}

		if (!ctx.Read(HUDChatOutlineOpacity))
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatOutlineOpacity!");
			return false;
		}

		if (!ctx.Read(HUDChatShadowSize))
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatShadowSize!");
			return false;
		}

		if (!ctx.Read(HUDChatShadowOpacity))
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatShadowOpacity!");
			return false;
		}

		if (!ctx.Read(HUDChatShadowOffsetX))
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatShadowOffsetX!");
			return false;
		}

		if (!ctx.Read(HUDChatShadowOffsetY))
		{
			EXPrint(ToString() + "::OnRead - ERROR: Couldn't read HUDChatShadowOffsetY!");
			return false;
		}

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
		
		ctx.Write( RarityColorToggle );

		//! Quest HUD visibility
		ctx.Write( QuestHUDVisibility );
		ctx.Write( QuestVisibilityStates.Count() );

		foreach (int questID, bool visState: QuestVisibilityStates)
		{
			ctx.Write( questID );
			ctx.Write( visState );
		}

		ctx.Write( EnableLiquidTypeColors );

		ctx.Write( AutoOpenLockMenuAfterPlacing );

		ctx.Write( ViewDistance );
		ctx.Write( ObjectViewDistance );

		ctx.Write( MaxDistance3DClientMarkers );
		ctx.Write( MaxDistance3DPlayerMarkers );
		ctx.Write( MaxDistance3DPartyMarkers );
		ctx.Write( MaxDistance3DGlobalMarkers );
		ctx.Write( ShowMarkerTextColor );
		ctx.Write( OpenMapOnPlayerPos );

		ctx.Write( HUDChatMessagesHistoryLimit );

		ctx.Write(HUDChatOutlineSize);
		ctx.Write(HUDChatOutlineOpacity);
		ctx.Write(HUDChatShadowSize);
		ctx.Write(HUDChatShadowOpacity);
		ctx.Write(HUDChatShadowOffsetX);
		ctx.Write(HUDChatShadowOffsetY);
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

		MaxDistance3DClientMarkers = 30000;
		MaxDistance3DPlayerMarkers = 30000;
		MaxDistance3DPartyMarkers = 30000;
		MaxDistance3DGlobalMarkers = 30000;

		OpenMapOnPlayerPos = true;

		ShowMarkerTextColor = false;

		ShowNotifications = true;
		NotificationSound = true;
		NotificationJoin = true;
		NotificationLeave = true;

		StreamerMode = false;
		ShowPINCode = false;
		AutoOpenLockMenuAfterPlacing = false;

		EnableLiquidTypeColors = true;

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
		HUDChatOutlineSize = 0;
		HUDChatOutlineOpacity = 1;
		HUDChatShadowSize = 4;
		HUDChatShadowOpacity = 1;
		HUDChatShadowOffsetX = 1;
		HUDChatShadowOffsetY = 1;
		HUDChatFadeOut = 3.0;

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

		DefaultChatChannel = ExpansionClientUIChatChannel.GLOBAL;
		HUDChatMessagesHistoryLimit = 50;

		VehicleResyncTimeout = 5.0;
		ShowDesyncInvulnerabilityNotifications = false;
	
		QuestHUDVisibility = true;

		ViewDistance = 0;  //! 0 = use game default
		ObjectViewDistance = 0;  //! 0 = use game default
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
		CreateToggle( "CastLightShadows", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_LIGHTSHADOWS", "#STR_EXPANSION_SETTINGS_CLIENT_VIDEO_LIGHTSHADOWS_DESC" );
	#ifdef EXPANSIONMODNAVIGATION
		CreateCategory( "3DMapMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_3D" );

		//! Option to toggle view of all personal 3D Map-Markers
		CreateToggle( "Show3DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DCLIENTMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Player-Markers
		CreateToggle( "Show3DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPLAYERMARKERS_DESC" );
		//! Option to toggle view of all 3D Party Map-Markers
		CreateToggle( "Show3DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMARKERS_DESC" );
		//! Option to toggle view of all 3D Global Server-Markers
		CreateToggle( "Show3DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DGLOBALMARKERS_DESC" );
	#ifdef EXPANSIONMODGROUPS
		//! Option to show/hide the icon that gets displayed next to party member markers
		CreateToggle( "Show3DPartyMemberIcon", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMEMBERICON", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW3DPARTYMEMBERICON_DESC" );
	#endif
		CreateToggle( "ShowMarkerTextColor", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOWMARKERTEXTCOLOR", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOWMARKERTEXTCOLOR_DESC" );
		CreateSlider( "MaxDistance3DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_CLIENT3DDISTANCE", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_CLIENT3DDISTANCE_DESC", 100, 30000, 100 );
		CreateSlider( "MaxDistance3DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_PLAYER3DDISTANCE", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_PLAYER3DDISTANCE_DESC", 100, 30000, 100 );
		CreateSlider( "MaxDistance3DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_PARTY3DDISTANCE", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_PARTY3DDISTANCE_DESC", 100, 30000, 100 );
		CreateSlider( "MaxDistance3DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_GLOBAL3DDISTANCE", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_GLOBAL3DDISTANCE_DESC", 100, 30000, 100 );

		//! Option to use the desired party member marker
		//CreateEnum( "MemberMarkerType", ExpansionClientUIMemberMarkerType, "MemberMarkerType", "MemberMarkerType" );
		//! Option to set the 3D marker size
		CreateEnum( "MarkerSize", ExpansionClientUIMarkerSize, "#STR_EXPANSION_SETTINGS_MARKER_SIZE", "#STR_EXPANSION_SETTINGS_MARKER_SIZE_DESC" );
		//! Option to toggle party Member name under their marker
		CreateToggle( "ShowMemberNameMarker", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_NAME", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_NAME_DESC" );
		//! Option to toggle party Member distance under their marker
		CreateToggle( "ShowMemberDistanceMarker", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_DISTANCE", "#STR_EXPANSION_SETTINGS_PARTY_MEMBER_MARKER_DISTANCE_DESC" );
		//! Option to use the desired party member color instead of a randomized color
		//CreateToggle( "ForceColorMemberMarker", "ForceColorMemberMarker", "ForceColorMemberMarker" );

		//! Option to toggle name under quick markers
		CreateToggle( "ShowNameQuickMarkers", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_NAME", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_NAME_DESC" );
		//! Option to toggle distance under quick markers
		CreateToggle( "ShowDistanceQuickMarkers", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_DISTANCE", "#STR_EXPANSION_SETTINGS_PARTY_QUICK_MARKER_DISTANCE_DESC" );

		//Color slider for party member on top of player head
		//CreateSlider( "AlphaColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_ALPHA_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD_ALPHA_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
		//CreateSlider( "RedColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_RED_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD_RED_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
		//CreateSlider( "GreenColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_GREEN_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD_GREEN_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
		//CreateSlider( "BlueColorHUDOnTopOfHeadOfPlayers", "#STR_EXPANSION_SETTINGS_HUD_BLUE_HEAD_PLAYER", "#STR_EXPANSION_SETTINGS_HUD_BLUE_HEAD_PLAYER_DESC", 0.0, 255.0, 5.0 );
	
		CreateSlider( "AlphaColorLookAtMinimum", "#STR_EXPANSION_SETTINGS_HUD_3D_MARKER_POINTING", "#STR_EXPANSION_SETTINGS_HUD_3D_MARKER_POINTING_DESC", 0.0, 255.0, 5.0 );
	#endif
	

	#ifdef EXPANSIONMODNAVIGATION
		CreateCategory( "2DMapMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_MARKERS_2D" );

		//! Option to toggle view of all personal 2D Map-Markers
		CreateToggle( "Show2DClientMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DCLIENTMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DCLIENTMARKERS_DESC" );
		//! Option to toggle view of all 2D party Player-Markers
		CreateToggle( "Show2DPlayerMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPLAYERMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPLAYERMARKERS_DESC" );
		//! Option to toggle view of all 2D party Map-Markers
		CreateToggle( "Show2DPartyMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPARTYMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DPARTYMARKERS_DESC" );
		//! Option to toggle view of all 2D global Server-Markers
		CreateToggle( "Show2DGlobalMarkers", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DGLOBALMARKERS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_SHOW2DGLOBALMARKERS_DESC" );
		//! Option to set default marker lock state for new created map markers.
		CreateToggle( "DefaultMarkerLockState", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_DEFAULTMARKERLOCK", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_DEFAULTMARKERLOCK_DESC" );
		CreateToggle( "OpenMapOnPlayerPos", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_OPENMAPONPLAYERPOS", "#STR_EXPANSION_SETTINGS_CLIENT_MAP_OPENMAPONPLAYERPOS_DESC" );

		CreateToggle( "ShowMapMarkerList", "#STR_EXPANSION_SETTINGS_MAPMENULIST_STATE_PREFERENCE", "#STR_EXPANSION_SETTINGS_MAPMENULIST_STATE_PREFERENCE_DESC" );

		CreateEnum( "PlayerArrowColor", ExpansionClientUIPlayerArrowColor, "#STR_EXPANSION_SETTINGS_PLAYERARROW_COLOR", "#STR_EXPANSION_SETTINGS_PLAYERARROW_COLOR_DESC" );
	#endif

		CreateCategory( "Notifications", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS" );

		//! Option to toggle notification sounds
		CreateToggle( "ShowNotifications", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_DISPLAY", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_DISPLAY_DESC" );
		//! Option to toggle notification sounds
		CreateToggle( "NotificationSound", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND_DESC" );
		//! Option to toggle notification sounds
		CreateToggle( "NotificationSoundLeaveJoin", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND_LEAVE_JOIN", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_SOUND_LEAVE_JOIN_DESC" );
		//! Option to toggle display of player join notifications
		CreateToggle( "NotificationJoin", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_JOIN", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_JOIN_DESC" );
		//! Option to toggle display of player left notifications
		CreateToggle( "NotificationLeave", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_LEAVE", "#STR_EXPANSION_SETTINGS_NOTIFICATIONS_LEAVE_DESC" );

		CreateCategory( "StreamerMode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE" );

		//! Option to toggle streamer mode
		CreateToggle( "StreamerMode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_OPTION", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_OPTION_DESC" );
		//! Option to toggle display of pins and passwords
		CreateToggle( "ShowPINCode", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_SHOW_PIN_CODE", "#STR_EXPANSION_SETTINGS_STREAMER_MODE_SHOW_PIN_CODE_DESC" );
		
	#ifdef EXPANSION_INSPECT_MENU_NEW_ENABLE
		CreateToggle( "EnableLiquidTypeColors", "#STR_EXPANSION_SETTINGS_ENABLE_LIQUID_TYPE_COLORS", "#STR_EXPANSION_SETTINGS_ENABLE_LIQUID_TYPE_COLORS_DESC" );
	#endif

	#ifdef EXPANSIONMOD	
		//! Option to change ear plug level 
		CreateSlider( "EarplugLevel", "#STR_EXPANSION_SETTINGS_HUD_EARPLUG_LEVEL", "#STR_EXPANSION_SETTINGS_HUD_EARPLUG_LEVEL_DESC", 0.0, 1.0, 0.05 );
	#endif

		CreateCategory( "BaseBuilding", "#STR_EXPANSION_SETTINGS_BASEBUILDING" );

		//! Option to automaticaly open the lock menu after placing the codelock
		CreateToggle( "AutoOpenLockMenuAfterPlacing", "#STR_EXPANSION_SETTINGS_AUTOOPEN_CODELOCK_PLACING", "#STR_EXPANSION_SETTINGS_AUTOOPEN_CODELOCK_PLACING_DESC" );

		if ( m_ShouldShowHUDCategory )
			CreateCategory( "HUD", "#STR_EXPANSION_SETTINGS_HUD" );

	#ifdef EXPANSIONMODGROUPS
		//! Option to show/hide the party hud on client side
		CreateToggle( "ShowPartyMemberHUD", "#STR_EXPANSION_SETTINGS_CLIENT_PARTY_SHOWPARTYMEMBERHUD", "#STR_EXPANSION_SETTINGS_CLIENT_PARTY_SHOWPARTYMEMBERHUD_DESC" );
	#endif

	#ifdef EXPANSIONMODNAVIGATION
		CreateToggle( "EnableGPSBasedOnVehicleSpeed", "#STR_EXPANSION_SETTINGS_GPS_AUTO_UPDATE_STATE_PREFERENCE", "#STR_EXPANSION_SETTINGS_GPS_AUTO_UPDATE_STATE_PREFERENCE_DESC" );
	#endif

	#ifdef EXPANSIONMODHARDLINE
		CreateToggle( "ShowUnderRoofIndicator", "#STR_EXPANSION_HUD_SHOW_ROOF_INDICATOR", "#STR_EXPANSION_HUD_SHOW_ROOF_INDICATOR_DESC" );
	#endif

	#ifdef EXPANSIONMODCHAT
		CreateCategory( "HUD", "#STR_EXPANSION_SETTINGS_HUD_CHAT" );
		CreateToggle( "HUDChatToggle", "#STR_EXPANSION_SETTINGS_HUD_CHAT_TOGGLE", "#STR_EXPANSION_SETTINGS_HUD_CHAT_TOGGLE_DESC" );
		CreateEnum( "HUDChatSize", ExpansionClientUIChatSize, "#STR_EXPANSION_SETTINGS_HUD_CHAT_SIZE", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SIZE_DESC" );
		CreateSlider( "HUDChatOutlineSize", "#STR_EXPANSION_SETTINGS_HUD_CHAT_OUTLINE_SIZE", "#STR_EXPANSION_SETTINGS_HUD_CHAT_OUTLINE_SIZE_DESC", 0.0, 10.0, 1.0 );
		CreateSlider( "HUDChatOutlineOpacity", "#STR_EXPANSION_SETTINGS_HUD_CHAT_OUTLINE_OPACITY", "#STR_EXPANSION_SETTINGS_HUD_CHAT_OUTLINE_OPACITY_DESC", 0.0, 1.0, 0.01 );
		CreateSlider( "HUDChatShadowSize", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_SIZE", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_SIZE_DESC", 0.0, 10.0, 1.0 );
		CreateSlider( "HUDChatShadowOpacity", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_OPACITY", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_OPACITY_DESC", 0.0, 1.0, 0.01 );
		CreateSlider( "HUDChatShadowOffsetX", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_OFFSET_X", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_OFFSET_X_DESC", 0.0, 4.0, 0.25 );
		CreateSlider( "HUDChatShadowOffsetY", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_OFFSET_Y", "#STR_EXPANSION_SETTINGS_HUD_CHAT_SHADOW_OFFSET_Y_DESC", 0.0, 4.0, 0.25 );
		CreateSlider( "HUDChatMessageTimeThreshold", "#STR_EXPANSION_SETTINGS_HUD_CHAT_MESSAGE_TIME_THRESHOLD", "#STR_EXPANSION_SETTINGS_HUD_CHAT_MESSAGE_TIME_THRESHOLD_DESC", 1.0, 20.0, 0.25 );
		CreateSlider( "HUDChatFadeOut", "#STR_EXPANSION_SETTINGS_HUD_CHAT_FADEOUT", "#STR_EXPANSION_SETTINGS_HUD_CHAT_FADEOUT_DESC", 0.0, 6.0, 0.1 );
		CreateEnum( "DefaultChatChannel", ExpansionClientUIChatChannel, "#STR_EXPANSION_SETTINGS_HUD_CHAT_CHANNEL", "#STR_EXPANSION_SETTINGS_HUD_CHAT_CHANNEL_DESC" );
		CreateSlider( "HUDChatMessagesHistoryLimit", "#STR_EXPANSION_SETTINGS_HUD_CHAT_MESSAGES_HISTORY_LIMIT", "#STR_EXPANSION_SETTINGS_HUD_CHAT_MESSAGES_HISTORY_LIMIT_DESC", 0.0, 100.0, 1.0 );
	#endif

	#ifdef EXPANSIONMODHARDLINE
		CreateCategory( "Inventory", "#STR_EXPANSION_SETTINGS_INVENTORY" );
		CreateToggle( "RarityColorToggle", "#STR_EXPANSION_SETTINGS_INVENTORY_RARITY_COLOR_TOGGLE", "#STR_EXPANSION_SETTINGS_INVENTORY_RARITY_COLOR_TOGGLE_DESC" );
	#endif

	#ifdef EXPANSIONMODVEHICLE
		CreateCategory( "Vehicles", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES" );

		//! Option to toggle the vehicle camera
		//CreateToggle( "UseCameraLock", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_FREELOOK_DESC" );

		//! Option to change vehicle camera height
		CreateSlider( "VehicleCameraHeight", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_CAMERA_HEIGHT", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_CAMERA_HEIGHT_DESC", 0.5, 10.0, 0.1 );
		CreateSlider( "VehicleCameraDistance", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_CAMERA_DISTANCE", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_CAMERA_DISTANCE_DESC", 0.5, 5.0, 0.1 );
		//CreateSlider( "VehicleCameraOffsetY", "VEHICLE CAMERA OFFSET VERTICAL", "", -10.0, 5.0, 0.5 );

		//! Heli mouse ctrl
		CreateToggle( "UseHelicopterMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_CONTROL_DESC" );
		CreateToggle( "UseInvertedMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_INVERTED", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_MOUSE_CONTROL_DESC_INVERTED" );
		CreateSlider( "HelicopterMouseVerticalSensitivity", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_VERTICAL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_VERTICAL_DESC", 0.1, 3.0, 0.1 );
		CreateSlider( "HelicopterMouseHorizontalSensitivity", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_HORIZONTAL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_MOUSE_HORIZONTAL_DESC", 0.1, 3.0, 0.1 );
		CreateToggle( "TurnOffAutoHoverDuringFlight", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_TURN_OFF_AUTOHOVER_DURING_FLIGHT", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_HELICOPTER_TURN_OFF_AUTOHOVER_DURING_FLIGHT_DESC" );
		
		//CreateToggle( "UsePlaneMouseControl", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_PLANE_MOUSE_CONTROL_DESC" );

		//! @note Lower values for VehicleResyncTimeout allow for faster resync, but may cause more stutter in crowded areas of the map,
		//! higher values may avoid stutter, but can result in more extreme vehicle position changes in case of reaching the timeout or prevent resync altogether.
		//! A vehicle that fails to resync will remain in that state for the affected client until the player leaves the vehicle's network bubble.
		CreateSlider( "VehicleResyncTimeout", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_RESYNC_TIMEOUT", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_RESYNC_TIMEOUT_DESC", 1.0, 10.0, 0.5 );
		CreateToggle( "ShowDesyncInvulnerabilityNotifications", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_INVULNERABILITY_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_CLIENT_VEHICLES_INVULNERABILITY_NOTIFICATIONS_DESC" );
	#endif

	#ifdef EXPANSIONMODMARKET
		CreateCategory( "MarketMenu", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET" );
		CreateToggle( "MarketMenuCategoriesState", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_CATEGORIES_TOGGLE_STATE", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_CATEGORIES_TOGGLE_STATE_DESC" );
		CreateToggle( "MarketMenuSkipConfirmations", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_SKIP_CONFIRMATIONS", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_SKIP_CONFIRMATIONS_DESC" );
		CreateToggle( "MarketMenuFilterPurchasableState", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_PURCHASABLES_FILTER_STATE", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_PURCHASABLES_FILTER_STATE_DESC" );
		CreateToggle( "MarketMenuFilterSellableState", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_SELLABLES_FILTER_STATE", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_SELLABLES_FILTER_STATE_DESC" );
		CreateToggle( "MarketMenuDisableSuccessNotifications", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_DISABLE_SUCCESS_NOTIFICATIONS", "#STR_EXPANSION_SETTINGS_CLIENT_MARKET_DISABLE_SUCCESS_NOTIFICATIONS_DESC" );
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
	private void CreateToggle( string variable, string name, string detailContent )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateToggle");
	#endif

		ExpansionSettingSerializationToggle setting = new ExpansionSettingSerializationToggle(this, variable);

		setting.m_Name = name;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings CreateSlider
	// -----------------------------------------------------------
	private void CreateSlider( string variable, string name, string detailContent, float min, float max, float step = 0 )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateSlider");
	#endif

		ExpansionSettingSerializationSlider setting = new ExpansionSettingSerializationSlider(this, variable);

		setting.m_Name = name;
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
	private void CreateInt( string variable, string name, string detailContent )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateInt");
	#endif

		ExpansionSettingSerializationInt setting = new ExpansionSettingSerializationInt(this, variable);

		setting.m_Name = name;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
	}

	// -----------------------------------------------------------
	// ExpansionClientSettings CreateEnum
	// ----------------------------------------------------------
	private void CreateEnum( string variable, typename enm, string name, string detailContent )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateEnum");
	#endif

		ExpansionSettingSerializationEnum setting = new ExpansionSettingSerializationEnum(this, variable);

		setting.m_Name = name;
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
	private void CreateString( string variable, string name, string detailContent )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, this, "CreateString");
	#endif

		ExpansionSettingSerializationString setting = new ExpansionSettingSerializationString(this, variable);

		setting.m_Name = name;
		setting.m_DetailContent = detailContent;

		m_CurrentCategory.m_Settings.Insert( setting );
	}
	
	void SetQuestHUDVisibility(bool state)
	{
		QuestHUDVisibility = state;
		Save();
	}
	
	bool GetQuestHUDVisibility()
	{
		return QuestHUDVisibility;
	}
	
	void SetQuestVisibilityState(int questID, bool state)
	{
		QuestVisibilityStates[questID] = state;
		Save();
	}

	bool GetQuestVisibilityState(int questID)
	{
		if (QuestVisibilityStates.Contains(questID))
			return QuestVisibilityStates[questID];

		return true;
	}
}

static ref ExpansionClientSettings g_ExClientSettings;

static ExpansionClientSettings GetExpansionClientSettings()
{
#ifdef EXPANSIONTRACE
	auto trace = CF_Trace_0(EXTrace.CLIENT_SETTINGS, "GetExpansionClientSettings");
#endif

	if (g_Game.IsDedicatedServer())
		return null;

	if ( !g_ExClientSettings )
	{
		g_ExClientSettings = new ExpansionClientSettings;
	}

	return g_ExClientSettings;
}
