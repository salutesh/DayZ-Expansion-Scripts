#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Data
	{
		units[] = {"ExpansionCarDoorFix","ExpansionSpraycanBlack","ExpansionSpraycanBlackRust","ExpansionSpraycanGrey","ExpansionSpraycanGreyRust","ExpansionSpraycanWhite","ExpansionSpraycanWhiteRust","ExpansionSpraycanGreen","ExpansionSpraycanGreenRust","ExpansionSpraycanRed","ExpansionSpraycanRedRust","ExpansionSpraycanBlue","ExpansionSpraycanBlueRust","ExpansionSpraycanYellow","ExpansionSpraycanYellowRust","ExpansionSpraycanYellowBright","ExpansionSpraycanYellowBrightRust","ExpansionSpraycanWine","ExpansionSpraycanWineRust","ExpansionSpraycanLime","ExpansionSpraycanPolice","ExpansionSpraycanBanditKitty"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgVehicleClasses
{
	class Expansion_Helicopter
	{
		displayName = "Expansion Helicopter";
	};
	class Expansion_Motorbike
	{
		displayName = "Expansion Bike";
	};
	class Expansion_Quadbike
	{
		displayName = "Expansion Quad";
	};
	class Expansion_Bicycle
	{
		displayName = "Expansion Bicycle";
	};
	class Expansion_Truck
	{
		displayName = "Expansion Truck";
	};
	class Expansion_Plane
	{
		displayName = "Expansion Plane";
	};
	class Expansion_Boat
	{
		displayName = "Expansion Boat";
	};
	class Expansion_Ship
	{
		displayName = "Expansion Ship";
	};
	class Expansion_Car
	{
		displayName = "Expansion Car";
	};
	class Expansion_Vehicle
	{
		displayName = "Expansion Vehicle";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Container_Base;
	class CarWheel: Inventory_Base
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class drop;
			};
		};
	};
	class CarDoor: Inventory_Base
	{
		hasDoors = 1;
		hiddenSelections[] = {"dmgZone_doors"};
		hiddenSelectionsTextures[] = {""};
		hiddenSelectionsMaterials[] = {""};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health;
			};
			class DamageZones
			{
				class Window
				{
					class Health;
				};
				class Doors
				{
					class Health;
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUp;
				class drop;
			};
		};
	};
	class Car;
	class CarScript: Car
	{
		applySkinsTo[] = {};
		class ExpansionAttachments{};
	};
	class Crew;
	class Driver;
	class CoDriver;
	class SimulationModule;
	class Axles;
	class Front;
	class Wheels;
	class Left;
	class Right;
	class Rear;
	class ViewPilot;
	class AnimationSources;
	class DoorsDriver;
	class DoorsCoDriver;
	class DoorsHood;
	class DoorsTrunk;
	class HideDestroyed_1_1;
	class HideDestroyed_1_2;
	class HideDestroyed_2_1;
	class HideDestroyed_2_2;
	class AnimHitWheel_1_1;
	class AnimHitWheel_1_2;
	class AnimHitWheel_2_1;
	class AnimHitWheel_2_2;
	class HitDoorsHood;
	class HitDoorsTrunk;
	class HitDoorsDrivers;
	class HitDoorsCoDrivers;
	class HitDoorsCargo;
	class ExpansionCarDoorFix: CarDoor
	{
		scope = 2;
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
	class ExpansionSpraycanBlackRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BlackRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BlackRust_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "BlackRust";
	};
	class ExpansionSpraycanGrey: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Grey_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Grey_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"};
		skinName = "Grey";
	};
	class ExpansionSpraycanGreyRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_GreyRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_GreyRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"};
		skinName = "GreyRust";
	};
	class ExpansionSpraycanWhite: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_White_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_White_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_white_co.paa"};
		skinName = "White";
	};
	class ExpansionSpraycanWhiteRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_WhiteRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_WhiteRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_white_co.paa"};
		skinName = "WhiteRust";
	};
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionSpraycanGreenRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_GreenRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_GreenRust_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "GreenRust";
	};
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_red_co.paa"};
		skinName = "Red";
	};
	class ExpansionSpraycanRedRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_RedRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_RedRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_red_co.paa"};
		skinName = "RedRust";
	};
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_blue_co.paa"};
		skinName = "Blue";
	};
	class ExpansionSpraycanBlueRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BlueRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BlueRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_blue_co.paa"};
		skinName = "BlueRust";
	};
	class ExpansionSpraycanYellow: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Yellow_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Yellow_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "Yellow";
	};
	class ExpansionSpraycanYellowRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_YellowRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_YellowRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "YellowRust";
	};
	class ExpansionSpraycanYellowBright: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_YellowBright_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_YellowBright_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "YellowBright";
	};
	class ExpansionSpraycanYellowBrightRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_YellowBrightRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_YellowBrightRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "YellowBrightRust";
	};
	class ExpansionSpraycanWine: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Wine_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Wine_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_wine_co.paa"};
		skinName = "Wine";
	};
	class ExpansionSpraycanWineRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_WineRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_WineRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_wine_co.paa"};
		skinName = "WineRust";
	};
	class ExpansionSpraycanLime: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Lime_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Lime_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Lime";
	};
	class ExpansionSpraycanPolice: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Police_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Police_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_white_co.paa"};
		skinName = "Police";
	};
	class ExpansionSpraycanBanditKitty: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BanditKitty_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BanditKitty_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_pink_co.paa"};
		skinName = "BanditKitty";
	};
	class ExpansionWheelBase: Inventory_Base
	{
		scope = 0;
		mass = 80.0;
		radius = 0.536;
		width = 0.316;
	};
	class ExpansionVehicleBase: Inventory_Base
	{
		scope = 0;
		vehicleClass = "Expansion_Vehicle";
		bounding = "BSphere";
		overrideDrawArea = "8.0";
		forceFarBubble = "true";
		physLayer = "item_large";
		carveNavmesh = 0;
		fuelCapacity = 50;
		brakeFluidCapacity = 1;
		oilCapacity = 4;
		coolantCapacity = 6;
		hasDoors = 0;
	};
	class ExpansionVehicleBikeBase: ExpansionVehicleBase
	{
		scope = 0;
		vehicleClass = "Expansion_Bicycle";
		rotationFlags = 64;
		storageCategory = 4;
		insideSoundCoef = 0.9;
		fuelCapacity = 50;
		brakeFluidCapacity = 1;
		oilCapacity = 4;
		coolantCapacity = 6;
		brakeFluidLeakDebit[] = {0.0,0.0};
		oilLeakDebit[] = {0.0,0.0};
		coolantLeakDebit[] = {0.0,0.0};
		brakeFluidForceCoef[] = {0.0,1.0,1.0,1.0};
		damageFromOil[] = {0.0,0.0,1.0,0.0};
		damageFromCoolant[] = {0.0,0.0,1.0,0.0};
		engineBeltSlot = "EngineBelt";
		batterySlot = "CarBattery";
		electricPowerResName = "power";
		electricConsumptionIgnition = 3001;
		electricConsumptionEngine = 0.0;
		electricConsumptionLights = 0.0;
		electricOutputEngine = 5.0;
		selectionDashboard = "light_dashboard";
		selectionLightFrontL = "light_left";
		selectionLightFrontR = "light_right";
		selectionBrakeLights = "light_break";
		attachments[] = {"CarBattery","Reflector_1_1"};
		hiddenSelections[] = {""};
		hiddenSelectionsTextures[] = {""};
		hiddenSelectionsMaterials[] = {""};
		class SimulationModule
		{
			class Axles
			{
				class Front
				{
					class Wheels
					{
						class Center
						{
							inventorySlot = "";
							animTurn = "turnfront";
							animRotation = "wheelfront";
							animDamper = "damperfront";
							wheelHub = "wheel_1_damper_land";
						};
					};
				};
				class Rear
				{
					class Wheels
					{
						class Right
						{
							inventorySlot = "";
							animTurn = "turnback";
							animRotation = "wheelback";
							animDamper = "damperback";
							wheelHub = "wheel_2_damper_land";
						};
					};
				};
			};
		};
	};
	class ExpansionVehicleCarBase: ExpansionVehicleBase
	{
		scope = 0;
		vehicleClass = "Expansion_Car";
		rotationFlags = 64;
		storageCategory = 4;
		insideSoundCoef = 0.9;
		fuelCapacity = 50;
		brakeFluidCapacity = 1;
		oilCapacity = 4;
		coolantCapacity = 6;
		brakeFluidLeakDebit[] = {0.0,0.0};
		oilLeakDebit[] = {0.0,0.0};
		coolantLeakDebit[] = {0.0,0.0};
		brakeFluidForceCoef[] = {0.0,1.0,1.0,1.0};
		damageFromOil[] = {0.0,0.0,1.0,0.0};
		damageFromCoolant[] = {0.0,0.0,1.0,0.0};
		engineBeltSlot = "EngineBelt";
		batterySlot = "CarBattery";
		electricPowerResName = "power";
		electricConsumptionIgnition = 3001;
		electricConsumptionEngine = 0.0;
		electricConsumptionLights = 0.0;
		electricOutputEngine = 5.0;
		selectionDashboard = "light_dashboard";
		selectionLightFrontL = "light_left";
		selectionLightFrontR = "light_right";
		selectionBrakeLights = "light_break";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1"};
		hiddenSelections[] = {""};
		hiddenSelectionsTextures[] = {""};
		hiddenSelectionsMaterials[] = {""};
		class Crew
		{
			class Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
				isDriver = 1;
			};
			class CoDriver
			{
				actionSel = "seat_coDriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_coDriver";
				getInDir = "pos_coDriver_dir";
			};
		};
		class SimulationModule
		{
			class Axles
			{
				class Front
				{
					class Wheels
					{
						class Left
						{
							inventorySlot = "";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damperfrontleft";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right
						{
							inventorySlot = "";
							animTurn = "turnfrontright";
							animRotation = "wheelfrontright";
							animDamper = "damperfrontright";
							wheelHub = "wheel_2_1_damper_land";
						};
					};
				};
				class Rear
				{
					class Wheels
					{
						class Left
						{
							inventorySlot = "";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damperbackleft";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right
						{
							inventorySlot = "";
							animTurn = "turnbackright";
							animRotation = "wheelbackright";
							animDamper = "damperbackright";
							wheelHub = "wheel_2_2_damper_land";
						};
					};
				};
			};
		};
		class AnimationSources
		{
			class DoorsDriver
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class DoorsCoDriver: DoorsDriver{};
			class DoorsHood: DoorsDriver{};
			class DoorsTrunk: DoorsDriver{};
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
			class HitDoorsHood: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsHood";
			};
			class HitDoorsTrunk: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsTrunk";
			};
			class HitDoorsDrivers: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsDriver";
			};
			class HitDoorsCoDrivers: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsCoDriver";
			};
			class HitDoorsCargo: AnimHitWheel_1_1
			{
				hitpoint = "HitDoorsCargo";
			};
		};
	};
	class ExpansionWreck: Container_Base
	{
		scope = 0;
		forceFarBubble = "true";
		carveNavmesh = 1;
		itemsCargoSize[] = {10,100};
		rotationFlags = 2;
		itemSize[] = {20,20};
		weight = 1000000;
		physLayer = "item_large";
		cargoClass = "";
		inventoryCondition = "true";
		storageCategory = 1;
		openable = 0;
		lootCategory = "Containers";
		class GUIInventoryAttachmentsProps{};
		class Cargo
		{
			itemsCargoSize[] = {10,50};
		};
	};
	class ExpansionHelicopterScript: CarScript
	{
		scope = 0;
		vehicleClass = "Expansion_Helicopter";
		attachments[] = {"ExpansionHelicopterBattery","Reflector_1_1","CarRadiator","GlowPlug"};
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
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragFrontTotal = 1.0;
			maxSpeed = 220;
			altitudeFullForce = 1000;
			altitudeNoForce = 2000;
			bodyFrictionCoef = 1.1;
			bankForceCoef = 0.05;
			tailForceCoef = 2.0;
			linearFrictionCoef[] = {16.0,0.04,0.04};
			angularFrictionCoef = 1.5;
			class Rotor
			{
				minAutoRotateSpeed = 2.0;
				maxAutoRotateSpeed = 10.0;
				startUpTime = 15;
			};
			class AntiTorque
			{
				speed = 1.5;
				max = 0.16;
			};
			class Cyclic
			{
				forceCoefficient = 1.3;
				class Forward
				{
					speed = 10.0;
					max = 0.7;
					coefficient = 1.0;
					animation = "cyclicForward";
				};
				class Side
				{
					speed = 4.0;
					max = 0.6;
					coefficient = 0.6;
					animation = "cyclicAside";
				};
			};
			class Steering
			{
				maxSteeringAngle = 30;
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
			braking[] = {0.0,0.1,1.0,0.8,3.0,0.9,3.5,1.0};
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
			class Engine
			{
				torqueCurve[] = {650,0,750,40,1400,80,3400,114,5400,95,8000,0};
				inertia = 0.15;
				frictionTorque = 100;
				rollingFriction = 0.5;
				viscousFriction = 0.5;
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
			class Clutch
			{
				maxTorqueTransfer = 260;
				uncoupleTime = 0.3;
				coupleTime = 0.45;
			};
			class Gearbox
			{
				type = "GEARBOX_MANUAL";
				reverse = 3.526;
				ratios[] = {3.667,2.1,1.361,1.0};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 260;
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
					maxSteeringAngle = 30;
					finalRatio = 4.1;
					brakeBias = 0.6;
					brakeForce = 4000;
					wheelHubMass = 1;
					wheelHubRadius = 0.01;
					maxBrakeTorque = 2000;
					maxHandbrakeTorque = 2500;
					class Differential
					{
						ratio = 4.1;
						type = "DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						swayBar = 1;
						stiffness = 1;
						compression = 1;
						damping = 1;
						travelMaxUp = 0;
						travelMaxDown = 0;
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
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.4;
					brakeForce = 3800;
					wheelHubMass = 1;
					wheelHubRadius = 0.01;
					maxBrakeTorque = 1000;
					maxHandbrakeTorque = 2500;
					class Differential
					{
						ratio = 4.1;
						type = "DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						swayBar = 1;
						stiffness = 1;
						compression = 1;
						damping = 1;
						travelMaxUp = 0;
						travelMaxDown = 0;
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
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "Engine";
				description = "";
				attachmentSlots[] = {"ExpansionHelicopterBattery","GlowPlug"};
				icon = "missing";
			};
			class Body
			{
				name = "Body";
				description = "";
				attachmentSlots[] = {"Reflector_1_1"};
				icon = "missing";
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,25};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
	};
	class ExpansionVehicleHelicopterBase: ExpansionVehicleBase
	{
		scope = 0;
		vehicleClass = "Expansion_Helicopter";
		attachments[] = {"ExpansionHelicopterBattery","Reflector_1_1","CarRadiator","GlowPlug"};
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
		};
		class SimulationModule
		{
			airDragFrontTotal = 1.0;
			maxSpeed = 220;
			altitudeFullForce = 1000;
			altitudeNoForce = 2000;
			bodyFrictionCoef = 1.1;
			bankForceCoef = 0.05;
			tailForceCoef = 2.0;
			linearFrictionCoef[] = {16.0,0.04,0.04};
			angularFrictionCoef = 1.5;
			class Rotor
			{
				minAutoRotateSpeed = 2.0;
				maxAutoRotateSpeed = 10.0;
				startUpTime = 15;
			};
			class AntiTorque
			{
				speed = 1.5;
				max = 0.16;
			};
			class Cyclic
			{
				forceCoefficient = 1.3;
				class Forward
				{
					speed = 10.0;
					max = 0.7;
					coefficient = 1.0;
					animation = "cyclicForward";
				};
				class Side
				{
					speed = 4.0;
					max = 0.6;
					coefficient = 0.6;
					animation = "cyclicAside";
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "Engine";
				description = "";
				attachmentSlots[] = {"ExpansionHelicopterBattery","GlowPlug"};
				icon = "missing";
			};
			class Body
			{
				name = "Body";
				description = "";
				attachmentSlots[] = {"Reflector_1_1"};
				icon = "missing";
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,25};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
	};
	class ExpansionBoatScript: CarScript
	{
		scope = 0;
		vehicleClass = "Expansion_Boat";
		displayName = "";
		model = "";
		fuelCapacity = 192;
		fuelConsumption = 12;
		attachments[] = {"Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug"};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "Engine";
				description = "";
				attachmentSlots[] = {"GlowPlug","TruckBattery"};
				icon = "missing";
			};
			class Body
			{
				name = "Body";
				description = "";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
				icon = "missing";
			};
		};
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
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragFrontTotal = 0.995;
			braking[] = {0.0,0.1,1.0,0.8,3.0,0.9,3.5,1.0};
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
			class Clutch
			{
				maxTorqueTransfer = 260;
				uncoupleTime = 0.3;
				coupleTime = 0.45;
			};
			class Gearbox
			{
				reverse = 2.526;
				ratios[] = {1.3,1.2,1.1,1.0};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 260;
			};
			class Engines
			{
				class BoatMotor
				{
					inertia = 0.15;
					torqueMax = 11.4;
					torqueRpm = 3400;
					powerMax = 24.7;
					powerRpm = 5400;
					rpmIdle = 850;
					rpmMin = 900;
					rpmClutch = 1350;
					rpmRedline = 6000;
					rpmMax = 8000;
					gear = -1;
				};
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
					class Differential
					{
						ratio = 4.1;
						type = "DIFFERENTIAL_OPEN";
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
							inventorySlot = "NivaWheel_1_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque = 1000;
					maxHandbrakeTorque = 2500;
					class Differential
					{
						ratio = 4.1;
						type = "DIFFERENTIAL_OPEN";
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "NivaWheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "NivaWheel_1_1";
						};
					};
				};
			};
		};
		class ObstacleGenerator
		{
			carve = 1;
			timeToStationary = 5;
			moveThreshold = 0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,0.7};
					};
					class Cyl3
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,-0.7};
					};
				};
			};
		};
	};
	class ExpansionVehicleBoatBase: ExpansionVehicleBase
	{
		scope = 0;
		vehicleClass = "Expansion_Boat";
		displayName = "";
		model = "";
		fuelCapacity = 192;
		fuelConsumption = 12;
		attachments[] = {"Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug"};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "Engine";
				description = "";
				attachmentSlots[] = {"GlowPlug","TruckBattery"};
				icon = "missing";
			};
			class Body
			{
				name = "Body";
				description = "";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
				icon = "missing";
			};
		};
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
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragFrontTotal = 0.995;
			braking[] = {0.0,0.1,1.0,0.8,3.0,0.9,3.5,1.0};
			class Gearbox
			{
				reverse = 2.526;
				ratios[] = {1.3,1.2,1.1,1.0};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 260;
			};
			class Axles: Axles
			{
				class Front: Front
				{
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
							inventorySlot = "NivaWheel_1_1";
						};
					};
				};
				class Rear: Rear
				{
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "NivaWheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "NivaWheel_1_1";
						};
					};
				};
			};
		};
		class ObstacleGenerator
		{
			carve = 1;
			timeToStationary = 5;
			moveThreshold = 0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,0.7};
					};
					class Cyl3
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,-0.7};
					};
				};
			};
		};
	};
	class ExpansionVehiclePlaneBase: ExpansionVehicleBase
	{
		scope = 0;
		vehicleClass = "Expansion_Plane";
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
		class SimulationModule: SimulationModule
		{
			class Engines
			{
				class Engine
				{
					inertia = 0.5;
					torqueMax = 29.1;
					torqueRpm = 1670;
					powerMax = 29.1;
					powerRpm = 2200;
					rpmIdle = 1000;
					rpmMin = 1050;
					rpmClutch = 1250;
					rpmRedline = 2150;
					rpmMax = 2250;
					gear = -1;
				};
			};
			class Props{};
		};
	};
};
class CfgSurfaces
{
	class DZ_SurfacesInt;
	class DZ_SurfacesExt;
	class expansion_rubber: DZ_SurfacesExt
	{
		Density = 300;
		Thickness = 5;
		deflection = 0.2;
		bulletPenetrability = 600;
		bulletPenetrabilityWithThickness = 200;
		transparency = 0;
		rough = 0.1;
		dust = 0;
		audibility = 0.5;
		soundEnviron = "Empty";
		isLiquid = "false";
		friction = 0.0;
		restitution = 0.0;
		impact = "Hit_Rubber";
		soundHit = "rubber";
	};
};
