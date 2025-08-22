modded class EXError
{
	static void MarketCfgError(Class instance, string msg, bool includeStackTrace = false)
	{
		msg = string.Format("MARKET CONFIGURATION ERROR: %1\nUse https://github.com/lava76/check_expansion_market_variants/releases to fix", msg);

		if (includeStackTrace)
			Error(instance, msg);
		else
			Error(instance, msg, {});
	}

	static void MarketCfgWarn(Class instance, string msg, bool includeStackTrace = false)
	{
		msg = string.Format("Market configuration warning: %1", msg);

		if (includeStackTrace)
			Warn(instance, msg);
		else
			Warn(instance, msg, {});
	}

}
