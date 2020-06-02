/**
 * ExpansionChat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Chat
{
	const int MAX_CHAT_HISTORY = 100;

	private ref ExpansionChatHandler m_Handler;

	private ref array< ref ExpansionChatMessage > m_ChatParams;
	private ref array< ref ExpansionChatLine > m_ChatLines;

	private bool m_IsChatInputVisible;

	void Chat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Chat Start");
		#endif
		
		m_ChatParams = new array< ref ExpansionChatMessage >;
		m_ChatLines = new array< ref ExpansionChatLine >;

		ChatInputMenu.SI_OnChatInputHide.Insert( OnChatInputHide );
		ChatInputMenu.SI_OnChatInputShow.Insert( OnChatInputShow );

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Chat End");
		#endif
	}

	void ~Chat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::~Chat Start");
		#endif

		ChatInputMenu.SI_OnChatInputHide.Remove( OnChatInputHide );
		ChatInputMenu.SI_OnChatInputShow.Remove( OnChatInputShow );

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::~Chat End");
		#endif
	}
	
	override void Init( Widget root_widget )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Init Start");
		#endif

		Destroy();
		
		m_RootWidget = root_widget;
		
		if ( m_RootWidget )
		{
			m_Handler = new ExpansionChatHandler( this );
			m_RootWidget.SetHandler( m_Handler );

			GridSpacerWidget rows;
			if ( Class.CastTo( rows, UIActionManager.CreateActionRows( m_Handler.GetContentWidget() ) ) )
			{
				//rows.SetContentAlignmentV( WidgetAlignment.WA_BOTTOM );
				for ( int i = 0; i < 10; i++ )
				{
					GridSpacerWidget gsw;
					if ( !Class.CastTo( gsw, rows.FindAnyWidget( "Content_Row_0" + i ) ) )
						continue;

					//gsw.SetContentAlignmentV( WidgetAlignment.WA_BOTTOM );

					for ( int j = 0; j < 100; j++ )
					{
						m_ChatLines.Insert( new ExpansionChatLine( gsw ) );
					}
				}
			}

			UpdateScroller();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Init End");
		#endif
	}
	
	Widget GetRootWidget()
	{
		return m_RootWidget;
	}

	override void Destroy()
	{
		m_ChatLines.Clear();
	}
	
	override void Clear()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Clear Start");
		#endif

		if (m_ChatLines.Count() >= 0)
		{
			for (int i = 0; i < m_ChatLines.Count(); i++)
			{
				m_ChatLines.Get(i).Clear();
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Clear End");
		#endif
	}
	
	override void Add( ChatMessageEventParams params )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Add Start");
		#endif

		int max_lenght = 100;
		int name_lenght = params.param2.Length();
		int text_lenght = params.param3.Length();
		int total_lenght = text_lenght + name_lenght;
		int channel =  params.param1;

		if ( channel & CCSystem )
 		{
			if ( g_Game.GetProfileOption( EDayZProfilesOptions.GAME_MESSAGES ) )
			{
				#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Add End");
		#endif
				return;
			}
 		} else if ( channel & CCAdmin )
		{
			if ( g_Game.GetProfileOption( EDayZProfilesOptions.ADMIN_MESSAGES ) )
			{
				#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Add End");
		#endif
				return;
			}
		}
		else if( channel & CCDirect || channel & CCMegaphone || channel & CCTransmitter || channel & CCPublicAddressSystem || channel & ExpansionChatChannels.CCTransport ) 
		{
			if ( g_Game.GetProfileOption( EDayZProfilesOptions.PLAYER_MESSAGES ) )
			{
				#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Add End");
		#endif
				return;
			}
		}
		
		//! Expansion messages
		else if( channel & ExpansionChatChannels.CCTransport ) 
		{
			if ( g_Game.GetProfileOption( EDayZProfilesOptions.PLAYER_MESSAGES ) )
			{
				#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Add End");
		#endif
				return;
			}
		}
		else if( channel & ExpansionChatChannels.CCGlobal ) 
		{
			if ( g_Game.GetProfileOption( EDayZProfilesOptions.PLAYER_MESSAGES ) )
			{
				#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Add End");
		#endif
				return;
			}
		}

		AddInternal( params );
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Add End");
		#endif
	}
	
	override void AddInternal( ChatMessageEventParams params )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::AddInternal Start");
		#endif

		ExpansionChatMessage message = new ExpansionChatMessage();
		message.Channel = params.param1;
		message.From = params.param2;
		message.Text = params.param3;

		m_ChatParams.InsertAt( message, 0 );

		while ( m_ChatParams.Count() > MAX_CHAT_HISTORY )
		{
			m_ChatParams.Remove( m_ChatParams.Count() - 1 );
		}

		for ( int i = 0; i < m_ChatLines.Count(); i++ )
		{
			int idx = 1000 - (i + 1);

			if ( i < m_ChatParams.Count() )
			{
				m_ChatLines[idx].Set( m_ChatParams[i] );
			} else
			{
				m_ChatLines[idx].Set( NULL );
			}
		}

		UpdateScroller();

		if ( !m_IsChatInputVisible )
		{
			m_Handler.ScrollTo( 0 );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::AddInternal End");
		#endif
	}

	void UpdateScroller()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::UpdateScroller Start");
		#endif

		m_Handler.UpdateScroller();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::UpdateScroller End");
		#endif
	}

	void ScrollTo( float x )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::ScrollTo Start");
		#endif

		m_Handler.ScrollTo( x );

		m_Handler.UpdateScroller();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::ScrollTo End");
		#endif
	}

	void Update( float timeSlice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Update Start");
		#endif

		m_Handler.Update( timeSlice );

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Update End");
		#endif
	}

	void OnChatInputShow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputShow Start");
		#endif

		m_Handler.ShowScroller();

		m_Handler.UpdateScroller();

		m_Handler.ShowChat();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputShow End");
		#endif
	}

	void OnChatInputHide()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputHide Start");
		#endif

		m_Handler.HideScroller();

		m_Handler.ScrollTo( 0 );

		m_Handler.UpdateScroller();

		m_Handler.HideChat();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputHide End");
		#endif
	}
}