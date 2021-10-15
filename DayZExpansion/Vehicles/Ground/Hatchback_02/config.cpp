#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Hatchback_02
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
	class Hatchback_02: CarScript
	{
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","camo_chassi"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"};
		doors[] = {"Hatchback_02_Door_1_1","Hatchback_02_Door_1_2","Hatchback_02_Door_2_1","Hatchback_02_Door_2_2","Hatchback_02_Trunk","Hatchback_02_Hood"};
		defaultSkin = "Red";
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
	class Hatchback_02_Door_2_1: Hatchback_02_Door_1_1
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
	class Hatchback_02_Door_1_2: Hatchback_02_Door_1_1
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
	class Hatchback_02_Door_2_2: Hatchback_02_Door_1_2
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
	class Hatchback_02_RedRust: Hatchback_02
	{
		skinBase = "Hatchback_02";
		skinName = "RedRust";
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
	class Hatchback_02_BlackRust: Hatchback_02
	{
		skinBase = "Hatchback_02";
		skinName = "BlackRust";
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
	class Hatchback_02_BlueRust: Hatchback_02
	{
		skinBase = "Hatchback_02";
		skinName = "BlueRust";
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
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyHatchback_02_Wheel: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\Hatchback_02\proxy\Hatchback_02_Wheel.p3d";
		inventorySlot[] = {"Hatchback_02_Wheel_1_1","Hatchback_02_Wheel_1_2","Hatchback_02_Wheel_2_1","Hatchback_02_Wheel_2_2"};
	};
	class ProxyHatchback_02_Wheel_ruined: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\Hatchback_02\proxy\Hatchback_02_Wheel_ruined.p3d";
		inventorySlot[] = {"Hatchback_02_Wheel_1_1","Hatchback_02_Wheel_1_2","Hatchback_02_Wheel_2_1","Hatchback_02_Wheel_2_2"};
	};
};
