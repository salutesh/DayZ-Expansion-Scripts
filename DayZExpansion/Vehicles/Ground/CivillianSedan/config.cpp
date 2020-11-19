#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Sedan_02
	{
		units[] = {"Vehicle_CivSedanWheel","Vehicle_CivSedanWheel_Ruined","Vehicle_CivilianSedan","Vehicle_CivilianSedan_White","Vehicle_CivilianSedan_Wine","Vehicle_CivilianSedan_Black","Vehicle_CivilianSedan_Police","Vehicle_CivilianSedan_Lime"};
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
	class ExpansionCarDoorFix;
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
	class Vehicle_CivSedanWheel: ExpansionWheelBase
	{
		scope = 2;
		displayName = "$STR_CivSedanWheel0";
		descriptionShort = "$STR_CivSedanWheel1";
		model = "\DZ\vehicles\wheeled\civiliansedan\proxy\sedanwheel.p3d";
		inventorySlot[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2"};
		rotationFlags = 4;
		radiusByDamage[] = {0,0.34,0.3,0.3,0.9998,0.25,0.9999,0.2};
		radius = 0.34;
		friction = 0.96;
		width = 0.16;
		tyreRollResistance = 0.015;
		tyreTread = 1.0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel_damage.rvmat"}},{0.3,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel_damage.rvmat"}},{0.0,{"DZ\vehicles\wheeled\civiliansedan\data\gaz_wheel_destruct.rvmat"}}};
				};
			};
		};
	};
	class Vehicle_CivSedanWheel_Ruined: Vehicle_CivSedanWheel
	{
		scope = 2;
		displayName = "$STR_CivSedanWheel0";
		model = "\DZ\vehicles\wheeled\civiliansedan\proxy\sedanwheel_destroyed.p3d";
		radius = 0.238;
		width = 0.2;
		tyreRollResistance = 1;
		tyreRollDrag = 75;
		tyreRoughness = 1.2;
		tyreTread = 0.5;
	};
	class Vehicle_CivilianSedan: ExpansionVehicleCarBase
	{
		scope = 2;
		displayName = "$STR_CivilianSedan0";
		descriptionShort = "$STR_CivilianSedan1";
		model = "\DZ\vehicles\wheeled\civiliansedan\CivilianSedan.p3d";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","CivSedanDriverDoors","CivSedanCoDriverDoors","CivSedanCargo1Doors","CivSedanCargo2Doors","CivSedanHood","CivSedanTrunk","CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2"};
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat","dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan_int2e_.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan_int2e_off.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		fuelCapacity = 55;
		fuelConsumption = 13;
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
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_RWD";
			airDragFrontTotal = 0.928;
			class Steering
			{
				increaseSpeed[] = {0,50,30,40,60,25,120,5};
				decreaseSpeed[] = {0,50,60,30,120,10};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime = 0.25;
				defaultThrust = 0.8;
				gentleThrust = 0.6;
				turboCoef = 5.0;
				gentleCoef = 0.7;
			};
			braking[] = {0.0,0.1,1.0,0.8,3.0,0.9,3.5,1.0};
			class Engine
			{
				inertia = 0.45;
				torqueMax = 187;
				torqueRpm = 2400;
				powerMax = 73.5;
				powerRpm = 4500;
				rpmIdle = 1000;
				rpmMin = 1050;
				rpmClutch = 1250;
				rpmRedline = 4700;
				rpmMax = 6800;
			};
			class Gearbox
			{
				reverse = 3.51;
				ratios[] = {3.5,2.26,1.45,1.0};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.3;
				maxClutchTorque = 180;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 35;
					brakeBias = 0.7;
					brakeForce = 4250;
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
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_1";
							inventorySlot = "CivSedanWheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "CivSedanWheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle = 0;
					brakeBias = 0.3;
					brakeForce = 4500;
					finalRatio = 4.1;
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
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "CivSedanWheel_1_2";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "CivSedanWheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,40};
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
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1e-06;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.5936;
				animPeriod = 1e-06;
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
						hitpoints = 1500;
						transferToGlobalCoef = 0;
						RefTexsMats[] = {"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat"};
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat"}},{0.7,{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat"}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.7,0.7,0.8};
					inventorySlots[] = {"CivSedanHood","CarRadiator","CivSedanWheel_1_1","CivSedanWheel_2_1"};
					inventorySlotsCoefs[] = {0.7,0.5,0.7,0.7};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_lights_1_1"};
					componentNames[] = {"dmgZone_lights_1_1"};
					class Health
					{
						hitpoints = 20;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Fender_1_1"};
					transferToZonesCoefs[] = {1.0,1.0};
					inventorySlots[] = {"Reflector_1_1","CivSedanWheel_1_1"};
					inventorySlotsCoefs[] = {1.0,0.9};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1"};
					inventorySlots[] = {"Reflector_2_1","CivSedanWheel_2_1"};
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
						RefTexsMats[] = {"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat"};
					};
					transferToZonesNames[] = {"Fender_1_2","Fender_2_2"};
					transferToZonesCoefs[] = {0.7,0.7};
					inventorySlots[] = {"CivSedanTrunk","CivSedanWheel_1_2","CivSedanWheel_2_2"};
					inventorySlotsCoefs[] = {1.0,0.9,0.9};
				};
				class Roof
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_roof"};
					componentNames[] = {"dmgZone_roof"};
					class Health
					{
						hitpoints = 700;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"}},{0.7,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan_destruct.rvmat"}}};
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
						hitpoints = 1200;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"}},{0.7,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Reflector_1_1","Engine"};
					transferToZonesCoefs[] = {0.3,0.6,0.4};
					inventorySlots[] = {"CivSedanHood","CivSedanWheel_1_1","CivSedanDriverDoors"};
					inventorySlotsCoefs[] = {0.6,0.9,0.3};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
					transferToZonesNames[] = {"Front","Reflector_2_1","Engine"};
					transferToZonesCoefs[] = {0.3,0.6,0.4};
					inventorySlots[] = {"CivSedanHood","CivSedanWheel_2_1","CivSedanCoDriverDoors"};
					inventorySlotsCoefs[] = {0.6,0.9,0.3};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_1_2"};
					componentNames[] = {"dmgZone_fender_1_2"};
					transferToZonesNames[] = {"Back","FuelTank"};
					transferToZonesCoefs[] = {0.7,0.7};
					inventorySlots[] = {"CivSedanTrunk","CivSedanWheel_1_2","CivSedanCargo1Doors"};
					inventorySlotsCoefs[] = {0.7,0.9,0.3};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_2"};
					componentNames[] = {"dmgZone_fender_2_2"};
					transferToZonesNames[] = {"Back","FuelTank"};
					transferToZonesCoefs[] = {0.7,0.7};
					inventorySlots[] = {"CivSedanTrunk","CivSedanWheel_2_2","CivSedanCargo1Doors"};
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
						hitpoints = 600;
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
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","CivSedanHood","CivSedanTrunk","CivSedanDriverDoors","CivSedanCoDriverDoors","CivSedanCargo1Doors","CivSedanCargo2Doors"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2"};
			};
		};
	};
	class CivilianSedan: CarScript
	{
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","","","","","","","",""};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","DZ\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat","DZ\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat","dz\vehicles\wheeled\civiliansedan\data\white\civiliansedan.rvmat"};
		doors[] = {"CivSedanDriverDoors","CivSedanCoDriverDoors","CivSedanCargo1Doors","CivSedanCargo2Doors","CivSedanTrunk","CivSedanHood"};
		applySkinsTo[] = {"CivSedanDoors_Driver","CivSedanDoors_CoDriver","CivSedanDoors_BackLeft","CivSedanDoors_BackRight","CivSedanHood","CivSedanTrunk","Expansion_CivSedanDoors_Driver","Expansion_CivSedanDoors_CoDriver","Expansion_CivSedanDoors_BackLeft","Expansion_CivSedanDoors_BackRight","Expansion_CivSedanHood","Expansion_CivSedanTrunk"};
		defaultSkin = "White";
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
					componentNames[] = {"dmgZone_chassis"};
					fatalInjuryCoef = -1;
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
					inventorySlots[] = {"CivSedanHood","CarRadiator"};
					inventorySlotsCoefs[] = {0.3,0.3};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints = 10;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_damage.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"},{"dz\vehicles\wheeled\civiliansedan\data\civiliansedan_cr_destruct.rvmat"}}};
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
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.1,0.15,0.05};
					inventorySlotsCoefs[] = {1.0};
					inventorySlots[] = {"Reflector_2_1"};
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
					transferToZonesNames[] = {"Fender_1_2","Fender_2_2","WindowBack"};
					transferToZonesCoefs[] = {0.3,0.3,0.2};
					inventorySlots[] = {"CivSedanTrunk"};
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
					transferToZonesNames[] = {"Front","Engine","Reflector_1_1","WindowFront"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4};
					memoryPoints[] = {"dmgZone_fender_1_1"};
					componentNames[] = {"dmgZone_fender_1_1"};
					inventorySlots[] = {"CivSedanHood","CivSedanWheel_1_1","CivSedanDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.3,0.3};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_1_2"};
					componentNames[] = {"dmgZone_fender_1_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowBack"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlotsCoefs[] = {0.3,0.1};
					inventorySlots[] = {"CivSedanTrunk","CivSedanWheel_1_2"};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
					transferToZonesNames[] = {"Front","Engine","Reflector_2_1","WindowFront","Battery"};
					transferToZonesThreshold[] = {0.2,0.5,0.9,0.5,0.5};
					transferToZonesCoefs[] = {0.3,0.6,0.6,0.4,0.7};
					inventorySlots[] = {"CivSedanHood","CivSedanWheel_2_1","CivSedanDriverDoors"};
					inventorySlotsCoefs[] = {0.3,0.2,31};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_2"};
					componentNames[] = {"dmgZone_fender_2_2"};
					transferToZonesNames[] = {"Back","FuelTank","WindowBack"};
					transferToZonesThreshold[] = {0.2,0.2,0.2};
					transferToZonesCoefs[] = {0.3,0.2,0.7};
					inventorySlots[] = {"CivSedanTrunk","CivSedanWheel_2_2"};
					inventorySlotsCoefs[] = {0.3,0.1};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints = 150;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowFront"};
					componentNames[] = {"dmgZone_windowFront"};
					inventorySlots[] = {};
				};
				class WindowBack: WindowFront
				{
					componentNames[] = {"dmgZone_windowBack"};
					memoryPoints[] = {"dmgZone_windowBack"};
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
					componentNames[] = {"dmgZone_fuelTank"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
				};
			};
		};
	};
	class CivSedanDoors_Driver: CarDoor
	{
		defaultSkin = "White";
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class CivSedanDoors_CoDriver: CarDoor
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class CivSedanDoors_BackLeft: CarDoor
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class CivSedanDoors_BackRight: CarDoor
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
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\civiliansedan\data\glass.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\civiliansedan\data\glass_destruct.rvmat","dz\vehicles\wheeled\civiliansedan\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
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
	class CivSedanHood: CarDoor
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
		};
	};
	class CivSedanTrunk: CarDoor
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
		};
	};
	class Expansion_CivSedanDoors_Driver: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_Driver";
	};
	class Expansion_CivSedanDoors_CoDriver: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_CoDriver";
	};
	class Expansion_CivSedanDoors_BackLeft: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_BackLeft";
	};
	class Expansion_CivSedanDoors_BackRight: ExpansionCarDoorFix
	{
		newBase = "CivSedanDoors_BackRight";
	};
	class Expansion_CivSedanHood: ExpansionCarDoorFix
	{
		newBase = "CivSedanHood";
	};
	class Expansion_CivSedanTrunk: ExpansionCarDoorFix
	{
		newBase = "CivSedanTrunk";
	};
	class CivilianSedan_White: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "White";
	};
	class Vehicle_CivilianSedan_White: Vehicle_CivilianSedan
	{
		skinBase = "Vehicle_CivilianSedan";
		skinName = "White";
	};
	class CivSedanDoors_Driver_White: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "White";
	};
	class CivSedanDoors_CoDriver_White: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "White";
	};
	class CivSedanDoors_BackLeft_White: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "White";
	};
	class CivSedanDoors_BackRight_White: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "White";
	};
	class CivSedanHood_White: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "White";
	};
	class CivSedanTrunk_White: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "White";
	};
	class CivilianSedan_Wine: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Wine";
	};
	class Vehicle_CivilianSedan_Wine: Vehicle_CivilianSedan
	{
		skinBase = "Vehicle_CivilianSedan";
		skinName = "Wine";
	};
	class CivSedanDoors_Driver_Wine: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Wine";
	};
	class CivSedanDoors_CoDriver_Wine: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Wine";
	};
	class CivSedanDoors_BackLeft_Wine: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Wine";
	};
	class CivSedanDoors_BackRight_Wine: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Wine";
	};
	class CivSedanHood_Wine: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Wine";
	};
	class CivSedanTrunk_Wine: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Wine";
	};
	class CivilianSedan_Black: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Black";
	};
	class Vehicle_CivilianSedan_Black: Vehicle_CivilianSedan
	{
		skinBase = "Vehicle_CivilianSedan";
		skinName = "Black";
	};
	class CivSedanDoors_Driver_Black: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Black";
	};
	class CivSedanDoors_CoDriver_Black: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Black";
	};
	class CivSedanDoors_BackLeft_Black: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Black";
	};
	class CivSedanDoors_BackRight_Black: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Black";
	};
	class CivSedanHood_Black: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Black";
	};
	class CivSedanTrunk_Black: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Black";
	};
	class CivilianSedan_Police: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Police";
	};
	class Vehicle_CivilianSedan_Police: Vehicle_CivilianSedan
	{
		skinBase = "Vehicle_CivilianSedan";
		skinName = "Police";
	};
	class CivSedanDoors_Driver_Police: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Police";
	};
	class CivSedanDoors_CoDriver_Police: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Police";
	};
	class CivSedanDoors_BackLeft_Police: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Police";
	};
	class CivSedanDoors_BackRight_Police: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Police";
	};
	class CivSedanHood_Police: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Police";
	};
	class CivSedanTrunk_Police: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Police";
	};
	class CivilianSedan_Lime: CivilianSedan
	{
		skinBase = "CivilianSedan";
		skinName = "Lime";
	};
	class Vehicle_CivilianSedan_Lime: Vehicle_CivilianSedan
	{
		skinBase = "Vehicle_CivilianSedan";
		skinName = "Lime";
	};
	class CivSedanDoors_Driver_Lime: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "Lime";
	};
	class CivSedanDoors_CoDriver_Lime: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "Lime";
	};
	class CivSedanDoors_BackLeft_Lime: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "Lime";
	};
	class CivSedanDoors_BackRight_Lime: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "Lime";
	};
	class CivSedanHood_Lime: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "Lime";
	};
	class CivSedanTrunk_Lime: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "Lime";
	};
	class CivSedanDoors_Driver_WhiteRust: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_CoDriver_WhiteRust: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_BackLeft_WhiteRust: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_BackRight_WhiteRust: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "WhiteRust";
	};
	class CivSedanHood_WhiteRust: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "WhiteRust";
	};
	class CivSedanTrunk_WhiteRust: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "WhiteRust";
	};
	class CivSedanDoors_Driver_WineRust: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "WineRust";
	};
	class CivSedanDoors_CoDriver_WineRust: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "WineRust";
	};
	class CivSedanDoors_BackLeft_WineRust: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "WineRust";
	};
	class CivSedanDoors_BackRight_WineRust: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "WineRust";
	};
	class CivSedanHood_WineRust: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "WineRust";
	};
	class CivSedanTrunk_WineRust: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "WineRust";
	};
	class CivSedanDoors_Driver_BlackRust: CivSedanDoors_Driver
	{
		skinBase = "CivSedanDoors_Driver";
		skinName = "BlackRust";
	};
	class CivSedanDoors_CoDriver_BlackRust: CivSedanDoors_CoDriver
	{
		skinBase = "CivSedanDoors_CoDriver";
		skinName = "BlackRust";
	};
	class CivSedanDoors_BackLeft_BlackRust: CivSedanDoors_BackLeft
	{
		skinBase = "CivSedanDoors_BackLeft";
		skinName = "BlackRust";
	};
	class CivSedanDoors_BackRight_BlackRust: CivSedanDoors_BackRight
	{
		skinBase = "CivSedanDoors_BackRight";
		skinName = "BlackRust";
	};
	class CivSedanHood_BlackRust: CivSedanHood
	{
		skinBase = "CivSedanHood";
		skinName = "BlackRust";
	};
	class CivSedanTrunk_BlackRust: CivSedanTrunk
	{
		skinBase = "CivSedanTrunk";
		skinName = "BlackRust";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxySedanWheel: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\civiliansedan\proxy\sedanWheel.p3d";
		inventorySlot[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2"};
	};
	class ProxySedanWheel_destroyed: ProxyVehiclePart
	{
		model = "DZ\vehicles\wheeled\civiliansedan\proxy\sedanWheel_destroyed.p3d";
		inventorySlot[] = {"CivSedanWheel_1_1","CivSedanWheel_1_2","CivSedanWheel_2_1","CivSedanWheel_2_2"};
	};
};
