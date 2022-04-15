/**
 * ExpansionChat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	ref WidgetFadeTimer m_FadeOutTimerChat;
	
	private GridSpacerWidget m_ContentRow;
	
	private string m_LayoutPath;

	private bool m_HideChatToggle = true;

	void Chat()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Chat");
#endif

		m_ChatParams = new array< ref ExpansionChatMessage >;
		m_ChatLines = new array< ref ExpansionChatLine >;

		m_FadeInTimerChat = new WidgetFadeTimer;
		m_FadeOutTimerChat = new WidgetFadeTimer;

		GetExpansionClientSettings().SI_UpdateSetting.Insert(OnSettingChanged);
				
		GetClientSettings();
	}

	void ~Chat()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "~Chat");
#endif

		GetExpansionClientSettings().SI_UpdateSetting.Remove( OnSettingChanged );
	}

	override void Init(Widget root_widget)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Init");
#endif

		Destroy();
		
		m_RootWidget = root_widget;
		
		if (m_RootWidget)
		{
			m_Content = GridSpacerWidget.Cast(m_RootWidget.FindAnyWidget("Content"));
			m_ScrollerContainer = Widget.Cast(m_RootWidget.FindAnyWidget("ScrollerContainer"));
			m_Scroller = Widget.Cast(m_RootWidget.FindAnyWidget("Scroller"));

			m_Position = 1;
			
			if ( Class.CastTo( m_ContentRow, CreateActionRows( m_Content ) ) )
			{
				int totalInContentRow = 100;
				int currentContentRow = 0;

				GridSpacerWidget gsw;

				for (int i = 0; i < 10; i++)
				{
					if (totalInContentRow >= 100)
					{
						Class.CastTo(gsw, m_ContentRow.FindAnyWidget("Content_Row_0" + currentContentRow));
						gsw.Show(true);
						currentContentRow++;
						totalInContentRow = 0;
					}
					
					CreateChatLines(gsw);
				}
			}
		}
	}

	static Widget CreateActionRows( Widget parent )
	{
		Widget widget = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/Chat/GUI/layouts/expansion_chat_contentrows.layout", parent );

		return widget;
	}

	void CreateChatLines(GridSpacerWidget gsw)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "CreateChatLines");
#endif
		
		for (int j = 0; j < 100; j++)
		{
			m_ChatLines.Insert(new ExpansionChatLine(gsw, m_LayoutPath, this));
		}
	}

	void OnSettingChanged()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "OnSettingChanged");
#endif
		
		GetClientSettings();
		
		Destroy();
		
		if (m_ContentRow)
			m_ContentRow.Unlink();
		
		if (m_RootWidget)
		{
			m_Content = GridSpacerWidget.Cast(m_RootWidget.FindAnyWidget("Content"));
			m_ScrollerContainer = Widget.Cast(m_RootWidget.FindAnyWidget("ScrollerContainer"));
			m_Scroller = Widget.Cast(m_RootWidget.FindAnyWidget("Scroller"));

			m_Position = 1;
			
			if ( Class.CastTo( m_ContentRow, CreateActionRows( m_Content ) ) )
			{
				int totalInContentRow = 100;
				int currentContentRow = 0;

				GridSpacerWidget gsw;

				for (int i = 0; i < 10; i++)
				{
					if (totalInContentRow >= 100)
					{
						Class.CastTo(gsw, m_ContentRow.FindAnyWidget("Content_Row_0" + currentContentRow));
						gsw.Show(true);
						currentContentRow++;
						totalInContentRow = 0;
					}
					
					CreateChatLines(gsw);
				}
			}
		}
	}

	override void Destroy()
	{
		m_ChatLines.Clear();
	}

	override void Clear()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Clear");
#endif

		if (m_ChatLines.Count() >= 0)
		{
			for (int i = 0; i < m_ChatLines.Count(); i++)
			{
				m_ChatLines.Get(i).Clear();
			}
		}
	}

	override void Add(ChatMessageEventParams params)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Add");
#endif

		int channel =  params.param1;

		if (channel & CCSystem)
 		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.GAME_MESSAGES))
			{
				return;
			}
 		} else if (channel & CCAdmin || channel & CCBattlEye)
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.ADMIN_MESSAGES))
			{
				return;
			}
		}
		else if (channel & CCDirect || channel & CCMegaphone || channel & CCTransmitter || channel & CCPublicAddressSystem) 
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
			{
				return;
			}
		}
		
		//! Expansion messages
		else if (channel & ExpansionChatChannels.CCTransport) 
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
			{
				return;
			}
		}
		else if (channel & ExpansionChatChannels.CCGlobal) 
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
			{
				return;
			}
		}
		else if (channel & ExpansionChatChannels.CCTeam)
		{
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
			{
				return;
			}
		}

		AddInternal( params );
	}

	override void AddInternal(ChatMessageEventParams params)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "AddInternal");
#endif

		ExpansionChatMessage message = new ExpansionChatMessage();
		message.Channel = params.param1;
		message.From = params.param2;
		message.Text = params.param3;
		
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

		m_ChatParams.InsertAt(message, 0);

		while (m_ChatParams.Count() > MAX_CHAT_HISTORY)
		{
			m_ChatParams.Remove(m_ChatParams.Count() - 1);
		}

		for (int i = 0; i < m_ChatLines.Count(); i++)
		{
			int idx = 1000 - (i + 1);

			if (i < m_ChatParams.Count())
			{
				m_ChatLines[idx].Set(m_ChatParams[i]);
			} else
			{
				m_ChatLines[idx].Set(NULL);
			}
		}

		UpdateScroller();

		if (!m_IsChatInputVisible)
		{
			ScrollTo(0);
		}
		
		//! This will allow chat messages to fade out
		HideChat();
	}
	
	void Update(float timeSlice)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "Update");
#endif
		
		if (m_IsMouseScrolling)
		{
			UpdateMouseScroll();
			UpdateScroller();
		}
	}

	void HideChatToggle()
	{
		m_HideChatToggle = !m_HideChatToggle;
		m_RootWidget.Show(m_HideChatToggle);

		if (m_FadeOutTimerChat)
			m_FadeOutTimerChat.Stop();

		if (m_FadeInTimerChat)
			m_FadeInTimerChat.Stop();
	}

	bool GetChatToggleState()
	{
		return m_HideChatToggle;
	}

	void OnChatInputShow()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "OnChatInputShow");
#endif
		
		if (GetChatToggleState())
		{
			ShowScroller();
			ShowChat();
		}

		UpdateScroller();
	}

	void OnChatInputHide()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "OnChatInputHide");
#endif

		HideScroller();
		ScrollTo(0);
		UpdateScroller();

		HideChat();
	}

	void ScrollTo(float x)
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "UpdateScroller");
#endif

		float width;
		float height;
		float diff;
		float scroller_height;
		float scroller_width;
	
		m_ScrollerContainer.GetScreenSize(scroller_width, m_RootHeight);
		m_Content.GetScreenSize(width, m_ContentHeight);

		float layoutRootWidth;
		float layoutRootHeight;
		m_RootWidget.GetScreenSize(layoutRootWidth, layoutRootHeight);

		diff = m_ContentHeight - m_RootHeight;
		
		if (diff < 0)
		{
			m_Content.SetPos(0, m_RootHeight);
			m_ScrollerContainer.Show(false);
			m_Scroller.Show(false);

			m_Content.SetSize(layoutRootWidth, m_ContentHeight);
			m_Position = 0;
			
			return;
		}
		
		m_Content.SetSize(layoutRootWidth - scroller_width, m_ContentHeight);
		m_ScrollerContainer.SetSize(scroller_width, 1);
		m_ScrollerContainer.SetPos(layoutRootWidth - scroller_width, 0);
		m_ScrollerContainer.SetSort(1);
		
		scroller_height = (m_RootHeight / m_ContentHeight) * m_RootHeight;

		m_ScrollerContainer.Show(m_ShowScroller);
		m_Scroller.Show(m_ShowScroller);
		m_Scroller.GetSize(width, height);
		m_Scroller.SetSize(width, scroller_height);

		if (m_Position < 0)
			m_Position = 0;

		if (m_Position > 1)
			m_Position = 1;

		float scrollerPos = (m_RootHeight - scroller_height) * (1.0 - m_Position);

		float contentPos = m_RootHeight + ((m_ContentHeight - m_RootHeight) * m_Position);

		m_Scroller.SetPos( 0, scrollerPos );
		m_Content.SetPos( 0, contentPos );
	}

	void UpdateMouseScroll()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "UpdateMouseScroll");
#endif

		float posX;
		float posY;
		int mouse_x;
		int mouse_y;

		m_Scroller.GetScreenPos(posX, posY);
		GetMousePos(mouse_x, mouse_y);

		int wheel = 0;
		if (posY > mouse_y - WHEEL_STEP)
		{
			wheel = 1;
		} else if (posY < mouse_y + WHEEL_STEP)
		{
			wheel = -1;
		}

		float step = (1.0 / (m_ContentHeight - m_RootHeight)) * WHEEL_STEP;
		m_Position += wheel * step;
	}

	void ShowChat()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "ShowChat");
#endif

		if (m_FadeOutTimerChat)
			m_FadeOutTimerChat.Stop();
		
		m_FadeInTimerChat.FadeIn(m_RootWidget, FADE_IN_DURATION);
	}

	void HideChat()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHAT, this, "HideChat");
#endif

		if (m_FadeInTimerChat)
			m_FadeInTimerChat.Stop();

		if (CHAT_FADEOUT_TIME > 0)
			m_FadeOutTimerChat.FadeOut(m_RootWidget, CHAT_FADEOUT_TIME);
	}

	float GetContentHeight()
	{
		return m_ContentHeight;
	}

	float GetRootHeight()
	{
		return m_RootHeight;
	}

	void SetPosition(float pos)
	{
		m_Position = pos;
	}

	void SetIsInputVisible(bool state)
	{
		m_IsChatInputVisible = state;
	}

	void GetClientSettings()
	{
		if (GetExpansionClientSettings())
		{
			ExpansionClientUIChatSize chatsize = GetExpansionClientSettings().HUDChatSize;
			switch (chatsize)
			{
				case ExpansionClientUIChatSize.VERYSMALL:
					m_LayoutPath = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_verysmall.layout";
					break;
				case ExpansionClientUIChatSize.SMALL:
					m_LayoutPath = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_small.layout";
					break;
				case ExpansionClientUIChatSize.MEDIUM:
					m_LayoutPath = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_medium.layout";
					break;
				case ExpansionClientUIChatSize.LARGE:
					m_LayoutPath = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_large.layout";
					break;
				case ExpansionClientUIChatSize.VERYLARGE:
					m_LayoutPath = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_verylarge.layout";
					break;
				default:
					m_LayoutPath = "DayZExpansion/Chat/GUI/layouts/expansion_chat_entry_large.layout";
					break;
			}

			CHAT_FADEOUT_TIME = Math.Round(GetExpansionClientSettings().HUDChatFadeOut);
		}
	}
};
