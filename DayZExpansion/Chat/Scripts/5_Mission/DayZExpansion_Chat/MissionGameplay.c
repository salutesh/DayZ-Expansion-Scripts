/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	Chat m_Chat;
	ExpansionChatChannels m_ChatChannel;
	
	ref WidgetFadeTimer m_ExpChatChannelFadeInTimer;
	ref Timer m_ExpChatChannelFadeOutTimer;
	
	ref WidgetFadeTimer m_ExpChatChannelNameFadeInTimer;
	ref Timer m_ExpChatChannelNameFadeOutTimer;
	
	Widget m_ChatChannelRootWidget;
	Widget m_WidgetChatChannel;
	TextWidget m_ChatChannelName;
	Widget m_ChatRootWidget;
	Widget m_ChatPanel;

	void MissionGameplay()
	{
		m_ChatRootWidget = null;
		
		m_ExpChatChannelFadeInTimer = new WidgetFadeTimer();
		m_ExpChatChannelFadeOutTimer = new Timer(CALL_CATEGORY_GUI);
		
		m_ExpChatChannelNameFadeInTimer = new WidgetFadeTimer();
		m_ExpChatChannelNameFadeOutTimer = new Timer(CALL_CATEGORY_GUI);
	}

	override void OnInit()
	{
		super.OnInit();
		
		InitChat();
	}
	
	void InitChat()
	{
		if (!m_ChatRootWidget && m_HudRootWidget)
		{
			m_ChatPanel = Widget.Cast(m_HudRootWidget.FindAnyWidget("ChatFrameWidget"));
			m_ChatPanel.SetFlags(WidgetFlags.IGNOREPOINTER, true);

			m_ChatRootWidget = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Chat/GUI/layouts/expansion_chat.layout", m_ChatPanel);
			m_Chat.Init(m_ChatRootWidget);

			if (m_Chat)
			{
				m_ChatChannelRootWidget = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Chat/GUI/layouts/expansion_chat_channel.layout");
				m_WidgetChatChannel = Widget.Cast(m_ChatChannelRootWidget.FindAnyWidget("ChatChannelPanel"));
				m_ChatChannelName = TextWidget.Cast(m_ChatChannelRootWidget.FindAnyWidget("ChatChannelName"));

				SwitchChatChannelToDirect();
			}
		}
	}

	override void ShowChat()
	{
		//! Disable fade-in/out timers while chat window is opened
		m_ExpChatChannelFadeInTimer.Stop();
		m_ExpChatChannelFadeOutTimer.Stop();
		m_ExpChatChannelNameFadeInTimer.Stop();
		m_ExpChatChannelNameFadeOutTimer.Stop();
		
		//! Disable certain controlls and inputs when the chat input menu gets opened.
		PlayerControlDisable(INPUT_EXCLUDE_ALL);
		PlayerControlDisable(INPUT_EXCLUDE_CHAT_EXPANSION);
		GetUApi().GetInputByName("UAPersonView").Supress();
		GetUApi().GetInputByName("UAPersonView").ForceDisable(true);
		GetGame().GetUIManager().ShowUICursor(true);
		
		//! Not sure why this is here?!
		if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
			return;

		//! If we are no longer in a vehicle and last used channel was transport, switch to direct
		if (m_ChatChannel == ExpansionChatChannels.CCTransport)
		{
			PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
			Object parent = Object.Cast(player.GetParent());

			if (!parent || !parent.IsTransport() || !GetExpansionSettings().GetChat().EnableTransportChat)
				SwitchChatChannelToDirect();
		}
		else if (m_ChatChannel == ExpansionChatChannels.CCDirect)
		{
			//! Initialize direct chat channel name to correct color
			m_ChatChannelName.SetColor(GetExpansionSettings().GetChat().ChatColors.Get("DirectChatColor"));
		}
				
		//! Fade-in chat channel name widgets
		FadeInChatChannel();
		
		//! Update mic voice level indicator widgets
		int level = GetGame().GetVoiceLevel();
		UpdateVoiceLevelWidgets(level);
		
		//! Open chat input menu
		m_UIManager.EnterScriptedMenu(MENU_CHAT_INPUT, NULL);
	}

	override void HideChat()
	{
		super.HideChat();

		//! Enable inputs
		GetUApi().GetInputByName("UAPersonView").ForceDisable(false);
	}
	
	void FadeInChatChannel()
	{
		m_ExpChatChannelFadeInTimer.FadeIn(m_WidgetChatChannel, EXP_FADE_IN_DURATION);
		m_ExpChatChannelNameFadeInTimer.FadeIn(m_ChatChannelName, EXP_FADE_IN_DURATION);
	}
	
	void FadeOutChatChannel()
	{
		m_ExpChatChannelFadeOutTimer.Run(EXP_FADE_TIMEOUT, m_ExpChatChannelFadeInTimer, "FadeOut", new Param2<Widget, float>(m_WidgetChatChannel, EXP_FADE_OUT_DURATION));
		m_ExpChatChannelNameFadeOutTimer.Run(EXP_FADE_TIMEOUT, m_ExpChatChannelNameFadeInTimer, "FadeOut", new Param2<Widget, float>(m_ChatChannelName, EXP_FADE_OUT_DURATION));
	}

	void SwitchChatChannelToGlobal()
	{
		if (GetExpansionSettings().GetChat().EnableGlobalChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCGlobal;
			m_ChatChannelName.SetText("Global Chat");
			m_ChatChannelName.SetColor(GetExpansionSettings().GetChat().ChatColors.Get("GlobalChatColor"));
		}
		else
		{
			SwitchChatChannelToTeam();
		}
	}

	void SwitchChatChannelToTeam()
	{
#ifdef EXPANSIONMODGROUPS
		ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));

		if (partyModule.HasParty() && GetExpansionSettings().GetChat().EnablePartyChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCTeam;
			m_ChatChannelName.SetText("Team Chat");
			m_ChatChannelName.SetColor(GetExpansionSettings().GetChat().ChatColors.Get("PartyChatColor"));
		}
		else
		{
			SwitchChatChannelToTransport();
		}
