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
	protected ExpansionChatChannels m_ChatChannel;
	protected Widget m_ChatPanel;
	protected Widget m_ChatChannelRootWidget;
	protected TextWidget m_ChatChannelName;

	void MissionGameplay()
	{
		ExpansionSettings.SI_Chat.Insert(Expansion_OnChatSettingsReceived);
	}

	void ~MissionGameplay()
	{
		ExpansionSettings.SI_Chat.Remove(Expansion_OnChatSettingsReceived);
	}

	override void OnInit()
	{
		super.OnInit();

		InitExpansionChat();
	}

	void InitExpansionChat()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		m_ChatPanel = Widget.Cast(m_HudRootWidget.FindAnyWidget("ChatFrameWidget"));
		if (m_Chat && m_ChatPanel)
		{
			m_ChatChannelRootWidget = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Chat/GUI/layouts/expansion_chat_channel.layout");
			m_ChatChannelName = TextWidget.Cast(m_ChatChannelRootWidget.FindAnyWidget("ChatChannelName"));

			//! Set default to direct
			SwitchChatChannelToDirect();
		}
	}

	void Expansion_OnChatSettingsReceived()
	{
		ExpansionSettings.SI_Chat.Remove(Expansion_OnChatSettingsReceived);

		ExpansionClientUIChatChannel chatChannel = GetExpansionClientSettings().DefaultChatChannel;
		switch (chatChannel)
		{
			case ExpansionClientUIChatChannel.GLOBAL:
				SwitchChatChannelToGlobal();
				break;
		}
	}

	override void ShowChat()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		//! Disable certain controlls and inputs when the chat input menu gets opened.
		PlayerControlDisable(INPUT_EXCLUDE_ALL);
		PlayerControlDisable(INPUT_EXCLUDE_CHAT_EXPANSION);
		GetUApi().GetInputByName("UAPersonView").Supress();
		GetUApi().GetInputByName("UAPersonView").ForceDisable(true);
		GetGame().GetUIManager().ShowUICursor(true);

		//! If we are no longer in a vehicle and last used channel was transport, switch to direct
		if (m_ChatChannel == ExpansionChatChannels.CCTransport)
		{
			PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
			Object parent = Object.Cast(player.GetParent());

			if (!parent || !parent.IsTransport() || !GetExpansionSettings().GetChat().EnableTransportChat)
				SwitchChatChannelToDirect();
		}

		UpdateChannelColor();

		//! Update mic voice level indicator widgets
		int level = GetGame().GetVoiceLevel();
		UpdateVoiceLevelWidgets(level);

		//! Open chat input menu
		m_UIManager.EnterScriptedMenu(MENU_CHAT_INPUT, NULL);

		//! Show chat channel widgets
		m_ChatChannelRootWidget.Show(true);
	}

	override void HideChat()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		super.HideChat();

		//! Enable inputs
		GetUApi().GetInputByName("UAPersonView").ForceDisable(false);

		m_ChatChannelRootWidget.Show(false);
	}

	void SwitchChatChannelToGlobal()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		if (GetExpansionSettings().GetChat().EnableGlobalChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCGlobal;
			m_ChatChannelName.SetText("Global Chat");
		}
		else
		{
			SwitchChatChannelToTeam();
		}
	}

	void SwitchChatChannelToTeam()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

#ifdef EXPANSIONMODGROUPS
		ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));

		if (partyModule.HasParty() && GetExpansionSettings().GetChat().EnablePartyChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCTeam;
			m_ChatChannelName.SetText("Team Chat");
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
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player || !player.GetParent())
		{
			SwitchChatChannelToAdmin();
			return;
		}

		Object parent = Object.Cast(player.GetParent());
		if (parent && parent.IsTransport() && GetExpansionSettings().GetChat().EnableTransportChat)
		{
			m_ChatChannel = ExpansionChatChannels.CCTransport;
			m_ChatChannelName.SetText("Transport Chat");
		}
		else
		{
			SwitchChatChannelToAdmin();
		}
	}

	void SwitchChatChannelToAdmin()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		if (GetPermissionsManager().HasPermission("Admin.Chat"))
		{
			m_ChatChannel = ExpansionChatChannels.CCAdmin;
			m_ChatChannelName.SetText("Admin Chat");
		}
		else
		{
			SwitchChatChannelToDirect();
		}
	}

	void SwitchChatChannelToDirect()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		m_ChatChannel = ExpansionChatChannels.CCDirect;
		m_ChatChannelName.SetText("Proximity Chat");
	}

	void SwitchChannel()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		m_ChatChannelHideTimer.Stop();
		m_ChatChannelRootWidget.Show(true);

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

		UpdateChannelColor();

		m_ChatChannelHideTimer.Run(2, m_ChatChannelRootWidget, "Show", new Param1<bool>(false));
	}

	void UpdateChannelColor()
	{
		//! Initialize chat channel name to correct color
		string chatChannelName = typename.EnumToString(ExpansionChatChannels, m_ChatChannel);
		if (chatChannelName == "CCTeam")
			chatChannelName = "Party";
		else
			chatChannelName = chatChannelName.Substring(2, chatChannelName.Length() - 2);
		m_ChatChannelName.SetColor(GetExpansionSettings().GetChat().ChatColors.Get(chatChannelName + "ChatColor"));
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

					if (GetExpansionClientSettings().HUDChatToggle)
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
				}
			}
		}
	}
};
