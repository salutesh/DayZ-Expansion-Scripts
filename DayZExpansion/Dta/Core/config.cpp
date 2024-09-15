class CfgPatches
{
	class DayZExpansion_Dta_Core
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Characters",
			"DZ_Gear_Camping",
			"DZ_Pistols_FNX45",
			"DZ_Surfaces",
			"DZ_Weapons_Ammunition",
			"DZ_Weapons_Projectiles"
		};
	};
};
class CfgSurfaces
{
	class DZ_SurfacesExt;
	class expansion_test_surface: DZ_SurfacesExt
	{
		files="lino_int*";
		friction=5;
		restitution=1;
		soundEnviron="road";
		soundHit="hard_ground";
		character="Empty";
		audibility=0.40000001;
		footDamage=0.023;
		impact="Hit_Rubber";
		isDigable=0;
		isFertile=0;
	};
};
class CfgVehicleClasses
{
	class Expansion_Gear
	{
		displayName="Expansion Gear";
	};
	class Expansion_Launchers
	{
		displayName="Expansion Launchers";
	};
	class Expansion_Flags
	{
		displayName="Expansion Flags";
	};
	class Expansion_Items
	{
		displayName="Expansion Items";
	};
	class Expansion_Explosives
	{
		displayName="Expansion Explosives";
	};
	class Expansion_Barricading
	{
		displayName="Expansion Barricading";
	};
	class Expansion_Airdrop
	{
		displayName="Expansion Airdrop";
	};
	class Expansion_Trader
	{
		displayName="Expansion Traders";
	};
	class Expansion_Currency
	{
		displayName="Expansion Currency";
	};
	class Expansion_Npc
	{
		displayName="Expansion Npc";
	};
	class Expansion_Static
	{
		displayName="Expansion Static";
	};
	class Expansion_Construction
	{
		displayName="Expansion Construction";
	};
	class Expansion_Sign
	{
		displayName="Expansion Sign";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Container_Base;
	class HouseNoDestruct;
	class SurvivorM_Mirek;
	class SurvivorM_Denis;
	class SurvivorM_Boris;
	class SurvivorM_Cyril;
	class SurvivorM_Elias;
	class SurvivorM_Francis;
	class SurvivorM_Guo;
	class SurvivorM_Hassan;
	class SurvivorM_Indar;
	class SurvivorM_Jose;
	class SurvivorM_Kaito;
	class SurvivorM_Lewis;
	class SurvivorM_Manua;
	class SurvivorM_Niki;
	class SurvivorM_Oliver;
	class SurvivorM_Peter;
	class SurvivorM_Quinn;
	class SurvivorM_Rolf;
	class SurvivorM_Seth;
	class SurvivorM_Taiki;
	class SurvivorF_Linda;
	class SurvivorF_Maria;
	class SurvivorF_Frida;
	class SurvivorF_Gabi;
	class SurvivorF_Helga;
	class SurvivorF_Irena;
	class SurvivorF_Judy;
	class SurvivorF_Keiko;
	class SurvivorF_Eva;
	class SurvivorF_Naomi;
	class SurvivorF_Baty;
	class SeaChest;
	class ExpansionAdminChest: SeaChest
	{
		displayName="Admin Chest";
		attachments[]=
		{
			"Shoulder",
			"Melee",
			"Vest",
			"Body",
			"Hips",
			"Legs",
			"Back",
			"Headgear",
			"Mask",
			"Eyewear",
			"Gloves",
			"Feet",
			"Armband",
			"LEHS",
			"Dogtag"
		};
		class Cargo
		{
			itemsCargoSize[]={10,100};
		};
		class DamageSystem
		{
			class GlobalArmor
			{
				class FragGrenade
				{
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
						damage=0;
					};
				};
			};
		};
	};
	class ExpansionPickup: Inventory_Base
	{
		scope=2;
	};
	class ExpansionPickupCone: ExpansionPickup
	{
		scope=2;
		model="\DayZExpansion\Dta\Core\cone.p3d";
	};
	class ExplosionPoint: HouseNoDestruct
	{
		scope=2;
		model="\DZ\data\lightpoint.p3d";
	};
	class ExplosionSmall: ExplosionPoint
	{
		scope=2;
	};
	class ExpansionNPCMirek: SurvivorM_Mirek
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCDenis: SurvivorM_Denis
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCBoris: SurvivorM_Boris
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCCyril: SurvivorM_Cyril
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCElias: SurvivorM_Elias
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCFrancis: SurvivorM_Francis
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCGuo: SurvivorM_Guo
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCHassan: SurvivorM_Hassan
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCIndar: SurvivorM_Indar
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCJose: SurvivorM_Jose
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCKaito: SurvivorM_Kaito
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCLewis: SurvivorM_Lewis
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCManua: SurvivorM_Manua
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCNiki: SurvivorM_Niki
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCOliver: SurvivorM_Oliver
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCPeter: SurvivorM_Peter
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCQuinn: SurvivorM_Quinn
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCRolf: SurvivorM_Rolf
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCSeth: SurvivorM_Seth
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCTaiki: SurvivorM_Taiki
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCLinda: SurvivorF_Linda
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCMaria: SurvivorF_Maria
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCFrida: SurvivorF_Frida
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCGabi: SurvivorF_Gabi
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCHelga: SurvivorF_Helga
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCIrena: SurvivorF_Irena
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCJudy: SurvivorF_Judy
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCKeiko: SurvivorF_Keiko
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCEva: SurvivorF_Eva
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCNaomi: SurvivorF_Naomi
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionNPCBaty: SurvivorF_Baty
	{
		scope=2;
		vehicleClass="Expansion_Npc";
	};
	class ExpansionMoneyBase: Inventory_Base
	{
		scope=0;
		vehicleClass="Expansion_Currency";
	};
	class ExpansionATMBase: HouseNoDestruct
	{
		scope=0;
		vehicleClass="Expansion_Static";
	};
	class ExpansionATMLocker: ExpansionATMBase
	{
		scope=1;
		displayName="$STR_EXPANSION_ATM_LOCKER";
		model="\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
	};
};
class Mode_SemiAuto;
class OpticsInfoPistol;
class CfgWeapons
{
	class FNX45_Base;
	class Expansion_LobotomyDartGun_Base: FNX45_Base
	{
		scope=0;
		weight=950;
		PPDOFProperties[]={};
		chamberSize=1;
		repairableWithKits[]={7};
		repairCosts[]={25};
		class NoiseShoot
		{
			strength=5;
			type="shot";
		};
		attachments[]=
		{
			"weaponOptics"
		};
		magazines[]=
		{
			"Mag_Expansion_LobotomyDart"
		};
		chamberableFrom[]=
		{
			"Ammo_Expansion_LobotomyDart"
		};
		drySound[]=
		{
			"dz\sounds\weapons\nonlethal\dartgun\dartgun_dry",
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
				"Dartgun_Shot_SoundSet",
				"Dartgun_Tail_SoundSet",
				"Dartgun_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"Dartgun_Shot_SoundSet",
					"Dartgun_Tail_SoundSet",
					"Dartgun_InteriorTail_SoundSet"
				}
			};
			reloadTime=0.086000003;
			recoil="Expansion_recoil_dartgun";
			recoilProne="Expansion_recoil_dartgun_prone";
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
								"DZ\weapons\nonlethal\dartgun\data\dartgun.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\nonlethal\dartgun\data\dartgun.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\nonlethal\dartgun\data\dartgun_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\nonlethal\dartgun\data\dartgun_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\nonlethal\dartgun\data\dartgun_destruct.rvmat"
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
	class Expansion_LobotomyDartGun: Expansion_LobotomyDartGun_Base
	{
		scope=2;
		displayName="Lobotomy Gun";
		descriptionShort="Darts fired by this gun lobotomize animals and Infected. DEBUG ITEM, DO NOT USE ON LIVE SERVER";
		model="\dz\weapons\nonlethal\dartgun\dartgun.p3d";
		itemSize[]={6,3};
	};
	class Expansion_LobotomyDartGun_Debug: Expansion_LobotomyDartGun
	{
		displayName="Debug Lobotomy Gun";
	};
};
class CfgMagazines
{
	class Mag_FNX45_15Rnd;
	class Ammo_22;
	class Mag_Expansion_LobotomyDart: Mag_FNX45_15Rnd
	{
		scope=2;
		displayName="Lobotomy Dart Magazine";
		descriptionShort="DEBUG ITEM, DO NOT USE ON LIVE SERVER";
		model="\dz\weapons\attachments\magazine\magazine_dartgun_CO2.p3d";
		weight=143;
		itemSize[]={2,1};
		count=2;
		ammo="Bullet_Expansion_LobotomyDart";
		ammoItems[]=
		{
			"Ammo_Expansion_LobotomyDart"
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
								"DZ\weapons\attachments\magazine\data\magazine_dartgun_CO2.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\attachments\magazine\data\magazine_dartgun_CO2.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\attachments\magazine\data\magazine_dartgun_CO2_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\attachments\magazine\data\magazine_dartgun_CO2_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\attachments\magazine\data\magazine_dartgun_CO2_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Ammo_Expansion_LobotomyDart: Ammo_22
	{
		scope=2;
		displayName="Lobotomy Dart";
		descriptionShort="DEBUG ITEM, DO NOT USE ON LIVE SERVER";
		model="\dz\weapons\projectiles\dart_syringe.p3d";
		weight=2;
		count=1;
		ammo="Bullet_Expansion_LobotomyDart";
	};
};
class CfgAmmoTypes
{
	class AType_Bullet_Expansion_LobotomyDart
	{
		name="Bullet_Expansion_LobotomyDart";
	};
};
class CfgAmmo
{
	class Bullet_22;
	class Bullet_Expansion_LobotomyDart: Bullet_22
	{
		scope=2;
		lootCategory="Crafted";
		cartridge="FxCartridge_22";
		spawnPileType="Ammo_Expansion_LobotomyDart";
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
				damage=100;
			};
		};
		class NoiseHit
		{
			strength=3;
			type="shot";
		};
	};
};
