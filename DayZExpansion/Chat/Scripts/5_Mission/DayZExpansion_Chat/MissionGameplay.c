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
	protected bool m_ExpansionUseChat;

	void MissionGameplay()
	{
		ExpansionSettings.SI_Chat.Insert(Expansion_OnChatSettingsReceived);
	}

	void ~MissionGameplay()
	{
		ExpansionSettings.SI_Chat.Remove(Expansion_OnChatSettingsReceived);
	}

	void InitExpansionChat()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		m_ChatPanel = Widget.Cast(m_HudRootWidget.FindAnyWidget("ChatFrameWidget"));
		if (m_Chat && m_ChatPanel)
		{
			m_ChatChannelRootWidget = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Chat/GUI/layouts/expansion_chat_channel.layout");
			m_ChatChannelName = TextWidget.Cast(m_ChatChannelRootWidget.FindAnyWidget("ChatChannelName"));

			ExpansionClientUIChatChannel chatChannel = GetExpansionClientSettings().DefaultChatChannel;
			switch (chatChannel)
			{
				case ExpansionClientUIChatChannel.GLOBAL:
					SwitchChatChannelToGlobal();
					break;
				default:
					//! Set default to direct
					SwitchChatChannelToDirect();
					break;
			}
		}
		
		m_Chat.Expansion_UseChat(true);
	}

	void Expansion_OnChatSettingsReceived()
	{
		ExpansionSettings.SI_Chat.Remove(Expansion_OnChatSettingsReceived);
		
		auto settings = GetExpansionSettings().GetChat();
		
		m_ExpansionUseChat = settings.EnableGlobalChat;
		if (!m_ExpansionUseChat)
			m_ExpansionUseChat = settings.EnableTransportChat;
	#ifdef EXPANSIONMODGROUPS
		if (!m_ExpansionUseChat)
			m_ExpansionUseChat = settings.EnablePartyChat;
	#endif
		
		if (m_ExpansionUseChat)
			InitExpansionChat();
	}
	
	override void ShowChat()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		super.ShowChat();

		if (m_ExpansionUseChat)
		{
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

			//! Show chat channel widgets
			m_ChatChannelRootWidget.Show(true);
		}
	}

	override void HideChat()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		super.HideChat();

		if (m_ExpansionUseChat)
		{
			m_ChatChannelRootWidget.Show(false);
		}
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

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

		if (isAliveConscious && !inputIsFocused && !menu && !viewMenu && m_ExpansionUseChat)
		{
			//! Open main chat input and window
			if (input.LocalPress("UAChat", false))
			{
				ShowChat();
			}

			//! Hide chat toggle
			if (input.LocalPress("UAExpansionHideChatToggle", false))
			{
				m_Chat.HideChatToggle();

				if (GetExpansionClientSettings().HUDChatToggle)
				{
					ExpansionNotification("STR_EXPANSION_CHATTOGGLE_TITLE", "STR_EXPANSION_CHATTOGGLE_ON", EXPANSION_NOTIFICATION_ICON_T_Walkie_Talkie, COLOR_EXPANSION_NOTIFICATION_SUCCESS, 5).Info(player.GetIdentity());
				}
				else
				{
					ExpansionNotification("STR_EXPANSION_CHATTOGGLE_TITLE", "STR_EXPANSION_CHATTOGGLE_OFF", EXPANSION_NOTIFICATION_ICON_T_Walkie_Talkie, COLOR_EXPANSION_NOTIFICATION_SUCCESS, 5).Info(player.GetIdentity());
				}
			}

			//! Switch chat channel
			if (input.LocalPress("UAExpansionChatSwitchChannel", false))
			{
				SwitchChannel();
			}
		}
	}
};
