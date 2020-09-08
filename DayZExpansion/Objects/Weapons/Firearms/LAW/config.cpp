#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Firearms_LAW
	{
		units[] = {"ExpansionRocket"};
		weapons[] = {"ExpansionLAW"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Weapons_Firearms","DayZExpansion_Sounds_Weapons_RPG7"};
		magazines[] = {"ExpansionAmmoLAW"};
		ammo[] = {"DummyShockDamage","ExpansionRocketLAW"};
	};
};
class CfgAmmoTypes
{
	class AType_ExpansionRocketLAW
	{
		name = "ExpansionRocketLAW";
	};
};
class CfgAmmo
{
	class Bullet_Base;
	class MeleeDamage;
	class DummyShockDamage: MeleeDamage
	{
		hitAnimation = 0;
		class DamageApplied
		{
			type = "DummyShockHit";
			class Health
			{
				damage = 0;
			};
			class Blood
			{
				damage = 0;
			};
			class Shock
			{
				damage = 1;
			};
		};
	};
	class ExpansionRocketLAW: Bullet_Base
	{
		scope = 2;
		casing = "FxCartridge_Expansion_M203";
		round = "FxRound_Expansion_M203";
		spawnPileType = "ExpansionAmmoRPG";
		hit = 0;
		model = "\dz\weapons\ammunition\rocket_rpg7_inflight.p3d";
		indirectHit = 0;
		indirectHitRange = 0;
		tracerScale = 2;
		caliber = 0.01;
		deflecting = 5;
		impactBehaviour = 1;
		initSpeed = 115;
		typicalSpeed = 115;
		timeToLive = 30;
		airFriction = -0.00315;
		weight = 2.6;
		supersonicCrackNear[] = {};
		supersonicCrackFar[] = {};
		class DamageApplied
		{
			type = "Projectile";
			dispersion = 0;
			bleedThreshold = 1;
			defaultDamageOverride[] = {{0.5,1}};
			class Health
			{
				damage = 5;
			};
			class Blood
			{
				damage = 0;
			};
			class Shock
			{
				damage = 15;
			};
		};
		class NoiseHit
		{
			strength = 15;
			type = "shot";
		};
	};
};
class CfgMagazines
{
	class Ammunition_Base;
	class ExpansionAmmoLAWHE: Ammunition_Base
	{
		scope = 0;
		displayName = "$STR_CfgMagazines_Ammo_762x39Tracer0";
		descriptionShort = "$STR_CfgMagazines_Ammo_762x39Tracer1";
		model = "\dz\weapons\launchers\law\law_rocket.p3d";
		weight = 8;
		count = 20;
		ammo = "Bullet_762x39Tracer";
		muzzleFlashParticle = "weapon_shot_izh18_01";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"dz\weapons\ammunition\data\762x39_tracer_co.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\ammunition\data\762x39.rvmat"}},{0.7,{"DZ\weapons\ammunition\data\762x39.rvmat"}},{0.5,{"DZ\weapons\ammunition\data\762x39_damage.rvmat"}},{0.3,{"DZ\weapons\ammunition\data\762x39_damage.rvmat"}},{0.0,{"DZ\weapons\ammunition\data\762x39_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionAmmoLAW: Ammunition_Base
	{
		scope = 2;
		itemSize[] = {4,2};
		displayName = "$STR_EXPANSION_M27_LAW_WARHEAD";
		descriptionShort = "$STR_CfgMagazines_Ammo_LAW_HE1";
		model = "\dz\weapons\launchers\law\law_rocket.p3d";
		rotationFlags = 34;
		weight = 1800;
		count = 1;
		ammo = "ExpansionRocketLAW";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\launchers\law\data\law_rocket.rvmat"}},{0.7,{"DZ\weapons\launchers\law\data\law_rocket.rvmat"}},{0.5,{"DZ\weapons\launchers\law\data\law_rocket_damage.rvmat"}},{0.3,{"DZ\weapons\launchers\law\data\law_rocket_damage.rvmat"}},{0.0,{"DZ\weapons\launchers\law\data\law_rocket_destruct.rvmat"}}};
				};
			};
		};
	};
};
class CfgWeapons
{
	class SKS;
	class Mode_SemiAuto;
	class ExpansionLAWBase: SKS
	{
		scope = 0;
		weight = 3200;
		absorbency = 0;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30,25};
		PPDOFProperties[] = {};
		ironsightsExcludingOptics[] = {};
		DisplayMagazine = 0;
		WeaponLength = 0.78105503;
		chamberSize = 1;
		chamberedRound = "";
		chamberableFrom[] = {"ExpansionAmmoLAW"};
		magazines[] = {};
		ejectType = 3;
		recoilModifier[] = {2.5,2.5,2.5};
		swayModifier[] = {1,1,1};
		reloadAction = "ReloadIZH18";
		shotAction = "";
		modes[] = {"Single"};
		class Single: Mode_SemiAuto
		{
			soundSetShot[] = {"Expansion_RPG_Shot_SoundSet","IZH18_Tail_SoundSet","IZH18_InteriorTail_SoundSet"};
			reloadTime = 1;
			recoil = "recoil_izh18";
			recoilProne = "recoil_izh18_prone";
			dispersion = 0.001;
			magazineSlot = "magazine";
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
					overridePoint = "Usti hlavne";
				};
				class ChamberFlash
				{
					overridePoint = "dust_point";
					overrideParticle = "weapon_dust_izh18_01";
				};
			};
		};
	};
	class ExpansionLAW: ExpansionLAWBase
	{
		scope = 2;
		vehicleClass = "Expansion_Launchers";
		displayName = "$STR_EXPANSION_M27_LAW";
		descriptionShort = "$STR_cfgWeapons_LAW1";
		model = "\dz\weapons\launchers\law\law.p3d";
		attachments[] = {};
		baseAttachments[] = {};
		itemSize[] = {8,6};
		dexterity = 3.6;
		class Damage
		{
			tex[] = {};
			mat[] = {"DZ\weapons\pistols\flaregun\data\flaregun.rvmat","DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat","DZ\weapons\pistols\flaregun\data\flaregun_destruct.rvmat"};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLabels[] = {1,0.7,0.5,0.3,0};
					healthLevels[] = {{1,{"DZ\weapons\pistols\fnx45\data\herstal45.rvmat"}},{0.5,{"DZ\weapons\pistols\fnx45\data\herstal45_damage.rvmat"}},{0,{"DZ\weapons\pistols\fnx45\data\herstal45_destruct.rvmat"}}};
				};
			};
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash{};
				class SmokeCloud1
				{
					overrideParticle = "weapon_shot_winded_smoke";
				};
				class MuzzleFlash2
				{
					overrideParticle = "weapon_shot_m4a1_01";
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
				};
				class MuzzleFlashStar
				{
					overrideParticle = "weapon_shot_Flame_3D_4star";
					overridePoint = "StarFlash";
				};
				class GasPistonBurstR
				{
					overrideParticle = "weapon_shot_chamber_smoke";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {0,0,0};
				};
				class GasPistonBurstL
				{
					overrideParticle = "weapon_shot_chamber_smoke";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {180,0,0};
				};
				class GasPistonSmokeRaiseR
				{
					overrideParticle = "weapon_shot_chamber_smoke_raise";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {0,0,0};
				};
				class GasPistonSmokeRaiseL
				{
					overrideParticle = "weapon_shot_chamber_smoke_raise";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {180,0,0};
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating = 3;
				maxOverheatingValue = 30;
				overheatingDecayInterval = 2;
				class SmokingBarrel1
				{
					overridePoint = "GasPiston";
					positionOffset[] = {0.05,-0.02,0};
					overrideParticle = "smoking_barrel_small";
					onlyWithinOverheatLimits[] = {0.0,0.2};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHot1
				{
					overridePoint = "GasPiston";
					positionOffset[] = {0.12,-0.02,0};
					overrideParticle = "smoking_barrel";
					onlyWithinOverheatLimits[] = {0.2,0.6};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHot3
				{
					overridePoint = "GasPiston";
					positionOffset[] = {0.21,-0.02,0};
					overrideParticle = "smoking_barrel_heavy";
					onlyWithinOverheatLimits[] = {0.6,1};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class Steam
				{
					overrideParticle = "smoking_barrel_steam_small";
					positionOffset[] = {0.4,0.02,0};
					onlyWithinOverheatLimits[] = {0,0.5};
					onlyWithinRainLimits[] = {0.2,1};
				};
				class Steam2
				{
					overrideParticle = "smoking_barrel_steam";
					positionOffset[] = {0.45,0.02,0};
					onlyWithinOverheatLimits[] = {0.5,1};
					onlyWithinRainLimits[] = {0.2,1};
				};
				class ChamberSmokeRaise
				{
					overrideParticle = "smoking_barrel_small";
					overridePoint = "Nabojnicestart";
					onlyWithinOverheatLimits[] = {0.5,1};
				};
			};
		};
	};
};
class CfgVehicles
{
	class Roadflare;
	class ExpansionRocket: Roadflare
	{
		scope = 2;
		model = "\dz\weapons\ammunition\rocket_rpg7_inflight.p3d";
	};
};
