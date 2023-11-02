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
	static ref ScriptInvoker SI_P2PMarket = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionP2PMarketSettings, true);
	}

	void RPC_P2PMarketSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionP2PMarketSettings, ctx);
	}

	ExpansionP2PMarketSettings GetP2PMarket(bool checkLoaded = true)
	{
		return ExpansionP2PMarketSettings.Cast(Get(ExpansionP2PMarketSettings, checkLoaded));
	}
};
