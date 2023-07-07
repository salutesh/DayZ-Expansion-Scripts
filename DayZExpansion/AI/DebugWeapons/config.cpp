#define _ARMA_

class CfgPatches
{
	class DZ_Expansion_AI_Weapons_Debug
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Ammunition","DZ_Weapons_Firearms","DZ_Weapons_Firearms_AKM","DZ_Weapons_Magazines","DZ_Weapons_Projectiles"};
	};
};
class CfgAmmoTypes
{
	class AType_Bullet_762x39Tracer_eAI_Debug
	{
		name = "Bullet_762x39Tracer_eAI_Debug";
	};
};
class CfgAmmo
{
	class Bullet_762x39Tracer;
	class Bullet_762x39Tracer_eAI_Debug: Bullet_762x39Tracer
	{
		spawnPileType = "Ammo_762x39Tracer_eAI_Debug";
		damageBarrel = 0;
		damageBarrelDestroyed = 0;
		class DamageApplied
		{
			type = "Projectile";
			dispersion = 0;
			bleedThreshold = 0;
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
};
class CfgMagazines
{
	class Ammo_762x39Tracer;
	class Mag_AKM_Drum75Rnd;
	class Ammo_762x39Tracer_eAI_Debug: Ammo_762x39Tracer
	{
		scope = 2;
		displayName = "Expansion AI Debug AKM Tracer Ammo";
		descriptionShort = "Special ammo that does no health damage.";
		ammo = "Bullet_762x39Tracer_eAI_Debug";
	};
	class Mag_AKM_Drum_eAI_Debug: Mag_AKM_Drum75Rnd
	{
		scope = 2;
		displayName = "Expansion AI Debug AKM Mag";
		descriptionShort = "Uses special ammo that does no health damage.";
		ammo = "Bullet_762x39Tracer_eAI_Debug";
		ammoItems[] = {"Ammo_762x39Tracer_eAI_Debug"};
	};
};
class CfgWeapons
{
	class AKM_Base;
	class AKM_eAI_Debug: AKM_Base
	{
		scope = 2;
		displayName = "Expansion AI Debug AKM";
		descriptionShort = "Uses special ammo that does no health damage.";
		model = "\dz\weapons\firearms\AKM\AKM.p3d";
		attachments[] = {"weaponButtstockAK","WeaponHandguardAK","weaponWrap","weaponOpticsAK","weaponFlashlight","weaponMuzzleAK","weaponBayonetAK"};
		itemSize[] = {8,3};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\AKM\data\akm_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\weapons\firearms\AKM\data\AKM.rvmat"};
		chamberableFrom[] = {"Ammo_762x39Tracer_eAI_Debug"};
		magazines[] = {"Mag_AKM_Drum_eAI_Debug"};
		class Particles
		{
			class OnFire
			{
				class SmokeCloud
				{
					overrideParticle = "weapon_shot_winded_smoke";
				};
				class MuzzleFlash
				{
					overrideParticle = "weapon_shot_akm_01";
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
					positionOffset[] = {-0.05,0,0};
				};
				class ChamberSmokeBurst
				{
					overrideParticle = "weapon_shot_chamber_smoke";
					overridePoint = "Nabojnicestart";
					overrideDirectionPoint = "Nabojniceend";
				};
			};
			class OnOverheating
			{
				maxOverheatingValue = 60;
				shotsToStartOverheating = 25;
				overheatingDecayInterval = 1;
				class SmokingBarrel1
				{
					overrideParticle = "smoking_barrel_small";
					onlyWithinOverheatLimits[] = {0,0.5};
					positionOffset[] = {0.1,0,0};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHot1
				{
					overrideParticle = "smoking_barrel";
					onlyWithinOverheatLimits[] = {0.5,0.8};
					positionOffset[] = {0.1,0,0};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHot3
				{
					overrideParticle = "smoking_barrel_heavy";
					onlyWithinOverheatLimits[] = {0.8,1};
					positionOffset[] = {0.1,0,0};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHotSteamSmall
				{
					overrideParticle = "smoking_barrel_steam_small";
					positionOffset[] = {0.3,0,0};
					onlyWithinRainLimits[] = {0.2,0.5};
				};
				class SmokingBarrelHotSteam
				{
					overrideParticle = "smoking_barrel_steam";
					positionOffset[] = {0.3,0,0};
					onlyWithinRainLimits[] = {0.5,1};
				};
				class OpenChamberSmoke
				{
					onlyIfBoltIsOpen = 1;
					overrideParticle = "smoking_barrel_small";
					overridePoint = "Nabojnicestart";
				};
			};
			class OnBulletCasingEject
			{
				class ChamberSmokeRaise
				{
					overrideParticle = "weapon_shot_chamber_smoke_raise";
					overridePoint = "Nabojnicestart";
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 300;
					healthLevels[] = {{1,{"DZ\weapons\firearms\AKM\data\AKM.rvmat"}},{0.7,{"DZ\weapons\firearms\AKM\data\AKM.rvmat"}},{0.5,{"DZ\weapons\firearms\AKM\data\AKM_damage.rvmat"}},{0.3,{"DZ\weapons\firearms\AKM\data\AKM_damage.rvmat"}},{0,{"DZ\weapons\firearms\AKM\data\AKM_destruct.rvmat"}}};
				};
			};
		};
	};
};
