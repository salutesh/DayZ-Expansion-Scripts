#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Longhorn
	{
		units[] = {};
		weapons[] = {"Expansion_Longhorn"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Pistols"};
	};
};
class cfgWeapons
{
	class Pistol_Base;
	class Mode_Single;
	class OpticsInfoPistol;
	class Expansion_Longhorn_Base: Pistol_Base
	{
		scope = 0;
		weight = 600;
		absorbency = 0;
		repairableWithKits[] = {1};
		repairCosts[] = {30,25};
		PPDOFProperties[] = {1,0.5,10,250,4,10};
		WeaponLength = 0.45;
		barrelArmor = 3.125;
		chamberSize = 1;
		chamberedRound = "";
		magazines[] = {};
		chamberableFrom[] = {"Ammo_308Win","Ammo_308WinTracer"};
		DisplayMagazine = 0;
		ejectType = 2;
		recoilModifier[] = {1.5,1.5,1.5};
		hiddenSelections[] = {"zasleh"};
		modes[] = {"Single"};
		class Single: Mode_Single
		{
			soundSetShot[] = {"LongHorn_Shot_SoundSet","LongHorn_Tail_SoundSet","LongHorn_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"LongHorn_silencerHomeMade_SoundSet","LongHorn_silencerHomeMadeTail_SoundSet","LongHorn_silencerInteriorHomeMadeTail_SoundSet"}};
			reloadTime = 0.13;
			dispersion = 0.002;
			magazineSlot = "magazine";
		};
		class OpticsInfo: OpticsInfoPistol
		{
			memoryPointCamera = "eye";
			discreteDistance[] = {25};
			discreteDistanceInitIndex = 0;
			modelOptics = "-";
			distanceZoomMin = 100;
			distanceZoomMax = 100;
		};
	};
	class Expansion_Longhorn: Expansion_Longhorn_Base
	{
		scope = 2;
		displayName = "$STR_cfgWeapons_LongHorn0";
		descriptionShort = "$STR_cfgWeapons_LongHorn1";
		model = "DZ\weapons\pistols\longhorn\longhorn.p3d";
		attachments[] = {"weaponOpticsCrossbow","suppressorImpro"};
		itemSize[] = {4,2};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\pistols\LongHorn\data\LongHorn.rvmat"}},{0.7,{"DZ\weapons\pistols\LongHorn\data\LongHorn.rvmat"}},{0.5,{"DZ\weapons\pistols\LongHorn\data\LongHorn_damage.rvmat"}},{0.3,{"DZ\weapons\pistols\LongHorn\data\LongHorn_damage.rvmat"}},{0.0,{"DZ\weapons\pistols\LongHorn\data\LongHorn_destruct.rvmat"}}};
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
				class SmokeCloud
				{
					overrideParticle = "weapon_shot_winded_smoke_small";
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating = 1;
				maxOverheatingValue = 7;
				overheatingDecayInterval = 1;
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
	};
};
