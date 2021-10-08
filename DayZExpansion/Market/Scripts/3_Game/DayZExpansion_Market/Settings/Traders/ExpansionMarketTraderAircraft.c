/**
 * ExpansionMarketTraderAircraft.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		
		TraderName = "Aircraft";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_AIRCRAFT";
		m_FileName = "Aircraft";
		TraderIcon = "Helicopter";
		
	#ifdef EXPANSIONMODVEHICLE
		// HELICOPTERS
		AddItem("ExpansionGyrocopter");
		AddItem("ExpansionMh6");
		AddItem("ExpansionUh1h");
		AddItem("ExpansionMerlin");
	#endif

		// VEHICLE PARTS
		AddItem("HeadlightH7_Box");
		AddItem("TireRepairKit");

	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionIgniterPlug");
		AddItem("ExpansionHydraulicHoses");
		AddItem("ExpansionHelicopterBattery");

		AddItem("ExpansionMerlinBackWheel");
		AddItem("ExpansionMerlinFrontWheel");

		AddItem("ExpansionUh1hDoor_1_1");
		AddItem("ExpansionUh1hDoor_1_2");

		AddItem("Expansion_Mh6_Door_1_1");
		AddItem("Expansion_Mh6_Door_1_2");
		AddItem("Expansion_Mh6_Door_2_1");
		AddItem("Expansion_Mh6_Door_2_2");
		
		// VEHICLE KEYS
		AddItem("ExpansionCarKey");
	#endif

		// LIQUIDS
		//AddItem("EngineOil");
		AddItem("CanisterGasoline");
	}
}