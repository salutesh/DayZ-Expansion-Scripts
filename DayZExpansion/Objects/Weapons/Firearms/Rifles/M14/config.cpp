class CfgPatches
{
	class DayZExpansion_Weapons_Rifles_M14
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Weapons_Firearms_M14"
		};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoRifle;
class cfgWeapons
{
	class M14_Base;
	class Expansion_M14_Base: M14_Base
	{
		scope=0;
		weight=3500;
		PPDOFProperties[]={};
		ironsightsExcludingOptics[]=
		{
			"M68Optic",
			"M4_T3NRDSOptic",
			"ReflexOptic",
			"ACOGOptic"
		};
		WeaponLength=0.88;
		barrelArmor=8.6999998;
		chamberedRound="";
		chamberSize=1;
		chamberableFrom[]=
		{
			"Ammo_308Win",
			"Ammo_308WinTracer"
		};
		magazines[]=
		{
			"Mag_Expansion_M14_10Rnd",
			"Mag_Expansion_M14_20Rnd"
		};
		magazineSwitchTime=0.25;
		ejectType=1;
		recoilModifier[]={1,1,1};
		simpleHiddenSelections[]=
		{
			"hide_barrel"
		};
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
		s_recoilControlMisalignmentX=0.30000001;
		s_recoilControlMisalignmentY=0.30000001;
		class FullAuto: Mode_FullAuto
		{
			soundSetShot[]=
			{
				"M14_Shot_SoundSet",
				"M14_Tail_SoundSet",
				"M14_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"M14_silencerHomeMade_SoundSet",
					"M14_silencerHomeMadeTail_SoundSet",
					"M14_silencerInteriorHomeMadeTail_SoundSet"
				}
			};
			envShootingDecrease=0.80000001;
			envShootingDecreaseExt[]={0.050000001,0.050000001};
			reloadTime=0.12;
			recoil="recoil_m14";
			recoilProne="recoil_m14";
			dispersion=0.00044999999;
			magazineSlot="magazine";
		};
	};
	class Expansion_M14: Expansion_M14_Base
	{
		scope=2;
		displayName="$STR_EXPANSION_M14";
		descriptionShort="$STR_EXPANSION_M14_DESC";
		model="\DayZExpansion\Objects\Weapons\Firearms\Rifles\M14\M14.p3d";
		attachments[]=
		{
			"weaponWrap",
			"weaponMuzzleM4",
			"Expansion_M1AScopeRail",
			"ExpansionSniperOptics",
			"weaponOptics"
		};
		itemSize[]={9,3};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=175;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DayZExpansion\Objects\Weapons\Firearms\Rifles\M14\data\M14.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DayZExpansion\Objects\Weapons\Firearms\Rifles\M14\data\M14.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DayZExpansion\Objects\Weapons\Firearms\Rifles\M14\data\M14_Damaged.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DayZExpansion\Objects\Weapons\Firearms\Rifles\M14\data\M14_Damaged.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DayZExpansion\Objects\Weapons\Firearms\Rifles\M14\data\M14_Ruined.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
