modded class MissionServer
{
	float m_eAI_LastTickTime;

	override void TickScheduler(float timeslice)
	{
		int playerCount = PlayerBase.s_eAI_TickSchedulerPlayers.Count();
		int tickCountMax = Math.Min(playerCount, SCHEDULER_PLAYERS_PER_TICK);

		float tickTime = g_Game.GetTickTime();

		if (m_eAI_LastTickTime > 0)
			ModifiersManager.s_eAI_TickSchedulerDT = tickTime - m_eAI_LastTickTime;

		m_eAI_LastTickTime = tickTime;

		for (int i = 0; i < tickCountMax; ++i)
		{
			if (m_currentPlayer >= playerCount)
				m_currentPlayer = 0;

			PlayerBase currentPlayer = PlayerBase.s_eAI_TickSchedulerPlayers[m_currentPlayer];

			if (currentPlayer)
				currentPlayer.OnTick();

			++m_currentPlayer;
		}
	}
}
