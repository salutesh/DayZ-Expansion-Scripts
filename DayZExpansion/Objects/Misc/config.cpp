#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Misc
	{
		units[] = {"ExpansionBoltCutters","ExpansionPropaneTorch","ExpansionToolBox"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Objects_Structures_Props"};
	};
};
class CfgVehicles
{
	class Container_Base;
	class Inventory_Base;
	class Pliers;
	class ExpansionBoltCutters: Pliers
	{
		scope = 2;
		displayName = "$STR_EXPANSION_ITEM_BOLTCUTTERS";
		descriptionShort = "$STR_EXPANSION_ITEM_BOLTCUTTERS_DESC";
		model = "DayZExpansion\Objects\Misc\BoltCutters.p3d";
		weight = 4000;
		itemSize[] = {2,5};
	};
	class ExpansionPropaneTorch: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_ITEM_PROPANETORCH";
		descriptionShort = "$STR_EXPANSION_ITEM_PROPANETORCH_DESC";
		model = "DayZExpansion\Objects\Misc\PropaneTorch.p3d";
		weight = 5000;
		itemBehaviour = 2;
		itemSize[] = {2,5};
		rotationFlags = 17;
		stackedUnit = "percentage";
		quantityBar = 1;
		varQuantityInit = 100;
		varQuantityMin = 0;
		varQuantityMax = 100;
		varQuantityDestroyOnMin = 0;
		animClass = "Knife";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
		class EnergyManager
		{
			hasIcon = 1;
			switchOnAtSpawn = 0;
			autoSwitchOff = 1;
			autoSwitchOffWhenInCargo = 1;
			convertEnergyToQuantity = 1;
			energyAtSpawn = 5000;
			energyStorageMax = 5000;
			energyUsagePerSecond = 2.0;
			reduceMaxEnergyByDamageCoef = 0.5;
			updateInterval = 3;
		};
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeLightBlunt";
				range = 1.0;
			};
			class Heavy
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 1.0;
			};
			class Sprint
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 2.8;
			};
		};
	};
	class ExpansionToolBox: Container_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_ITEM_TOOLBOX";
		descriptionShort = "$STR_EXPANSION_ITEM_TOOLBOX_DESC";
		model = "DayZExpansion\Objects\Misc\ToolBox.p3d";
		rotationFlags = 2;
		weight = 10000;
		heavyitem = 1;
		itemBehaviour = 2;
		itemSize[] = {5,3};
		physLayer = "item_large";
		carveNavmesh = 1;
		canBeDigged = 1;
		class Cargo
		{
			itemsCargoSize[] = {10,3};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUp_Light
				{
					soundSet = "pickUpCanisterGasolineLight_SoundSet";
					id = 796;
				};
				class pickUp
				{
					soundSet = "pickUpCanisterGasoline_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "carradiator_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
};
