#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Water_Boat_Utility
	{
		units[] = {"ExpansionUtilityBoat","ExpansionSpraycanBlack","ExpansionUtilityBoat_Black","ExpansionSpraycanGreen","ExpansionUtilityBoat_Green"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class CarScript;
	class CarWheel;
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
	class ExpansionBoatScript;
	class OffroadHatchback;
	class HatchbackWheel;
	class ExpansionUtilityBoat: ExpansionBoatScript
	{
		scope = 2;
		hornSoundSetEXT = "Expansion_Utility_Horn_Ext_SoundSet";
		hornSoundSetINT = "Expansion_Utility_Horn_Ext_SoundSet";
		displayname = "$STR_EXPANSION_VEHICLE_UTILITY";
		model = "\DayZExpansion\Vehicles\Water\Utility\RHIB.p3d";
		modelZeroPointDistanceFromGround = 0.63;
		vehicleClass = "Expansion_Boat";
		fuelCapacity = 192;
		fuelConsumption = 21;
		doors[] = {};
		hiddenSelections[] = {"light_1_1","light_2_1","camo","antiwater"};
		hiddenSelectionsTextures[] = {"","","DayZExpansion\Vehicles\Water\Utility\data\rhib_co.paa",""};
		hiddenSelectionsMaterials[] = {"","","DayZExpansion\Vehicles\Water\Utility\data\rhib.rvmat",""};
		frontReflectorMatOn = "dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		frontReflectorMatOff = "dayzexpansion\vehicles\water\utility\data\rhib.rvmat";
		attachments[] = {"Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug","TruckBattery"};
		class Cargo
		{
			itemsCargoSize[] = {10,50};
			allowOwnedCargoManipulation = 1;
			openable = 0;
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
				getInPos = "pos cargo";
				getInDir = "pos cargo dir";
			};
			class Cargo1
			{
				actionSel = "seat_cargo1";
				proxyPos = "crewCargo1";
				getInPos = "pos cargo";
				getInDir = "pos cargo dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos cargo";
				getInDir = "pos cargo dir";
			};
			class Cargo3
			{
				actionSel = "seat_cargo3";
				proxyPos = "crewCargo3";
				getInPos = "pos cargo";
				getInDir = "pos cargo dir";
			};
		};
		class AnimationSources
		{
			class drivingWheel
			{
				source = "user";
				animPeriod = 2;
				initPhase = 0;
			};
			class antiwater
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
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
				reactionTime = 1.0;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 4.0;
				gentleCoef = 0.75;
			};
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
			braking[] = {0.0,0.1,1.0,0.8,3.0,0.9,3.5,1.0};
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
					class Wheels
					{
						class Left
						{
							animDamper = "damper_1_1";
							inventorySlot = "NivaWheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "NivaWheel_1_1";
							animTurn = "turnfrontright";
							animRotation = "wheelfrontright";
							wheelHub = "wheel_2_1_damper_land";
						};
					};
				};
				class Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.4;
					brakeForce = 0;
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
					class Wheels
					{
						class Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "NivaWheel_1_1";
							animTurn = "wheelbackleft";
							animRotation = "wheelbackleft";
							wheelHub = "wheel_2_1_damper_land";
						};
						class Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "NivaWheel_1_1";
							animTurn = "turnbackright";
							animRotation = "wheelbackright";
							wheelHub = "wheel_2_1_damper_land";
						};
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
				attachmentSlots[] = {"GlowPlug","TruckBattery"};
			};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_Utility_Engine_Ext_SoundSet","offroad_Engine_Offload_Ext_Rpm2_SoundSet","offroad_Engine_Offload_Ext_Rpm3_SoundSet","offroad_Engine_Offload_Ext_Rpm4_SoundSet","offroad_Engine_Offload_Ext_Rpm5_SoundSet","Expansion_Utility_Idle_Ext_SoundSet","offroad_Engine_Ext_Rpm1_SoundSet","offroad_Engine_Ext_Rpm2_SoundSet","offroad_Engine_Ext_Rpm3_SoundSet","offroad_Engine_Ext_Rpm4_SoundSet","offroad_Engine_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Broken_SoundSet","offroad_Tires_rock_slow_Ext_SoundSet","offroad_Tires_rock_fast_Ext_SoundSet","offroad_Tires_grass_slow_Ext_SoundSet","offroad_Tires_grass_fast_Ext_SoundSet","offroad_Tires_gravel_slow_Ext_SoundSet","offroad_Tires_gravel_fast_Ext_SoundSet","offroad_Tires_gravel_dust_fast_Ext_SoundSet","offroad_Tires_asphalt_slow_Ext_SoundSet","offroad_Tires_asphalt_fast_Ext_SoundSet","offroad_Tires_water_slow_Ext_SoundSet","offroad_Tires_water_fast_Ext_SoundSet","Offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_Rain_Ext_SoundSet","offroad_damper_left_SoundSet","offroad_damper_right_SoundSet"};
			soundSetsInt[] = {"Offroad_Tires_Asphalt_Fast_General_Int_SoundSet","Offroad_Wind_SoundSet"};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanBlack: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Black_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Black_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Black";
	};
	class ExpansionUtilityBoat_Black: ExpansionUtilityBoat
	{
		skinBase = "ExpansionUtilityBoat";
		skinName = "Black";
	};
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionUtilityBoat_Green: ExpansionUtilityBoat
	{
		skinBase = "ExpansionUtilityBoat";
		skinName = "Green";
	};
};
