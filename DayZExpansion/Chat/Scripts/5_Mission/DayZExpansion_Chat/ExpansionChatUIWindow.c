/**
 * ExpansionChatUIWindow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionChatUIWindow: ExpansionScriptView
{
	private ref ExpansionChatUIWindowController m_ChatWindowController;
	private ref Chat m_Chat;
	private ref array<ref ExpansionChatMessage> m_ChatParams;
	private ref array<ref ExpansionChatLineBase> m_ChatLines;
	private float m_RootHeight;
	private float m_ContentHeight;

	//! Chat Handling
	private float m_ChatFadeoutTime = 10;
	const int m_MaxChatMessages = 100;

	private Widget m_Parent;
	private GridSpacerWidget ChatContent;
	private GridSpacerWidget ContentRows;
	private Widget ScrollerContainer;
	private Widget Scroller;
	
	private ScrollWidget ChatScroller;
	private Widget ChatPanel;
	private Widget ChatBackground;
	ref WidgetFadeTimer m_FadeOutTimerChat;

	ExpansionClientUIChatSize m_ChatSize;

	bool m_ChatHover;

	void ExpansionChatUIWindow(Widget parent, Chat chat)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		m_Parent = parent;
		m_Chat = chat;

		m_ChatWindowController = ExpansionChatUIWindowController.Cast(GetController());
		m_Parent.AddChild(GetLayoutRoot(), true);

		m_ChatLines = new array<ref ExpansionChatLineBase>;
		m_ChatParams = new array<ref ExpansionChatMessage>;
		m_FadeOutTimerChat = new WidgetFadeTimer;

		m_ChatSize = GetExpansionClientSettings().HUDChatSize;

		GetExpansionClientSettings().SI_UpdateSetting.Insert(OnSettingChanged);

		m_ChatFadeoutTime = Math.Round(GetExpansionClientSettings().HUDChatFadeOut);

		GetLayoutRoot().Show(false);
		CreateChatLines();
	}

	void ~ExpansionChatUIWindow()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		m_ChatLines.Clear();
		ExpansionClientSettings().SI_UpdateSetting.Remove(OnSettingChanged);
	}

	void CreateChatLines()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		ExpansionChatLineBase chatLine = new ExpansionChatLineBase(ContentRows, m_Chat);
		m_ChatLines.Insert(chatLine);

		//! Don't lag the game while creating chat lines
		if (m_ChatLines.Count() < m_MaxChatMessages)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CreateChatLines, 1, false);
		}
		else
		{
			RefreshChatMessages();
		}
	}

	void OnSettingChanged()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		m_ChatFadeoutTime = Math.Round(GetExpansionClientSettings().HUDChatFadeOut);

		//! Recreate chat lines if chat font size setting changed
		if (m_ChatSize == GetExpansionClientSettings().HUDChatSize)
			return;

		m_ChatSize = GetExpansionClientSettings().HUDChatSize;

		ClearChatLines();

		if (GetLayoutRoot())
		{
			GetLayoutRoot().Show(true);
			CreateChatLines();
		}
	}

	void Add(ChatMessageEventParams params)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		int channel =  params.param1;

		if (channel & CCSystem)
 		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.GAME_MESSAGES))
				return;
 		}
		else if (channel & CCAdmin || channel & CCBattlEye)
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.ADMIN_MESSAGES))
				return;
		}
		else if (channel & CCDirect || channel & CCMegaphone || channel & CCTransmitter || channel & CCPublicAddressSystem)
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
				return;
		}
		else if (channel & ExpansionChatChannels.CCTransport)  //! Expansion messages start here
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
				return;
		}
		else if (channel & ExpansionChatChannels.CCGlobal)
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
				return;
		}
		else if (channel & ExpansionChatChannels.CCTeam)
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
				return;
		}

		AddInternal( params );
	}

	void AddInternal(ChatMessageEventParams params)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		ExpansionChatMessage message = new ExpansionChatMessage();
		message.Channel = params.param1;
		message.From = params.param2;
		message.Text = params.param3;

		if (message.From == "BattlEye")
		{
			if (message.Text.IndexOf( "(Global) Admin:" ) == 0)
			{
				message.From = "(Global)";
				message.Text = message.Text.Substring(15, message.Text.Length() - 15).Trim();
			}
			else if (message.Text.IndexOf( "(Private) Admin:" ) == 0)
			{
				message.From = "(Private)";
				message.Text = message.Text.Substring(16, message.Text.Length() - 16).Trim();
			}
		}

		m_ChatParams.InsertAt(message, 0);

		while (m_ChatParams.Count() > m_MaxChatMessages)
		{
			m_ChatParams.Remove(m_ChatParams.Count() - 1);
		}

		RefreshChatMessages();
		
		//! This will allow chat messages to fade out, but only if chat input is not open
		if (!IsVisible())
			HideChat();
	}

	void RefreshChatMessages()
	{
		int count = m_ChatLines.Count();
		if (count != m_MaxChatMessages)  //! Still creating chat lines
			return;
		for (int i = 0; i < count; i++)
		{
			int idx = count - (i + 1);

			if (i < m_ChatParams.Count())
			{
				m_ChatLines[idx].Set(m_ChatParams[i]);
				m_ChatLines[idx].Show();
			}
			else
			{
				m_ChatLines[idx].Set(NULL);

				//! Make sure the first 12 lines are always shown even if empty so new messages appear at the bottom
				if (i < 12)
					m_ChatLines[idx].Show();
			}
		}
	}

	void HideChat()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		m_FadeOutTimerChat.FadeOut(GetLayoutRoot(), m_ChatFadeoutTime);
	}
	
	float GetContentHeight()
	{
		return m_ContentHeight;
	}

	float GetRootHeight()
	{
		return m_RootHeight;
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Chat/GUI/layouts/expansion_chat.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionChatUIWindowController;
	}

	override void OnShow()
	{
		super.OnShow();
		
		if (m_FadeOutTimerChat.IsRunning())
			m_FadeOutTimerChat.Stop();
		
		ChatBackground.Show(true);
		ChatScroller.SetAlpha(0.1);
	}
	
	override void Hide()
	{
		SetIsVisible(false);
		OnHide();
	}

	override void OnHide()
	{
		super.OnHide();

		ChatBackground.Show(false);
		ChatScroller.SetAlpha(0);
		HideChat();
	}

	ScrollWidget GetChatScroller()
	{
		return ChatScroller;
	}

	void ClearChatLines()
	{
		m_ChatLines.Clear();
	}

	Widget GetParentWidget()
	{
		return m_Parent;
	}

	override float GetUpdateTickRate()
	{
		return 0.1;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL && w == ChatScroller)
		{
			m_ChatHover = true;
			return true;
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL && w == ChatScroller && enterW != ChatScroller)
		{
			m_ChatHover = false;
			return true;
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override void Update()
	{
		//! Scroll new messages into view, but only if chat input is not open
		//! OR if mouse is not hovering chat area
		if (ChatScroller && (!IsVisible() || !m_ChatHover))
			ChatScroller.VScrollToPos01(1);  //! STAY DOWN YOU FUCKER
	}
};

class ExpansionChatUIWindowController: ExpansionViewController {};
