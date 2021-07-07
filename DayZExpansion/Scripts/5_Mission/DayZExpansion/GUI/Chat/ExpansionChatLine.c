/**
 * ExpansionChatLine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	
	// ------------------------------------------------------------
	// ExpansionChatLine Constructor
	// ------------------------------------------------------------
	void ExpansionChatLine( Widget root_widget, string layout, ref Chat chat)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::ExpansionChatLine Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::ExpansionChatLine - Create layout: " + layout );
		#endif
		
		m_CurrentLayout = layout;

		m_RootWidget	= GetGame().GetWorkspace().CreateWidgets( layout, root_widget );
		
		m_NameWidget	= TextWidget.Cast( m_RootWidget.FindAnyWidget( "ChatItemSenderWidget" ) );
		m_TextWidget	= MultilineTextWidget.Cast( m_RootWidget.FindAnyWidget( "ChatItemTextWidget" ) );
		
		m_RootWidget.Update();
		m_RootWidget.Show(false);
		
		m_Chat = chat;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::ExpansionChatLine End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine Destructor
	// ------------------------------------------------------------
	void ~ExpansionChatLine()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::~ExpansionChatLine Start");
		#endif
		
		Clear();
		
		delete m_RootWidget;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::~ExpansionChatLine End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine ExpSetTextColor
	// ------------------------------------------------------------
	private void ExpSetTextColor(int colour)
	{
		m_TextWidget.SetColor(colour);
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine ExpSenderSetColour
	// ------------------------------------------------------------
	private void ExpSenderSetColour(int colour)
	{
		m_NameWidget.SetColor(colour);
	}
		
	// ------------------------------------------------------------
	// ExpansionChatLine Set
	// ------------------------------------------------------------
	void Set(ExpansionChatMessage message)	// Param 1 --> Channel, Param 2 --> sender name, Param 3 --> message, Param 4 ?? 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Set Start");
		#endif
		
		MissionGameplay mission;
		if (!Class.CastTo(mission, GetGame().GetMission()))
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionChatLine::Set End");
			#endif
			return;
		}
		
		m_RootWidget.Show(false);

		m_NameWidget.SetText("");
		m_TextWidget.SetText("");

		if (message == NULL)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionChatLine::Set End");
			#endif
			return;
		}
		
		m_RootWidget.Show(true);
		
		switch (message.Channel)
		{
		case CCSystem:
			ExpSenderSetColour(GetExpansionSettings().GetGeneral().SystemChatColor);
			ExpSetTextColor(GetExpansionSettings().GetGeneral().SystemChatColor);

			m_NameWidget.SetText(" " + "Game" + ": ");
			break;
		case CCAdmin:
			ExpSenderSetColour(GetExpansionSettings().GetGeneral().AdminChatColor);	
			ExpSetTextColor(GetExpansionSettings().GetGeneral().AdminChatColor);
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" " + "Admin" + ": ");
			}
			break;
		case CCTransmitter:
			ExpSenderSetColour(GetExpansionSettings().GetGeneral().TransmitterChatColor);	
			ExpSetTextColor(GetExpansionSettings().GetGeneral().TransmitterChatColor);

			m_NameWidget.SetText(" " + "PAS" + ": ");
			break;
		case ExpansionChatChannels.CCTransport:
			ExpSenderSetColour(GetExpansionSettings().GetGeneral().TransportChatColor);	
			ExpSetTextColor(GetExpansionSettings().GetGeneral().TransportChatColor);
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		case ExpansionChatChannels.CCGlobal:
			ExpSenderSetColour(GetExpansionSettings().GetGeneral().GlobalChatColor);	
			ExpSetTextColor(GetExpansionSettings().GetGeneral().GlobalChatColor);
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		case ExpansionChatChannels.CCTeam:
			ExpSenderSetColour(GetExpansionSettings().GetGeneral().PartyChatColor);	
			ExpSetTextColor(GetExpansionSettings().GetGeneral().PartyChatColor);
			
			if (message.From)
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		default:
			ExpSenderSetColour(GetExpansionSettings().GetGeneral().DirectChatColor);	
			ExpSetTextColor(GetExpansionSettings().GetGeneral().DirectChatColor);

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

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Set End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine FadeInChatLine
	// ------------------------------------------------------------
	private void FadeInChatLine()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::FadeInChatLine Start");
		#endif
		
		m_Chat.OnChatInputShow();
		
		if (m_FadeInTimer)
			m_FadeInTimer.Stop();	

		m_FadeInTimer = new WidgetFadeTimer;
		m_FadeInTimer.FadeIn(m_RootWidget, FADE_IN_DURATION);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::FadeInChatLine End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine FadeOutChatLine
	// ------------------------------------------------------------
	//! Not used
	private void FadeOutChatLine()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::FadeOutChatLine Start");
		#endif
		
		if (m_FadeOutTimer)
			m_FadeOutTimer.Stop();

		m_FadeOutTimer	= new Timer(CALL_CATEGORY_GUI);
		m_FadeOutTimer.Run(FADE_TIMEOUT, m_FadeInTimer, "FadeOut", new Param2<Widget, float>(m_RootWidget, FADE_OUT_DURATION));
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::FadeOutChatLine End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine Clear
	// ------------------------------------------------------------
	void Clear()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Clear Start");
		#endif
		
		if (m_FadeOutTimer)
			m_FadeOutTimer.Stop();
		
		if (m_FadeInTimer)
			m_FadeInTimer.Stop();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Clear End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionChatLine GetCurrentLayout
	// ------------------------------------------------------------
	string GetCurrentLayout()
	{
		return m_CurrentLayout;
	}
}