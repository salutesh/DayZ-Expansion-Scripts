modded class PlayerStats
{
#ifdef EXPANSION_MODSTORAGE
	void CF_SaveStats(CF_ModStorage ctx)
	{
		int current_version = g_Game.SaveVersion();
		PlayerStatsPCO_Base pco = GetPCO(current_version);
		
		if (pco)
			pco.CF_OnStoreSave(ctx);
	}

	bool CF_LoadStats(CF_ModStorage ctx, int version)
	{
		PlayerStatsPCO_Base pco = GetPCO(version);
		if (pco && pco.CF_OnStoreLoad(ctx))
			return true;

		return false;
	}
#endif
}
