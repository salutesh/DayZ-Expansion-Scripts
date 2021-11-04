/**
 * ExpansionMarketTraderFishing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderFishing: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Fishing";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_FISHING";
		m_FileName = "Fishing";
		TraderIcon = "Fishing";

		// FISHING ITEMS
		Categories.Insert("Fishing");
		
		// FISHES
		AddItem("Carp");
		AddItem("Sardines");
		AddItem("Mackerel");
	}
}