#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Vehicles
	{
		units[] = {"ExpansionCarKey","ExpansionCarAdminKey","GlowPlug","ExpansionIgniterPlug","ExpansionHydraulicHoses"};
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
