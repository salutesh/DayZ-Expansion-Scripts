#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Air_Merlin
	{
		units[] = {"ExpansionMerlinFrontWheel","ExpansionMerlinBackWheel","ExpansionMerlinCargoRamp","ExpansionMerlinWreck","ExpansionMerlin","ExpansionSpraycanGreen","ExpansionMerlin_Green","ExpansionSpraycanDahoman","ExpansionMerlin_Dahoman","ExpansionSpraycanIon","ExpansionMerlin_Ion","ExpansionSpraycanPresident","ExpansionMerlin_President","ExpansionSpraycanVrana","ExpansionMerlin_Vrana","ExpansionSpraycanWave","ExpansionMerlin_Wave"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgSlots
{
	class Slot_merlinwheel_1_1
	{
		name = "merlinwheel_1_1";
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_FRONT_DESC";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_merlinwheel_1_2
	{
		name = "merlinwheel_1_2";
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_FRONT_DESC";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_merlinwheel_2_1
	{
		name = "merlinwheel_2_1";
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK_DESC";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_merlinwheel_2_2
	{
		name = "merlinwheel_2_2";
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK_DESC";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_merlindoor_1_1
	{
		name = "merlindoor_1_1";
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK_DESC";
		selection = "merlindoor_1_1";
		ghostIcon = "doorfront";
	};
	class Slot_merlindoor_1_2
	{
		name = "merlindoor_1_2";
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK_DESC";
		selection = "merlindoor_1_2";
		ghostIcon = "doorback";
	};
	class Slot_merlinhood
	{
		name = "merlinhood";
		displayName = "Merlin Hood";
		descriptionShort = "";
		selection = "merlinhood";
		ghostIcon = "hood";
	};
	class Slot_merlinhatch_1_1
	{
		name = "merlinhatch_1_1";
		displayName = "Merlin Hatch";
		descriptionShort = "";
		selection = "merlinhatch_1_1";
		ghostIcon = "trunk";
	};
	class Slot_merlinhatch_1_2
	{
		name = "merlinhatch_1_2";
		displayName = "Merlin Hatch";
		descriptionShort = "";
		selection = "merlinhatch_1_2";
		ghostIcon = "trunk";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionWreck;
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
	class ExpansionHelicopterScript;
	class GUIInventoryAttachmentsProps;
	class Body;
	class DamageSystem;
	class DamageZones;
	class GlobalHealth;
	class HouseNoDestruct;
	class CarWheel;
	class ExpansionMerlinFrontWheel: CarWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_FRONT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_FRONT_DESC";
		model = "\DayZExpansion\Vehicles\Air\Merlin\proxy\merlinfrontwheel.p3d";
		weight = 15000;
		inventorySlot[] = {"merlinwheel_1_1","merlinwheel_2_1"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.316;
		width = 0.152;
		tyreRollResistance = 0.015;
		tyreTread = 0.7;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 300;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.3,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}}};
				};
			};
		};
	};
	class ExpansionMerlinBackWheel: CarWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_MERLIN_WHEEL_BACK_DESC";
		model = "\DayZExpansion\Vehicles\Air\Merlin\proxy\merlinbackwheel.p3d";
		weight = 15000;
		inventorySlot[] = {"merlinwheel_1_2","merlinwheel_2_2"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.221;
		width = 0.618;
		tyreRollResistance = 0.015;
		tyreTread = 0.7;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 300;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.3,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}}};
				};
			};
		};
	};
	class ExpansionMerlinCargoRamp: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Air\Merlin\proxy\cargoramp.p3d";
	};
	class ExpansionMerlinWreck: ExpansionWreck
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_MERLIN";
		model = "\DayZExpansion\Vehicles\Air\Merlin\MerlinWreck.p3d";
		class Cargo
		{
			itemsCargoSize[] = {10,80};
		};
	};
	class ExpansionMerlin: ExpansionHelicopterScript
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_MERLIN";
		model = "\DayZExpansion\Vehicles\Air\Merlin\merlin.p3d";
		modelZeroPointDistanceFromGround = 3.81;
		vehicleClass = "Expansion_Helicopter";
		wreck = "ExpansionMerlinWreck";
		fuelCapacity = 292;
		fuelConsumption = 95;
		attachments[] = {"ExpansionHelicopterBattery","Reflector_1_1","ExpansionIgniterPlug","ExpansionHydraulicHoses","merlinwheel_1_1","merlinwheel_1_2","merlinwheel_2_1","merlinwheel_2_2"};
		doors[] = {};
		applySkinsTo[] = {};
		hiddenSelections[] = {"camo1","camo2","camo3"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\heavy_mer_1_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_2_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_3_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\air\merlin\data\heavy_mer_1_h.rvmat","dayzexpansion\vehicles\air\merlin\data\heavy_mer_2_h.rvmat","dayzexpansion\vehicles\air\merlin\data\heavy_mer_3_h.rvmat"};
		defaultSkin = "Green";
		class AnimationSources
		{
			class HideDestroyed_1_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_1_2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_2_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class HideDestroyed_2_2
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
			class AnimHitWheel_1_1
			{
				source = "Hit";
				hitpoint = "HitWheel_1_1";
				raw = 1;
			};
			class AnimHitWheel_1_2: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_1_2";
			};
			class AnimHitWheel_2_1: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_2_1";
			};
			class AnimHitWheel_2_2: AnimHitWheel_1_1
			{
				hitpoint = "HitWheel_2_2";
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
			class rotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class rearrotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class hiderotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
			class EnableMonitor
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
			class gear
			{
				source = "user";
				animPeriod = 2;
				initPhase = 0;
			};
			class cyclicForward
			{
				source = "user";
				animPeriod = 0.25;
				initPhase = 0;
			};
			class cyclicAside
			{
				source = "user";
				animPeriod = 0.25;
				initPhase = 0;
			};
			class door_back_l
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 1.0;
			};
			class door_back_r
			{
				source = "user";
				animPeriod = 1.5;
				initPhase = 1.0;
			};
			class cargoramp
			{
				source = "user";
				animPeriod = 5;
				initPhase = 0;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,80};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.1;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_Merlin_Engine_Ext_SoundSet","Expansion_Merlin_Rotor_Ext_SoundSet"};
			soundSetsInt[] = {"Expansion_Merlin_Engine_Int_SoundSet","Expansion_Merlin_Rotor_Int_SoundSet"};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"ExpansionHelicopterBattery","ExpansionIgniterPlug","ExpansionHydraulicHoses"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"merlinwheel_1_1","merlinwheel_1_2","merlinwheel_2_1","merlinwheel_2_2"};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10000;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints = 5000;
						transferToGlobalCoef = 0;
					};
					fatalInjuryCoef = -1;
					componentNames[] = {"camo1","camo2","camo3"};
					inventorySlots[] = {};
				};
				class Engine
				{
					class Health
					{
						hitpoints = 2500;
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
						hitpoints = 1600;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					fatalInjuryCoef = -1;
					componentNames[] = {"dmgZone_fuelTank"};
					inventorySlots[] = {};
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
						healthLevels[] = {{1.0,{},{}},{0.7,{}},{0.5,{},{}},{0.3,{}},{0.0,{},{}}};
					};
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
			};
		};
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_RWD";
			airDragFrontTotal = 1.0;
			maxSpeed = 200;
			altitudeFullForce = 1000;
			altitudeNoForce = 2000;
			bodyFrictionCoef = 1.6;
			liftForceCoef = 1.0;
			bankForceCoef = 0.2;
			tailForceCoef = 6.0;
			linearFrictionCoef[] = {16.0,0.04,0.04};
			angularFrictionCoef = 1.5;
			class Rotor
			{
				minAutoRotateSpeed = 2.0;
				maxAutoRotateSpeed = 10.0;
				startUpTime = 20.0;
			};
			class AntiTorque
			{
				speed = 1.8;
				max = 0.04;
			};
			class Cyclic
			{
				forceCoefficient = 1.5;
				class Forward
				{
					speed = 10.0;
					max = 1.0;
					coefficient = 0.4;
					animation = "cyclicForward";
				};
				class Side
				{
					speed = 10.0;
					max = 1.0;
					coefficient = 0.4;
					animation = "cyclicAside";
				};
			};
			braking[] = {2.5,0.1,1.0,0.8,2.5,0.9,3.0,1.0};
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
					maxSteeringAngle = 0;
					brakeBias = 0.9;
					brakeForce = 15000;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 400;
						stiffness = 76000;
						compression = 5900;
						damping = 6600;
						travelMaxUp = 0.079;
						travelMaxDown = 0.06;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "merlinwheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "merlinwheel_2_1";
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
					brakeBias = 0.8;
					brakeForce = 500000;
					finalRatio = 4.1;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 400;
						stiffness = 46000;
						compression = 10450;
						damping = 13200;
						travelMaxUp = 0.086;
						travelMaxDown = 0.133;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "merlinwheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "merlinwheel_2_2";
							animTurn = "turnbackright";
							animRotation = "wheelbackright";
							animDamper = "damper_2_2";
							wheelHub = "wheel_2_2_damper_land";
						};
					};
				};
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
	class ExpansionMerlin_Green: ExpansionMerlin
	{
		skinBase = "ExpansionMerlin";
		skinName = "Green";
	};
	class ExpansionSpraycanDahoman: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Dahoman_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Dahoman_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Dahoman";
	};
	class ExpansionMerlin_Dahoman: ExpansionMerlin
	{
		skinBase = "ExpansionMerlin";
		skinName = "Dahoman";
	};
	class ExpansionSpraycanIon: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Ion_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Ion_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Ion";
	};
	class ExpansionMerlin_Ion: ExpansionMerlin
	{
		skinBase = "ExpansionMerlin";
		skinName = "Ion";
	};
	class ExpansionSpraycanPresident: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_President_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_President_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "President";
	};
	class ExpansionMerlin_President: ExpansionMerlin
	{
		skinBase = "ExpansionMerlin";
		skinName = "President";
	};
	class ExpansionSpraycanVrana: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Vrana_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Vrana_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Vrana";
	};
	class ExpansionMerlin_Vrana: ExpansionMerlin
	{
		skinBase = "ExpansionMerlin";
		skinName = "Vrana";
	};
	class ExpansionSpraycanWave: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Wave_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Wave_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Wave";
	};
	class ExpansionMerlin_Wave: ExpansionMerlin
	{
		skinBase = "ExpansionMerlin";
		skinName = "Wave";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyMerlinFrontWheel: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Air\Merlin\proxy\merlinfrontwheel.p3d";
		inventorySlot[] = {"merlinwheel_1_1","merlinwheel_2_1"};
	};
	class ProxyMerlinBackWheel: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Air\Merlin\proxy\merlinbackwheel.p3d";
		inventorySlot[] = {"merlinwheel_1_2","merlinwheel_2_2"};
	};
};
