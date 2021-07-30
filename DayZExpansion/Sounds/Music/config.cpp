#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Music
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_Music_1_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Music\apocalypse",1}};
		volume = 0.70794576;
	};
	class Expansion_Music_2_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Music\scott_buckley_the_climb",1}};
		volume = 0.70794576;
	};
};
class CfgSoundSets
{
	class Expansion_Music_1_SoundSet
	{
		soundShaders[] = {"Expansion_Music_1_SoundShader"};
		volumeFactor = 1;
		frequencyFactor = 1;
		spatial = 0;
	};
	class Expansion_Music_2_SoundSet
	{
		soundShaders[] = {"Expansion_Music_2_SoundShader"};
		volumeFactor = 1;
		frequencyFactor = 1;
		spatial = 0;
	};
};
