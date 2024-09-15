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
	//! Max messages in history
	protected const int MAX_MESSAGES = 100;

	//! Number of chat lines (partly) visible at smallest chat font size (adjust this when changing height of the chathistory box in layout)
	protected const int MAX_MESSAGES_VISIBLE = 16;

	protected ref ExpansionChatUIWindowController m_ChatWindowController;
	protected ref Chat m_Chat;
	protected ref array<ref ExpansionChatMessage> m_ChatParams;
	protected ref array<ref ExpansionChatLineBase> m_ChatLines;

	protected float m_MessageTimeTheshold = 10.0;  //! How long messages are displayed (seconds)
	protected float m_MessageFadeoutDuration = 3.0;  //! Duration of fading messages out effect (seconds)

	protected Widget m_Parent;
	protected Widget ChatPanel;
	protected WrapSpacerWidget ContentRows;
	protected Widget MutePanel;
	protected ScrollWidget MuteScroller;
	protected ButtonWidget MuteListButton;
	protected ImageWidget MuteListButtonIcon;

	protected ScrollWidget ChatScroller;
	protected Widget ChatBackground;

	protected ExpansionClientUIChatSize m_ChatSize;

	void ExpansionChatUIWindow(Widget parent, Chat chat)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		m_Parent = parent;
		m_Chat = chat;

		m_ChatWindowController = ExpansionChatUIWindowController.Cast(GetController());
		m_Parent.AddChild(GetLayoutRoot(), true);

		m_ChatLines = new array<ref ExpansionChatLineBase>;
		m_ChatParams = new array<ref ExpansionChatMessage>;

		m_ChatSize = GetExpansionClientSettings().HUDChatSize;
		m_MessageTimeTheshold = GetExpansionClientSettings().HUDChatMessageTimeThreshold;
		m_MessageFadeoutDuration = GetExpansionClientSettings().HUDChatFadeOut;

		GetExpansionClientSettings().SI_UpdateSetting.Insert(OnSettingChanged);

		GetLayoutRoot().Show(false);
		CreateChatLines();
	}

	void ~ExpansionChatUIWindow()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		m_ChatLines.Clear();
		ExpansionClientSettings().SI_UpdateSetting.Remove(OnSettingChanged);
	}

	void CreateChatLines()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		if (!ContentRows)
			return;

		ExpansionChatLineBase chatLine = new ExpansionChatLineBase(ContentRows, m_Chat);
		m_ChatLines.Insert(chatLine);

		//! Don't lag the game while creating chat lines
		if (m_ChatLines.Count() < MAX_MESSAGES)
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		m_MessageTimeTheshold = GetExpansionClientSettings().HUDChatMessageTimeThreshold;
		m_MessageFadeoutDuration = GetExpansionClientSettings().HUDChatFadeOut;
		

		//! Recreate chat lines if chat font size setting changed
		if (m_ChatSize == GetExpansionClientSettings().HUDChatSize)
			return;

		m_ChatSize = GetExpansionClientSettings().HUDChatSize;

		ClearChatLines();

		if (GetLayoutRoot())
		{
			if (GetExpansionClientSettings().HUDChatToggle)
				GetLayoutRoot().Show(true);
			CreateChatLines();
		}
	}

	void Add(ChatMessageEventParams params)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

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
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		ExpansionChatMessage message = new ExpansionChatMessage();
		message.Channel = params.param1;
		message.From = params.param2;
		message.Text = params.param3;
		message.SetColorByName(params.param4);

		ExpansionChatMessageEventParams exParams;
		if (Class.CastTo(exParams, params))
		{
			message.UID = exParams.param5;
		#ifdef EXPANSIONMODGROUPS
			message.PlayerTag = exParams.param6;
		#endif
		}

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

		message.FormatText();

		message.TimeStamp = GetGame().GetTickTime();

		int hour;
		int minute;
		int second;
		GetHourMinuteSecond(hour, minute, second);
		message.Time = hour.ToStringLen(2) + ":" + minute.ToStringLen(2);

		m_ChatParams.InsertAt(message, 0);

		while (m_ChatParams.Count() > MAX_MESSAGES)
		{
			m_ChatParams.Remove(m_ChatParams.Count() - 1);
		}

		SetChatMessage(message);
	}

	void SetChatMessage(ExpansionChatMessage message)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		int count = m_ChatLines.Count();
		if (count != MAX_MESSAGES)  //! Still creating chat lines
			return;

		if (message.IsMuted)
			return;

		if (GetExpansionClientSettings().HUDChatToggle)
			GetLayoutRoot().Show(true);

		int idx = count - 1;

		ExpansionChatLineBase chatLine = m_ChatLines[0];

		bool isChatHistoryVisible = IsVisible();

		int firstVisibleMessageIndex = count - MAX_MESSAGES_VISIBLE;
		for (int i = 0; i < idx; i++)
		{
			m_ChatLines[i] = m_ChatLines[i + 1];
			m_ChatLines[i].GetLayoutRoot().SetSort(i);
		}

		m_ChatLines[idx] = chatLine;

		chatLine.Set(message);
		chatLine.GetLayoutRoot().SetSort(idx);
		chatLine.Show();

		if (!isChatHistoryVisible)
		{
			float time = GetGame().GetTickTime();
			chatLine.FadeOutLater(m_MessageTimeTheshold - (time - message.TimeStamp), m_MessageFadeoutDuration);
		}
	}

	void RefreshChatMessages(bool setMessages = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		int count = m_ChatLines.Count();
		if (count != MAX_MESSAGES)  //! Still creating chat lines
			return;

		bool isChatHistoryVisible = IsVisible();

		int firstVisibleMessageIndex = count - MAX_MESSAGES_VISIBLE;
		int i;
		int idx = count - 1;
		float time = GetGame().GetTickTime();
		float showMessageTimestamp = time - m_MessageTimeTheshold;
		while (idx >= 0)
		{
			if (i < m_ChatParams.Count())
			{
				ExpansionChatMessage message = m_ChatParams[i++];
				if (message.IsMuted)
					continue;

				if (setMessages)
					m_ChatLines[idx].Set(message);

				m_ChatLines[idx].Show();
				
				if (isChatHistoryVisible)
					m_ChatLines[idx].SetAlphaEx(1.0);
				else if (idx < firstVisibleMessageIndex || message.TimeStamp < showMessageTimestamp)
					m_ChatLines[idx].SetAlphaEx(0.0);
				else
					m_ChatLines[idx].FadeOutLater(m_MessageTimeTheshold - (time - message.TimeStamp), m_MessageFadeoutDuration);
			}
			else
			{
				if (setMessages)
					m_ChatLines[idx].Set(NULL);

				//! Make sure the number of lines that fit in the visible area of the chathistory box
				//! are always shown even if empty so new messages appear at the bottom
				if (idx >= firstVisibleMessageIndex)
					m_ChatLines[idx].Show();
				else
					m_ChatLines[idx].Hide();
			}

			m_ChatLines[idx].GetLayoutRoot().SetSort(idx);

			idx--;
		}
	}

	void ShowChatMessages(bool show = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		RefreshChatMessages(false);
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

		ChatBackground.Show(true);
		ChatScroller.SetAlpha(0.1);
		MuteListButton.Show(true);
		ShowChatMessages();
	}

	override void Hide()
	{
		SetIsVisible(false);
		OnHide();
	}

	override void OnHide()
	{
		super.OnHide();
		
		if (ChatBackground)
			ChatBackground.Show(false);
		
		if (ChatScroller)
			ChatScroller.SetAlpha(0);
		
		if (MutePanel)
			MutePanel.Show(false);
		
		if (MuteListButton)
			MuteListButton.Show(false);

		ShowChatMessages(false);
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
		if (w == MuteListButton)
		{
			MuteListButtonIcon.SetColor(ARGB(200, 0, 0, 0));
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == MuteListButton)
		{
			MuteListButtonIcon.SetColor(ARGB(200, 255, 255, 255));
			return true;
		}

		return false;
	}

	bool IsMouseLeftDown()
	{
		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)
			return true;

		return false;
	}

	bool IsMouseInChatPanel()
	{
		int x, y;
		GetMousePos(x, y);

		float chatPanelW, chatPanelH;
		float chatPanelX, chatPanelY;
		ChatPanel.GetScreenSize(chatPanelW, chatPanelH);
		ChatPanel.GetScreenPos(chatPanelX, chatPanelY);
	
		if (x > chatPanelX && x < chatPanelX + chatPanelW && y > chatPanelY && y < chatPanelY + chatPanelH)
			return true;

		return false;
	}

	override void Expansion_Update()
	{
		//! Scroll new messages into view, but only if chat input is not open
		//! OR if mouse is not hovering chat area
		if (ChatScroller && (!IsVisible() || (!IsMouseLeftDown() && !IsMouseInChatPanel())))
			ChatScroller.VScrollToPos01(1.0);  //! STAY DOWN YOU FUCKER
	}

	void UpdateMuteList()
	{
		m_ChatWindowController.MuteEntries.Clear();

		ExpansionClientSettings clientSettings = GetExpansionClientSettings();

		if (clientSettings.MutedPlayers.Count() == 0)
			return;

		if (ClientData && ClientData.m_PlayerList && ClientData.m_PlayerList.m_PlayerList)
		{
			foreach (SyncPlayer player: ClientData.m_PlayerList.m_PlayerList)
			{
				if (clientSettings.MutedPlayers.Find(player.m_RUID) == -1)
					continue;

				ExpansionChatMuteEntry entry = new ExpansionChatMuteEntry(this, player);
				m_ChatWindowController.MuteEntries.Insert(entry);
			}
		}
	}

	void Mute(string uid)
	{
		ExpansionClientSettings clientSettings = GetExpansionClientSettings();
		foreach (SyncPlayer player: ClientData.m_PlayerList.m_PlayerList)
		{
			if (player.m_RUID == uid)
			{
				if (clientSettings.MutedPlayers.Find(player.m_RUID) == -1)
				{
#ifdef EXTRACE
					auto trace = EXTrace.StartStack(EXTrace.CHAT, this, "Mute " + player.m_RUID + " " + player.m_PlayerName + " " + clientSettings.MutedPlayers.Count());
#endif
					
					clientSettings.MutedPlayers.Insert(player.m_RUID);
					if (clientSettings.MutedPlayers.Count() > 100)
						clientSettings.MutedPlayers.RemoveOrdered(0);
					clientSettings.Save();
					UpdateMuteList();
					UpdateMute(player.m_RUID, true);
				}
				break;
			}
		}
	}

	void Unmute(string playerUID, string playerName)
	{
		ExpansionClientSettings clientSettings = GetExpansionClientSettings();
		int index = clientSettings.MutedPlayers.Find(playerUID);
		EXTrace.Print(EXTrace.CHAT, this, "Unmute " + playerUID + " " + playerName + " " + index);
		if (index > -1)
		{
			clientSettings.MutedPlayers.RemoveOrdered(index);
			clientSettings.Save();
			UpdateMute(playerUID, false);
		}
	}

	void UpdateMute(string uid, bool mute)
	{
		foreach (ExpansionChatMessage message: m_ChatParams)
		{
			if (message.UID == uid)
			{
				//! https://feedback.bistudio.com/T173348
				if (mute && m_Chat.CanMute(message.Channel))
					message.IsMuted = true;
				else
					message.IsMuted = false;
			}
		}

		RefreshChatMessages();
	}

	void OnMuteListButtonClick()
	{
		bool state = MutePanel.IsVisible();
		MutePanel.Show(!state);

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
		return MutePanel.IsVisible();
	}
};

class ExpansionChatUIWindowController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionChatMuteEntry> MuteEntries = new ObservableCollection<ref ExpansionChatMuteEntry>(this);
};