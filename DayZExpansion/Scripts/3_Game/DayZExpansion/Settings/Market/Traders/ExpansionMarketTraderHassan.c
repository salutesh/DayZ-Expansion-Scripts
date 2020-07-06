/**
 * ExpansionMarketTraderHassan.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderHassan: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderHassan";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_VEHICLE";
		m_FileName = "Vehicle_Parts_Trader_Hassan";
		
		AddItem("TireRepairKit");
		AddItem("EngineOil");	
		AddItem("CanisterGasoline");	
		AddItem("SparkPlug");
		AddItem("GlowPlug");
		AddItem("CarBattery");
		AddItem("TruckBattery");
		AddItem("ExpansionHelicopterBattery");
		AddItem("ExpansionAircraftBattery");
		AddItem("CarRadiator");
		
		AddItem("HeadlightH7_Box");
		AddItem("HatchbackHood");
		AddItem("HatchbackTrunk");
		AddItem("HatchbackDoors_Driver");
		AddItem("HatchbackDoors_CoDriver");
		AddItem("HatchbackWheel");

		AddItem("Hatchback_02_Door_1_1");
		AddItem("Hatchback_02_Door_1_2");
		AddItem("Hatchback_02_Door_2_1");
		AddItem("Hatchback_02_Door_2_2");
		AddItem("Hatchback_02_Hood");
		AddItem("Hatchback_02_Trunk");
		AddItem("Hatchback_02_Wheel");

		AddItem("CivSedanHood");
		AddItem("CivSedanTrunk");
		AddItem("CivSedanDoors_Driver");
		AddItem("CivSedanDoors_CoDriver");
		AddItem("CivSedanDoors_BackLeft");
		AddItem("CivSedanDoors_BackRight");
		AddItem("CivSedanWheel");

		AddItem("Sedan_02_Hood");
		AddItem("Sedan_02_Trunk");
		AddItem("Sedan_02_Door_1_1");
		AddItem("Sedan_02_Door_1_2");
		AddItem("Sedan_02_Door_2_1");
		AddItem("Sedan_02_Door_2_2");
		AddItem("Sedan_02_Wheel");

		AddItem("ExpansionUAZWheel"	);
		AddItem("ExpansionUAZDoorDriver");
		AddItem("ExpansionUAZDoorCoDriver");
		AddItem("ExpansionUAZDoorCargo1");
		AddItem("ExpansionUAZDoorCargo2");
		
		AddItem("ExpansionVodnikWheel");
		AddItem("ExpansionVodnikDoorDriver");
		AddItem("ExpansionVodnikDoorCoDriver");
		
		AddItem("ExpansionTractorBackWheel");
		AddItem("ExpansionTractorFrontWheel");
		AddItem("ExpansionTractorDoorsDriver");
		AddItem("ExpansionTractorDoorsCodriver");
	}
}