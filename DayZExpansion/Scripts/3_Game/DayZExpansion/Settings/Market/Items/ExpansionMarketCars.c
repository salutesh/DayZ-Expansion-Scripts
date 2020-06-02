/**
 * ExpansionMarketCars.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCars: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	//
	// Price calculation and legend:
	// SN - Silver Nugget 	- .1
	// SB - Silver Bar 		- 1
	// GN - Gold Nugget 	- 10
	// GB - Gold Bar 		- 1000
	//
	//	string 	ClassName 			- Item class name that gets defined
	//	float 	MinPriceThreshold 	- Min price for the price threshold calculation
	//	float	MaxPriceThreshold 	- Max price for the price threshold calculation
	//	int		MinStockThreshold 	- Min stock for the price threshold calculation
	//	int		MaxStockThreshold 	- Max stock for the price threshold calculation
	//	int		PurchaseType		- 0 = Normal Item Trader / 1 = Vehicle Trader
	//	array	SpawnAttachments	- Array of item class names that gets attached to this item on buy.
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 14;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_CARS";
		m_FileName = "Cars";

		AddItem("ExpansionTractor", 			10000,		15000,		1,		10,		1, 		{"ExpansionTractorBackWheel", "ExpansionTractorBackWheel", "ExpansionTractorFrontWheel", "ExpansionTractorFrontWheel", "ExpansionTractorDoorsDriver", "ExpansionTractorDoorsCodriver", "HeadlightH7", "HeadlightH7", "CarBattery",} );
		
		AddItem("CivilianSedan", 				10000,		15000,		1,		10,		1, 		{"CivSedanWheel", "CivSedanWheel", "CivSedanWheel", "CivSedanWheel", "CarBattery", "CarRadiator", "SparkPlug", "Expansion_CivSedanHood", "Expansion_CivSedanTrunk", "Expansion_CivSedanDoors_Driver", "Expansion_CivSedanDoors_CoDriver", "Expansion_CivSedanDoors_BackLeft", "Expansion_CivSedanDoors_BackRight", "HeadlightH7", "HeadlightH7"} );
		AddItem("Sedan_02", 					10000,		15000,		1,		10,		1, 		{"Sedan_02_Wheel", "Sedan_02_Wheel", "Sedan_02_Wheel", "Sedan_02_Wheel", "CarBattery", "CarRadiator", "SparkPlug", "Expansion_Sedan_02_Hood", "Expansion_Sedan_02_Trunk", "Expansion_Sedan_02_Door_1_1", "Expansion_Sedan_02_Door_2_1", "Expansion_Sedan_02_Door_1_2", "Expansion_Sedan_02_Door_2_2", "HeadlightH7", "HeadlightH7"} );
		AddItem("Hatchback_02", 				10000,		15000,		1,		10,		1, 		{"Hatchback_02_Wheel", "Hatchback_02_Wheel", "Hatchback_02_Wheel", "Hatchback_02_Wheel", "Expansion_Hatchback_02_Door_1_1", "Expansion_Hatchback_02_Door_2_1", "Expansion_Hatchback_02_Door_1_2", "Expansion_Hatchback_02_Door_2_2", "Expansion_Hatchback_02_Hood", "Expansion_Hatchback_02_Trunk", "CarBattery", "CarRadiator", "SparkPlug", "HeadlightH7", "HeadlightH7"} );
		AddItem("OffroadHatchback", 			10000,		20000,		1,		10,		1, 		{"HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "CarBattery", "CarRadiator", "SparkPlug", "Expansion_HatchbackHood", "Expansion_HatchbackTrunk", "Expansion_HatchbackDoors_Driver", "Expansion_HatchbackDoors_CoDriver", "HeadlightH7", "HeadlightH7"} );
		
		AddItem("ExpansionUAZ", 				11000,		13000,		1,		10,		1, 		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"} );
		AddItem("ExpansionUAZRoofless", 		11000,		13000,		1,		10,		1, 		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"} );
		AddItem("ExpansionUAZCargo", 			11000,		13000,		1,		10,		1, 		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"} );
		AddItem("ExpansionUAZCargoRoofless", 	11000,		13000,		1,		10,		1, 		{"ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "ExpansionUAZWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionUAZDoorDriver", "ExpansionUAZDoorCoDriver", "ExpansionUAZDoorCargo1", "ExpansionUAZDoorCargo2", "HeadlightH7", "HeadlightH7"} );
		AddItem("ExpansionVodnik", 				13000,		15000,		1,		10,		1, 		{"ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionVodnikDoorDriver", "ExpansionVodnikDoorCoDriver"} );
		AddItem("ExpansionVodnikMedical", 		13000,		15000,		1,		10,		1, 		{"ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "ExpansionVodnikWheel", "CarBattery", "CarRadiator", "SparkPlug", "ExpansionVodnikDoorDriver", "ExpansionVodnikDoorCoDriver"} );
		AddItem("ExpansionBus", 				15000,		20000,		1,		10,		1, 		{"TransitBusWheel", "TransitBusWheel", "TransitBusWheelDouble", "TransitBusWheelDouble", "HeadlightH7", "HeadlightH7", "TruckBattery", "CarRadiator", "GlowPlug"} );
	}
}