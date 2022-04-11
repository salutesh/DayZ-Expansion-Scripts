#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Firearms_Flaregun
	{
		units[] = {};
		weapons[] = {"ExpansionFlaregun"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Pistols_Flaregun"};
	};
};
class CfgWeapons
{
	class Flaregun_Base;
	class ExpansionFlaregunBase: Flaregun_Base
	{
		scope = 0;
		hiddenSelections[] = {"zasleh"};
		hiddenSelectionsTextures[] = {""};
	};
	class ExpansionFlaregun: ExpansionFlaregunBase
	{
		scope = 2;
		displayName = "$STR_cfgWeapons_Flaregun0";
		descriptionShort = "The Flaregun is now part of the official weaponary of vanilla dayz. This item still exist so nobody lose their gear. Please do not spawn this item anymore.";
		model = "\dz\weapons\pistols\flaregun\flaregun.p3d";
		attachments[] = {};
		itemSize[] = {2,2};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 150;
					healthLevels[] = {{1,{"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"}},{0.7,{"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"}},{0.5,{"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"}},{0.3,{"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"}},{0,{"DZ\weapons\pistols\flaregun\data\flaregun_destruct.rvmat"}}};
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
				maxOverheatingValue = 1;
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
