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

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnUpdate").Add(timeslice);
		#endif

		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

#ifdef DIAG
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

		if (!inputIsFocused && input.LocalPress("eAISetWaypoint", false))
		{
			if (player && player.GetGroup() && !player.IsRaised())
			{
				GetDayZExpansion().GetCommandManager().Send(eAICommands.MOV_SETWP);
			}
		}
		
		if (player && player.GetHumanInventory())
		{
			if (player.GetPlayerState() == EPlayerStates.ALIVE && !player.IsUnconscious())
			{
				if (viewMenu || menu)
				{
					m_Hud.Expansion_GetCooldownIndicator().Hide();
				}
				else
				{
					if (player.eAI_GetLastAggressionCooldown() > 0)
					{
						m_Hud.Expansion_GetCooldownIndicator().Show();
					}
					else
					{
						m_Hud.Expansion_GetCooldownIndicator().Hide();
					}
				}
			}
			else
			{
				m_Hud.Expansion_GetCooldownIndicator().Hide();
			}
		}
	}
};
