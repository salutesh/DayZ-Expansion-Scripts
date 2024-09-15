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
	protected bool m_ExpansionUseChat;
	
	void Chat()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Chat");
	#endif
	}
	
	void Expansion_UseChat(bool state)
	{
		m_ExpansionUseChat = state;
		if (m_ExpansionUseChat)
		{
			if (m_RootWidget && !m_ExChatUI)
			{
				m_ExChatUI = new ExpansionChatUIWindow(m_RootWidget, this);
				m_ExChatUI.Hide();
			}
		}
	}

	void OnChatInputShow()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "OnChatInputShow");
		#endif
		
		if (m_ExChatUI)
			m_ExChatUI.Show();
	}

	void OnChatInputHide()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "OnChatInputHide");
	#endif

		if (m_ExChatUI)
			m_ExChatUI.Hide();
	}

	void HideChatToggle()
	{
		GetExpansionClientSettings().HUDChatToggle = !GetExpansionClientSettings().HUDChatToggle;

		if (m_ExChatUI)
			m_ExChatUI.GetLayoutRoot().Show(GetExpansionClientSettings().HUDChatToggle);
	}

	override void Add(ChatMessageEventParams params)
	{
		if (!m_ExpansionUseChat)
		{
			super.Add(params);
			return;
		}

		ExpansionChatMessageEventParams exParams;
		if (CanMute(params.param1) && Class.CastTo(exParams, params) && IsPlayerMuted(exParams.param5))
			return;

		if (m_ExChatUI)
			m_ExChatUI.Add(params);
	}

	override void AddInternal(ChatMessageEventParams params)
	{
		if (!m_ExpansionUseChat)
		{
			super.AddInternal(params);
			return;
		}

		ExpansionChatMessageEventParams exParams;
		if (CanMute(params.param1) && Class.CastTo(exParams, params) && IsPlayerMuted(exParams.param5))
			return;

		if (m_ExChatUI)
			m_ExChatUI.AddInternal(params);
	}

	override void Clear()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Clear");
	#endif

		//! DON'T clear chat lines when expansion chat is enabled
		if (m_ExpansionUseChat)
			return;
		
		super.Clear();
	}

	bool IsPlayerMuted(string uid)
	{
		if (!uid)
			return false;

		ExpansionClientSettings clientSettings = GetExpansionClientSettings();

		if (clientSettings.MutedPlayers.Find(uid) > -1)
			return true;

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