	class Slot_expansion_c130jwheel_1_1
	{
		name = "expansion_c130jwheel_1_1";
		displayName = "C130J Wheel";
		descriptionShort = "";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_c130jwheel_2_1
	{
		name = "expansion_c130jwheel_2_1";
		displayName = "C130J Wheel";
		descriptionShort = "";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_c130jwheel_1_2
	{
		name = "expansion_c130jwheel_1_2";
		displayName = "C130J Wheel";
		descriptionShort = "";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_expansion_c130jwheel_2_2
	{
		name = "expansion_c130jwheel_2_2";
		displayName = "C130J Wheel";
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
	class ExpansionC130JWheel: HatchbackWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_C130J_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_C130J_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Air\C130J\proxy\C130JWheelMain.p3d";
		weight = 15000;
		inventorySlot[] = {"expansion_c130jwheel_1_1","expansion_c130jwheel_2_1","expansion_c130jwheel_1_2","expansion_c130jwheel_2_2"};
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
	class ExpansionC130J: ExpansionPlaneScript
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_C130J";
		model = "DayZExpansion\Vehicles\Air\C130J\C130J.p3d";
		vehicleClass = "Expansion_Plane";
		fuelCapacity = 192;
		fuelConsumption = 21;
		attachments[] = {"ExpansionAircraftBattery","Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug","expansion_c130jwheel_1_1","expansion_c130jwheel_2_1","expansion_c130jwheel_1_2","expansion_c130jwheel_2_2"};
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
			braking[] = {0.0,0.1,1.0,0.8,2.5,0.9,3.0,1.0};
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
							inventorySlot = "expansion_c130jwheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "expansion_c130jwheel_2_1";
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
							inventorySlot = "expansion_c130jwheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "expansion_c130jwheel_2_2";
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
				getInPos = "pos driver";
				getInDir = "pos driver dir";
			};
			class CoDriver: CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos cargo";
				getInDir = "pos cargo dir";
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
				attachmentSlots[] = {"expansion_c130jwheel_1_1","expansion_c130jwheel_2_1","expansion_c130jwheel_1_2","expansion_c130jwheel_2_2"};
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
			class aileron
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class elevator
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class rudder
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class flap
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanGrey: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Grey_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Grey_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_grey_co.paa"};
		skinName = "Grey";
	};
	class ExpansionC130J_Gray: ExpansionC130J
	{
		skinBase = "ExpansionC130J";
		skinName = "Gray";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyC130JWheelMain: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Air\C130J\proxy\C130JWheelMain.p3d";
		inventorySlot[] = {"expansion_c130jwheel_1_1","expansion_c130jwheel_2_1","expansion_c130jwheel_1_2","expansion_c130jwheel_2_2"};
	};
};
