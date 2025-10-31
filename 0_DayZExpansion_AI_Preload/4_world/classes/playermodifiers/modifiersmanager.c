modded class ModifiersManager
{
	static float s_eAI_TickSchedulerDT;

	float m_eAI_TickTime;
	int m_eAI_BatchSize;
	int m_eAI_CurrentModifierIdx;

#ifdef DIAG_DEVELOPER
	void ~ModifiersManager()
	{
		if (g_Game)
			Print(m_eAI_BatchSize);
	}
#endif

	override void SetModifiers(bool enable)
	{
		super.SetModifiers(enable);

		if (!enable)
			m_eAI_TickTime = 0;
	}

	override void OnScheduledTick(float delta_time)
	{
		if (!m_AllowModifierTick)
			return;

		int modifierCount = m_ModifierListArray.Count();

		m_eAI_TickTime += delta_time;

		//! Adjust batch size dynamically based on delta time since last TickScheduler call. AI update at half the rate of players.
		//! The intent is for a loop of ticking all modifiers for all players/AI to complete in roughly similar time
		//! within a tick rate range of 30-300 (higher tick rate will take less time). The formula to calculate that time is
		//! [(<players_count> / <players_batch_size>) * [<modifiers_per_player> / <modifiers_batch_size>]] * (1 / <ticks_per_second>)
		//! <players_batch_size> is 5 in vanilla (MissionServer::SCHEDULER_PLAYERS_PER_TICK)
		//! <modifiers_per_player> is 51 in vanilla
		//! e.g. 60 players, server tick rate 60 tps
		//! [(60 / 5) * [51 / 26]] * (1 / 60) = 0.4 s
		//! e.g. 60 players, server tick rate 120 tps
		//! [(60 / 5) * [51 / 13]] * (1 / 120) = 0.4 s
		//! e.g. 60 players, server tick rate 200 tps
		//! [(60 / 5) * [51 / 8]] * (1 / 200) = 0.42 s

		int batchSizeHigh;
		int batchSizeLow;

		if (m_Player.Expansion_IsAI())
		{
			batchSizeHigh = modifierCount * 0.5 + 1;
			batchSizeLow = modifierCount * 0.05 + 1;
		}
		else
		{
			batchSizeHigh = modifierCount;
			batchSizeLow = modifierCount * 0.1 + 1;
		}

		m_eAI_BatchSize = ExpansionMath.LinearConversion(0.033333, 0.003333, s_eAI_TickSchedulerDT, batchSizeHigh, batchSizeLow);

		for (int i = 0; i < m_eAI_BatchSize; ++i)
		{
			if (m_eAI_CurrentModifierIdx >= modifierCount)
				m_eAI_CurrentModifierIdx = 0;

			ModifierBase modifier = m_ModifierListArray[m_eAI_CurrentModifierIdx];
			modifier.Tick(m_eAI_TickTime - modifier.m_eAI_LastTickTime);
			modifier.m_eAI_LastTickTime = m_eAI_TickTime;

			++m_eAI_CurrentModifierIdx;
		}
	}
}
