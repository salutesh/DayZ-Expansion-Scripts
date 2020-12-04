#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Ikarus
	{
		units[] = {"ExpansionBusWheel","ExpansionBusWheel_Ruined","ExpansionBusWheelDouble","ExpansionBusWheelDouble_Ruined","ExpansionBus","Vehicle_ExpansionBusWheel","Vehicle_ExpansionBusWheel_Ruined","Vehicle_ExpansionBusWheelDouble","Vehicle_ExpansionBusWheelDouble_Ruined","Vehicle_ExpansionBus","ExpansionSpraycanBlue","ExpansionBus_Blue","Vehicle_ExpansionBus_Blue","ExpansionSpraycanWeeb","ExpansionBus_Weeb","Vehicle_ExpansionBus_Weeb","ExpansionSpraycanRed","ExpansionBus_Red","Vehicle_ExpansionBus_Red"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgSlots
{
	class Slot_busmegaphone
	{
		name = "busmegaphone";
		displayName = "";
		selection = "busmegaphone";
		ghostIcon = "walkietalkie";
	};
};
class CfgVehicles
{
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
	class CoDriver;
	class TransitBusWheel;
	class TransitBusWheelDouble;
	class CarScript;
	class CarWheel;
	class ExpansionBusWheel: CarWheel
	{
		scope = 2;
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel.p3d";
		displayName = "$STR_TransitBusWheel0";
		itemSize[] = {6,6};
		weight = 25000;
		physLayer = "item_large";
		rotationFlags = 12;
		inventorySlot[] = {"BusWheel_1_1","BusWheel_2_1"};
		radiusByDamage[] = {0,0.51,0.3,0.45,0.9998,0.38,0.9999,0.3};
		radius = 0.51;
		friction = 0.999;
		width = 0.213;
		tyreRollResistance = 0.015;
		tyreTread = 0.8;
	};
	class ExpansionBusWheel_Ruined: ExpansionBusWheel
	{
		scope = 2;
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_destroyed.p3d";
		displayName = "$STR_TransitBusWheel_Ruined0";
		itemSize[] = {6,6};
		weight = 25000;
		physLayer = "item_large";
		rotationFlags = 12;
		inventorySlot[] = {"BusWheel_1_1","BusWheel_2_1"};
		radius = 0.3;
		friction = -1.0;
		width = 0.213;
	};
	class ExpansionBusWheelDouble: ExpansionBusWheel
	{
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear.p3d";
		displayName = "$STR_TransitBusWheelDouble0";
		width = 0.426;
		inventorySlot[] = {"BusWheel_1_2","BusWheel_2_2"};
		tyreRollResistance = 0.015;
		tyreTread = 0.8;
	};
	class ExpansionBusWheelDouble_Ruined: ExpansionBusWheelDouble
	{
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear_destroyed.p3d";
		displayName = "$STR_TransitBusWheelDouble_Ruined0";
		width = 0.426;
		inventorySlot[] = {"BusWheel_1_2","BusWheel_2_2"};
	};
	class ExpansionBus: CarScript
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_BUS";
		model = "\DayZExpansion\Vehicles\Ground\Bus\Ikarus.p3d";
		vehicleClass = "Expansion_Car";
		attachments[] = {"TruckBattery","Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug","BusWheel_1_1","BusWheel_1_2","BusWheel_2_1","BusWheel_2_2"};
		doors[] = {};
		fuelCapacity = 42;
		fuelConsumption = 11;
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","body"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior_co.paa"};
		hiddenSelectionsMaterials[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat"};
		defaultSkin = "Blue";
		dashboardMatOn = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		frontReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		brakeReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on_red.rvmat";
		brakeReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		ReverseReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		ReverseReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		TailReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		TailReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos driver";
				getInDir = "pos driver dir";
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
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo3
			{
				actionSel = "seat_cargo3";
				proxyPos = "crewCargo3";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo4
			{
				actionSel = "seat_cargo4";
				proxyPos = "crewCargo4";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo5
			{
				actionSel = "seat_cargo5";
				proxyPos = "crewCargo5";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo6
			{
				actionSel = "seat_cargo6";
				proxyPos = "crewCargo6";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo7
			{
				actionSel = "seat_cargo7";
				proxyPos = "crewCargo7";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo8
			{
				actionSel = "seat_cargo8";
				proxyPos = "crewCargo8";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo9
			{
				actionSel = "seat_cargo9";
				proxyPos = "crewCargo9";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo10
			{
				actionSel = "seat_cargo10";
				proxyPos = "crewCargo10";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo11
			{
				actionSel = "seat_cargo11";
				proxyPos = "crewCargo11";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo12
			{
				actionSel = "seat_cargo12";
				proxyPos = "crewCargo12";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo13
			{
				actionSel = "seat_cargo13";
				proxyPos = "crewCargo13";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo14
			{
				actionSel = "seat_cargo14";
				proxyPos = "crewCargo14";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo15
			{
				actionSel = "seat_cargo15";
				proxyPos = "crewCargo15";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo16
			{
				actionSel = "seat_cargo16";
				proxyPos = "crewCargo16";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo17
			{
				actionSel = "seat_cargo17";
				proxyPos = "crewCargo17";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo18
			{
				actionSel = "seat_cargo18";
				proxyPos = "crewCargo18";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo19
			{
				actionSel = "seat_cargo19";
				proxyPos = "crewCargo19";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo20
			{
				actionSel = "seat_cargo20";
				proxyPos = "crewCargo20";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragFrontTotal = 0.795;
			class Steering
			{
				increaseSpeed[] = {0,45,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
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
				reverse = 5.526;
				ratios[] = {5.667,2.5,1.861,1.3};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 660;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 30;
					finalRatio = 4.1;
					brakeBias = 0.6;
					brakeForce = 4000;
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
							inventorySlot = "BusWheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "BusWheel_2_1";
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
					brakeForce = 3800;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1800;
						stiffness = 40000;
						compression = 2200;
						damping = 7600;
						travelMaxUp = 0.1587;
						travelMaxDown = 0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "BusWheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "BusWheel_2_2";
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
			itemsCargoSize[] = {10,50};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1e-05;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.4002;
				animPeriod = 1e-05;
			};
			class damper_2_2: damper_1_2{};
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
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"TruckBattery","CarRadiator","GlowPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"BusWheel_1_1","BusWheel_1_2","BusWheel_2_1","BusWheel_2_2"};
			};
		};
	};
	class ExpansionVehicleCarBase;
	class ExpansionWheelBase;
	class Vehicle_ExpansionBusWheel: ExpansionWheelBase
	{
		scope = 2;
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel.p3d";
		displayName = "$STR_TransitBusWheel0";
		itemSize[] = {6,6};
		weight = 25000;
		physLayer = "item_large";
		rotationFlags = 12;
		inventorySlot[] = {"BusWheel_1_1","BusWheel_2_1"};
		radiusByDamage[] = {0,0.51,0.3,0.45,0.9998,0.38,0.9999,0.3};
		radius = 0.5;
		friction = 0.999;
		width = 0.213;
		tyreRollResistance = 0.015;
		tyreTread = 0.8;
	};
	class Vehicle_ExpansionBusWheel_Ruined: Vehicle_ExpansionBusWheel
	{
		scope = 2;
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_destroyed.p3d";
		displayName = "$STR_TransitBusWheel_Ruined0";
		itemSize[] = {6,6};
		weight = 25000;
		physLayer = "item_large";
		rotationFlags = 12;
		inventorySlot[] = {"BusWheel_1_1","BusWheel_2_1"};
		radius = 0.3;
		friction = -1.0;
		width = 0.213;
	};
	class Vehicle_ExpansionBusWheelDouble: Vehicle_ExpansionBusWheel
	{
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear.p3d";
		displayName = "$STR_TransitBusWheelDouble0";
		width = 0.426;
		inventorySlot[] = {"BusWheel_1_2","BusWheel_2_2"};
		tyreRollResistance = 0.015;
		tyreTread = 0.8;
	};
	class Vehicle_ExpansionBusWheelDouble_Ruined: Vehicle_ExpansionBusWheelDouble
	{
		model = "\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear_destroyed.p3d";
		displayName = "$STR_TransitBusWheelDouble_Ruined0";
		width = 0.426;
		inventorySlot[] = {"BusWheel_1_2","BusWheel_2_2"};
	};
	class Vehicle_ExpansionBus: ExpansionVehicleCarBase
	{
		scope = 2;
		displayname = "[NOT READY]  BUS";
		model = "\DayZExpansion\Vehicles\Ground\Bus\Ikarus.p3d";
		vehicleClass = "Expansion_Car";
		attachments[] = {"TruckBattery","Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug","BusWheel_1_1","BusWheel_1_2","BusWheel_2_1","BusWheel_2_2"};
		doors[] = {};
		fuelCapacity = 42;
		fuelConsumption = 11;
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","body"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior_co.paa"};
		hiddenSelectionsMaterials[] = {"","","","","","","","","","DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat"};
		defaultSkin = "Blue";
		dashboardMatOn = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		frontReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		brakeReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on_red.rvmat";
		brakeReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		ReverseReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		ReverseReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		TailReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		TailReflectorMatOff = "dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos driver";
				getInDir = "pos driver dir";
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
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo3
			{
				actionSel = "seat_cargo3";
				proxyPos = "crewCargo3";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo4
			{
				actionSel = "seat_cargo4";
				proxyPos = "crewCargo4";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo5
			{
				actionSel = "seat_cargo5";
				proxyPos = "crewCargo5";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo6
			{
				actionSel = "seat_cargo6";
				proxyPos = "crewCargo6";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo7
			{
				actionSel = "seat_cargo7";
				proxyPos = "crewCargo7";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo8
			{
				actionSel = "seat_cargo8";
				proxyPos = "crewCargo8";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo9
			{
				actionSel = "seat_cargo9";
				proxyPos = "crewCargo9";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo10
			{
				actionSel = "seat_cargo10";
				proxyPos = "crewCargo10";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo11
			{
				actionSel = "seat_cargo11";
				proxyPos = "crewCargo11";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo12
			{
				actionSel = "seat_cargo12";
				proxyPos = "crewCargo12";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo13
			{
				actionSel = "seat_cargo13";
				proxyPos = "crewCargo13";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo14
			{
				actionSel = "seat_cargo14";
				proxyPos = "crewCargo14";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo15
			{
				actionSel = "seat_cargo15";
				proxyPos = "crewCargo15";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo16
			{
				actionSel = "seat_cargo16";
				proxyPos = "crewCargo16";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo17
			{
				actionSel = "seat_cargo17";
				proxyPos = "crewCargo17";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo18
			{
				actionSel = "seat_cargo18";
				proxyPos = "crewCargo18";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo19
			{
				actionSel = "seat_cargo19";
				proxyPos = "crewCargo19";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo20
			{
				actionSel = "seat_cargo20";
				proxyPos = "crewCargo20";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragFrontTotal = 0.795;
			class Steering
			{
				increaseSpeed[] = {0,45,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
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
				steepness = 6;
				torqueMax = 134;
				torqueRpm = 3400;
				powerMax = 63.7;
				powerRpm = 5400;
				rpmIdle = 850;
				rpmMin = 900;
				rpmClutch = 1350;
				rpmRedline = 6000;
				rpmMax = 8000;
			};
			class Gearbox
			{
				reverse = 5.526;
				ratios[] = {5.667,2.5,1.861,1.3};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 660;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 30;
					finalRatio = 2.1;
					brakeBias = 0.4;
					brakeForce = 10000;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1700;
						stiffness = 45000;
						compression = 2100;
						damping = 8500;
						travelMaxUp = 0.0882;
						travelMaxDown = 0.0833;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "BusWheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "BusWheel_2_1";
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
					brakeBias = 0.6;
					brakeForce = 12000;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1800;
						stiffness = 45000;
						compression = 2200;
						damping = 8600;
						travelMaxUp = 0.1587;
						travelMaxDown = 0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "BusWheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "BusWheel_2_2";
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
			itemsCargoSize[] = {10,50};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1e-05;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.4002;
				animPeriod = 1e-05;
			};
			class damper_2_2: damper_1_2{};
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
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"TruckBattery","CarRadiator","GlowPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"BusWheel_1_1","BusWheel_1_2","BusWheel_2_1","BusWheel_2_2"};
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_blue_co.paa"};
		skinName = "Blue";
	};
	class ExpansionBus_Blue: ExpansionBus
	{
		skinBase = "ExpansionBus";
		skinName = "Blue";
	};
	class Vehicle_ExpansionBus_Blue: Vehicle_ExpansionBus
	{
		skinBase = "Vehicle_ExpansionBus";
		skinName = "Blue";
	};
	class ExpansionSpraycanWeeb: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Weeb_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Weeb_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Weeb";
	};
	class ExpansionBus_Weeb: ExpansionBus
	{
		skinBase = "ExpansionBus";
		skinName = "Weeb";
	};
	class Vehicle_ExpansionBus_Weeb: Vehicle_ExpansionBus
	{
		skinBase = "Vehicle_ExpansionBus";
		skinName = "Weeb";
	};
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_red_co.paa"};
		skinName = "Red";
	};
	class ExpansionBus_Red: ExpansionBus
	{
		skinBase = "ExpansionBus";
		skinName = "Red";
	};
	class Vehicle_ExpansionBus_Red: Vehicle_ExpansionBus
	{
		skinBase = "Vehicle_ExpansionBus";
		skinName = "Red";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class Proxymegaphone: ProxyVehiclePart
	{
		model = "\DZ\gear\radio\megaphone.p3d";
		inventorySlot[] = {"busmegaphone"};
	};
};
