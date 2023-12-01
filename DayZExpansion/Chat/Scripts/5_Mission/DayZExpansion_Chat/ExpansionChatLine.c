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
	string From;
	string Text;
	bool IsMuted;
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
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		m_ChatLineController = ExpansionChatLineController.Cast(GetController());
		m_Parent = parent;
		m_Chat = chat;
		m_FadeOutLaterTimer = new Timer(CALL_CATEGORY_GUI);
		m_FadeTimer = new ExpansionScriptViewFadeTimer;

		m_Parent.AddChild(GetLayoutRoot());
	}

	void ~ExpansionChatLineBase()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

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
			m_ChatLineController.Time = "";
			m_ChatLineController.SenderName = "";
			m_ChatLineController.Message = "";
			m_ChatLineController.NotifyPropertiesChanged({"Time","SenderName", "Message"});
			return;
		}

		switch (message.Channel)
		{
		case CCSystem:
			SenderSetColour(ARGB(255, 255, 255, 255));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("SystemChatColor"));
			m_ChatLineController.SenderName = " Game: ";
			break;
		case CCAdmin:
		case CCBattlEye:
			SenderSetColour(ARGB(255, 255, 255, 255));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("AdminChatColor"));
			SetSenderName(message, " Admin: ");
			break;
		case CCTransmitter:
			SenderSetColour(ARGB(255, 255, 255, 255));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("TransmitterChatColor"));
			m_ChatLineController.SenderName = " PAS: ";
			break;
		case ExpansionChatChannels.CCTransport:
			SenderSetColour(ARGB(255, 255, 255, 255));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("TransportChatColor"));
			SetSenderName(message);
			break;
		case ExpansionChatChannels.CCGlobal:
			SenderSetColour(ARGB(255, 255, 255, 255));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("GlobalChatColor"));
			SetSenderName(message);
			break;
#ifdef EXPANSIONMODGROUPS
		case ExpansionChatChannels.CCTeam:
			SenderSetColour(ARGB(255, 255, 255, 255));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("PartyChatColor"));
			SetSenderName(message);
			break;
#endif
		default:
			SenderSetColour(ARGB(255, 255, 255, 255));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("DirectChatColor"));
			SetSenderName(message);
			break;
		}

		m_ChatLineController.Message = BreakLongWords(message);
		m_ChatLineController.Time = message.Time;
		m_ChatLineController.NotifyPropertiesChanged({"Time","SenderName", "Message"});

		//! Adjust message size so it actually fits and doesn't get cut off
		float root_w, root_h;
		GetLayoutRoot().GetScreenSize(root_w, root_h);
		float time_w, time_h;
		Time.GetScreenSize(time_w, time_h);
		float sender_w, sender_h;
		SenderName.GetScreenSize(sender_w, sender_h);
		Message.SetSize(1.0 - (time_w + 4 + sender_w) / root_w, 1.0);
	}

	string BreakLongWords(ExpansionChatMessage message)
	{
		int maxWordCharacters = 5;

		TStringArray words = {};
		message.Text.Split(" ", words);
		string messageText;
		foreach (string word: words)
		{
			if (!word.Contains("#"))
			{
				while (word.LengthUtf8() > maxWordCharacters)
				{
					//! @note have to use RichTextWidget and `<wbr />` tag because there is no support for zero-width space
					messageText += word.SubstringUtf8(0, maxWordCharacters) + "<wbr />";
					word = word.SubstringUtf8(maxWordCharacters, word.LengthUtf8() - maxWordCharacters);
				}
			}
			messageText += word + " ";
		}

		return messageText;
	}

	void FadeIn()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		if (m_FadeOutLaterTimer.IsRunning())
			m_FadeOutLaterTimer.Stop();

		if (m_FadeTimer.IsRunning())
			m_FadeTimer.Stop();

		m_FadeTimer.FadeIn(this, 1.5, true);
	}

	void Clear()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

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
			m_ChatLineController.SenderName = " " + message.From + ": ";
		}
		else
		{
			m_ChatLineController.SenderName = fallback;
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

		if (m_Message.From == GetGame().GetPlayer().GetIdentity().GetName())
			return false;

		return m_Chat.CanMute(m_Message.Channel);
	}

	void OnEntryButtonClick()
	{
		if (!CanMute())
			return;

		m_Chat.GetChatWindow().Mute(m_Message.From);
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
