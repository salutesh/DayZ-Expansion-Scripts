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
	protected EditBoxWidget m_edit_box;
	protected float m_Position;
	protected ref Chat m_Chat;

	override Widget Init()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		m_BackInputWrapper = GetUApi().GetInputByID(UAUIBack).GetPersistentWrapper();

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

#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		string text = m_edit_box.GetText();

		if (text != "")
		{
			MissionGameplay gameplayMission = MissionGameplay.Cast(GetGame().GetMission());

			bool useExpansionChat = false;

			switch (gameplayMission.GetChatChannel())
			{
				case ExpansionChatChannels.CCDirect:
					if (text[0] == "#")  //! Allow vanilla chat commands
						break;
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
					ExpansionChatMessageEventParams chat_params_rpc = new ExpansionChatMessageEventParams(gameplayMission.GetChatChannel(), "", text, "");
					ExpansionGlobalChatModule module;
					CF_Modules<ExpansionGlobalChatModule>.Get(module);
					auto rpc = module.Expansion_CreateRPC("RPC_AddChatMessage");
					rpc.Write(chat_params_rpc);
					rpc.Expansion_Send(true);
				}
				else
				{
					string name;
					GetGame().GetPlayerName(name);

					ExpansionChatMessageEventParams chat_params = new ExpansionChatMessageEventParams(gameplayMission.GetChatChannel(), name, text, "");
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		super.OnShow();

		if (m_Chat)
			m_Chat.OnChatInputShow();
	}

	override void OnHide()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		if (GetGame() && GetGame().GetMission())  //! Prevent NULL pointer on game exit
			super.OnHide();

		if (m_Chat)
			m_Chat.OnChatInputHide();
	}

	EditBoxWidget GetEditboxWidget()
	{
		return m_edit_box;
	}
};
