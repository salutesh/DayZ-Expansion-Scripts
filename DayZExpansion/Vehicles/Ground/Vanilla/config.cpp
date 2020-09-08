#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Vanilla
	{
		units[] = {"ExpansionCarDoorFix","ExpansionSpraycanBlack","ExpansionSpraycanGrey","ExpansionSpraycanWhite","ExpansionSpraycanGreen","ExpansionSpraycanRed","ExpansionSpraycanBlue","ExpansionSpraycanBanditKitty","ExpansionSpraycanYellow","ExpansionSpraycanWine","Expansion_HatchbackDoors_Driver","Expansion_HatchbackDoors_CoDriver","Expansion_HatchbackHood","Expansion_HatchbackTrunk","ExpansionCivSedanWheel","ExpansionCivilianSedanTest","Expansion_CivSedanDoors_Driver","Expansion_CivSedanDoors_CoDriver","Expansion_CivSedanDoors_BackLeft","Expansion_CivSedanDoors_BackRight","Expansion_CivSedanHood","Expansion_CivSedanTrunk","Expansion_Hatchback_02_Door_1_1","Expansion_Hatchback_02_Door_1_2","Expansion_Hatchback_02_Door_2_1","Expansion_Hatchback_02_Door_2_2","Expansion_Hatchback_02_Hood","Expansion_Hatchback_02_Trunk","Expansion_Sedan_02_Door_1_1","Expansion_Sedan_02_Door_1_2","Expansion_Sedan_02_Door_2_1","Expansion_Sedan_02_Door_2_2","Expansion_Sedan_02_Hood","Expansion_Sedan_02_Trunk"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgSlots
{
	class Slot_Expansion_CivSedanWheel_1_1
	{
		name = "Expansion_CivSedanWheel_1_1";
		displayName = "Front Left CivSedan Wheel";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_CivSedanWheel_1_2
	{
		name = "Expansion_CivSedanWheel_1_2";
		displayName = "Front Right CivSedan Wheel";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_CivSedanWheel_2_1
	{
		name = "Expansion_CivSedanWheel_2_1";
		displayName = "Front Left CivSedan Wheel";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_CivSedanWheel_2_2
	{
		name = "Expansion_CivSedanWheel_2_2";
		displayName = "Front Right CivSedan Wheel";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_CivSedanWheel_Spare_1
	{
		name = "Expansion_CivSedanWheel_Spare_1";
		displayName = "Spare CivSedan Wheel";
		selection = "";
		ghostIcon = "";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class CarScript;
	class CarDoor: Inventory_Base
	{
		hiddenSelections[] = {"dmgZone_doors"};
		hiddenSelectionsTextures[] = {""};
		hiddenSelectionsMaterials[] = {""};
	};
	class ExpansionCarDoorFix: CarDoor
	{
		scope = 2;
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanBlack: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Black_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Black_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Black";
	};
	class ExpansionSpraycanGrey: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Grey_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Grey_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_grey_co.paa"};
		skinName = "Grey";
	};
	class ExpansionSpraycanWhite: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_White_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_White_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_white_co.paa"};
		skinName = "White";
	};
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_red_co.paa"};
		skinName = "Red";
	};
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_blue_co.paa"};
		skinName = "Blue";
	};
	class ExpansionSpraycanBanditKitty: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BanditKitty_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BanditKitty_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_pink_co.paa"};
		skinName = "BanditKitty";
	};
	class ExpansionSpraycanYellow: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Yellow_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Yellow_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_yellow_co.paa"};
		skinName = "Yellow";
	};
	class ExpansionSpraycanWine: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Wine_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Wine_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_wine_co.paa"};
		skinName = "Wine";
	};
	class OffroadHatchback: CarScript
	{
		doors[] = {"NivaDriverDoors","NivaCoDriverDoors","NivaTrunk","NivaHood"};
		applySkinsTo[] = {"HatchbackDoors_Driver","HatchbackDoors_CoDriver","HatchbackHood","HatchbackTrunk","Expansion_HatchbackDoors_Driver","Expansion_HatchbackDoors_CoDriver","Expansion_HatchbackHood","Expansion_HatchbackTrunk"};
		defaultSkin = "Green";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
					};
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_chassis"};
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
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine","WindowFront"};
					transferToZonesThreshold[] = {0.5,0.5,0.5,0.1};
					transferToZonesCoefs[] = {0.7,0.7,0.5,0.05};
					inventorySlots[] = {"NivaHood","CarRadiator"};
					inventorySlotsCoefs[] = {0.3,0.3};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_lights_1_1"};
					componentNames[] = {"dmgZone_lights_1_1"};
					transferToZonesNames[] = {"Front","Fender_1_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Reflector_2_1: Reflector_1_1
				{
					transferToZonesNames[] = {"Front","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					inventorySlots[] = {"Reflector_2_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Back
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					transferToZonesNames[] = {"Fender_1_2","Fender_2_2","WindowLR","WindowRR"};
					transferToZonesCoefs[] = {0.3,0.3,0.2,0.2};
					inventorySlots[] = {"NivaTrunk"};
					inventorySlotsCoefs[] = {0.8};
				};
				class Roof
				{
					class Health
					{
						hitpoints = 700;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_roof"};
					componentNames[] = {"dmgZone_roof"};
					inventorySlotsCoefs[] = {0.0};
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints = 1200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_fender_1_1"};
					componentNames[] = {"dmgZone_fender_1_1"};
					transferToZonesNames[] = {"Front","Engine","Reflector_1_1","WindowFront"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4};
					inventorySlots[] = {"NivaHood","NivaWheel_1_1","NivaDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.3,0.3};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_1_2"};
					componentNames[] = {"dmgZone_fender_1_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowLR"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlots[] = {"NivaTrunk","NivaWheel_1_2"};
					inventorySlotsCoefs[] = {0.3,0.1};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
					transferToZonesNames[] = {"Front","Engine","Reflector_2_1","WindowFront","Battery"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4,0.7};
					inventorySlots[] = {"NivaHood","NivaWheel_2_1","NivaCoDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.2,3};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_2"};
					componentNames[] = {"dmgZone_fender_2_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowRR"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlots[] = {"NivaTrunk","NivaWheel_2_2","NivaCoDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.1};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 150;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowFront"};
					componentNames[] = {"dmgZone_windowFront"};
					inventorySlots[] = {};
				};
				class WindowLR
				{
					class Health
					{
						hitpoints = 150;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowLeft"};
					componentNames[] = {"dmgZone_windowLeft"};
					inventorySlots[] = {};
				};
				class WindowRR: WindowLR
				{
					memoryPoints[] = {"dmgZone_windowRight"};
					componentNames[] = {"dmgZone_windowRight"};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}}};
					};
					fatalInjuryCoef = 0.001;
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					inventorySlots[] = {"CarBattery","SparkPlug","EngineBelt","CarRadiator"};
					inventorySlotsCoefs[] = {0.2,0.2,0.01,0.4};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 600;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_fuelTank"};
					inventorySlots[] = {};
				};
			};
		};
	};
	class Transport;
	class Car: Transport
	{
		acceleration = 3;
	};
	class OffroadHatchback_Test: OffroadHatchback
	{
		acceleration = 1;
	};
	class HatchbackDoors_Driver: CarDoor
	{
		applySkinsTo[] = {"HatchbackDoors_CoDriver","Expansion_HatchbackDoors_CoDriver"};
		defaultSkin = "Green";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class HatchbackDoors_CoDriver: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class HatchbackHood: CarDoor
	{
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
		};
	};
	class HatchbackTrunk: CarDoor
	{
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
		};
	};
	class Expansion_HatchbackDoors_Driver: ExpansionCarDoorFix
	{
		newBase = "HatchbackDoors_Driver";
	};
	class Expansion_HatchbackDoors_CoDriver: ExpansionCarDoorFix
	{
		newBase = "HatchbackDoors_CoDriver";
	};
	class Expansion_HatchbackHood: ExpansionCarDoorFix
	{
		newBase = "HatchbackHood";
	};
	class Expansion_HatchbackTrunk: ExpansionCarDoorFix
	{
		newBase = "HatchbackTrunk";
	};
	class OffroadHatchback_Green: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "Green";
	};
	class HatchbackDoors_Driver_Green: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "Green";
	};
	class HatchbackDoors_CoDriver_Green: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "Green";
	};
	class HatchbackHood_Green: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "Green";
	};
	class HatchbackTrunk_Green: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "Green";
	};
	class OffroadHatchback_Blue: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "Blue";
	};
	class HatchbackDoors_Driver_Blue: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "Blue";
	};
	class HatchbackDoors_CoDriver_Blue: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "Blue";
	};
	class HatchbackHood_Blue: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "Blue";
	};
	class HatchbackTrunk_Blue: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "Blue";
	};
	class OffroadHatchback_White: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "White";
	};
	class HatchbackDoors_Driver_White: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "White";
	};
	class HatchbackDoors_CoDriver_White: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "White";
	};
	class HatchbackHood_White: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "White";
	};
	class HatchbackTrunk_White: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "White";
	};
	class HatchbackDoors_Driver_GreenRust: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "GreenRust";
	};
	class HatchbackDoors_CoDriver_GreenRust: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "GreenRust";
	};
	class HatchbackHood_GreenRust: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "GreenRust";
	};
	class HatchbackTrunk_GreenRust: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "GreenRust";
	};
	class HatchbackDoors_Driver_BlueRust: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "BlueRust";
	};
	class HatchbackDoors_CoDriver_BlueRust: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "BlueRust";
	};
	class HatchbackHood_BlueRust: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "BlueRust";
	};
	class HatchbackTrunk_BlueRust: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "BlueRust";
	};
	class HatchbackDoors_Driver_WhiteRust: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "WhiteRust";
	};
	class HatchbackDoors_CoDriver_WhiteRust: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "WhiteRust";
	};
	class HatchbackHood_WhiteRust: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "WhiteRust";
	};
	class HatchbackTrunk_WhiteRust: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "WhiteRust";
	};
	class ExpansionWheel;
	class ExpansionCivSedanWheel: ExpansionWheel
	{
		scope = 2;
		displayName = "$STR_CivSedanWheel0";
		descriptionShort = "$STR_CivSedanWheel1";
		model = "\DZ\vehicles\wheeled\civiliansedan\proxy\sedanwheel.p3d";
		inventorySlot[] = {"Expansion_CivSedanWheel_Spare_1","Expansion_CivSedanWheel_1_1","Expansion_CivSedanWheel_1_2","Expansion_CivSedanWheel_2_1","Expansion_CivSedanWheel_2_2"};
		rotationFlags = 4;
		radiusByDamage[] = {0,0.34,0.3,0.3,0.9998,0.25,0.9999,0.2};
		radius = 0.34;
		friction = 0.96;
		width = 0.16;
		tyreRollResistance = 0.015;
		tyreTread = 1.0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel_damage.rvmat"}},{0.3,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel_damage.rvmat"}},{0.0,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionCarScript;
	class ExpansionCivilianSedanTest: ExpansionCarScript
	{
		scope = 2;
		displayName = "CIVILLIAN SEDAN TEST";
		descriptionShort = "$STR_CivilianSedan1";
		model = "\DZ\vehicles\wheeled\civiliansedan\CivilianSedan.p3d";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","CivSedanDriverDoors","CivSedanCoDriverDoors","CivSedanCargo1Doors","CivSedanCargo2Doors","CivSedanHood","CivSedanTrunk","Expansion_CivSedanWheel_1_1","Expansion_CivSedanWheel_1_2","Expansion_CivSedanWheel_2_1","Expansion_CivSedanWheel_2_2"};
		class Crew
		{
			class Driver
			{
				isDriver = 1;
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo1
			{
				actionSel = "seat_cargo1";
				proxyPos = "crewCargo1";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
		};
		class VehicleSimulation
		{
			class Axles
			{
				class Front
				{
					maxSteeringAngle = 35;
					finalRatio = 0.0;
					brakeBias = 0.7;
					brakeForce = 4500;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 300;
						stiffness = 60000;
						compression = 2100;
						damping = 5500;
						travelMaxUp = 0.079;
						travelMaxDown = 0.06;
					};
					class Wheels
					{
						class Left
						{
							inventorySlot = "Expansion_CivSedanWheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right
						{
							inventorySlot = "Expansion_CivSedanWheel_2_1";
							animTurn = "turnfrontright";
							animRotation = "wheelfrontright";
							animDamper = "damper_2_1";
							wheelHub = "wheel_2_1_damper_land";
						};
					};
				};
				class Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.3;
					brakeForce = 3500;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 275;
						stiffness = 34000;
						compression = 2000;
						damping = 5100;
						travelMaxUp = 0.086;
						travelMaxDown = 0.133;
					};
					class Wheels
					{
						class Left
						{
							inventorySlot = "Expansion_CivSedanWheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right
						{
							inventorySlot = "Expansion_CivSedanWheel_2_2";
							animTurn = "turnbackright";
							animRotation = "wheelbackright";
							animDamper = "damper_2_2";
							wheelHub = "wheel_2_2_damper_land";
						};
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
			class DoorsHood: DoorsDriver{};
			class DoorsTrunk: DoorsDriver{};
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
				animPeriod = 0.0001;
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
			class DoorsCargo1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class DoorsCargo2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1e-05;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.5936;
				animPeriod = 1e-05;
			};
			class damper_2_2: damper_1_2{};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","CivSedanHood","CivSedanTrunk","CivSedanDriverDoors","CivSedanCoDriverDoors","CivSedanCargo1Doors","CivSedanCargo2Doors"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"Expansion_CivSedanWheel_1_1","Expansion_CivSedanWheel_1_2","Expansion_CivSedanWheel_2_1","Expansion_CivSedanWheel_2_2","Expansion_CivSedanWheel_Spare_1"};
			};
		};
	};
	class CivilianSedan: CarScript
	{
		doors[] = {"CivSedanDriverDoors","CivSedanCoDriverDoors","CivSedanCargo1Doors","CivSedanCargo2Doors","CivSedanTrunk","CivSedanHood"};
		applySkinsTo[] = {"CivSedanDoors_Driver","CivSedanDoors_CoDriver","CivSedanDoors_BackLeft","CivSedanDoors_BackRight","CivSedanHood","CivSedanTrunk","Expansion_CivSedanDoors_Driver","Expansion_CivSedanDoors_CoDriver","Expansion_CivSedanDoors_BackLeft","Expansion_CivSedanDoors_BackRight","Expansion_CivSedanHood","Expansion_CivSedanTrunk"};
		defaultSkin = "White";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
					};
					componentNames[] = {"dmgZone_chassis"};
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
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine","WindowFront"};
					transferToZonesThreshold[] = {0.5,0.5,0.5,0.1};
					transferToZonesCoefs[] = {0.7,0.7,0.5,0.05};
					inventorySlots[] = {"CivSedanHood","CarRadiator"};
					inventorySlotsCoefs[] = {0.3,0.3};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_destruct.rvmat"}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_lights_1_1"};
					componentNames[] = {"dmgZone_lights_1_1"};
					transferToZonesNames[] = {"Front","Fender_1_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlotsCoefs[] = {1.0};
					inventorySlots[] = {"Reflector_2_1"};
				};
				class Back
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					transferToZonesNames[] = {"Fender_1_2","Fender_2_2","WindowBack"};
					transferToZonesCoefs[] = {0.3,0.3,0.2};
					inventorySlots[] = {"CivSedanTrunk"};
				};
				class Roof
				{
					class Health
					{
						hitpoints = 700;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_roof"};
					componentNames[] = {"dmgZone_roof"};
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints = 1200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					transferToZonesNames[] = {"Front","Engine","Reflector_1_1","WindowFront"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4};
					memoryPoints[] = {"dmgZone_fender_1_1"};
					componentNames[] = {"dmgZone_fender_1_1"};
					inventorySlots[] = {"CivSedanHood","CivSedanWheel_1_1","CivSedanDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.3,0.3};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_1_2"};
					componentNames[] = {"dmgZone_fender_1_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowBack"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlotsCoefs[] = {0.3,0.1};
					inventorySlots[] = {"CivSedanTrunk","CivSedanWheel_1_2"};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
					transferToZonesNames[] = {"Front","Engine","Reflector_2_1","WindowFront","Battery"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4,0.7};
					inventorySlots[] = {"CivSedanHood","CivSedanWheel_2_1","CivSedanDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.2,31};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_2"};
					componentNames[] = {"dmgZone_fender_2_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowBack"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlots[] = {"CivSedanTrunk","CivSedanWheel_2_2"};
					inventorySlotsCoefs[] = {0.3,0.1};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 150;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowFront"};
					componentNames[] = {"dmgZone_windowFront"};
					inventorySlots[] = {};
				};
				class WindowBack: WindowFront
				{
					componentNames[] = {"dmgZone_windowBack"};
					memoryPoints[] = {"dmgZone_windowBack"};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}}};
					};
					fatalInjuryCoef = 0.001;
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					inventorySlots[] = {"CarBattery","SparkPlug","EngineBelt","CarRadiator"};
					inventorySlotsCoefs[] = {0.2,0.2,0.01,0.4};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 600;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_fuelTank"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class CivSedanDoors_Driver: CarDoor
	{
		defaultSkin = "White";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class CivSedanDoors_CoDriver: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class CivSedanDoors_BackLeft: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class CivSedanDoors_BackRight: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class CivSedanHood: CarDoor
	{
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
		};
	};
	class CivSedanTrunk: CarDoor
	{
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
		};
	};
	class Expansion_CivSedanDoors_Driver: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_Driver";
	};
	class Expansion_CivSedanDoors_CoDriver: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_CoDriver";
	};
	class Expansion_CivSedanDoors_BackLeft: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_BackLeft";
	};
	class Expansion_CivSedanDoors_BackRight: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_BackRight";
	};
	class Expansion_CivSedanHood: ExpansionCarDoorFix
	{
		newBase = "CivSedanHood";
	};
	class Expansion_CivSedanTrunk: ExpansionCarDoorFix
	{
		newBase = "CivSedanTrunk";
	};
	class CivilianSedan_White: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "White";
	};
	class CivSedanDoors_Driver_White: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "White";
	};
	class CivSedanDoors_CoDriver_White: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "White";
	};
	class CivSedanDoors_BackLeft_White: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "White";
	};
	class CivSedanDoors_BackRight_White: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "White";
	};
	class CivSedanHood_White: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "White";
	};
	class CivSedanTrunk_White: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "White";
	};
	class CivilianSedan_Wine: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Wine";
	};
	class CivSedanDoors_Driver_Wine: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Wine";
	};
	class CivSedanDoors_CoDriver_Wine: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Wine";
	};
	class CivSedanDoors_BackLeft_Wine: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Wine";
	};
	class CivSedanDoors_BackRight_Wine: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Wine";
	};
	class CivSedanHood_Wine: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Wine";
	};
	class CivSedanTrunk_Wine: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Wine";
	};
	class CivilianSedan_Black: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Black";
	};
	class CivSedanDoors_Driver_Black: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Black";
	};
	class CivSedanDoors_CoDriver_Black: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Black";
	};
	class CivSedanDoors_BackLeft_Black: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Black";
	};
	class CivSedanDoors_BackRight_Black: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Black";
	};
	class CivSedanHood_Black: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Black";
	};
	class CivSedanTrunk_Black: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Black";
	};
	class CivSedanDoors_Driver_WhiteRust: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_CoDriver_WhiteRust: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_BackLeft_WhiteRust: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_BackRight_WhiteRust: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "WhiteRust";
	};
	class CivSedanHood_WhiteRust: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "WhiteRust";
	};
	class CivSedanTrunk_WhiteRust: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_Driver_WineRust: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "WineRust";
	};
	class CivSedanDoors_CoDriver_WineRust: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "WineRust";
	};
	class CivSedanDoors_BackLeft_WineRust: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "WineRust";
	};
	class CivSedanDoors_BackRight_WineRust: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "WineRust";
	};
	class CivSedanHood_WineRust: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "WineRust";
	};
	class CivSedanTrunk_WineRust: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "WineRust";
	};
	class CivSedanDoors_Driver_BlackRust: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "BlackRust";
	};
	class CivSedanDoors_CoDriver_BlackRust: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "BlackRust";
	};
	class CivSedanDoors_BackLeft_BlackRust: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "BlackRust";
	};
	class CivSedanDoors_BackRight_BlackRust: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "BlackRust";
	};
	class CivSedanHood_BlackRust: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "BlackRust";
	};
	class CivSedanTrunk_BlackRust: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "BlackRust";
	};
	class Hatchback_02: CarScript
	{
		doors[] = {"Hatchback_02_Door_1_1","Hatchback_02_Door_1_2","Hatchback_02_Door_2_1","Hatchback_02_Door_2_2","Hatchback_02_Trunk","Hatchback_02_Hood"};
		defaultSkin = "Red";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","camo_chassi","dmgZone_chassis","dmgZone_back","dmgZone_front","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
					};
					componentNames[] = {"dmgZone_chassis"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Front
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_front"};
					memoryPoints[] = {"dmgZone_front","dmgZone_bumper_1"};
					fatalInjuryCoef = -1;
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine","WindowFront"};
					transferToZonesThreshold[] = {0.5,0.5,0.5,0.1};
					transferToZonesCoefs[] = {0.7,0.7,0.5,0.05};
					inventorySlots[] = {"Hatchback_02_Hood","CarRadiator"};
					inventorySlotsCoefs[] = {0.3,0.3};
				};
				class Back
				{
					class Health
					{
						hitpoints = 100;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_back"};
					memoryPoints[] = {"dmgZone_back","dmgZone_bumper_1"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 100;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_windowFront"};
					memoryPoints[] = {"dmgZone_windowFront"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"}}};
					};
					componentNames[] = {"dmgZone_lights_1_1"};
					memoryPoints[] = {"dmgZone_lights_1_1"};
					fatalInjuryCoef = -1;
					transferToZonesNames[] = {"Front","Fender_1_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Reflector_2_1: Reflector_1_1
				{
					componentNames[] = {"dmgZone_lights_2_1"};
					memoryPoints[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlots[] = {"Reflector_2_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Roof
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_roof"};
					memoryPoints[] = {"dmgZone_roof"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_fender_1_1"};
					memoryPoints[] = {"dmgZone_fender_1_1"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Fender_2_1: Fender_1_1
				{
					componentNames[] = {"dmgZone_fender_2_1"};
					memoryPoints[] = {"dmgZone_fender_2_1"};
				};
				class Fender_1_2: Fender_1_1
				{
					componentNames[] = {"dmgZone_fender_1_2"};
					memoryPoints[] = {"dmgZone_fender_1_2"};
				};
				class Fender_2_2: Fender_1_1
				{
					componentNames[] = {"dmgZone_fender_2_2"};
					memoryPoints[] = {"dmgZone_fender_2_2"};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 1;
					};
					componentNames[] = {"dmgZone_engine"};
					memoryPoints[] = {"dmgZone_engine"};
					fatalInjuryCoef = 0.001;
					inventorySlots[] = {};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = -1;
					};
					componentNames[] = {"dmgZone_fuelTank"};
					memoryPoints[] = {"dmgZone_fuelTank"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Hatchback_02_Door_1_1: CarDoor
	{
		applySkinsTo[] = {"Hatchback_02_Door_2_1","Expansion_Hatchback_02_Door_2_1"};
		defaultSkin = "Red";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Hatchback_02_Door_2_1: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Hatchback_02_Door_1_2: CarDoor
	{
		applySkinsTo[] = {"Hatchback_02_Door_2_2","Expansion_Hatchback_02_Door_2_2"};
		defaultSkin = "Red";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Hatchback_02_Door_2_2: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Hatchback_02_Hood: CarDoor
	{
		defaultSkin = "Red";
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
		};
	};
	class Hatchback_02_Trunk: CarDoor
	{
		defaultSkin = "Red";
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
		};
	};
	class Expansion_Hatchback_02_Door_1_1: ExpansionCarDoorFix
	{
		newBase = "Hatchback_02_Door_1_1";
	};
	class Expansion_Hatchback_02_Door_1_2: ExpansionCarDoorFix
	{
		newBase = "Hatchback_02_Door_1_2";
	};
	class Expansion_Hatchback_02_Door_2_1: ExpansionCarDoorFix
	{
		newBase = "Hatchback_02_Door_2_1";
	};
	class Expansion_Hatchback_02_Door_2_2: ExpansionCarDoorFix
	{
		newBase = "Hatchback_02_Door_2_2";
	};
	class Expansion_Hatchback_02_Hood: ExpansionCarDoorFix
	{
		newBase = "Hatchback_02_Hood";
	};
	class Expansion_Hatchback_02_Trunk: ExpansionCarDoorFix
	{
		newBase = "Hatchback_02_Trunk";
	};
	class Hatchback_02_Red: Hatchback_02
	{
		skinBase = "Hatchback_02";
		skinName = "Red";
	};
	class Hatchback_02_Door_1_1_Red: Hatchback_02_Door_1_1
	{
		skinBase = "Hatchback_02_Door_1_1";
		skinName = "Red";
	};
	class Hatchback_02_Door_1_2_Red: Hatchback_02_Door_1_2
	{
		skinBase = "Hatchback_02_Door_1_2";
		skinName = "Red";
	};
	class Hatchback_02_Door_2_1_Red: Hatchback_02_Door_2_1
	{
		skinBase = "Hatchback_02_Door_2_1";
		skinName = "Red";
	};
	class Hatchback_02_Door_2_2_Red: Hatchback_02_Door_2_2
	{
		skinBase = "Hatchback_02_Door_2_2";
		skinName = "Red";
	};
	class Hatchback_02_Hood_Red: Hatchback_02_Hood
	{
		skinBase = "Hatchback_02_Hood";
		skinName = "Red";
	};
	class Hatchback_02_Trunk_Red: Hatchback_02_Trunk
	{
		skinBase = "Hatchback_02_Trunk";
		skinName = "Red";
	};
	class Hatchback_02_Black: Hatchback_02
	{
		skinBase = "Hatchback_02";
		skinName = "Black";
	};
	class Hatchback_02_Door_1_1_Black: Hatchback_02_Door_1_1
	{
		skinBase = "Hatchback_02_Door_1_1";
		skinName = "Black";
	};
	class Hatchback_02_Door_1_2_Black: Hatchback_02_Door_1_2
	{
		skinBase = "Hatchback_02_Door_1_2";
		skinName = "Black";
	};
	class Hatchback_02_Door_2_1_Black: Hatchback_02_Door_2_1
	{
		skinBase = "Hatchback_02_Door_2_1";
		skinName = "Black";
	};
	class Hatchback_02_Door_2_2_Black: Hatchback_02_Door_2_2
	{
		skinBase = "Hatchback_02_Door_2_2";
		skinName = "Black";
	};
	class Hatchback_02_Hood_Black: Hatchback_02_Hood
	{
		skinBase = "Hatchback_02_Hood";
		skinName = "Black";
	};
	class Hatchback_02_Trunk_Black: Hatchback_02_Trunk
	{
		skinBase = "Hatchback_02_Trunk";
		skinName = "Black";
	};
	class Hatchback_02_Blue: Hatchback_02
	{
		skinBase = "Hatchback_02";
		skinName = "Blue";
	};
	class Hatchback_02_Door_1_1_Blue: Hatchback_02_Door_1_1
	{
		skinBase = "Hatchback_02_Door_1_1";
		skinName = "Blue";
	};
	class Hatchback_02_Door_1_2_Blue: Hatchback_02_Door_1_2
	{
		skinBase = "Hatchback_02_Door_1_2";
		skinName = "Blue";
	};
	class Hatchback_02_Door_2_1_Blue: Hatchback_02_Door_2_1
	{
		skinBase = "Hatchback_02_Door_2_1";
		skinName = "Blue";
	};
	class Hatchback_02_Door_2_2_Blue: Hatchback_02_Door_2_2
	{
		skinBase = "Hatchback_02_Door_2_2";
		skinName = "Blue";
	};
	class Hatchback_02_Hood_Blue: Hatchback_02_Hood
	{
		skinBase = "Hatchback_02_Hood";
		skinName = "Blue";
	};
	class Hatchback_02_Trunk_Blue: Hatchback_02_Trunk
	{
		skinBase = "Hatchback_02_Trunk";
		skinName = "Blue";
	};
	class Hatchback_02_Door_1_1_RedRust: Hatchback_02_Door_1_1
	{
		skinBase = "Hatchback_02_Door_1_1";
		skinName = "RedRust";
	};
	class Hatchback_02_Door_1_2_RedRust: Hatchback_02_Door_1_2
	{
		skinBase = "Hatchback_02_Door_1_2";
		skinName = "RedRust";
	};
	class Hatchback_02_Door_2_1_RedRust: Hatchback_02_Door_2_1
	{
		skinBase = "Hatchback_02_Door_2_1";
		skinName = "RedRust";
	};
	class Hatchback_02_Door_2_2_RedRust: Hatchback_02_Door_2_2
	{
		skinBase = "Hatchback_02_Door_2_2";
		skinName = "RedRust";
	};
	class Hatchback_02_Hood_RedRust: Hatchback_02_Hood
	{
		skinBase = "Hatchback_02_Hood";
		skinName = "RedRust";
	};
	class Hatchback_02_Trunk_RedRust: Hatchback_02_Trunk
	{
		skinBase = "Hatchback_02_Trunk";
		skinName = "RedRust";
	};
	class Hatchback_02_Door_1_1_BlackRust: Hatchback_02_Door_1_1
	{
		skinBase = "Hatchback_02_Door_1_1";
		skinName = "BlackRust";
	};
	class Hatchback_02_Door_1_2_BlackRust: Hatchback_02_Door_1_2
	{
		skinBase = "Hatchback_02_Door_1_2";
		skinName = "BlackRust";
	};
	class Hatchback_02_Door_2_1_BlackRust: Hatchback_02_Door_2_1
	{
		skinBase = "Hatchback_02_Door_2_1";
		skinName = "BlackRust";
	};
	class Hatchback_02_Door_2_2_BlackRust: Hatchback_02_Door_2_2
	{
		skinBase = "Hatchback_02_Door_2_2";
		skinName = "BlackRust";
	};
	class Hatchback_02_Hood_BlackRust: Hatchback_02_Hood
	{
		skinBase = "Hatchback_02_Hood";
		skinName = "BlackRust";
	};
	class Hatchback_02_Trunk_BlackRust: Hatchback_02_Trunk
	{
		skinBase = "Hatchback_02_Trunk";
		skinName = "BlackRust";
	};
	class Hatchback_02_Door_1_1_BlueRust: Hatchback_02_Door_1_1
	{
		skinBase = "Hatchback_02_Door_1_1";
		skinName = "BlueRust";
	};
	class Hatchback_02_Door_1_2_BlueRust: Hatchback_02_Door_1_2
	{
		skinBase = "Hatchback_02_Door_1_2";
		skinName = "BlueRust";
	};
	class Hatchback_02_Door_2_1_BlueRust: Hatchback_02_Door_2_1
	{
		skinBase = "Hatchback_02_Door_2_1";
		skinName = "BlueRust";
	};
	class Hatchback_02_Door_2_2_BlueRust: Hatchback_02_Door_2_2
	{
		skinBase = "Hatchback_02_Door_2_2";
		skinName = "BlueRust";
	};
	class Hatchback_02_Hood_BlueRust: Hatchback_02_Hood
	{
		skinBase = "Hatchback_02_Hood";
		skinName = "BlueRust";
	};
	class Hatchback_02_Trunk_BlueRust: Hatchback_02_Trunk
	{
		skinBase = "Hatchback_02_Trunk";
		skinName = "BlueRust";
	};
	class Hatchback_02_BanditKitty: Hatchback_02
	{
		skinBase = "Hatchback_02";
		skinName = "BanditKitty";
	};
	class Hatchback_02_Door_1_1_BanditKitty: Hatchback_02_Door_1_1
	{
		skinBase = "Hatchback_02_Door_1_1";
		skinName = "BanditKitty";
	};
	class Hatchback_02_Door_1_2_BanditKitty: Hatchback_02_Door_1_2
	{
		skinBase = "Hatchback_02_Door_1_2";
		skinName = "BanditKitty";
	};
	class Hatchback_02_Door_2_1_BanditKitty: Hatchback_02_Door_2_1
	{
		skinBase = "Hatchback_02_Door_2_1";
		skinName = "BanditKitty";
	};
	class Hatchback_02_Door_2_2_BanditKitty: Hatchback_02_Door_2_2
	{
		skinBase = "Hatchback_02_Door_2_2";
		skinName = "BanditKitty";
	};
	class Hatchback_02_Hood_BanditKitty: Hatchback_02_Hood
	{
		skinBase = "Hatchback_02_Hood";
		skinName = "BanditKitty";
	};
	class Hatchback_02_Trunk_BanditKitty: Hatchback_02_Trunk
	{
		skinBase = "Hatchback_02_Trunk";
		skinName = "BanditKitty";
	};
	class Sedan_02: CarScript
	{
		doors[] = {"Sedan_02_Door_1_1","Sedan_02_Door_1_2","Sedan_02_Door_2_1","Sedan_02_Door_2_2","Sedan_02_Trunk","Sedan_02_Hood"};
		defaultSkin = "Yellow";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","camo_chassi","dmgZone_chassis","dmgZone_back","dmgZone_front","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
					};
					componentNames[] = {"dmgZone_chassis"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Front
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_front"};
					memoryPoints[] = {"dmgZone_front","dmgZone_bumper_1"};
					fatalInjuryCoef = -1;
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine","WindowFront"};
					transferToZonesThreshold[] = {0.5,0.5,0.5,0.1};
					transferToZonesCoefs[] = {0.7,0.7,0.5,0.05};
					inventorySlots[] = {"Sedan_02_Hood","CarRadiator"};
					inventorySlotsCoefs[] = {0.3,0.3};
				};
				class Back
				{
					class Health
					{
						hitpoints = 100;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_back"};
					memoryPoints[] = {"dmgZone_back","dmgZone_bumper_1"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 100;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_windowFront"};
					memoryPoints[] = {"dmgZone_windowFront"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowBack: WindowFront
				{
					componentNames[] = {"dmgZone_windowBack"};
					memoryPoints[] = {"dmgZone_windowBack"};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\sedan_02\data\glass_i_destruct.rvmat"}}};
					};
					componentNames[] = {"dmgZone_lights_1_1"};
					memoryPoints[] = {"dmgZone_lights_1_1"};
					fatalInjuryCoef = -1;
					transferToZonesNames[] = {"Front","Fender_1_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Reflector_2_1: Reflector_1_1
				{
					componentNames[] = {"dmgZone_lights_2_1"};
					memoryPoints[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlots[] = {"Reflector_2_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Roof
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_roof"};
					memoryPoints[] = {"dmgZone_roof"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					componentNames[] = {"dmgZone_fender_1_1"};
					memoryPoints[] = {"dmgZone_fender_1_1"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Fender_2_1: Fender_1_1
				{
					componentNames[] = {"dmgZone_fender_2_1"};
					memoryPoints[] = {"dmgZone_fender_2_1"};
				};
				class Fender_1_2: Fender_1_1
				{
					componentNames[] = {"dmgZone_fender_1_2"};
					memoryPoints[] = {"dmgZone_fender_1_2"};
				};
				class Fender_2_2: Fender_1_1
				{
					componentNames[] = {"dmgZone_fender_2_2"};
					memoryPoints[] = {"dmgZone_fender_2_2"};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 1;
					};
					componentNames[] = {"dmgZone_engine"};
					memoryPoints[] = {"dmgZone_engine"};
					fatalInjuryCoef = 0.001;
					inventorySlots[] = {};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = -1;
					};
					componentNames[] = {"dmgZone_fuelTank"};
					memoryPoints[] = {"dmgZone_fuelTank"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Sedan_02_Door_1_1: CarDoor
	{
		applySkinsTo[] = {"Sedan_02_Door_2_1","Expansion_Sedan_02_Door_2_1"};
		defaultSkin = "Yellow";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Sedan_02_Door_2_1: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Sedan_02_Door_1_2: CarDoor
	{
		applySkinsTo[] = {"Sedan_02_Door_2_2","Expansion_Sedan_02_Door_2_2"};
		defaultSkin = "Yellow";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Sedan_02_Door_2_2: CarDoor
	{
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class Sedan_02_Hood: CarDoor
	{
		defaultSkin = "Yellow";
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
		};
	};
	class Sedan_02_Trunk: CarDoor
	{
		defaultSkin = "Yellow";
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
		};
	};
	class Expansion_Sedan_02_Door_1_1: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_1_1";
	};
	class Expansion_Sedan_02_Door_1_2: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_1_2";
	};
	class Expansion_Sedan_02_Door_2_1: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_2_1";
	};
	class Expansion_Sedan_02_Door_2_2: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_2_2";
	};
	class Expansion_Sedan_02_Hood: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Hood";
	};
	class Expansion_Sedan_02_Trunk: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Trunk";
	};
	class Sedan_02_Red: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "Red";
	};
	class Sedan_02_Door_1_1_Red: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "Red";
	};
	class Sedan_02_Door_1_2_Red: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "Red";
	};
	class Sedan_02_Door_2_1_Red: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "Red";
	};
	class Sedan_02_Door_2_2_Red: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "Red";
	};
	class Sedan_02_Hood_Red: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "Red";
	};
	class Sedan_02_Trunk_Red: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "Red";
	};
	class Sedan_02_Yellow: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "Yellow";
	};
	class Sedan_02_Door_1_1_Yellow: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "Yellow";
	};
	class Sedan_02_Door_1_2_Yellow: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "Yellow";
	};
	class Sedan_02_Door_2_1_Yellow: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "Yellow";
	};
	class Sedan_02_Door_2_2_Yellow: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "Yellow";
	};
	class Sedan_02_Hood_Yellow: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "Yellow";
	};
	class Sedan_02_Trunk_Yellow: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "Yellow";
	};
	class Sedan_02_Grey: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "Grey";
	};
	class Sedan_02_Door_1_1_Grey: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "Grey";
	};
	class Sedan_02_Door_1_2_Grey: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "Grey";
	};
	class Sedan_02_Door_2_1_Grey: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "Grey";
	};
	class Sedan_02_Door_2_2_Grey: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "Grey";
	};
	class Sedan_02_Hood_Grey: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "Grey";
	};
	class Sedan_02_Trunk_Grey: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "Grey";
	};
	class Sedan_02_Door_1_1_RedRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "RedRust";
	};
	class Sedan_02_Door_1_2_RedRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "RedRust";
	};
	class Sedan_02_Door_2_1_RedRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "RedRust";
	};
	class Sedan_02_Door_2_2_RedRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "RedRust";
	};
	class Sedan_02_Hood_RedRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "RedRust";
	};
	class Sedan_02_Trunk_RedRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "RedRust";
	};
	class Sedan_02_Door_1_1_YellowRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_1_2_YellowRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_2_1_YellowRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_2_2_YellowRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "YellowRust";
	};
	class Sedan_02_Hood_YellowRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "YellowRust";
	};
	class Sedan_02_Trunk_YellowRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_1_1_GreyRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "GreyRust";
	};
	class Sedan_02_Door_1_2_GreyRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "GreyRust";
	};
	class Sedan_02_Door_2_1_GreyRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "GreyRust";
	};
	class Sedan_02_Door_2_2_GreyRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "GreyRust";
	};
	class Sedan_02_Hood_GreyRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "GreyRust";
	};
	class Sedan_02_Trunk_GreyRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "GreyRust";
	};
	class Truck_01_Chassis: CarScript
	{
		doors[] = {"Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood"};
	};
	class Truck_02: CarScript
	{
		doors[] = {"Truck_02_Door_1_1","Truck_02_Door_2_1"};
	};
	class Van_01: CarScript
	{
		doors[] = {"Van_01_Door_1_1","Van_01_Door_2_1","Van_01_Door_2_2","Van_01_Trunk_1","Van_01_Trunk_2"};
	};
};
class CfgSurfaces
{
	class DZ_SurfacesInt;
	class DZ_SurfacesExt;
	class expansion_rubber: DZ_SurfacesExt
	{
		Density = 300;
		Thickness = 5;
		deflection = 0.2;
		bulletPenetrability = 600;
		bulletPenetrabilityWithThickness = 200;
		transparency = 0;
		rough = 0.1;
		dust = 0;
		audibility = 0.5;
		soundEnviron = "Empty";
		isLiquid = "false";
		friction = 0.0;
		restitution = 0.0;
		impact = "Hit_Rubber";
		soundHit = "rubber";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyVehiclePart: ProxyAttachment
	{
		scope = 2;
		simulation = "ProxyInventory";
		autocenter = 0;
		animated = 0;
		shadow = 1;
		reversed = 0;
	};
	class ProxySedanWheel: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\civiliansedan\proxy\sedanWheel.p3d";
		inventorySlot[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2","Expansion_CivSedanWheel_1_1","Expansion_CivSedanWheel_1_2","Expansion_CivSedanWheel_2_1","Expansion_CivSedanWheel_2_2"};
	};
	class ProxySedanWheel_destroyed: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\civiliansedan\proxy\sedanWheel_destroyed.p3d";
		inventorySlot[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2","Expansion_CivSedanWheel_1_1","Expansion_CivSedanWheel_1_2","Expansion_CivSedanWheel_2_1","Expansion_CivSedanWheel_2_2"};
	};
};
