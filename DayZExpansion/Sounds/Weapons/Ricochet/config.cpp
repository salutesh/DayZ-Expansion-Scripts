class CfgPatches
{
	class DayZExpansion_Sounds_Weapons_Ricochet
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
class CfgSoundSets
{
	class Expansion_Ricochet_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Ricochet_SoundShader"
		};
		volumeFactor=1;
		frequencyFactor=1;
		spatial=0;
	};
};
class CfgSoundShaders
{
	class Expansion_Ricochet_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\Ricochet\ricochet_1",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Ricochet\ricochet_2",
				1
			},
			
			{
				"DayZExpansion\Sounds\Weapons\Ricochet\ricochet_3",
				1
			}
		};
		volume=0.69999999;
	};
};
