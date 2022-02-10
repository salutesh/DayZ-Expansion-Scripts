#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_CivilianSedan
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
	class CivilianSedan: CarScript
	{
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","DZ\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat","DZ\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"};
		doors[] = {"CivSedanDriverDoors","CivSedanCoDriverDoors","CivSedanCargo1Doors","CivSedanCargo2Doors","CivSedanTrunk","CivSedanHood"};
		applySkinsTo[] = {"CivSedanDoors_Driver","CivSedanDoors_CoDriver","CivSedanDoors_BackLeft","CivSedanDoors_BackRight","CivSedanHood","CivSedanTrunk","Expansion_CivSedanDoors_Driver","Expansion_CivSedanDoors_CoDriver","Expansion_CivSedanDoors_BackLeft","Expansion_CivSedanDoors_BackRight","Expansion_CivSedanHood","Expansion_CivSedanTrunk"};
		defaultSkin = "White";
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
	class CivSedanDoors_CoDriver: CivSedanDoors_Driver
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
	class CivSedanDoors_BackLeft: CivSedanDoors_Driver
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
	class CivSedanDoors_BackRight: CivSedanDoors_Driver
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
			class DamageZones
			{
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
			class DamageZones
			{
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
	class CivilianSedan_Police: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Police";
	};
	class CivSedanDoors_Driver_Police: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Police";
	};
	class CivSedanDoors_CoDriver_Police: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Police";
	};
	class CivSedanDoors_BackLeft_Police: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Police";
	};
	class CivSedanDoors_BackRight_Police: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Police";
	};
	class CivSedanHood_Police: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Police";
	};
	class CivSedanTrunk_Police: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Police";
	};
	class CivilianSedan_Lime: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Lime";
	};
	class CivSedanDoors_Driver_Lime: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Lime";
	};
	class CivSedanDoors_CoDriver_Lime: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Lime";
	};
	class CivSedanDoors_BackLeft_Lime: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Lime";
	};
	class CivSedanDoors_BackRight_Lime: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Lime";
	};
	class CivSedanHood_Lime: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Lime";
	};
	class CivSedanTrunk_Lime: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Lime";
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
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxySedanWheel: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\civiliansedan\proxy\sedanWheel.p3d";
		inventorySlot[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2"};
	};
	class ProxySedanWheel_destroyed: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\civiliansedan\proxy\sedanWheel_destroyed.p3d";
		inventorySlot[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2"};
	};
};