#else
		SwitchChatChannelToTransport();
#endif
	}

	void SwitchChatChannelToTransport()
	{
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		Object parent = Object.Cast(player.GetParent());

		if (parent && parent.IsTransport() && GetExpansionSettings().GetChat().EnableTransportChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCTransport;
			m_ChatChannelName.SetText("Transport Chat");
			m_ChatChannelName.SetColor(GetExpansionSettings().GetChat().ChatColors.Get("TransportChatColor"));
		}
		else
		{
			SwitchChatChannelToAdmin();
		}
	}

	void SwitchChatChannelToAdmin()
	{
		if (GetPermissionsManager().HasPermission("Admin.Chat"))
		{
			m_ChatChannel = ExpansionChatChannels.CCAdmin;
			m_ChatChannelName.SetText("Admin Chat");
			m_ChatChannelName.SetColor(GetExpansionSettings().GetChat().ChatColors.Get("AdminChatColor"));
		}
		else
		{
			SwitchChatChannelToDirect();
		}
	}

	void SwitchChatChannelToDirect()
	{
		m_ChatChannel = ExpansionChatChannels.CCDirect;
		m_ChatChannelName.SetText("Direct Chat");
		m_ChatChannelName.SetColor(GetExpansionSettings().GetChat().ChatColors.Get("DirectChatColor"));
	}

	void SwitchChannel()
	{
		FadeInChatChannel();
		
		switch (m_ChatChannel)
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
	}

	int GetChatChannel()
	{
		return m_ChatChannel;
	}

	Chat GetChat()
	{
		return m_Chat;
	}
	
	override void PlayerControlDisable(int mode)
	{
		switch (mode)
		{
			case INPUT_EXCLUDE_CHAT_EXPANSION:
			{
				GetUApi().ActivateExclude("chatexpansion");
				GetUApi().UpdateControls();
				break;
			}
		}

		super.PlayerControlDisable(mode);
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if (!m_bLoaded)
		{
			return;
		}

		//! Checking for keyboard focus
		bool inputIsFocused = false;

		//! Reference to focused windget
		Widget focusedWidget = GetFocus();

		if (focusedWidget)
		{
			if (focusedWidget.ClassName().Contains("EditBoxWidget"))
			{
				inputIsFocused = true;
			}
			else if (focusedWidget.ClassName().Contains("MultilineEditBoxWidget"))
			{
				inputIsFocused = true;
			}
		}

		Man player = GetGame().GetPlayer(); //! Reference to player
		Input input = GetGame().GetInput(); //! Reference to input
		UIScriptedMenu topMenu = m_UIManager.GetMenu(); //! Reference to current opened scripted menu
		ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu()); //! Reference to current opened script view menu

		if (player)
		{
			if (m_LifeState == EPlayerStates.ALIVE && !player.IsUnconscious())
			{
				//TODO: Make ExpansionInputs class and handle stuff there to keep this clean
				//! Open main chat input and window
				if (input.LocalPress("UAChat", false) && !inputIsFocused && !topMenu && !viewMenu)
				{
					ShowChat();
				}

				//! Hide chat toggle
				if (input.LocalPress("UAExpansionHideChatToggle", false) && !inputIsFocused && !topMenu && !viewMenu)
				{
					m_Chat.HideChatToggle();

					if (m_Chat.GetChatToggleState())
					{
						ExpansionNotification("STR_EXPANSION_CHATTOGGLE_TITLE", "STR_EXPANSION_CHATTOGGLE_ON", EXPANSION_NOTIFICATION_ICON_T_Walkie_Talkie, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(player.GetIdentity());
					}
					else
					{
						ExpansionNotification("STR_EXPANSION_CHATTOGGLE_TITLE", "STR_EXPANSION_CHATTOGGLE_OFF", EXPANSION_NOTIFICATION_ICON_T_Walkie_Talkie, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(player.GetIdentity());
					}
				}

				//! Switch chat channel
				if (input.LocalPress("UAExpansionChatSwitchChannel", false) && !inputIsFocused && !topMenu && !viewMenu)
				{
					SwitchChannel();
					FadeOutChatChannel();
				}
			}
		}

		m_Chat.Update(timeslice);
	}
};
