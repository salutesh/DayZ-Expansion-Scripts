class CfgPatches
{
	class DayZExpansion_Weapons_Pistols_Taser
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Weapons_Firearms"
		};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoPistol;
class cfgWeapons
{
	class FNX45_Base;
	class Expansion_Taser_Base: FNX45_Base
	{
		scope=0;
		weight=550;
		PPDOFProperties[]={};
		chamberSize=1;
		repairableWithKits[]={7};
		repairCosts[]={25};
		class NoiseShoot
		{
			strength=5;
			type="shot";
		};
		class OpticsInfo: OpticsInfoPistol
		{
			memoryPointCamera="eye";
			discreteDistance[]={25};
			discreteDistanceInitIndex=0;
			modelOptics="-";
			distanceZoomMin=100;
			distanceZoomMax=100;
		};
		magazines[]=
		{
			"Mag_Expansion_Taser"
		};
		chamberableFrom[]=
		{
			"Expansion_Ammo_Taser"
		};
		drySound[]=
		{
			"dz\sounds\weapons\nonlethal\shockpistol\Shockpistol_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\FNX45\FNX45_reload",
			0.80000001,
			1,
			20
		};
		modes[]=
		{
			"SemiAuto"
		};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"Shockpistol_Shot_SoundSet",
				"Shockpistol_Tail_SoundSet",
				"Shockpistol_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"Shockpistol_Shot_SoundSet",
					"Shockpistol_Tail_SoundSet",
					"Shockpistol_InteriorTail_SoundSet"
				}
			};
			reloadTime=0.086000003;
			recoil="Expansion_recoil_taser";
			recoilProne="Expansion_recoil_taser_prone";
			dispersion=0.0060000001;
			magazineSlot="magazine";
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\nonlethal\shockpistol\data\shockpistol_main.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\nonlethal\shockpistol\data\shockpistol_main.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\nonlethal\shockpistol\data\shockpistol_main_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\nonlethal\shockpistol\data\shockpistol_main_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\nonlethal\shockpistol\data\shockpistol_main_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class walkErc_L
				{
					soundSet="Weapon_Movement_Pistol_walkErc_L_SoundSet";
					id=1;
				};
				class walkErc_R
				{
					soundSet="Weapon_Movement_Pistol_walkErc_R_SoundSet";
					id=2;
				};
				class runErc_L
				{
					soundSet="Weapon_Movement_Pistol_runErc_L_SoundSet";
					id=3;
				};
				class runErc_R
				{
					soundSet="Weapon_Movement_Pistol_runErc_R_SoundSet";
					id=4;
				};
				class sprintErc_L
				{
					soundSet="Weapon_Movement_Pistol_sprintErc_SoundSet";
					id=5;
				};
				class sprintErc_R
				{
					soundSet="Weapon_Movement_Pistol_sprintErc_SoundSet";
					id=6;
				};
				class walkCro_L
				{
					soundSet="Weapon_Movement_Pistol_walkCro_SoundSet";
					id=21;
				};
				class walkCro_R
				{
					soundSet="Weapon_Movement_Pistol_walkCro_SoundSet";
					id=22;
				};
				class runCro_L
				{
					soundSet="Weapon_Movement_Pistol_runCro_SoundSet";
					id=23;
				};
				class runCro_R
				{
					soundSet="Weapon_Movement_Pistol_runCro_SoundSet";
					id=24;
				};
				class walkProne_L
				{
					soundSet="Weapon_Movement_Pistol_walkCro_SoundSet";
					id=31;
				};
				class walkProne_R
				{
					soundSet="Weapon_Movement_Pistol_walkCro_SoundSet";
					id=32;
				};
				class runProne_L
				{
					soundSet="Weapon_Movement_Pistol_runCro_SoundSet";
					id=33;
				};
				class runProne_R
				{
					soundSet="Weapon_Movement_Pistol_runCro_SoundSet";
					id=34;
				};
				class jumpErc_L
				{
					soundSet="Weapon_Movement_Pistol_runErc_L_SoundSet";
					id=35;
				};
				class jumpErc_R
				{
					soundSet="Weapon_Movement_Pistol_runErc_L_SoundSet";
					id=36;
				};
				class landFootErc_L
				{
					soundSet="Weapon_Movement_Pistol_landFootErc_SoundSet";
					id=37;
				};
				class landFootErc_R
				{
					soundSet="Weapon_Movement_Pistol_landFootErc_SoundSet";
					id=38;
				};
				class walkRasErc_L
				{
					soundSet="Weapon_Movement_Pistol_walkRasErc_SoundSet";
					id=51;
				};
				class walkRasErc_R
				{
					soundSet="Weapon_Movement_Pistol_walkRasErc_SoundSet";
					id=52;
				};
				class runRasErc_L
				{
					soundSet="Weapon_Movement_Pistol_runRasErc_SoundSet";
					id=53;
				};
				class runRasErc_R
				{
					soundSet="Weapon_Movement_Pistol_runRasErc_SoundSet";
					id=54;
				};
				class HandStep_L
				{
					soundSet="Weapon_Movement_Pistol_walkErc_L_SoundSet";
					id=61;
				};
				class HandStep_R
				{
					soundSet="Weapon_Movement_Pistol_walkErc_L_SoundSet";
					id=62;
				};
				class HandStep_Hard_L
				{
					soundSet="Weapon_Movement_Pistol_runErc_L_SoundSet";
					id=63;
				};
				class HandStep_Hard_R
				{
					soundSet="Weapon_Movement_Pistol_runErc_L_SoundSet";
					id=64;
				};
				class landFeetErc
				{
					soundSet="Weapon_Movement_Pistol_landFeetErc_SoundSet";
					id=100;
				};
				class Weapon_Movement_Pistol_Walk
				{
					soundSet="Weapon_Movement_Pistol_walkErc_L_SoundSet";
					id=101;
				};
				class Weapon_Movement_Pistol_Run
				{
					soundSet="Weapon_Movement_Pistol_runErc_L_SoundSet";
					id=102;
				};
				class Weapon_Movement_Pistol_Sprint
				{
					soundSet="Weapon_Movement_Pistol_sprintErc_SoundSet";
					id=103;
				};
				class Weapon_Movement_Pistol_Land
				{
					soundSet="Weapon_Movement_Pistol_landFeetErc_SoundSet";
					id=104;
				};
				class pickup
				{
					soundSet="pickUpPistol_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="pistol_drop_SoundSet";
					id=898;
				};
				class bodyfall_pistol
				{
					soundSet="pickUpPistol_SoundSet";
					id=13400;
				};
			};
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlashForward
				{
					ignoreIfSuppressed=1;
					illuminateWorld=1;
					overrideParticle="weapon_shot_fnx_01";
				};
				class SmokeCloud
				{
					overrideParticle="weapon_shot_winded_smoke_small";
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating=1;
				maxOverheatingValue=7;
				overheatingDecayInterval=1;
				class OpenChamberSmoke
				{
					onlyIfBoltIsOpen=1;
					overrideParticle="smoking_barrel_small";
					overridePoint="Nabojnicestart";
				};
			};
			class OnBulletCasingEject
			{
				class ChamberSmokeRaise
				{
					overrideParticle="weapon_shot_chamber_smoke_raise";
					overridePoint="Nabojnicestart";
				};
			};
		};
	};
	class Expansion_Taser_Yellow: Expansion_Taser_Base
	{
		scope=2;
		displayName="$STR_EXPANSION_TASER";
		descriptionShort="$STR_EXPANSION_TASER_DESC";
		model="dz\weapons\nonlethal\shockpistol\shockpistol.p3d";
		hiddenSelections[]=
		{
			"camo"
		};
		attachments[]={};
		itemSize[]={4,2};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\nonlethal\shockpistol\data\shockpistol_main_yel_co.paa"
		};
	};
	class Expansion_Taser_Black: Expansion_Taser_Yellow
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\nonlethal\shockpistol\data\shockpistol_main_bk_co.paa"
		};
	};
	class Expansion_Taser_Debug: Expansion_Taser_Yellow
	{
		displayName="Debug Taser";
		scope=2;
	};
};
class CfgMagazines
{
	class Mag_FNX45_15Rnd;
	class Mag_Expansion_Taser: Mag_FNX45_15Rnd
	{
		scope=2;
		displayName="$STR_EXPANSION_TASER_MAG";
		descriptionShort="$STR_EXPANSION_TASER_MAG_DESC";
		model="\DZ\weapons\attachments\magazine\shockpistol_cartridge.p3d";
		weight=143;
		itemSize[]={2,1};
		count=2;
		ammo="Bullet_Expansion_Taser";
		ammoItems[]=
		{
			"Expansion_Ammo_Taser"
		};
		tracersEvery=0;
		mass=10;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\pistols\fnx45\data\herstal45.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\fnx45\data\herstal45_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\fnx45\data\herstal45_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Ammo_22;
	class Expansion_Ammo_Taser: Ammo_22
	{
		scope=2;
		displayName="Magical taser bullets. Do not ingest.";
		descriptionShort="You should not be seeing this. Toss these, they're useless and you're not meant to have them.";
		model="\dz\weapons\ammunition\9mm_LooseRounds.p3d";
		weight=2;
		count=1;
		ammo="Bullet_Expansion_Taser";
	};
};
class cfgAmmoTypes
{
	class AType_Bullet_Expansion_Taser
	{
		name="Bullet_Expansion_Taser";
	};
};
class cfgAmmo
{
	class Bullet_22;
	class Bullet_Expansion_Taser: Bullet_22
	{
		scope=2;
		lootCategory="Crafted";
		cartridge="FxCartridge_22";
		spawnPileType="Expansion_Ammo_Taser";
		hit=4.5;
		indirectHit=0;
		indirectHitRange=0;
		visibleFire=2;
		audibleFire=3.5;
		visibleFireTime=3;
		cost=1;
		tracerScale=0.60000002;
		tracerStartTime=-1;
		tracerEndTime=1;
		deflecting=30;
		caliber=0.69999999;
		airFriction=-0.00076899998;
		typicalSpeed=280;
		initSpeed=280;
		timeToLive=0.045000002;
		weight=0.0024999999;
		soundFly[]={};
		supersonicCrackNear[]={};
		supersonicCrackFar[]={};
		class DamageApplied
		{
			type="Projectile";
			dispersion=0;
			bleedThreshold=0;
			class Health
			{
				damage=0;
			};
			class Blood
			{
				damage=0;
			};
			class Shock
			{
				damage=9001;
			};
		};
		class NoiseHit
		{
			strength=3;
			type="shot";
		};
	};
};
