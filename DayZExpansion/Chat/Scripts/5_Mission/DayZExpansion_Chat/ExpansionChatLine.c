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
	int TimeStamp;
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
	private TextWidget Message;
	private ref WidgetFadeTimer m_FadeInTimer;
	private Widget m_Parent;
	private string m_LayoutPath;
	private ButtonWidget ChatItemButton;
	private ImageWidget ChatItemButtonIcon;
	private GridSpacerWidget ChatItemWidget;
	ExpansionChatMessage m_Message;

	void ExpansionChatLineBase(Widget parent, Chat chat)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		m_ChatLineController = ExpansionChatLineController.Cast(GetController());
		m_Parent = parent;
		m_Chat = chat;

		m_Parent.AddChild(GetLayoutRoot());

		GetLayoutRoot().Show(false);
	}

	void ~ExpansionChatLineBase()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		if (m_FadeInTimer)
			m_FadeInTimer.Stop();
	}

	void Set(ExpansionChatMessage message)	// Param 1 --> Channel, Param 2 --> sender name, Param 3 --> message, Param 4 ??
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);
#endif

		MissionGameplay mission;
		if (!Class.CastTo(mission, GetGame().GetMission()))
			return;

		m_Message = message;

		if (!message)
		{
			GetLayoutRoot().Show(false);
			m_ChatLineController.Time = "";
			m_ChatLineController.NotifyPropertyChanged("Time");
			m_ChatLineController.SenderName = "";
			m_ChatLineController.NotifyPropertyChanged("SenderName");
			m_ChatLineController.Message = "";
			m_ChatLineController.NotifyPropertyChanged("Message");
			return;
		}

		GetLayoutRoot().Show(true);

		switch (message.Channel)
		{
		case CCSystem:
			SenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("SystemChatColor"));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("SystemChatColor"));
			m_ChatLineController.SenderName = " Game: ";
			break;
		case CCAdmin:
		case CCBattlEye:
			SenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("AdminChatColor"));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("AdminChatColor"));
			SetSenderName(message, " Admin: ");
			break;
		case CCTransmitter:
			SenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("TransmitterChatColor"));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("TransmitterChatColor"));
			m_ChatLineController.SenderName = " PAS: ";
			break;
		case ExpansionChatChannels.CCTransport:
			SenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("TransportChatColor"));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("TransportChatColor"));
			SetSenderName(message);
			break;
		case ExpansionChatChannels.CCGlobal:
			SenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("GlobalChatColor"));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("GlobalChatColor"));
			SetSenderName(message);
			break;
#ifdef EXPANSIONMODGROUPS
		case ExpansionChatChannels.CCTeam:
			SenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("PartyChatColor"));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("PartyChatColor"));
			SetSenderName(message);
			break;
#endif
		default:
			SenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("DirectChatColor"));
			SetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("DirectChatColor"));
			SetSenderName(message);
			break;
		}

		//BreakWords(message);
		m_ChatLineController.Message = message.Text;
		m_ChatLineController.Time = message.Time;		
		m_ChatLineController.NotifyPropertiesChanged({"Time","SenderName", "Message"});

		if (!IsVisible())
		{
			FadeInChatLine();
		}

		//! Adjust message size so it actually fits and doesn't get cut off
		float root_w, root_h;
		GetLayoutRoot().GetScreenSize(root_w, root_h);
		float sender_w, sender_h;
		SenderName.GetScreenSize(sender_w, sender_h);
		Message.SetSize(1.0 - sender_w / root_w, 1.0);
	}

	protected void FadeInChatLine()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		m_Chat.OnChatInputShow();

		if (m_FadeInTimer)
			m_FadeInTimer.Stop();

		m_FadeInTimer = new WidgetFadeTimer;
		m_FadeInTimer.FadeIn(GetLayoutRoot(), 1.5);
	}

	void Clear()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		if (m_FadeInTimer)
			m_FadeInTimer.Stop();
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

	void SetAlpha(float opacity)
	{
		Time.SetAlpha(opacity);
		SenderName.SetAlpha(opacity);
		Message.SetAlpha(opacity);
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
};

class ExpansionChatLineController: ExpansionViewController
{
	string Time;
	string SenderName;
	string Message;
};