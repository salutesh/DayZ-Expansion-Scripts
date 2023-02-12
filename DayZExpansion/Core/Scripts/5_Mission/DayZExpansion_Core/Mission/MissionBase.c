modded class MissionBase
{
#ifdef SERVER
	static const float EXPANSION_UPDATEINTERVAL = 0.025;
#else
	static const float EXPANSION_UPDATEINTERVAL = 0.005;
#endif

	protected float m_Expansion_UpdateTime;

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

		m_Expansion_UpdateTime += timeslice;

		if (m_Expansion_UpdateTime >= EXPANSION_UPDATEINTERVAL)
		{
			CF_ModuleGameManager.OnUpdate(this, new CF_EventUpdateArgs(m_Expansion_UpdateTime));

			m_Expansion_UpdateTime = 0;
		}
	}
#endif
};
