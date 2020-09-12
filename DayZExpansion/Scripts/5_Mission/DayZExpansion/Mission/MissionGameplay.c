/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionGameplay
 * @brief		
 **/
modded class MissionGameplay
{
	//! Expansion chat channel counter
	ExpansionChatChannels 						m_ChatChannel;
	//! Expansion chat fade timer
	ref ExpansionWidgetFadeTimer 				exp_m_ChannelFadeTimer;
	//! Expansion chat fade timeout timer
	ref Timer 									exp_m_ChannelTimeoutTimer;
	//! Expansion chat fade timer
	ref ExpansionWidgetFadeTimer 				exp_m_ChannelNameFadeTimer;
	//! Expansion chat fade timeout timer
	ref Timer 									exp_m_ChannelNameTimeoutTimer;
	//! Expansion HUD Root Widget
	Widget 										m_ExpansionHudRootWidget;
	//! Expansion chat channel widget
	Widget 										m_WidgetChatChannel;
	//! Expansion chat channel name widget
	TextWidget 									m_ChatChannelName;
	//! Expansion Hud
	ref ExpansionIngameHud 						m_ExpansionHud;
	//!	Earplug check
	protected bool 								m_WasEarplugToggled;
	//! Client/Player Data
	protected bool 								m_DataSent;
	
	Widget										m_ChatRootWidget;
	
	protected bool								m_WasGPSToggled;
	
	Widget										m_ChatPanel;
	Widget										m_VoiceLevelSeperator;
	ref WidgetFadeTimer							m_SeperatorFadeTimer;

	//! Modules
	ref ExpansionAutorunModule 					m_AutoRunModule;
	ExpansionMarkerModule 						m_MarkerModule;
	private bool								m_MarkerToggleState = true;
	private bool								m_PartyMarkerToggleState = true;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void MissionGameplay()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::MissionGameplay - Start");
		#endif
		
		m_ExpansionHudRootWidget = null;		
		m_ChatRootWidget = null;
		
		exp_m_ChannelFadeTimer = new ExpansionWidgetFadeTimer();
		exp_m_ChannelTimeoutTimer = new Timer(CALL_CATEGORY_GUI);
		exp_m_ChannelNameFadeTimer = new ExpansionWidgetFadeTimer();
		exp_m_ChannelNameTimeoutTimer = new Timer(CALL_CATEGORY_GUI);
		
		CreateDayZExpansion();
		
		if ( IsMissionClient() )
			GetExpansionClientSettings().Load();

		if ( !IsMissionOffline() )
			g_exGlobalSettings.Unload();

		Class.CastTo( m_AutoRunModule, GetModuleManager().GetModule( ExpansionAutorunModule ) );
		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );

		m_DataSent = false;
			
		m_SeperatorFadeTimer = new WidgetFadeTimer;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::MissionGameplay - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~MissionGameplay()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::~MissionGameplay - Start");
		#endif

		DestroyDayZExpansion();
		DestroyNotificationSystem();

		g_exGlobalSettings.Unload();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::~MissionGameplay - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SwitchChatChannelToGlobal
	// ------------------------------------------------------------
	private void SwitchChatChannelToGlobal()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToGlobal - Start");
		#endif
		
		if ( GetExpansionSettings().GetGeneral().EnableGlobalChat )
		{
			m_ChatChannel = ExpansionChatChannels.CCGlobal;

			m_ChatChannelName.SetText( "Global Communication" );
			m_ChatChannelName.SetColor( EXP_GLOBAL_TEXT_COLOUR );
		} else
		{
			SwitchChatChannelToTeam();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToGlobal - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SwitchChatChannelToTeam
	// ------------------------------------------------------------
	private void SwitchChatChannelToTeam()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToGlobal - Start");
		#endif

		ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
		
		if (partyModule.HasParty() && GetExpansionSettings().GetGeneral().EnablePartyChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCTeam;

			m_ChatChannelName.SetText("Team Communication");
			m_ChatChannelName.SetColor(EXP_TEAM_TEXT_COLOUR);
		} else
		{
			SwitchChatChannelToTransport();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToGlobal - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SwitchChatChannelToTransport
	// ------------------------------------------------------------
	private void SwitchChatChannelToTransport()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToTransport - Start");
		#endif

		PlayerBase player = PlayerBase.Cast( g_Game.GetPlayer() );
		Object parent = Object.Cast( player.GetParent() );

		if (parent && parent.IsTransport() && GetExpansionSettings().GetGeneral().EnableTransportChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCTransport;

			m_ChatChannelName.SetText("Transport Communication");
			m_ChatChannelName.SetColor(EXP_TRANSPORT_COLOUR);
		} else
		{
			SwitchChatChannelToAdmin();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToTransport - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SwitchChatChannelToAdmin
	// ------------------------------------------------------------
	private void SwitchChatChannelToAdmin()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToAdmin - Start");
		#endif

		if ( GetPermissionsManager().HasPermission( "Admin.Chat" ) )
		{
			m_ChatChannel = ExpansionChatChannels.CCAdmin;

			m_ChatChannelName.SetText( "Admin Communication" );
			m_ChatChannelName.SetColor( EXP_ADMIN_TEXT_COLOUR );
		} else
		{
			SwitchChatChannelToDirect();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToAdmin - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SwitchChatChannelToDirect
	// ------------------------------------------------------------
	private void SwitchChatChannelToDirect()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToDirect - Start");
		#endif

		m_ChatChannel = ExpansionChatChannels.CCDirect;

		m_ChatChannelName.SetText( "Direct Communication" );
		m_ChatChannelName.SetColor( EXP_DIRECT_TEXT_COLOUR );

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToDirect - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SwitchChannel
	// ------------------------------------------------------------
	void SwitchChannel()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChannel - Start");
		#endif

		switch ( m_ChatChannel )
		{
			case ExpansionChatChannels.CCDirect:
				SwitchChatChannelToGlobal();
				break;
			case ExpansionChatChannels.CCGlobal:
				SwitchChatChannelToTeam();
				break;
			case ExpansionChatChannels.CCTeam:
				SwitchChatChannelToTransport();
				break;
			case ExpansionChatChannels.CCTransport:
				SwitchChatChannelToAdmin();
				break;
			case ExpansionChatChannels.CCAdmin:
				SwitchChatChannelToDirect();
				break;
			default:
				SwitchChatChannelToDirect();
				break;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChannel - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetChatChannel
	// ------------------------------------------------------------
	int GetChatChannel()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "MissionGameplay::GetChatChannel" );
		#endif

		return m_ChatChannel;
	}

	// ------------------------------------------------------------
	// OnInit
	// ------------------------------------------------------------
	override void ShowChat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ShowChat - Start");
		#endif

		if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
			return;
				
		exp_m_ChannelNameTimeoutTimer.Stop();
		exp_m_ChannelTimeoutTimer.Stop();
		m_WidgetChatChannel.SetAlpha( 0.6 );
		m_ChatChannelName.SetAlpha( 1 );
		m_WidgetChatChannel.Show( true );
		m_ChatChannelName.Show( true );
		
		int level = GetGame().GetVoiceLevel();
		UpdateVoiceLevelWidgets( level );

		m_UIManager.EnterScriptedMenu( MENU_CHAT_INPUT, NULL );
	
		PlayerControlDisable( INPUT_EXCLUDE_ALL );
		
		PlayerControlDisable( INPUT_EXCLUDE_CHAT_EXPANSION );
		
		//SetFocus( m_ChatRootWidget.FindAnyWidget("ScrollerContainer") );
		
		GetUApi().GetInputByName("UAPersonView").Supress();	
		GetUApi().GetInputByName( "UAPersonView" ).ForceDisable( true );
		GetGame().GetUIManager().ShowUICursor( true );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ShowChat - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override HideChat
	// ------------------------------------------------------------
	override void HideChat()
	{
		super.HideChat();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::HideChat - Start");
		#endif

		GetUApi().GetInputByName( "UAPersonView" ).ForceDisable( false );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::HideChat - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override PlayerControlDisable
	// ------------------------------------------------------------
	override void PlayerControlDisable(int mode)
	{
		switch (mode)
		{
			case INPUT_EXCLUDE_ALL:
			{
				GetUApi().ActivateExclude("menuexpansion");
				break;
			}
			
			case INPUT_EXCLUDE_CHAT_EXPANSION:
			{
				GetUApi().ActivateExclude("chatexpansion");
				break;
			}
		}
		
		super.PlayerControlDisable(mode);
	}

	// ------------------------------------------------------------
	// OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnInit - Start");
		#endif

		if ( m_Initialized )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MissionGameplay::OnInit - End");
			#endif

			return;
		}
		
		super.OnInit();
		
		//! Expansion Hud
		InitExpansionHud();
		
		//! Expansion Chat
		InitChat();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// InitExpansionHud
	// ------------------------------------------------------------
	void InitExpansionHud()
	{
		if ( !m_ExpansionHudRootWidget )
		{
			m_ExpansionHudRootWidget = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/hud/expansion_hud.layout");
			m_ExpansionHudRootWidget.Show( false );
			
			m_ExpansionHud = new ExpansionIngameHud;
			m_ExpansionHud.Init( m_ExpansionHudRootWidget );
			
			if (m_Hud && m_ExpansionHud)
				m_Hud.SetExpansionHUD(true);
			
			InitVoiceLevelIndicator();
		}
	}
	
	// ------------------------------------------------------------
	// InitChat
	// ------------------------------------------------------------
	void InitChat()
	{
		if ( !m_ChatRootWidget && m_HudRootWidget )
		{
			m_ChatPanel = Widget.Cast( m_HudRootWidget.FindAnyWidget("ChatFrameWidget") );
			m_ChatPanel.SetFlags( WidgetFlags.IGNOREPOINTER, true );
			
			m_ChatRootWidget = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/chat/expansion_chat_panel.layout", m_ChatPanel );
			m_Chat.Init( m_ChatRootWidget );
	
			if ( m_Chat )
			{
				m_WidgetChatChannel = Widget.Cast( m_ExpansionHudRootWidget.FindAnyWidget("ChatChannelPanel") );
				m_ChatChannelName = TextWidget.Cast( m_ExpansionHudRootWidget.FindAnyWidget("ChatChannelName") );
	
				m_WidgetChatChannel.SetAlpha(0);
				m_ChatChannelName.SetAlpha(0);
	
				m_WidgetChatChannel.Show(false);
				m_ChatChannelName.Show(false);
	
				SwitchChatChannelToDirect();
			}
			
			m_ChatRootWidget.Show( true );
		}
	}
		
	// ------------------------------------------------------------
	// InitChat
	// ------------------------------------------------------------
	void InitVoiceLevelIndicator()
	{
		//! Unlink vanilla voice level indicator
		m_HudRootWidget.FindAnyWidget("mic").Unlink();
		m_HudRootWidget.FindAnyWidget("VoiceLevelsPanel").Unlink();
		
		m_VoiceLevelsWidgets.Clear();
		m_VoiceLevelTimers.Clear();
		
		//! Von enabled icon
		m_MicrophoneIcon = ImageWidget.Cast( m_ExpansionHudRootWidget.FindAnyWidget("mic") );
		m_MicrophoneIcon.Show(false);
		
		//! Seperator
		m_VoiceLevelSeperator = Widget.Cast( m_ExpansionHudRootWidget.FindAnyWidget("BadgesSpacer") );
		m_VoiceLevelSeperator.Show(false);
		
		//! Von voice level
		m_VoiceLevels = m_ExpansionHudRootWidget.FindAnyWidget("VoiceLevelsPanel");
		m_VoiceLevelsWidgets = new map<int, ImageWidget>; // [key] voice level
		m_VoiceLevelTimers = new map<int,ref WidgetFadeTimer>; // [key] voice level
	
		if( m_VoiceLevels )
		{
			m_VoiceLevelsWidgets.Set(VoiceLevelWhisper, ImageWidget.Cast( m_VoiceLevels.FindAnyWidget("Whisper") ));
			m_VoiceLevelsWidgets.Set(VoiceLevelTalk, ImageWidget.Cast( m_VoiceLevels.FindAnyWidget("Talk") ));
			m_VoiceLevelsWidgets.Set(VoiceLevelShout, ImageWidget.Cast( m_VoiceLevels.FindAnyWidget("Shout") ));
			
			m_VoiceLevelTimers.Set(VoiceLevelWhisper, new WidgetFadeTimer);
			m_VoiceLevelTimers.Set(VoiceLevelTalk, new WidgetFadeTimer);
			m_VoiceLevelTimers.Set(VoiceLevelShout, new WidgetFadeTimer);
		}
		
		HideVoiceLevelWidgets();
	}

	// ------------------------------------------------------------
	// OnMissionStart
	// ------------------------------------------------------------
	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnMissionStart - Start");
		#endif

		if ( !GetGame().IsMultiplayer() )
		{
			// GetDayZExpansion().OnMissionStart();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnMissionStart - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnMissionLoaded - Start");
		#endif
		
		if ( !GetGame().IsMultiplayer() )
		{
			// GetDayZExpansion().OnMissionLoaded();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnMissionLoaded - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnResizeScreen
	// ------------------------------------------------------------
	void OnResizeScreen()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnResizeScreen - Start");
		#endif

		m_Chat.Init( m_ChatRootWidget );

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnResizeScreen - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnUpdate - Start");
		#endif

		super.OnUpdate( timeslice );

		if ( !m_bLoaded )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MissionGameplay::OnUpdate - End");
			#endif

			return;
		}
		
		// GetDayZExpansion().OnUpdate( timeslice );

		//! Checking for keyboard focus
		bool inputIsFocused = false;
		
		//! Reference to focused windget
		Widget focusedWidget = GetFocus();

		if ( focusedWidget )
		{
			if ( focusedWidget.ClassName().Contains( "EditBoxWidget" ) )
			{
				inputIsFocused = true;
			} 
			else if ( focusedWidget.ClassName().Contains( "MultilineEditBoxWidget" ) )
			{
				inputIsFocused = true;
			}
		}

		//! Refernce to man
		Man man = GetGame().GetPlayer();

		//! Reference to input
		Input input = GetGame().GetInput();

		//! Expansion reference to menu
		UIScriptedMenu topMenu = m_UIManager.GetMenu();

		//! Expansion reference to player
		PlayerBase playerPB = PlayerBase.Cast( man );
		
		if ( playerPB && playerPB.GetHumanInventory() ) 
		{
			//! Expansion reference to item in hands
			ItemBase itemInHands = ItemBase.Cast(playerPB.GetHumanInventory().GetEntityInHands());

			//! Expansion reference to hologram
			ref Hologram hologram;	

			if ( playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious() )
			{
				//TODO: Make ExpansionInputs class and handle stuff there to keep this clean

				//! Chat
				if ( input.LocalPress( "UAChat", false ) && !inputIsFocused && !topMenu )
				{
					ShowChat();
				}
				
				//! Switch Chat Channel
				if ( input.LocalPress( "UAExpansionChatSwitchChannel", false ) && !inputIsFocused )
				{
					SwitchChannel();

					exp_m_ChannelNameFadeTimer.FadeIn(m_ChatChannelName, EXP_FADE_IN_DURATION);
					exp_m_ChannelNameTimeoutTimer.Run(EXP_FADE_TIMEOUT, exp_m_ChannelNameFadeTimer, "FadeOut", new Param2<Widget, float>(m_ChatChannelName, EXP_FADE_OUT_DURATION));

					exp_m_ChannelFadeTimer.FadeIn(m_WidgetChatChannel, EXP_FADE_IN_DURATION);
					exp_m_ChannelTimeoutTimer.Run(EXP_FADE_TIMEOUT, exp_m_ChannelFadeTimer, "FadeOut", new Param2<Widget, float>(m_WidgetChatChannel, EXP_FADE_OUT_DURATION));
				}

				if ( !topMenu && !inputIsFocused )
				{
					//! Autorun
					if ( input.LocalPress( "UAExpansionAutoRunToggle", false ) )
					{
						if ( !man.GetParent() && GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableAutoRun )
						{
							m_AutoRunModule.AutoRun();
						}
					}
					
					//! Book Menu
					if ( input.LocalPress( "UAExpansionBookToggle", false ) )
					{
						if ( !GetGame().GetUIManager().GetMenu() && GetExpansionSettings() && GetExpansionSettings().GetBook().EnableBook )
						{
							GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_BOOK_MENU, NULL );
						}
					}
					
					//! Map Menu
					if ( input.LocalPress( "UAExpansionMapToggle", false ) )
					{
						ExpansionMapMenu map_menu;
						if ( Class.CastTo( map_menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_MAP ) ) )
						{
							map_menu.Hide();
							map_menu.Close();
						} else if ( !GetGame().GetUIManager().GetMenu() && GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().CanOpenMapWithKeyBinding )
						{
							if ( GetExpansionSettings().GetGeneral().NeedMapItemForKeyBinding )
							{
								if ( PlayerBase.Cast( GetGame().GetPlayer() ).HasItemMap() || PlayerBase.Cast( GetGame().GetPlayer() ).HasItemGPS() )
									GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_MAP, NULL );
							} else
							{
								GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_MAP, NULL );
							}
						}
					}
					
					//! GPS	
					if ( input.LocalPress( "UAExpansionGPSToggle", false ) )
					{
						#ifdef EXPANSIONEXLOGPRINT
						EXLogPrint("MissionGameplay::OnUpdate - UAExpansionGPSToggle pressed and setting for item is: " + GetExpansionSettings().GetGeneral().NeedGPSItemForKeyBinding.ToString() );
						#endif

						if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableHUDGPS/*&& m_ExpansionHud.IsInitialized() && m_ExpansionHud.GetGPSState()*/ )
						{		
							if ( GetExpansionSettings().GetGeneral().NeedGPSItemForKeyBinding )
							{
								#ifdef EXPANSIONEXLOGPRINT
								EXLogPrint("MissionGameplay::OnUpdate - UAExpansionGPSToggle pressed and player has gps: " + PlayerBase.Cast( GetGame().GetPlayer() ).HasItemGPS().ToString() );
								#endif
								
								if ( PlayerBase.Cast( GetGame().GetPlayer() ).HasItemGPS() )
									ToggleHUDGPSMode();
							}
							else
							{
								ToggleHUDGPSMode();
							}
						}
					}
					
					if ( input.LocalPress( "UAExpansionGPSMapScaleDown", false ) )
					{
						if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableHUDGPS && m_ExpansionHud.IsInitialized() && m_ExpansionHud.GetGPSMapState() )
						{							
							DecreaseGPSMapScale();
						}
					}
					
					if ( input.LocalPress( "UAExpansionGPSMapScaleUp", false ) )
					{
						if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableHUDGPS && m_ExpansionHud.IsInitialized() && m_ExpansionHud.GetGPSMapState() )
						{
							IncreaseGPSMapScale();
						}
					}
					
					//! Player List Menu
					if ( input.LocalPress( "UAExpansionPlayerListToggle", false ) )
					{
						if ( !GetGame().GetUIManager().GetMenu() && GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnablePlayerList )
						{
							GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_PLAYER_LIST_MENU, NULL );
						}
					}
					
					//! Expansion Hud
					if ( input.LocalHold( "UAUIQuickbarToggle", false ) )
					{
						if ( !m_Hud.GetHudState() )
						{
							m_ExpansionHud.ShowHud( false );
						}
						else
						{
							m_ExpansionHud.ShowHud( true );
						}
					}
					
					//! Gestures
					if ( input.LocalPress( "UAUIGesturesOpen",false ) )
					{
						//! Open gestures menu
						if ( !playerPB.IsRaised() && !playerPB.GetCommand_Vehicle() )
						{
							if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
							{
								m_ExpansionHud.ShowHud( false );
							}
						}
					}
					
					//! Toggle Earplugs
					if ( input.LocalPress( "UAExpansionEarplugsToggle", false ) )
					{
						m_ExpansionHud.ToggleEarplugs();
					}

					if (m_MarkerModule)
					{						
						PlayerBase player = PlayerBase.Cast( g_Game.GetPlayer() );
						
						if (input.LocalPress("UAExpansion3DMarkerToggle", false)) {
							
							m_MarkerToggleState = !m_MarkerToggleState;
							m_PartyMarkerToggleState = m_MarkerToggleState;
							
							if (m_MarkerToggleState) {
								GetNotificationSystem().CreateNotification(new StringLocaliser("STR_EXPANSION_MARKERTOGGLE_TITLE"), new StringLocaliser("STR_EXPANSION_MARKERTOGGLEALL_OFF"), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5, player.GetIdentity());
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
								
							} else {
								GetNotificationSystem().CreateNotification(new StringLocaliser("STR_EXPANSION_MARKERTOGGLE_TITLE"), new StringLocaliser("STR_EXPANSION_MARKERTOGGLEALL_ON"), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5, player.GetIdentity());
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
							}
						}
						
						if (input.LocalPress("UAExpansionOnlyPartyMembersMarkersToggle", false)) {
							
							m_PartyMarkerToggleState = !m_PartyMarkerToggleState;
							
							if (m_PartyMarkerToggleState) {
								GetNotificationSystem().CreateNotification(new StringLocaliser("STR_EXPANSION_MARKERTOGGLE_TITLE"), new StringLocaliser("STR_EXPANSION_MARKERTOGGLEPARTY_OFF"), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5, player.GetIdentity());
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
								
							} else {
								
								GetNotificationSystem().CreateNotification(new StringLocaliser("STR_EXPANSION_MARKERTOGGLE_TITLE"), new StringLocaliser("STR_EXPANSION_MARKERTOGGLEPARTY_ON"), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5, player.GetIdentity());
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
							}
						}
					}
				}

				//! Basebuilding Snaping
				if ( playerPB && playerPB.IsPlacingLocal() && !inputIsFocused )
				{
					hologram = playerPB.GetHologramLocal();

					if ( hologram )
					{
						if ( input.LocalPress( "UAExpansionSnappingToggle" ) )
						{
							hologram.SetUsingSnap( !hologram.IsUsingSnap() );
						}

						if ( input.LocalValue( "UAExpansionSnappingDirectionNext" ) != 0 )
						{
							hologram.NextDirection();
						}

						if ( input.LocalValue( "UAExpansionSnappingDirectionPrevious" ) != 0 )
						{
							hologram.PreviousDirection();
						}
					}
				}

				if ( m_AutoRunModule )
				{
					//! Autowalk
					if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableAutoRun )
					{
						m_AutoRunModule.UpdateAutoWalk();
					}
					
					//! Stop autorun when different inputs are pressed
					if ( !m_AutoRunModule.IsDisabled() )
					{
						if ( INPUT_FORWARD() || INPUT_BACK() || INPUT_LEFT() || INPUT_RIGHT() || INPUT_GETOVER() || INPUT_STANCE() )
						{
							m_AutoRunModule.AutoRun();
						}
					}
				}
						
				//! Data
				if ( !m_DataSent ) 
				{
					ExpansionPlayerData();
					m_DataSent = true;
				}
			}
		}
		
		if ( playerPB )
		{
			HumanCommandVehicle hcv = playerPB.GetCommand_Vehicle();
			if ( hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				CarScript carScript = CarScript.Cast( hcv.GetTransport() );
				if ( carScript )
				{
					carScript.UpdateExpansionController();
				}
			}

			ExpansionHumanCommandVehicle ehcv = playerPB.GetCommand_ExpansionVehicle();
			if ( ehcv && ehcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				ExpansionVehicleScript expansionVehicleScript = ExpansionVehicleScript.Cast( ehcv.GetTransport() );
				if ( expansionVehicleScript )
				{
					expansionVehicleScript.UpdateExpansionController();
				}
			}
		
			if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableHUDNightvisionOverlay )
			{
				PlayerCheckNV( playerPB );
			}
		}
		
		//! Toggle HUD elements in different menus
		if ( m_Hud && m_ExpansionHud.IsInitialized() && m_Chat && GetCommunityOnlineTools() )
			RefreshHUDElements();
		
		//! Expansion hud update
		if ( m_Hud &&  m_ExpansionHud.IsInitialized() )
			m_ExpansionHud.Update( timeslice );
		
		//! Chat update
		m_Chat.Update( timeslice );

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion PlayerCheckNV
	// ------------------------------------------------------------
	void PlayerCheckNV(PlayerBase player)
	{		
		if ( !GetGame() )
			return;
		
		if ( player && player.GetCurrentCamera() )
		{
			private DayZPlayerCameraBase camera = DayZPlayerCameraBase.Cast( GetGame().GetPlayer().GetCurrentCamera() );
			if ( camera )
			{
				if ( camera && camera.IsCameraNV() ) 
				{
					if ( !m_ExpansionHud.GetNVState() )
				 		m_ExpansionHud.ShowNV( true );
				}
				else
				{
					if ( m_ExpansionHud.GetNVState() )
				 		m_ExpansionHud.ShowNV( false );
				}
			}
		}
		
		EntityAI entity;
		NVGoggles googles;
		ItemBase headgear;
		ItemBase eyewear;
		ItemBase handitem;
		
		if (player && player.FindAttachmentBySlotName("Headgear") != null)
			headgear = ItemBase.Cast(player.FindAttachmentBySlotName("Headgear"));
		
		if (player && player.FindAttachmentBySlotName("Eyewear") != null)
			eyewear = ItemBase.Cast(player.FindAttachmentBySlotName("Eyewear"));
		
		if (player && player.GetHumanInventory().GetEntityInHands() != null)
			handitem = ItemBase.Cast( player.GetHumanInventory().GetEntityInHands() );
		
		// Nvg - Headgear check
		if ( headgear )
		{
			entity = headgear.FindAttachmentBySlotName("NVG");
			if (entity)
			{
				Class.CastTo(googles, entity);
				GetNVBatteryState( googles );
			}
		}
		// Nvg - Eyewear check
		if ( eyewear )
		{
			entity = eyewear.FindAttachmentBySlotName("NVG");
			if (entity)
			{
				Class.CastTo(googles, entity);
				GetNVBatteryState( googles );
			}
		}
		// Nvg - In hands check
		if ( handitem )
		{
			entity = handitem;
			if (entity)
			{
				Class.CastTo(googles, entity);
				GetNVBatteryState( googles );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion PlayerCheckNV
	// ------------------------------------------------------------
	void GetNVBatteryState(NVGoggles googles)
	{
		if ( GetGame().IsClient() )
		{
			int energy_percent = 0;
			if ( googles && googles.GetCompEM().CanWork() )
			{
				energy_percent = googles.GetBatteryEnergy();					
				m_ExpansionHud.SetNVBatteryState( energy_percent );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetExpansionHud
	// ------------------------------------------------------------
	ExpansionIngameHud GetExpansionHud()
	{ 
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::GetExpansionHud");
		#endif

		return m_ExpansionHud;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPlayerData
	// ------------------------------------------------------------
	void ExpansionPlayerData()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ExpansionPlayerData - Start");
		#endif
		
		Man man = GetGame().GetPlayer();
		PlayerBase player = PlayerBase.Cast(man);
		
		//! Offline Check?!
		if ( !GetPermissionsManager().GetClientPlayer() )
		{
			m_DataSent = true;
			return;
		}

		string guid = "OFFLINE";

		if ( player.GetIdentity() )
		{
			guid = player.GetIdentityUID();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ExpansionPlayerData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnKeyPress
	//! Includes key-press commands for some inputs in menus
	// ------------------------------------------------------------
	override void OnKeyPress( int key )
	{
		super.OnKeyPress( key );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnKeyPress - Start");
		#endif

		m_Hud.KeyPress( key );
		
		ExpansionLockUIBase menu;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_NUMPAD_MENU ) ) )
			menu.OnKeyPress( key );
		
		menu = NULL;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_CODELOCK_MENU ) ) )
			menu.OnKeyPress( key );
		
		switch (key)
		{
			case KeyCode.KC_PRIOR:
			{
				if (m_ExpansionHud.GetEarplugsState())
				{
					GetExpansionClientSettings().EarplugLevel = Math.Clamp( GetExpansionClientSettings().EarplugLevel + 0.01, 0.0, 1.0 );
					GetExpansionClientSettings().Save();
					
					m_ExpansionHud.UpdateEarplugs();
				}
					
				break;
			}
			
			case KeyCode.KC_NEXT:
			{
				if (m_ExpansionHud.GetEarplugsState())
				{
					GetExpansionClientSettings().EarplugLevel = Math.Clamp( GetExpansionClientSettings().EarplugLevel - 0.01, 0.0, 1.0 );
					GetExpansionClientSettings().Save();
					
					m_ExpansionHud.UpdateEarplugs();
				}
				
				break;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnKeyPress - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion INPUT_FORWARD
	// ------------------------------------------------------------
	bool INPUT_FORWARD()
	{
   		return GetGame().GetInput().LocalPress( "UAMoveForward", false );
	}
	
	// ------------------------------------------------------------
	// Expansion INPUT_BACK
	// ------------------------------------------------------------
	bool INPUT_BACK()
	{
   		return GetGame().GetInput().LocalPress( "UAMoveBack", false );
	}
	
	// ------------------------------------------------------------
	// Expansion INPUT_S
	// ------------------------------------------------------------
	bool INPUT_LEFT()
	{
   		return GetGame().GetInput().LocalPress( "UAMoveLeft", false );
	}
	
	// ------------------------------------------------------------
	// Expansion INPUT_RIGHT
	// ------------------------------------------------------------
	bool INPUT_RIGHT()
	{		
   		return GetGame().GetInput().LocalPress( "UAMoveRight", false );
	}
		
	// ------------------------------------------------------------
	// Expansion INPUT_GETOVER
	// ------------------------------------------------------------
	bool INPUT_GETOVER()
	{
   		return GetGame().GetInput().LocalPress( "UAGetOver", false );
	}
	
	// ------------------------------------------------------------
	// Expansion INPUT_STANCE
	// ------------------------------------------------------------
	bool INPUT_STANCE()
	{
   		return GetGame().GetInput().LocalPress( "UAStance", false );
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshHUDElements
	//! Hides/Shows HUD elements in different menus
	// ------------------------------------------------------------
	void RefreshHUDElements()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::RefreshHUDElements - Start");
		#endif
		
		UIManager m_ExUIManager = GetGame().GetUIManager();
		bool isOpenInGameMenu = m_ExUIManager.IsMenuOpen(MENU_INGAME);
		bool isOpenInventoryMenu = m_ExUIManager.IsMenuOpen(MENU_INVENTORY);
		bool isOpenInspectMenu = m_ExUIManager.IsMenuOpen(MENU_INSPECT);
		bool isOpenMapMenu = m_ExUIManager.IsMenuOpen(MENU_EXPANSION_MAP);
		bool isOpenMarketMenu = m_ExUIManager.IsMenuOpen(MENU_EXPANSION_MARKET_MENU);
		bool isOpenBookMenu = m_ExUIManager.IsMenuOpen(MENU_EXPANSION_BOOK_MENU);
		bool isOpenCOTMenu = GetCommunityOnlineTools().IsOpen();
		bool isChatVisible = m_ChatRootWidget.IsVisible();
		bool isOpenPlayerListMenu = m_ExUIManager.IsMenuOpen(MENU_EXPANSION_PLAYER_LIST_MENU);
		
		if (isOpenInGameMenu || isOpenInventoryMenu || isOpenInspectMenu || isOpenMapMenu || isOpenMarketMenu || isOpenCOTMenu || isOpenPlayerListMenu)
		{
			ShowExpansionHudElements(false);
			if (isOpenMapMenu || isOpenMarketMenu || isOpenCOTMenu || isOpenPlayerListMenu)
			{
				ShowHudElements(false);
			}
		}
		else if ( (!isOpenInGameMenu || !isOpenInventoryMenu || !isOpenInspectMenu || !isOpenMapMenu || !isOpenMarketMenu || !isOpenCOTMenu) && !m_UIManager.GetMenu() || !isOpenPlayerListMenu)
		{
			ShowExpansionHudElements(true);
			if (!isOpenMapMenu || !isOpenMarketMenu || !isOpenCOTMenu || !isOpenPlayerListMenu)
			{
				ShowHudElements(true);
			}
		}
		
		//m_ChatRootWidget.Show( m_ExpansionChatState );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::RefreshHUDElements - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion DecreaseGPSMapScale
	// ------------------------------------------------------------
	void DecreaseGPSMapScale()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::DecreaseGPSMapScale - Start");
		#endif
		
		if ( m_ExpansionHud )
		{
			float current_scale;
			float new_scale;
			
			current_scale = m_ExpansionHud.GetCurrentGPSMapScale();
			new_scale = ( current_scale - 0.1 );
			
			if (new_scale <= 0.1)
				return;
			
			m_ExpansionHud.SetGPSMapScale( new_scale );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::DecreaseGPSMapScale - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion IncreaseGPSMapScale
	// ------------------------------------------------------------
	void IncreaseGPSMapScale()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::IncreaseGPSMapScale - Start");
		#endif

		if ( m_ExpansionHud )
		{		
			float current_scale;
			float new_scale;
			
			current_scale = m_ExpansionHud.GetCurrentGPSMapScale();
			new_scale = ( current_scale + 0.1 );
			
			if ( new_scale >= 0.8 )
				return;
			
			m_ExpansionHud.SetGPSMapScale( new_scale );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::IncreaseGPSMapScale - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ToggleHUDGPSMode
	//! Changes GPS Mode
	// ------------------------------------------------------------
	void ToggleHUDGPSMode()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("MissionGameplay::ToggleHUDGPSMode - Start");
		#endif

		if ( !m_ExpansionHud.GetGPSMapState() && m_ExpansionHud.GetGPSMapStatsState() )
		{
			m_ExpansionHud.ShowGPSMap( true );
			m_ExpansionHud.ShowGPSMapStats( false );
		}
		else if ( m_ExpansionHud.GetGPSMapState() && !m_ExpansionHud.GetGPSMapStatsState() )
		{	
			m_ExpansionHud.ShowGPSMap( false );
			m_ExpansionHud.ShowGPSMapStats( true );
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("MissionGameplay::ToggleHUDGPSMode - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ShowHudElements
	// ------------------------------------------------------------
	void ShowHudElements(bool show)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ShowHudElements - Start");
		#endif
	
		if ( m_Hud )
		{
			m_Hud.ShowHudUI( show );
			m_Hud.ShowQuickbarUI( show );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ShowHudElements - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ShowExpansionHudElements
	// ------------------------------------------------------------
	void ShowExpansionHudElements(bool show)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ShowHudElements - Start");
		#endif
		
		if ( m_ExpansionHud )
		{
			if ( !show )
			{
				m_ExpansionHud.ShowDebugger( false );
				
				if ( m_ExpansionHud.GetGPSState() )
				{
					m_ExpansionHud.ShowGPS( false );
					m_WasGPSToggled = true;
				}
			} 
			else 
			{
				m_ExpansionHud.ShowDebugger( true );				
				if ( m_WasGPSToggled && !m_ExpansionHud.GetEarplugsState() && !GetGame().GetUIManager().GetMenu() )
				{
					m_ExpansionHud.ShowGPS( true );
					m_WasGPSToggled = false;
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ShowHudElements - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetChat
	// ------------------------------------------------------------
	ref Chat GetChat()
	{
		return m_Chat;
	}
	
	// ------------------------------------------------------------
	// Override UpdateVoiceLevelWidgets
	// ------------------------------------------------------------
	override void UpdateVoiceLevelWidgets(int level)
	{
		for( int n = 0; n < m_VoiceLevelsWidgets.Count(); n++ )
		{
			int voiceKey = m_VoiceLevelsWidgets.GetKey(n);
			ImageWidget voiceWidget = m_VoiceLevelsWidgets.Get(n);
			
			// stop fade timer since it will be refreshed
			ref WidgetFadeTimer timer = m_VoiceLevelTimers.Get(n);		
			timer.Stop();
		
			// show widgets according to the level
			if( voiceKey <= level )
			{
				voiceWidget.SetAlpha(1.0); // reset from possible previous fade out 
				voiceWidget.Show(true);
				
				if( !m_VoNActive && !GetUIManager().FindMenu(MENU_CHAT_INPUT) ) 	
					timer.FadeOut(voiceWidget, 3.0);	
			}
			else
				voiceWidget.Show(false);
		}
		
		// fade out microphone icon when switching levels without von on
		if( !m_VoNActive )
		{
		  	if( !GetUIManager().FindMenu(MENU_CHAT_INPUT) )
			{
				m_MicrophoneIcon.SetAlpha(1.0); 
				m_MicrophoneIcon.Show(true);
				
				m_MicFadeTimer.FadeOut(m_MicrophoneIcon, 3.0);
				m_SeperatorFadeTimer.FadeOut(m_VoiceLevelSeperator, 3.0);
			}
		}
		else
		{
			// stop mic icon fade timer when von is activated
			m_MicFadeTimer.Stop();
			m_SeperatorFadeTimer.Stop();
		}
	}
}