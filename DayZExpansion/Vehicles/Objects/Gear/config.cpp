#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Objects_Gear
	{
		units[] = {"ExpansionStaticKeyGrinder","ExpansionCarKey","ExpansionCarAdminKey","GlowPlug","ExpansionIgniterPlug","ExpansionHydraulicHoses","ExpansionHelicopterBattery","ExpansionAircraftBattery"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts","DayZExpansion_Objects_Gear_Spraycans"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
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
					healthLevels[] = {{1,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}},{0,{"DayZExpansion\Vehicles\Objects\Gear\data\key_01.rvmat"}}};
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
};
class CfgSlots
{
	class Slot_GlowPlug
	{
		displayName = "$STR_EXPANSION_GLOWPLUG";
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
};
