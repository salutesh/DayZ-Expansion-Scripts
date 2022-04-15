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
	string From;
	string Text;
}

class ExpansionChatLine
{
	const float FADE_TIMEOUT = 30;
	const float FADE_OUT_DURATION = 3;
	const float FADE_IN_DURATION = 1;
	
	private const string EXP_RADIO_PREFIX = "Radio: ";
	private const string EXP_GAME_PREFIX = "Game: ";
	private const string EXP_ADMIN_PREFIX = "Admin: ";
	private const string EXP_TRANSPORT_PREFIX = "Transport: ";

	Widget m_RootWidget;
	TextWidget m_NameWidget;
	MultilineTextWidget m_TextWidget;
	
	private ref WidgetFadeTimer m_FadeInTimer;
	private ref Timer m_FadeOutTimer;

	private bool m_IsShown;
	
	private ref Chat m_Chat;
	
	private string m_CurrentLayout;

	void ExpansionChatLine(Widget root_widget, string layout, Chat chat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "ExpansionChatLine");
#endif
		
		m_CurrentLayout = layout;

		m_RootWidget	= GetGame().GetWorkspace().CreateWidgets(layout, root_widget);
		
		m_NameWidget	= TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemSenderWidget"));
		m_TextWidget	= MultilineTextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemTextWidget"));
		
		m_RootWidget.Update();
		m_RootWidget.Show(false);
		
		m_Chat = chat;
	}

	void ~ExpansionChatLine()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "~ExpansionChatLine");
#endif
	
		Clear();
		
		delete m_RootWidget;
	}

	private void ExpSetTextColor(int colour)
	{
		m_TextWidget.SetColor(colour);
	}
	
	private void ExpSenderSetColour(int colour)
	{
		m_NameWidget.SetColor(colour);
	}
	
	void Set(ExpansionChatMessage message)	// Param 1 --> Channel, Param 2 --> sender name, Param 3 --> message, Param 4 ?? 
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Set");
#endif
		
		MissionGameplay mission;
		if (!Class.CastTo(mission, GetGame().GetMission()))
		{
			return;
		}
		
		m_RootWidget.Show(false);
		m_NameWidget.SetText("");
		m_TextWidget.SetText("");

		if (message == NULL)
		{
			return;
		}
		
		m_RootWidget.Show(true);
		
		switch (message.Channel)
		{
		case CCSystem:
			ExpSenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("SystemChatColor"));
			ExpSetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("SystemChatColor"));
			m_NameWidget.SetText(" " + "Game" + ": ");
			break;
		case CCAdmin:
		case CCBattlEye:
			ExpSenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("AdminChatColor"));	
			ExpSetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("AdminChatColor"));
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" " + "Admin" + ": ");
			}
			break;
		case CCTransmitter:
			ExpSenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("TransmitterChatColor"));	
			ExpSetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("TransmitterChatColor"));
			m_NameWidget.SetText(" " + "PAS" + ": ");
			break;
		case ExpansionChatChannels.CCTransport:
			ExpSenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("TransportChatColor"));	
			ExpSetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("TransportChatColor"));
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		case ExpansionChatChannels.CCGlobal:
			ExpSenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("GlobalChatColor"));	
			ExpSetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("GlobalChatColor"));
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
#ifdef EXPANSIONMODGROUPS
		case ExpansionChatChannels.CCTeam:
			ExpSenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("PartyChatColor"));	
			ExpSetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("PartyChatColor"));
			
			if (message.From)
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
#endif
		default:
			ExpSenderSetColour(GetExpansionSettings().GetChat().ChatColors.Get("DirectChatColor"));	
			ExpSetTextColor(GetExpansionSettings().GetChat().ChatColors.Get("DirectChatColor"));

			if (message.From)
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		}

		m_TextWidget.SetText(message.Text);	

		if (!m_IsShown)
		{
			m_IsShown = true;

			FadeInChatLine();
		}
	}

	private void FadeInChatLine()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "FadeInChatLine");
#endif
		
		m_Chat.OnChatInputShow();
		
		if (m_FadeInTimer)
			m_FadeInTimer.Stop();	

		m_FadeInTimer = new WidgetFadeTimer;
		m_FadeInTimer.FadeIn(m_RootWidget, FADE_IN_DURATION);
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine FadeOutChatLine
	// ------------------------------------------------------------
	//! Not used
	private void FadeOutChatLine()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "FadeOutChatLine");
#endif
		
		if (m_FadeOutTimer)
			m_FadeOutTimer.Stop();

		m_FadeOutTimer	= new Timer(CALL_CATEGORY_GUI);
		m_FadeOutTimer.Run(FADE_TIMEOUT, m_FadeInTimer, "FadeOut", new Param2<Widget, float>(m_RootWidget, FADE_OUT_DURATION));
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine Clear
	// ------------------------------------------------------------
	void Clear()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Clear");
#endif

		if (m_FadeOutTimer)
			m_FadeOutTimer.Stop();
		
		if (m_FadeInTimer)
			m_FadeInTimer.Stop();
	}

	string GetCurrentLayout()
	{
		return m_CurrentLayout;
	}
};
