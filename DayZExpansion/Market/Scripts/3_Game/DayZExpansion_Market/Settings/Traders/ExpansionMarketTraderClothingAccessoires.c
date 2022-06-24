/**
 * ExpansionMarketTraderClothingAccessoires.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderClothingAccessoires: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CLOTHING_ACCESSORIES";
		m_FileName = "Clothing_Accessoires";
		TraderIcon = "Backpack";
		
		Categories.Insert("Helmets");
		Categories.Insert("Caps");
		Categories.Insert("Hats_And_Hoods");
		Categories.Insert("Armbands");
		Categories.Insert("Backpacks");
		Categories.Insert("Eyewear");
		Categories.Insert("Gloves");
		Categories.Insert("Bandanas");
		Categories.Insert("Masks");
		Categories.Insert("Vests");
		Categories.Insert("Holsters_And_Pouches");
		Categories.Insert("Ghillies");
		Categories.Insert("Belts");
	}
}