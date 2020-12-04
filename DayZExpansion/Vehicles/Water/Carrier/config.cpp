#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Water_Carrier
	{
		units[] = {"ExpansionLHD","Vehicle_ExpansionLHD","ExpansionLHD1","ExpansionLHD2","ExpansionLHD3","ExpansionLHD4","ExpansionLHD5","ExpansionLHD6","ExpansionLHDHouse1","ExpansionLHDHouse2","ExpansionLHDElevR","bldr_expansion_lhd1","bldr_expansion_lhd2","bldr_expansion_lhd3","bldr_expansion_lhd4","bldr_expansion_lhd5","bldr_expansion_lhd6","bldr_expansion_evelv_r","bldr_expansion_house1","bldr_expansion_house2","bldr_expansion_shadow","bldr_expansion_wall_lamp","bldr_expansion_lhd_evelv_r","bldr_expansion_lhd_house1","bldr_expansion_lhd_house2","bldr_expansion_lhd_shadow","bldr_expansion_lhd_wall_lamp"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
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
	class Vehicle_ExpansionLHD: ExpansionVehicleBoatBase
	{
		scope = 2;
		hornSoundSetEXT = "Expansion_LHD_Horn_Ext_SoundSet";
		hornSoundSetINT = "Expansion_LHD_Horn_Int_SoundSet";
		displayname = "[NOT READY]  LHD";
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD.p3d";
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
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_house_1.p3d";
	};
	class ExpansionLHDHouse2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_house_2.p3d";
	};
	class ExpansionLHDElevR: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\LHD_elev_R.p3d";
	};
	class bldr_expansion_lhd: HouseNoDestruct
	{
		scope = 0;
	};
	class bldr_expansion_lhd1: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_1.p3d";
	};
	class bldr_expansion_lhd2: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_2.p3d";
	};
	class bldr_expansion_lhd3: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_3.p3d";
	};
	class bldr_expansion_lhd4: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_4.p3d";
	};
	class bldr_expansion_lhd5: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_5.p3d";
	};
	class bldr_expansion_lhd6: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_6.p3d";
	};
	class bldr_expansion_evelv_r: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_elev_R.p3d";
	};
	class bldr_expansion_house1: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_house_1.p3d";
	};
	class bldr_expansion_house2: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_house_2.p3d";
	};
	class bldr_expansion_shadow: bldr_expansion_lhd
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Water\Carrier\bldr\LHD_shadow.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_expansion_wall_lamp: bldr_expansion_lhd
	{
		scope = 2;
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
