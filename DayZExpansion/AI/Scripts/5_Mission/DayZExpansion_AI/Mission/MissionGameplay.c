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
	}

	override void OnUpdate(float timeslice)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnUpdate").Add(timeslice);
		#endif

		super.OnUpdate(timeslice);

		DayZPlayerImplement player;
		Class.CastTo(player, GetGame().GetPlayer());

#ifdef DIAG
		if (player && g_Expansion_Car && g_ExpansionNavMesh)
		{
			g_ExpansionNavMesh.DebugScripts();
		}
#endif

		//TODO: move to 5_Mission/DayZExpansion

		Input input = GetGame().GetInput();

		// If we want to open the command menu, and nothing else is open
		if (input.LocalPress("eAICommandMenu", false) && !GetGame().GetUIManager().GetMenu())
		{
			if (GetExpansionSettings().GetAI().IsAdmin() || (player && player.GetGroup()))
			{
				eAICommandMenu.OpenMenu();
			}
		}

		// If we want to close the command menu, and our menu is open
		if (input.LocalRelease("eAICommandMenu", false) || input.LocalValue("eAICommandMenu", false) == 0)
		{
			auto menu = GetGame().GetUIManager().GetMenu();
			if (menu && menu == eAICommandMenu.instance)
			{
				eAICommandMenu.instance.OnMenuRelease();
				GetUIManager().Back();
			}
		}
	}
};
