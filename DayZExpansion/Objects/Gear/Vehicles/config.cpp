#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Vehicles
	{
		units[] = {"ExpansionCarKey","ExpansionCarAdminKey","GlowPlug","ExpansionIgniterPlug","ExpansionHydraulicHoses","ExpansionUniversalWheel"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionCarKey: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_CAR_KEYS";
		descriptionShort = "$STR_EXPANSION_CAR_KEYS_DESC";
		model = "DayZExpansion\Objects\Gear\Vehicles\vehicle_key_01.p3d";
		animClass = "Knife";
		rotationFlags = 17;
		weight = 4;
		itemSize[] = {1,1};
		fragility = 0.01;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"DayZExpansion\Objects\Gear\Vehicles\data\key_01.rvmat"}},{0.7,{"DayZExpansion\Objects\Gear\Vehicles\data\key_01.rvmat"}},{0.5,{"DayZExpansion\Objects\Gear\Vehicles\data\key_01.rvmat"}},{0.3,{"DayZExpansion\Objects\Gear\Vehicles\data\key_01.rvmat"}},{0,{"DayZExpansion\Objects\Gear\Vehicles\data\key_01.rvmat"}}};
				};
			};
		};
	};
	class ExpansionCarAdminKey: ExpansionCarKey
	{
		displayName = "Admin Key";
	};
	class GlowPlug: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_GlowPlug0";
		descriptionShort = "$STR_CfgVehicles_GlowPlug1";
		model = "\dz\vehicles\parts\GlowPlug.p3d";
		weight = 200;
		itemSize[] = {1,1};
		absorbency = 0;
		inventorySlot = "GlowPlug";
		rotationFlags = 12;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] = {{1,{"DZ\vehicles\parts\data\glowplug.rvmat"}},{0.7,{"DZ\vehicles\parts\data\glowplug.rvmat"}},{0.5,{"DZ\vehicles\parts\data\glowplug_damage.rvmat"}},{0.3,{"DZ\vehicles\parts\data\glowplug_damage.rvmat"}},{0,{"DZ\vehicles\parts\data\glowplug_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionIgniterPlug: GlowPlug
	{
		scope = 2;
		displayName = "$STR_EXPANSION_IGNITERPLUG";
		descriptionShort = "$STR_EXPANSION_IGNITERPLUG_DESC";
		model = "\dz\vehicles\parts\GlowPlug.p3d";
		weight = 200;
		itemSize[] = {1,1};
		absorbency = 0;
		inventorySlot = "ExpansionIgniterPlug";
		rotationFlags = 12;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] = {{1.0,{"DZ\vehicles\parts\data\glowplug.rvmat"}},{0.7,{"DZ\vehicles\parts\data\glowplug.rvmat"}},{0.5,{"DZ\vehicles\parts\data\glowplug_damage.rvmat"}},{0.3,{"DZ\vehicles\parts\data\glowplug_damage.rvmat"}},{0.0,{"DZ\vehicles\parts\data\glowplug_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionHydraulicHoses: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_HYDROHOSE";
		descriptionShort = "$STR_EXPANSION_HYDROHOSE_DESC";
		model = "DayZExpansion\Objects\Gear\Vehicles\HydraulicHoses.p3d";
		weight = 200;
		itemSize[] = {2,1};
		absorbency = 0;
		inventorySlot = "ExpansionHydraulicHoses";
		rotationFlags = 17;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 80;
					healthLevels[] = {{1.0,{"DZ\vehicles\parts\data\hydraulic_hoses.rvmat"}},{0.7,{"DZ\vehicles\parts\data\hydraulic_hoses.rvmat"}},{0.5,{"DZ\vehicles\parts\data\hydraulic_hoses_damage.rvmat"}},{0.3,{"DZ\vehicles\parts\data\hydraulic_hoses_damage.rvmat"}},{0.0,{"DZ\vehicles\parts\data\hydraulic_hoses_destruct.rvmat"}}};
				};
			};
		};
	};
	class CarWheel;
	class ExpansionUniversalWheel: CarWheel
	{
		scope = 2;
		displayName = "Car Wheel";
		descriptionShort = "$STR_CivSedanWheel1";
		model = "\DZ\vehicles\wheeled\civiliansedan\proxy\sedanwheel.p3d";
		inventorySlot[] = {"NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","NivaWheel_Spare_1","Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2","CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2","Hatchback_02_Wheel_1_1","Hatchback_02_Wheel_1_2","Hatchback_02_Wheel_2_1","Hatchback_02_Wheel_2_2","BusWheel_1_1","BusWheel_2_1","BusWheel_1_2","BusWheel_2_2","ExpansionTractorFrontWheel_1_1","ExpansionTractorFrontWheel_2_1","ExpansionTractorBackWheel_1_2","ExpansionTractorBackWheel_2_2","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
		rotationFlags = 4;
		radiusByDamage[] = {0,0.34,0.3,0.3,0.9998,0.25,0.9999,0.2};
		radius = 0.34;
		friction = 0.96;
		width = 0.16;
		tyreRollResistance = 0.015;
		tyreTread = 1;
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
};
class CfgSlots
{
	class Slot_ExpansionIgniterPlug
	{
		name = "ExpansionIgniterPlug";
		displayName = "ExpansionIgniterPlug";
		ghostIcon = "glowplug";
	};
	class Slot_ExpansionHydraulicHoses
	{
		name = "ExpansionHydraulicHoses";
		displayName = "ExpansionHydraulicHoses";
		ghostIcon = "exhaust";
	};
};
