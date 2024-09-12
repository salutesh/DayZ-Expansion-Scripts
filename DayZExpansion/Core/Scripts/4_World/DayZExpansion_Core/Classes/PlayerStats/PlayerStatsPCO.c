modded class PlayerStatsPCO_Base
{
#ifdef EXPANSION_MODSTORAGE
	void CF_OnStoreSave(CF_ModStorage ctx)
	{
		//! Unlike vanilla, we write count so we'll always be able to successfully read back what was actually saved
		ctx.Write(m_PlayerStats.Count());

		foreach (PlayerStatBase playerStat: m_PlayerStats)
		{
			playerStat.CF_OnStoreSave(ctx);
		}
	}
	
	bool CF_OnStoreLoad(CF_ModStorage ctx)
	{
		int count;
		if (!ctx.Read(count))
			return false;

		for (int i = 0; i < count; i++)
		{
			auto playerStat = m_PlayerStats[i];

			if (!playerStat)
			{
				//! Dummy, since stats are always either float or int, should be able to read successfully
				playerStat = new PlayerStat<int>(float.MIN, float.MAX, 0, "", 0);
			}

			if (!playerStat.CF_OnStoreLoad(ctx))
				return false;
		}

		return true;
	}
#endif
}
