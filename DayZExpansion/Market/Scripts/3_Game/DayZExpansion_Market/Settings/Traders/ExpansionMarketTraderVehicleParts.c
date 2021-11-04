/**
 * ExpansionMarketTraderVehicleParts.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderVehicleParts: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Vehicle_Parts";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_VEHICLE_PARTS";
		m_FileName = "Vehicle_Parts";
		TraderIcon = "Gas";
		
		Categories.Insert("Vehicle_Parts");
	}
}