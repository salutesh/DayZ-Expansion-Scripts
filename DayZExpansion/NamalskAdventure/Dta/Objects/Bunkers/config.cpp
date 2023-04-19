#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Bunkers
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DayZExpansion_NamalskAdventure_Items","Domaged_Structures"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Bunker_Generator: HouseNoDestruct
	{
		scope = 2;
		displayName = "Electric Generator";
		descriptionShort = "";
		model = "ZStuff01\Compound Gate\Data\Generator.p3d";
		carveNavmesh = 1;
		allowOwnedCargoManipulation = 1;
		attachments[] = {"Att_ExpansionGeneratorKey"};
		weight = 1000000;
		physLayer = "item_large";
		inventoryCondition = "true";
		rotationFlags = 2;
		handheld = "false";
		storageCategory = 1;
		openable = 1;
		lootCategory = "Containers";
		vehicleClass = "Inventory";
		mapSize = 1;
		oldpower = 0;
		ChargeEnergyPerSecond = 1.0;
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 1;
			energyUsagePerSecond = 0;
			plugType = 5;
			attachmentAction = 1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10000;
					healthLevels[] = {};
				};
			};
		};
	};
};
