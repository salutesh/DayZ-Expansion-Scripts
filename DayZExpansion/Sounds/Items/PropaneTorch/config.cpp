#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_PropaneTorch
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_PropaneTorch_Base_SoundShader
	{
		range = 20;
	};
	class Expansion_PropaneTorch_Weld_SoundShader: Expansion_PropaneTorch_Base_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Items\PropaneTorch\PropaneTorch_Weld",1}};
		volume = 1.0;
	};
	class Expansion_PropaneTorch_Burn_SoundShader: Expansion_PropaneTorch_Base_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Items\PropaneTorch\PropaneTorch_Burn",1}};
		volume = 0.8;
	};
};
class CfgSoundSets
{
	class Expansion_PropaneTorch_Base_SoundSet
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Expansion_PropaneTorch_Weld_SoundSet: Expansion_PropaneTorch_Base_SoundSet
	{
		soundShaders[] = {"Expansion_PropaneTorch_Weld_SoundShader"};
		frequencyRandomizer = 0;
		volumeRandomizer = 0;
	};
	class Expansion_PropaneTorch_Burn_SoundSet: Expansion_PropaneTorch_Base_SoundSet
	{
		soundShaders[] = {"Expansion_PropaneTorch_Burn_SoundShader"};
		frequencyRandomizer = 0;
		volumeRandomizer = 0;
	};
};
