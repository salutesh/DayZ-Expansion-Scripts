#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_UAZ
	{
		units[] = {"ExpansionUAZWheel","Vehicle_ExpansionUAZWheel","ExpansionUAZDoorDriver","ExpansionUAZDoorCoDriver","ExpansionUAZDoorCargo1","ExpansionUAZDoorCargo2","ExpansionUAZDoorTrunk","ExpansionUAZ","ExpansionUAZRoofless","ExpansionUAZCargoRoofless","ExpansionUAZCargo","Vehicle_ExpansionUAZ","Vehicle_ExpansionUAZRoofless","Vehicle_ExpansionUAZCargoRoofless","Vehicle_ExpansionUAZCargo","ExpansionSpraycanGreen","ExpansionUAZ_Green","ExpansionUAZCargo_Green","ExpansionUAZRoofless_Green","ExpansionUAZCargoRoofless_Green","Vehicle_ExpansionUAZ_Green","Vehicle_ExpansionUAZCargo_Green","Vehicle_ExpansionUAZRoofless_Green","Vehicle_ExpansionUAZCargoRoofless_Green","ExpansionUAZDoorDriver_Green","ExpansionUAZDoorCoDriver_Green","ExpansionUAZDoorCargo1_Green","ExpansionUAZDoorCargo2_Green","ExpansionSpraycanBlack","ExpansionUAZ_Black","ExpansionUAZCargo_Black","ExpansionUAZRoofless_Black","ExpansionUAZCargoRoofless_Black","Vehicle_ExpansionUAZ_Black","Vehicle_ExpansionUAZCargo_Black","Vehicle_ExpansionUAZRoofless_Black","Vehicle_ExpansionUAZCargoRoofless_Black","ExpansionUAZDoorDriver_Black","ExpansionUAZDoorCoDriver_Black","ExpansionUAZDoorCargo1_Black","ExpansionUAZDoorCargo2_Black"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgSlots
{
	class Slot_uazwheel_1_1
	{
		name = "uazwheel_1_1";
		displayName = "UAZ Wheel";
		descriptionShort = "";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_uazwheel_1_2
	{
		name = "uazwheel_1_2";
		displayName = "UAZ Wheel";
		descriptionShort = "";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_uazwheel_2_1
	{
		name = "uazwheel_2_1";
		displayName = "UAZ Wheel";
		descriptionShort = "";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_uazwheel_2_2
	{
		name = "uazwheel_2_2";
		displayName = "UAZ Wheel";
		descriptionShort = "";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_uazdriverdoor
	{
		name = "uazdriverdoor";
		displayName = "";
		selection = "uazdriverdoor";
		ghostIcon = "doorfront";
	};
	class Slot_uazcodriverdoor
	{
		name = "uazcodriverdoor";
		displayName = "";
		descriptionShort = "";
		selection = "uazcodriverdoor";
		ghostIcon = "doorfront";
	};
	class Slot_uazcargo1door
	{
		name = "uazcargo1door";
		displayName = "";
		descriptionShort = "";
		selection = "uazcargo1door";
		ghostIcon = "doorrear";
	};
	class Slot_uazcargo2door
	{
		name = "uazcargo2door";
		displayName = "";
		descriptionShort = "";
		selection = "uazcargo2door";
		ghostIcon = "doorrear";
	};
	class Slot_uaztrunkdoor
	{
		name = "uaztrunkdoor";
		displayName = "";
		descriptionShort = "";
		selection = "uaztrunkdoor";
		ghostIcon = "trunk";
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
	class ExpansionUAZWheel: HatchbackWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel.p3d";
		weight = 15000;
		inventorySlot[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
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
	class Vehicle_ExpansionUAZWheel: ExpansionWheelBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel.p3d";
		weight = 15000;
		inventorySlot[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
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
	class ExpansionUAZDoorDriver: CivSedanDoors_Driver
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT_DES";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazdriverdoor.p3d";
		weight = 15000;
		itemSize[] = {10,10};
		itemBehaviour = 0;
		inventorySlot = "uazdriverdoor";
		rotationFlags = 8;
		physLayer = "item_large";
	};
	class ExpansionUAZDoorCoDriver: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcodriverdoor.p3d";
		inventorySlot = "uazcodriverdoor";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZDoorCargo1: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo1door.p3d";
		inventorySlot = "uazcargo1door";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZDoorCargo2: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo2door.p3d";
		inventorySlot = "uazcargo2door";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZDoorTrunk: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazTrunkdoor.p3d";
		inventorySlot = "uaztrunkdoor";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZBase: CivilianSedan
	{
		scope = 0;
		displayName = "";
		model = "";
		vehicleClass = "Expansion_Car";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
		doors[] = {"uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
		defaultSkin = "Green";
		fuelCapacity = 42;
		fuelConsumption = 11;
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
				reverse = 3.426;
				ratios[] = {4.067,1.9999999,1.2609999,0.9};
				timeToUncoupleClutch = 0.2;
				timeToCoupleClutch = 0.35;
				maxClutchTorque = 300;
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
							inventorySlot = "uazwheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "uazwheel_2_1";
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
							inventorySlot = "uazwheel_1_2";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "uazwheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,30};
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
			class uazdriverdoor
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class uazcodriverdoor: uazdriverdoor{};
			class uazcargo1door: uazdriverdoor{};
			class uazcargo2door: uazdriverdoor{};
			class uaztrunkdoor: uazdriverdoor{};
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
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
			};
		};
	};
	class Vehicle_ExpansionUAZBase: ExpansionVehicleCarBase
	{
		scope = 0;
		displayName = "";
		model = "";
		vehicleClass = "Expansion_Car";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
		doors[] = {"uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
		defaultSkin = "Green";
		fuelCapacity = 42;
		fuelConsumption = 11;
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
				reverse = 3.426;
				ratios[] = {4.067,1.9999999,1.2609999,0.9};
				timeToUncoupleClutch = 0.2;
				timeToCoupleClutch = 0.35;
				maxClutchTorque = 300;
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
						class Left
						{
							inventorySlot = "uazwheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right
						{
							inventorySlot = "uazwheel_2_1";
							animTurn = "turnfrontright";
							animRotation = "wheelfrontright";
							animDamper = "damper_2_1";
							wheelHub = "wheel_2_1_damper_land";
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
						class Left
						{
							inventorySlot = "uazwheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right
						{
							inventorySlot = "uazwheel_2_2";
							animTurn = "turnbackright";
							animRotation = "wheelbackright";
							animDamper = "damper_2_2";
							wheelHub = "wheel_2_2_damper_land";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,30};
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
			class uazdriverdoor
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class uazcodriverdoor: uazdriverdoor{};
			class uazcargo1door: uazdriverdoor{};
			class uazcargo2door: uazdriverdoor{};
			class uaztrunkdoor: uazdriverdoor{};
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
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
			};
		};
	};
	class ExpansionUAZ: ExpansionUAZBase
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZ.p3d";
		vehicleClass = "Expansion_Car";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","body_exterior","__interior"};
		hiddenSelectionsTextures[] = {"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa","dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa","","","","","","","","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_int_01_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_interior_01.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		attachments[] = {"CarBattery","CarRadiator","SparkPlug","Reflector_1_1","Reflector_2_1","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
		};
	};
	class ExpansionUAZRoofless: ExpansionUAZ
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ_ROOFLESS";
		vehicleClass = "Expansion_Car";
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
		};
	};
	class ExpansionUAZCargoRoofless: ExpansionUAZ
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ_CARGO_ROOFLESS";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZCargoRoofless.p3d";
		vehicleClass = "Expansion_Car";
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,45};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
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
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
			class Cargo2
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
		};
	};
	class ExpansionUAZCargo: ExpansionUAZ
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ_CARGO";
		vehicleClass = "Expansion_Car";
		class AnimationSources: AnimationSources
		{
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,45};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
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
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
			class Cargo2
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
		};
	};
	class Vehicle_ExpansionUAZ: Vehicle_ExpansionUAZBase
	{
		scope = 2;
		displayname = "[NOT READY]  UAZ";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZ.p3d";
		vehicleClass = "Expansion_Car";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","body_exterior","__interior"};
		hiddenSelectionsTextures[] = {"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa","dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa","","","","","","","","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_int_01_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_interior_01.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		attachments[] = {"CarBattery","CarRadiator","SparkPlug","Reflector_1_1","Reflector_2_1","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
		};
	};
	class Vehicle_ExpansionUAZRoofless: Vehicle_ExpansionUAZ
	{
		scope = 2;
		displayname = "[NOT READY]  UAZ ROOFLESS";
		vehicleClass = "Expansion_Car";
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
		};
	};
	class Vehicle_ExpansionUAZCargoRoofless: Vehicle_ExpansionUAZ
	{
		scope = 2;
		displayname = "[NOT READY]  UAZ CARGO ROOFLESS";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZCargoRoofless.p3d";
		vehicleClass = "Expansion_Car";
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,45};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
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
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
			class Cargo2
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
		};
	};
	class Vehicle_ExpansionUAZCargo: Vehicle_ExpansionUAZ
	{
		scope = 2;
		displayname = "[NOT READY]  UAZ CARGO";
		vehicleClass = "Expansion_Car";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
		doors[] = {"uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
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
			class uazdriverdoor
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class uazcodriverdoor: uazdriverdoor{};
			class uazcargo1door: uazdriverdoor{};
			class uazcargo2door: uazdriverdoor{};
			class uaztrunkdoor: uazdriverdoor{};
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,45};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
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
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
			class Cargo2
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
		};
		class SimulationModule
		{
			class Axles
			{
				class Front
				{
					maxSteeringAngle = 35;
					finalRatio = 0.0;
					brakeBias = 0.7;
					brakeForce = 4500;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 300;
						stiffness = 60000;
						compression = 2100;
						damping = 5500;
						travelMaxUp = 0.079;
						travelMaxDown = 0.06;
					};
				};
				class Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.3;
					brakeForce = 3500;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 275;
						stiffness = 34000;
						compression = 2000;
						damping = 5100;
						travelMaxUp = 0.086;
						travelMaxDown = 0.133;
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
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionUAZ_Green: ExpansionUAZ
	{
		skinBase = "ExpansionUAZ";
		skinName = "Green";
	};
	class ExpansionUAZCargo_Green: ExpansionUAZCargo
	{
		skinBase = "ExpansionUAZCargo";
		skinName = "Green";
	};
	class ExpansionUAZRoofless_Green: ExpansionUAZRoofless
	{
		skinBase = "ExpansionUAZRoofless";
		skinName = "Green";
	};
	class ExpansionUAZCargoRoofless_Green: ExpansionUAZCargoRoofless
	{
		skinBase = "ExpansionUAZCargoRoofless";
		skinName = "Green";
	};
	class Vehicle_ExpansionUAZ_Green: Vehicle_ExpansionUAZ
	{
		skinBase = "Vehicle_ExpansionUAZ";
		skinName = "Green";
	};
	class Vehicle_ExpansionUAZCargo_Green: Vehicle_ExpansionUAZCargo
	{
		skinBase = "Vehicle_ExpansionUAZCargo";
		skinName = "Green";
	};
	class Vehicle_ExpansionUAZRoofless_Green: Vehicle_ExpansionUAZRoofless
	{
		skinBase = "Vehicle_ExpansionUAZRoofless";
		skinName = "Green";
	};
	class Vehicle_ExpansionUAZCargoRoofless_Green: Vehicle_ExpansionUAZCargoRoofless
	{
		skinBase = "Vehicle_ExpansionUAZCargoRoofless";
		skinName = "Green";
	};
	class ExpansionUAZDoorDriver_Green: ExpansionUAZDoorDriver
	{
		skinBase = "ExpansionUAZDoorDriver";
		skinName = "Green";
	};
	class ExpansionUAZDoorCoDriver_Green: ExpansionUAZDoorCoDriver
	{
		skinBase = "ExpansionUAZDoorCoDriver";
		skinName = "Green";
	};
	class ExpansionUAZDoorCargo1_Green: ExpansionUAZDoorCargo1
	{
		skinBase = "ExpansionUAZDoorCargo1";
		skinName = "Green";
	};
	class ExpansionUAZDoorCargo2_Green: ExpansionUAZDoorCargo2
	{
		skinBase = "ExpansionUAZDoorCargo2";
		skinName = "Green";
	};
	class ExpansionSpraycanBlack: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Black_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Black_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Black";
	};
	class ExpansionUAZ_Black: ExpansionUAZ
	{
		skinBase = "ExpansionUAZ";
		skinName = "Black";
	};
	class ExpansionUAZCargo_Black: ExpansionUAZCargo
	{
		skinBase = "ExpansionUAZCargo";
		skinName = "Black";
	};
	class ExpansionUAZRoofless_Black: ExpansionUAZRoofless
	{
		skinBase = "ExpansionUAZRoofless";
		skinName = "Black";
	};
	class ExpansionUAZCargoRoofless_Black: ExpansionUAZCargoRoofless
	{
		skinBase = "ExpansionUAZCargoRoofless";
		skinName = "Black";
	};
	class Vehicle_ExpansionUAZ_Black: Vehicle_ExpansionUAZ
	{
		skinBase = "Vehicle_ExpansionUAZ";
		skinName = "Black";
	};
	class Vehicle_ExpansionUAZCargo_Black: Vehicle_ExpansionUAZCargo
	{
		skinBase = "Vehicle_ExpansionUAZCargo";
		skinName = "Black";
	};
	class Vehicle_ExpansionUAZRoofless_Black: Vehicle_ExpansionUAZRoofless
	{
		skinBase = "Vehicle_ExpansionUAZRoofless";
		skinName = "Black";
	};
	class Vehicle_ExpansionUAZCargoRoofless_Black: Vehicle_ExpansionUAZCargoRoofless
	{
		skinBase = "Vehicle_ExpansionUAZCargoRoofless";
		skinName = "Black";
	};
	class ExpansionUAZDoorDriver_Black: ExpansionUAZDoorDriver
	{
		skinBase = "ExpansionUAZDoorDriver";
		skinName = "Black";
	};
	class ExpansionUAZDoorCoDriver_Black: ExpansionUAZDoorCoDriver
	{
		skinBase = "ExpansionUAZDoorCoDriver";
		skinName = "Black";
	};
	class ExpansionUAZDoorCargo1_Black: ExpansionUAZDoorCargo1
	{
		skinBase = "ExpansionUAZDoorCargo1";
		skinName = "Black";
	};
	class ExpansionUAZDoorCargo2_Black: ExpansionUAZDoorCargo2
	{
		skinBase = "ExpansionUAZDoorCargo2";
		skinName = "Black";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyUAZWheel: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel.p3d";
		inventorySlot[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
	};
	class Proxyuazdriverdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazdriverdoor.p3d";
		inventorySlot = "uazdriverdoor";
	};
	class Proxyuazcodriverdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcodriverdoor.p3d";
		inventorySlot = "uazcodriverdoor";
	};
	class Proxyuazcargo1door: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo1door.p3d";
		inventorySlot = "uazcargo1door";
	};
	class Proxyuazcargo2door: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo2door.p3d";
		inventorySlot = "uazcargo2door";
	};
	class Proxyuaztrunkdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uaztrunkdoor.p3d";
		inventorySlot = "uaztrunkdoor";
	};
};
