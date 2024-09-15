class CfgPatches
{
	class DayZExpansion_Weapons_Shotguns_DT11
	{
		units[]={};
		weapons[]=
		{
			"Expansion_DT11"
		};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Weapons_Firearms"
		};
	};
};
class Mode_Safe;
class Mode_Single;
class Mode_Double;
class Mode_FullAuto;
class Muzzle_Base;
class OpticsInfoRifle;
class cfgWeapons
{
	class Rifle_Base;
	class Expansion_DT11_Base: Rifle_Base
	{
		scope=0;
		weight=3700;
		model="\DayZExpansion\Objects\Weapons\Firearms\Shotguns\DT11\dt11.p3d";
		itemSize[]={9,3};
		absorbency=0;
		repairableWithKits[]={5,1};
		repairCosts[]={30,25};
		chamberSize=1;
		chamberedRound="";
		attachments[]=
		{
			"weaponWrap"
		};
		chamberableFrom[]=
		{
			"Ammo_12gaPellets",
			"Ammo_12gaSlug",
			"Ammo_12gaRubberSlug",
			"Ammo_12gaBeanbag"
		};
		magazines[]={};
		DisplayMagazine=0;
		PPDOFProperties[]={1,0.5,50,160,4,10};
		WeaponLength=1.273;
		muzzles[]=
		{
			"this",
			"SecondMuzzle"
		};
		ejectType=3;
		recoilModifier[]={1,1,1};
		swayModifier[]={2,2,1};
		drySound[]=
		{
			"dz\sounds\weapons\shotguns\Izh43\izh43_dry",
			0.5,
			1,
			20
		};
		reloadAction="ReloadB95";
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\B95\b95_reload2",
			0.80000001,
			1,
			20
		};
		reloadSound[]=
		{
			"",
			0.056234129,
			1,
			20
		};
		shotAction="";
		tooltip="_text = _this call fnc_generateTooltip;_text";
		hiddenSelections[]=
		{
			"camoGround"
		};
		simpleHiddenSelections[]=
		{
			"bullet",
			"bullet2",
			"hide_barrel"
		};
		modes[]=
		{
			"Single",
			"Double"
		};
		class Single: Mode_Single
		{
			soundSetShot[]=
			{
				"IZH43_Shot_SoundSet",
				"IZH43_Tail_SoundSet",
				"IZH43_InteriorTail_SoundSet"
			};
			reloadTime=0.1;
			dispersion=0.001;
			magazineSlot="magazine";
		};
		class Double: Mode_Double
		{
			soundSetShot[]=
			{
				"IZH43_Shot_SoundSet",
				"IZH43_Tail_SoundSet",
				"IZH43_InteriorTail_SoundSet"
			};
			reloadTime=0.1;
			dispersion=0.001;
			magazineSlot="magazine";
		};
		class SecondMuzzle: Muzzle_Base
		{
			chamberSize=1;
			muzzlePos="usti hlavne_2";
			muzzleEnd="konec hlavne_2";
			cartridgePos="nabojnicestart_2";
			cartridgeVel="nabojniceend_2";
			modes[]=
			{
				"Single",
				"Double"
			};
			magazines[]={};
			chamberableFrom[]=
			{
				"Ammo_12gaPellets",
				"Ammo_12gaSlug",
				"Ammo_12gaRubberSlug",
				"Ammo_12gaBeanbag"
			};
			barrelArmor=1000;
			initSpeedMultiplier=1;
			irDistance=0;
			irLaserPos="laser pos";
			irLaserEnd="laser dir";
			drySound[]=
			{
				"dz\sounds\weapons\shotguns\Izh43\izh43_dry",
				0.0056234128,
				1
			};
			soundBullet[]={};
			class Single: Mode_Single
			{
				soundSetShot[]=
				{
					"IZH43_Shot_SoundSet",
					"IZH43_Tail_SoundSet",
					"IZH43_InteriorTail_SoundSet"
				};
				reloadTime=0.1;
				dispersion=0.001;
				magazineSlot="magazine";
			};
			class Double: Mode_Double
			{
				soundSetShot[]=
				{
					"IZH43_Shot_SoundSet",
					"IZH43_Tail_SoundSet",
					"IZH43_InteriorTail_SoundSet"
				};
				reloadTime=0.1;
				dispersion=0.001;
				magazineSlot="magazine";
			};
			class OpticsInfo: OpticsInfoRifle
			{
				memoryPointCamera="eye";
				modelOptics="-";
				distanceZoomMin=50;
				distanceZoomMax=200;
				discreteDistance[]={50,100,200};
				discreteDistanceInitIndex=0;
			};
		};
		class OpticsInfo: OpticsInfoRifle
		{
			memoryPointCamera="eye";
			modelOptics="-";
			distanceZoomMin=50;
			distanceZoomMax=200;
			discreteDistance[]={50,100,200};
			discreteDistanceInitIndex=0;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=250;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\firearms\B95\Data\b95.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\firearms\B95\Data\b95.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\firearms\B95\Data\b95_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\firearms\B95\Data\b95_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\firearms\B95\Data\b95_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class Particles
		{
			class OnFire
			{
				class SmokeCloud
				{
					overrideParticle="weapon_shot_winded_smoke";
					muzzleIndex=0;
				};
				class SmokeCloud_2
				{
					overrideParticle="weapon_shot_winded_smoke";
					overridePoint="Usti hlavne_2";
					muzzleIndex=1;
				};
				class MuzzleFlash
				{
					overrideParticle="weapon_shot_winch70_01";
					ignoreIfSuppressed=1;
					illuminateWorld=1;
				};
				class MuzzleFlash_2
				{
					overrideParticle="weapon_shot_winch70_01";
					overridePoint="Usti hlavne_2";
					illuminateWorld=1;
					muzzleIndex=1;
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating=1;
				maxOverheatingValue=3;
				overheatingDecayInterval=4;
				class BarrelSmoke
				{
					overrideParticle="smoking_barrel_small";
				};
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
	class Expansion_DT11: Expansion_DT11_Base
	{
		scope=2;
		displayName="$STR_EXPANSION_DT11";
		descriptionShort="$STR_EXPANSION_DT11_DESC";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Objects\Weapons\Firearms\Shotguns\DT11\data\dayz_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Objects\Weapons\Firearms\Shotguns\DT11\data\DT11.rvmat"
		};
	};
};
