#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Sedan_02
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
	class Sedan_02: CarScript
	{
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","camo","camo_chassi","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"};
		doors[] = {"Sedan_02_Door_1_1","Sedan_02_Door_1_2","Sedan_02_Door_2_1","Sedan_02_Door_2_2","Sedan_02_Trunk","Sedan_02_Hood"};
		defaultSkin = "Yellow";
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
	class Sedan_02_Door_2_1: Sedan_02_Door_1_1
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
	class Sedan_02_Door_1_2: Sedan_02_Door_1_1
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
	class Sedan_02_Door_2_2: Sedan_02_Door_1_1
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
	class Sedan_02_YellowBright: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "YellowBright";
	};
	class Sedan_02_Door_1_1_YellowBright: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "YellowBright";
	};
	class Sedan_02_Door_1_2_YellowBright: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "YellowBright";
	};
	class Sedan_02_Door_2_1_YellowBright: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "YellowBright";
	};
	class Sedan_02_Door_2_2_YellowBright: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "YellowBright";
	};
	class Sedan_02_Hood_YellowBright: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "YellowBright";
	};
	class Sedan_02_Trunk_YellowBright: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "YellowBright";
	};
	class Sedan_02_Blue: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "Blue";
	};
	class Sedan_02_Door_1_1_Blue: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "Blue";
	};
	class Sedan_02_Door_1_2_Blue: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "Blue";
	};
	class Sedan_02_Door_2_1_Blue: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "Blue";
	};
	class Sedan_02_Door_2_2_Blue: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "Blue";
	};
	class Sedan_02_Hood_Blue: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "Blue";
	};
	class Sedan_02_Trunk_Blue: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "Blue";
	};
	class Sedan_02_RedRust: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "RedRust";
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
	class Sedan_02_YellowRust: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "YellowRust";
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
	class Sedan_02_GreyRust: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "GreyRust";
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
	class Sedan_02_YellowBrightRust: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "YellowBrightRust";
	};
	class Sedan_02_Door_1_1_YellowBrightRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "YellowBrightRust";
	};
	class Sedan_02_Door_1_2_YellowBrightRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "YellowBrightRust";
	};
	class Sedan_02_Door_2_1_YellowBrightRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "YellowBrightRust";
	};
	class Sedan_02_Door_2_2_YellowBrightRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "YellowBrightRust";
	};
	class Sedan_02_Hood_YellowBrightRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "YellowBrightRust";
	};
	class Sedan_02_Trunk_YellowBrightRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "YellowBrightRust";
	};
	class Sedan_02_BlueRust: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "BlueRust";
	};
	class Sedan_02_Door_1_1_BlueRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "BlueRust";
	};
	class Sedan_02_Door_1_2_BlueRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "BlueRust";
	};
	class Sedan_02_Door_2_1_BlueRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "BlueRust";
	};
	class Sedan_02_Door_2_2_BlueRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "BlueRust";
	};
	class Sedan_02_Hood_BlueRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "BlueRust";
	};
	class Sedan_02_Trunk_BlueRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "BlueRust";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxySedan_02_Wheel: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel.p3d";
		inventorySlot[] = {"Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2"};
	};
	class ProxySedan_02_Wheel_ruined: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel_ruined.p3d";
		inventorySlot[] = {"Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2"};
	};
};
