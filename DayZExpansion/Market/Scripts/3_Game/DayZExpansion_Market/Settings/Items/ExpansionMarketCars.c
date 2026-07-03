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

		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_CARS";
		m_FileName = "Cars";
		
		AddItem("offroadhatchback", 	20160, 	33600,	1,4,{"HatchbackWheel","HatchbackWheel","HatchbackWheel","HatchbackWheel","HatchbackWheel","CarBattery","CarRadiator","SparkPlug","HatchbackHood","HatchbackTrunk","HatchbackDoors_Driver","HatchbackDoors_CoDriver","HeadlightH7","HeadlightH7"}, {"OffroadHatchback_Green", "OffroadHatchback_Blue", "OffroadHatchback_White", "OffroadHatchback_Police", "OffroadHatchback_GreenRust", "OffroadHatchback_BlueRust", "OffroadHatchback_WhiteRust"});
		AddItem("hatchback_02", 	14400, 	24000,	1,5,{"Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Door_1_1","Hatchback_02_Door_2_1","Hatchback_02_Door_1_2","Hatchback_02_Door_2_2","Hatchback_02_Hood","Hatchback_02_Trunk","CarBattery","CarRadiator","SparkPlug","HeadlightH7","HeadlightH7"}, {"Hatchback_02_Red", "Hatchback_02_Black", "Hatchback_02_Blue", "Hatchback_02_RedRust", "Hatchback_02_BlackRust", "Hatchback_02_BlueRust", "Hatchback_02_BanditKitty"});
		AddItem("sedan_02", 	18720, 	31200,	1,5,{"Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel","CarBattery","CarRadiator","SparkPlug","Sedan_02_Hood","Sedan_02_Trunk","Sedan_02_Door_1_1","Sedan_02_Door_2_1","Sedan_02_Door_1_2","Sedan_02_Door_2_2","HeadlightH7","HeadlightH7"}, {"Sedan_02_Red", "Sedan_02_Yellow", "Sedan_02_Grey", "Sedan_02_YellowBright", "Sedan_02_Blue", "Sedan_02_RedRust", "Sedan_02_YellowRust", "Sedan_02_GreyRust", "Sedan_02_YellowBrightRust", "Sedan_02_BlueRust"});
		AddItem("civiliansedan", 	23040, 	38400,	1,4,{"CivSedanWheel","CivSedanWheel","CivSedanWheel","CivSedanWheel","CivSedanWheel","CarBattery","CarRadiator","SparkPlug","CivSedanHood","CivSedanTrunk","CivSedanDoors_Driver","CivSedanDoors_CoDriver","CivSedanDoors_BackLeft","CivSedanDoors_BackRight","HeadlightH7","HeadlightH7"}, {"CivilianSedan_White", "CivilianSedan_Wine", "CivilianSedan_Black", "CivilianSedan_Police", "CivilianSedan_Lime"});
		AddItem("truck_01_covered", 	28800, 	48000,	1,7,{"Truck_01_Wheel","Truck_01_Wheel","Truck_01_Wheel","Truck_01_Wheel","Truck_01_WheelDouble","Truck_01_WheelDouble","Truck_01_WheelDouble","Truck_01_WheelDouble","TruckBattery","Truck_01_Hood","Truck_01_Door_1_1","Truck_01_Door_2_1","HeadlightH7","HeadlightH7"}, {"Truck_01_Covered_Blue", "Truck_01_Covered_Orange"});
		AddItem("offroad_02", 	26640, 	44400,	1,4,{"Offroad_02_Wheel","Offroad_02_Wheel","Offroad_02_Wheel","Offroad_02_Wheel","Offroad_02_Wheel","Offroad_02_Door_1_1","Offroad_02_Door_2_1","Offroad_02_Door_1_2","Offroad_02_Door_2_2","Offroad_02_Hood","Offroad_02_Trunk","CarBattery","GlowPlug","HeadlightH7","HeadlightH7"});
	
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionTractor", 			10000,		20000,		1,		7,		{"ExpansionTractorBackWheel", "ExpansionTractorBackWheel", "ExpansionTractorFrontWheel", "ExpansionTractorFrontWheel", "ExpansionTractorDoorsDriver", "ExpansionTractorDoorsCodriver", "HeadlightH7", "HeadlightH7", "SparkPlug", "CarBattery"}, {"ExpansionTractor_Red", "ExpansionTractor_Blue", "ExpansionTractor_Green"});
		AddItem("ExpansionUAZ", 				30000,		60000,		1,		3,		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorHood", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"}, {"ExpansionUAZRoofless", "ExpansionUAZCargo", "ExpansionUAZCargoRoofless", "ExpansionUAZ_Green", "ExpansionUAZ_Black", "ExpansionUAZ_LightGreen"});
		AddItem("ExpansionBus", 				70000,		180000,		1,		3,		{"ExpansionBusWheel", "ExpansionBusWheel", "ExpansionBusWheelDouble", "ExpansionBusWheelDouble", "HeadlightH7", "HeadlightH7", "TruckBattery", "CarRadiator", "GlowPlug"}, {"ExpansionBus_Blue", "ExpansionBus_Weeb", "ExpansionBus_Red"});
		AddItem("ExpansionVodnik", 				100000,		200000,		1,		4,		{"ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "TruckBattery", "CarRadiator", "SparkPlug", "ExpansionVodnikDoorDriver", "ExpansionVodnikDoorCoDriver"}, {"ExpansionVodnik_Camo", "ExpansionVodnik_CamoMedical"});
		AddItem("Expansion_Landrover", 			70000,		180000,		1,		3,		{"Expansion_Landrover_Hood", "Expansion_Landrover_Driverdoor", "Expansion_Landrover_Codriverdoor", "Expansion_Landrover_Left", "Expansion_Landrover_Right", "Expansion_Landrover_Trunk", "CarBattery", "CarRadiator", "SparkPlug", "HeadlightH7", "HeadlightH7", "CanisterGasoline", "SeaChest", "WoodenCrate", "WoodenCrate", "WoodenCrate", "Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel", "Expansion_Landrover_Wheel"}, {"Expansion_Landrover_Yellow", "Expansion_Landrover_Green", "Expansion_Landrover_Red", "Expansion_Landrover_Grey", "Expansion_Landrover_Blue", "Expansion_Landrover_Orange"});
	#endif
	}
};