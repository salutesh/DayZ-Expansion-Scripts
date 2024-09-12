modded class MissionBase
{
#ifdef SERVER
	protected float m_Expansion_UpdateTime;
#endif

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
		if (!GetGame())
			return;

#ifdef DIAG_DEVELOPER
		Print("~MissionBase");
#endif
	}

#ifndef CF_ONUPDATE_RATE_LIMIT
	override void CF_OnUpdate(float timeslice)
	{
#ifdef SERVER
		float updateTime = GetGame().GetTickTime();
		float elapsed = updateTime - m_Expansion_UpdateTime;
		bool update;
		if (elapsed >= 0.025)
			update = true;

		if (update)
		{
			m_Expansion_UpdateTime = updateTime;
		}
#endif

		if (!m_bLoaded)
		{
			if (g_Game.IsLoading())
			{
				return;
			}

			m_bLoaded = true;
			OnMissionLoaded();
		}

#ifdef SERVER
		if (update)
		{
			CF_ModuleGameManager.OnUpdate(this, new CF_EventUpdateArgs(elapsed));
		}
#else
		CF_ModuleGameManager.OnUpdate(this, new CF_EventUpdateArgs(timeslice));
#endif
	}
#endif
};
