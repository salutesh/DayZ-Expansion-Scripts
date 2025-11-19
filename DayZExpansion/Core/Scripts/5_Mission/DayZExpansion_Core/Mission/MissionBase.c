modded class MissionBase
{
	CF_EventUpdateArgs m_Expansion_CF_EventUpdateArgs = new CF_EventUpdateArgs;

	void MissionBase()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
#endif

		GetDayZGame().Expansion_SetIsMissionMainMenu(IsInherited(MissionMainMenu));

		CreateDayZExpansion();
	}

	void ~MissionBase()
	{
		if (!g_Game)
			return;

		DestroyDayZExpansion();

#ifdef DIAG_DEVELOPER
		Print("[DayZ Expansion] ~MissionBase");
#endif

		delete m_Expansion_CF_EventUpdateArgs;
	}

	override void CF_OnUpdate(float timeslice)
	{
#ifdef SERVER
		float updateTime = g_Game.GetTickTime();
		float elapsed = updateTime - m_CF_UpdateTime;
		bool update = elapsed >= 0.025;

		if (update)
		{
			m_CF_UpdateTime = updateTime;
		}
#endif

		if (!m_bLoaded)
		{
			if (g_Game.IsLoading())
			{
				return;
			}

			m_bLoaded = true;
			CF_ModuleGameManager.UpdateGameFlag();  //! CF calls UpdateGameFlag each update loop which is redundant. Call it once when loaded instead. TODO: This should be in CF.
			OnMissionLoaded();
		}

		//! CF creates a new CF_EventUpdateArgs instance each update loop. Instantiate once & reuse instead.
		//! TODO: This should be in CF.
#ifdef SERVER
		if (update)
		{
			m_Expansion_CF_EventUpdateArgs.DeltaTime = elapsed;
			CF_ModuleCoreManager.s_Update.OnUpdate(this, m_Expansion_CF_EventUpdateArgs);
		}
#else
		m_Expansion_CF_EventUpdateArgs.DeltaTime = timeslice;
		CF_ModuleCoreManager.s_Update.OnUpdate(this, m_Expansion_CF_EventUpdateArgs);
#endif
	}
};
