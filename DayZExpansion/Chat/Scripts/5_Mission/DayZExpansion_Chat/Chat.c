/**
 * ExpansionChat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Chat
{
	protected ref ExpansionChatUIWindow m_ExChatUI;

	void Chat()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Chat");
#endif
	}

	override void Init(Widget root_widget)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		m_RootWidget = root_widget;

		if (m_RootWidget && !m_ExChatUI)
		{
			m_ExChatUI = new ExpansionChatUIWindow(m_RootWidget, this);
			m_ExChatUI.Hide();
		}
	}

	void OnChatInputShow()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "OnChatInputShow");
#endif
		
		m_RootWidget.Show(true);
		m_ExChatUI.Show();
	}

	void OnChatInputHide()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "OnChatInputHide");
#endif

		m_RootWidget.Show(GetExpansionClientSettings().HUDChatToggle);
		m_ExChatUI.Hide();
	}

	void HideChatToggle()
	{
		GetExpansionClientSettings().HUDChatToggle = !GetExpansionClientSettings().HUDChatToggle;

		m_RootWidget.Show(GetExpansionClientSettings().HUDChatToggle);
		m_ExChatUI.GetLayoutRoot().Show(GetExpansionClientSettings().HUDChatToggle);
	}

	bool GetChatToggleState()
	{
		return GetExpansionClientSettings().HUDChatToggle;
	}

	override void Add(ChatMessageEventParams params)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Add");
#endif

		if (CanMute(params.param1) && IsPlayerMuted(params.param2))
			return;

		if (m_ExChatUI)
			m_ExChatUI.Add(params);
	}

	override void AddInternal(ChatMessageEventParams params)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "AddInternal");
#endif

		if (CanMute(params.param1) && IsPlayerMuted(params.param2))
			return;

		if (m_ExChatUI)
			m_ExChatUI.AddInternal(params);
	}

	override void Clear()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Clear");
#endif

		//! DON'T clear chat lines
	}

	bool IsPlayerMuted(string playerName)
	{
		ExpansionClientSettings clientSettings = GetExpansionClientSettings();

		if (clientSettings.MutedPlayers.Count() == 0)
			return false;

		foreach (SyncPlayer player: ClientData.m_PlayerList.m_PlayerList)
		{
			if (player.m_PlayerName == playerName)
			{
				return clientSettings.MutedPlayers.Find(player.m_RUID) > -1;
			}
		}

		return false;
	}

	bool CanMute(int channel)
	{
		switch (channel)
		{
			case ExpansionChatChannels.CCSystem:
			case ExpansionChatChannels.CCAdmin:
			case ExpansionChatChannels.CCBattlEye:
				return false;
		}

		return true;
	}

	ExpansionChatUIWindow GetChatWindow()
	{
		return m_ExChatUI;
	}
};