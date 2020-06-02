/**
 * ExpansionChatHandler.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionChatHandler extends ScriptedWidgetEventHandler
{
	const int WHEEL_STEP = 20;
	const float CHAT_FADEOUT_TIME = 3;
	const float FADE_OUT_DURATION = 3;
	const float FADE_IN_DURATION = 0.5;

	Chat m_Chat;

	Widget layoutRoot;

	Widget m_Content;
	Widget m_ContentBorder;
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

	private ref WidgetFadeTimer m_FadeInTimerScroller;
	private ref WidgetFadeTimer m_FadeInTimerBorder;
	private ref Timer m_TimeOutTimerScroller;
	private ref Timer m_TimeOutTimerBorder;

	void ExpansionChatHandler(Chat chat)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::ExpansionChatHandler Start");
		#endif
		
		m_Chat = chat;

		layoutRoot = m_Chat.GetRootWidget();

		m_Content = layoutRoot.FindAnyWidget( "Content" );
		m_ContentBorder = layoutRoot.FindAnyWidget( "FrameBorder" );
		m_ScrollerContainer = layoutRoot.FindAnyWidget( "ScrollerContainer" );
		m_Scroller = layoutRoot.FindAnyWidget( "Scroller" );
	
		m_Position = 1;

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::ExpansionChatHandler End");
		#endif
	}

	Widget GetContentWidget()
	{
		return m_Content;
	}

	void ScrollTo( float x )
	{
		m_Position = x;
	}

	void ShowScroller()
	{
		m_ShowScroller = true;
	}

	void HideScroller()
	{
		m_ShowScroller = false;
	}

	void UpdateScroller()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::UpdateScroller Start");
		#endif

		m_Content.Update();
		m_ContentBorder.Update();
		m_ScrollerContainer.Update();
		m_Scroller.Update();
		layoutRoot.Update();

		float width;
		float height;
		float diff;
		float scroller_height;
		float scroller_width;
	
		m_ScrollerContainer.GetScreenSize( scroller_width, m_RootHeight );
		m_Content.GetScreenSize( width, m_ContentHeight );

		float layoutRootWidth;
		float layoutRootHeight;
		layoutRoot.GetScreenSize( layoutRootWidth, layoutRootHeight );

		diff = m_ContentHeight - m_RootHeight;
		
		if ( diff < 0 )
		{
			m_Content.SetPos( 0, m_RootHeight );
			m_ScrollerContainer.Show( false );
			m_Scroller.Show( false );

			m_Content.SetSize( layoutRootWidth, m_ContentHeight );
			m_Position = 0;
			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatHandler::UpdateScroller End");
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
		EXLogPrint("ExpansionChatHandler::UpdateScroller End");
		#endif
	}

	protected void UpdateDragScroll( int mouse_x, int mouse_y, bool is_dragging )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::UpdateDragScroll Start");
		#endif
		
		layoutRoot.Update();
		m_Content.Update();
		float width;
	
		layoutRoot.GetScreenSize( width, m_RootHeight );
		m_Content.GetScreenSize( width, m_ContentHeight );
		
		if ( m_IsDragScrolling )
		{
			if ( is_dragging )
			{
				float diff = ( m_MouseStartPos - mouse_y );
				float scroller_height = ( m_RootHeight / m_ContentHeight ) * m_RootHeight; 
				m_Position = m_ScrollStartPos + ( diff / ( m_RootHeight - scroller_height ) );
			} 
			else
			{
				m_IsDragScrolling = false;
				StopDragScrolling();
			}
		}
		
		UpdateScroller();

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::UpdateDragScroll End");
		#endif
	}

	protected void UpdateMouseScroll()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::UpdateMouseScroll Start");
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
		EXLogPrint("ExpansionChatHandler::UpdateMouseScroll End");
		#endif
	}

	protected void StopDragScrolling()
	{
		if ( m_IsDragScrolling )
		{
			m_IsDragScrolling = false;
		}
	}

	protected void StopMouseScrolling()
	{
		if ( m_IsMouseScrolling )
		{
			m_IsMouseScrolling = false;
		}
	}

	void Update( float timeSlice )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::Update Start");
		#endif

		if ( m_IsMouseScrolling )
		{
			UpdateMouseScroll();
			UpdateScroller();
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionChatHandler::Update End");
		#endif
	}

	override bool OnMouseButtonDown( Widget w, int x, int y, int button)
	{
		if ( button != MouseState.LEFT )
		{
			return false;
		}

		if ( w == m_ScrollerContainer && !m_IsMouseScrolling && !m_IsDragScrolling )
		{
			m_IsMouseScrolling = true;
			
			return true;
		}

		if ( w == m_Scroller && !m_IsMouseScrolling && !m_IsDragScrolling )
		{
			m_IsDragScrolling = true;
			m_ScrollStartPos = m_Position;
			int mouse_x;
			GetMousePos( mouse_x, m_MouseStartPos );
			GetGame().GetDragQueue().Call( this, "UpdateDragScroll" );
			return true;
		}
	
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button)
	{
		StopDragScrolling();
		StopMouseScrolling();

		return false;
	}
	
	override bool OnMouseWheel( Widget w, int x, int y, int wheel )
	{
		if ( !IsMouseOnChat() )
		{
			return false;
		}
		
		/*if ( m_IsDragScrolling || m_ContentHeight <= m_RootHeight )
			return false;*/
	
		float step = ( 1.0 / ( m_ContentHeight - m_RootHeight ) ) * WHEEL_STEP;
		m_Position += wheel * step;

		UpdateScroller();

		return true;
	}
	
	//------------------------------------------
	// IsMouseInScene
	//------------------------------------------
	private bool IsMouseOnChat()
	{
		Widget widget_under_mouse = GetWidgetUnderCursor();
		
		if ( widget_under_mouse != NULL )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionChatHandler::IsMouseOnChat - widget_under_mouse: " + widget_under_mouse.GetName());
			#endif
		}
			
		if ( widget_under_mouse != NULL && widget_under_mouse.GetName() == "ExpansionChatFrame" )
		{
			return true;
		}
		
		if ( widget_under_mouse != NULL && widget_under_mouse.GetName() == "ChatFrameWidget" )
		{
			return true;
		}
		
		return false;
	}

	void ShowChat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatHandler::ShowChat Start");
		#endif
		
		if (m_FadeInTimerScroller)
			m_FadeInTimerScroller.Stop();
		
		if (m_FadeInTimerBorder)
			m_FadeInTimerBorder.Stop();
		
		m_FadeInTimerScroller	= new WidgetFadeTimer;
		m_FadeInTimerScroller.FadeIn(m_Scroller, FADE_IN_DURATION);
		
		m_FadeInTimerBorder	= new WidgetFadeTimer;
		m_FadeInTimerBorder.FadeIn(m_ContentBorder, FADE_IN_DURATION);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatHandler::ShowChat End");
		#endif
	}

	void HideChat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatHandler::HideChat Start");
		#endif
		
		if (m_TimeOutTimerScroller)
			m_TimeOutTimerScroller.Stop();
		
		if (m_TimeOutTimerBorder)
			m_TimeOutTimerBorder.Stop();

		m_TimeOutTimerScroller = new Timer(CALL_CATEGORY_GUI);
		m_TimeOutTimerScroller.Run(CHAT_FADEOUT_TIME, m_FadeInTimerScroller, "FadeOut", new Param2<Widget, float>(m_Scroller, FADE_OUT_DURATION));
		
		m_TimeOutTimerBorder = new Timer(CALL_CATEGORY_GUI);
		m_TimeOutTimerBorder.Run(CHAT_FADEOUT_TIME, m_FadeInTimerBorder, "FadeOut", new Param2<Widget, float>(m_ContentBorder, FADE_OUT_DURATION));
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatHandler::HideChat End");
		#endif
	}
}