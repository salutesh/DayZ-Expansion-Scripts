#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Tents
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionCamoBox","ExpansionCamoTent"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class ExpansionBaseBuilding;
	class ExpansionCamoBox: ExpansionBaseBuilding
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		displayName = "$STR_EXPANSION_CAMO_BOX";
		descriptionShort = "$STR_EXPANSION_CAMO_BOX_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Tents\Camo_Box2.p3d";
		allowOwnedCargoManipulation = 1;
		class Cargo
		{
			itemsCargoSize[] = {10,10};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					Health = 0;
					Blood = 0;
					Shock = 0;
				};
			};
		};
	};
	class ExpansionCamoTent: ExpansionBaseBuilding
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		displayName = "$STR_EXPANSION_CAMO_TENT";
		descriptionShort = "$STR_EXPANSION_CAMO_TENT_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Tents\ATent.p3d";
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		lootCategory = "Tents";
		placement = "ForceSlopeOnTerrain";
		physLayer = "item_large";
		repairableWithKits[] = {5,2};
		repairCosts[] = {30.0,25.0};
		openable = 0;
		carveNavmesh = 1;
		weight = 10000;
		class GUIInventoryAttachmentsProps{};
		itemsCargoSize[] = {10,15};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					Health = 0;
					Blood = 0;
					Shock = 0;
				};
			};
		};
	};
};
