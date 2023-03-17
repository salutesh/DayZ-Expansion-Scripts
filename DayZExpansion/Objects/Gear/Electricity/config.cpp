#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Electricity
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionGenerator_Base: Inventory_Base
	{
		scope = 0;
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
		soundImpactType = "metal";
		repairableWithKits[] = {7,10};
		repairCosts[] = {40,50};
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
	class ExpansionGenerator_Large: ExpansionGenerator_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MOBILE_GENERATOR";
		descriptionShort = "$STR_EXPANSION_MOBILE_GENERATOR_DESC";
		model = "\DayZExpansion\Objects\Gear\Electricity\NewGenerator.p3d";
		inputRange = 24;
		fuelTankCapacity = 7000;
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
		class AnimationSources
		{
			class socket_1_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class socket_2_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class socket_3_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class socket_4_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class dial_fuel
			{
				source = "user";
				animPeriod = 1;
				initPhase = 0;
			};
		};
	};
	class ExpansionGenerator_Small: ExpansionGenerator_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MOBILE_GENERATOR";
		descriptionShort = "$STR_EXPANSION_MOBILE_GENERATOR_DESC";
		model = "\DayZExpansion\Objects\Gear\Electricity\Generator.p3d";
		inputRange = 18;
		fuelTankCapacity = 5000;
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 1;
			energyStorageMax = 7000;
			energyUsagePerSecond = 0.28;
			reduceMaxEnergyByDamageCoef = 0.5;
			energyAtSpawn = 3000;
			powerSocketsCount = 4;
			compatiblePlugTypes[] = {2,"PLUG_PAS_TERMINAL",6};
		};
	};
};
