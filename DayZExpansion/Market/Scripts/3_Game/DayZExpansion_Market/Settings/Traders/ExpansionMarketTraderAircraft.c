/**
 * ExpansionMarketTraderAircraft.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderAircraft: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_AIRCRAFT";
		m_FileName = "Aircraft";
		TraderIcon = "Helicopter";
		
		Categories.Insert("Helicopters");
		AddCategory("Vehicle_Parts", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddCategory("Batteries", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	}
}