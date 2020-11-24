/**
 * ExpansionMarketVehicleParts.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketVehicleParts: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 59;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_VEHICLE_PARTS";
		m_FileName = "Vehicle_Parts";

		AddItem("TireRepairKit", 							30,		300,	1,		250,	0 );
		AddItem("SparkPlug", 								7,		9,		1,		250,	0 );
		AddItem("GlowPlug", 								7,		9,		1,		250,	0 );
		AddItem("CarRadiator", 								25,		27,		1,		250,	0 );
	
		AddItem("HeadlightH7_Box", 							3,		5,		1,		250,	0 );
		AddItem("HatchbackHood",		 					30,		32,		1,		250,	0 );
		AddItem("HatchbackTrunk",		 					35,		37,		1,		250,	0 );
		AddItem("HatchbackDoors_Driver",		 			40,		42,		1,		250,	0 );
		AddItem("HatchbackDoors_CoDriver",		 			40,		42,		1,		250,	0 );
		AddItem("HatchbackWheel", 							50,		52,		1,		250,	0 );
	
		AddItem("Hatchback_02_Door_1_1",		 			40,		42,		1,		250,	0 );
		AddItem("Hatchback_02_Door_1_2",		 			40,		42,		1,		250,	0 );
		AddItem("Hatchback_02_Door_2_1",		 			40,		42,		1,		250,	0 );
		AddItem("Hatchback_02_Door_2_2",		 			40,		42,		1,		250,	0 );
		AddItem("Hatchback_02_Hood",		 				30,		32,		1,		250,	0 );
		AddItem("Hatchback_02_Trunk",		 				35,		37,		1,		250,	0 );
		AddItem("Hatchback_02_Wheel", 						50,		52,		1,		250,	0 );
	
		AddItem("CivSedanHood",		 						35,		37,		1,		250,	0 );
		AddItem("CivSedanTrunk",		 					40,		42,		1,		250,	0 );
		AddItem("CivSedanDoors_Driver",		 				45,		47,		1,		250,	0 );
		AddItem("CivSedanDoors_CoDriver",		 			45,		47,		1,		250,	0 );
		AddItem("CivSedanDoors_BackLeft",					45,		47,		1,		250,	0 );
		AddItem("CivSedanDoors_BackRight",					45,		47,		1,		250,	0 );
		AddItem("CivSedanWheel", 							50,		52,		1,		250,	0 );
	
		AddItem("Sedan_02_Hood",		 					35,		37,		1,		250,	0 );
		AddItem("Sedan_02_Trunk",		 					40,		42,		1,		250,	0 );
		AddItem("Sedan_02_Door_1_1",		 				45,		47,		1,		250,	0 );
		AddItem("Sedan_02_Door_1_2",		 				45,		47,		1,		250,	0 );
		AddItem("Sedan_02_Door_2_1",		 				45,		47,		1,		250,	0 );
		AddItem("Sedan_02_Door_2_2",		 				45,		47,		1,		250,	0 );
		AddItem("Sedan_02_Wheel", 							50,		52,		1,		250,	0 );
	
		AddItem("ExpansionUAZWheel", 						55,		57,		1,		250,	0 );
		AddItem("ExpansionUAZDoorDriver", 					50,		52,		1,		250,	0 );
		AddItem("ExpansionUAZDoorCoDriver", 				50,		52,		1,		250,	0 );
		AddItem("ExpansionUAZDoorCargo1", 					50,		52,		1,		250,	0 );
		AddItem("ExpansionUAZDoorCargo2", 					50,		52,		1,		250,	0 );
	
		AddItem("ExpansionVodnikWheel", 					55,		57,		1,		250,	0 );
		AddItem("ExpansionVodnikDoorDriver", 				50,		52,		1,		250,	0 );
		AddItem("ExpansionVodnikDoorCoDriver", 				50,		52,		1,		250,	0 );
	
		AddItem("ExpansionTractorBackWheel", 				52,		52,		1,		250,	0 );
		AddItem("ExpansionTractorFrontWheel", 				57,		57,		1,		250,	0 );
		AddItem("ExpansionTractorDoorsDriver", 				37,		37,		1,		250,	0 );
		AddItem("ExpansionTractorDoorsCodriver",			37,		37,		1,		250,	0 );
	}
};