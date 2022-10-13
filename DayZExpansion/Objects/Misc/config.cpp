#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Misc
	{
		units[] = {"ExpansionBoltCutters","ExpansionPropaneTorch","ExpansionToolBox","ExpansionParkingMeter","ExpansionCircuitBoard_MK1","ExpansionCircuitBoard_MK2","ExpansionCircuitBoard_MK3"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgSlots
{
	class Slot_Att_ExpansionCircuitBoard
	{
		name = "Att_ExpansionCircuitBoard";
		displayName = "Circuit Board";
		selection = "att_circuit_board";
		ghostIcon = "set:expansion_inventory image:circuit_board";
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
		inventorySlot[] = {"WoodenCrate","woodencrate_1","woodencrate_2","woodencrate_3","woodencrate_4","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","Truck_01_WoodenCrate4","Truck_01_WoodenCrate5","Truck_01_WoodenCrate6"};
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
	class HouseNoDestruct;
	class ExpansionParkingMeter: Inventory_Base
	{
		scope = 2;
		displayName = "Parking Meter";
		descriptionShort = "Used to get access to the Virtual Garage.";
		model = "DayZExpansion\Objects\Misc\ParkingMeter.p3d";
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		slopeTolerance = 0.2;
		yawPitchRollLimit[] = {12,12,12};
		hiddenSelections[] = {"camo","light_green","light_red"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Misc\data\parking_meter_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Misc\data\parking_meter.rvmat"};
		openable = 1;
		carveNavmesh = 1;
		weight = 1000;
		itemSize[] = {4,4};
		itemsCargoSize[] = {};
		itemBehaviour = 2;
		lootCategory = "Crafted";
		placement = "ForceSlopeOnTerrain";
		physLayer = "item_large";
		repairableWithKits[] = {7,10};
		repairCosts[] = {25.0,30.0};
		attachments[] = {"CarBattery","Att_ExpansionCircuitBoard"};
		rotationFlags = 2;
		soundImpactType = "metal";
		oldpower = 0;
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 1;
			energyUsagePerSecond = 0.05;
			plugType = 5;
			attachmentAction = 1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DayZExpansion\Objects\Misc\data\parking_meter.rvmat"}},{0.7,{"DayZExpansion\Objects\Misc\data\parking_meter.rvmat"}},{0.5,{"DayZExpansion\Objects\Misc\data\parking_meter.rvmat"}},{0.3,{"DayZExpansion\Objects\Misc\data\parking_meter.rvmat"}},{0.0,{"DayZExpansion\Objects\Misc\data\parking_meter.rvmat"}}};
				};
			};
		};
		class AnimationSources
		{
			class camo
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class carbattery
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class battery_installed
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class placing
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				name = "$STR_attachment_accessories";
				description = "";
				attachmentSlots[] = {"CarBattery","Att_ExpansionCircuitBoard"};
				icon = "set:expansion_iconset image:icon_options";
			};
		};
	};
	class ExpansionCircuitBoardBase: Inventory_Base
	{
		scope = 1;
		displayName = "Circuit Board - Color Base";
		model = "DayZExpansion\Objects\Misc\CircuitBoard.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Misc\data\circuit_board_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"};
		weight = 180;
		itemSize[] = {2,2};
		absorbency = 0;
		inventorySlot[] = {"Att_ExpansionCircuitBoard"};
		physLayer = "item_small";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] = {{1.0,{"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"}},{0.7,{"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"}},{0.5,{"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"}},{0.3,{"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"}},{0.0,{"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"}}};
				};
			};
		};
	};
	class ExpansionCircuitBoard_MK1: ExpansionCircuitBoardBase
	{
		scope = 2;
		displayName = "Circuit Board - MK1";
		descriptionShort = "Used to modify various electronic devices. This is the first generation of a PCB from an unknown device manufacturer. Various variants of these devices were shipped to Chernarus from an undisclosed island between Siberia and Alaska for unknown purposes. The product description number is: NTL-PCB-MK-001";
		model = "DayZExpansion\Objects\Misc\CircuitBoard.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Misc\data\circuit_board_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"};
	};
	class ExpansionCircuitBoard_MK2: ExpansionCircuitBoardBase
	{
		scope = 2;
		displayName = "Circuit Board - MK2";
		descriptionShort = "Used to modify various electronic devices. This is the second generation of a PCB from an unknown device manufacturer. Various variants of these devices were shipped to Chernarus from an undisclosed island between Siberia and Alaska for unknown purposes. The product description number is: NTL-PCB-MK-002";
		model = "DayZExpansion\Objects\Misc\CircuitBoard.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Misc\data\circuit_board_mk2_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"};
	};
	class ExpansionCircuitBoard_MK3: ExpansionCircuitBoardBase
	{
		scope = 2;
		displayName = "Circuit Board - MK3";
		descriptionShort = "Used to modify various electronic devices. This is the second generation of a PCB from an unknown device manufacturer. Various variants of these devices were shipped to Chernarus from an undisclosed island between Siberia and Alaska for unknown purposes. The product description number is: NTL-PCB-MK-003";
		model = "DayZExpansion\Objects\Misc\CircuitBoard.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Misc\data\circuit_board_mk3_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Misc\data\circuit_board.rvmat"};
	};
};
