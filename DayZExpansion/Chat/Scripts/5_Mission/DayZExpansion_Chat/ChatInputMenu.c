/**
 * ChatInputMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ChatInputMenu 
{
	const int WHEEL_STEP = 20;
	private EditBoxWidget m_edit_box;
	private float m_Position;
	private ref Chat m_Chat;
	
	// Don't want the vanilla layout to load at all...
	override Widget Init()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Chat/GUI/layouts/expansion_chat_input.layout");
		m_edit_box = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("InputEditBoxWidget"));

		m_Position = 1;
		
		MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
		if (mission && mission.GetChat())
			m_Chat = mission.GetChat();
		
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

	override bool OnChange(Widget w, int x, int y, bool finished)
	{				
		if (!finished)
			return false;

		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		string text = m_edit_box.GetText();

		if (text != "")
		{
			MissionGameplay gameplayMission = MissionGameplay.Cast(GetGame().GetMission());

			bool useExpansionChat = false;

			switch (gameplayMission.GetChatChannel())
			{
				case ExpansionChatChannels.CCGlobal:
				case ExpansionChatChannels.CCTeam:
				case ExpansionChatChannels.CCTransport:
				case ExpansionChatChannels.CCAdmin:
					useExpansionChat = true;
					break;
			}

			if (useExpansionChat)
			{
				if (GetGame().IsMultiplayer())
				{
					ChatMessageEventParams chat_params_rpc = new ChatMessageEventParams(gameplayMission.GetChatChannel(), "", text, "");
					ScriptRPC rpc = new ScriptRPC;
					rpc.Write(chat_params_rpc);
					rpc.Send(null, ExpansionGlobalChatRPC.AddChatMessage, true);
				} 
				else
				{
					string name;
					GetGame().GetPlayerName(name);

					ChatMessageEventParams chat_params = new ChatMessageEventParams(gameplayMission.GetChatChannel(), name, text, "");
					gameplayMission.m_Chat.Add(chat_params);
				}
				
				m_close_timer.Run(0.1, this, "Close");
				
				return true;
			}
		}
		
		return super.OnChange(w, x, y, finished);
	}

	override void OnShow()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);
		
		super.OnShow();
		
		if (m_Chat)
			m_Chat.OnChatInputShow();
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);
		
		//! Controlls scroll steps in the chat panel grid
		float step = (1.0 / (m_Chat.GetChatWindow().GetContentHeight() - m_Chat.GetChatWindow().GetRootHeight())) * WHEEL_STEP;
		m_Position += wheel * step;

		//m_Chat.GetChatWindow().SetPosition(m_Position);
		//m_Chat.GetChatWindow().UpdateScroller();
		
		return true;
	}
	
	override void OnHide()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT);

		super.OnHide();
			
		if (m_Chat)
			m_Chat.OnChatInputHide();
	}
};
