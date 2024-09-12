#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Tractor
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgSlots
{
	class Slot_ExpansionTractorFrontWheel_1_1
	{
		name = "ExpansionTractorFrontWheel_1_1";
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT_DESC";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_ExpansionTractorFrontWheel_2_1
	{
		name = "ExpansionTractorFrontWheel_2_1";
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT_DESC";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_ExpansionTractorBackWheel_1_2
	{
		name = "ExpansionTractorBackWheel_1_2";
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK_DESC";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_ExpansionTractorBackWheel_2_2
	{
		name = "ExpansionTractorBackWheel_2_2";
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK_DESC";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_ExpansionTractorDoorsDriver
	{
		name = "ExpansionTractorDoorsDriver";
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_DOOR_LEFT";
		selection = "ExpansionTractorDoorsDriver";
		ghostIcon = "doorfront";
	};
	class Slot_ExpansionTractorDoorsCodriver
	{
		name = "ExpansionTractorDoorsCodriver";
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_DOOR_RIGHT";
		selection = "ExpansionTractorDoorsCodriver";
		ghostIcon = "doorfront";
	};
};
class CfgVehicles
{
	class ExpansionWheelBase;
	class ExpansionVehicleCarBase;
	class CarDoor;
	class CarScript;
	class CarWheel;
	class Crew;
	class Driver;
	class CoDriver;
	class SimulationModule;
	class Axles;
	class Front;
	class Rear;
	class Wheels;
	class Left;
	class Right;
	class AnimationSources;
	class Truck_01_WheelDouble;
	class CivSedanDoors_Driver;
	class ExpansionTractorFrontWheel: CarWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorFrontWheel.p3d";
		inventorySlot[] = {"ExpansionTractorFrontWheel_1_1","ExpansionTractorFrontWheel_2_1"};
		rotationFlags = 4;
		repairableWithKits[] = {6};
		repairCosts[] = {30.0};
		physLayer = "item_large";
		itemSize[] = {6,6};
		weight = 35000;
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.55;
		width = 0.211;
		tyreRollResistance = 0.015;
		tyreTread = 1.5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{"dayzexpansion\vehicles\ground\tractor\data\traktor_2.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionTractorFrontWheel_Ruined: ExpansionTractorFrontWheel
	{
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\tractor2_front_wheel_destroyed.p3d";
		radius = 0.62;
		width = 0.45;
		tyreRollResistance = 0.015;
		friction = 0.96;
		tyreTread = 1.5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
	class ExpansionTractorBackWheel: CarWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorBackWheel.p3d";
		inventorySlot[] = {"ExpansionTractorBackWheel_1_2","ExpansionTractorBackWheel_2_2"};
		rotationFlags = 4;
		repairableWithKits[] = {6};
		repairCosts[] = {30.0};
		physLayer = "item_large";
		itemSize[] = {8,8};
		weight = 50000;
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.718;
		width = 0.012;
		tyreRollResistance = 0.015;
		tyreTread = 1.5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class drop
				{
					soundset = "hatchbackwheel_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class ExpansionTractorBackWheel_Ruined: ExpansionTractorBackWheel
	{
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\tractor2_back_wheel_destroyed.p3d";
		radius = 0.793;
		width = 0.55;
		tyreRollResistance = 0.015;
		friction = 0.96;
		tyreTread = 1.5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
	class Vehicle_ExpansionTractorFrontWheel: ExpansionWheelBase
	{
		scope = 0;
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_FRONT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorFrontWheel.p3d";
		inventorySlot[] = {"ExpansionTractorFrontWheel_1_1","ExpansionTractorFrontWheel_2_1"};
		rotationFlags = 4;
		itemSize[] = {6,6};
		weight = 35000;
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.62;
		width = 0.45;
		tyreRollResistance = 0.015;
		friction = 0.96;
		tyreTread = 1.5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
	class Vehicle_ExpansionTractorBackWheel: ExpansionWheelBase
	{
		scope = 0;
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_WHEEL_BACK_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorBackWheel.p3d";
		inventorySlot[] = {"ExpansionTractorBackWheel_1_2","ExpansionTractorBackWheel_2_2"};
		rotationFlags = 4;
		itemSize[] = {8,8};
		weight = 50000;
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.793;
		width = 0.55;
		tyreRollResistance = 0.015;
		friction = 0.96;
		tyreTread = 1.5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
	class ExpansionTractorDoorsDriver: CivSedanDoors_Driver
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_DOOR_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_DOOR_LEFT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorDoorsDriver.p3d";
		weight = 25000;
		itemSize[] = {6,6};
		itemBehaviour = 0;
		inventorySlot = "ExpansionTractorDoorsDriver";
		rotationFlags = 8;
		physLayer = "item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUp
				{
					soundSet = "hatchbackdoors_driver_pickup_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "hatchbackdoors_driver_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class ExpansionTractorDoorsCodriver: ExpansionTractorDoorsDriver
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR_DOOR_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_TRACTOR_DOOR_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorDoorsCodriver.p3d";
		inventorySlot = "ExpansionTractorDoorsCodriver";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionTractor: CarScript
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_TRACTOR";
		model = "\DayZExpansion\Vehicles\Ground\Tractor\Tractor2.p3d";
		modelZeroPointDistanceFromGround = 0.0;
		attachments[] = {"ExpansionTractorFrontWheel_1_1","ExpansionTractorFrontWheel_2_1","ExpansionTractorBackWheel_1_2","ExpansionTractorBackWheel_2_2","ExpansionTractorDoorsDriver","ExpansionTractorDoorsCodriver","CarBattery","SparkPlug","Reflector_1_1","Reflector_2_1","CamoNet","KeyChain"};
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","camo","dmgZone_front","dmgZone_back","dmgZone_roof"};
		hiddenSelectionsTextures[] = {"","","","","","","","","DayZExpansion\Vehicles\Ground\Tractor\Data\tractor_2_co.paa","","",""};
		hiddenSelectionsMaterials[] = {"","","","","","","","","DayZExpansion\Vehicles\Ground\Tractor\Data\traktor_2.rvmat","DayZExpansion\Vehicles\Ground\Tractor\Data\traktor_2.rvmat","DayZExpansion\Vehicles\Ground\Tractor\Data\traktor_2.rvmat","DayZExpansion\Vehicles\Ground\Tractor\Data\traktor_2.rvmat"};
		doors[] = {"ExpansionTractorDoorsDriver","ExpansionTractorDoorsCodriver"};
		applySkinsTo[] = {"ExpansionTractorDoorsDriver","ExpansionTractorDoorsCodriver"};
		defaultSkin = "Red";
		fuelCapacity = 67;
		fuelConsumption = 13;
		batterySlot = "CarBattery";
		dashboardMatOn = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff = "DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff = "DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff = "DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat";
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			class Steering
			{
				maxSteeringAngle = 30;
				increaseSpeed[] = {0,25,30,11,50,6};
				decreaseSpeed[] = {0,40,30,20,55,10};
				centeringSpeed[] = {0,0,7,12,30,20,50,30};
			};
			class Throttle
			{
				reactionTime = 1.3;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 10;
				gentleCoef = 0.75;
			};
			class Brake
			{
				pressureBySpeed[] = {0,0.5,10,0.46,30,0.43,40,0.4,60,0.5,80,0.6};
				reactionTime = 0.3;
				driverless = 0.1;
			};
			class Aerodynamics
			{
				frontalArea = 5.75;
				dragCoefficient = 0.53;
			};
			class Engine
			{
				torqueCurve[] = {650,0,750,53,1400,105,3400,150,5400,130,8000,0};
				inertia = 0.15;
				frictionTorque = 100;
				rollingFriction = 0.5;
				viscousFriction = 0.5;
				rpmIdle = 800;
				rpmMin = 900;
				rpmClutch = 1400;
				rpmRedline = 6000;
			};
			class Clutch
			{
				maxTorqueTransfer = 560;
				uncoupleTime = 0.15;
				coupleTime = 0.15;
			};
			class Gearbox
			{
				type = "GEARBOX_MANUAL";
				reverse = 6.526;
				ratios[] = {6.6670003,3.25};
			};
			class CentralDifferential
			{
				ratio = 1.45;
				type = "DIFFERENTIAL_LOCKED";
			};
			class Axles: Axles
			{
				class Front: Front
				{
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					maxBrakeTorque = 5000;
					maxHandbrakeTorque = 6000;
					class Differential
					{
						ratio = 8.35;
						type = "DIFFERENTIAL_LOCKED";
					};
					class Suspension
					{
						stiffness = 55000;
						compression = 2100;
						damping = 7500;
						travelMaxUp = 0.0882;
						travelMaxDown = 0.0833;
					};
					class Wheels
					{
						class Left: Left
						{
							inventorySlot = "ExpansionTractorFrontWheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "ExpansionTractorFrontWheel_2_1";
							animTurn = "turnfrontright";
							animRotation = "wheelfrontright";
							animDamper = "damper_2_1";
							wheelHub = "wheel_2_1_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					maxBrakeTorque = 5000;
					maxHandbrakeTorque = 6000;
					class Differential
					{
						ratio = 8.35;
						type = "DIFFERENTIAL_LOCKED";
					};
					class Suspension
					{
						stiffness = 60000;
						compression = 2200;
						damping = 7600;
						travelMaxUp = 0.1587;
						travelMaxDown = 0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "ExpansionTractorBackWheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "ExpansionTractorBackWheel_2_2";
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
			itemsCargoSize[] = {7,10};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class AnimationSources: AnimationSources
		{
			class ExpansionTractorDoorsDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.8;
			};
			class ExpansionTractorDoorsCodriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.8;
			};
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
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Truck_01_Engine_Offload_Ext_Rpm1_SoundSet","Truck_01_Engine_Offload_Ext_Rpm2_SoundSet","Truck_01_Engine_Offload_Ext_Rpm3_SoundSet","Truck_01_Engine_Offload_Ext_Rpm4_SoundSet","Truck_01_Engine_Ext_Rpm0_SoundSet","Truck_01_Engine_Ext_Rpm1_SoundSet","Truck_01_Engine_Ext_Rpm2_SoundSet","Truck_01_Engine_Ext_Rpm3_SoundSet","Truck_01_Engine_Ext_Rpm4_SoundSet","Truck_01_Engine_Ext_Broken_SoundSet","Truck_01_Tires_rock_slow_Ext_SoundSet","Truck_01_Tires_rock_fast_Ext_SoundSet","Truck_01_Tires_ice_slow_Ext_SoundSet","Truck_01_Tires_ice_fast_Ext_SoundSet","Truck_01_Tires_snow_slow_Ext_SoundSet","Truck_01_Tires_snow_fast_Ext_SoundSet","Truck_01_Tires_grass_slow_Ext_SoundSet","Truck_01_Tires_grass_fast_Ext_SoundSet","Truck_01_Tires_gravel_slow_Ext_SoundSet","Truck_01_Tires_gravel_fast_Ext_SoundSet","Truck_01_Tires_asphalt_slow_Ext_SoundSet","Truck_01_Tires_asphalt_fast_Ext_SoundSet","Truck_01_Tires_water_slow_Ext_SoundSet","Truck_01_Tires_water_fast_Ext_SoundSet","offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_damper_left_SoundSet","offroad_damper_right_SoundSet","Truck_01_Rain_Ext_SoundSet"};
			soundSetsInt[] = {"Truck_01_Tires_Asphalt_Slow_Ext_SoundSet","Truck_01_Wind_SoundSet","Truck_01_Rattle_Int_SoundSet"};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 2000;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
					};
					componentNames[] = {"dmgzone_chassis"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Front
				{
					class Health
					{
						hitpoints = 2000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					transferToZonesNames[] = {"Engine","Reflector_1_1","Reflector_2_1"};
					transferToZonesThreshold[] = {0.25,0.3,0.3};
					transferToZonesCoefs[] = {0.2,0.3,0.3};
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {0.3,0.3,0.3};
					inventorySlots[] = {"Slot_ExpansionTractorFrontWheel_1_1","Slot_ExpansionTractorFrontWheel_2_1","Reflector_1_1","Reflector_2_1"};
				};
				class Back
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					transferToZonesNames[] = {"WindowBack"};
					transferToZonesCoefs[] = {0.3};
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {};
					inventorySlots[] = {};
				};
				class Roof
				{
					class Health
					{
						hitpoints = 700;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					memoryPoints[] = {"dmgZone_roof"};
					componentNames[] = {"dmgZone_roof"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 100;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgZone_window_front"};
					componentNames[] = {"window_front"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class WindowBack
				{
					class Health
					{
						hitpoints = 100;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgZone_window_back"};
					componentNames[] = {"window_back"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class WindowFLR
				{
					class Health
					{
						hitpoints = 100;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgZone_window_front_left"};
					componentNames[] = {"window_front_left"};
				};
				class WindowFRR
				{
					class Health
					{
						hitpoints = 100;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgZone_window_front_right"};
					componentNames[] = {"window_front_right"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class WindowLR
				{
					class Health
					{
						hitpoints = 100;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgZone_window_door_left"};
					componentNames[] = {"window_door_left"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class WindowRR
				{
					class Health
					{
						hitpoints = 100;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_skla.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					memoryPoints[] = {"dmgZone_window_door_right"};
					componentNames[] = {"window_door_right"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}}};
					};
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					fatalInjuryCoef = 0.001;
					inventorySlotsCoefs[] = {0.2,0.2};
					inventorySlots[] = {"CarBattery","SparkPlug","FuelTank"};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					transferToZonesNames[] = {"Front","Engine"};
					transferToZonesCoefs[] = {0.1,0.15};
					memoryPoints[] = {"dmgZone_lights_1_1"};
					componentNames[] = {"dmgZone_lights_1_1"};
					fatalInjuryCoef = -1;
					inventorySlotsCoefs[] = {1.0};
					inventorySlots[] = {"Reflector_1_1"};
				};
				class Reflector_2_1: Reflector_1_1
				{
					transferToZonesNames[] = {"Front","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					inventorySlotsCoefs[] = {1.0};
					inventorySlots[] = {"Reflector_2_1"};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 600;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2.rvmat"}},{0.7,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.5,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.3,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}},{0.0,{"DayZExpansion\Vehicles\Ground\Tractor\data\traktor_2_destruct.rvmat"}}};
					};
					componentNames[] = {"dmgZone_fuelTank"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
		class ObstacleGenerator
		{
			carve = 1;
			timeToStationary = 5.0;
			moveThreshold = 0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius = 1.0;
						height = 1.5;
						center[] = {0,0,0.7};
					};
					class Cyl3
					{
						radius = 1.0;
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
				attachmentSlots[] = {"CarBattery","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"ExpansionTractorDoorsDriver","ExpansionTractorDoorsCodriver","Reflector_1_1","Reflector_2_1","CamoNet","KeyChain"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"ExpansionTractorFrontWheel_1_1","ExpansionTractorFrontWheel_2_1","ExpansionTractorBackWheel_1_2","ExpansionTractorBackWheel_2_2"};
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_red_co.paa"};
		skinName = "Red";
	};
	class ExpansionTractor_Red: ExpansionTractor
	{
		skinBase = "ExpansionTractor";
		skinName = "Red";
	};
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_blue_co.paa"};
		skinName = "Blue";
	};
	class ExpansionTractor_Blue: ExpansionTractor
	{
		skinBase = "ExpansionTractor";
		skinName = "Blue";
	};
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionTractor_Green: ExpansionTractor
	{
		skinBase = "ExpansionTractor";
		skinName = "Green";
	};
};
class CfgDestroy
{
	class EngineHit
	{
		sound[] = {};
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
	class ProxyExpansionTractorFrontWheel: ProxyVehiclePart
	{
		model = "DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorFrontWheel.p3d";
		inventorySlot[] = {"ExpansionTractorFrontWheel_1_1","ExpansionTractorFrontWheel_2_1"};
	};
	class ProxyExpansionTractorFrontWheel_destroyed: ProxyVehiclePart
	{
		model = "DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorFrontWheel.p3d";
		inventorySlot[] = {"ExpansionTractorFrontWheel_1_1","ExpansionTractorFrontWheel_2_1"};
	};
	class ProxyExpansionTractorBackWheel: ProxyVehiclePart
	{
		model = "DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorBackWheel.p3d";
		inventorySlot[] = {"ExpansionTractorBackWheel_1_2","ExpansionTractorBackWheel_2_2"};
	};
	class ProxyExpansionTractorBackWheel_destroyed: ProxyVehiclePart
	{
		model = "DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorBackWheel.p3d";
		inventorySlot[] = {"ExpansionTractorBackWheel_1_2","ExpansionTractorBackWheel_2_2"};
	};
	class ProxyExpansionTractorDoorsDriver: ProxyVehiclePart
	{
		model = "DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorDoorsDriver.p3d";
		inventorySlot = "ExpansionTractorDoorsDriver";
	};
	class ProxyExpansionTractorDoorsCodriver: ProxyVehiclePart
	{
		model = "DayZExpansion\Vehicles\Ground\Tractor\proxy\ExpansionTractorDoorsCodriver.p3d";
		inventorySlot = "ExpansionTractorDoorsCodriver";
	};
	class ProxyCarBattery: ProxyVehiclePart
	{
		model = "\DZ\vehicles\parts\battery_car.p3d";
		inventorySlot[] = {"CarBattery"};
	};
};
