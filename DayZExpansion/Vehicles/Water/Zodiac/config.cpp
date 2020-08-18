#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Water_Boat_Zodiac
	{
		units[] = {"ExpansionZodiacBoat","ExpansionSpraycanOrange","ExpansionZodiacBoat_Orange","ExpansionSpraycanBlack","ExpansionZodiacBoat_Black","ExpansionSpraycanGreen","ExpansionZodiacBoat_Green"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
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
	class ExpansionZodiacBoat: ExpansionBoatScript
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_ZODIAC";
		model = "\DayZExpansion\Vehicles\Water\Zodiac\zodiac.p3d";
		vehicleClass = "Expansion_Boat";
		fuelCapacity = 192;
		fuelConsumption = 21;
		attachments[] = {"CarRadiator","GlowPlug","TruckBattery"};
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Water\Zodiac\data\inflatable_boat_ca.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Vehicles\Water\Zodiac\data\zodiac.rvmat"};
		defaultSkin = "Orange";
		doors[] = {};
		class Cargo
		{
			itemsCargoSize[] = {10,15};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"GlowPlug","TruckBattery"};
				doors[] = {};
			};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_boat_utility_loop_Ext_SoundSet"};
			soundSetsInt[] = {"offroad_Wind_SoundSet"};
		};
		class SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragCoefficient = 0.995;
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
				reverse = 1.0;
				ratios[] = {1.0};
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
	class ExpansionSpraycanBase;
	class ExpansionSpraycanOrange: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Orange_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Orange_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_orange_co.paa"};
		skinName = "Orange";
	};
	class ExpansionZodiacBoat_Orange: ExpansionZodiacBoat
	{
		skinBase = "ExpansionZodiacBoat";
		skinName = "Orange";
	};
	class ExpansionSpraycanBlack: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Black_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Black_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Black";
	};
	class ExpansionZodiacBoat_Black: ExpansionZodiacBoat
	{
		skinBase = "ExpansionZodiacBoat";
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
	class ExpansionZodiacBoat_Green: ExpansionZodiacBoat
	{
		skinBase = "ExpansionZodiacBoat";
		skinName = "Green";
	};
};
