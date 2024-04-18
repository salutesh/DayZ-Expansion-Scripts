#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Objects_Gear
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Gear_Camping","DZ_Scripts","DayZExpansion_Objects_Gear_Spraycans"};
	};
};
class CfgVehicles
{
	class CamoNet;
	class Inventory_Base;
	class Container_Base;
	class BatteryCharger: Inventory_Base
	{
		attachments[] += {"ExpansionHelicopterBattery","ExpansionAircraftBattery"};
	};
	class ExpansionStaticKeyGrinder: Inventory_Base
	{
		scope = 2;
		displayName = "Key Grinder";
		descriptionShort = "A static Key Grinder";
		model = "DayZExpansion\Objects\Structures\Props\jigsaw.p3d";
		weight = 5000;
		itemSize[] = {4,4};
		canBeSplit = 0;
		rotationFlags = 256;
		physLayer = "item_large";
		useEntityHierarchy = "true";
		class AnimationSources
		{
			class showWire
			{
				source = "user";
				animPeriod = 1;
				initPhase = 1;
			};
		};
	};
	class ExpansionCarKey: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_CAR_KEYS";
		descriptionShort = "$STR_EXPANSION_CAR_KEYS_DESC";
		model = "DayZExpansion\Vehicles\Objects\Gear\vehicle_key_01.p3d";
		animClass = "Knife";
		rotationFlags = 16;
		weight = 4;
		itemSize[] = {1,2};
		fragility = 0.01;
		attachments[] = {"KeyChain"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}}};
				};
			};
		};
	};
	class ExpansionCarAdminKey: ExpansionCarKey
	{
		displayName = "Admin Key";
	};
	class ExpansionKeyChainBase: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_KEYCHAIN";
		descriptionShort = "$STR_EXPANSION_KEYCHAIN_DESC";
		model = "DayZExpansion\Vehicles\Objects\Gear\keychain\expansion_keychain.p3d";
		rotationFlags = 16;
		weight = 4;
		itemSize[] = {1,1};
		inventorySlot[] = {"KeyChain"};
		fragility = 0.01;
		hiddenSelections[] = {"body","label","ring"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_red_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_worn.rvmat","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn.rvmat","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring.rvmat"};
	};
	class ExpansionKeyChain_Red: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_red_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_White: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_white_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Black: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_black_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Blue: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_blue_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Green: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_green_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Grey: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_grey_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Orange: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_orange_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Pink: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_pink_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Purple: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_purple_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class ExpansionKeyChain_Yellow: ExpansionKeyChainBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\Gear\keychain\data\body_yellow_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\label_worn_co.paa","DayZExpansion\Vehicles\Objects\Gear\keychain\data\ring_co.paa"};
	};
	class GlowPlug: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_GLOWPLUG";
		descriptionShort = "$STR_EXPANSION_GLOWPLUG_DESC";
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
		model = "\DZ\vehicles\parts\igniter_plug.p3d";
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
		model = "DayZExpansion\Vehicles\Objects\Gear\HydraulicHoses.p3d";
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
	class ExpansionHelicopterBattery: Inventory_Base
	{
		scope = 2;
		vehicleClass = "Expansion_Helicopter";
		displayName = "$STR_EXPANSION_HELICOPTER_BATTERY";
		descriptionShort = "$STR_EXPANSION_HELICOPTER_BATTERY_DESC";
		model = "\dz\vehicles\parts\helicopter_battery.p3d";
		attachments[] = {"MetalWire"};
		weight = 10000;
		absorbency = 0.1;
		itemSize[] = {4,4};
		stackedUnit = "w";
		quantityBar = 1;
		varQuantityInit = 50;
		varQuantityMin = 0;
		varQuantityMax = 50;
		varQuantityDestroyOnMin = 0;
		repairableWithKits[] = {5,7};
		repairCosts[] = {30,25};
		inventorySlot = "ExpansionHelicopterBattery";
		rotationFlags = 2;
		tooltipResources[] = {"power"};
		class Resources
		{
			power = 77000.0;
		};
		class EnergyManager
		{
			hasIcon = 1;
			switchOnAtSpawn = 1;
			isPassiveDevice = 1;
			energyStorageMax = 500;
			energyAtSpawn = 500;
			reduceMaxEnergyByDamageCoef = 0.5;
			powerSocketsCount = 1;
			plugType = 4;
			compatiblePlugTypes[] = {5,8,6};
			attachmentAction = 2;
			convertEnergyToQuantity = 1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\vehicles\parts\data\battery_smallcar.rvmat"}},{0.7,{"DZ\vehicles\parts\data\battery_smallcar.rvmat"}},{0.5,{"DZ\vehicles\parts\data\battery_smallcar_damage.rvmat"}},{0.3,{"DZ\vehicles\parts\data\battery_smallcar_damage.rvmat"}},{0.0,{"DZ\vehicles\parts\data\battery_smallcar_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionAircraftBattery: Inventory_Base
	{
		scope = 2;
		vehicleClass = "Expansion_Plane";
		displayName = "$STR_EXPANSION_AIRCRAFT_BATTERY";
		descriptionShort = "$STR_EXPANSION_HELICOPTER_BATTERY_DESC";
		model = "\dz\vehicles\parts\aircraft_battery.p3d";
		attachments[] = {"MetalWire"};
		weight = 10000;
		repairableWithKits[] = {5,7};
		repairCosts[] = {30.0,25.0};
		itemSize[] = {4,4};
		itemBehaviour = 0;
		heavyItem = 1;
		absorbency = 0;
		inventorySlot = "ExpansionAircraftBattery";
		rotationFlags = 2;
		tooltipResources[] = {"power"};
		class Resources
		{
			power = 77000.0;
		};
		class EnergyManager
		{
			hasIcon = 1;
			switchOnAtSpawn = 1;
			isPassiveDevice = 1;
			energyStorageMax = 500;
			energyAtSpawn = 500;
			reduceMaxEnergyByDamageCoef = 0.5;
			powerSocketsCount = 1;
			plugType = 4;
			compatiblePlugTypes[] = {5,8,6};
			attachmentAction = 2;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\vehicles\parts\data\battery_smallcar.rvmat"}},{0.7,{"DZ\vehicles\parts\data\battery_smallcar.rvmat"}},{0.5,{"DZ\vehicles\parts\data\battery_smallcar_damage.rvmat"}},{0.3,{"DZ\vehicles\parts\data\battery_smallcar_damage.rvmat"}},{0.0,{"DZ\vehicles\parts\data\battery_smallcar_destruct.rvmat"}}};
				};
			};
		};
	};
	class CarWheel;
	class ExpansionSpraycanGreen;
	class ExpansionSpraycanBlueline;
	class ExpansionSpraycanDigital;
	class ExpansionSpraycanElliptical;
	class ExpansionSpraycanFurious;
	class ExpansionSpraycanGrayWatcher;
	class ExpansionSpraycanJeans;
	class ExpansionSpraycanResistance;
	class ExpansionSpraycanShadow;
	class ExpansionSpraycanSheriff;
	class ExpansionSpraycanSpeedy;
	class ExpansionSpraycanSunset;
	class ExpansionSpraycanVrana;
	class ExpansionSpraycanWasp;
	class ExpansionSpraycanWave;
	class ExpansionSpraycanCivilian;
	class ExpansionSpraycanMedical;
	class ExpansionSpraycanUber;
	class ExpansionSpraycanDahoman;
	class ExpansionSpraycanIon;
	class ExpansionSpraycanPresident;
	class ExpansionSpraycanBlue;
	class ExpansionSpraycanWeeb;
	class ExpansionSpraycanMh6Default: ExpansionSpraycanGreen{};
	class ExpansionSpraycanMh6Blueline: ExpansionSpraycanBlueline{};
	class ExpansionSpraycanMh6Digital: ExpansionSpraycanDigital{};
	class ExpansionSpraycanMh6Elliptical: ExpansionSpraycanElliptical{};
	class ExpansionSpraycanMh6Furious: ExpansionSpraycanFurious{};
	class ExpansionSpraycanMh6GrayWatcher: ExpansionSpraycanGrayWatcher{};
	class ExpansionSpraycanMh6Jeans: ExpansionSpraycanJeans{};
	class ExpansionSpraycanMh6Resistance: ExpansionSpraycanResistance{};
	class ExpansionSpraycanMh6Shadow: ExpansionSpraycanShadow{};
	class ExpansionSpraycanMh6Sheriff: ExpansionSpraycanSheriff{};
	class ExpansionSpraycanMh6Speedy: ExpansionSpraycanSpeedy{};
	class ExpansionSpraycanMh6Sunset: ExpansionSpraycanSunset{};
	class ExpansionSpraycanMh6Vrana: ExpansionSpraycanVrana{};
	class ExpansionSpraycanMh6Wasp: ExpansionSpraycanWasp{};
	class ExpansionSpraycanMh6Wave: ExpansionSpraycanWave{};
	class ExpansionSpraycanUh1hDefault: ExpansionSpraycanGreen{};
	class ExpansionSpraycanUh1hCivilian: ExpansionSpraycanCivilian{};
	class ExpansionSpraycanUh1hMedical: ExpansionSpraycanMedical{};
	class ExpansionSpraycanUh1hUber: ExpansionSpraycanUber{};
	class ExpansionSpraycanUh1hResistance: ExpansionSpraycanResistance{};
	class ExpansionSpraycanMerlinDefault: ExpansionSpraycanGreen{};
	class ExpansionSpraycanMerlinDahoman: ExpansionSpraycanDahoman{};
	class ExpansionSpraycanMerlinIon: ExpansionSpraycanIon{};
	class ExpansionSpraycanMerlinPresident: ExpansionSpraycanPresident{};
	class ExpansionSpraycanMerlinVrana: ExpansionSpraycanVrana{};
	class ExpansionSpraycanMerlinWave: ExpansionSpraycanWave{};
	class ExpansionSpraycanBusDefault: ExpansionSpraycanBlue{};
	class ExpansionSpraycanBusWeeb: ExpansionSpraycanWeeb{};
	class CamoNetCivil: CamoNet{};
	class CamoNetDesert: CamoNet{};
	class CamoNetWinter: CamoNet{};
	class CarCoverBase: Container_Base
	{
		scope = 0;
		itemBehaviour = 0;
		itemSize[] = {10,100};
		carveNavmesh = 1;
		weight = 1000000;
		physLayer = "item_large";
		allowOwnedCargoManipulation = 1;
		forceFarBubble = "true";
		rotationFlags = 2;
		itemsCargoSize[] = {10,100};
		attachments[] = {"CamoNet"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
	};
	class ExpansionVehicleCover: Inventory_Base
	{
		scope = 0;
		displayName = "$STR_EXPANSION_VEHICLE_COVER";
		itemBehaviour = 0;
		itemSize[] = {10,100};
		carveNavmesh = 1;
		weight = 1000000;
		physLayer = "item_large";
		allowOwnedCargoManipulation = 1;
		forceFarBubble = "true";
		rotationFlags = 2;
		itemsCargoSize[] = {10,100};
		attachments[] = {"CamoNet","KeyChain"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
	};
	class Expansion_Generic_Vehicle_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\Sedan_02_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionBus_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\bus_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class Expansion_Landrover_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\landrover_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionUAZ_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\uaz_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionUAZCargo_Cover: ExpansionUAZ_Cover{};
	class ExpansionUAZRoofless_Cover: ExpansionUAZ_Cover{};
	class ExpansionUAZCargoRoofless_Cover: ExpansionUAZ_Cover{};
	class ExpansionMh6_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\mh6_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionUh1h_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\uh1h_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionMerlin_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\merlin_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionGyrocopter_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\gyrocopter_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionBigGyrocopter_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\gyrocopter_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionLHD_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\LHD_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionUtilityBoat_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\RHIB_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionZodiacBoat_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\zodiac_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionTractor_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\tractor_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionVodnik_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\vodnik_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class OffroadHatchback_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\OffroadHatchback_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class CivilianSedan_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\CivilianSedan_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class Hatchback_02_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\Hatchback_02_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class Sedan_02_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\Sedan_02_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class Truck_01_Covered_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\Truck_01_Covered_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class Offroad_02_Cover: ExpansionVehicleCover
	{
		scope = 2;
		model = "dayzexpansion\vehicles\objects\gear\vehicle_covers\Offroad_02_cover.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\objects\gear\vehicle_covers\data\camonet_east.rvmat"};
	};
	class ExpansionHatchBird_Cover: OffroadHatchback_Cover{};
};
class CfgSlots
{
	class Slot_GlowPlug
	{
		displayName = "$STR_GlowPlug0";
	};
	class Slot_ExpansionIgniterPlug
	{
		name = "ExpansionIgniterPlug";
		displayName = "$STR_EXPANSION_IGNITERPLUG";
		ghostIcon = "glowplug";
	};
	class Slot_ExpansionHydraulicHoses
	{
		name = "ExpansionHydraulicHoses";
		displayName = "$STR_EXPANSION_HYDROHOSE";
		ghostIcon = "exhaust";
	};
	class Slot_ExpansionHelicopterBattery
	{
		name = "ExpansionHelicopterBattery";
		displayName = "$STR_EXPANSION_HELICOPTER_BATTERY";
		selection = "battery";
		ghostIcon = "carbattery";
	};
	class Slot_ExpansionAircraftBattery
	{
		name = "ExpansionAircraftBattery";
		displayName = "$STR_EXPANSION_AIRCRAFT_BATTERY";
		selection = "battery";
		ghostIcon = "carbattery";
	};
	class Slot_KeyChain
	{
		name = "KeyChain";
		displayName = "$STR_EXPANSION_KEYCHAIN";
		ghostIcon = "missing";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxyexpansion_keychain: ProxyAttachment
	{
		scope = 2;
		inventorySlot[] = {"KeyChain"};
		model = "DayZExpansion\Vehicles\Objects\Gear\keychain\expansion_keychain.p3d";
	};
};
