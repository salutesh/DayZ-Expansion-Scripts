modded class MissionGameplay
{
	void MissionGameplay()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "MissionGameplay");
		#endif

		CF_Log.Info("eAI - Loaded Client Mission");
	}

	override void OnMissionStart()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "OnMissionStart");
		#endif

		super.OnMissionStart();
	}

	override void OnMissionFinish()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "OnMissionFinish");
		#endif

		super.OnMissionFinish();

		GetDayZGame().GetExpansionGame().SetInGroup(false);
	}

	override void OnEvent(EventType eventTypeId, Param params)
	{
		switch (eventTypeId)
		{
			case ChatMessageEventTypeID:
				ChatMessageEventParams chat_params = ChatMessageEventParams.Cast(params);
				//! Intercept AI messages and alter them in-place
				if (chat_params.param1 == ChatChannelType.System && (chat_params.param4 == "AI" || chat_params.param4 == "friendlyAI"))
				{
					int index = chat_params.param3.IndexOf(":");
					if (index > -1)
					{
						//! Change channel type so that sender field is used by Expansion Chat
						if (chat_params.param4 == "friendlyAI")
							chat_params.param1 = ExpansionChatChannels.CCTeam;
						else
							chat_params.param1 = ChatChannelType.Direct;

						//! Extract AI name from message body and place into sender field
						chat_params.param2 = chat_params.param3.Substring(0, index);

						//! Remove AI name from message body
						chat_params.param3 = chat_params.param3.Substring(index + 1, chat_params.param3.Length() - index - 1);
					}

					//! Remove color style
					chat_params.param4 = "";
				}
				break;
		}

		super.OnEvent(eventTypeId, params);
	}

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnUpdate").Add(timeslice);
		#endif

		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

#ifdef DIAG_DEVELOPER
		if (player && player.DEBUG_EXPANSION_AI_VEHICLE && g_Expansion_Car && g_ExpansionNavMesh)
		{
			g_ExpansionNavMesh.DebugScripts();
		}
#endif

		// If we want to open the command menu, and nothing else is open
		if (!inputIsFocused && input.LocalPress("eAICommandMenu", false) && !menu && !viewMenu)
		{
			if (GetExpansionSettings().GetAI().IsAdmin() || (player && player.GetGroup()))
			{
				eAICommandMenu.OpenMenu();
			}
		}

		// If we want to close the command menu, and our menu is open
		if (input.LocalRelease("eAICommandMenu", false) || input.LocalValue("eAICommandMenu", false) == 0)
		{
			if (menu && menu == eAICommandMenu.instance)
			{
				eAICommandMenu.instance.OnMenuRelease();
				GetUIManager().Back();
			}
		}

		if (!inputIsFocused && input.LocalPress("eAISetWaypoint", false) && !menu && !viewMenu)
		{
			if (player && player.GetGroup() && !player.IsRaised())
			{
				GetDayZExpansion().GetCommandManager().Send(eAICommands.MOV_SETWP, eAICommandCategories.CAT_MOVEMENT);
			}
		}
		
		// Show/hide & update ai aggresion cooldown timer
		if (player && player.GetHumanInventory())
		{
			bool cooldownVisible = m_Hud.Expansion_IsAggressionCooldownVisible();
			int aggressionCooldown = player.eAI_GetLastAggressionCooldown();
			if (m_Hud.Expansion_CanShowHUDElements(player) && aggressionCooldown > 0)
			{
				if (!cooldownVisible)
					m_Hud.Expansion_ShowAggressionCooldown(true);

				m_Hud.Expansion_UpdateAggressionCooldown(player, aggressionCooldown);
			}
			else
			{
				if (cooldownVisible)
					m_Hud.Expansion_ShowAggressionCooldown(false);
			}
		}
	}

	override void HideInventory()
	{
		super.HideInventory();

		PlayerBase player;
		if (Class.CastTo(player, g_Game.GetPlayer()))
		{
			eAIGroup group = player.GetGroup();
			if (group)
			{
				for (int i = 0; i < group.Count(); ++i)
				{
					eAIBase ai;
					if (Class.CastTo(ai, group.GetMember(i)) && ai.m_eAI_IsInventoryVisible)
						ai.eAI_SetIsInventoryVisible(false, player);
				}
			}
		}
	}
};
