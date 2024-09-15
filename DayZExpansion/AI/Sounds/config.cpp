class CfgPatches
{
	class DayZExpansion_Sounds_AI
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
class CfgSoundShaders
{
	class baseCharacter_SoundShader;
	class Expansion_AI_The_Sound_Of_Love_01_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\The_Sound_Of_Love_01",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_The_Sound_Of_Love_02_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\The_Sound_Of_Love_02",
				1
			}
		};
		volume=1;
	};
};
class CfgSoundSets
{
	class baseCharacter_SoundSet;
	class Expansion_AI_The_Sound_Of_Love_01_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_The_Sound_Of_Love_01_SoundShader"
		};
	};
	class Expansion_AI_The_Sound_Of_Love_02_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_The_Sound_Of_Love_02_SoundShader"
		};
	};
};
