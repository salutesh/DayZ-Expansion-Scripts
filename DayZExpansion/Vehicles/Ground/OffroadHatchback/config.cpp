#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Hatchback
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class ExpansionWheelBase;
	class ExpansionVehicleCarBase;
	class CarDoor;
	class CarScript;
	class ExpansionAttachments;
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
	class GUIInventoryAttachmentsProps;
	class Body;
	class Vehicle_HatchbackWheel: ExpansionWheelBase
	{
		scope = 0;
		displayName = "$STR_HatchbackWheel0";
		descriptionShort = "$STR_hatchbackwheel1";
		model = "\DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaWheel.p3d";
		weight = 15000;
		inventorySlot[] = {"NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","NivaWheel_Spare_1"};
		rotationFlags = 4;
		repairableWithKits[] = {6};
		repairCosts[] = {30.0};
		radiusByDamage[] = {0,0.362,0.3,0.3,0.9998,0.25,0.9999,0.2};
		radius = 0.35;
		width = 0.18;
		tyreRollResistance = 0.015;
		tyreTread = 0.8;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"}},{0.3,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"}},{0.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"}}};
				};
			};
		};
	};
	class Vehicle_HatchbackWheel_Ruined: Vehicle_HatchbackWheel
	{
		scope = 0;
		displayName = "$STR_HatchbackWheel0";
		descriptionShort = "$STR_hatchbackwheel1";
		model = "\DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaWheel_destroyed.p3d";
		weight = 10000;
		inventorySlot[] = {"NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","NivaWheel_Spare_1"};
		rotationFlags = 4;
		radius = 0.2;
		width = 0.17;
		tyreRollResistance = 1;
		tyreRollDrag = 70;
		tyreRoughness = 1;
		tyreTread = 0.5;
	};
	class Vehicle_OffroadHatchback: ExpansionVehicleCarBase
	{
		scope = 0;
		displayname = "$STR_OffroadHatchback0";
		descriptionShort = "$STR_OffroadHatchback1";
		model = "\dz\vehicles\wheeled\OffroadHatchback\OffroadHatchback.p3d";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","NivaDriverDoors","NivaCoDriverDoors","NivaHood","NivaTrunk","NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","NivaWheel_Spare_1"};
		hiddenSelections[] = {"light_left","light_right","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\offroadhatchback\data\niva_dashboard_light.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\offroadhatchback\data\niva_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\niva_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\OffroadHatchback_lights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\OffroadHatchback_lights.rvmat";
		doors[] = {"NivaDriverDoors","NivaCoDriverDoors","NivaTrunk","NivaHood"};
		defaultSkin = "Green";
		fuelCapacity = 42;
		fuelConsumption = 11;
		class Crew: Crew
		{
			class Driver: Driver{};
			class CoDriver: CoDriver{};
			class Cargo1
			{
				actionSel = "seat_cargo1";
				proxyPos = "crewCargo1";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_coDriver";
				getInDir = "pos_coDriver_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle = 30;
				increaseSpeed[] = {0,40,30,20,100,5};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime = 0.9;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 4;
				gentleCoef = 0.75;
			};
			class Brake
			{
				pressureBySpeed[] = {0,0.85,10,0.7,20,0.5,40,0.4,60,0.43,80,0.46,100,0.52,120,0.7};
				reactionTime = 0.3;
				driverless = 0.1;
			};
			class Aerodynamics
			{
				frontalArea = 2.18;
				dragCoefficient = 0.56;
			};
			drive = "DRIVE_AWD";
			class Engine
			{
				torqueCurve[] = {650,0,750,40,1400,80,3400,114,5400,95,8000,0};
				inertia = 0.11;
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
				maxTorqueTransfer = 240;
				uncoupleTime = 0.3;
				coupleTime = 0.45;
			};
			class Gearbox
			{
				type = "GEARBOX_MANUAL";
				reverse = 3.526;
				ratios[] = {3.667,2.1,1.361,1};
			};
			class CentralDifferential
			{
				ratio = 1.5;
				type = "DIFFERENTIAL_LOCKED";
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque = 2000;
					maxHandbrakeTorque = 2500;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Differential
					{
						ratio = 4.1;
						type = "DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
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
							inventorySlot = "NivaWheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "NivaWheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque = 1000;
					maxHandbrakeTorque = 2500;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Differential
					{
						ratio = 4.1;
						type = "DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
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
							animDamper = "damper_1_2";
							inventorySlot = "NivaWheel_1_2";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "NivaWheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,30};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class AnimationSources: AnimationSources
		{
			class SeatDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.8;
			};
			class SeatCoDriver
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
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_chassis"};
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
					};
					inventorySlots[] = {};
				};
				class Front
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					class Health
					{
						hitpoints = 1200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.7,0.7,0.8};
					inventorySlots[] = {"NivaHood","CarRadiator","NivaWheel_1_1","NivaWheel_2_1"};
					inventorySlotsCoefs[] = {0.7,0.5,0.8,0.8};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_lights_1_1"};
					memoryPoints[] = {"dmgZone_lights_1_1"};
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Fender_1_1"};
					transferToZonesCoefs[] = {1.0,1.0};
					inventorySlots[] = {"Reflector_1_1","NivaWheel_1_1"};
					inventorySlotsCoefs[] = {1.0,0.9};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1"};
					inventorySlots[] = {"Reflector_2_1","NivaWheel_2_1"};
				};
				class Back
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Fender_1_2","Fender_2_2","WindowLR","WindowRR"};
					transferToZonesCoefs[] = {0.3,0.3,0.2,0.2};
					inventorySlots[] = {"NivaTrunk","NivaWheel_1_2","NivaWheel_2_2"};
					inventorySlotsCoefs[] = {0.9,0.9,0.9};
				};
				class Roof
				{
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_roof"};
					memoryPoints[] = {"dmgZone_roof"};
					class Health
					{
						hitpoints = 700;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_destruct.rvmat"}}};
					};
					inventorySlotsCoefs[] = {};
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_fender_1_1"};
					memoryPoints[] = {"dmgZone_fender_1_1"};
					class Health
					{
						hitpoints = 1200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\green\niva_body_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Reflector_1_1","Engine"};
					transferToZonesCoefs[] = {0.3,0.6,0.4};
					inventorySlots[] = {"NivaHood","NivaWheel_1_1","NivaDriverDoors"};
					inventorySlotsCoefs[] = {0.6,0.9,0.3};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
					transferToZonesNames[] = {"Front","Reflector_2_1","Engine"};
					transferToZonesCoefs[] = {0.3,0.6,0.4};
					inventorySlots[] = {"NivaHood","NivaWheel_2_1","NivaCoDriverDoors"};
					inventorySlotsCoefs[] = {0.6,0.9,0.3};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_1_2"};
					componentNames[] = {"dmgZone_fender_1_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowLR"};
					transferToZonesCoefs[] = {0.7,0.7,0.7};
					inventorySlots[] = {"NivaTrunk","NivaWheel_1_2","NivaDriverDoors"};
					inventorySlotsCoefs[] = {0.7,0.9,0.3};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_2"};
					componentNames[] = {"dmgZone_fender_2_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowRR"};
					transferToZonesCoefs[] = {0.7,0.7,0.7};
					inventorySlots[] = {"NivaTrunk","NivaWheel_2_2","NivaCoDriverDoors"};
					inventorySlotsCoefs[] = {0.7,0.9,0.3};
				};
				class WindowFront
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowFront"};
					componentNames[] = {"dmgZone_windowFront"};
					class Health
					{
						hitpoints = 120;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowLR
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowLeft"};
					componentNames[] = {"dmgZone_windowLeft"};
					class Health
					{
						hitpoints = 150;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowRR: WindowLR
				{
					memoryPoints[] = {"dmgZone_windowRight"};
					componentNames[] = {"dmgZone_windowRight"};
				};
				class Engine
				{
					fatalInjuryCoef = 0.001;
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}}};
					};
					inventorySlots[] = {"CarBattery","SparkPlug","CarRadiator"};
					inventorySlotsCoefs[] = {0.2,0.2,0.4};
				};
				class FuelTank
				{
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_fuelTank"};
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
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
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","NivaHood","NivaTrunk","NivaDriverDoors","NivaCoDriverDoors","NivaWheel_Spare_1"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2"};
			};
		};
	};
	class OffroadHatchback: CarScript
	{
		hiddenSelections[] = {"light_left","light_right","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat","dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\offroadhatchback\data\niva_dashboard_light.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\offroadhatchback\data\niva_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\niva_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\green\niva_body.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\offroadhatchback\data\OffroadHatchback_lights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\OffroadHatchback_lights.rvmat";
		fuelCapacity = 42;
		fuelConsumption = 11;
		doors[] = {"NivaDriverDoors","NivaCoDriverDoors","NivaTrunk","NivaHood"};
		defaultSkin = "Green";
		attachments[] += {"CamoNet","KeyChain"};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Body: Body
			{
				attachmentSlots[] += {"CamoNet","KeyChain"};
			};
		};
		class ExpansionAttachments: ExpansionAttachments
		{
			class NivaDriverDoors
			{
				selection = "doors_driver";
				animation = "DoorsDriver";
				isDoor = 1;
			};
			class NivaCoDriverDoors
			{
				selection = "doors_codriver";
				animation = "DoorsCoDriver";
				isDoor = 1;
			};
			class NivaTrunk
			{
				selection = "doors_trunk";
				animation = "DoorsTrunk";
				isDoor = 1;
			};
			class NivaHood
			{
				selection = "doors_hood";
				animation = "DoorsHood";
				isDoor = 1;
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
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
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_chassis"};
					inventorySlots[] = {};
				};
				class Front
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine","WindowFront"};
					transferToZonesThreshold[] = {0.5,0.5,0.5,0.1};
					transferToZonesCoefs[] = {0.7,0.7,0.5,0.05};
					inventorySlots[] = {"NivaHood","CarRadiator"};
					inventorySlotsCoefs[] = {0.3,0.3};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_lights_1_1"};
					componentNames[] = {"dmgZone_lights_1_1"};
					transferToZonesNames[] = {"Front","Fender_1_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Reflector_2_1: Reflector_1_1
				{
					transferToZonesNames[] = {"Front","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					inventorySlots[] = {"Reflector_2_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Back
				{
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					transferToZonesNames[] = {"Fender_1_2","Fender_2_2","WindowLR","WindowRR"};
					transferToZonesCoefs[] = {0.3,0.3,0.2,0.2};
					inventorySlots[] = {"NivaTrunk"};
					inventorySlotsCoefs[] = {0.8};
				};
				class Roof
				{
					class Health
					{
						hitpoints = 700;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_roof"};
					componentNames[] = {"dmgZone_roof"};
					inventorySlotsCoefs[] = {0.0};
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints = 1200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_fender_1_1"};
					componentNames[] = {"dmgZone_fender_1_1"};
					transferToZonesNames[] = {"Front","Engine","Reflector_1_1","WindowFront"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4};
					inventorySlots[] = {"NivaHood","NivaWheel_1_1","NivaDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.3,0.3};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_1_2"};
					componentNames[] = {"dmgZone_fender_1_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowLR"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlots[] = {"NivaTrunk","NivaWheel_1_2"};
					inventorySlotsCoefs[] = {0.3,0.1};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
					transferToZonesNames[] = {"Front","Engine","Reflector_2_1","WindowFront","Battery"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4,0.7};
					inventorySlots[] = {"NivaHood","NivaWheel_2_1","NivaCoDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.2,3};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_2"};
					componentNames[] = {"dmgZone_fender_2_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowRR"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlots[] = {"NivaTrunk","NivaWheel_2_2","NivaCoDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.1};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 150;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowFront"};
					componentNames[] = {"dmgZone_windowFront"};
					inventorySlots[] = {};
				};
				class WindowLR
				{
					class Health
					{
						hitpoints = 150;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowLeft"};
					componentNames[] = {"dmgZone_windowLeft"};
					inventorySlots[] = {};
				};
				class WindowRR: WindowLR
				{
					memoryPoints[] = {"dmgZone_windowRight"};
					componentNames[] = {"dmgZone_windowRight"};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.3,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"}}};
					};
					fatalInjuryCoef = 0.001;
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					inventorySlots[] = {"CarBattery","SparkPlug","EngineBelt","CarRadiator"};
					inventorySlotsCoefs[] = {0.2,0.2,0.01,0.4};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 600;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_fuelTank"};
					inventorySlots[] = {};
				};
			};
		};
	};
	class HatchbackDoors_Driver: CarDoor
	{
		defaultSkin = "Green";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class HatchbackDoors_CoDriver: HatchbackDoors_Driver
	{
		defaultSkin = "Green";
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
				class Window
				{
					class Health
					{
						hitpoints = 70;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i.rvmat"}},{0.7,{"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.5,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"}},{0.3,{"hidden","dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"}},{0.0,"hidden","hidden"}};
					};
					componentNames[] = {"dmgZone_window"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class HatchbackHood: CarDoor
	{
		defaultSkin = "Green";
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
				class Doors
				{
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{""}}};
					};
					componentNames[] = {"dmgZone_doors"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class HatchbackTrunk: CarDoor
	{
		defaultSkin = "Green";
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
		};
	};
	class OffroadHatchback_Green: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "Green";
	};
	class Vehicle_OffroadHatchback_Green: Vehicle_OffroadHatchback
	{
		skinBase = "Vehicle_OffroadHatchback";
		skinName = "Green";
	};
	class HatchbackDoors_Driver_Green: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "Green";
	};
	class HatchbackDoors_CoDriver_Green: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "Green";
	};
	class HatchbackHood_Green: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "Green";
	};
	class HatchbackTrunk_Green: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "Green";
	};
	class OffroadHatchback_Blue: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "Blue";
	};
	class Vehicle_OffroadHatchback_Blue: Vehicle_OffroadHatchback
	{
		skinBase = "Vehicle_OffroadHatchback";
		skinName = "Blue";
	};
	class HatchbackDoors_Driver_Blue: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "Blue";
	};
	class HatchbackDoors_CoDriver_Blue: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "Blue";
	};
	class HatchbackHood_Blue: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "Blue";
	};
	class HatchbackTrunk_Blue: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "Blue";
	};
	class OffroadHatchback_White: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "White";
	};
	class Vehicle_OffroadHatchback_White: Vehicle_OffroadHatchback
	{
		skinBase = "Vehicle_OffroadHatchback";
		skinName = "White";
	};
	class HatchbackDoors_Driver_White: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "White";
	};
	class HatchbackDoors_CoDriver_White: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "White";
	};
	class HatchbackHood_White: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "White";
	};
	class HatchbackTrunk_White: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "White";
	};
	class OffroadHatchback_Police: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "Police";
	};
	class Vehicle_OffroadHatchback_Police: Vehicle_OffroadHatchback
	{
		skinBase = "Vehicle_OffroadHatchback";
		skinName = "Police";
	};
	class HatchbackDoors_Driver_Police: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "Police";
	};
	class HatchbackDoors_CoDriver_Police: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "Police";
	};
	class HatchbackHood_Police: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "Police";
	};
	class HatchbackTrunk_Police: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "Police";
	};
	class OffroadHatchback_GreenRust: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "GreenRust";
	};
	class Vehicle_OffroadHatchback_GreenRust: Vehicle_OffroadHatchback
	{
		skinBase = "Vehicle_OffroadHatchback";
		skinName = "GreenRust";
	};
	class HatchbackDoors_Driver_GreenRust: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "GreenRust";
	};
	class HatchbackDoors_CoDriver_GreenRust: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "GreenRust";
	};
	class HatchbackHood_GreenRust: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "GreenRust";
	};
	class HatchbackTrunk_GreenRust: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "GreenRust";
	};
	class OffroadHatchback_BlueRust: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "BlueRust";
	};
	class Vehicle_OffroadHatchback_BlueRust: Vehicle_OffroadHatchback
	{
		skinBase = "Vehicle_OffroadHatchback";
		skinName = "BlueRust";
	};
	class HatchbackDoors_Driver_BlueRust: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "BlueRust";
	};
	class HatchbackDoors_CoDriver_BlueRust: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "BlueRust";
	};
	class HatchbackHood_BlueRust: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "BlueRust";
	};
	class HatchbackTrunk_BlueRust: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "BlueRust";
	};
	class OffroadHatchback_WhiteRust: OffroadHatchback
	{
		skinBase = "OffroadHatchback";
		skinName = "WhiteRust";
	};
	class Vehicle_OffroadHatchback_WhiteRust: Vehicle_OffroadHatchback
	{
		skinBase = "Vehicle_OffroadHatchback";
		skinName = "WhiteRust";
	};
	class HatchbackDoors_Driver_WhiteRust: HatchbackDoors_Driver
	{
		skinBase = "HatchbackDoors_Driver";
		skinName = "WhiteRust";
	};
	class HatchbackDoors_CoDriver_WhiteRust: HatchbackDoors_CoDriver
	{
		skinBase = "HatchbackDoors_CoDriver";
		skinName = "WhiteRust";
	};
	class HatchbackHood_WhiteRust: HatchbackHood
	{
		skinBase = "HatchbackHood";
		skinName = "WhiteRust";
	};
	class HatchbackTrunk_WhiteRust: HatchbackTrunk
	{
		skinBase = "HatchbackTrunk";
		skinName = "WhiteRust";
	};
};
