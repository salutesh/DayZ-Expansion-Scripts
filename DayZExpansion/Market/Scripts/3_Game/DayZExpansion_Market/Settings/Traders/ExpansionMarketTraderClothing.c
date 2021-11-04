/**
 * ExpansionMarketTraderClothing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderClothing: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Clothing";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CLOTHING";
		m_FileName = "Clothing";
		TraderIcon = "Backpack";
					
		Categories.Insert("Blouses_And_Suits");
		Categories.Insert("Coats_And_Jackets");
		Categories.Insert("Shirts_And_TShirts");
		Categories.Insert("Skirts_And_Dresses");
		Categories.Insert("Pants_And_Shorts");
		Categories.Insert("Boots_And_Shoes");
	}
}