#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Hatchback
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class CarDoor;
	class CarScript;
	class ExpansionCarDoorFix;
	class Crew;
	class Driver;
	class CoDriver;
	class SimulationModule;
	class Axles;
	class Front;
	class Rear;
	class Wheels;
	class Left;
	class Right;
	class AnimationSources;
	class OffroadHatchback: CarScript
	{
		hiddenSelections[] = {"light_left","light_right","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\offroadhatchback\data\niva_dashboard_light.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\offroadhatchback\data\niva_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\niva_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\OffroadHatchback_lights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\OffroadHatchback_lights.rvmat";
		fuelCapacity = 42;
		fuelConsumption = 11;
		doors[] = {"NivaDriverDoors","NivaCoDriverDoors","NivaTrunk","NivaHood"};
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
	class HatchbackDoors_Driver: CarDoor
	{
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
	class HatchbackDoors_CoDriver: HatchbackDoors_Driver
	{
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
	class HatchbackHood: CarDoor
	{
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
		};
	};
	class HatchbackTrunk: CarDoor
	{
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
	class OffroadHatchback_Police: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "Police";
	};
	class HatchbackDoors_Driver_Police: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "Police";
	};
	class HatchbackDoors_CoDriver_Police: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "Police";
	};
	class HatchbackHood_Police: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "Police";
	};
	class HatchbackTrunk_Police: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "Police";
	};
	class OffroadHatchback_GreenRust: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "GreenRust";
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
	class OffroadHatchback_BlueRust: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "BlueRust";
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
	class OffroadHatchback_WhiteRust: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "WhiteRust";
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
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyNivaWheel: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaWheel.p3d";
		inventorySlot[] = {"NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","NivaWheel_Spare_1"};
	};
	class ProxyNivaWheel_destroyed: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaWheel_destroyed.p3d";
		inventorySlot[] = {"NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","NivaWheel_Spare_1"};
	};
};
