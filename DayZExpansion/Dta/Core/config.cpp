#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Dta_Core
	{
		units[] = {"ExpansionPickup","ExpansionPickupCone","ExpansionDebugCone","ExpansionDebugConeSmall","ExplosionPoint","ExplosionSmall","ExpansionNpcDenis","ExpansionNpcBoris","ExpansionNpcCyril","ExpansionNpcElias","ExpansionNpcFrancis","ExpansionNpcGuo","ExpansionNpcHassan","ExpansionNpcIndar","ExpansionNpcJose","ExpansionNpcKaito","ExpansionNpcLewis","ExpansionNpcManua","ExpansionNpcNiki","ExpansionNpcOliver","ExpansionNpcPeter","ExpansionNpcQuinn","ExpansionNpcRolf","ExpansionNpcSeth","ExpansionNpcTaiki","ExpansionNpcLinda","ExpansionNpcMaria","ExpansionNpcFrida","ExpansionNpcGabi","ExpansionNpcHelga","ExpansionNpcIrena","ExpansionNpcJudy","ExpansionNpcKeiko","ExpansionNpcEva","ExpansionNpcNaomi","ExpansionSoldierMirek","ExpansionSoldierDenis","ExpansionSoldierBoris","ExpansionSoldierCyril","ExpansionSoldierElias","ExpansionSoldierFrancis","ExpansionSoldierGuo","ExpansionSoldierHassan","ExpansionSoldierIndar","ExpansionSoldierJose","ExpansionSoldierKaito","ExpansionSoldierLewis","ExpansionSoldierManua","ExpansionSoldierNiki","ExpansionSoldierOliver","ExpansionSoldierPeter","ExpansionSoldierQuinn","ExpansionSoldierRolf","ExpansionSoldierSeth","ExpansionSoldierTaiki","ExpansionSoldierLinda","ExpansionSoldierMaria","ExpansionSoldierFrida","ExpansionSoldierGabi","ExpansionSoldierHelga","ExpansionSoldierIrena","ExpansionSoldierJudy","ExpansionSoldierKeiko","ExpansionSoldierEva","ExpansionSoldierNaomi","ExpansionSoldier2Mirek","ExpansionSoldier2Denis","ExpansionSoldier2Boris","ExpansionSoldier2Cyril","ExpansionSoldier2Elias","ExpansionSoldier2Francis","ExpansionSoldier2Guo","ExpansionSoldier2Hassan","ExpansionSoldier2Indar","ExpansionSoldier2Jose","ExpansionSoldier2Kaito","ExpansionSoldier2Lewis","ExpansionSoldier2Manua","ExpansionSoldier2Niki","ExpansionSoldier2Oliver","ExpansionSoldier2Peter","ExpansionSoldier2Quinn","ExpansionSoldier2Rolf","ExpansionSoldier2Seth","ExpansionSoldier2Taiki","ExpansionSoldier2Linda","ExpansionSoldier2Maria","ExpansionSoldier2Frida","ExpansionSoldier2Gabi","ExpansionSoldier2Helga","ExpansionSoldier2Irena","ExpansionSoldier2Judy","ExpansionSoldier2Keiko","ExpansionSoldier2Eva","ExpansionSoldier2Naomi"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DZ_Characters","DZ_Scripts","DZ_AI"};
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
	class Expansion_Sign
	{
		displayName = "Expansion Sign";
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
	class Inventory_Base;
	class Container_Base;
	class HouseNoDestruct;
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
		scope = 0;
		vehicleClass = "Expansion_Currency";
	};
	class ExpansionATMLocker: HouseNoDestruct
	{
		scope = 1;
		vehicleClass = "Expansion_Static";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
	};
};
