#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Firearms_Flaregun
	{
		units[] = {};
		weapons[] = {"ExpansionFlaregun"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Weapons_Firearms","DZ_Pistols_FNX45"};
		magazines[] = {"ExpansionAmmoFlare","ExpansionAmmoFlareLight"};
		ammo[] = {"ExpansionBulletFlare","ExpansionBulletFlareLight"};
	};
};
class CfgAmmoTypes
{
	class AType_ExpansionBulletFlare
	{
		name = "ExpansionBulletFlare";
	};
	class AType_ExpansionBulletFlareLight
	{
		name = "ExpansionBulletFlareLight";
	};
};
class CfgAmmo
{
	class Bullet_762x39;
	class ExpansionBulletFlare: Bullet_762x39
	{
		scope = 2;
		model = "\dz\data\lightpoint.p3d";
		spawnPileType = "Ammo_762x39Tracer";
		tracerScale = 1.2;
		tracerStartTime = 0.075;
		tracerEndTime = 3;
		initSpeed = 100;
		typicalSpeed = 100;
		class DamageApplied
		{
			type = "Projectile";
			dispersion = 0;
			bleedThreshold = 0.0;
			Health = 0;
			Blood = 0;
			Shock = 0;
		};
	};
	class ExpansionBulletFlareLight: Bullet_762x39
	{
		scope = 2;
		model = "\dz\data\lightpoint.p3d";
		spawnPileType = "Ammo_762x39Tracer";
		tracerScale = 1.2;
		tracerStartTime = 0.075;
		tracerEndTime = 3;
		initSpeed = 100;
		typicalSpeed = 100;
		class DamageApplied
		{
			type = "Projectile";
			dispersion = 0;
			bleedThreshold = 0.0;
			Health = 0;
			Blood = 0;
			Shock = 0;
		};
	};
};
class CfgMagazines
{
	class Ammunition_Base;
	class ExpansionAmmoFlare: Ammunition_Base
	{
		scope = 2;
		itemSize[] = {1,1};
		displayName = "$STR_EXPANSION_FLARE";
		descriptionShort = "$STR_EXPANSION_FLARE_DESC";
		model = "\dz\weapons\ammunition\Flare_SingleRound.p3d";
		rotationFlags = 34;
		weight = 80;
		count = 1;
		ammo = "ExpansionBulletFlare";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"}},{0.7,{"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"}},{0.5,{"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"}},{0.3,{"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"}},{0.0,{"DZ\weapons\pistols\flaregun\data\flaregun_destruct.rvmat"}}};
				};
			};
		};
	};
	class ExpansionAmmoFlareLight: Ammunition_Base
	{
		scope = 2;
		itemSize[] = {1,1};
		displayName = "$STR_EXPANSION_FLARE";
		descriptionShort = "$STR_EXPANSION_FLARE_DESC";
		model = "\dz\weapons\ammunition\Flare_SingleRound.p3d";
		rotationFlags = 34;
		weight = 80;
		count = 1;
		ammo = "ExpansionBulletFlareLight";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"}},{0.7,{"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"}},{0.5,{"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"}},{0.3,{"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"}},{0.0,{"DZ\weapons\pistols\flaregun\data\flaregun_destruct.rvmat"}}};
				};
			};
		};
	};
};
class CfgWeapons
{
	class Mode_SemiAuto;
	class FNX45;
	class ExpansionFlaregunBase: FNX45
	{
		scope = 0;
		lootTag[] = {"Military_east","Military_west","Civilian","Police"};
		weight = 400;
		absorbency = 0.0;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30.0,25.0};
		modelOptics = "-";
		distanceZoomMin = 100;
		distanceZoomMax = 100;
		optics = 1;
		value = 0;
		chamberSize = 1;
		chamberedRound = "";
		magazines[] = {};
		chamberableFrom[] = {"ExpansionAmmoFlareLight","ExpansionAmmoFlare"};
		barrelArmor = 2000;
		ejectType = 2;
		recoilModifier[] = {1,1,1};
		reloadAction = "reloadFlaregun";
		modes[] = {"Single"};
		drySound[] = {"dz\sounds\weapons\firearms\FNX45\FNX_dry",0.5,1,20};
		reloadMagazineSound[] = {"dz\sounds\weapons\firearms\FNX45\FNX45_reload",0.8,1,20};
		class Single: Mode_SemiAuto
		{
			reloadTime = 0.13;
			recoil = "recoil_flaregun";
			recoilProne = "recoil_flaregun_prone";
			dispersion = 0.03;
			magazineSlot = "magazine";
		};
	};
	class ExpansionFlaregun: ExpansionFlaregunBase
	{
		scope = 2;
		vehicleClass = "Expansion_Airdrop";
		displayName = "$STR_EXPANSION_FLARE_GUN";
		descriptionShort = "$STR_EXPANSION_FLARE_GUN_DESC";
		model = "\dz\weapons\pistols\flaregun\flaregun.p3d";
		attachments[] = {};
		baseAttachments[] = {};
		itemSize[] = {3,2};
		dexterity = 3.6;
		hiddenSelections[] = {"zasleh"};
		hiddenSelectionsTextures[] = {""};
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
				class MuzzleFlashForward
				{
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
					overrideParticle = "weapon_shot_fnx_01";
				};
				class ChamberSmoke
				{
					overrideParticle = "weapon_shot_chamber_smoke";
					overridePoint = "Nabojnicestart";
					overrideDirectionPoint = "Nabojniceend";
				};
				class ChamberSmokeRaise
				{
					overrideParticle = "weapon_shot_chamber_smoke_raise";
					overridePoint = "Nabojnicestart";
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating = 3;
				maxOverheatingValue = 7;
				overheatingDecayInterval = 1;
				class SmokingBarrel
				{
					overridePoint = "Nabojnicestart";
					overrideParticle = "smoking_barrel_small";
				};
			};
		};
	};
};
class CfgVehicles
{
	class Roadflare;
	class ExpansionRoadflare: Roadflare
	{
		scope = 1;
		displayName = "$STR_EXPANSION_FLARE";
		descriptionShort = "...";
		model = "\DayZExpansion\Objects\Weapons\Firearms\Flaregun\flare.p3d";
		forceFarBubble = "true";
	};
};
