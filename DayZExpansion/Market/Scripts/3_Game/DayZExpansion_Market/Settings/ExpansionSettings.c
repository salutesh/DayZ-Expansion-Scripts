/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_Market = new ScriptInvoker();

	override void Init()
	{
		super.Init();

	#ifdef EXPANSIONMODHARDLINE
		//! Need to load hardline before market so we have access to rarity for market items
		Init(ExpansionHardlineSettings, true);
	#endif

		Init(ExpansionMarketSettings, true);
	}

	void RPC_MarketSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionMarketSettings, ctx);
	}

	ExpansionMarketSettings GetMarket(bool checkLoaded = true)
	{
		return ExpansionMarketSettings.Cast(Get(ExpansionMarketSettings, checkLoaded));
	}
};
