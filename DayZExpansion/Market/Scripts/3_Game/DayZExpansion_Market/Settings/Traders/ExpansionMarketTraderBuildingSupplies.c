/**
 * ExpansionMarketTraderBuildingSupplies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderBuildingSupplies: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "BuildingSupplies";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_BUILDING_SUPPLIES";
		m_FileName = "BuildingSupplies";
		TraderIcon = "Nails";
		
		Categories.Insert("Gardening");
		Categories.Insert("Tools");
		Categories.Insert("Supplies");
		Categories.Insert("Locks");
		Categories.Insert("Containers");
		Categories.Insert("Tents");
		Categories.Insert("Furnishings");
		Categories.Insert("Flags");
	}
}