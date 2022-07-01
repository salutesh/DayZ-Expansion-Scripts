#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_LandRover
	{
		units[] = {"Expansion_Landrover_Driverdoor","Expansion_Landrover_Codriverdoor","Expansion_Landrover_Left","Expansion_Landrover_Right","Expansion_Landrover_Trunk","Expansion_Landrover_Hood","Expansion_Landrover_Wheel","Expansion_Landrover_Wheel_Ruined","Expansion_Landrover","ExpansionSpraycanYellow","Expansion_Landrover_Yellow","Expansion_Landrover_Driverdoor_Yellow","Expansion_Landrover_Codriverdoor_Yellow","Expansion_Landrover_Left_Yellow","Expansion_Landrover_Right_Yellow","Expansion_Landrover_Trunk_Yellow","Expansion_Landrover_Hood_Yellow","Expansion_Landrover_Wheel_Yellow","ExpansionSpraycanGreen","Expansion_Landrover_Green","Expansion_Landrover_Driverdoor_Green","Expansion_Landrover_Codriverdoor_Green","Expansion_Landrover_Left_Green","Expansion_Landrover_Right_Green","Expansion_Landrover_Trunk_Green","Expansion_Landrover_Hood_Green","Expansion_Landrover_Wheel_Green","ExpansionSpraycanRed","Expansion_Landrover_Red","Expansion_Landrover_Driverdoor_Red","Expansion_Landrover_Codriverdoor_Red","Expansion_Landrover_Left_Red","Expansion_Landrover_Right_Red","Expansion_Landrover_Trunk_Red","Expansion_Landrover_Hood_Red","Expansion_Landrover_Wheel_Red","ExpansionSpraycanGrey","Expansion_Landrover_Grey","Expansion_Landrover_Driverdoor_Grey","Expansion_Landrover_Codriverdoor_Grey","Expansion_Landrover_Left_Grey","Expansion_Landrover_Right_Grey","Expansion_Landrover_Trunk_Grey","Expansion_Landrover_Hood_Grey","Expansion_Landrover_Wheel_Grey","ExpansionSpraycanBlue","Expansion_Landrover_Blue","Expansion_Landrover_Driverdoor_Blue","Expansion_Landrover_Codriverdoor_Blue","Expansion_Landrover_Left_Blue","Expansion_Landrover_Right_Blue","Expansion_Landrover_Trunk_Blue","Expansion_Landrover_Hood_Blue","Expansion_Landrover_Wheel_Blue","ExpansionSpraycanOrange","Expansion_Landrover_Orange","Expansion_Landrover_Driverdoor_Orange","Expansion_Landrover_Codriverdoor_Orange","Expansion_Landrover_Left_Orange","Expansion_Landrover_Right_Orange","Expansion_Landrover_Trunk_Orange","Expansion_Landrover_Hood_Orange","Expansion_Landrover_Wheel_Orange"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Gear_Camping","DZ_Vehicles_Parts","DayZExpansion_Vehicles_Data"};
	};
};
class CfgSlots
{
	class Slot_expansion_landrover_wheel_1_1
	{
		name = "expansion_landrover_wheel_1_1";
		displayName = "expansion_landrover_wheel_1_1";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_landrover_wheel_2_1
	{
		name = "expansion_landrover_wheel_2_1";
		displayName = "expansion_landrover_wheel_2_1";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_landrover_wheel_2_2
	{
		name = "expansion_landrover_wheel_2_2";
		displayName = "expansion_landrover_wheel_2_2";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_expansion_landrover_wheel_1_2
	{
		name = "expansion_landrover_wheel_1_2";
		displayName = "expansion_landrover_wheel_1_2";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_expansion_landrover_sparewheel
	{
		name = "expansion_landrover_sparewheel";
		displayName = "expansion_landrover_sparewheel";
		selection = "wheel_spare";
		ghostIcon = "wheel";
	};
	class Slot_expansion_landrover_hood
	{
		name = "expansion_landrover_hood";
		displayName = "expansion_landrover_hood";
		selection = "doors_hood";
		ghostIcon = "hood";
	};
	class Slot_expansion_landrover_driverdoor
	{
		name = "expansion_landrover_driverdoor";
		displayName = "expansion_landrover_driverdoor";
		selection = "doors_driver";
		ghostIcon = "doorfront";
	};
	class Slot_expansion_landrover_codriverdoor
	{
		name = "expansion_landrover_codriverdoor";
		displayName = "expansion_landrover_codriverdoor";
		selection = "doors_codriver";
		ghostIcon = "doorfront";
	};
	class Slot_expansion_landrover_left
	{
		name = "expansion_landrover_left";
		displayName = "expansion_landrover_left";
		selection = "doors_cargo_left";
		ghostIcon = "doorrear";
	};
	class Slot_expansion_landrover_right
	{
		name = "expansion_landrover_right";
		displayName = "expansion_landrover_right";
		selection = "doors_cargo_right";
		ghostIcon = "doorrear";
	};
	class Slot_expansion_landrover_trunk
	{
		name = "expansion_landrover_trunk";
		displayName = "expansion_landrover_trunk";
		selection = "doors_trunk";
		ghostIcon = "trunk";
	};
	class Slot_CanisterGasoline
	{
		name = "CanisterGasoline";
		displayName = "CanisterGasoline";
		selection = "CanisterGasoline";
		ghostIcon = "gascanister";
	};
	class Slot_Truck_01_WoodenCrate1
	{
		name = "Truck_01_WoodenCrate1";
		displayName = "WoodenCrate";
		selection = "Truck_01_WoodenCrate1";
		ghostIcon = "woodcrate";
	};
	class Slot_Truck_01_WoodenCrate2
	{
		name = "Truck_01_WoodenCrate2";
		displayName = "WoodenCrate";
		selection = "Truck_01_WoodenCrate2";
		ghostIcon = "woodcrate";
	};
	class Slot_Truck_01_WoodenCrate3
	{
		name = "Truck_01_WoodenCrate3";
		displayName = "WoodenCrate";
		selection = "Truck_01_WoodenCrate3";
		ghostIcon = "woodcrate";
	};
	class Slot_Sea_Chest
	{
		name = "sea_chest";
		displayName = "sea_chest";
		selection = "sea_chest";
		ghostIcon = "cat_common_cargo";
	};
};
class CfgVehicles
{
	class Bottle_Base;
	class CanisterGasoline: Bottle_Base
	{
		inventorySlot[] = {"CanisterGasoline","CanisterGasoline2","CanisterGasoline_1","CanisterGasoline_2"};
	};
	class Container_Base;
	class SeaChest: Container_Base
	{
		inventorySlot[] = {"sea_chest","sea_chest_1","sea_chest_2","sea_chest_3","sea_chest_4"};
	};
	class WoodenCrate: Container_Base
	{
		inventorySlot[] = {"WoodenCrate","woodencrate_1","woodencrate_2","woodencrate_3","woodencrate_4","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","Truck_01_WoodenCrate4","Truck_01_WoodenCrate5","Truck_01_WoodenCrate6"};
	};
	class CarDoor;
	class Expansion_Landrover_Driverdoor: CarDoor
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_LEFT_DESC";
		Model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_driverdoor.p3d";
		weight = 15000;
		defaultSkin = "Green";
		itemSize[] = {8,8};
		itemBehaviour = 0;
		inventorySlot = "expansion_landrover_driverdoor";
		rotationFlags = 8;
		physLayer = "item_large";
		hiddenSelections[] = {"body_inside","body_outdoor","dmgzone_door_outside","dmgzone_door_inside"};
		hiddenSelectionsTextures[] = {"","","",""};
		hiddenSelectionsMaterials[] = {"","","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Window_Outside
				{
					class Health
					{
						hitpoints = 500;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_windows.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					transferToGlobalCoef = 0;
					memoryPoints[] = {"dmgzone_w_1_1"};
					componentNames[] = {"dmgzone_w_1_1"};
				};
				class Door_Outside
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					memoryPoints[] = {"dmgzone_door_outside"};
					componentNames[] = {"dmgzone_door_outside"};
				};
				class Door_inside: Door_Outside
				{
					componentNames[] = {"dmgzone_door_inside"};
				};
			};
		};
	};
	class Expansion_Landrover_Codriverdoor: CarDoor
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_RIGHT_DESC";
		Model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_codriverdoor.p3d";
		weight = 15000;
		defaultSkin = "Green";
		itemSize[] = {8,8};
		itemBehaviour = 0;
		inventorySlot = "expansion_landrover_codriverdoor";
		rotationFlags = 8;
		physLayer = "item_large";
		hiddenSelections[] = {"body_inside","body_outdoor","dmgzone_door_outside","dmgzone_door_inside"};
		hiddenSelectionsTextures[] = {"","","",""};
		hiddenSelectionsMaterials[] = {"","","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Window_Outside
				{
					class Health
					{
						hitpoints = 500;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_windows.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					transferToGlobalCoef = 0;
					memoryPoints[] = {"dmgzone_w_2_1"};
					componentNames[] = {"dmgzone_w_2_1"};
				};
				class Door_Outside
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					memoryPoints[] = {"dmgzone_door_outside"};
					componentNames[] = {"dmgzone_door_outside"};
				};
				class Door_inside: Door_Outside
				{
					componentNames[] = {"dmgzone_door_inside"};
				};
			};
		};
	};
	class Expansion_Landrover_Left: CarDoor
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_LEFT_DESC";
		Model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_left.p3d";
		weight = 15000;
		defaultSkin = "Green";
		itemSize[] = {8,8};
		itemBehaviour = 0;
		inventorySlot = "expansion_landrover_left";
		rotationFlags = 8;
		physLayer = "item_large";
		hiddenSelections[] = {"body_inside","body_outdoor","dmgzone_door_outside","dmgzone_door_inside"};
		hiddenSelectionsTextures[] = {"","","",""};
		hiddenSelectionsMaterials[] = {"","","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Window_Outside
				{
					class Health
					{
						hitpoints = 500;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_windows.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					transferToGlobalCoef = 0;
					memoryPoints[] = {"dmgzone_w_1_2"};
					componentNames[] = {"dmgzone_w_1_2"};
				};
				class Door_Outside
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					memoryPoints[] = {"dmgzone_door_outside"};
					componentNames[] = {"dmgzone_door_outside"};
				};
				class Door_inside: Door_Outside
				{
					componentNames[] = {"dmgzone_door_inside"};
				};
			};
		};
	};
	class Expansion_Landrover_Right: CarDoor
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_RIGHT_DESC";
		Model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_right.p3d";
		weight = 15000;
		defaultSkin = "Green";
		itemSize[] = {8,8};
		itemBehaviour = 0;
		inventorySlot = "expansion_landrover_right";
		rotationFlags = 8;
		physLayer = "item_large";
		hiddenSelections[] = {"body_inside","body_outdoor","dmgzone_door_outside","dmgzone_door_inside"};
		hiddenSelectionsTextures[] = {"","","",""};
		hiddenSelectionsMaterials[] = {"","","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Window_Outside
				{
					class Health
					{
						hitpoints = 500;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_windows.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					transferToGlobalCoef = 0;
					memoryPoints[] = {"dmgzone_w_2_2"};
					componentNames[] = {"dmgzone_w_2_2"};
				};
				class Door_Outside
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					memoryPoints[] = {"dmgzone_door_outside"};
					componentNames[] = {"dmgzone_door_outside"};
				};
				class Door_inside: Door_Outside
				{
					componentNames[] = {"dmgzone_door_inside"};
				};
			};
		};
	};
	class Expansion_Landrover_Trunk: CarDoor
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_TRUNK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_TRUNK_DESC";
		Model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_trunk.p3d";
		weight = 15000;
		defaultSkin = "Green";
		itemSize[] = {8,8};
		itemBehaviour = 0;
		inventorySlot = "expansion_landrover_trunk";
		rotationFlags = 16;
		physLayer = "item_large";
		hiddenSelections[] = {"body_inside","body_outdoor","dmgzone_door_outside","dmgzone_door_inside"};
		hiddenSelectionsTextures[] = {"","","",""};
		hiddenSelectionsMaterials[] = {"","","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Window_Outside
				{
					class Health
					{
						hitpoints = 500;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_windows.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\LandRover\data\lr_windows_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					transferToGlobalCoef = 0;
					memoryPoints[] = {"dmgzone_w_trunk"};
					componentNames[] = {"dmgzone_w_trunk"};
				};
				class Door_Outside
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					transferToZonesNames[] = {"Door_inside"};
					transferToZonesThreshold[] = {1};
					transferToZonesCoefs[] = {1};
					memoryPoints[] = {"dmgzone_door_outside"};
					componentNames[] = {"dmgzone_door_outside"};
				};
				class Door_inside: Door_Outside
				{
					componentNames[] = {"dmgzone_door_inside"};
				};
			};
		};
	};
	class Expansion_Landrover_Hood: CarDoor
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_HOOD";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_HOOD_DESC";
		Model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_hood.p3d";
		weight = 15000;
		defaultSkin = "Green";
		itemSize[] = {8,8};
		itemBehaviour = 0;
		inventorySlot = "expansion_landrover_hood";
		rotationFlags = 16;
		physLayer = "item_large";
		hiddenSelections[] = {"body_outdoor","dmgzone_hood"};
		hiddenSelectionsTextures[] = {"",""};
		hiddenSelectionsMaterials[] = {"","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Door_Outside
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {"expansion_landrover_hood"};
					memoryPoints[] = {"dmgzone_hood"};
					componentNames[] = {"dmgzone_hood"};
				};
			};
		};
	};
	class CarWheel;
	class Expansion_Landrover_Wheel: CarWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\landrover_wheel.p3d";
		weight = 20000;
		defaultSkin = "Green";
		itemSize[] = {6,6};
		inventorySlot[] = {"expansion_landrover_wheel_1_1","expansion_landrover_wheel_1_2","expansion_landrover_wheel_2_1","expansion_landrover_wheel_2_2","expansion_landrover_sparewheel"};
		rotationFlags = 8;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.408,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.41;
		width = 0.15;
		friction = 0.85;
		tyre = "TYRE_DEFAULT";
		tyreRollDrag = 5;
		tyreRollResistance = 0.014;
		tyreTread = 0.95;
		hiddenSelections[] = {"bottom","dmgzone_rim","dmgzone_tire"};
		hiddenSelectionsTextures[] = {"","",""};
		hiddenSelectionsMaterials[] = {"","dayzexpansion\vehicles\ground\landrover\data\lr_bottom_metal.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_bottom.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 800;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Rim
				{
					class Health
					{
						hitpoints = 500;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					transferToGlobalCoef = 0;
					memoryPoints[] = {"dmgzone_rim"};
					componentNames[] = {"dmgzone_rim"};
				};
				class Tire
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgzone_tire"};
					componentNames[] = {"dmgzone_tire"};
				};
			};
		};
	};
	class Expansion_Landrover_Wheel_Ruined: CarWheel
	{
		scope = 2;
		displayName = "Landrover Defender Wheel";
		descriptionShort = "Landrover Defender 110 Wheel";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\proxy\landrover_wheel_destroyed.p3d";
		weight = 15000;
		itemSize[] = {6,6};
		defaultSkin = "Green";
		inventorySlot[] = {"expansion_landrover_wheel_1_1","expansion_landrover_wheel_1_2","expansion_landrover_wheel_2_1","expansion_landrover_wheel_2_2","expansion_landrover_sparewheel"};
		rotationFlags = 8;
		physLayer = "item_large";
		radius = 0.238;
		width = 0.14;
		tyreRollResistance = 1;
		tyreRollDrag = 75;
		tyreRoughness = 1.2;
		tyreTread = 0.5;
		hiddenSelections[] = {"bottom","dmgzone_rim","dmgzone_tire"};
		hiddenSelectionsTextures[] = {"","",""};
		hiddenSelectionsMaterials[] = {"","dayzexpansion\vehicles\ground\landrover\data\lr_bottom_metal.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_bottom.rvmat"};
	};
	class Carscript;
	class Expansion_Landrover_Base: Carscript
	{
		scope = 0;
		displayname = "$STR_EXPANSION_VEHICLE_LANDROVER";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DESC";
		Model = "\DayZExpansion\Vehicles\Ground\LandRover\expansion_landrover.p3d";
		modelZeroPointDistanceFromGround = 0.52;
		vehicleClass = "Expansion_Car";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","body_inside","body_outdoor","bottom","dmgzone_front","dmgzone_back","dmgzone_roof","dmgzone_doors","dmgzone_chassis","dmgzone_w_frameL","dmgzone_w_frameR","dmgzone_fender_1_1","dmgzone_fender_1_2","dmgzone_fender_2_1","dmgzone_fender_2_2","dmgzone_interior"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"","","","","","","","","","dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_bottom.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_bottom.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat","dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat"};
		frontReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_lights_on.rvmat";
		frontReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_lights_off.rvmat";
		brakeReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_brake_lights.rvmat";
		brakeReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat";
		ReverseReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_reverse_lights.rvmat";
		ReverseReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat";
		TailReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_tail_lights.rvmat";
		TailReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat";
		dashboardMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_dashboard_lights.rvmat";
		dashboardMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat";
		doors[] = {"expansion_landrover_driverdoor","expansion_landrover_codriverdoor","expansion_landrover_left","expansion_landrover_right","expansion_landrover_trunk","expansion_landrover_hood"};
		defaultSkin = "Green";
		fuelCapacity = 80;
		fuelConsumption = 10;
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","expansion_landrover_left","expansion_landrover_right","expansion_landrover_trunk","expansion_landrover_hood","expansion_landrover_codriverdoor","expansion_landrover_driverdoor","expansion_landrover_wheel_1_1","expansion_landrover_wheel_1_2","expansion_landrover_wheel_2_1","expansion_landrover_wheel_2_2","expansion_landrover_sparewheel","CanisterGasoline","Sea_Chest","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3"};
		class Crew
		{
			class Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
				isDriver = 1;
			};
			class CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_coDriver";
				getInDir = "pos_coDriver_dir";
			};
			class Cargo1
			{
				actionSel = "seat_cargo1";
				proxyPos = "CrewCargo1";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "CrewCargo2";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
		};
		class SimulationModule
		{
			drive = "DRIVE_AWD";
			centralDiffRatio = 1.45;
			airDragFrontTotal = 0.995;
			class Steering
			{
				increaseSpeed[] = {0,45,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime = 1.25;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 5;
				gentleCoef = 0.75;
			};
			braking[] = {0.0,0.1,1.0,0.8,2.5,0.9,3.0,1.0};
			class Engine
			{
				inertia = 0.27;
				steepness = 1.5;
				torqueMax = 250;
				torqueRpm = 3000;
				powerMax = 100;
				powerRpm = 5800;
				rpmIdle = 1250;
				rpmMin = 1350;
				rpmClutch = 1500;
				rpmRedline = 6000;
				rpmMax = 7000;
			};
			class Gearbox
			{
				reverse = 3.726;
				ratios[] = {4.4,3.0,2.0,1.6,1};
				timeToUncoupleClutch = 0.2;
				timeToCoupleClutch = 0.35;
				maxClutchTorque = 300;
			};
			class Axles
			{
				class Front
				{
					maxSteeringAngle = 30;
					finalRatio = 4.1;
					brakeBias = 0.6;
					brakeForce = 4000;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1700;
						stiffness = 35000;
						compression = 2100;
						damping = 7400;
						travelMaxUp = 0.06;
						travelMaxDown = 0.07;
					};
					class Wheels
					{
						class Left
						{
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							wheelHub = "wheel_1_1_damper_land";
							animDamper = "damper_1_1";
							inventorySlot = "expansion_landrover_wheel_1_1";
						};
						class Right
						{
							animTurn = "turnfrontright";
							animRotation = "wheelfrontright";
							wheelHub = "wheel_2_1_damper_land";
							animDamper = "damper_2_1";
							inventorySlot = "expansion_landrover_wheel_2_1";
						};
					};
				};
				class Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.4;
					brakeForce = 3800;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1800;
						stiffness = 37000;
						compression = 2200;
						damping = 7600;
						travelMaxUp = 0.06;
						travelMaxDown = 0.07;
					};
					class Wheels
					{
						class Left
						{
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							wheelHub = "wheel_1_2_damper_land";
							animDamper = "damper_1_2";
							inventorySlot = "expansion_landrover_wheel_1_2";
						};
						class Right
						{
							animTurn = "turnbacktright";
							animRotation = "wheelbackright";
							wheelHub = "wheel_2_2_damper_land";
							animDamper = "damper_2_2";
							inventorySlot = "expansion_landrover_wheel_2_2";
						};
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug","Reflector_1_1","Reflector_2_1"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"expansion_landrover_hood","expansion_landrover_driverdoor","expansion_landrover_codriverdoor","expansion_landrover_left","expansion_landrover_right","expansion_landrover_trunk"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"expansion_landrover_wheel_1_1","expansion_landrover_wheel_1_2","expansion_landrover_wheel_2_1","expansion_landrover_wheel_2_2"};
			};
			class Trunk_Cargo
			{
				name = "Trunk Cargo";
				description = "";
				icon = "trunk";
				attachmentSlots[] = {"CanisterGasoline","Sea_Chest","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","expansion_landrover_sparewheel"};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,50};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 3000;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 2000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					memoryPoints[] = {"dmgzone_chassis"};
					componentNames[] = {"dmgzone_chassis"};
					transferTozonesNames[] = {"Engine"};
					transferTozonesThreshold[] = {0.4};
					transferTozonesCoefs[] = {0.3};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Interior
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgzone_interior"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Front
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					transferTozonesNames[] = {"Reflector_1_1","Reflector_2_1","WindowFront","Engine","Radiator","Battery"};
					transferTozonesThreshold[] = {0.5,0.8,0.8,0.8};
					transferTozonesCoefs[] = {0.8,0.6,0.6,0.6};
					memoryPoints[] = {"dmgzone_front"};
					componentNames[] = {"dmgzone_front"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {"CarRadiator","expansion_landrover_wheel_1_1","expansion_landrover_wheel_2_1","expansion_landrover_hood"};
				};
				class Back
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					transferTozonesNames[] = {"Fender_1_2","Fender_2_2","WindowBL","WindowBR"};
					transferTozonesCoefs[] = {0.2,0.2,0.2,0.2};
					memoryPoints[] = {"dmgzone_back"};
					componentNames[] = {"dmgzone_back"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Roof
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					memoryPoints[] = {"dmgzone_roof"};
					componentNames[] = {"dmgzone_roof"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class FrameL
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					memoryPoints[] = {"dmgzone_w_frameL"};
					componentNames[] = {"dmgzone_w_frameL"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class FrameR: FrameL
				{
					memoryPoints[] = {"dmgzone_w_frameR"};
					componentNames[] = {"dmgzone_w_frameR"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					transferToZonesNames[] = {"Front","Reflector_1_1","Engine","Radiator","WindowFront","Battery"};
					transferToZonesCoefs[] = {0.3,0.3,0.6,0.5,0.4,0.7};
					memoryPoints[] = {"dmgzone_fender_1_1"};
					componentNames[] = {"dmgzone_fender_1_1"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {"expansion_landrover_wheel_1_1"};
					inventorySlotsCoefs[] = {0.5};
				};
				class Fender_1_2: Fender_1_1
				{
					transferTozonesNames[] = {"Back","FuelTank","WindowBL"};
					transferTozonesThreshold[] = {0.2,0.2,0.2};
					transferTozonesCoefs[] = {0.3,0.2,0.7};
					memoryPoints[] = {"dmgzone_fender_1_2"};
					componentNames[] = {"dmgzone_fender_1_2"};
					inventorySlots[] = {"expansion_landrover_wheel_1_2"};
				};
				class Fender_2_1: Fender_1_1
				{
					transferToZonesNames[] = {"Front","Reflector_2_1","Engine","Radiator","WindowFront","Battery"};
					transferToZonesCoefs[] = {0.3,0.3,0.6,0.5,0.4,0.7};
					memoryPoints[] = {"dmgzone_fender_2_1"};
					componentNames[] = {"dmgzone_fender_2_1"};
					inventorySlots[] = {"expansion_landrover_wheel_2_1"};
				};
				class Fender_2_2: Fender_1_1
				{
					transferTozonesNames[] = {"Back","FuelTank","WindowBR"};
					transferTozonesThreshold[] = {0.2,0.2,0.2};
					transferTozonesCoefs[] = {0.3,0.2,0.7};
					memoryPoints[] = {"dmgzone_fender_2_2"};
					componentNames[] = {"dmgzone_fender_2_2"};
					inventorySlots[] = {"expansion_landrover_wheel_2_2"};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 800;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_windows.rvmat"}},{0.7,{"DayzExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.5,{"DayzExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.3,{"DayzExpansion\Vehicles\Ground\LandRover\data\lr_windows_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgzone_w_front"};
					componentNames[] = {"dmgzone_w_front"};
					inventorySlots[] = {};
				};
				class WindowBL
				{
					class Health
					{
						hitpoints = 800;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_windows.rvmat"}},{0.7,{"DayzExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.5,{"DayzExpansion\Vehicles\Ground\LandRover\data\lr_windows_damage.rvmat"}},{0.3,{"DayzExpansion\Vehicles\Ground\LandRover\data\lr_windows_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgzone_w_backL"};
					componentNames[] = {"dmgzone_w_backL"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class WindowBR: WindowBL
				{
					memoryPoints[] = {"dmgzone_w_backR"};
					componentNames[] = {"dmgzone_w_backR"};
				};
				class WindowL1: WindowBL
				{
					memoryPoints[] = {"dmgzone_w_left1"};
					componentNames[] = {"dmgzone_w_left1"};
				};
				class WindowL2: WindowBL
				{
					memoryPoints[] = {"dmgzone_w_left2"};
					componentNames[] = {"dmgzone_w_left2"};
				};
				class WindowR1: WindowBL
				{
					memoryPoints[] = {"dmgzone_w_right1"};
					componentNames[] = {"dmgzone_w_right1"};
				};
				class WindowR2: WindowBL
				{
					memoryPoints[] = {"dmgzone_w_right2"};
					componentNames[] = {"dmgzone_w_right2"};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					memoryPoints[] = {"dmgzone_engine"};
					componentNames[] = {"dmgzone_engine"};
					fatalInjuryCoef = 0.001;
					inventorySlots[] = {"CarBattery","SparkPlug","Radiator"};
					inventorySlotsCoefs[] = {0.2,0.1,0.2};
				};
				class Radiator
				{
					class Health
					{
						hitpoints = 800;
						transferToGlobalCoef = 0;
					};
					memoryPoints[] = {};
					componentNames[] = {};
					fatalInjuryCoef = -1;
					inventorySlots[] = {"CarRadiator"};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 2500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 100;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\landrover\data\lr_headlights.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}}};
					};
					memoryPoints[] = {"dmgzone_lights_1_1"};
					componentNames[] = {"dmgzone_lights_1_1"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[] = {"dmgzone_lights_2_1"};
					componentNames[] = {"dmgzone_lights_2_1"};
					inventorySlots[] = {"Reflector_2_1"};
				};
				class Light_Bumper_1: Reflector_1_1
				{
					componentNames[] = {"dmgzone_light_bumper_1"};
					memoryPoints[] = {};
					inventorySlots[] = {};
				};
				class Light_Bumper_2: Reflector_1_1
				{
					componentNames[] = {"dmgzone_light_bumper_2"};
					memoryPoints[] = {};
					inventorySlots[] = {};
				};
				class Light_Bar_1_1: Reflector_1_1
				{
					componentNames[] = {"dmgzone_light_roof_1_1"};
					memoryPoints[] = {};
					inventorySlots[] = {};
				};
				class Light_Bar_1_2: Reflector_1_1
				{
					componentNames[] = {"dmgzone_light_roof_1_2"};
					memoryPoints[] = {};
					inventorySlots[] = {};
				};
				class Light_Bar_2_1: Reflector_1_1
				{
					componentNames[] = {"dmgzone_light_roof_2_1"};
					memoryPoints[] = {};
					inventorySlots[] = {};
				};
				class Light_Bar_2_2: Reflector_1_1
				{
					componentNames[] = {"dmgzone_light_roof_2_2"};
					memoryPoints[] = {};
					inventorySlots[] = {};
				};
			};
		};
		class ObstacleGenerator
		{
			carve = 1;
			timeToStationary = 5;
			moveThreshold = 0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,0.7};
					};
					class Cyl3
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,-0.7};
					};
				};
			};
		};
		class AnimationSources
		{
			class DoorsDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class DoorsCoDriver: DoorsDriver{};
			class DoorsCargo1: DoorsDriver{};
			class DoorsCargo2: DoorsDriver{};
			class DoorsHood: DoorsDriver{};
			class DoorsTrunk: DoorsDriver{};
			class EngineShake
			{
				source = "enginerun";
				initPhase = 0;
				animPeriod = 0.2;
			};
			class EngineShake1: EngineShake{};
			class EngineFan
			{
				source = "enginerun";
				initPhase = 0;
				animPeriod = 1.0;
			};
			class HideDestroyed_1_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_1_2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_2_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_2_2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class AnimHitWheel_1_1
			{
				source = "Hit";
				hitpoint = "HitWheel_1_1";
				raw = 1;
			};
			class AnimHitWheel_1_2: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_1_2";
			};
			class AnimHitWheel_2_1: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_2_1";
			};
			class AnimHitWheel_2_2: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_2_2";
			};
			class HitDoorsHood: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsHood";
			};
			class HitDoorsTrunk: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsTrunk";
			};
			class HitDoorsDrivers: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsDriver";
			};
			class HitDoorsCoDrivers: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsCoDriver";
			};
			class HitDoorsCargo: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsCargo";
			};
			class SeatDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.8;
			};
			class SeatCoDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.8;
			};
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.4002;
				animPeriod = 1;
			};
			class damper_2_2: damper_1_2{};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"offroad_Engine_Offload_Ext_Rpm1_SoundSet","offroad_Engine_Offload_Ext_Rpm2_SoundSet","offroad_Engine_Offload_Ext_Rpm3_SoundSet","offroad_Engine_Offload_Ext_Rpm4_SoundSet","offroad_Engine_Offload_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Rpm0_SoundSet","offroad_Engine_Ext_Rpm1_SoundSet","offroad_Engine_Ext_Rpm2_SoundSet","offroad_Engine_Ext_Rpm3_SoundSet","offroad_Engine_Ext_Rpm4_SoundSet","offroad_Engine_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Broken_SoundSet","offroad_Tires_rock_slow_Ext_SoundSet","offroad_Tires_rock_fast_Ext_SoundSet","offroad_Tires_grass_slow_Ext_SoundSet","offroad_Tires_grass_fast_Ext_SoundSet","offroad_Tires_gravel_slow_Ext_SoundSet","offroad_Tires_gravel_fast_Ext_SoundSet","offroad_Tires_gravel_dust_fast_Ext_SoundSet","offroad_Tires_asphalt_slow_Ext_SoundSet","offroad_Tires_asphalt_fast_Ext_SoundSet","offroad_Tires_water_slow_Ext_SoundSet","offroad_Tires_water_fast_Ext_SoundSet","Offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_Rain_Ext_SoundSet","offroad_damper_left_SoundSet","offroad_damper_right_SoundSet"};
			soundSetsInt[] = {"Offroad_Tires_Asphalt_Slow_General_Int_SoundSet","Offroad_Tires_Asphalt_Fast_General_Int_SoundSet","Offroad_Wind_SoundSet"};
		};
	};
	class Expansion_Landrover: Expansion_Landrover_Base
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_LANDROVER";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DESC";
		Model = "\DayzExpansion\Vehicles\Ground\LandRover\expansion_landrover.p3d";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","expansion_landrover_left","expansion_landrover_right","expansion_landrover_trunk","expansion_landrover_hood","expansion_landrover_codriverdoor","expansion_landrover_driverdoor","expansion_landrover_wheel_1_1","expansion_landrover_wheel_1_2","expansion_landrover_wheel_2_1","expansion_landrover_wheel_2_2","expansion_landrover_sparewheel","CanisterGasoline","Sea_Chest","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3"};
		frontReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_lights_on.rvmat";
		frontReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_lights_off.rvmat";
		brakeReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_brake_lights.rvmat";
		brakeReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat";
		ReverseReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_reverse_lights.rvmat";
		ReverseReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat";
		TailReflectorMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_brake_lights.rvmat";
		TailReflectorMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_outdoor.rvmat";
		dashboardMatOn = "dayzexpansion\vehicles\ground\landrover\data\lr_dashboard_lights.rvmat";
		dashboardMatOff = "dayzexpansion\vehicles\ground\landrover\data\lr_body_inside.rvmat";
		class AnimationSources
		{
			class DoorsDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class DoorsCoDriver: DoorsDriver{};
			class DoorsCargo1: DoorsDriver{};
			class DoorsCargo2: DoorsDriver{};
			class DoorsHood: DoorsDriver{};
			class DoorsTrunk: DoorsDriver{};
			class EngineShake
			{
				source = "enginerun";
				initPhase = 0;
				animPeriod = 0.2;
			};
			class EngineShake1: EngineShake{};
			class EngineFan
			{
				source = "enginerun";
				initPhase = 0;
			};
			class HideDestroyed_1_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_1_2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_2_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_2_2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class AnimHitWheel_1_1
			{
				source = "Hit";
				hitpoint = "HitWheel_1_1";
				raw = 1;
			};
			class AnimHitWheel_1_2: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_1_2";
			};
			class AnimHitWheel_2_1: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_2_1";
			};
			class AnimHitWheel_2_2: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_2_2";
			};
			class HitDoorsHood: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsHood";
			};
			class HitDoorsTrunk: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsTrunk";
			};
			class HitDoorsDrivers: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsDriver";
			};
			class HitDoorsCoDrivers: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsCoDriver";
			};
			class HitDoorsCargo: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsCargo";
			};
			class SeatDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.8;
			};
			class SeatCoDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.8;
			};
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.4002;
				animPeriod = 1;
			};
			class damper_2_2: damper_1_2{};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanYellow: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Yellow_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Yellow_DESC";
		hiddenSelectionsTextures[] = {"\DayzExpansion\Objects\Gear\Spraycans\data\spraycan_blue_co.paa"};
		skinName = "Yellow";
	};
	class Expansion_Landrover_Yellow: Expansion_Landrover
	{
		skinBase = "Expansion_Landrover";
		skinName = "Yellow";
	};
	class Expansion_Landrover_Driverdoor_Yellow: Expansion_Landrover_Driverdoor
	{
		skinBase = "Expansion_Landrover_Driverdoor";
		skinName = "Yellow";
	};
	class Expansion_Landrover_Codriverdoor_Yellow: Expansion_Landrover_Codriverdoor
	{
		skinBase = "Expansion_Landrover_Codriverdoor";
		skinName = "Yellow";
	};
	class Expansion_Landrover_Left_Yellow: Expansion_Landrover_Left
	{
		skinBase = "Expansion_Landrover_Left";
		skinName = "Yellow";
	};
	class Expansion_Landrover_Right_Yellow: Expansion_Landrover_Right
	{
		skinBase = "Expansion_Landrover_Right";
		skinName = "Yellow";
	};
	class Expansion_Landrover_Trunk_Yellow: Expansion_Landrover_Trunk
	{
		skinBase = "Expansion_Landrover_Trunk";
		skinName = "Yellow";
	};
	class Expansion_Landrover_Hood_Yellow: Expansion_Landrover_Hood
	{
		skinBase = "Expansion_Landrover_Hood";
		skinName = "Yellow";
	};
	class Expansion_Landrover_Wheel_Yellow: Expansion_Landrover_Wheel
	{
		skinBase = "Expansion_Landrover_Wheel";
		skinName = "Yellow";
	};
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"Dz\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class Expansion_Landrover_Green: Expansion_Landrover
	{
		skinBase = "Expansion_Landrover";
		skinName = "Green";
	};
	class Expansion_Landrover_Driverdoor_Green: Expansion_Landrover_Driverdoor
	{
		skinBase = "Expansion_Landrover_Driverdoor";
		skinName = "Green";
	};
	class Expansion_Landrover_Codriverdoor_Green: Expansion_Landrover_Codriverdoor
	{
		skinBase = "Expansion_Landrover_Codriverdoor";
		skinName = "Green";
	};
	class Expansion_Landrover_Left_Green: Expansion_Landrover_Left
	{
		skinBase = "Expansion_Landrover_Left";
		skinName = "Green";
	};
	class Expansion_Landrover_Right_Green: Expansion_Landrover_Right
	{
		skinBase = "Expansion_Landrover_Right";
		skinName = "Green";
	};
	class Expansion_Landrover_Trunk_Green: Expansion_Landrover_Trunk
	{
		skinBase = "Expansion_Landrover_Trunk";
		skinName = "Green";
	};
	class Expansion_Landrover_Hood_Green: Expansion_Landrover_Hood
	{
		skinBase = "Expansion_Landrover_Hood";
		skinName = "Green";
	};
	class Expansion_Landrover_Wheel_Green: Expansion_Landrover_Wheel
	{
		skinBase = "Expansion_Landrover_Wheel";
		skinName = "Green";
	};
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[] = {"Dz\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Red";
	};
	class Expansion_Landrover_Red: Expansion_Landrover
	{
		skinBase = "Expansion_Landrover";
		skinName = "Red";
	};
	class Expansion_Landrover_Driverdoor_Red: Expansion_Landrover_Driverdoor
	{
		skinBase = "Expansion_Landrover_Driverdoor";
		skinName = "Red";
	};
	class Expansion_Landrover_Codriverdoor_Red: Expansion_Landrover_Codriverdoor
	{
		skinBase = "Expansion_Landrover_Codriverdoor";
		skinName = "Red";
	};
	class Expansion_Landrover_Left_Red: Expansion_Landrover_Left
	{
		skinBase = "Expansion_Landrover_Left";
		skinName = "Red";
	};
	class Expansion_Landrover_Right_Red: Expansion_Landrover_Right
	{
		skinBase = "Expansion_Landrover_Right";
		skinName = "Red";
	};
	class Expansion_Landrover_Trunk_Red: Expansion_Landrover_Trunk
	{
		skinBase = "Expansion_Landrover_Trunk";
		skinName = "Red";
	};
	class Expansion_Landrover_Hood_Red: Expansion_Landrover_Hood
	{
		skinBase = "Expansion_Landrover_Hood";
		skinName = "Red";
	};
	class Expansion_Landrover_Wheel_Red: Expansion_Landrover_Wheel
	{
		skinBase = "Expansion_Landrover_Wheel";
		skinName = "Red";
	};
	class ExpansionSpraycanGrey: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Grey_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Grey_DESC";
		hiddenSelectionsTextures[] = {"\DayzExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"};
		skinName = "Grey";
	};
	class Expansion_Landrover_Grey: Expansion_Landrover
	{
		skinBase = "Expansion_Landrover";
		skinName = "Grey";
	};
	class Expansion_Landrover_Driverdoor_Grey: Expansion_Landrover_Driverdoor
	{
		skinBase = "Expansion_Landrover_Driverdoor";
		skinName = "Grey";
	};
	class Expansion_Landrover_Codriverdoor_Grey: Expansion_Landrover_Codriverdoor
	{
		skinBase = "Expansion_Landrover_Codriverdoor";
		skinName = "Grey";
	};
	class Expansion_Landrover_Left_Grey: Expansion_Landrover_Left
	{
		skinBase = "Expansion_Landrover_Left";
		skinName = "Grey";
	};
	class Expansion_Landrover_Right_Grey: Expansion_Landrover_Right
	{
		skinBase = "Expansion_Landrover_Right";
		skinName = "Grey";
	};
	class Expansion_Landrover_Trunk_Grey: Expansion_Landrover_Trunk
	{
		skinBase = "Expansion_Landrover_Trunk";
		skinName = "Grey";
	};
	class Expansion_Landrover_Hood_Grey: Expansion_Landrover_Hood
	{
		skinBase = "Expansion_Landrover_Hood";
		skinName = "Grey";
	};
	class Expansion_Landrover_Wheel_Grey: Expansion_Landrover_Wheel
	{
		skinBase = "Expansion_Landrover_Wheel";
		skinName = "Grey";
	};
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[] = {"\DayzExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"};
		skinName = "Blue";
	};
	class Expansion_Landrover_Blue: Expansion_Landrover
	{
		skinBase = "Expansion_Landrover";
		skinName = "Blue";
	};
	class Expansion_Landrover_Driverdoor_Blue: Expansion_Landrover_Driverdoor
	{
		skinBase = "Expansion_Landrover_Driverdoor";
		skinName = "Blue";
	};
	class Expansion_Landrover_Codriverdoor_Blue: Expansion_Landrover_Codriverdoor
	{
		skinBase = "Expansion_Landrover_Codriverdoor";
		skinName = "Blue";
	};
	class Expansion_Landrover_Left_Blue: Expansion_Landrover_Left
	{
		skinBase = "Expansion_Landrover_Left";
		skinName = "Blue";
	};
	class Expansion_Landrover_Right_Blue: Expansion_Landrover_Right
	{
		skinBase = "Expansion_Landrover_Right";
		skinName = "Blue";
	};
	class Expansion_Landrover_Trunk_Blue: Expansion_Landrover_Trunk
	{
		skinBase = "Expansion_Landrover_Trunk";
		skinName = "Blue";
	};
	class Expansion_Landrover_Hood_Blue: Expansion_Landrover_Hood
	{
		skinBase = "Expansion_Landrover_Hood";
		skinName = "Blue";
	};
	class Expansion_Landrover_Wheel_Blue: Expansion_Landrover_Wheel
	{
		skinBase = "Expansion_Landrover_Wheel";
		skinName = "Blue";
	};
	class ExpansionSpraycanOrange: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Orange_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Orange_DESC";
		hiddenSelectionsTextures[] = {"\DayzExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"};
		skinName = "Orange";
	};
	class Expansion_Landrover_Orange: Expansion_Landrover
	{
		skinBase = "Expansion_Landrover";
		skinName = "Orange";
	};
	class Expansion_Landrover_Driverdoor_Orange: Expansion_Landrover_Driverdoor
	{
		skinBase = "Expansion_Landrover_Driverdoor";
		skinName = "Orange";
	};
	class Expansion_Landrover_Codriverdoor_Orange: Expansion_Landrover_Codriverdoor
	{
		skinBase = "Expansion_Landrover_Codriverdoor";
		skinName = "Orange";
	};
	class Expansion_Landrover_Left_Orange: Expansion_Landrover_Left
	{
		skinBase = "Expansion_Landrover_Left";
		skinName = "Orange";
	};
	class Expansion_Landrover_Right_Orange: Expansion_Landrover_Right
	{
		skinBase = "Expansion_Landrover_Right";
		skinName = "Orange";
	};
	class Expansion_Landrover_Trunk_Orange: Expansion_Landrover_Trunk
	{
		skinBase = "Expansion_Landrover_Trunk";
		skinName = "Orange";
	};
	class Expansion_Landrover_Hood_Orange: Expansion_Landrover_Hood
	{
		skinBase = "Expansion_Landrover_Hood";
		skinName = "Orange";
	};
	class Expansion_Landrover_Wheel_Orange: Expansion_Landrover_Wheel
	{
		skinBase = "Expansion_Landrover_Wheel";
		skinName = "Orange";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class Proxyexpansion_landrover_driverdoor: ProxyVehiclePart
	{
		Model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_driverdoor.p3d";
		inventorySlot = "expansion_landrover_driverdoor";
	};
	class Proxyexpansion_landrover_codriverdoor: ProxyVehiclePart
	{
		Model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_codriverdoor.p3d";
		inventorySlot = "expansion_landrover_codriverdoor";
	};
	class Proxyexpansion_landrover_left: ProxyVehiclePart
	{
		Model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_left.p3d";
		inventorySlot = "expansion_landrover_left";
	};
	class Proxyexpansion_landrover_right: ProxyVehiclePart
	{
		Model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_right.p3d";
		inventorySlot = "expansion_landrover_right";
	};
	class Proxyexpansion_landrover_trunk: ProxyVehiclePart
	{
		Model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_trunk.p3d";
		inventorySlot = "expansion_landrover_trunk";
	};
	class Proxyexpansion_landrover_hood: ProxyVehiclePart
	{
		Model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\expansion_landrover_hood.p3d";
		inventorySlot = "expansion_landrover_hood";
	};
	class Proxylandrover_wheel: ProxyVehiclePart
	{
		model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\landrover_wheel.p3d";
		inventorySlot[] = {"expansion_landrover_wheel_1_1","expansion_landrover_wheel_1_2","expansion_landrover_wheel_2_1","expansion_landrover_wheel_2_2","expansion_landrover_sparewheel"};
	};
	class Proxylandrover_wheel_destroyed: ProxyVehiclePart
	{
		model = "DayzExpansion\Vehicles\Ground\LandRover\proxy\landrover_wheel_destroyed.p3d";
		inventorySlot[] = {"expansion_landrover_wheel_1_1","expansion_landrover_wheel_1_2","expansion_landrover_wheel_2_1","expansion_landrover_wheel_2_2","expansion_landrover_sparewheel"};
	};
	class Proxyjerrycan: ProxyVehiclePart
	{
		model = "dz\vehicles\parts\jerrycan.p3d";
		inventorySlot[] = {"CanisterGasoline"};
	};
	class Proxysea_chest: ProxyVehiclePart
	{
		model = "dz\gear\camping\sea_chest.p3d";
		inventorySlot[] = {"sea_chest","sea_chest_1","sea_chest_2","sea_chest_3","sea_chest_4"};
	};
	class Proxywooden_case: ProxyVehiclePart
	{
		model = "dz\gear\camping\wooden_case.p3d";
		inventorySlot[] = {"WoodenCrate","woodencrate_1","woodencrate_2","woodencrate_3","woodencrate_4","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","Truck_01_WoodenCrate4","Truck_01_WoodenCrate5","Truck_01_WoodenCrate6"};
	};
};
