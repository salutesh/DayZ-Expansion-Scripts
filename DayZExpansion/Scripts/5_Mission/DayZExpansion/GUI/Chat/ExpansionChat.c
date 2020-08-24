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
	
	private ref array< ref ExpansionChatMessage > m_ChatParams;
	private ref array< ref ExpansionChatLine > m_ChatLines;

	private bool m_IsChatInputVisible;

	//! Chat Handling
	const int WHEEL_STEP = 20;
	private float CHAT_FADEOUT_TIME = 10;
	const float FADE_OUT_DURATION = 0.5;
	const float FADE_IN_DURATION = 0.5;

	GridSpacerWidget m_Content;
	Widget m_ScrollerContainer;
	Widget m_Scroller;

	private float m_RootHeight;
	private float m_ContentHeight;
	private float m_Position;

	private float m_ScrollStartPos;
	private int m_MouseStartPos;

	private bool m_IsDragScrolling;
	private bool m_IsMouseScrolling;

	private bool m_ShowScroller;

	ref WidgetFadeTimer m_FadeInTimerChat;
	ref Timer m_TimeOutTimerChat;
	
	private GridSpacerWidget m_ContentRow;
	
	private string m_LayoutPath;
	
	private float m_ChatToggleTime;
	
	private bool m_IsVisible;
	
	// ------------------------------------------------------------
	// Chat Constructor
	// ------------------------------------------------------------
	void Chat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Chat Start");
		#endif
		
		m_ChatParams = new array< ref ExpansionChatMessage >;
		m_ChatLines = new array< ref ExpansionChatLine >;

		GetExpansionClientSettings().SI_UpdateSetting.Insert( OnSettingChanged );
				
		GetClientSettings();
		
		m_IsVisible = false;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Chat End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Chat Destructor
	// ------------------------------------------------------------
	void ~Chat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::~Chat Start");
		#endif

		GetExpansionClientSettings().SI_UpdateSetting.Remove( OnSettingChanged );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::~Chat End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Chat Init
	// ------------------------------------------------------------
	override void Init( Widget root_widget )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Init Start");
		#endif

		Destroy();
		
		m_RootWidget = root_widget;
		
		if ( m_RootWidget )
		{
			m_Content = GridSpacerWidget.Cast( m_RootWidget.FindAnyWidget( "Content" ) );
			m_ScrollerContainer = Widget.Cast( m_RootWidget.FindAnyWidget( "ScrollerContainer" ) );
			m_Scroller = Widget.Cast( m_RootWidget.FindAnyWidget( "Scroller" ) );

			m_Position = 1;
			
			if ( Class.CastTo( m_ContentRow, UIActionManager.CreateActionRows( m_Content ) ) )
			{
				int totalInContentRow = 100;
				int currentContentRow = 0;

				GridSpacerWidget gsw;

				for ( int i = 0; i < 10; i++ )
				{
					if ( totalInContentRow >= 100 )
					{
						Class.CastTo( gsw, m_ContentRow.FindAnyWidget( "Content_Row_0" + currentContentRow ) );
						gsw.Show( true );
						currentContentRow++;
						totalInContentRow = 0;
					}
					
					CreateChatLines(gsw);
				}
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Init End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionChat CreateChatLines
	// ------------------------------------------------------------
	void CreateChatLines(GridSpacerWidget gsw)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::CreateChatLines - Start");
		#endif
		
		GetClientSettings();
		
		for ( int j = 0; j < 100; j++ )
		{
			m_ChatLines.Insert( new ExpansionChatLine( gsw, m_LayoutPath, this ) );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::CreateChatLines - End");
		#endif
	}

	// ------------------------------------------------------------
	// Chat OnSettingChanged
	// ------------------------------------------------------------
	void OnSettingChanged()
	{
		Destroy();
		
		if ( m_ContentRow )
			m_ContentRow.Unlink();
		
		if ( m_RootWidget )
		{
			m_Content = GridSpacerWidget.Cast( m_RootWidget.FindAnyWidget( "Content" ) );
			m_ScrollerContainer = Widget.Cast( m_RootWidget.FindAnyWidget( "ScrollerContainer" ) );
			m_Scroller = Widget.Cast( m_RootWidget.FindAnyWidget( "Scroller" ) );

			m_Position = 1;
			
			if ( Class.CastTo( m_ContentRow, UIActionManager.CreateActionRows( m_Content ) ) )
			{
				int totalInContentRow = 100;
				int currentContentRow = 0;

				GridSpacerWidget gsw;

				for ( int i = 0; i < 10; i++ )
				{
					if ( totalInContentRow >= 100 )
					{
						Class.CastTo( gsw, m_ContentRow.FindAnyWidget( "Content_Row_0" + currentContentRow ) );
						gsw.Show( true );
						currentContentRow++;
						totalInContentRow = 0;
					}
					
					CreateChatLines(gsw);
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Override Destroy
	// ------------------------------------------------------------
	override void Destroy()
	{
		m_ChatLines.Clear();
	}
	
	// ------------------------------------------------------------
	// Override Clear
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// Override Add
	// ------------------------------------------------------------
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
		else if(channel & ExpansionChatChannels.CCTeam)
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
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
		
	// ------------------------------------------------------------
	// Override AddInternal
	// ------------------------------------------------------------
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
			ScrollTo( 0 );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::AddInternal End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Chat Update
	// ------------------------------------------------------------
	void Update( float timeSlice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Update Start");
		#endif
		
		if ( m_IsMouseScrolling )
		{
			UpdateMouseScroll();
			UpdateScroller();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::Update End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Chat OnChatInputShow
	// ------------------------------------------------------------
	void OnChatInputShow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputShow Start");
		#endif
		
		ShowScroller();
		UpdateScroller();
		ShowChat();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputShow End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Chat OnChatInputHide
	// ------------------------------------------------------------
	void OnChatInputHide()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputHide Start");
		#endif

		HideScroller();
		ScrollTo( 0 );
		UpdateScroller();
		HideChat();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::OnChatInputHide End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Chat ScrollTo
	// ------------------------------------------------------------
	void ScrollTo( float x )
	{
		m_Position = x;
	}
		
	// ------------------------------------------------------------
	// Chat ShowScroller
	// ------------------------------------------------------------
	void ShowScroller()
	{
		m_ShowScroller = true;
	}
		
	// ------------------------------------------------------------
	// Chat HideScroller
	// ------------------------------------------------------------
	void HideScroller()
	{
		m_ShowScroller = false;
	}
			
	// ------------------------------------------------------------
	// Chat UpdateScroller
	// ------------------------------------------------------------
	void UpdateScroller()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Chat::UpdateScroller Start");
		#endif

		float width;
		float height;
		float diff;
		float scroller_height;
		float scroller_width;
	
		m_ScrollerContainer.GetScreenSize( scroller_width, m_RootHeight );
		m_Content.GetScreenSize( width, m_ContentHeight );

		float layoutRootWidth;
		float layoutRootHeight;
		m_RootWidget.GetScreenSize( layoutRootWidth, layoutRootHeight );

		diff = m_ContentHeight - m_RootHeight;
		
		if ( diff < 0 )
		{
			m_Content.SetPos( 0, m_RootHeight );
			m_ScrollerContainer.Show( false );
			m_Scroller.Show( false );

			m_Content.SetSize( layoutRootWidth, m_ContentHeight );
			m_Position = 0;
			
			#ifdef EXPANSIONEXPRINT
			EXPrint("Chat::UpdateScroller End");
			#endif
			
			return;
		}
		
		m_Content.SetSize( layoutRootWidth - scroller_width, m_ContentHeight );
		m_ScrollerContainer.SetSize( scroller_width, 1 );
		m_ScrollerContainer.SetPos( layoutRootWidth - scroller_width, 0 );
		m_ScrollerContainer.SetSort( 1 );
		
		scroller_height = ( m_RootHeight / m_ContentHeight ) * m_RootHeight;

		m_ScrollerContainer.Show( m_ShowScroller );
		m_Scroller.Show( m_ShowScroller );
		m_Scroller.GetSize( width, height );
		m_Scroller.SetSize( width, scroller_height );

		if ( m_Position < 0 )
			m_Position = 0;

		if ( m_Position > 1 )
			m_Position = 1;

		float scrollerPos = ( m_RootHeight - scroller_height ) * (1.0 - m_Position);

		float contentPos = m_RootHeight + ( ( m_ContentHeight - m_RootHeight ) * m_Position );

		m_Scroller.SetPos( 0, scrollerPos );
		m_Content.SetPos( 0, contentPos );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Chat::UpdateScroller End");
		#endif
	}
				
	// ------------------------------------------------------------
	// Chat UpdateMouseScroll
	// ------------------------------------------------------------
	void UpdateMouseScroll()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Chat::UpdateMouseScroll Start");
		#endif

		float posX;
		float posY;
		int mouse_x;
		int mouse_y;

		m_Scroller.GetScreenPos( posX, posY );
		GetMousePos( mouse_x, mouse_y );

		int wheel = 0;
		if ( posY > mouse_y - WHEEL_STEP )
		{
			wheel = 1;
		} else if ( posY < mouse_y + WHEEL_STEP )
		{
			wheel = -1;
		}

		float step = ( 1.0 / ( m_ContentHeight - m_RootHeight ) ) * WHEEL_STEP;
		m_Position += wheel * step;

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Chat::UpdateMouseScroll End");
		#endif
	}
				
	// ------------------------------------------------------------
	// Chat ShowChat
	// ------------------------------------------------------------
	void ShowChat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::ShowChat Start");
		#endif
		
		if (m_FadeInTimerChat)
			m_FadeInTimerChat.Stop();
		
		m_FadeInTimerChat = new WidgetFadeTimer;
		m_FadeInTimerChat.FadeIn(m_RootWidget, FADE_IN_DURATION);
		
		m_IsVisible = true;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::ShowChat End");
		#endif
	}
					
	// ------------------------------------------------------------
	// Chat HideChat
	// ------------------------------------------------------------
	void HideChat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::HideChat Start");
		#endif

		if (m_TimeOutTimerChat)
			m_TimeOutTimerChat.Stop();
		
		m_TimeOutTimerChat = new Timer(CALL_CATEGORY_GUI);
		m_TimeOutTimerChat.Run(CHAT_FADEOUT_TIME, m_FadeInTimerChat, "FadeOut", new Param2<Widget, float>(m_RootWidget, EXP_FADE_OUT_DURATION));
		
		m_IsVisible = false;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Chat::HideChat End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Chat GetContentHeight
	// ------------------------------------------------------------
	float GetContentHeight()
	{
		return m_ContentHeight;
	}
	
	// ------------------------------------------------------------
	// Chat GetContentHeight
	// ------------------------------------------------------------
	float GetRootHeight()
	{
		return m_RootHeight;
	}
	
	// ------------------------------------------------------------
	// Chat SetPosition
	// ------------------------------------------------------------
	void SetPosition(float pos)
	{
		m_Position = pos;
	}
	
	// ------------------------------------------------------------
	// Chat SetPosition
	// ------------------------------------------------------------
	void SetIsInputVisible(bool state)
	{
		m_IsChatInputVisible = state;
	}
		
	// ------------------------------------------------------------
	// Chat GetClientSettings
	// ------------------------------------------------------------
	void GetClientSettings()
	{
		if ( GetExpansionClientSettings() )
		{
			ExpansionClientUIChatSize chatsize = GetExpansionClientSettings().HUDChatSize;
			switch ( chatsize )
			{
				case ExpansionClientUIChatSize.VERYSMALL:
					m_LayoutPath = "DayZExpansion/GUI/layouts/chat/expansion_chat_entry_12.layout";
					break;
				case ExpansionClientUIChatSize.SMALL:
					m_LayoutPath = "DayZExpansion/GUI/layouts/chat/expansion_chat_entry_14.layout";
					break;
				case ExpansionClientUIChatSize.MEDIUM:
					m_LayoutPath = "DayZExpansion/GUI/layouts/chat/expansion_chat_entry_16.layout";
					break;
				case ExpansionClientUIChatSize.LARGE:
					m_LayoutPath = "DayZExpansion/GUI/layouts/chat/expansion_chat_entry_22.layout";
					break;
			}

			CHAT_FADEOUT_TIME = Math.Round( GetExpansionClientSettings().HUDChatFadeOut );
		}
	}
	
	bool IsChatVisible()
	{
		return m_IsVisible;
	}
}