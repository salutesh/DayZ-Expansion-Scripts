/**
 * ExpansionMarketTraderConsumables.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderConsumables: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Consumables";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CONSUMABLES";
		m_FileName = "Consumables";
		TraderIcon = "Can Of Beans Big";
		
		Categories.Insert("Drinks");
		Categories.Insert("Food");
		Categories.Insert("Fruit_And_Vegetables");
		Categories.Insert("Meat");
		Categories.Insert("Fish");
	}
}