class CfgPatches
{
	class DayZExpansion_Sounds_Weapons_Kar98
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Sounds_Effects"
		};
	};
};
class cfgSoundsets
{
	class Rifle_Shot_Base_SoundSet;
	class Expansion_Kar98_Shot_SoundSet: Rifle_Shot_Base_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Kar98_closeShot_SoundShader",
			"Expansion_Kar98_midShot_SoundShader",
			"Expansion_Kar98_distShot_SoundShader"
		};
	};
};
class CfgSoundShaders
{
	class Expansion_Kar98_closeShot_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_close_shot_01",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_close_shot_02",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_close_shot_03",
				1
			}
		};
		volume=1.9782794;
		range=50;
		rangeCurve="closeShotCurve";
	};
	class Expansion_Kar98_midShot_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_mid_shot_01",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_mid_shot_02",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_mid_shot_03",
				1
			}
		};
		volume=0.56234133;
		range=1500;
		rangeCurve[]=
		{
			{0,0.2},
			{50,1},
			{300,0},
			{1500,0}
		};
	};
	class Expansion_Kar98_distShot_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_dist_shot_01",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_dist_shot_02",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Kar98\Kar98_dist_shot_03",
				1
			}
		};
		volume=1;
		range=1500;
		rangeCurve[]=
		{
			{0,0},
			{50,0},
			{300,1},
			{1500,1}
		};
	};
};
