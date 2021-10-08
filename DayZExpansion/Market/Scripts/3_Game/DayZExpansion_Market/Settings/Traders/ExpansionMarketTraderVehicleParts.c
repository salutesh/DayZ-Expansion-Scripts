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
		
		// VEHICLE PARTS
		AddItem("HeadlightH7_Box");
		AddItem("CarBattery");
		AddItem("TruckBattery");
		AddItem("CarRadiator");

		AddItem("TireRepairKit");
		AddItem("SparkPlug");
	#ifdef EXPANSIONMODVEHICLE
		AddItem("GlowPlug");
		AddItem("ExpansionHydraulicHoses");
	#endif
		
		AddItem("HatchbackHood");
		//AddItem("HatchbackHood_Blue");
		//AddItem("HatchbackHood_White");

		AddItem("HatchbackTrunk");
		//AddItem("HatchbackTrunk_Blue");
		//AddItem("HatchbackTrunk_White");

		AddItem("HatchbackDoors_Driver");
		//AddItem("HatchbackDoors_Driver_Blue");
		//AddItem("HatchbackDoors_Driver_White");

		AddItem("HatchbackDoors_CoDriver");
		//AddItem("HatchbackDoors_CoDriver_Blue");
		//AddItem("HatchbackDoors_CoDriver_White");
		
		AddItem("HatchbackWheel");
	
		AddItem("Hatchback_02_Hood");
		//AddItem("Hatchback_02_Hood_Black");
		//AddItem("Hatchback_02_Hood_Blue");

		AddItem("Hatchback_02_Trunk");
		//AddItem("Hatchback_02_Trunk_Black");	
		//AddItem("Hatchback_02_Trunk_Blue");

		AddItem("Hatchback_02_Door_1_1");
		//AddItem("Hatchback_02_Door_1_1_Black");
		//AddItem("Hatchback_02_Door_1_1_Blue");

		AddItem("Hatchback_02_Door_1_2");
		//AddItem("Hatchback_02_Door_1_2_Black");
		//AddItem("Hatchback_02_Door_1_2_Blue");

		AddItem("Hatchback_02_Door_2_1");
		//AddItem("Hatchback_02_Door_2_1_Black");		
		//AddItem("Hatchback_02_Door_2_1_Blue");

		AddItem("Hatchback_02_Door_2_2");
		//AddItem("Hatchback_02_Door_2_2_Black");	
		//AddItem("Hatchback_02_Door_2_2_Blue");
		
		AddItem("Hatchback_02_Wheel");
		
		AddItem("CivSedanHood");
		//AddItem("CivSedanHood_Wine");
		//AddItem("CivSedanHood_Black");
		
		AddItem("CivSedanTrunk");
		//AddItem("CivSedanTrunk_Wine");
		//AddItem("CivSedanTrunk_Black");

		AddItem("CivSedanDoors_Driver");
		//AddItem("CivSedanDoors_Driver_Wine");
		//AddItem("CivSedanDoors_Driver_Black");
		
		AddItem("CivSedanDoors_CoDriver");
		//AddItem("CivSedanDoors_CoDriver_Wine");
		//AddItem("CivSedanDoors_CoDriver_Black");

		AddItem("CivSedanDoors_BackLeft");
		//AddItem("CivSedanDoors_BackLeft_Wine");
		//AddItem("CivSedanDoors_BackLeft_Black");

		AddItem("CivSedanDoors_BackRight");
		//AddItem("CivSedanDoors_BackRight_Wine");
		//AddItem("CivSedanDoors_BackRight_Black");
		
		AddItem("CivSedanWheel");
	
		AddItem("Sedan_02_Hood");
		//AddItem("Sedan_02_Hood_Red");
		//AddItem("Sedan_02_Hood_Grey");

		AddItem("Sedan_02_Trunk");
		//AddItem("Sedan_02_Trunk_Red");
		//AddItem("Sedan_02_Trunk_Grey");

		AddItem("Sedan_02_Door_1_1");
		//AddItem("Sedan_02_Door_1_1_Red");
		//AddItem("Sedan_02_Door_1_1_Grey");

		AddItem("Sedan_02_Door_1_2");
		//AddItem("Sedan_02_Door_1_2_Red");	
		//AddItem("Sedan_02_Door_1_2_Grey");
		
		AddItem("Sedan_02_Door_2_1");
		//AddItem("Sedan_02_Door_2_1_Red");
		//AddItem("Sedan_02_Door_2_1_Grey");

		AddItem("Sedan_02_Door_2_2");
		//AddItem("Sedan_02_Door_2_2_Red");
		//AddItem("Sedan_02_Door_2_2_Grey");
		
		AddItem("Sedan_02_Wheel");
	
		AddItem("Truck_01_Hood");
		//AddItem("Truck_01_Hood_Blue");	
		//AddItem("Truck_01_Hood_Orange");
		
		AddItem("Truck_01_Door_1_1");
		//AddItem("Truck_01_Door_1_1_Blue");
		//AddItem("Truck_01_Door_1_1_Orange");

		AddItem("Truck_01_Door_2_1");
		//AddItem("Truck_01_Door_2_1_Blue");
		//AddItem("Truck_01_Door_2_1_Orange");
		
		AddItem("Truck_01_Wheel");
		AddItem("Truck_01_WheelDouble");
		
		//! Vanilla - Not released yet
		//AddItem("Truck_02_Hood");
		//AddItem("Truck_02_Door_1_1");
		//AddItem("Truck_02_Door_1_2");
		//AddItem("Truck_02_Wheel");
	
		//AddItem("Van_01_Trunk_1");
		//AddItem("Van_01_Trunk_2");
		//AddItem("Van_01_Door_1_1");
		//AddItem("Van_01_Door_1_2");
		//AddItem("Van_01_Door_2_1");
		//AddItem("Van_01_Door_2_2");
		//AddItem("Van_01_Wheel");
	
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionTractorDoorsDriver");
		AddItem("ExpansionTractorDoorsCodriver");
		AddItem("ExpansionTractorBackWheel");
		AddItem("ExpansionTractorFrontWheel");

		AddItem("ExpansionUAZDoorHood");
		AddItem("ExpansionUAZDoorDriver");
		AddItem("ExpansionUAZDoorCoDriver");
		AddItem("ExpansionUAZDoorCargo1");
		AddItem("ExpansionUAZDoorCargo2");
		AddItem("ExpansionUAZWheel");
	
		AddItem("ExpansionVodnikDoorDriver");
		AddItem("ExpansionVodnikDoorCoDriver");
		AddItem("ExpansionVodnikWheel");

		AddItem("ExpansionMerlinBackWheel");
		AddItem("ExpansionMerlinFrontWheel");

		AddItem("ExpansionUh1hDoor_1_1");
		AddItem("ExpansionUh1hDoor_1_2");

		AddItem("Expansion_Mh6_Door_1_1");
		AddItem("Expansion_Mh6_Door_1_2");
		AddItem("Expansion_Mh6_Door_2_1");
		AddItem("Expansion_Mh6_Door_2_2");
		
		AddItem("ExpansionBusWheel");
		AddItem("ExpansionBusWheelDouble");
	#endif

		// LIQUIDS
		AddItem("EngineOil");
		AddItem("CanisterGasoline");

		// VEHICLE KEYS
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionCarKey");
	#endif
	}
}