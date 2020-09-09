#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_250N
	{
		units[] = {"Expansion_250N_Wheel_Front","Expansion_250N_Wheel_Back","Expansion250N"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DZ_Vehicles_Parts","DZ_Sounds_Effects"};
	};
};
class CfgSlots
{
	class Slot_Expansion_250N_wheel_front_1_1
	{
		name = "Expansion_250N_wheel_front_1_1";
		displayName = "Front Bike Wheel";
		selection = "wheel_1_1";
		ghostIcon = "";
	};
	class Slot_Expansion_250N_wheel_back_1_2
	{
		name = "Expansion_250N_wheel_back_1_2";
		displayName = "Rear Bike Wheel";
		selection = "wheel_1_2";
		ghostIcon = "";
	};
};
class CfgVehicles
{
	class ExpansionWheel;
	class ExpansionBikeScript;
	class Inventory_Base;
	class SimulationModule;
	class Axles;
	class Front;
	class Wheels;
	class Rear;
	class Left;
	class Right;
	class AnimationSources;
	class Crew;
	class Driver;
	class Window;
	class DamageSystem;
	class GlobalHealth;
	class DamageZones;
	class Doors;
	class Health;
	class Expansion_250N_Wheel_Front: ExpansionWheel
	{
		scope = 2;
		displayName = "$Unit$ 250N Bike Wheel Front";
		descriptionShort = "$Unit$ NOT READY, you shouldn't be reading this !";
		model = "\DayZExpansion\Vehicles\Ground\250N\proxy\Expansion_250N_wheel_front.p3d";
		itemSize[] = {6,6};
		inventorySlot[] = {"Expansion_250N_wheel_front_1_1"};
		rotationFlags = 4;
		radiusByDamage[] = {0,0.28,0.31,0.28,0.9998,0.25,0.9999,0.2};
		radius = 0.38;
		width = 0.16;
		mass = 20.0;
	};
	class Expansion_250N_Wheel_Back: Expansion_250N_Wheel_Front
	{
		scope = 2;
		displayName = "$Unit$ 250N Bike Wheel Back";
		descriptionShort = "$Unit$ NOT READY, you shouldn't be reading this !";
		model = "\DayZExpansion\Vehicles\Ground\250N\proxy\Expansion_250N_wheel_back.p3d";
		inventorySlot[] = {"Expansion_250N_wheel_back_1_2"};
		radius = 0.4;
		width = 0.16;
		mass = 20.0;
	};
	class Expansion250N: ExpansionBikeScript
	{
		scope = 2;
		displayName = "$Unit$ Old Motorbike";
		descriptionShort = "$Unit$ Old Motobike From Arma2 Imported by ZeRoY";
		model = "\DayZExpansion\Vehicles\Ground\250N\Expansion250N.p3d";
		attachments[] = {"Expansion_250N_wheel_front_1_1","Expansion_250N_wheel_back_1_2","Reflector_1_1","CarBattery","SparkPlug"};
		fuelCapacity = 20;
		fuelConsumption = 15;
		batterySlot = "CarBattery";
		electricPowerResName = "power";
		electricConsumptionIgnition = 3001;
		electricConsumptionEngine = 0.0;
		electricConsumptionLights = 0.0;
		electricOutputEngine = 5.0;
		hiddenSelections[] = {"light_left","light_right","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","zbytek"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\250N\data\old_moto_co.paa"};
		hiddenSelectionsMaterials[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"};
		frontReflectorMatOn = "dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_lights.rvmat";
		frontReflectorMatOff = "DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_lights.rvmat";
		brakeReflectorMatOff = "DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat";
		ReverseReflectorMatOn = "";
		ReverseReflectorMatOff = "";
		TailReflectorMatOn = "dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_lights.rvmat";
		TailReflectorMatOff = "DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat";
		soundController = "Expansion_SoundController_250N";
		class Crew
		{
			numCrew = 2;
			class Crew_0
			{
				isDriver = 1;
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class Crew_1
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
		};
		class VehicleSimulation
		{
			numWheels = 2;
			numAxles = 2;
			numTracks = 0;
			class Wheels
			{
				class Wheel_0
				{
					animDamper = "wheel_1_1_damper";
					inventorySlot = "Expansion_250N_wheel_front_1_1";
					axle = 0;
					direction = "1 0 0";
					animTurn = "wheel_1_1_turn";
					animRotation = "wheel_1_1_rotation";
					wheelHub = "wheel_1_1_damper";
				};
				class Wheel_1
				{
					animDamper = "wheel_1_2_damper";
					inventorySlot = "Expansion_250N_wheel_back_1_2";
					axle = 1;
					direction = "-1 0 0";
					animTurn = "wheel_1_1_turn";
					animRotation = "wheel_1_2_rotation";
					wheelHub = "wheel_1_2_damper";
				};
			};
			class Axles
			{
				class Axle_0
				{
					maxSteeringAngle = 45;
					brakeForce = 4000;
					class Suspension
					{
						stiffness = 500;
						damping = 0.1;
						travelMaxUp = 0.2;
						travelMaxDown = 0.2;
					};
				};
				class Axle_1
				{
					maxSteeringAngle = 0;
					brakeForce = 4000;
					class Suspension
					{
						stiffness = 500;
						damping = 0.1;
						travelMaxUp = 0.2;
						travelMaxDown = 0.2;
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {4,4};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class AnimationSources: AnimationSources
		{
			class Wheel_1_1_Damper
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.01;
			};
			class Wheel_1_2_Damper: Wheel_1_1_Damper{};
			class Wheel_1_1_Rotation
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1;
			};
			class Wheel_1_2_Rotation: Wheel_1_1_Rotation{};
			class Wheel_1_1_Turn
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.01;
			};
			class SteeringWheel
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.01;
			};
			class EngineShake
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.01;
			};
			class BikeStand
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.01;
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 400;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_destruct.rvmat"}}};
					};
					componentNames[] = {"dmgZone_chassis"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {"Engine","SparkPlug","CarBattery"};
				};
				class Front
				{
					class Health
					{
						hitpoints = 200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Engine","dmgZone_light_front"};
					transferToZonesThreshold[] = {0.5,0.5};
					transferToZonesCoefs[] = {0.7,0.7};
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {0.3,0.3};
					inventorySlots[] = {"Reflector_1_1"};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front"};
					transferToZonesCoefs[] = {0.1};
					memoryPoints[] = {"dmgZone_light_front"};
					componentNames[] = {"dmgZone_light_front"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {1.0,1.0};
					inventorySlots[] = {"Reflector_1_1"};
				};
				class Back
				{
					class Health
					{
						hitpoints = 200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"chassis"};
					transferToZonesCoefs[] = {0.3};
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {0.5};
					inventorySlots[] = {};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_destruct.rvmat"}}};
					};
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					fatalInjuryCoef = 0.001;
					inventorySlotsCoefs[] = {0.2};
					inventorySlots[] = {"Engine","SparkPlug","CarBattery"};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\250N\data\old_moto_destruct.rvmat"}}};
					};
					componentNames[] = {"dmgZone_fuelTank"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "Motorbike Engine";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","SparkPlug"};
			};
			class Body
			{
				name = "Motorbike Body";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1"};
			};
			class Chassis
			{
				name = "Motorbike Chassis";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"Expansion_250N_wheel_front_1_1","Expansion_250N_wheel_back_1_2"};
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyVehiclePart: ProxyAttachment
	{
		scope = 2;
		simulation = "ProxyInventory";
		autocenter = 0;
		animated = 0;
		shadow = 1;
		reversed = 0;
	};
	class ProxyExpansion_250N_wheel_front: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\250N\proxy\Expansion_250N_wheel_front.p3d";
		inventorySlot[] = {"Expansion_250N_wheel_front_1_1"};
	};
	class ProxyExpansion_250N_wheel_front_destroyed: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\250N\proxy\Expansion_250N_wheel_front.p3d";
		inventorySlot[] = {"Expansion_250N_wheel_front_1_1"};
	};
	class ProxyExpansion_250N_wheel_back: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\250N\proxy\Expansion_250N_wheel_back.p3d";
		inventorySlot[] = {"Expansion_250N_wheel_back_1_2"};
	};
	class ProxyExpansion_250N_wheel_back_destroyed: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\250N\proxy\Expansion_250N_wheel_back.p3d";
		inventorySlot[] = {"Expansion_250N_wheel_back_1_2"};
	};
};
