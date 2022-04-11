#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Landrover
	{
		units[] = {"ExpansionLandRover_Wheel","Expansion_LandRover_Door_1","Expansion_LandRover_Door_2","Expansion_LandRover_Door_3","Expansion_LandRover_Door_4","Expansion_LandRover_Door_5","Expansion_LandRover_Door_6","ExpansionLandRover"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgSlots
{
	class Slot_Expansion_Landrover_Wheel_1_1
	{
		name = "Expansion_Landrover_Wheel_1_1";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL_DESC";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_Landrover_Wheel_1_2
	{
		name = "Expansion_Landrover_Wheel_1_2";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL_DESC";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_Landrover_Wheel_2_1
	{
		name = "Expansion_Landrover_Wheel_2_1";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL_DESC";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_Landrover_Wheel_2_2
	{
		name = "Expansion_Landrover_Wheel_2_2";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL_DESC";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_LandRover_Door_1
	{
		name = "Expansion_LandRover_Door_1";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_LEFT_DESC";
		selection = "Expansion_LandRover_Door_1";
		ghostIcon = "doorfront";
	};
	class Slot_Expansion_LandRover_Door_2
	{
		name = "Expansion_LandRover_Door_2";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_LEFT_DESC";
		selection = "Expansion_LandRover_Door_2";
		ghostIcon = "doorrear";
	};
	class Slot_Expansion_LandRover_Door_3
	{
		name = "Expansion_LandRover_Door_3";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_RIGHT_DESC";
		selection = "Expansion_LandRover_Door_3";
		ghostIcon = "doorfront";
	};
	class Slot_Expansion_LandRover_Door_4
	{
		name = "Expansion_LandRover_Door_4";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_RIGHT_DESC";
		selection = "Expansion_LandRover_Door_4";
		ghostIcon = "doorrear";
	};
	class Slot_Expansion_LandRover_Door_5
	{
		name = "Expansion_LandRover_Door_5";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_TRUNK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_TRUNK_DESC";
		selection = "Expansion_LandRover_Door_5";
		ghostIcon = "trunk";
	};
	class Slot_Expansion_LandRover_Door_6
	{
		name = "Expansion_LandRover_Door_6";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_HOOD";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_HOOD_DESC";
		selection = "Expansion_LandRover_Door_6";
		ghostIcon = "hood";
	};
};
class CfgVehicles
{
	class CarScript;
	class HatchbackWheel;
	class ExpansionWheelBase;
	class Inventory_Base;
	class CivSedanDoors_Driver;
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
	class CoDriver;
	class CivilianSedan;
	class ExpansionVehicleCarBase;
	class HouseNoDestruct;
	class ExpansionLandRover_Wheel: HatchbackWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Wheel.p3d";
		weight = 15000;
		inventorySlot[] = {"Expansion_Landrover_Wheel_1_1","Expansion_Landrover_Wheel_1_2","Expansion_Landrover_Wheel_2_1","Expansion_Landrover_Wheel_2_2"};
		hiddenSelections[] = {"chassis"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Ground\LandRover\data\landrover_body_ca.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Vehicles\Ground\LandRover\data\landrover_body.rvmat"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.416;
		width = 0.316;
		tyreRollResistance = 0.015;
		tyreTread = 0.7;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.3,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}}};
				};
			};
		};
	};
	class Expansion_LandRover_Door_1: CivSedanDoors_Driver
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_LEFT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_1.p3d";
		hiddenSelections[] = {"interior","body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Ground\LandRover\data\landrover_int_ca.paa","DayZExpansion\Vehicles\Ground\LandRover\data\landrover_body_ca.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Vehicles\Ground\LandRover\data\landrover_int.rvmat","DayZExpansion\Vehicles\Ground\LandRover\data\landrover_body.rvmat"};
		weight = 15000;
		itemSize[] = {10,10};
		itemBehaviour = 0;
		inventorySlot = "Expansion_LandRover_Door_1";
		rotationFlags = 8;
		physLayer = "item_large";
	};
	class Expansion_LandRover_Door_2: Expansion_LandRover_Door_1
	{
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_LEFT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_2.p3d";
		inventorySlot = "Expansion_LandRover_Door_2";
	};
	class Expansion_LandRover_Door_3: Expansion_LandRover_Door_1
	{
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_FRONT_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_3.p3d";
		inventorySlot = "Expansion_LandRover_Door_3";
	};
	class Expansion_LandRover_Door_4: Expansion_LandRover_Door_1
	{
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_BACK_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_4.p3d";
		inventorySlot = "Expansion_LandRover_Door_4";
	};
	class Expansion_LandRover_Door_5: Expansion_LandRover_Door_1
	{
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_TRUNK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_TRUNK_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_5.p3d";
		inventorySlot = "Expansion_LandRover_Door_5";
		rotationFlags = 2;
		itemBehaviour = 0;
	};
	class Expansion_LandRover_Door_6: Expansion_LandRover_Door_1
	{
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_HOOD";
		descriptionShort = "$STR_EXPANSION_VEHICLE_LANDROVER_DOOR_HOOD_DESC";
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_6.p3d";
		inventorySlot = "Slot_Expansion_LandRover_Door_6";
		rotationFlags = 2;
		itemBehaviour = 0;
	};
	class ExpansionLandRover: CivilianSedan
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover.p3d";
		vehicleClass = "Expansion_Car";
		displayName = "$STR_EXPANSION_VEHICLE_LANDROVER";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","Expansion_Landrover_Wheel_1_1","Expansion_Landrover_Wheel_1_2","Expansion_Landrover_Wheel_2_1","Expansion_Landrover_Wheel_2_2","Expansion_LandRover_Door_1","Expansion_LandRover_Door_2","Expansion_LandRover_Door_3","Expansion_LandRover_Door_4","Expansion_LandRover_Door_5","Expansion_LandRover_Door_6"};
		doors[] = {"Expansion_LandRover_Door_1","Expansion_LandRover_Door_2","Expansion_LandRover_Door_3","Expansion_LandRover_Door_4"};
		defaultSkin = "Green";
		fuelCapacity = 40;
		fuelConsumption = 10;
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo1
			{
				actionSel = "seat_cargo1";
				proxyPos = "crewCargo1";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			centralDiffRatio = 1.45;
			airDragFrontTotal = 0.995;
			class Steering
			{
				increaseSpeed[] = {0,45,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime = 1.25;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 5;
				gentleCoef = 0.75;
			};
			braking[] = {0.0,0.1,1.0,0.8,2.5,0.9,3.0,1.0};
			class Engine
			{
				inertia = 0.15;
				steepness = 1.5;
				torqueMax = 136;
				torqueRpm = 3400;
				powerMax = 58.7;
				powerRpm = 5400;
				rpmIdle = 850;
				rpmMin = 900;
				rpmClutch = 1350;
				rpmRedline = 6000;
				rpmMax = 8000;
			};
			class Gearbox
			{
				reverse = 3.726;
				ratios[] = {3.967,2.4,1.561,1};
				timeToUncoupleClutch = 0.2;
				timeToCoupleClutch = 0.35;
				maxClutchTorque = 330;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 30;
					finalRatio = 4.1;
					brakeBias = 0.6;
					brakeForce = 4500;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1700;
						stiffness = 40000;
						compression = 2100;
						damping = 7500;
						travelMaxUp = 0.0882;
						travelMaxDown = 0.0833;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_1";
							inventorySlot = "Expansion_Landrover_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "Expansion_Landrover_Wheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.4;
					brakeForce = 4200;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1800;
						stiffness = 21000;
						compression = 2200;
						damping = 7600;
						travelMaxUp = 0.1587;
						travelMaxDown = 0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "Expansion_Landrover_Wheel_1_2";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "Expansion_Landrover_Wheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,45};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.4002;
				animPeriod = 1;
			};
			class damper_2_2: damper_1_2{};
			class Expansion_LandRover_Door_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class Expansion_LandRover_Door_2: Expansion_LandRover_Door_1{};
			class Expansion_LandRover_Door_3: Expansion_LandRover_Door_1{};
			class Expansion_LandRover_Door_4: Expansion_LandRover_Door_1{};
			class Expansion_LandRover_Door_5: Expansion_LandRover_Door_1{};
			class Expansion_LandRover_Door_6: Expansion_LandRover_Door_1{};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"offroad_Engine_Offload_Ext_Rpm1_SoundSet","offroad_Engine_Offload_Ext_Rpm2_SoundSet","offroad_Engine_Offload_Ext_Rpm3_SoundSet","offroad_Engine_Offload_Ext_Rpm4_SoundSet","offroad_Engine_Offload_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Rpm0_SoundSet","offroad_Engine_Ext_Rpm1_SoundSet","offroad_Engine_Ext_Rpm2_SoundSet","offroad_Engine_Ext_Rpm3_SoundSet","offroad_Engine_Ext_Rpm4_SoundSet","offroad_Engine_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Broken_SoundSet","offroad_Tires_rock_slow_Ext_SoundSet","offroad_Tires_rock_fast_Ext_SoundSet","offroad_Tires_grass_slow_Ext_SoundSet","offroad_Tires_grass_fast_Ext_SoundSet","offroad_Tires_gravel_slow_Ext_SoundSet","offroad_Tires_gravel_fast_Ext_SoundSet","offroad_Tires_gravel_dust_fast_Ext_SoundSet","offroad_Tires_asphalt_slow_Ext_SoundSet","offroad_Tires_asphalt_fast_Ext_SoundSet","offroad_Tires_water_slow_Ext_SoundSet","offroad_Tires_water_fast_Ext_SoundSet","Offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_Rain_Ext_SoundSet","offroad_damper_left_SoundSet","offroad_damper_right_SoundSet"};
			soundSetsInt[] = {"Offroad_Tires_Asphalt_Slow_General_Int_SoundSet","Offroad_Tires_Asphalt_Fast_General_Int_SoundSet","Offroad_Wind_SoundSet"};
		};
		class ObstacleGenerator
		{
			carve = 1;
			timeToStationary = 5;
			moveThreshold = 0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,0.7};
					};
					class Cyl3
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,-0.7};
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","Expansion_LandRover_Door_1","Expansion_LandRover_Door_2","Expansion_LandRover_Door_3","Expansion_LandRover_Door_4","Expansion_LandRover_Door_5","Expansion_LandRover_Door_6"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"Expansion_Landrover_Wheel_1_1","Expansion_Landrover_Wheel_1_2","Expansion_Landrover_Wheel_2_1","Expansion_Landrover_Wheel_2_2","Expansion_Landrover_Wheel_Spare"};
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyLandRover_Wheel: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Wheel.p3d";
		inventorySlot[] = {"Expansion_Landrover_Wheel_1_1","Expansion_Landrover_Wheel_1_2","Expansion_Landrover_Wheel_2_1","Expansion_Landrover_Wheel_2_2","Expansion_Landrover_Wheel_Spare"};
	};
	class ProxyLandRover_Door_1: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_1.p3d";
		inventorySlot = "Expansion_LandRover_Door_1";
	};
	class ProxyLandRover_Door_2: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_2.p3d";
		inventorySlot = "Expansion_LandRover_Door_2";
	};
	class ProxyLandRover_Door_3: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_3.p3d";
		inventorySlot = "Expansion_LandRover_Door_3";
	};
	class ProxyLandRover_Door_4: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_4.p3d";
		inventorySlot = "Expansion_LandRover_Door_4";
	};
	class ProxyLandRover_Door_5: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_5.p3d";
		inventorySlot = "Expansion_LandRover_Door_5";
	};
	class ProxyLandRover_Door_6: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\LandRover\LandRover_Door_6.p3d";
		inventorySlot = "Expansion_LandRover_Door_6";
	};
};
