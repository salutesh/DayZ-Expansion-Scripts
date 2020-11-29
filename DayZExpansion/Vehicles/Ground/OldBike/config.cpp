#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_oldbike
	{
		units[] = {"Vehicle_ExpansionOldBikeWheel","Vehicle_ExpansionOldBike"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DZ_Vehicles_Parts","DZ_Sounds_Effects"};
	};
};
class CfgSlots
{
	class Slot_Expansion_Old_Bike_Wheel_1
	{
		name = "expansion_old_bike_wheel_1";
		displayName = "Front Bicycle Wheel";
		selection = "wheel_1";
		ghostIcon = "";
	};
	class Slot_Expansion_Old_Bike_Wheel_2
	{
		name = "expansion_old_bike_wheel_2";
		displayName = "Rear Bicycle Wheel";
		selection = "wheel_2";
		ghostIcon = "";
	};
};
class CfgVehicles
{
	class ExpansionWheelBase;
	class ExpansionVehicleBikeBase;
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
	class Vehicle_ExpansionOldBikeWheel: ExpansionWheelBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_OLDBIKE_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_OLDBIKE_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\oldbike\proxy\Expansion_old_bike_wheel.p3d";
		itemSize[] = {6,6};
		inventorySlot[] = {"expansion_old_bike_wheel_1","expansion_old_bike_wheel_2"};
		rotationFlags = 4;
		radiusByDamage[] = {0,0.28,0.31,0.28,0.9998,0.25,0.9999,0.2};
		radius = 0.429;
		width = 0.1;
		mass = 20.0;
	};
	class Vehicle_ExpansionOldBike: ExpansionVehicleBikeBase
	{
		scope = 2;
		displayname = "[NOT READY]  OLD BIKE";
		model = "\DayZExpansion\Vehicles\Ground\OldBike\old_bike.p3d";
		attachments[] = {"expansion_old_bike_wheel_1","expansion_old_bike_wheel_2"};
		fuelCapacity = 0;
		fuelConsumption = 0;
		batterySlot = "";
		electricPowerResName = "power";
		electricConsumptionIgnition = 3001;
		electricConsumptionEngine = 0.0;
		electricConsumptionLights = 0.0;
		electricOutputEngine = 5.0;
		hiddenSelections[] = {"light_left","light_right","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","zbytek"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_co.paa"};
		hiddenSelectionsMaterials[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike.rvmat"};
		frontReflectorMatOn = "";
		frontReflectorMatOff = "";
		brakeReflectorMatOn = "";
		brakeReflectorMatOff = "";
		ReverseReflectorMatOn = "";
		ReverseReflectorMatOff = "";
		TailReflectorMatOn = "";
		TailReflectorMatOff = "";
		soundController = "Expansion_SoundController_oldbike";
		class Crew
		{
			class Driver
			{
				isDriver = 1;
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
		};
		class SimulationModule
		{
			class Steering
			{
				increaseSpeed[] = {3.0};
				decreaseSpeed[] = {40.0};
				centeringSpeed[] = {30.0};
			};
			class Throttle
			{
				reactionTime = 1.0;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 4.0;
				gentleCoef = 0.75;
			};
			braking[] = {0.0,0.1,1.0,0.8,2.5,0.9,3.0,1.0};
			class Engine
			{
				inertia = 0.15;
				torqueMax = 114;
				torqueRpm = 3400;
				powerMax = 53.7;
				powerRpm = 5400;
				rpmIdle = 850;
				rpmMin = 900;
				rpmClutch = 1350;
				rpmRedline = 6000;
				rpmMax = 8000;
			};
			class Gearbox
			{
				reverse = 3.526;
				ratios[] = {3.667,2.1,1.361,1.0};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 260;
			};
			class Axles
			{
				class Front
				{
					maxSteeringAngle = 45;
					finalRatio = 0.0;
					brakeBias = 0.7;
					brakeForce = 4000;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 0;
						stiffness = 21000;
						compression = 5100;
						damping = 6700;
						travelMaxUp = 0.001;
						travelMaxDown = 0.001;
					};
					class Wheels
					{
						class Center
						{
							inventorySlot = "expansion_old_bike_wheel_1";
							animTurn = "turnfront";
							animRotation = "wheelfront";
							animDamper = "damper_1";
							wheelHub = "wheel_1_damper_land";
							axis_start = "suspension_1_start";
							axis_end = "suspension_1_end";
						};
					};
				};
				class Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 3.0;
					brakeBias = 0.7;
					brakeForce = 4000;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 0;
						stiffness = 20000;
						compression = 5000;
						damping = 6500;
						travelMaxUp = 0.001;
						travelMaxDown = 0.001;
					};
					class Wheels
					{
						class Center
						{
							inventorySlot = "expansion_old_bike_wheel_2";
							animTurn = "";
							animRotation = "wheelback";
							animDamper = "damper_2";
							wheelHub = "wheel_2_damper_land";
						};
					};
				};
			};
		};
		class AnimationSources
		{
			class Damper_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1e-05;
			};
			class Damper_2: Damper_1{};
			class wheelfront
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1e-05;
			};
			class wheelback: wheelfront{};
			class DrivingWheel
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1e-05;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {4,4};
			allowOwnedCargoManipulation = 1;
			openable = 0;
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
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_destruct.rvmat"}}};
					};
					componentNames[] = {"dmgZone_chassis"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {"","",""};
				};
				class Front
				{
					class Health
					{
						hitpoints = 200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Engine","dmgZone_light_front"};
					transferToZonesThreshold[] = {0.5,0.5};
					transferToZonesCoefs[] = {0.7,0.7};
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {0.3,0.3};
					inventorySlots[] = {""};
				};
				class Back
				{
					class Health
					{
						hitpoints = 200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_damage.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_damage.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\oldbike\data\expansion_old_bike_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"chassis"};
					transferToZonesCoefs[] = {0.3};
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {0.5};
					inventorySlots[] = {};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"expansion_old_bike_wheel_1","expansion_old_bike_wheel_2"};
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class Proxyexpansion_old_bike_wheel: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\OldBike\proxy\expansion_old_bike_wheel.p3d";
		inventorySlot[] = {"expansion_old_bike_wheel_1","expansion_old_bike_wheel_2"};
	};
	class Proxyexpansion_old_bike_wheel_destroyed: Proxyexpansion_old_bike_wheel
	{
		model = "\DayZExpansion\Vehicles\Ground\OldBike\proxy\expansion_old_bike_wheel.p3d";
	};
};
