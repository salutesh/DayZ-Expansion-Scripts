#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Data
	{
		units[] = {"ExpansionCarDoorFix","ExpansionSpraycanBlack","ExpansionSpraycanBlackRust","ExpansionSpraycanGrey","ExpansionSpraycanGreyRust","ExpansionSpraycanWhite","ExpansionSpraycanWhiteRust","ExpansionSpraycanGreen","ExpansionSpraycanGreenRust","ExpansionSpraycanRed","ExpansionSpraycanRedRust","ExpansionSpraycanBlue","ExpansionSpraycanBlueRust","ExpansionSpraycanYellow","ExpansionSpraycanYellowRust","ExpansionSpraycanYellowBright","ExpansionSpraycanYellowBrightRust","ExpansionSpraycanWine","ExpansionSpraycanWineRust","ExpansionSpraycanLime","ExpansionSpraycanPolice","ExpansionSpraycanBanditKitty","ExpansionWreck"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DayZExpansion_Core_Scripts"};
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
	class ExpansionWreck: Container_Base
	{
		scope = 2;
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
		scope = 1;
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
			class Gearbox
			{
				reverse = 3.526;
				ratios[] = {3.667,2.1,1.361,1.0};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 260;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 30;
					finalRatio = 4.1;
					brakeBias = 0.6;
					brakeForce = 4000;
					wheelHubMass = 0;
					wheelHubRadius = 0.0;
					class Suspension
					{
						swayBar = 1700;
						stiffness = 40000;
						compression = 2100;
						damping = 7500;
						travelMaxUp = 0.0882;
						travelMaxDown = 0.0833;
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
					wheelHubMass = 0;
					wheelHubRadius = 0.0;
					class Suspension
					{
						swayBar = 1800;
						stiffness = 40000;
						compression = 2200;
						damping = 7600;
						travelMaxUp = 0.1587;
						travelMaxDown = 0.1059;
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
			class Props
			{
				class Prop
				{
					engine = 1;
					start = "engine_end";
					end = "engine_start";
					maxYaw = 45;
					type = "water";
				};
			};
			class Buoyancy
			{
				class Surface1
				{
					point = "surface1";
				};
				class Surface2
				{
					point = "surface2";
				};
				class Surface3
				{
					point = "surface3";
				};
				class Surface4
				{
					point = "surface4";
				};
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
