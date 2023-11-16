#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Water_Carrier
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class SimulationModule;
	class Axles;
	class Front;
	class Wheels;
	class Rear;
	class Left;
	class Right;
	class DamageSystem;
	class DamageZones;
	class GlobalHealth;
	class HouseNoDestruct;
	class ExpansionBoatScript;
	class ExpansionVehicleBoatBase;
	class ExpansionLHD: ExpansionBoatScript
	{
		scope = 2;
		hornSoundSetEXT = "Expansion_LHD_Horn_Ext_SoundSet";
		hornSoundSetINT = "Expansion_LHD_Horn_Int_SoundSet";
		displayname = "$STR_EXPANSION_VEHICLE_LHD";
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD.p3d";
		modelZeroPointDistanceFromGround = 12.1;
		vehicleClass = "Expansion_Boat";
		fuelCapacity = 192;
		fuelConsumption = 21;
		attachments[] = {"Reflector_1_1","Reflector_2_1","GlowPlug","TruckBattery","NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","CamoNet","KeyChain"};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"GlowPlug","TruckBattery"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","CamoNet","KeyChain"};
			};
		};
		class Crew
		{
			class Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			class Steering
			{
				maxSteeringAngle = 30;
				increaseSpeed[] = {0,45,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
			};
			class Engine
			{
				torqueCurve[] = {650,0,750,40,1400,80,3400,114,5400,95,8000,0};
				inertia = 0.15;
				frictionTorque = 100;
				rollingFriction = 0.5;
				viscousFriction = 0.5;
				rpmIdle = 850;
				rpmMin = 900;
				rpmClutch = 1350;
				rpmRedline = 6000;
			};
			class Gearbox
			{
				type = "GEARBOX_MANUAL";
				reverse = 3.526;
				ratios[] = {3.667,2.1,1.361,1.0};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000000;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 500000;
						transferToGlobalCoef = 0;
					};
					fatalInjuryCoef = -1;
					componentNames[] = {};
					inventorySlots[] = {};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 250000;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = 0.001;
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints = 160000;
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
	class Vehicle_ExpansionLHD: ExpansionVehicleBoatBase
	{
		scope = 0;
		displayname = "$STR_EXPANSION_VEHICLE_LHD";
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD.p3d";
		modelZeroPointDistanceFromGround = 12.1;
		hornSoundSetEXT = "Expansion_LHD_Horn_Ext_SoundSet";
		hornSoundSetINT = "Expansion_LHD_Horn_Int_SoundSet";
		vehicleClass = "Expansion_Boat";
		fuelCapacity = 192;
		fuelConsumption = 21;
		attachments[] = {"Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug","TruckBattery"};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"GlowPlug","TruckBattery"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
			};
		};
		class SimulationModule
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
	};
	class ExpansionLHD1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_1.p3d";
	};
	class ExpansionLHD2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_2.p3d";
	};
	class ExpansionLHD3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_3.p3d";
	};
	class ExpansionLHD4: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_4.p3d";
	};
	class ExpansionLHD5: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_5.p3d";
	};
	class ExpansionLHD6: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_6.p3d";
	};
	class ExpansionLHDHouse1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_House_1.p3d";
	};
	class ExpansionLHDHouse2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_House_2.p3d";
	};
	class ExpansionLHDElevatorL: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_Elevator_L.p3d";
	};
	class ExpansionLHDElevatorR: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_Elevator_R.p3d";
	};
	class ExpansionLHDInterior1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_Interior_1.p3d";
	};
	class ExpansionLHDInterior2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_Interior_2.p3d";
	};
	class ExpansionLHDInterior3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_Interior_3.p3d";
	};
	class bldr_expansion_lhd: HouseNoDestruct
	{
		scope = 0;
	};
	class bldr_expansion_lhd1: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_1.p3d";
	};
	class bldr_expansion_lhd2: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_2.p3d";
	};
	class bldr_expansion_lhd3: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_3.p3d";
	};
	class bldr_expansion_lhd4: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_4.p3d";
	};
	class bldr_expansion_lhd5: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_5.p3d";
	};
	class bldr_expansion_lhd6: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_6.p3d";
	};
	class bldr_expansion_evelv_r: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_elev_R.p3d";
	};
	class bldr_expansion_house1: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_house_1.p3d";
	};
	class bldr_expansion_house2: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_house_2.p3d";
	};
	class bldr_expansion_shadow: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_shadow.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_expansion_wall_lamp: bldr_expansion_lhd
	{
		scope = 1;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\Misc_wall_lamp.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_expansion_lhd_evelv_r: bldr_expansion_evelv_r{};
	class bldr_expansion_lhd_house1: bldr_expansion_house1{};
	class bldr_expansion_lhd_house2: bldr_expansion_house2{};
	class bldr_expansion_lhd_shadow: bldr_expansion_shadow{};
	class bldr_expansion_lhd_wall_lamp: bldr_expansion_wall_lamp{};
};
