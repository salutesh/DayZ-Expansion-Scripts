#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Sedan_02
	{
		units[] = {"Expansion_Sedan_02_Wheel","Sedan_02_Wheel_Ruined","Expansion_Sedan_02","Expansion_Sedan_02_Red","Expansion_Sedan_02_Yellow","Expansion_Sedan_02_Grey"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class ExpansionWheel;
	class ExpansionCarScript;
	class CarDoor;
	class CarScript;
	class ExpansionCarDoorFix;
	class Crew;
	class Driver;
	class CoDriver;
	class VehicleSimulation;
	class Axles;
	class Front;
	class Rear;
	class Wheels;
	class Left;
	class Right;
	class AnimationSources;
	class Expansion_Sedan_02_Wheel: ExpansionWheel
	{
		scope = 2;
		displayName = "$STR_Sedan_02_Wheel0";
		descriptionShort = "$STR_Sedan_02_Wheel1";
		model = "\DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel.p3d";
		inventorySlot[] = {"Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2"};
		rotationFlags = 8;
		radiusByDamage[] = {0,0.33,0.3,0.29,0.9998,0.25,0.9999,0.21};
		radius = 0.33;
		friction = 0.96;
		width = 0.16;
		tyreRollResistance = 0.01;
		tyreTread = 0.95;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel_damaged.rvmat"}},{0.3,{"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel_damaged.rvmat"}},{0.0,{"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel_damaged.rvmat"}}};
				};
			};
		};
	};
	class Sedan_02_Wheel_Ruined: Expansion_Sedan_02_Wheel
	{
		model = "\DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel_ruined.p3d";
		radius = 0.22;
		friction = -1.0;
		width = 0.2;
		tyreRollResistance = 1;
		tyreRollDrag = 80;
		tyreRoughness = 1.2;
		tyreTread = 0.4;
	};
	class Expansion_Sedan_02: ExpansionCarScript
	{
		scope = 2;
		displayName = "TEST $STR_Sedan_020";
		model = "\DZ\vehicles\wheeled\Sedan_02\Sedan_02.p3d";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","EngineBelt","SparkPlug","Sedan_02_Hood","Sedan_02_Trunk","Sedan_02_Door_1_1","Sedan_02_Door_2_1","Sedan_02_Door_1_2","Sedan_02_Door_2_2","Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2"};
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\sedan_02\data\sedan_02_int2e.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\sedan_02\data\sedan_02_int2.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\sedan_02\data\sedan_02_chrome_e.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\sedan_02\data\sedan_02_chrome.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\sedan_02\data\sedan_02_lights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\sedan_02\data\sedan_02_lights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\sedan_02\data\sedan_02_lights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\sedan_02\data\sedan_02_lights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\sedan_02\data\sedan_02_lights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\sedan_02\data\sedan_02_lights.rvmat";
		fuelCapacity = 38;
		fuelConsumption = 7;
		class Crew: Crew
		{
			class Driver: Driver{};
			class CoDriver: CoDriver{};
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
		class VehicleSimulation: VehicleSimulation
		{
			drive = "DRIVE_RWD";
			airDragCoefficient = 0.73;
			class Steering
			{
				increaseSpeed[] = {0,45,60,25,100,10};
				decreaseSpeed[] = {0,80,60,40,100,15};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime = 0.8;
				defaultThrust = 0.9;
				gentleThrust = 0.8;
				turboCoef = 2.8;
				gentleCoef = 0.5;
			};
			braking[] = {0.0,0.2,0.5,0.4,2.5,0.6,3.0,1.0};
			class Engine
			{
				inertia = 0.23;
				torqueMax = 82;
				torqueRpm = 3000;
				powerMax = 37.0;
				powerRpm = 5000;
				rpmIdle = 900;
				rpmMin = 1000;
				rpmRedline = 5750;
				rpmMax = 7000;
				rpmClutch = 1500;
			};
			class Gearbox
			{
				reverse = 3.27;
				ratios[] = {3.8,2.12,1.41,0.96};
				timeToUncoupleClutch = 0.1;
				timeToCoupleClutch = 0.3;
				maxClutchTorque = 165;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 30;
					finalRatio = 4.1;
					brakeBias = 0.6;
					brakeForce = 3800;
					wheelHubMass = 5;
					wheelHubRadius = 0.17;
					class Suspension
					{
						swayBar = 800;
						stiffness = 25000;
						compression = 1600;
						damping = 5200;
						travelMaxUp = 0.07;
						travelMaxDown = 0.08;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_1";
							inventorySlot = "Sedan_02_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "Sedan_02_Wheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.22;
					brakeBias = 0.4;
					brakeForce = 3600;
					wheelHubMass = 5;
					wheelHubRadius = 0.17;
					class Suspension
					{
						swayBar = 600;
						stiffness = 21500;
						compression = 1800;
						damping = 5600;
						travelMaxUp = 0.1;
						travelMaxDown = 0.1;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "Sedan_02_Wheel_1_2";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "Sedan_02_Wheel_2_2";
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
			class DoorsCargo1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class DoorsCargo2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
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
					componentNames[] = {"dmgZone_front","dmgZone_bumper_1"};
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1"};
					transferToZonesCoefs[] = {0.7,0.7};
					inventorySlots[] = {"Sedan_02_Hood","CarRadiator","Sedan_02_Wheel_1_1","Sedan_02_Wheel_2_1"};
					inventorySlotsCoefs[] = {0.7,0.5,0.8,0.8};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_lights_1_1"};
					componentNames[] = {"dmgZone_lights_1_1"};
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\glass_i_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\sedan_02\data\glass_i_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Fender_1_1"};
					transferToZonesCoefs[] = {1.0,1.0};
					inventorySlots[] = {"Reflector_1_1","Sedan_02_Wheel_1_1"};
					inventorySlotsCoefs[] = {1.0,0.9};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1"};
					inventorySlots[] = {"Reflector_2_1","Sedan_02_Wheel_2_1"};
				};
				class Back
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_back"};
					componentNames[] = {"dmgZone_back"};
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Fender_1_2","Fender_2_2","Engine"};
					transferToZonesCoefs[] = {0.7,0.7,0.8};
					inventorySlots[] = {"Sedan_02_Trunk","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_2"};
					inventorySlotsCoefs[] = {1.0,0.9,0.9};
				};
				class Roof
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_roof"};
					componentNames[] = {"dmgZone_roof"};
					class Health
					{
						hitpoints = 600;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"}}};
					};
					inventorySlots[] = {};
				};
				class Fender_1_1
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_fender_1_1"};
					componentNames[] = {"dmgZone_fender_1_1"};
					class Health
					{
						hitpoints = 800;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.7,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Reflector_1_1"};
					transferToZonesCoefs[] = {0.3,0.6};
					inventorySlots[] = {"Sedan_02_Hood","Sedan_02_Wheel_1_1","Sedan_02_Door_1_1"};
					inventorySlotsCoefs[] = {0.6,0.9,0.8};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
					transferToZonesNames[] = {"Front","Reflector_2_1"};
					transferToZonesCoefs[] = {0.3,0.6};
					inventorySlots[] = {"Sedan_02_Hood","Sedan_02_Wheel_2_1","Sedan_02_Door_2_1"};
					inventorySlotsCoefs[] = {0.6,0.9,0.8};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_1_2"};
					componentNames[] = {"dmgZone_fender_1_2"};
					transferToZonesNames[] = {"Back","Engine","FuelTank"};
					transferToZonesCoefs[] = {0.7,0.4,0.7};
					inventorySlots[] = {"Sedan_02_Trunk","Sedan_02_Wheel_1_2","Sedan_02_Door_1_2"};
					inventorySlotsCoefs[] = {0.7,0.9,0.8};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_2"};
					componentNames[] = {"dmgZone_fender_2_2"};
					transferToZonesNames[] = {"Back","Engine","FuelTank"};
					transferToZonesCoefs[] = {0.7,0.4,0.7};
					inventorySlots[] = {"Sedan_02_Trunk","Sedan_02_Wheel_2_2","Sedan_02_Door_2_2"};
					inventorySlotsCoefs[] = {0.7,0.9,0.8};
				};
				class WindowFront
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowFront"};
					componentNames[] = {"dmgZone_windowFront"};
					class Health
					{
						hitpoints = 90;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowBack: WindowFront
				{
					componentNames[] = {"dmgZone_windowBack"};
					memoryPoints[] = {"dmgZone_windowBack"};
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\sedan_02_engine.rvmat"}},{0.7,{"dz\vehicles\wheeled\sedan_02\data\sedan_02_engine.rvmat"}},{0.5,{"dz\vehicles\wheeled\sedan_02\data\sedan_02_RUST_engine.rvmat"}},{0.3,{"dz\vehicles\wheeled\sedan_02\data\sedan_02_RUST_engine.rvmat"}},{0.0,{"dz\vehicles\wheeled\sedan_02\data\sedan_02_RUST_engine.rvmat"}}};
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
						hitpoints = 400;
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
						center[] = {0,0,0.8};
					};
					class Cyl3
					{
						radius = 1.0;
						height = 1.5;
						center[] = {0,0,-0.8};
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
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","Sedan_02_Door_1_1","Sedan_02_Door_2_1","Sedan_02_Door_1_2","Sedan_02_Door_2_2","Sedan_02_Hood","Sedan_02_Trunk"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2"};
			};
		};
	};
	class Sedan_02: CarScript
	{
		doors[] = {"Sedan_02_Door_1_1","Sedan_02_Door_1_2","Sedan_02_Door_2_1","Sedan_02_Door_2_2","Sedan_02_Trunk","Sedan_02_Hood"};
		defaultSkin = "Yellow";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","camo_chassi","dmgZone_chassis","dmgZone_back","dmgZone_front","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat","dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"};
	};
	class Sedan_02_Door_1_1: CarDoor
	{
		applySkinsTo[] = {"Sedan_02_Door_2_1","Expansion_Sedan_02_Door_2_1"};
		defaultSkin = "Yellow";
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class Sedan_02_Door_2_1: CarDoor
	{
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class Sedan_02_Door_1_2: CarDoor
	{
		applySkinsTo[] = {"Sedan_02_Door_2_2","Expansion_Sedan_02_Door_2_2"};
		defaultSkin = "Yellow";
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class Sedan_02_Door_2_2: CarDoor
	{
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class Sedan_02_Hood: CarDoor
	{
		defaultSkin = "Yellow";
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
	class Sedan_02_Trunk: CarDoor
	{
		defaultSkin = "Yellow";
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
	class Expansion_Sedan_02_Door_1_1: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_1_1";
	};
	class Expansion_Sedan_02_Door_1_2: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_1_2";
	};
	class Expansion_Sedan_02_Door_2_1: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_2_1";
	};
	class Expansion_Sedan_02_Door_2_2: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Door_2_2";
	};
	class Expansion_Sedan_02_Hood: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Hood";
	};
	class Expansion_Sedan_02_Trunk: ExpansionCarDoorFix
	{
		newBase = "Sedan_02_Trunk";
	};
	class Sedan_02_Red: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "Red";
	};
	class Expansion_Sedan_02_Red: Expansion_Sedan_02
	{
		skinBase = "Expansion_Sedan_02";
		skinName = "Red";
	};
	class Sedan_02_Door_1_1_Red: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "Red";
	};
	class Sedan_02_Door_1_2_Red: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "Red";
	};
	class Sedan_02_Door_2_1_Red: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "Red";
	};
	class Sedan_02_Door_2_2_Red: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "Red";
	};
	class Sedan_02_Hood_Red: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "Red";
	};
	class Sedan_02_Trunk_Red: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "Red";
	};
	class Sedan_02_Yellow: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "Yellow";
	};
	class Expansion_Sedan_02_Yellow: Expansion_Sedan_02
	{
		skinBase = "Expansion_Sedan_02";
		skinName = "Yellow";
	};
	class Sedan_02_Door_1_1_Yellow: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "Yellow";
	};
	class Sedan_02_Door_1_2_Yellow: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "Yellow";
	};
	class Sedan_02_Door_2_1_Yellow: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "Yellow";
	};
	class Sedan_02_Door_2_2_Yellow: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "Yellow";
	};
	class Sedan_02_Hood_Yellow: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "Yellow";
	};
	class Sedan_02_Trunk_Yellow: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "Yellow";
	};
	class Sedan_02_Grey: Sedan_02
	{
		skinBase = "Sedan_02";
		skinName = "Grey";
	};
	class Expansion_Sedan_02_Grey: Expansion_Sedan_02
	{
		skinBase = "Expansion_Sedan_02";
		skinName = "Grey";
	};
	class Sedan_02_Door_1_1_Grey: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "Grey";
	};
	class Sedan_02_Door_1_2_Grey: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "Grey";
	};
	class Sedan_02_Door_2_1_Grey: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "Grey";
	};
	class Sedan_02_Door_2_2_Grey: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "Grey";
	};
	class Sedan_02_Hood_Grey: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "Grey";
	};
	class Sedan_02_Trunk_Grey: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "Grey";
	};
	class Sedan_02_Door_1_1_RedRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "RedRust";
	};
	class Sedan_02_Door_1_2_RedRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "RedRust";
	};
	class Sedan_02_Door_2_1_RedRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "RedRust";
	};
	class Sedan_02_Door_2_2_RedRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "RedRust";
	};
	class Sedan_02_Hood_RedRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "RedRust";
	};
	class Sedan_02_Trunk_RedRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "RedRust";
	};
	class Sedan_02_Door_1_1_YellowRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_1_2_YellowRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_2_1_YellowRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_2_2_YellowRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "YellowRust";
	};
	class Sedan_02_Hood_YellowRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "YellowRust";
	};
	class Sedan_02_Trunk_YellowRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "YellowRust";
	};
	class Sedan_02_Door_1_1_GreyRust: Sedan_02_Door_1_1
	{
		skinBase = "Sedan_02_Door_1_1";
		skinName = "GreyRust";
	};
	class Sedan_02_Door_1_2_GreyRust: Sedan_02_Door_1_2
	{
		skinBase = "Sedan_02_Door_1_2";
		skinName = "GreyRust";
	};
	class Sedan_02_Door_2_1_GreyRust: Sedan_02_Door_2_1
	{
		skinBase = "Sedan_02_Door_2_1";
		skinName = "GreyRust";
	};
	class Sedan_02_Door_2_2_GreyRust: Sedan_02_Door_2_2
	{
		skinBase = "Sedan_02_Door_2_2";
		skinName = "GreyRust";
	};
	class Sedan_02_Hood_GreyRust: Sedan_02_Hood
	{
		skinBase = "Sedan_02_Hood";
		skinName = "GreyRust";
	};
	class Sedan_02_Trunk_GreyRust: Sedan_02_Trunk
	{
		skinBase = "Sedan_02_Trunk";
		skinName = "GreyRust";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxySedan_02_Wheel: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel.p3d";
		inventorySlot[] = {"Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2"};
	};
	class ProxySedan_02_Wheel_ruined: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel_ruined.p3d";
		inventorySlot[] = {"Sedan_02_Wheel_1_1","Sedan_02_Wheel_1_2","Sedan_02_Wheel_2_1","Sedan_02_Wheel_2_2"};
	};
};
