/**
 * ExpansionMarketTraderBoris.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderBoris: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderBoris";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_EXCHANGE";
		m_FileName = "Exchange_Trader_Boris";

		AddItem("ExpansionGoldBar");
		AddItem("ExpansionGoldNugget");
		AddItem("ExpansionSilverBar");	
		AddItem("ExpansionSilverNugget");
	}
}