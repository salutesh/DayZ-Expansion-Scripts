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
	protected ref ExpansionChatUIWindowController m_ChatWindowController;
	protected ref Chat m_Chat;
	protected ref array<ref ExpansionChatMessage> m_ChatParams;
	protected ref array<ref ExpansionChatLineBase> m_ChatLines;
	protected float m_RootHeight;
	protected float m_ContentHeight;

	//! Chat Handling
	protected float m_ChatFadeoutTime = 10;
	protected const int m_MaxChatMessages = 100;

	protected Widget m_Parent;
	protected GridSpacerWidget ChatContent;
	protected GridSpacerWidget ContentRows;
	protected Widget ScrollerContainer;
	protected Widget Scroller;
	protected Widget MutePanel;
	protected ScrollWidget MuteScroller;
	protected ButtonWidget MuteListButton;
	protected ImageWidget MuteListButtonIcon;

	protected ScrollWidget ChatScroller;
	protected Widget ChatPanel;
	protected Widget ChatBackground;
	protected ref WidgetFadeTimer m_FadeOutTimerChat;

	protected ExpansionClientUIChatSize m_ChatSize;
	
	protected int m_MutedPlayersCount;
	protected bool m_MuteListVisible;

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
		
		m_MutedPlayersCount = GetExpansionClientSettings().MutedPlayers.Count();
		if (m_MutedPlayersCount > 0)
			UpdateMuteList();
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

		if (!ContentRows)
			return;

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
			bool isMuted = m_Chat.IsPlayerMuted(m_ChatLines[idx].GetChatLineController().SenderName);

			if (i < m_ChatParams.Count() && !isMuted)
			{
				m_ChatLines[idx].Set(m_ChatParams[i]);
				m_ChatLines[idx].Show();
				m_ChatLines[idx].SetCanMute(true);
			}
			else
			{
				m_ChatLines[idx].Set(NULL);
				m_ChatLines[idx].SetCanMute(false);
				m_ChatLines[idx].Hide();

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

		UpdateMuteList();

		if (m_FadeOutTimerChat.IsRunning())
			m_FadeOutTimerChat.Stop();

		ChatBackground.Show(true);
		ChatScroller.SetAlpha(0.1);
		MuteListButton.Show(true);
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
		MutePanel.Show(false);
		MuteListButton.Show(false);
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
		EXTrace.Print(EXTrace.CHAT, this, "::OnMouseEnter - Widget: " + w.GetName());
		if ((w == ChatScroller || w == ContentRows || w.GetName() == "ChatItemWidget" || w.GetName() == "ChatItemButton"))
		{
			m_ChatHover = true;
			return true;
		}
		else if (w == MuteListButton)
		{
			MuteListButtonIcon.SetColor(ARGB(200, 0, 0, 0));
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (enterW)
			EXTrace.Print(EXTrace.CHAT, this, "::OnMouseLeave - Widget: " + w.GetName() + " | Enter Widget: " + enterW.GetName());

		if ((w == ChatScroller || w == ContentRows || w.GetName() == "ChatItemWidget" || w.GetName() == "ChatItemButton") && (!enterW || (enterW != ChatScroller && enterW != ContentRows && enterW.GetName() != "ChatItemWidget" && enterW.GetName() != "ChatItemButton")))
		{
			m_ChatHover = false;
			return true;
		}
		else if (w == MuteListButton)
		{
			MuteListButtonIcon.SetColor(ARGB(200, 255, 255, 255));
			return true;
		}

		return false;
	}

	override void Update()
	{
		//! Scroll new messages into view, but only if chat input is not open
		//! OR if mouse is not hovering chat area
		if (ChatScroller && (!IsVisible() || !m_ChatHover))
			ChatScroller.VScrollToPos01(1.0);  //! STAY DOWN YOU FUCKER
	}
	
	void UpdateMuteList()
	{
		m_ChatWindowController.MuteEntries.Clear();
		
		if (ClientData && ClientData.m_PlayerList && ClientData.m_PlayerList.m_PlayerList)
		{
			foreach (SyncPlayer player: ClientData.m_PlayerList.m_PlayerList)
			{
				if (GetExpansionClientSettings().MutedPlayers.Find(player.m_RUID) == -1)
					continue;
	
				ExpansionChatMuteEntry entry = new ExpansionChatMuteEntry(this, player);
				m_ChatWindowController.MuteEntries.Insert(entry);
			}
		}
	}

	void OnMuteListButtonClick()
	{
		bool state = MutePanel.IsVisible();
		MutePanel.Show(!state);
		m_MuteListVisible = !state;
		
		if (state)
		{
			MuteScroller.SetAlpha(1.0);
			ChatInputMenu inputMenu = ChatInputMenu.Cast(GetGame().GetUIManager().GetMenu());
			if (inputMenu)
				SetFocus(inputMenu.GetEditboxWidget());
		}
		else
		{
			MuteScroller.SetAlpha(0);
		}
	}
	
	bool IsMuteListVisible()
	{
		return m_MuteListVisible;
	}
};

class ExpansionChatUIWindowController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionChatMuteEntry> MuteEntries = new ObservableCollection<ref ExpansionChatMuteEntry>(this);
};