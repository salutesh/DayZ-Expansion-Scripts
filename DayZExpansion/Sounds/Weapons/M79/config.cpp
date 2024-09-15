class CfgPatches
{
	class DayZExpansion_Sounds_Weapons_M79
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Characters",
			"DZ_Sounds_Effects"
		};
	};
};
class CfgSoundShaders
{
	class base_closeShot_SoundShader;
	class base_midShot_SoundShader;
	class base_distShot_SoundShader;
	class Expansion_M79_closeShot_SoundShader: base_closeShot_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\M79\m79_shot.ogg",
				1
			}
		};
		volume=1.1220185;
	};
	class Expansion_M79_midShot_SoundShader: base_midShot_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\M79\m79_shot_mid.ogg",
				1
			}
		};
		volume=1.1220185;
	};
	class Expansion_M79_distShot_SoundShader: base_distShot_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\M79\m79_shot_long.ogg",
				1
			}
		};
		volume=1.1220185;
	};
};
class CfgSoundSets
{
	class Rifle_Shot_Base_SoundSet;
	class Expansion_M79_Shot_SoundSet: Rifle_Shot_Base_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_M79_closeShot_SoundShader",
			"Expansion_M79_midShot_SoundShader",
			"Expansion_M79_distShot_SoundShader"
		};
	};
};
