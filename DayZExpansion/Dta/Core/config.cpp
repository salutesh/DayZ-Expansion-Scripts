#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Dta_Core
	{
		units[] = {"ExpansionPickup","ExpansionPickupCone","ExpansionDebugCone","ExpansionDebugConeSmall","ExplosionPoint","ExplosionSmall","ExpansionWreck","ExpansionTraderNPCBase","ExpansionTraderMirek","ExpansionTraderDenis","ExpansionTraderBoris","ExpansionTraderCyril","ExpansionTraderElias","ExpansionTraderFrancis","ExpansionTraderGuo","ExpansionTraderHassan","ExpansionTraderIndar","ExpansionTraderJose","ExpansionTraderKaito","ExpansionTraderLewis","ExpansionTraderManua","ExpansionTraderNiki","ExpansionTraderOliver","ExpansionTraderPeter","ExpansionTraderQuinn","ExpansionTraderRolf","ExpansionTraderSeth","ExpansionTraderTaiki","ExpansionTraderLinda","ExpansionTraderMaria","ExpansionTraderFrida","ExpansionTraderGabi","ExpansionTraderHelga","ExpansionTraderIrena","ExpansionTraderJudy","ExpansionTraderKeiko","ExpansionTraderEva","ExpansionTraderNaomi","ExpansionNpcDenis","ExpansionNpcBoris","ExpansionNpcCyril","ExpansionNpcElias","ExpansionNpcFrancis","ExpansionNpcGuo","ExpansionNpcHassan","ExpansionNpcIndar","ExpansionNpcJose","ExpansionNpcKaito","ExpansionNpcLewis","ExpansionNpcManua","ExpansionNpcNiki","ExpansionNpcOliver","ExpansionNpcPeter","ExpansionNpcQuinn","ExpansionNpcRolf","ExpansionNpcSeth","ExpansionNpcTaiki","ExpansionNpcLinda","ExpansionNpcMaria","ExpansionNpcFrida","ExpansionNpcGabi","ExpansionNpcHelga","ExpansionNpcIrena","ExpansionNpcJudy","ExpansionNpcKeiko","ExpansionNpcEva","ExpansionNpcNaomi","ExpansionSoldierMirek","ExpansionSoldierDenis","ExpansionSoldierBoris","ExpansionSoldierCyril","ExpansionSoldierElias","ExpansionSoldierFrancis","ExpansionSoldierGuo","ExpansionSoldierHassan","ExpansionSoldierIndar","ExpansionSoldierJose","ExpansionSoldierKaito","ExpansionSoldierLewis","ExpansionSoldierManua","ExpansionSoldierNiki","ExpansionSoldierOliver","ExpansionSoldierPeter","ExpansionSoldierQuinn","ExpansionSoldierRolf","ExpansionSoldierSeth","ExpansionSoldierTaiki","ExpansionSoldierLinda","ExpansionSoldierMaria","ExpansionSoldierFrida","ExpansionSoldierGabi","ExpansionSoldierHelga","ExpansionSoldierIrena","ExpansionSoldierJudy","ExpansionSoldierKeiko","ExpansionSoldierEva","ExpansionSoldierNaomi","ExpansionSoldier2Mirek","ExpansionSoldier2Denis","ExpansionSoldier2Boris","ExpansionSoldier2Cyril","ExpansionSoldier2Elias","ExpansionSoldier2Francis","ExpansionSoldier2Guo","ExpansionSoldier2Hassan","ExpansionSoldier2Indar","ExpansionSoldier2Jose","ExpansionSoldier2Kaito","ExpansionSoldier2Lewis","ExpansionSoldier2Manua","ExpansionSoldier2Niki","ExpansionSoldier2Oliver","ExpansionSoldier2Peter","ExpansionSoldier2Quinn","ExpansionSoldier2Rolf","ExpansionSoldier2Seth","ExpansionSoldier2Taiki","ExpansionSoldier2Linda","ExpansionSoldier2Maria","ExpansionSoldier2Frida","ExpansionSoldier2Gabi","ExpansionSoldier2Helga","ExpansionSoldier2Irena","ExpansionSoldier2Judy","ExpansionSoldier2Keiko","ExpansionSoldier2Eva","ExpansionSoldier2Naomi","ExpansionMoneyBase"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DZ_Characters","DZ_Scripts","DZ_AI","DayZExpansion_Animations_Player"};
	};
};
class CfgSurfaces
{
	class DZ_SurfacesExt;
	class expansion_test_surface: DZ_SurfacesExt
	{
		files = "lino_int*";
		friction = 5.0;
		restitution = 1.0;
		soundEnviron = "road";
		soundHit = "hard_ground";
		character = "Empty";
		audibility = 0.4;
		footDamage = 0.023;
		impact = "Hit_Rubber";
		isDigable = 0;
		isFertile = 0;
	};
};
class CfgVehicleClasses
{
	class Expansion_Gear
	{
		displayName = "Expansion Gear";
	};
	class Expansion_Launchers
	{
		displayName = "Expansion Launchers";
	};
	class Expansion_Flags
	{
		displayName = "Expansion Flags";
	};
	class Expansion_Items
	{
		displayName = "Expansion Items";
	};
	class Expansion_Explosives
	{
		displayName = "Expansion Explosives";
	};
	class Expansion_Barricading
	{
		displayName = "Expansion Barricading";
	};
	class Expansion_Airdrop
	{
		displayName = "Expansion Airdrop";
	};
	class Expansion_Trader
	{
		displayName = "Expansion Traders";
	};
	class Expansion_Currency
	{
		displayName = "Expansion Currency";
	};
	class Expansion_Npc
	{
		displayName = "Expansion Npc";
	};
	class Expansion_Static
	{
		displayName = "Expansion Static";
	};
	class Expansion_Construction
	{
		displayName = "Expansion Construction";
	};
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
	class Expansion_Sign
	{
		displayName = "Expansion Sign";
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
class CfgAIBehaviours
{
	class Soldier
	{
		name = "Bot";
		HeadLookBoneName = "lookat";
		teamName = "Bots";
		class BehaviourHLBot
		{
			class MovementWalk
			{
				maxSpeed = 1.5;
				minSpeed = 0;
				acceleration = 5;
				maxAngleSpeed = 180;
				slowRadius = 0;
				stopRadius = 0.5;
				pathFilter = "BotCalm";
			};
			class MovementRun
			{
				maxSpeed = 3;
				minSpeed = 0;
				acceleration = 15;
				maxAngleSpeed = 120;
				slowRadius = 0;
				goalRadius = 1.5;
				stopRadius = 1.7;
				useStartAnimation = "false";
				pathFilter = "BotAlerted";
			};
			class MovementSprint
			{
				maxSpeed = 9;
				minSpeed = 0;
				acceleration = 27;
				maxAngleSpeed = 180;
				maxSpeedRange = 9;
				slowRadius = 3.2;
				goalRadius = 1.5;
				goalSpeed = 7.2;
				stopRadius = 1.7;
				waypointRadius = 1.5;
				useStartAnimation = "false";
				startAnimationMaxSpeed = 0;
				slowToTurn = "false";
				smoothAcceleration = "true";
				pathFilter = "BotAlerted";
			};
			class SlotCalm
			{
				class BehaviourBotCalm
				{
					StandingDurationMin = 4;
					StandingDurationMax = 8;
					WalkingDurationMin = 4;
					WalkingDurationMax = 12;
					MinLookTime = 1;
					MaxLookTime = 2;
					IsAttractMode = "false";
					class SoundsDuring
					{
						class Sound1
						{
							moveWithEntity = "true";
						};
						probability = 0.8;
						RepeatTimeMin = 9;
						RepeatTimeMax = 15;
						RepeatEnabled = "true";
					};
				};
			};
			class SlotAlerted
			{
				class BehaviourBotAlerted
				{
					maxTimeInDisturbedState = 5;
					alertToAttract = 0.5;
					fightStateEnterDistance = 2;
					fightStateExitDistance = 3;
					fightStateEnterOrientAngleDiff = 160;
					fightStateExitOrientAngleDiff = 160;
					disturbedTargetHistoryLength = 20;
					disturbedVisionUtilityWeight = 1;
					disturbedNoiseUtilityWeight = 1;
					disturbedDamageUtilityWeight = 0;
					attractedTargetHistoryLength = 40;
					attractedVisionUtilityWeight = 1;
					attractedNoiseUtilityWeight = 1;
					attractedDamageUtilityWeight = 1;
					chaseTargetHistoryLength = 20;
					chaseVisionUtilityWeight = 1;
					chaseNoiseUtilityWeight = 1;
					chaseDamageUtilityWeight = 1;
					class SoundsEntering
					{
						class Sound1{};
						probability = 1;
					};
					class SoundsDuring
					{
						class Sound1{};
						probability = 1;
						RepeatTimeMin = 3.9;
						RepeatTimeMax = 4.1;
						RepeatEnabled = "true";
					};
					noiseMakeAlertPeriod = 3;
					class NoiseMakeAlert
					{
						strength = 30;
						type = "sound";
					};
				};
			};
			class AlertSystem
			{
				visionToAlertMultiplier = 10;
				noiseToAlertMultiplier = 0.75;
				damageToAlertMultiplier = 10000;
				noiseShotToAlertMultiplier = 0.75;
				class Calm
				{
					DropSpeed = 3;
					DropDelay = 2;
					MaxAlertValue = 20;
				};
				class Alerted
				{
					DropSpeed = 1;
					DropDelay = 10;
					MaxAlertValue = 100;
				};
			};
			staminaDepletionSpeed = 5;
			staminaRechargeSpeed = 10;
		};
		class TargetSystemDZBase
		{
			visionProximityRange = 2.5;
			visionProximityStrengthMult = 3;
			visionCloseRange = 8;
			visionCloseHeight = 1.8;
			visionCloseStrengthMult = 3;
			visionRangeMin = 20;
			visionRangeMax = 40;
			visionFov = 1;
			visionPeripheralRangeMin = 5;
			visionPeripheralRangeMax = 20;
			visionPeripheralFov = 2.3;
			visionAngularSpeedMin = 0.1;
			visionAngularSpeedMax = 0.5;
			visionAngularSpeedMaxMult = 1;
			visionNightMinMult = 1;
			visionNightMaxMult = 0.4;
			visionRainMinMult = 1;
			visionRainMaxMult = 0.5;
			visionFogMinMult = 1;
			visionFogMaxMult = 0.5;
		};
		class NoiseSystemParams
		{
			rangeMin = 3;
			rangeMax = 35;
			rangeShotMin = 25;
			rangeShotMax = 200;
			radiusMin = 5;
			radiusMax = 20;
			radiusShotMin = 10;
			radiusShotMax = 50;
			class NoiseStrengthTeamMultipliers
			{
				BigGame = 1;
				Bots = 0;
				Player = 1;
			};
		};
	};
	class SoldierFemale: Soldier
	{
		class BehaviourHLBot: BehaviourHLBot
		{
			class SlotCalm: SlotCalm
			{
				class BehaviourBotCalm: BehaviourBotCalm
				{
					class SoundsDuring: SoundsDuring
					{
						class Sound1: Sound1
						{
							moveWithEntity = "true";
						};
					};
				};
			};
			class SlotAlerted: SlotAlerted
			{
				class BehaviourBotAlerted: BehaviourBotAlerted
				{
					class SoundsEntering: SoundsEntering
					{
						class Sound1: Sound1{};
					};
					class SoundsDuring: SoundsDuring
					{
						class Sound1: Sound1{};
					};
				};
			};
		};
	};
	class SoldierMale: Soldier
	{
		class BehaviourHLBot: BehaviourHLBot
		{
			class SlotCalm: SlotCalm
			{
				class BehaviourBotCalm: BehaviourBotCalm
				{
					class SoundsDuring: SoundsDuring
					{
						class Sound1: Sound1
						{
							moveWithEntity = "true";
						};
					};
				};
			};
			class SlotAlerted: SlotAlerted
			{
				class BehaviourBotAlerted: BehaviourBotAlerted
				{
					class SoundsEntering: SoundsEntering
					{
						class Sound1: Sound1{};
					};
					class SoundsDuring: SoundsDuring
					{
						class Sound1: Sound1{};
					};
				};
			};
		};
	};
};
class CfgVehicles
{
	class Man;
	class Inventory_Base;
	class Container_Base;
	class HouseNoDestruct;
	class DayZPlayer;
	class SurvivorM_Mirek;
	class SurvivorM_Denis;
	class SurvivorM_Boris;
	class SurvivorM_Cyril;
	class SurvivorM_Elias;
	class SurvivorM_Francis;
	class SurvivorM_Guo;
	class SurvivorM_Hassan;
	class SurvivorM_Indar;
	class SurvivorM_Jose;
	class SurvivorM_Kaito;
	class SurvivorM_Lewis;
	class SurvivorM_Manua;
	class SurvivorM_Niki;
	class SurvivorM_Oliver;
	class SurvivorM_Peter;
	class SurvivorM_Quinn;
	class SurvivorM_Rolf;
	class SurvivorM_Seth;
	class SurvivorM_Taiki;
	class SurvivorF_Linda;
	class SurvivorF_Maria;
	class SurvivorF_Frida;
	class SurvivorF_Gabi;
	class SurvivorF_Helga;
	class SurvivorF_Irena;
	class SurvivorF_Judy;
	class SurvivorF_Keiko;
	class SurvivorF_Eva;
	class SurvivorF_Naomi;
	class SurvivorBase: Man
	{
		class enfAnimSys
		{
			meshObject = "dz\characters\bodies\player_testing.xob";
			graphName = "DayZExpansion\Animations\Player\Graphs\player_main.agr";
			defaultInstance = "DayZExpansion\Animations\Player\player_main.asi";
			skeletonName = "player_testing.xob";
			startNode = "MasterControl";
		};
	};
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
	class ExpansionPickup: Inventory_Base
	{
		scope = 2;
	};
	class ExpansionPickupCone: ExpansionPickup
	{
		scope = 2;
		model = "\DayZExpansion\Dta\Core\cone.p3d";
	};
	class ExpansionDebugCone: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Dta\Core\cone.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,0.5,co)"};
	};
	class ExpansionDebugConeSmall: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Dta\Core\coneSmall.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,0.5,co)"};
	};
	class ExplosionPoint: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\data\lightpoint.p3d";
	};
	class ExplosionSmall: ExplosionPoint
	{
		scope = 2;
	};
	class ExpansionWheelBase: Inventory_Base
	{
		scope = 1;
		mass = 80.0;
		radius = 0.536;
		width = 0.316;
	};
	class ExpansionVehicleBase: Inventory_Base
	{
		scope = 1;
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
		scope = 1;
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
		scope = 1;
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
	class ExpansionSoundProxyBase: CarScript
	{
		scope = 1;
	};
	class ExpansionVehicleBaseSoundProxyBase: ExpansionSoundProxyBase
	{
		scope = 1;
	};
	class ExpansionBoatScriptSoundProxyBase: ExpansionSoundProxyBase
	{
		scope = 1;
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
	class ExpansionVehicleHelicopterBase: ExpansionVehicleBase
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
		scope = 1;
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
	};
	class ExpansionTraderNPCBase: DayZPlayer
	{
		scope = 2;
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderMirek: SurvivorM_Mirek
	{
		scope = 2;
		displayName = "Mirek";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderDenis: SurvivorM_Denis
	{
		scope = 2;
		displayName = "Denis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderBoris: SurvivorM_Boris
	{
		scope = 2;
		displayName = "Boris";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderCyril: SurvivorM_Cyril
	{
		scope = 2;
		displayName = "Cyril";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderElias: SurvivorM_Elias
	{
		scope = 2;
		displayName = "Elias";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderFrancis: SurvivorM_Francis
	{
		scope = 2;
		displayName = "Francis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderGuo: SurvivorM_Guo
	{
		scope = 2;
		displayName = "Guo";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderHassan: SurvivorM_Hassan
	{
		scope = 2;
		displayName = "Hassan";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderIndar: SurvivorM_Indar
	{
		scope = 2;
		displayName = "Indar";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderJose: SurvivorM_Jose
	{
		scope = 2;
		displayName = "Jose";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderKaito: SurvivorM_Kaito
	{
		scope = 2;
		displayName = "Kaito";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderLewis: SurvivorM_Lewis
	{
		scope = 2;
		displayName = "Lewis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderManua: SurvivorM_Manua
	{
		scope = 2;
		displayName = "Manua";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderNiki: SurvivorM_Niki
	{
		scope = 2;
		displayName = "Niki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderOliver: SurvivorM_Oliver
	{
		scope = 2;
		displayName = "Oliver";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderPeter: SurvivorM_Peter
	{
		scope = 2;
		displayName = "Peter";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderQuinn: SurvivorM_Quinn
	{
		scope = 2;
		displayName = "Quinn";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderRolf: SurvivorM_Rolf
	{
		scope = 2;
		displayName = "Rolf";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderSeth: SurvivorM_Seth
	{
		scope = 2;
		displayName = "Seth";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderTaiki: SurvivorM_Taiki
	{
		scope = 2;
		displayName = "Taiki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderLinda: SurvivorF_Linda
	{
		scope = 2;
		displayName = "Linda";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderMaria: SurvivorF_Maria
	{
		scope = 2;
		displayName = "Maria";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderFrida: SurvivorF_Frida
	{
		scope = 2;
		displayName = "Frida";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderGabi: SurvivorF_Gabi
	{
		scope = 2;
		displayName = "Gabi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderHelga: SurvivorF_Helga
	{
		scope = 2;
		displayName = "Helga";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderIrena: SurvivorF_Irena
	{
		scope = 2;
		displayName = "Irena";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderJudy: SurvivorF_Judy
	{
		scope = 2;
		displayName = "Judy";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderKeiko: SurvivorF_Keiko
	{
		scope = 2;
		displayName = "Keiko";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderEva: SurvivorF_Eva
	{
		scope = 2;
		displayName = "Eva";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderNaomi: SurvivorF_Naomi
	{
		scope = 2;
		displayName = "Naomi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionNpcDenis: SurvivorM_Denis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcBoris: SurvivorM_Boris
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcCyril: SurvivorM_Cyril
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcElias: SurvivorM_Elias
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcFrancis: SurvivorM_Francis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcGuo: SurvivorM_Guo
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcHassan: SurvivorM_Hassan
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcIndar: SurvivorM_Indar
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcJose: SurvivorM_Jose
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcKaito: SurvivorM_Kaito
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcLewis: SurvivorM_Lewis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcManua: SurvivorM_Manua
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcNiki: SurvivorM_Niki
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcOliver: SurvivorM_Oliver
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcPeter: SurvivorM_Peter
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcQuinn: SurvivorM_Quinn
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcRolf: SurvivorM_Rolf
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcSeth: SurvivorM_Seth
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcTaiki: SurvivorM_Taiki
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcLinda: SurvivorF_Linda
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcMaria: SurvivorF_Maria
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcFrida: SurvivorF_Frida
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcGabi: SurvivorF_Gabi
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcHelga: SurvivorF_Helga
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcIrena: SurvivorF_Irena
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcJudy: SurvivorF_Judy
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcKeiko: SurvivorF_Keiko
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcEva: SurvivorF_Eva
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcNaomi: SurvivorF_Naomi
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionSoldierMirek: SurvivorM_Mirek
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierDenis: SurvivorM_Denis
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierBoris: SurvivorM_Boris
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierCyril: SurvivorM_Cyril
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierElias: SurvivorM_Elias
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierFrancis: SurvivorM_Francis
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierGuo: SurvivorM_Guo
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierHassan: SurvivorM_Hassan
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierIndar: SurvivorM_Indar
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierJose: SurvivorM_Jose
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierKaito: SurvivorM_Kaito
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierLewis: SurvivorM_Lewis
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierManua: SurvivorM_Manua
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierNiki: SurvivorM_Niki
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierOliver: SurvivorM_Oliver
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierPeter: SurvivorM_Peter
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierQuinn: SurvivorM_Quinn
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierRolf: SurvivorM_Rolf
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierSeth: SurvivorM_Seth
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierTaiki: SurvivorM_Taiki
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierLinda: SurvivorF_Linda
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierMaria: SurvivorF_Maria
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierFrida: SurvivorF_Frida
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierGabi: SurvivorF_Gabi
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierHelga: SurvivorF_Helga
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierIrena: SurvivorF_Irena
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierJudy: SurvivorF_Judy
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierKeiko: SurvivorF_Keiko
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierEva: SurvivorF_Eva
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldierNaomi: SurvivorF_Naomi
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Mirek: SurvivorM_Mirek
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Denis: SurvivorM_Denis
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Boris: SurvivorM_Boris
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Cyril: SurvivorM_Cyril
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Elias: SurvivorM_Elias
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Francis: SurvivorM_Francis
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Guo: SurvivorM_Guo
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Hassan: SurvivorM_Hassan
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Indar: SurvivorM_Indar
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Jose: SurvivorM_Jose
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Kaito: SurvivorM_Kaito
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Lewis: SurvivorM_Lewis
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Manua: SurvivorM_Manua
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Niki: SurvivorM_Niki
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Oliver: SurvivorM_Oliver
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Peter: SurvivorM_Peter
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Quinn: SurvivorM_Quinn
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Rolf: SurvivorM_Rolf
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Seth: SurvivorM_Seth
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Taiki: SurvivorM_Taiki
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Linda: SurvivorF_Linda
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Maria: SurvivorF_Maria
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Frida: SurvivorF_Frida
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Gabi: SurvivorF_Gabi
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Helga: SurvivorF_Helga
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Irena: SurvivorF_Irena
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Judy: SurvivorF_Judy
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Keiko: SurvivorF_Keiko
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Eva: SurvivorF_Eva
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionSoldier2Naomi: SurvivorF_Naomi
	{
		scope = 2;
		vehicleClass = "Expansion_Soldier";
	};
	class ExpansionMoneyBase: Inventory_Base
	{
		scope = 2;
		vehicleClass = "Expansion_Currency";
	};
	class ExpansionATMLocker: HouseNoDestruct
	{
		scope = 1;
		vehicleClass = "Expansion_Static";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
	};
};
