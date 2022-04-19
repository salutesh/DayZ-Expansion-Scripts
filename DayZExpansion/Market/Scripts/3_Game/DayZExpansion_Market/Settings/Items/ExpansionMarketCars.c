/**
 * ExpansionMarketCars.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCars: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 14;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_CARS";
		m_FileName = "Cars";
		
		AddItem("OffroadHatchback", 			14000,		28000,		1,		4,		{"HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "CarBattery", "CarRadiator", "SparkPlug", "HatchbackHood", "HatchbackTrunk", "HatchbackDoors_Driver", "HatchbackDoors_CoDriver", "HeadlightH7", "HeadlightH7"});
		AddItem("Hatchback_02", 				16000,		32000,		1,		5,		{"Hatchback_02_Wheel", "Hatchback_02_Wheel", "Hatchback_02_Wheel", "Hatchback_02_Wheel", "Hatchback_02_Door_1_1", "Hatchback_02_Door_2_1", "Hatchback_02_Door_1_2", "Hatchback_02_Door_2_2", "Hatchback_02_Hood", "Hatchback_02_Trunk", "CarBattery", "CarRadiator", "SparkPlug", "HeadlightH7", "HeadlightH7"});
		AddItem("Sedan_02", 					18000,		36000,		1,		5,		{"Sedan_02_Wheel", "Sedan_02_Wheel", "Sedan_02_Wheel", "Sedan_02_Wheel", "CarBattery", "CarRadiator", "SparkPlug", "Sedan_02_Hood", "Sedan_02_Trunk", "Sedan_02_Door_1_1", "Sedan_02_Door_2_1", "Sedan_02_Door_1_2", "Sedan_02_Door_2_2", "HeadlightH7", "HeadlightH7"});
		AddItem("CivilianSedan", 				20000,		40000,		1,		4,		{"CivSedanWheel", "CivSedanWheel", "CivSedanWheel", "CivSedanWheel", "CarBattery", "CarRadiator", "SparkPlug", "CivSedanHood", "CivSedanTrunk", "CivSedanDoors_Driver", "CivSedanDoors_CoDriver", "CivSedanDoors_BackLeft", "CivSedanDoors_BackRight", "HeadlightH7", "HeadlightH7"});
		AddItem("Truck_01_Covered", 			50000,		100000,		1,		7,		{"Truck_01_Wheel", "Truck_01_Wheel", "Truck_01_Wheel", "Truck_01_Wheel", "Truck_01_WheelDouble", "Truck_01_WheelDouble", "Truck_01_WheelDouble", "Truck_01_WheelDouble", "TruckBattery", "Truck_01_Hood", "Truck_01_Door_1_1", "Truck_01_Door_2_1", "HeadlightH7", "HeadlightH7"});	
		//! Vanilla - Not released yet
		//AddItem("Truck_02", 					16000,		21000,		1,		7,		{"Truck_01_Wheel", "Truck_01_Wheel", "Truck_01_Wheel", "Truck_01_Wheel", "TruckBattery", "Truck_02_Hood", "Truck_02_Door_1_1", "Truck_02_Door_2_1", "HeadlightH7", "HeadlightH7"});
		//AddItem("Van_01", 					11000,		16000,		1,		5,		{"Van_01_Wheel", "Van_01_Wheel", "Van_01_Wheel", "Van_01_Wheel", "CarBattery", "CarRadiator", "SparkPlug", "Van_01_Door_1_1", "Van_01_Door_2_1",  "Van_01_Door_2_2", "Van_01_Trunk_1", "Van_01_Trunk_2", "HeadlightH7", "HeadlightH7"});
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionTractor", 			10000,		20000,		1,		7,		{"ExpansionTractorBackWheel", "ExpansionTractorBackWheel", "ExpansionTractorFrontWheel", "ExpansionTractorFrontWheel", "ExpansionTractorDoorsDriver", "ExpansionTractorDoorsCodriver", "HeadlightH7", "HeadlightH7", "SparkPlug", "CarBattery"});
		AddItem("ExpansionUAZ", 				30000,		60000,		1,		3,		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorHood", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"}, {"ExpansionUAZRoofless", "ExpansionUAZCargo", "ExpansionUAZCargoRoofless"});
		/*AddItem("ExpansionUAZRoofless", 		30000,		60000,		1,		3,		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"});
		AddItem("ExpansionUAZCargo", 			30000,		60000,		1,		3,		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"});
		AddItem("ExpansionUAZCargoRoofless", 	30000,		60000,		1,		3,		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"});*/
		AddItem("ExpansionBus", 				70000,		180000,		1,		3,		{"ExpansionBusWheel", "ExpansionBusWheel", "ExpansionBusWheelDouble", "ExpansionBusWheelDouble", "HeadlightH7", "HeadlightH7", "TruckBattery", "CarRadiator", "GlowPlug"});
		AddItem("ExpansionVodnik", 				100000,		200000,		1,		4,		{"ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "TruckBattery", "CarRadiator", "SparkPlug", "ExpansionVodnikDoorDriver", "ExpansionVodnikDoorCoDriver"});
		AddItem("Expansion_Landrover", 				70000,		180000,		1,		3,		{"Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel","CarBattery", "CarRadiator", "SparkPlug", "Expansion_Landrover_Hood", "Expansion_Landrover_Driverdoor", "Expansion_Landrover_Codriverdoor", "Expansion_Landrover_Left", "Expansion_Landrover_Right", "Expansion_Landrover_Trunk","HeadlightH7", "HeadlightH7","CanisterGasoline","SeaChest","WoodenCrate","WoodenCrate","WoodenCrate"});
		
		//! For the next vehicle update		
		//AddItem("Vehicle_ExpansionOldBike", 	8500,		11000,		1,		10,		{"Vehicle_ExpansionOldBikeWheel", "Vehicle_ExpansionOldBikeWheel"});
		//AddItem("Vehicle_ExpansionTT650", 	9000,		12500,		1,		8,		{"Vehicle_ExpansionTT650Wheel", "Vehicle_ExpansionTT650Wheel", "CarBattery", "GlowPlug", "HeadlightH7", "HeadlightH7"});
		//AddItem("Vehicle_Expansion250N", 		9000,		13000,		1,		8,		{"Vehicle_Expansion250NWheel", "Vehicle_Expansion250NWheel", "CarBattery", "GlowPlug", "HeadlightH7", "HeadlightH7"});	
	#endif
	}
};