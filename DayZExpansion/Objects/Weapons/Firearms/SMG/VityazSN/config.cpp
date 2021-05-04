#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Weapons_SMG_VityazSN
	{
		units[] = {};
		weapons[] = {"Expansion_VityazSN"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Firearms"};
		magazines[] = {"Mag_Expansion_Vityaz_30Rnd"};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoRifle;
class cfgWeapons
{
	class Rifle_Base;
	class Expansion_Vityaz_Base: Rifle_Base
	{
		scope = 0;
		weight = 2900;
		absorbency = 0.1;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30,25};
		discreteDistance[] = {25};
		discreteDistanceInitIndex = 0;
		modelOptics = "-";
		distanceZoomMin = 100;
		distanceZoomMax = 100;
		PPDOFProperties[] = {};
		optics = 1;
		opticsFlare = 0;
		ironsightsExcludingOptics[] = {"KashtanOptic","GrozaOptic","KobraOptic","M68Optic","M4_T3NRDSOptic","ReflexOptic","AcogOptic"};
		WeaponLength = 0.45;
		swayModifier[] = {1.25,1.25,1};
		value = 0;
		chamberSize = 1;
		chamberedRound = "";
		chamberableFrom[] = {"Ammo_9x19"};
		magazines[] = {"Mag_Expansion_Vityaz_30Rnd"};
		magazineSwitchTime = 0.2;
		barrelArmor = 3000;
		ejectType = 1;
		recoilModifier[] = {1,1,1};
		drySound[] = {"dz\sounds\weapons\firearms\m4a1\m4_dry",0.5,1,20};
		reloadAction = "ReloadAKM";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\data\VityazSN_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\data\PP1901.rvmat"};
		modes[] = {"SemiAuto","FullAuto"};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[] = {"MP5K_Shot_SoundSet","MP5K_Tail_SoundSet","MP5K_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			reloadTime = 0.08;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class FullAuto: Mode_FullAuto
		{
			soundSetShot[] = {"MP5K_Shot_SoundSet","MP5K_Tail_SoundSet","MP5K_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			reloadTime = 0.08;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class OpticsInfo: OpticsInfoRifle
		{
			memoryPointCamera = "eye";
			modelOptics = "-";
			distanceZoomMin = 100;
			distanceZoomMax = 100;
			discreteDistance[] = {25};
			discreteDistanceInitIndex = 0;
		};
	};
	class Expansion_VityazSN: Expansion_Vityaz_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VITYAZ";
		descriptionShort = "$STR_EXPANSION_VITYAZ_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\pp1901.p3d";
		attachments[] = {"weaponButtstockAK","WeaponHandguardAK","weaponOpticsAK","weaponOptics","weaponFlashlight","weaponWrap","pistolMuzzle"};
		itemSize[] = {5,3};
		dexterity = 3.2;
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
				};
				class SmokeCloud
				{
					overrideParticle = "weapon_shot_winded_smoke_small";
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
				maxOverheatingValue = 10;
				overheatingDecayInterval = 1;
				class SmokingBarrel
				{
					overridePoint = "Nabojnicestart";
					overrideParticle = "smoking_barrel_small";
					onlyWithinOverheatLimits[] = {0,0.8};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHot
				{
					overridePoint = "Nabojnicestart";
					overrideParticle = "smoking_barrel";
					onlyWithinOverheatLimits[] = {0.8,1};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHotSteam
				{
					overrideParticle = "smoking_barrel_steam_small";
					positionOffset[] = {0.1,0.02,0};
					onlyWithinOverheatLimits[] = {0,1};
					onlyWithinRainLimits[] = {0.2,1};
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\data\PP1901.rvmat"}},{0.7,{"DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\data\PP1901.rvmat"}},{0.5,{"DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\data\PP1901_damaged.rvmat"}},{0.3,{"DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\data\PP1901_damaged.rvmat"}},{0.0,{"DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\data\PP1901_ruined.rvmat"}}};
				};
			};
		};
	};
};
class cfgMagazines
{
	class Magazine_Base;
	class Mag_Expansion_Vityaz_30Rnd: Magazine_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VITYAZ_MAG";
		descriptionShort = "$STR_EXPANSION_VITYAZ_MAG_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\SMG\VityazSN\pp1901_magazine.p3d";
		weight = 170;
		itemSize[] = {1,3};
		count = 30;
		ammo = "Bullet_9x19";
		ammoItems[] = {"Ammo_9x19"};
		tracersEvery = 0;
		mass = 10;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"DZ\weapons\attachments\data\mp5k_mag.rvmat"}},{0.7,{}},{0.5,{"DZ\weapons\attachments\data\mp5k_mag_damage.rvmat"}},{0.3,{}},{0,{"DZ\weapons\attachments\data\mp5k_mag_destruct.rvmat"}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class MagRifle_fill_in
				{
					soundSet = "MagRifle_fill_in_SoundSet";
					id = 1;
				};
				class MagRifle_fill_loop
				{
					soundSet = "MagRifle_fill_loop_SoundSet";
					id = 2;
				};
				class MagRifle_fill_out
				{
					soundSet = "MagRifle_fill_out_SoundSet";
					id = 3;
				};
				class MagRifle_empty_in
				{
					soundSet = "MagRifle_empty_in_SoundSet";
					id = 4;
				};
				class MagRifle_empty_loop
				{
					soundSet = "MagRifle_empty_loop_SoundSet";
					id = 5;
				};
				class MagRifle_empty_out
				{
					soundSet = "MagRifle_empty_out_SoundSet";
					id = 6;
				};
				class MagPistol_fill_in
				{
					soundSet = "MagPistol_fill_in_SoundSet";
					id = 7;
				};
				class MagPistol_fill_loop
				{
					soundSet = "MagPistol_fill_loop_SoundSet";
					id = 8;
				};
				class MagPistol_fill_out
				{
					soundSet = "MagPistol_fill_out_SoundSet";
					id = 9;
				};
				class MagPistol_empty_in
				{
					soundSet = "MagPistol_empty_in_SoundSet";
					id = 10;
				};
				class MagPistol_empty_loop
				{
					soundSet = "MagPistol_empty_loop_SoundSet";
					id = 11;
				};
				class MagPistol_empty_out
				{
					soundSet = "MagPistol_empty_out_SoundSet";
					id = 12;
				};
			};
		};
	};
};
