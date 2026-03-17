modded class MissionServer
{
	int m_eAI_LastTickTime;

	override void TickScheduler(float timeslice)
	{
		int time = g_Game.GetTime();
		int elapsed = time - m_eAI_LastTickTime;

		if (elapsed == 0)
			return;

		if (m_eAI_LastTickTime > 0)
			ModifiersManager.s_eAI_TickSchedulerDT = elapsed * 0.001;

		m_eAI_LastTickTime = time;

		int playerCount = PlayerBase.s_eAI_TickSchedulerPlayers.Count();
		int tickCountMax = Math.Min(playerCount, SCHEDULER_PLAYERS_PER_TICK);

		for (int i = 0; i < tickCountMax; ++i)
		{
			if (m_currentPlayer >= playerCount)
				m_currentPlayer = 0;

			PlayerBase currentPlayer = PlayerBase.s_eAI_TickSchedulerPlayers[m_currentPlayer];

			//! If identity is NULL and player is not AI (e.g. logged out and didn't wait for
			//! logout countdown but character still on server), vanilla PlayerBase::OnTick
			//! (which calls OnScheduledTick) should not be called because VirtualHud and
			//! TransferValues attempt to send RPCs to identity.
			//! Note that OnTick/OnScheduledTick are only called for real connected players in vanilla.
			if (currentPlayer && (currentPlayer.GetIdentity() || currentPlayer.IsAI()) && !currentPlayer.GetIsSimulationDisabled())
				currentPlayer.OnTick();

			++m_currentPlayer;
		}
	}
}
