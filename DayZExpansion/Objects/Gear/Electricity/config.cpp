#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Electricity
	{
		units[] = {"ExpansionGenerator"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionGenerator: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MOBILE_GENERATOR";
		descriptionShort = "$STR_EXPANSION_MOBILE_GENERATOR_DESC";
		model = "\DayZExpansion\Objects\Gear\Electricity\NewGenerator.p3d";
		useEntityHierarchy = "true";
		forceFarBubble = "true";
		carveNavmesh = 1;
		bounding = "BSphere";
		physLayer = "item_large";
		allowOwnedCargoManipulation = 1;
		inputRange = 24;
		weight = 45000;
		itemSize[] = {10,10};
		itemBehaviour = 0;
		attachments[] = {"SparkPlug"};
		fuelTankCapacity = 7000;
		heavyItem = 1;
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 1;
			energyStorageMax = 10000;
			energyUsagePerSecond = 0.28;
			reduceMaxEnergyByDamageCoef = 0.5;
			energyAtSpawn = 5000;
			powerSocketsCount = 4;
			compatiblePlugTypes[] = {2,"PLUG_PAS_TERMINAL",6};
		};
	};
};
