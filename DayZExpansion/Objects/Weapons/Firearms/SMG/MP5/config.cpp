#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Weapons_SMG_MP5
	{
		units[] = {"Expansion_MP5_RailAtt"};
		weapons[] = {"Expansion_MP5","Expansion_MP5SD"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Firearms"};
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
	class Expansion_MP5_Base: Rifle_Base
	{
		scope = 0;
		weight = 2540;
		absorbency = 0;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30,25};
		PPDOFProperties[] = {1,0.5,50,120,4,10};
		ironsightsExcludingOptics[] = {};
		WeaponLength = 0.699;
		barrelArmor = 5.9;
		chamberSize = 1;
		chamberedRound = "";
		chamberableFrom[] = {"Ammo_9x19"};
		magazines[] = {"Mag_MP5_15Rnd","Mag_MP5_30Rnd"};
		magazineSwitchTime = 0.5;
		ejectType = 1;
		recoilModifier[] = {1,1,1};
		simpleHiddenSelections[] = {};
		drySound[] = {"dz\sounds\weapons\firearms\m4a1\m4_dry",0.5,1,20};
		reloadAction = "ReloadMP5";
		modes[] = {"FullAuto","SemiAuto","Burst"};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[] = {"MP5K_Shot_SoundSet","MP5K_Tail_SoundSet","MP5K_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			reloadTime = 0.12;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class Burst: Mode_Burst
		{
			soundSetShot[] = {"MP5K_Shot_SoundSet","MP5K_Tail_SoundSet","MP5K_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			burst = 3;
			reloadTime = 0.065;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class FullAuto: Mode_FullAuto
		{
			soundSetShot[] = {"MP5K_Shot_SoundSet","MP5K_Tail_SoundSet","MP5K_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			reloadTime = 0.065;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					overrideParticle = "weapon_shot_mp5k_01";
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
				};
				class SmokeCloud
				{
					overrideParticle = "weapon_shot_winded_smoke_small";
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
				shotsToStartOverheating = 3;
				maxOverheatingValue = 10;
				overheatingDecayInterval = 1;
				class SmokingBarrel
				{
					overrideParticle = "smoking_barrel_small";
					onlyWithinOverheatLimits[] = {0,0.8};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHotSteam
				{
					overrideParticle = "smoking_barrel_steam_small";
					positionOffset[] = {0.1,0.02,0};
					onlyWithinOverheatLimits[] = {0,1};
					onlyWithinRainLimits[] = {0.2,1};
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
		class OpticsInfo: OpticsInfoRifle
		{
			memoryPointCamera = "eye";
			modelOptics = "-";
			distanceZoomMin = 100;
			distanceZoomMax = 100;
			discreteDistance[] = {25};
			discreteDistanceInitIndex = 0;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class walkErc_L
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 1;
				};
				class walkErc_R
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_R_SoundSet";
					id = 2;
				};
				class runErc_L
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 3;
				};
				class runErc_R
				{
					soundSet = "Weapon_Movement_Rifle_runErc_R_SoundSet";
					id = 4;
				};
				class sprintErc_L
				{
					soundSet = "Weapon_Movement_Rifle_sprintErc_SoundSet";
					id = 5;
				};
				class sprintErc_R
				{
					soundSet = "Weapon_Movement_Rifle_sprintErc_SoundSet";
					id = 6;
				};
				class walkCro_L
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 21;
				};
				class walkCro_R
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 22;
				};
				class runCro_L
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 23;
				};
				class runCro_R
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 24;
				};
				class walkProne_L
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 31;
				};
				class walkProne_R
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 32;
				};
				class runProne_L
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 33;
				};
				class runProne_R
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 34;
				};
				class jumpErc_L
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 35;
				};
				class jumpErc_R
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 36;
				};
				class landFootErc_L
				{
					soundSet = "Weapon_Movement_Rifle_landFootErc_SoundSet";
					id = 37;
				};
				class landFootErc_R
				{
					soundSet = "Weapon_Movement_Rifle_landFootErc_SoundSet";
					id = 38;
				};
				class walkRasErc_L
				{
					soundSet = "Weapon_Movement_Rifle_walkRasErc_SoundSet";
					id = 51;
				};
				class walkRasErc_R
				{
					soundSet = "Weapon_Movement_Rifle_walkRasErc_SoundSet";
					id = 52;
				};
				class runRasErc_L
				{
					soundSet = "Weapon_Movement_Rifle_runRasErc_SoundSet";
					id = 53;
				};
				class runRasErc_R
				{
					soundSet = "Weapon_Movement_Rifle_runRasErc_SoundSet";
					id = 54;
				};
				class HandStep_L
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 61;
				};
				class HandStep_R
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 62;
				};
				class HandStep_Hard_L
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 63;
				};
				class HandStep_Hard_R
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 64;
				};
				class landFeetErc
				{
					soundSet = "Weapon_Movement_Rifle_landFeetErc_SoundSet";
					id = 100;
				};
				class Weapon_Movement_Rifle_Walk
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 101;
				};
				class Weapon_Movement_Rifle_Run
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 102;
				};
				class Weapon_Movement_Rifle_Sprint
				{
					soundSet = "Weapon_Movement_Rifle_sprintErc_SoundSet";
					id = 103;
				};
				class Weapon_Movement_Rifle_Land
				{
					soundSet = "Weapon_Movement_Rifle_landFeetErc_SoundSet";
					id = 104;
				};
				class Char_Gestures_Hand_Grab_Rifle
				{
					soundSet = "Char_Gestures_Hand_Grab_FabricRifle_SoundSet";
					id = 892;
				};
				class drop
				{
					soundset = "rifle_drop_SoundSet";
					id = 898;
				};
				class bodyfall_rifle_light
				{
					soundSet = "pickUpRifleLight_SoundSet";
					id = 13400;
				};
				class bodyfall_rifle
				{
					soundset = "pickUpRifle_SoundSet";
					id = 13401;
				};
			};
		};
	};
	class Expansion_MP5: Expansion_MP5_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MP5A2";
		descriptionShort = "$STR_EXPANSION_MP5A2_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\SMG\MP5\mp5a2.p3d";
		attachments[] = {"Expansion_MP5ScopeRail","weaponOptics","weaponMuzzleMP5","pistolMuzzle","weaponWrap"};
		itemSize[] = {6,3};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 125;
					healthLevels[] = {{1.0,{"DZ\weapons\firearms\MP5\data\mp5k_body.rvmat"}},{0.7,{"DZ\weapons\firearms\MP5\data\mp5k_body.rvmat"}},{0.5,{"DZ\weapons\firearms\MP5\data\mp5k_body_damage.rvmat"}},{0.3,{"DZ\weapons\firearms\MP5\data\mp5k_body_damage.rvmat"}},{0.0,{"DZ\weapons\firearms\MP5\data\mp5k_body_destruct.rvmat"}}};
				};
			};
		};
	};
	class Expansion_MP5SD: Expansion_MP5_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MP5SD";
		descriptionShort = "$STR_EXPANSION_MP5SD_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\SMG\MP5\mp5sd.p3d";
		ironsightsExcludingOptics[] = {"M4_CarryHandleOptic","BUISOptic","M68Optic","M4_T3NRDSOptic","ReflexOptic","ACOGOptic"};
		attachments[] = {"Expansion_MP5ScopeRail","weaponOptics","weaponWrap"};
		itemSize[] = {5,3};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[] = {"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			reloadTime = 0.12;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class Burst: Mode_Burst
		{
			soundSetShot[] = {"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			burst = 3;
			reloadTime = 0.065;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class FullAuto: Mode_FullAuto
		{
			soundSetShot[] = {"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			reloadTime = 0.065;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					overrideParticle = "weapon_shot_vss_01";
					ignoreIfSuppressed = 0;
					illuminateWorld = 0;
				};
				class SmokeCloud
				{
					overrideParticle = "weapon_shot_winded_smoke_small";
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
				shotsToStartOverheating = 3;
				maxOverheatingValue = 10;
				overheatingDecayInterval = 1;
				class SmokingBarrel
				{
					overrideParticle = "smoking_barrel_small";
					onlyWithinOverheatLimits[] = {0,0.8};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrelHotSteam
				{
					overrideParticle = "smoking_barrel_steam_small";
					positionOffset[] = {0.1,0.02,0};
					onlyWithinOverheatLimits[] = {0,1};
					onlyWithinRainLimits[] = {0.2,1};
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
					hitpoints = 125;
					healthLevels[] = {{1.0,{"DZ\weapons\firearms\MP5\data\mp5k_body.rvmat"}},{0.7,{"DZ\weapons\firearms\MP5\data\mp5k_body.rvmat"}},{0.5,{"DZ\weapons\firearms\MP5\data\mp5k_body_damage.rvmat"}},{0.3,{"DZ\weapons\firearms\MP5\data\mp5k_body_damage.rvmat"}},{0.0,{"DZ\weapons\firearms\MP5\data\mp5k_body_destruct.rvmat"}}};
				};
			};
		};
	};
};
class cfgVehicles
{
	class Inventory_Base;
	class Expansion_MP5_RailAtt: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MP5_SCOPE_RAIL";
		descriptionShort = "$STR_EXPANSION_MP5_SCOPE_RAIL_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\SMG\MP5\mp5railmount.p3d";
		rotationFlags = 17;
		reversed = 0;
		weight = 330;
		itemSize[] = {1,2};
		inventorySlot = "Expansion_MP5ScopeRail";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"DZ\weapons\attachments\data\akm_accessories.rvmat"}},{0.7,{"DZ\weapons\attachments\data\akm_accessories.rvmat"}},{0.5,{"DZ\weapons\attachments\data\akm_accessories_damage.rvmat"}},{0.3,{"DZ\weapons\attachments\data\akm_accessories_damage.rvmat"}},{0,{"DZ\weapons\attachments\data\akm_accessories_destruct.rvmat"}}};
				};
			};
		};
		isMeleeWeapon = 1;
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeLightBlunt";
				range = 1;
			};
			class Heavy
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 1;
			};
			class Sprint
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 2.8;
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyMP5Railmount: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Expansion_MP5ScopeRail";
		model = "\DayZExpansion\Objects\Weapons\Firearms\SMG\MP5\mp5railmount.p3d";
	};
};
class CfgSlots
{
	class Slot_Expansion_MP5ScopeRail
	{
		name = "Expansion_MP5ScopeRail";
		displayName = "$STR_EXPANSION_MP5_SCOPE_RAIL";
		ghostIcon = "handguard";
	};
};
