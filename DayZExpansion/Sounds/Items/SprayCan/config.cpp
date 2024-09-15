class CfgPatches
{
	class DayZExpansion_Sounds_Items_Spraycan
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgSoundShaders
{
	class Expansion_Spraycan_SoundShader
	{
		volume=1;
		frequency=1;
		range=10;
		rangeCurve[]=
		{
			{0,1},
			{5,0.69999999},
			{10,0}
		};
	};
	class Expansion_Spraying_SoundShader: Expansion_Spraycan_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\Sounds\Items\Spraycan\Expansion_Spraying",
				1
			}
		};
	};
};
class CfgSoundSets
{
	class Expansion_Spraycan_SoundSet
	{
		sound3DProcessingType="character3DProcessingType";
		volumeCurve="characterAttenuationCurve";
		spatial=1;
		doppler=0;
		loop=0;
	};
	class Expansion_Spraying_SoundSet: Expansion_Spraycan_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Spraying_SoundShader"
		};
	};
};
