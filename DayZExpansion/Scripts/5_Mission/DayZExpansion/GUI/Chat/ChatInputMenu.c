/**
 * ChatInputMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ChatInputMenu
{
	static ref ScriptInvoker SI_OnChatInputShow = new ScriptInvoker();
	static ref ScriptInvoker SI_OnChatInputHide = new ScriptInvoker();

	private EditBoxWidget m_edit_box;

	void ChatInputMenu()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::ChatInputMenu Start");
		#endif
		MissionGameplay.Cast( GetGame().GetMission() ).m_WidgetChatChannel.Show(true);
		MissionGameplay.Cast( GetGame().GetMission() ).m_ChatChannelName.Show(true);

		MissionGameplay.Cast( GetGame().GetMission() ).m_WidgetChatChannel.SetAlpha(0.75);
		MissionGameplay.Cast( GetGame().GetMission() ).m_ChatChannelName.SetAlpha(1);

		GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Insert( this.Update );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::ChatInputMenu End");
		#endif
	}

	void ~ChatInputMenu()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::~ChatInputMenu Start");
		#endif
		MissionGameplay.Cast( GetGame().GetMission() ).m_WidgetChatChannel.Show(false);
		MissionGameplay.Cast( GetGame().GetMission() ).m_ChatChannelName.Show(false);

		MissionGameplay.Cast( GetGame().GetMission() ).m_WidgetChatChannel.SetAlpha(0);
		MissionGameplay.Cast( GetGame().GetMission() ).m_ChatChannelName.SetAlpha(0);

		GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Remove( this.Update );

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater( GetGame().GetMission().HideChat, 300, false);
		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::~ChatInputMenu End");
		#endif
	}

	// Don't want the vanilla layout to load at all...
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::Init Start");
		#endif
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/chat/expansion_chat_input.layout" );
		m_edit_box = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "InputEditBoxWidget" ) );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::Init End");
		#endif
		return layoutRoot;
	}

	override bool UseKeyboard()
	{
		return true;
	}
	
	override bool UseMouse()
	{
		return true;
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
	{		
		if ( !finished )
			return false;

		string text = m_edit_box.GetText();

		if ( text != "" )
		{
			MissionGameplay gameplayMission = MissionGameplay.Cast( GetGame().GetMission() );

			bool useExpansionChat = false;

			if ( gameplayMission.GetChatChannel() == ExpansionChatChannels.CCGlobal ) 
			{
				useExpansionChat = true;
			} 
			else if ( gameplayMission.GetChatChannel() == ExpansionChatChannels.CCAdmin ) 
			{
				useExpansionChat = true;
			}

			if ( useExpansionChat )
			{
				if ( GetGame().IsMultiplayer() )
				{
					ChatMessageEventParams chat_params_rpc = new ChatMessageEventParams( gameplayMission.GetChatChannel(), "", text, "" );
					ScriptRPC rpc = new ScriptRPC;
					rpc.Write(chat_params_rpc);
					rpc.Send( null, ExpansionGlobalChatRPC.AddChatMessage, true );
				} 
				else
				{
					string name;
					GetGame().GetPlayerName( name );

					ChatMessageEventParams chat_params = new ChatMessageEventParams( gameplayMission.GetChatChannel(), name, text, "" );
					gameplayMission.m_Chat.Add( chat_params );
				}
				
				m_close_timer.Run(0.1, this, "Close");

				#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::OnChange End");
		#endif
				return true;
			}
		}
		
		SupressInputs();
		
		return super.OnChange( w, x, y, finished );
	}

	void SupressInputs()
	{
		GetUApi().GetInputByName("UAPersonView").Supress();	
		GetUApi().GetInputByName("UAMoveForward").Supress();	
		GetUApi().GetInputByName("UAMoveBack").Supress();
		GetUApi().GetInputByName("UAMoveLeft").Supress();
		GetUApi().GetInputByName("UAMoveRight").Supress();
		GetUApi().GetInputByName("UAGetOver").Supress();
		GetUApi().GetInputByName("UAStance").Supress();
	}
	
	override void OnShow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::OnShow Start");
		#endif
		
		SetFocus( m_edit_box );
		
		SupressInputs();
		
		SI_OnChatInputShow.Invoke();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::OnShow End");
		#endif
	}
	
	override void OnHide()
	{
		super.OnHide();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::OnHide Start");
		#endif
		
		SI_OnChatInputHide.Invoke();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ChatInputMenu::OnHide End");
		#endif
	}
}
