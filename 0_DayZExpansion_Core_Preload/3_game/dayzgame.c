modded class DayZGame
{
	protected float m_Expansion_ServerUpdateRateLimit_Interval;
	protected float m_Expansion_ServerUpdateRateLimit_LastUpdateTime;
	protected float m_Expansion_ServerUpdateRateLimit_TimeSlice;

	override void OnUpdate(bool doSim, float timeslice)
	{
#ifdef SERVER
		//! @note timeslice isn't accurate for measuring actual elapsed time, so it's just passed to super to match vanilla
		float updateTime = GetTickTime();
		float elapsed = updateTime - m_Expansion_ServerUpdateRateLimit_LastUpdateTime;
		m_Expansion_ServerUpdateRateLimit_TimeSlice += timeslice;
		if (elapsed >= m_Expansion_ServerUpdateRateLimit_Interval)
		{
			super.OnUpdate(doSim, m_Expansion_ServerUpdateRateLimit_TimeSlice);

			Expansion_OnUpdate(doSim, m_Expansion_ServerUpdateRateLimit_TimeSlice);

			m_Expansion_ServerUpdateRateLimit_LastUpdateTime = updateTime;
			m_Expansion_ServerUpdateRateLimit_TimeSlice = 0;
		}
#else
		super.OnUpdate(doSim, timeslice);

		Expansion_OnUpdate(doSim, timeslice);
#endif
	}

	void Expansion_OnUpdate(bool doSim, float timeslice)
	{
	}
}
