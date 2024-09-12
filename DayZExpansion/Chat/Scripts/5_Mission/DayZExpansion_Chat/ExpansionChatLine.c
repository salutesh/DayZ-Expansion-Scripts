/**
 * ExpansionChatLine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionChatMessage
{
	int Channel;
	float TimeStamp;
	string Time;
	string UID;
	string PlayerTag;
	string From;
	string Text;
	string FormattedText;
	bool IsMuted;
	int Color;

	void SetColorByName(string colorName)
	{
		auto chatColors = GetExpansionSettings().GetChat().ChatColors;

		//! See vanilla ChatLine::SetColorByParam
		if (colorName)
		{
			//! See vanilla ChatLine::ColorNameToColor
			switch (colorName)
			{
				case "colorStatusChannel":
					Color = chatColors.Get("StatusMessageColor");
					break;
				case "colorAction":
					Color = chatColors.Get("ActionMessageColor");
					break;
				case "colorFriendly":
					Color = chatColors.Get("FriendlyMessageColor");
					break;
				case "colorImportant":
					Color = chatColors.Get("ImportantMessageColor");
					break;
				default:
					Color = chatColors.Get("DefaultMessageColor");
					break;
			}
		}
		else
		{
			switch (Channel)
			{
				case CCSystem:
					Color = chatColors.Get("SystemChatColor");
					break;
				case CCAdmin:
				case CCBattlEye:
					Color = chatColors.Get("AdminChatColor");
					break;
				case CCTransmitter:
					Color = chatColors.Get("TransmitterChatColor");
					break;
				case ExpansionChatChannels.CCTransport:
					Color = chatColors.Get("TransportChatColor");
					break;
				case ExpansionChatChannels.CCGlobal:
					Color = chatColors.Get("GlobalChatColor");
					break;
		#ifdef EXPANSIONMODGROUPS
				case ExpansionChatChannels.CCTeam:
					Color = chatColors.Get("PartyChatColor");
					break;
		#endif
				default:
					Color = chatColors.Get("DirectChatColor");
					break;
			}
		}
	}

	void FormatText()
	{
		string text = Text;

		text.Replace("<", "‹");
		text.Replace(">", "›");

		int maxWordCharacters = 5;

		TStringArray words = {};
		text.Split(" ", words);
		string formattedText;
		foreach (string word: words)
		{
			if (!word.Contains("#"))
			{
				while (word.LengthUtf8() > maxWordCharacters)
				{
					//! @note have to use RichTextWidget and `<wbr />` tag because there is no support for zero-width space
					formattedText += word.SubstringUtf8(0, maxWordCharacters) + "<wbr />";
					word = word.SubstringUtf8(maxWordCharacters, word.LengthUtf8() - maxWordCharacters);
				}
			}

			formattedText += word + " ";
		}

		FormattedText = formattedText;
	}
}

class ExpansionChatLineBase: ExpansionScriptView
{
	private ref ExpansionChatLineController m_ChatLineController;
	private ref Chat m_Chat;

	private TextWidget Time;
	private TextWidget SenderName;
	private RichTextWidget Message;
	private ref Timer m_FadeOutLaterTimer;
	private ref ExpansionScriptViewFadeTimer m_FadeTimer;
	private Widget m_Parent;
	private string m_LayoutPath;
	private ButtonWidget ChatItemButton;
	private ImageWidget ChatItemButtonIcon;
	private GridSpacerWidget ChatItemWidget;
	ExpansionChatMessage m_Message;

	void ExpansionChatLineBase(Widget parent, Chat chat)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		m_ChatLineController = ExpansionChatLineController.Cast(GetController());
		m_Parent = parent;
		m_Chat = chat;
		m_FadeOutLaterTimer = new Timer(CALL_CATEGORY_GUI);
		m_FadeTimer = new ExpansionScriptViewFadeTimer;

		m_Parent.AddChild(GetLayoutRoot());
	}

	void ~ExpansionChatLineBase()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		if (m_FadeOutLaterTimer && m_FadeOutLaterTimer.IsRunning())
			m_FadeOutLaterTimer.Stop();

		if (m_FadeTimer && m_FadeTimer.IsRunning())
			m_FadeTimer.Stop();
	}

	void Set(ExpansionChatMessage message)	// Param 1 --> Channel, Param 2 --> sender name, Param 3 --> message, Param 4 ??
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		MissionGameplay mission;
		if (!Class.CastTo(mission, GetGame().GetMission()))
			return;

		m_Message = message;

		if (!message)
		{
			Time.SetText("");
			SenderName.SetText("");
			Message.SetText("");
			return;
		}

		switch (message.Channel)
		{
		case CCSystem:
			SenderName.SetText(" Game: ");
			break;
		case CCAdmin:
		case CCBattlEye:
			SetSenderName(message, " Admin: ");
			break;
		case CCTransmitter:
			SenderName.SetText(" PAS: ");
			break;
		case ExpansionChatChannels.CCTransport:
			SetSenderName(message);
			break;
		case ExpansionChatChannels.CCGlobal:
			SetSenderName(message);
			break;
#ifdef EXPANSIONMODGROUPS
		case ExpansionChatChannels.CCTeam:
			SetSenderName(message);
			break;
#endif
		default:
			SetSenderName(message);
			break;
		}

		SenderSetColour(ARGB(255, 255, 255, 255));
		SetTextColor(message.Color);

		Message.SetText(message.FormattedText);
		Time.SetText(message.Time);

		//! Adjust message size so it actually fits and doesn't get cut off
		float root_w, root_h;
		m_Parent.GetScreenSize(root_w, root_h);
		float time_w, time_h;
		Time.GetScreenSize(time_w, time_h);
		float sender_w, sender_h;
		SenderName.GetScreenSize(sender_w, sender_h);
		Message.SetSize(1.0 - (time_w + 4 + sender_w) / root_w, 1.0);
	}

	void FadeIn()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		if (m_FadeOutLaterTimer.IsRunning())
			m_FadeOutLaterTimer.Stop();

		if (m_FadeTimer.IsRunning())
			m_FadeTimer.Stop();

		m_FadeTimer.FadeIn(this, 1.5, true);
	}

	void Clear()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		if (m_FadeOutLaterTimer.IsRunning())
			m_FadeOutLaterTimer.Stop();

		if (m_FadeTimer)
			m_FadeTimer.Stop();
	}

	protected void SetTextColor(int colour)
	{
		Message.SetColor(colour);
	}

	protected void SetSenderName(ExpansionChatMessage message, string fallback = " ")
	{
		if ( message.From )
		{
			if (message.PlayerTag != string.Empty)
				SenderName.SetText(string.Format(" %1%2: ", message.PlayerTag, message.From));
			else
				SenderName.SetText(string.Format(" %1: ", message.From));
		}
		else
		{
			SenderName.SetText(fallback);
		}
	}

	protected void SenderSetColour(int colour)
	{
		SenderName.SetColor(colour);
	}

	override string GetLayoutFile()
	{
		string path;
		ExpansionClientUIChatSize chatsize = GetExpansionClientSettings().HUDChatSize;
		switch (chatsize)
		{
			case ExpansionClientUIChatSize.VERYSMALL:
				path = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_verysmall.layout";
				break;
			case ExpansionClientUIChatSize.SMALL:
				path = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_small.layout";
				break;
			case ExpansionClientUIChatSize.MEDIUM:
				path = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_medium.layout";
				break;
			case ExpansionClientUIChatSize.LARGE:
				path = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_large.layout";
				break;
			case ExpansionClientUIChatSize.VERYLARGE:
				path = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_verylarge.layout";
				break;
			default:
				path = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_small.layout";
				break;
		}

		return path;
	}

	override void SetAlpha(float alpha)
	{
		super.SetAlpha(alpha);

		if (Time)
			Time.SetAlpha(alpha * 0.8);
		if (SenderName)
			SenderName.SetAlpha(alpha);
		if (Message)
			Message.SetAlpha(alpha);
	}

	void SetAlphaEx(float alpha)
	{
		if (m_FadeOutLaterTimer.IsRunning())
			m_FadeOutLaterTimer.Stop();

		if (m_FadeTimer.IsRunning())
			m_FadeTimer.Stop();

		SetAlpha(alpha);
	}

	override typename GetControllerType()
	{
		return ExpansionChatLineController;
	}

	Widget GetParentWidget()
	{
		return m_Parent;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ((w == ChatItemWidget || w == ChatItemButton) && CanMute())
		{
			if (w == ChatItemButton)
				ChatItemButtonIcon.SetColor(ARGB(200, 0, 0, 0));

			ChatItemButton.Show(true);
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ((w == ChatItemWidget || w == ChatItemButton) && CanMute())
		{
			ChatItemButtonIcon.SetColor(ARGB(200, 255, 255, 255));

			ChatItemButton.Show(false);
			return true;
		}

		return false;
	}

	bool CanMute()
	{
		if (!m_Message)
			return false;

		if (!m_Message.UID || m_Message.UID == GetGame().GetPlayer().GetIdentity().GetId())
			return false;

		return m_Chat.CanMute(m_Message.Channel);
	}

	void OnEntryButtonClick()
	{
		if (!CanMute())
			return;

		m_Chat.GetChatWindow().Mute(m_Message.UID);
		ChatItemButton.Show(false);
	}

	ExpansionChatLineController GetChatLineController()
	{
		return m_ChatLineController;
	}

	override void OnHide()
	{
		super.OnHide();

		ChatItemButton.Show(false);
	}

	void FadeOut(float duration)
	{
		if (m_FadeTimer.IsRunning())
			m_FadeTimer.Stop();

		if (!m_Chat.GetChatWindow().IsVisible())
			m_FadeTimer.FadeOut(this, duration, true, false);
	}

	void FadeOutLater(float delay, float duration)
	{
		if (delay < 0)
			delay = 0;

		if (m_FadeOutLaterTimer.IsRunning())
			m_FadeOutLaterTimer.Stop();

		if (!m_FadeTimer.IsRunning())
			SetAlpha(1.0);

		m_FadeOutLaterTimer.Run(delay, this, "FadeOut", new Param1<float>(duration));
	}
};

class ExpansionChatLineController: ExpansionViewController
{
	string Time;
	string SenderName;
	string Message;
};
