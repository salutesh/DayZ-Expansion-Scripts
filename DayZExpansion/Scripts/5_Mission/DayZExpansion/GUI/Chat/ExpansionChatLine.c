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
	// Consts
	const float FADE_TIMEOUT = 30;
	const float FADE_OUT_DURATION = 3;
	const float FADE_IN_DURATION = 0.5;
	
	private const string EXP_RADIO_PREFIX = "Radio: ";
	private const string EXP_GAME_PREFIX = "Game: ";
	private const string EXP_ADMIN_PREFIX = "Admin: ";
	private const string EXP_TRANSPORT_PREFIX = "Transport: ";
	
	// Widgets
	GridSpacerWidget						m_RootWidget;
	TextWidget								m_NameWidget;
	TextWidget								m_TextWidget;
	
	private ref WidgetFadeTimer 			m_FadeInTimer;
	private ref Timer 						m_FadeOutTimer;
	
	ref Timer 				  				m_ShowTimer;
	ref Timer 				 				m_ScrollerTimer;

	bool									m_IsShown;

	void ExpansionChatLine( Widget root_widget )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::ExpansionChatLine Start");
		#endif
		m_RootWidget	= GridSpacerWidget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/chat/expansion_chat_entry.layout", root_widget) );
		
		m_NameWidget	= TextWidget.Cast( m_RootWidget.FindAnyWidget( "SenderName" ) );
		m_TextWidget	= TextWidget.Cast( m_RootWidget.FindAnyWidget( "Message" ) );
		
		m_RootWidget.Update();
		m_RootWidget.Show(false);
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::ExpansionChatLine End");
		#endif
	}

	void ~ExpansionChatLine()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::~ExpansionChatLine Start");
		#endif
		delete m_TextWidget;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::~ExpansionChatLine End");
		#endif
	}

	private void ExpSetTextColor(int colour)
	{
		m_TextWidget.SetColor(colour);
	}
	
	private void ExpSenderSetColour(int colour)
	{
		m_NameWidget.SetColor(colour);
	}
	
	void Set( ExpansionChatMessage message )	// Param 1 --> Channel, Param 2 --> sender name, Param 3 --> message, Param 4 ?? 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Set Start");
		#endif
		MissionGameplay mission;
		if ( !Class.CastTo( mission, GetGame().GetMission() ) )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Set End");
		#endif
			return;
		}
		
		m_RootWidget.Show( false );

		m_NameWidget.SetText("");
		m_TextWidget.SetText("");

		if ( message == NULL )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Set End");
		#endif
			return;
		}
		
		m_RootWidget.Show( true );
		
		switch ( message.Channel )
		{
		case CCSystem:
			ExpSenderSetColour( EXP_GAME_TEXT_COLOUR );
			ExpSetTextColor( EXP_GAME_TEXT_COLOUR );

			m_NameWidget.SetText(" " + "Game" + ": ");
			break;
		case CCAdmin:
			ExpSenderSetColour( EXP_ADMIN_TEXT_COLOUR );	
			ExpSetTextColor( EXP_ADMIN_TEXT_COLOUR );
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" " + "Admin" + ": ");
			}
			break;
		case CCTransmitter:
			ExpSenderSetColour( EXP_PAS_TEXT_COLOUR );	
			ExpSetTextColor( EXP_PAS_TEXT_COLOUR );

			m_NameWidget.SetText(" " + "PAS" + ": ");
			break;
		case ExpansionChatChannels.CCTransport:
			ExpSenderSetColour( EXP_TRANSPORT_COLOUR );	
			ExpSetTextColor( EXP_TRANSPORT_COLOUR );
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		case ExpansionChatChannels.CCGlobal:
			ExpSenderSetColour( EXP_GLOBAL_TEXT_COLOUR );	
			ExpSetTextColor( EXP_GLOBAL_TEXT_COLOUR );
			
			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		default:
			ExpSenderSetColour( EXP_DIRECT_TEXT_COLOUR );	
			ExpSetTextColor( EXP_DIRECT_TEXT_COLOUR );

			if ( message.From )
			{
				m_NameWidget.SetText(" " + message.From + ": ");
			} else
			{ 
				m_NameWidget.SetText(" ");
			}
			break;
		}

		m_TextWidget.SetText( message.Text );	

		if ( !m_IsShown )
		{
			m_IsShown = true;

			FadeInChatLine();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Set End");
		#endif
	}

	private void FadeInChatLine()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::FadeInChatLine Start");
		#endif
		if (m_FadeInTimer)
			m_FadeInTimer.Stop();	

		m_FadeInTimer = new WidgetFadeTimer;
		m_FadeInTimer.FadeIn(m_RootWidget, FADE_IN_DURATION);
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::FadeInChatLine End");
		#endif
	}

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

	void Clear()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Clear Start");
		#endif
		m_RootWidget.Show( false );
		
		if (m_FadeOutTimer)
			m_FadeOutTimer.Stop();
		
		if (m_FadeInTimer)
			m_FadeInTimer.Stop();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatLine::Clear End");
		#endif
	}
}
