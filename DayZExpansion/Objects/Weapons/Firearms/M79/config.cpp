#define _ARMA_

class CfgPatches
{
	class DayZExpansion_M79
	{
		units[] = {"Expansion_M203_HE_Explosion","Expansion_M203Round_Smoke_White","Expansion_M203Round_Smoke_Teargas","Expansion_M203Round_Smoke_Red","Expansion_M203Round_Smoke_Green","Expansion_M203Round_Smoke_Yellow","Expansion_M203Round_Smoke_Purple"};
		weapons[] = {"Expansion_M79"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Firearms"};
		magazines[] = {"Ammo_Expansion_M203_Smoke_White","Ammo_Expansion_M203_Smoke_Red","Ammo_Expansion_M203_Smoke_Green","Ammo_Expansion_M203_Smoke_Yellow","Ammo_Expansion_M203_Smoke_Purple","Ammo_Expansion_M203_Smoke_Teargas","Ammo_Expansion_M203_Sticky_Smoke_White","Ammo_Expansion_M203_Sticky_Smoke_Red","Ammo_Expansion_M203_Sticky_Smoke_Green","Ammo_Expansion_M203_Sticky_Smoke_Yellow","Ammo_Expansion_M203_Sticky_Smoke_Purple","Ammo_Expansion_M203_Sticky_Smoke_Teargas","Ammo_Expansion_M203_HE"};
		ammo[] = {"DummyShockDamage","Bullet_Expansion_M203_Smoke_Base","Bullet_Expansion_M203_Smoke_White","Bullet_Expansion_M203_Smoke_Red","Bullet_Expansion_M203_Smoke_Green","Bullet_Expansion_M203_Smoke_Yellow","Bullet_Expansion_M203_Smoke_Purple","Bullet_Expansion_M203_Smoke_Teargas","Bullet_Expansion_M203_Sticky_Smoke_White","Bullet_Expansion_M203_Sticky_Smoke_Red","Bullet_Expansion_M203_Sticky_Smoke_Green","Bullet_Expansion_M203_Sticky_Smoke_Yellow","Bullet_Expansion_M203_Sticky_Smoke_Purple","Bullet_Expansion_M203_Sticky_Smoke_Teargas","Bullet_Expansion_M203_HE"};
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
	class Expansion_M79_Base: Rifle_Base
	{
		scope = 0;
		weight = 3200;
		absorbency = 0;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30,25};
		PPDOFProperties[] = {1,0.5,10,180,4,10};
		ironsightsExcludingOptics[] = {};
		DisplayMagazine = 0;
		WeaponLength = 0.78105503;
		chamberSize = 1;
		chamberedRound = "";
		chamberableFrom[] = {"Ammo_Expansion_M203_Smoke_White","Ammo_Expansion_M203_Smoke_Red","Ammo_Expansion_M203_Smoke_Green","Ammo_Expansion_M203_Smoke_Yellow","Ammo_Expansion_M203_Smoke_Purple","Ammo_Expansion_M203_Smoke_Teargas","Ammo_Expansion_M203_Sticky_Smoke_White","Ammo_Expansion_M203_Sticky_Smoke_Red","Ammo_Expansion_M203_Sticky_Smoke_Green","Ammo_Expansion_M203_Sticky_Smoke_Yellow","Ammo_Expansion_M203_Sticky_Smoke_Purple","Ammo_Expansion_M203_Sticky_Smoke_Teargas","Ammo_Expansion_M203_HE"};
		magazines[] = {};
		ejectType = 3;
		recoilModifier[] = {2.5,2.5,2.5};
		swayModifier[] = {1,1,1};
		reloadAction = "ReloadIZH18";
		shotAction = "";
		modes[] = {"Single"};
		class Single: Mode_SemiAuto
		{
			soundSetShot[] = {"Expansion_M79_Shot_SoundSet"};
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
		class OpticsInfo: OpticsInfoRifle
		{
			memoryPointCamera = "eye";
			discreteDistance[] = {50,100,200,300,400,500,600,700,800,900,1000};
			discreteDistanceInitIndex = 0;
			modelOptics = "-";
			distanceZoomMin = 100;
			distanceZoomMax = 100;
		};
	};
	class Expansion_M79: Expansion_M79_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_M79";
		descriptionShort = "$STR_EXPANSION_M79_DESC";
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79.p3d";
		attachments[] = {};
		itemSize[] = {9,3};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					overrideParticle = "weapon_shot_izh18_01";
					ignoreIfSuppressed = 1;
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
					healthLevels[] = {{1.0,{"DayZExpansion\Objects\Weapons\Firearms\M79\m79\data\M79.rvmat"}},{0.7,{"DayZExpansion\Objects\Weapons\Firearms\M79\m79\data\M79.rvmat"}},{0.5,{"DayZExpansion\Objects\Weapons\Firearms\M79\m79\data\M79_damage.rvmat"}},{0.3,{"DayZExpansion\Objects\Weapons\Firearms\M79\m79\data\M79_damage.rvmat"}},{0.0,{"DayZExpansion\Objects\Weapons\Firearms\M79\m79\data\M79_destroy.rvmat"}}};
				};
			};
		};
	};
};
class CfgMagazines
{
	class Ammunition_Base;
	class Ammo_Expansion_M203_Smoke_Base: Ammunition_Base
	{
		scope = 0;
		descriptionShort = "$STR_EXPANSION_AMMO_M203_DESC";
		itemSize[] = {1,2};
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m203_round.p3d";
		iconCartridge = 1;
		weight = 12;
		count = 4;
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\M203.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\ammunition\data\45cal_loose.rvmat"}},{0.7,{"DZ\weapons\ammunition\data\45cal_loose.rvmat"}},{0.5,{"DZ\weapons\ammunition\data\45cal_loose_damage.rvmat"}},{0.3,{"DZ\weapons\ammunition\data\45cal_loose_damage.rvmat"}},{0.0,{"DZ\weapons\ammunition\data\45cal_loose_destruct.rvmat"}}};
				};
			};
		};
	};
	class Ammo_Expansion_M203_Smoke_White: Ammo_Expansion_M203_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_WHITE";
		ammo = "Bullet_Expansion_M203_Smoke_White";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_white_co.paa"};
	};
	class Ammo_Expansion_M203_Smoke_Red: Ammo_Expansion_M203_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_RED";
		ammo = "Bullet_Expansion_M203_Smoke_Red";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_red_co.paa"};
	};
	class Ammo_Expansion_M203_Smoke_Green: Ammo_Expansion_M203_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_GREEN";
		ammo = "Bullet_Expansion_M203_Smoke_Green";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_green_co.paa"};
	};
	class Ammo_Expansion_M203_Smoke_Yellow: Ammo_Expansion_M203_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_YELLOW";
		ammo = "Bullet_Expansion_M203_Smoke_Yellow";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_yellow_co.paa"};
	};
	class Ammo_Expansion_M203_Smoke_Purple: Ammo_Expansion_M203_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_PURPLE";
		ammo = "Bullet_Expansion_M203_Smoke_Purple";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_purple_co.paa"};
	};
	class Ammo_Expansion_M203_Smoke_Teargas: Ammo_Expansion_M203_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_TEARGAS";
		ammo = "Bullet_Expansion_M203_Smoke_Teargas";
	};
	class Ammo_Expansion_M203_Sticky_Smoke_Base: Ammo_Expansion_M203_Smoke_Base
	{
		scope = 0;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_WHITE";
		descriptionShort = "$STR_EXPANSION_AMMO_M203_STICKY_SMOKE_DESC";
		ammo = "Bullet_Expansion_M203_Sticky_Smoke_White";
	};
	class Ammo_Expansion_M203_Sticky_Smoke_White: Ammo_Expansion_M203_Sticky_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_WHITE";
		ammo = "Bullet_Expansion_M203_Sticky_Smoke_White";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_white_co.paa"};
	};
	class Ammo_Expansion_M203_Sticky_Smoke_Red: Ammo_Expansion_M203_Sticky_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_RED";
		ammo = "Bullet_Expansion_M203_Sticky_Smoke_Red";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_red_co.paa"};
	};
	class Ammo_Expansion_M203_Sticky_Smoke_Green: Ammo_Expansion_M203_Sticky_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_GREEN";
		ammo = "Bullet_Expansion_M203_Sticky_Smoke_Green";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_green_co.paa"};
	};
	class Ammo_Expansion_M203_Sticky_Smoke_Yellow: Ammo_Expansion_M203_Sticky_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_YELLOW";
		ammo = "Bullet_Expansion_M203_Sticky_Smoke_Yellow";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_yellow_co.paa"};
	};
	class Ammo_Expansion_M203_Sticky_Smoke_Purple: Ammo_Expansion_M203_Sticky_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_PURPLE";
		ammo = "Bullet_Expansion_M203_Sticky_Smoke_Purple";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_purple_co.paa"};
	};
	class Ammo_Expansion_M203_Sticky_Smoke_Teargas: Ammo_Expansion_M203_Sticky_Smoke_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_SMOKE_TEARGAS";
		descriptionShort = "$STR_EXPANSION_AMMO_M203_SMOKE_TEARGAS_DESC";
		ammo = "Bullet_Expansion_M203_Sticky_Smoke_Teargas";
	};
	class Ammo_Expansion_M203_HE: Ammunition_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AMMO_M203_HE";
		descriptionShort = "$STR_EXPANSION_AMMO_M203_HE_DESC";
		model = "\DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m203_round.p3d";
		itemSize[] = {1,2};
		iconCartridge = 1;
		weight = 12;
		count = 1;
		ammo = "Bullet_Expansion_M203_HE";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\ammunition\data\45cal_loose.rvmat"}},{0.7,{"DZ\weapons\ammunition\data\45cal_loose.rvmat"}},{0.5,{"DZ\weapons\ammunition\data\45cal_loose_damage.rvmat"}},{0.3,{"DZ\weapons\ammunition\data\45cal_loose_damage.rvmat"}},{0.0,{"DZ\weapons\ammunition\data\45cal_loose_destruct.rvmat"}}};
				};
			};
		};
	};
};
class cfgAmmoTypes
{
	class AType_Bullet_Expansion_M203_HE
	{
		name = "Bullet_Expansion_M203_HE";
	};
	class AType_Bullet_Expansion_M203_Smoke_White
	{
		name = "Bullet_Expansion_M203_Smoke_White";
	};
	class AType_Bullet_Expansion_M203_Smoke_Red
	{
		name = "Bullet_Expansion_M203_Smoke_Red";
	};
	class AType_Bullet_Expansion_M203_Smoke_Green
	{
		name = "Bullet_Expansion_M203_Smoke_Green";
	};
	class AType_Bullet_Expansion_M203_Smoke_Yellow
	{
		name = "Bullet_Expansion_M203_Smoke_Yellow";
	};
	class AType_Bullet_Expansion_M203_Smoke_Purple
	{
		name = "Bullet_Expansion_M203_Smoke_Purple";
	};
	class AType_Bullet_Expansion_M203_Smoke_Teargas
	{
		name = "Bullet_Expansion_M203_Smoke_Teargas";
	};
	class AType_Bullet_Expansion_M203_Sticky_Smoke_White
	{
		name = "Bullet_Expansion_M203_Sticky_Smoke_White";
	};
	class AType_Bullet_Expansion_M203_Sticky_Smoke_Red
	{
		name = "Bullet_Expansion_M203_Sticky_Smoke_Red";
	};
	class AType_Bullet_Expansion_M203_Sticky_Smoke_Green
	{
		name = "Bullet_Expansion_M203_Sticky_Smoke_Green";
	};
	class AType_Bullet_Expansion_M203_Sticky_Smoke_Yellow
	{
		name = "Bullet_Expansion_M203_Sticky_Smoke_Yellow";
	};
	class AType_Bullet_Expansion_M203_Sticky_Smoke_Purple
	{
		name = "Bullet_Expansion_M203_Sticky_Smoke_Purple";
	};
	class AType_Bullet_Expansion_M203_Sticky_Smoke_Teargas
	{
		name = "Bullet_Expansion_M203_Sticky_Smoke_Teargas";
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
	class Bullet_Expansion_M203_Smoke_Base: Bullet_Base
	{
		scope = 0;
		casing = "FxCartridge_Expansion_M203";
		round = "FxRound_Expansion_M203";
		spawnPileType = "Ammo_Expansion_M203_Smoke_White";
		hit = 0;
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m79_projectile.p3d";
		indirectHit = 0;
		indirectHitRange = 0;
		tracerScale = 1;
		caliber = 0.01;
		deflecting = 5;
		impactBehaviour = 1;
		initSpeed = 70;
		typicalSpeed = 70;
		timeToLive = 100;
		airFriction = -0.001;
		supersonicCrackNear[] = {};
		supersonicCrackFar[] = {};
		weight = 0.0149;
		class DamageApplied
		{
			type = "Projectile";
			dispersion = 0;
			bleedThreshold = 1;
			defaultDamageOverride[] = {{0.5,1}};
			class Health
			{
				damage = 10;
			};
			class Blood
			{
				damage = 0;
			};
			class Shock
			{
				damage = 25;
			};
		};
		class NoiseHit
		{
			strength = 15;
			type = "shot";
		};
	};
	class Bullet_Expansion_M203_Smoke_White: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Smoke_White";
	};
	class Bullet_Expansion_M203_Smoke_Red: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Smoke_Red";
	};
	class Bullet_Expansion_M203_Smoke_Green: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Smoke_Green";
	};
	class Bullet_Expansion_M203_Smoke_Yellow: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Smoke_Yellow";
	};
	class Bullet_Expansion_M203_Smoke_Purple: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Smoke_Purple";
	};
	class Bullet_Expansion_M203_Smoke_Teargas: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Smoke_Teargas";
	};
	class Bullet_Expansion_M203_Sticky_Smoke_White: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Sticky_Smoke_White";
	};
	class Bullet_Expansion_M203_Sticky_Smoke_Red: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Sticky_Smoke_Red";
	};
	class Bullet_Expansion_M203_Sticky_Smoke_Green: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Sticky_Smoke_Green";
	};
	class Bullet_Expansion_M203_Sticky_Smoke_Yellow: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Sticky_Smoke_Yellow";
	};
	class Bullet_Expansion_M203_Sticky_Smoke_Purple: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Sticky_Smoke_Purple";
	};
	class Bullet_Expansion_M203_Sticky_Smoke_Teargas: Bullet_Expansion_M203_Smoke_Base
	{
		spawnPileType = "Ammo_Expansion_M203_Sticky_Smoke_Teargas";
	};
	class Bullet_Expansion_M203_HE: Bullet_Base
	{
		scope = 2;
		casing = "FxCartridge_Expansion_M203";
		round = "FxRound_Expansion_M203";
		spawnPileType = "Ammo_Expansion_M203_HE";
		hit = 0;
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m79_projectile.p3d";
		indirectHit = 0;
		indirectHitRange = 0;
		tracerScale = 1;
		caliber = 0.01;
		deflecting = 5;
		impactBehaviour = 1;
		initSpeed = 70;
		typicalSpeed = 70;
		timeToLive = 100;
		airFriction = -0.001;
		supersonicCrackNear[] = {};
		supersonicCrackFar[] = {};
		weight = 0.0149;
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
class cfgVehicles
{
	class FxCartridge;
	class FxRound;
	class HouseNoDestruct;
	class Grenade_Base;
	class Expansion_M203_HE_Explosion: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\data\lightpoint.p3d";
		ammoType = "RGD5Grenade_Ammo";
	};
	class FxCartridge_Expansion_M203: FxCartridge
	{
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m203_casing.p3d";
	};
	class FxRound_Expansion_M203: FxRound
	{
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m203_fxround.p3d";
	};
	class Expansion_M203Round_Smoke_Colorbase: Grenade_Base
	{
		displayName = "";
		descriptionShort = "";
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m79_projectile.p3d";
		rotationFlags = 17;
		weight = 500;
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\M203.rvmat"};
		class EnergyManager
		{
			energyAtSpawn = 90;
			energyUsagePerSecond = 1;
			wetnessExposure = 0.1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Unpin
				{
					soundSet = "SmokegGrenades_M18_oneshot_ignite_SoundSet";
					id = 201;
				};
			};
		};
	};
	class Expansion_M203Round_Smoke_White: Expansion_M203Round_Smoke_Colorbase
	{
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_white_co.paa"};
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Teargas: Expansion_M203Round_Smoke_Colorbase
	{
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Red: Expansion_M203Round_Smoke_Colorbase
	{
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_red_co.paa"};
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Green: Expansion_M203Round_Smoke_Colorbase
	{
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_green_co.paa"};
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Yellow: Expansion_M203Round_Smoke_Colorbase
	{
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_yellow_co.paa"};
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Purple: Expansion_M203Round_Smoke_Colorbase
	{
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\data\dayz_purple_co.paa"};
		scope = 2;
	};
};
