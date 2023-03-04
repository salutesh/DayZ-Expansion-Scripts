modded class MissionBase
{
#ifdef SERVER
	protected float m_Expansion_UpdateTime;
#endif

	void MissionBase()
	{
		GetDayZGame().Expansion_SetIsMissionMainMenu(IsInherited(MissionMainMenu));

		CreateDayZExpansion();
	}

#ifndef CF_ONUPDATE_RATE_LIMIT
	override void CF_OnUpdate(float timeslice)
	{
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
		m_Expansion_UpdateTime += timeslice;

		if (m_Expansion_UpdateTime >= 0.025)
		{
			CF_ModuleGameManager.OnUpdate(this, new CF_EventUpdateArgs(m_Expansion_UpdateTime));

			m_Expansion_UpdateTime = 0;
		}
#else
		CF_ModuleGameManager.OnUpdate(this, new CF_EventUpdateArgs(timeslice));
#endif
	}
#endif
};
