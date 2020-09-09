#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Weapons_Sniper_AWM
	{
		units[] = {"AmmoBox_Expansion_338_15rnd"};
		weapons[] = {"Expansion_AWM","Expansion_AWM_Black","Expansion_AWM_Green"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_PMII25Optic"};
		magazines[] = {"Mag_Expansion_AWM_5rnd","Ammo_Expansion_338"};
		ammo[] = {"Bullet_Expansion_338"};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoRifle;
class cfgWeapons
{
	class BoltActionRifle_Base;
	class Expansion_AWM_Base: BoltActionRifle_Base
	{
		scope = 0;
		animName = "cz527";
		weight = 2720;
		absorbency = 0;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30,25};
		modelOptics = "-";
		distanceZoomMin = 100;
		distanceZoomMax = 100;
		PPDOFProperties[] = {};
		opticsFlare = 0;
		ironsightsExcludingOptics[] = {"M68Optic","M4_T3NRDSOptic","ReflexOptic","ACOGOptic","SniperOptic"};
		WeaponLength = 1.22;
		value = 0;
		chamberSize = 1;
		chamberedRound = "";
		chamberableFrom[] = {"Ammo_Expansion_338","Ammo_AE338","Ammo_338"};
		magazines[] = {"Mag_Expansion_AWM_5rnd"};
		magazineSwitchTime = 0.38;
		barrelArmor = 900;
		ejectType = 0;
		recoilModifier[] = {1,1,1};
		swayModifier[] = {2,2,1};
		simpleHiddenSelections[] = {"hide_barrel"};
		drySound[] = {"dz\sounds\weapons\firearms\mosin9130\mosin_dry",0.5,1,20};
		reloadMagazineSound[] = {"dz\sounds\weapons\firearms\CR527\cz527_reload_0",1,1,20};
		reloadSound[] = {"dz\sounds\weapons\firearms\CR527\cz527_cycling_0",1,1,20};
		reloadAction = "Reloadcz527";
		shotAction = "Reloadcz527Shot";
		hiddenSelections[] = {};
		modes[] = {"Single"};
		class Single: Mode_SemiAuto
		{
			soundSetShot[] = {"SVD_Shot_SoundSet","SVD_Tail_SoundSet","SVD_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"SVD_silencerHomeMade_SoundSet","SVD_silencerHomeMadeTail_SoundSet","SVD_silencerInteriorHomeMadeTail_SoundSet"}};
			reloadTime = 1;
			recoil = "recoil_cz527";
			recoilProne = "recoil_cz527_prone";
			dispersion = 0.001;
			magazineSlot = "magazine";
		};
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
					overrideParticle = "weapon_shot_cz527_01";
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
					positionOffset[] = {-0.05,0,0};
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating = 1;
				maxOverheatingValue = 4;
				overheatingDecayInterval = 3;
				class BarrelSmoke
				{
					overrideParticle = "smoking_barrel_small";
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
			discreteDistance[] = {100,200,300,400,500,600,700,800,900,1000};
			discreteDistanceInitIndex = 0;
			modelOptics = "-";
			distanceZoomMin = 100;
			distanceZoomMax = 1000;
		};
	};
	class Expansion_AWM: Expansion_AWM_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AWM";
		descriptionShort = "$STR_EXPANSION_AWM_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\AWM.p3d";
		attachments[] = {"weaponWrap","weaponOptics","ExpansionSniperOptics"};
		itemSize[] = {9,3};
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\data\dayz_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\data\AWM_Body.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\data\AWM_Body.rvmat"}},{0.7,{"DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\data\AWM_Body.rvmat"}},{0.5,{"DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\data\AWM_Body_Damaged.rvmat"}},{0.3,{"DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\data\AWM_Body_Damaged.rvmat"}},{0.0,{"DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\data\AWM_Body_Ruined.rvmat"}}};
				};
			};
		};
	};
	class Expansion_AWM_Black: Expansion_AWM
	{
		skinBase = "Expansion_AWM";
		skinName = "Black";
	};
	class Expansion_AWM_Green: Expansion_AWM
	{
		skinBase = "Expansion_AWM";
		skinName = "Green";
	};
};
class cfgAmmoTypes
{
	class AType_Bullet_Expansion_338
	{
		name = "Bullet_Expansion_338";
	};
};
class CfgAmmo
{
	class Bullet_Base;
	class Bullet_Expansion_338: Bullet_Base
	{
		scope = 2;
		lootCategory = "Crafted";
		cartridge = "FxCartridge_762";
		spawnPileType = "Ammo_Expansion_338";
		muzzleFlashParticle = "weapon_shot_mp5k_02_boris";
		hit = 12;
		indirectHit = 0;
		indirectHitRange = 0;
		visibleFire = 22;
		audibleFire = 22;
		visibleFireTime = 3;
		cost = 1.2;
		airLock = 1;
		initSpeed = 900;
		typicalSpeed = 900;
		airFriction = -0.00066;
		caliber = 1.5;
		damageBarrel = 6;
		damageBarrelDestroyed = 60;
		weight = 0.0162;
		impactBehaviour = 1;
		class DamageApplied
		{
			type = "Projectile";
			dispersion = 0;
			bleedThreshold = 1;
			class Health
			{
				damage = 147;
			};
			class Blood
			{
				damage = 100;
			};
			class Shock
			{
				damage = 125;
			};
		};
		class NoiseHit
		{
			strength = 40;
			type = "shot";
		};
	};
};
class cfgVehicles
{
	class Box_Base;
	class AmmoBox_Expansion_338_15rnd: Box_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMOBOX_338";
		descriptionShort = "$STR_EXPANSION_AMMOBOX_338_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\Ammobox_338.p3d";
		rotationFlags = 17;
		lootCategory = "Ammo";
		weight = 345;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"DZ\weapons\ammunition\data\45cal_box.rvmat"}},{0.7,{}},{0.5,{"DZ\weapons\ammunition\data\45cal_box_damage.rvmat"}},{0.3,{}},{0,{"DZ\weapons\ammunition\data\45cal_box_destruct.rvmat"}}};
				};
			};
		};
		class Resources
		{
			class Ammo_Expansion_338
			{
				value = 15;
				variable = "quantity";
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class interact
				{
					soundset = "ammoboxUnpack_SoundSet";
					id = 70;
				};
			};
		};
	};
};
class cfgMagazines
{
	class Magazine_Base;
	class Mag_Expansion_AWM_5rnd: Magazine_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AWM_5RND_MAGAZINE";
		descriptionShort = "$STR_EXPANSION_AWM_5RND_MAGAZINE_DESC";
		model = "DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\AWM_Mag.p3d";
		weight = 91;
		itemSize[] = {1,1};
		count = 5;
		ammo = "Bullet_Expansion_338";
		ammoItems[] = {"Ammo_Expansion_338","Ammo_AE338","Ammo_338"};
		tracersEvery = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\attachments\data\cz527_mag.rvmat"}},{0.7,{"DZ\weapons\attachments\data\cz527_mag.rvmat"}},{0.5,{"DZ\weapons\attachments\data\cz527_mag_damage.rvmat"}},{0.3,{"DZ\weapons\attachments\data\cz527_mag_damage.rvmat"}},{0.0,{"DZ\weapons\attachments\data\cz527_mag_destruct.rvmat"}}};
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
	class Ammunition_Base;
	class Ammo_Expansion_338: Ammunition_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_338";
		descriptionShort = "$STR_EXPANSION_AMMO_338_DESC";
		model = "DayZExpansion\Objects\Weapons\Firearms\Sniper\AWM\338LooseRounds.p3d";
		weight = 25;
		count = 15;
		ammo = "Bullet_Expansion_338";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\ammunition\data\45cal_loose.rvmat"}},{0.7,{}},{0.5,{"DZ\weapons\ammunition\data\45cal_loose_damage.rvmat"}},{0.3,{}},{0.0,{"DZ\weapons\ammunition\data\45cal_loose_destruct.rvmat"}}};
				};
			};
		};
	};
};
