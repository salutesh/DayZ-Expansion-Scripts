/**
 * ExpansionMarketVehicleParts.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketVehicleParts: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 57;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_VEHICLE_PARTS";
		m_FileName = "Vehicle_Parts";

		AddItem("headlighth7_box", 	610, 	1015,	1,100);
		AddItem("headlighth7", 	600, 	985,	1,100);
		AddItem("carradiator", 	610, 	1015,	1,100);

		AddItem("tirerepairkit", 	610, 	1015,	1,100);
		AddItem("sparkplug", 	610, 	1015,	1,100);
		AddItem("glowplug", 	680, 	1135,	1,100);
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionIgniterPlug", 					680, 	1135,	1,		100);
		AddItem("ExpansionHydraulicHoses", 					610, 	1015,	1,		100);
	#endif

		AddItem("hatchbackhood", 	520, 	870,	1,100,null,{"HatchbackHood_Blue","HatchbackHood_BlueRust","HatchbackHood_GreenRust","HatchbackHood_White","HatchbackHood_WhiteRust"});
		AddItem("hatchbacktrunk", 	520, 	870,	1,100,null,{"HatchbackTrunk_Blue","HatchbackTrunk_BlueRust","HatchbackTrunk_GreenRust","HatchbackTrunk_White","HatchbackTrunk_WhiteRust"});
		AddItem("hatchbackdoors_driver", 	520, 	870,	1,100,null,{"HatchbackDoors_Driver_Blue","HatchbackDoors_Driver_BlueRust","HatchbackDoors_Driver_GreenRust","HatchbackDoors_Driver_White","HatchbackDoors_Driver_WhiteRust"});
		AddItem("hatchbackdoors_codriver", 	520, 	870,	1,100,null,{"HatchbackDoors_CoDriver_Blue","HatchbackDoors_CoDriver_BlueRust","HatchbackDoors_CoDriver_GreenRust","HatchbackDoors_CoDriver_White","HatchbackDoors_CoDriver_WhiteRust"});
		AddItem("hatchbackwheel", 	480, 	800,	1,100);
	
		AddItem("hatchback_02_hood", 	520, 	870,	1,100,null,{"Hatchback_02_Hood_Black","Hatchback_02_Hood_BlackRust","Hatchback_02_Hood_RedRust","Hatchback_02_Hood_Blue","Hatchback_02_Hood_BlueRust"});
		AddItem("hatchback_02_trunk", 	520, 	870,	1,100,null,{"Hatchback_02_Trunk_Black","Hatchback_02_Trunk_BlackRust","Hatchback_02_Trunk_RedRust","Hatchback_02_Trunk_Blue","Hatchback_02_Trunk_BlueRust"});
		AddItem("hatchback_02_door_1_1", 	520, 	870,	1,100,null,{"Hatchback_02_Door_1_1_Black","Hatchback_02_Door_1_1_BlackRust","Hatchback_02_Door_1_1_RedRust","Hatchback_02_Door_1_1_Blue","Hatchback_02_Door_1_1_BlueRust"});
		AddItem("hatchback_02_door_1_2", 	520, 	870,	1,100,null,{"Hatchback_02_Door_1_2_Black","Hatchback_02_Door_1_2_BlackRust","Hatchback_02_Door_1_2_RedRust","Hatchback_02_Door_1_2_Blue","Hatchback_02_Door_1_2_BlueRust"});
		AddItem("hatchback_02_door_2_1", 	520, 	870,	1,100,null,{"Hatchback_02_Door_2_1_Black","Hatchback_02_Door_2_1_BlackRust","Hatchback_02_Door_2_1_RedRust","Hatchback_02_Door_2_1_Blue","Hatchback_02_Door_2_1_BlueRust"});
		AddItem("hatchback_02_door_2_2", 	520, 	870,	1,100,null,{"Hatchback_02_Door_2_2_Black","Hatchback_02_Door_2_2_BlackRust","Hatchback_02_Door_2_2_RedRust","Hatchback_02_Door_2_2_Blue","Hatchback_02_Door_2_2_BlueRust"});
		AddItem("hatchback_02_wheel", 	480, 	800,	1,100);
		
		AddItem("civsedanhood", 	520, 	870,	1,100,null,{"CivSedanHood_Wine","CivSedanHood_WineRust","CivSedanHood_WhiteRust","CivSedanHood_Black","CivSedanHood_BlackRust"});
		AddItem("civsedantrunk", 	520, 	870,	1,100,null,{"CivSedanTrunk_Wine","CivSedanTrunk_WineRust","CivSedanTrunk_WhiteRust","CivSedanTrunk_Black","CivSedanTrunk_BlackRust"});
		AddItem("civsedandoors_driver", 	520, 	870,	1,100,null,{"CivSedanDoors_Driver_Wine","CivSedanDoors_Driver_WineRust","CivSedanDoors_Driver_WhiteRust","CivSedanDoors_Driver_Black","CivSedanDoors_Driver_BlackRust"});
		AddItem("civsedandoors_codriver", 	520, 	870,	1,100,null,{"CivSedanDoors_CoDriver_Wine","CivSedanDoors_CoDriver_WineRust","CivSedanDoors_CoDriver_WhiteRust","CivSedanDoors_CoDriver_Black","CivSedanDoors_CoDriver_BlackRust"});
		AddItem("civsedandoors_backleft", 	520, 	870,	1,100,null,{"CivSedanDoors_BackLeft_Wine","CivSedanDoors_BackLeft_WineRust","CivSedanDoors_BackLeft_WhiteRust","CivSedanDoors_BackLeft_Black","CivSedanDoors_BackLeft_BlackRust"});
		AddItem("civsedandoors_backright", 	520, 	870,	1,100,null,{"CivSedanDoors_BackRight_Wine","CivSedanDoors_BackRight_WineRust","CivSedanDoors_BackRight_WhiteRust","CivSedanDoors_BackRight_Black","CivSedanDoors_BackRight_BlackRust"});
		AddItem("civsedanwheel", 	480, 	800,	1,100);
	
		AddItem("sedan_02_hood", 	520, 	870,	1,100,null,{"Sedan_02_Hood_Red","Sedan_02_Hood_RedRust","Sedan_02_Hood_YellowRust","Sedan_02_Hood_Grey","Sedan_02_Hood_GreyRust"});
		AddItem("sedan_02_trunk", 	520, 	870,	1,100,null,{"Sedan_02_Trunk_Red","Sedan_02_Trunk_RedRust","Sedan_02_Trunk_YellowRust","Sedan_02_Trunk_Grey","Sedan_02_Trunk_GreyRust"});
		AddItem("sedan_02_door_1_1", 	520, 	870,	1,100,null,{"Sedan_02_Door_1_1_Red","Sedan_02_Door_1_1_RedRust","Sedan_02_Door_1_1_YellowRust","Sedan_02_Door_1_1_Grey","Sedan_02_Door_1_1_GreyRust"});
		AddItem("sedan_02_door_1_2", 	520, 	870,	1,100,null,{"Sedan_02_Door_1_2_Red","Sedan_02_Door_1_2_RedRust","Sedan_02_Door_1_2_YellowRust","Sedan_02_Door_1_2_Grey","Sedan_02_Door_1_2_GreyRust"});
		AddItem("sedan_02_door_2_1", 	520, 	870,	1,100,null,{"Sedan_02_Door_2_1_Red","Sedan_02_Door_2_1_RedRust","Sedan_02_Door_2_1_YellowRust","Sedan_02_Door_2_1_Grey","Sedan_02_Door_2_1_GreyRust"});
		AddItem("sedan_02_door_2_2", 	520, 	870,	1,100,null,{"Sedan_02_Door_2_2_Red","Sedan_02_Door_2_2_RedRust","Sedan_02_Door_2_2_YellowRust","Sedan_02_Door_2_2_Grey","Sedan_02_Door_2_2_GreyRust"});
		AddItem("sedan_02_wheel", 	130, 	220,	1,100);
	
		AddItem("truck_01_hood", 	385, 	645,	1,100,null,{"Truck_01_Hood_Blue","Truck_01_Hood_Orange","Truck_01_Hood_OrangeRust","Truck_01_Hood_BlueRust","Truck_01_Hood_GreenRust"});
		AddItem("truck_01_door_1_1", 	385, 	645,	1,100,null,{"Truck_01_Door_1_1_Blue","Truck_01_Door_1_1_Orange","Truck_01_Door_1_1_OrangeRust","Truck_01_Door_1_1_BlueRust","Truck_01_Door_1_1_GreenRust"});
		AddItem("truck_01_door_2_1", 	385, 	645,	1,100,null,{"Truck_01_Door_2_1_Blue","Truck_01_Door_2_1_Orange","Truck_01_Door_2_1_OrangeRust","Truck_01_Door_2_1_BlueRust","Truck_01_Door_2_1_GreenRust"});
		AddItem("truck_01_wheel", 	355, 	590,	1,100);
		AddItem("truck_01_wheeldouble", 	130, 	220,	1,100);
	
		AddItem("offroad_02_hood", 	290, 	480,	1,100,null);
		AddItem("offroad_02_trunk", 	2160, 	3600,	1,100,null,{"Offroad_02_Trunk_Rust"});
		AddItem("offroad_02_door_1_1", 	2160, 	3600,	1,100,null,{"Offroad_02_Door_1_1_Rust"});
		AddItem("offroad_02_door_1_2", 	2160, 	3600,	1,100,null,{"Offroad_02_Door_1_2_Rust"});
		AddItem("offroad_02_door_2_1", 	2160, 	3600,	1,100,null,{"Offroad_02_Door_2_1_Rust"});
		AddItem("offroad_02_door_2_2", 	2160, 	3600,	1,100,null,{"Offroad_02_Door_2_2_Rust"});
		AddItem("offroad_02_wheel", 	6845, 	11410,	1,100);
	
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionTractorDoorsDriver", 				400,	800,	1,		100);
		AddItem("ExpansionTractorDoorsCodriver",			400,	800,	1,		100);
		AddItem("ExpansionTractorBackWheel", 				500,	1000,	1,		100);
		AddItem("ExpansionTractorFrontWheel", 				500,	1000,	1,		100);

		AddItem("ExpansionUAZDoorHood", 					400,	800,	1,		100);
		AddItem("ExpansionUAZDoorDriver", 					500,	1000,	1,		100);
		AddItem("ExpansionUAZDoorCoDriver", 				500,	1000,	1,		100);
		AddItem("ExpansionUAZDoorCargo1", 					500,	1000,	1,		100);
		AddItem("ExpansionUAZDoorCargo2", 					500,	1000,	1,		100);
		AddItem("ExpansionUAZWheel", 						600,	1200,	1,		100);

		AddItem("Expansion_Landrover_Hood", 				400,	800,	1,		100);
		AddItem("Expansion_Landrover_Driverdoor", 			500,	1000,	1,		100);
		AddItem("Expansion_Landrover_Codriverdoor", 		500,	1000,	1,		100);
		AddItem("Expansion_Landrover_Left", 				500,	1000,	1,		100);
		AddItem("Expansion_Landrover_Right", 				500,	1000,	1,		100);
		AddItem("Expansion_Landrover_Trunk", 				400,	800,	1,		100);
		AddItem("Expansion_Landrover_Wheel", 				600,	1200,	1,		100);
	
		AddItem("ExpansionVodnikDoorDriver", 				500,	1000,	1,		100);
		AddItem("ExpansionVodnikDoorCoDriver", 				500,	1000,	1,		100);
		AddItem("ExpansionVodnikWheel", 					600,	1200,	1,		100);

		AddItem("ExpansionMerlinBackWheel", 				600,	1200,	1,		100);
		AddItem("ExpansionMerlinFrontWheel", 				600,	1200,	1,		100);

		AddItem("ExpansionUh1hDoor_1_1", 					500,	1000,	1,		100);
		AddItem("ExpansionUh1hDoor_1_2",					500,	1000,	1,		100);
		AddItem("ExpansionUh1hDoor_2_1", 					500,	1000,	1,		100);
		AddItem("ExpansionUh1hDoor_2_2", 					500,	1000,	1,		100);

		AddItem("Expansion_Mh6_Door_1_1", 					500,	1000,	1,		100, null, {"Expansion_Mh6_Door_1_1_Black"});
		AddItem("Expansion_Mh6_Door_1_2",					500,	1000,	1,		100, null, {"Expansion_Mh6_Door_1_2_Black"});
		AddItem("Expansion_Mh6_Door_2_1", 					500,	1000,	1,		100, null, {"Expansion_Mh6_Door_2_1_Black"});
		AddItem("Expansion_Mh6_Door_2_2",					500,	1000,	1,		100, null, {"Expansion_Mh6_Door_2_2_Black"});
		
		AddItem("ExpansionBusWheel",						600,	1200,	1,		100);
		AddItem("ExpansionBusWheelDouble",					600,	1200,	1,		100);
		
		AddItem("ExpansionCarKey",							250,	500,	1,		100);
		
		AddItem("ExpansionKeyChain_Red",					100,	200,	1,		100, null, {"ExpansionKeyChain_White", "ExpansionKeyChain_Black", "ExpansionKeyChain_Blue", "ExpansionKeyChain_Green", "ExpansionKeyChain_Grey", "ExpansionKeyChain_Orange", "ExpansionKeyChain_Pink", "ExpansionKeyChain_Purple", "ExpansionKeyChain_Yellow"});
	#endif
	}
};