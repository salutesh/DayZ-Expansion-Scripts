/**
 * ExpansionMarketTraderVehicles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderVehicles: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_VEHICLES";
		m_FileName = "Vehicles";
		TraderIcon = "Car";
	}

	override void DefaultItems()
	{
		Categories.Insert("Cars");
		AddCategory("Vehicle_Parts", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddCategory("Batteries", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);		
	}
}