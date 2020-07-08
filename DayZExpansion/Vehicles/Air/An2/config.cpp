#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Air_An2
	{
		units[] = {"ExpansionAnWheel","ExpansionAnWheelSmall","ExpansionAn2","ExpansionSpraycanGreen","ExpansionAn2_Green","ExpansionSpraycanWhite","ExpansionAn2_White","ExpansionSpraycanOrange","ExpansionAn2_Orange"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core"};
	};
};
class CfgSlots
{
	class Slot_expansion_anwheel_1_1
	{
		name = "expansion_anwheel_1_1";
		displayName = "An Wheel";
		descriptionShort = "";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_anwheel_2_1
	{
		name = "expansion_anwheel_2_1";
		displayName = "An Wheel";
		descriptionShort = "";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_ansmallwheel_1_2
	{
		name = "expansion_ansmallwheel_1_2";
		displayName = "An Wheel";
		descriptionShort = "";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_expansion_ansmallwheel_2_2
	{
		name = "expansion_ansmallwheel_2_2";
		displayName = "An Wheel";
		descriptionShort = "";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
};
class CfgVehicles
{
	class CarScript;
	class OffroadHatchback;
	class Inventory_Base;
	class SimulationModule;
	class Axles;
	class Wheels;
	class Front;
	class Rear;
	class Left;
	class Right;
	class AnimationSources;
	class Crew;
	class Driver;
	class CoDriver;
	class ExpansionPlaneScript;
	class HatchbackWheel;
	class ExpansionAnWheel: HatchbackWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT_DESC";
		model = "\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelMain.p3d";
		weight = 15000;
		inventorySlot[] = {"expansion_anwheel_1_1","expansion_anwheel_2_1"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.536;
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
	class ExpansionAnWheelSmall: HatchbackWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_AN2_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_AN2_WHEEL_BACK_DESC";
		model = "\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelSmall.p3d";
		weight = 15000;
		inventorySlot[] = {"expansion_ansmallwheel_1_2","expansion_ansmallwheel_2_2"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.336;
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
	class ExpansionAn2: OffroadHatchback
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_AN2";
		model = "DayZExpansion\Vehicles\Air\An2\an2.p3d";
		vehicleClass = "Expansion_Plane";
		fuelCapacity = 192;
		fuelConsumption = 12;
		attachments[] = {"ExpansionAircraftBattery","Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug","expansion_anwheel_1_1","expansion_anwheel_2_1","expansion_ansmallwheel_1_2","expansion_ansmallwheel_2_2"};
		hiddenSelections[] = {};
		hiddenSelectionsTextures[] = {};
		hiddenSelectionsMaterials[] = {};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_RWD";
			airDragCoefficient = 0.928;
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
				reverse = 1.0;
				ratios[] = {1.0};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.3;
				maxClutchTorque = 180;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 25;
					brakeBias = 0.7;
					brakeForce = 1000;
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
							inventorySlot = "expansion_anwheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "expansion_anwheel_2_1";
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
					brakeBias = 0.3;
					brakeForce = 1000;
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
							inventorySlot = "expansion_ansmallwheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "expansion_ansmallwheel_2_2";
							animTurn = "turnbackright";
							animRotation = "wheelbackright";
							animDamper = "damper_2_2";
							wheelHub = "wheel_2_2_damper_land";
						};
					};
				};
			};
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
			class Cargo3
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"ExpansionAircraftBattery","GlowPlug"};
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
				attachmentSlots[] = {"expansion_anwheel_1_1","expansion_anwheel_2_1","expansion_ansmallwheel_1_2","expansion_ansmallwheel_2_2"};
			};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.1;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_An2_Engine_Ext_SoundSet"};
			soundSetsInt[] = {"Expansion_An2_Engine_Int_SoundSet"};
		};
		class AnimationSources
		{
			class rotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
			class hiderotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
			class hiderotorblur
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
			class AileronL
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 0;
			};
			class AileronR
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 0;
			};
			class FlapL
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 0;
			};
			class FlapR
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 0;
			};
			class ElevatorL
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 0;
			};
			class ElevatorR
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 0;
			};
			class Rudder
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 0;
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Green";
	};
	class ExpansionAn2_Green: ExpansionAn2
	{
		skinBase = "ExpansionAn2";
		skinName = "Green";
	};
	class ExpansionSpraycanWhite: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_White_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_White_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_white_co.paa"};
		skinName = "White";
	};
	class ExpansionAn2_White: ExpansionAn2
	{
		skinBase = "ExpansionAn2";
		skinName = "White";
	};
	class ExpansionSpraycanOrange: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Orange_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Orange_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_Orange_co.paa"};
		skinName = "Orange";
	};
	class ExpansionAn2_Orange: ExpansionAn2
	{
		skinBase = "ExpansionAn2";
		skinName = "Orange";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyAnWheelMain: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelMain.p3d";
		inventorySlot[] = {"expansion_anwheel_1_1","expansion_anwheel_2_1"};
	};
	class ProxyAnWheelSmall: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelSmall.p3d";
		inventorySlot[] = {"expansion_ansmallwheel_1_2","expansion_ansmallwheel_2_2"};
	};
};
