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
	//! Autorun module
	ref ExpansionAutorunModule 					m_AutoRunModule;
	
	//! Client settings
	protected bool								m_ClientClockShow;
	protected bool								m_ClientHumanityShow;

	protected bool 								m_WasEarplugToggled;
	//! Client/Player Data
	protected bool 								m_DataSent;
	
	Widget										m_ChatRootWidget;
	
	protected bool								m_WasGPSToggled;
	protected bool								m_ExpansionChatState;
	
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

		if ( !IsMissionOffline() )
			g_exGlobalSettings.Unload();

		m_AutoRunModule = ExpansionAutorunModule.Cast( GetModuleManager().GetModule( ExpansionAutorunModule ) );
		
		m_DataSent = false;
		
		m_ExpansionChatState = true;
		
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
			SwitchChatChannelToAdmin();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::SwitchChatChannelToGlobal - End");
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
		
		// SetFocus( m_ChatRootWidget.FindAnyWidget("ScrollerContainer") );
		
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
		// Print("Disabling Controls");

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
		
		//! EXPANSION HUD
		if ( !m_ExpansionHudRootWidget )
		{
			m_ExpansionHudRootWidget = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/hud/expansion_hud.layout");
			m_ExpansionHudRootWidget.Show( false );
			
			m_ExpansionHud = new ExpansionIngameHud;
			m_ExpansionHud.Init( m_ExpansionHudRootWidget );
			
			if (m_Hud && m_ExpansionHud)
				m_Hud.SetExpansionHUD(true);
		}
		
		//! CHAT
		if ( !m_ChatRootWidget )
		{			
			Widget ChatFrameWidget = m_HudRootWidget.FindAnyWidget("ChatFrameWidget");
			
			m_ChatRootWidget = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/chat/expansion_chat_panel.layout", ChatFrameWidget );
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnInit - End");
		#endif
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
						if ( !GetGame().GetUIManager().GetMenu() && GetExpansionSettings() && GetExpansionSettings().GetMap().CanOpenMapWithKeyBinding )
						{
							if ( GetExpansionSettings().GetGeneral().NeedMapItemForKeyBinding )
							{
								if ( PlayerBase.Cast( GetGame().GetPlayer() ).HasItemMap() || PlayerBase.Cast( GetGame().GetPlayer() ).HasItemGPS() )
									GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_MAP, NULL );
							}
							else
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
									ToogleHUDGPSMode();
							}
							else
							{
								ToogleHUDGPSMode();
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
					
					//! Toogle chat window
					if ( input.LocalPress( "UAExpansionChatToggle", false ) )
					{
						ToggleChat();
					}
					
					//! Toogle 3d marker visiblity
					if ( input.LocalPress( "UAExpansion3DMarkerToggle", false ) )
					{
						Expansion3DMarkerModule module3DMarker;
						if ( Class.CastTo( module3DMarker, GetModuleManager().GetModule( Expansion3DMarkerModule ) ) )
						{
							module3DMarker.ToggleShowMaarkers();
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
						
				//! Data
				if (!m_DataSent) 
				{
					ExpansionPlayerData();
					m_DataSent = true;
				}
			}
		}
		
		if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableHUDNightvisionOverlay )
		{
			//! Nightvision Overlay
			if ( playerPB && playerPB.GetCurrentCamera() )
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
			
			if ( playerPB && playerPB.GetInventory() )
			{
				NVGoggles nvgoogles = NVGoggles.Cast( playerPB.GetHumanInventory().GetEntityInHands() );
				if ( nvgoogles )
				{
					if ( nvgoogles.IsWorking() && nvgoogles.IsInOptics() )
					{
						if ( !m_ExpansionHud.GetNVState() )
							m_ExpansionHud.ShowNV( true );
					}
					else
					{
						if ( m_ExpansionHud.GetNVState() )
							m_ExpansionHud.ShowNV( true );
					}
				}
			}
		}

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			//! Get Client Settings from player and set ui Elemets
			CheckClientSettings();
		}
		
		//! Toggle HUD elements in different menus
		if ( m_Hud && m_ExpansionHud.IsInitialized() && m_Chat && GetCommunityOnlineTools() )
			RefreshHUDElements();

		m_ExpansionHud.Update( timeslice );
		
		m_Chat.Update( timeslice );

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnUpdate - End");
		#endif
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
	// Expansion CheckClientSettings
	// ------------------------------------------------------------
	void CheckClientSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::CheckClientSettings - Start");
		#endif

		// m_ClientClockShow = GetExpansionClientSettings().HUDShowClientClock;
		// m_ClientHumanityShow = GetExpansionClientSettings().HUDShowClientHumanity;

		if ( GetGame().IsMultiplayer() )
		{
			// GetGame().GetWorld().SetPreferredViewDistance(GetExpansionClientSettings().DrawDistance);
			// GetGame().GetWorld().SetPreferredViewDistance(1600.0);
		}

		// ToggleHUDClock();
		// ToggleHUDHumanity();

		PPEffects.UpdateSaturation();
		PPEffects.UpdateVignette();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::CheckClientSettings - End");
		#endif
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
					m_ExpansionHud.AddEarplugsVolume(0.01);
				break;
			}
			
			case KeyCode.KC_NEXT:
			{
				if (m_ExpansionHud.GetEarplugsState())
					m_ExpansionHud.AddEarplugsVolume(-0.01);
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

		if (isOpenInGameMenu || isOpenInventoryMenu || isOpenInspectMenu || isOpenMapMenu || isOpenMarketMenu || isOpenCOTMenu)
		{
			ShowExpansionHudElements(false);
			if (isOpenMapMenu || isOpenMarketMenu || isOpenCOTMenu)
			{
				ShowHudElements(false);
			}
		}
		else if ( (!isOpenInGameMenu || !isOpenInventoryMenu || !isOpenInspectMenu || !isOpenMapMenu || !isOpenMarketMenu || !isOpenCOTMenu) && !m_UIManager.GetMenu() )
		{
			ShowExpansionHudElements(true);
			if (!isOpenMapMenu || !isOpenMarketMenu || !isOpenCOTMenu)
			{
				ShowHudElements(true);
			}
		}
		
		if ( m_ExpansionChatState )
			m_ChatRootWidget.Show( true );
		else
			m_ChatRootWidget.Show( false );
		
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
	// Expansion ToogleHUDGPSMode
	//! Changes GPS Mode
	// ------------------------------------------------------------
	void ToogleHUDGPSMode()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("MissionGameplay::ToogleHUDGPSMode - Start");
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
		EXLogPrint("MissionGameplay::ToogleHUDGPSMode - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion ToggleHUDClock
	//! Hides/Shows HUD Clock elements
	// ------------------------------------------------------------
	void ToggleHUDClock()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ToggleHUDClock - Start");
		#endif

		//! Clock Toggle
		if ( m_ClientClockShow )
		{
			m_ExpansionHud.ClockToggle( true );
		} 
		else
		{
			m_ExpansionHud.ClockToggle( false );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ToggleHUDClock - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ToggleHUDHumanity
	//! Hides/Shows HUD Humanity notifier elements
	// ------------------------------------------------------------
	void ToggleHUDHumanity()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ToggleHUDHumanity - Start");
		#endif

		//! Humanity Toggle
		if ( m_ClientHumanityShow )
		{
			m_ExpansionHud.HumanityToggle( true );
		}
		else
		{
			m_ExpansionHud.HumanityToggle( false );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::ToggleHUDHumanity - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ToggleChat
	// ------------------------------------------------------------
	void ToggleChat()
	{
		m_ExpansionChatState = !m_ExpansionChatState;
		
		RefreshHUDElements();
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
				m_ExpansionHud.ShowClock( false );
				m_ExpansionHud.ShowHumanity( false );
				
				//m_ExpansionChatState = false;
				//m_ChatRootWidget.Show( false );
				
				if ( m_ExpansionHud.GetGPSState() )
				{
					m_ExpansionHud.ShowGPS( false );
					m_WasGPSToggled = true;
				}
			} 
			else 
			{
				m_ExpansionHud.ShowDebugger( true );
				m_ExpansionHud.ShowClock( true );
				m_ExpansionHud.ShowHumanity( true );
				
				m_ChatRootWidget.Show( true );
				
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
}