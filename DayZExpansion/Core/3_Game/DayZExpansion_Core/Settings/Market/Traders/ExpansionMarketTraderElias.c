/**
 * ExpansionMarketTraderElias.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderElias: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderElias";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_VEHICLES";
		m_FileName = "Vehicles_Trader_Elias";

		AddItem("OffroadHatchback");
		AddItem("CivilianSedan");
		AddItem("Hatchback_02");
		AddItem("Sedan_02");
		AddItem("ExpansionBus");
		AddItem("ExpansionTractor");
		AddItem("ExpansionUAZ");
		AddItem("ExpansionUAZRoofless");
		AddItem("ExpansionUAZCargo");
		AddItem("ExpansionUAZCargoRoofless");
		AddItem("ExpansionVodnik");
	}
}