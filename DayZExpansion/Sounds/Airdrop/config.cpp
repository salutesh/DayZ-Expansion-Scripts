#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Airdrop
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_C130J_Engine_SoundShader
	{
		range = 1000;
	};
	class Expansion_C130J_Loop_SoundShader: Expansion_C130J_Engine_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Airdrop\Expansion_Airdrop_Airplane",1}};
		volume = 1;
	};
};
class CfgSoundSets
{
	class Expansion_C130J_Engine_SoundSet
	{
		sound3DProcessingType = "Vehicle_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		spatial = 1;
		loop = 0;
	};
	class Expansion_C130J_Loop_SoundSet: Expansion_C130J_Engine_SoundSet
	{
		soundShaders[] = {"Expansion_C130J_Loop_SoundShader"};
		volumeFactor = 1;
	};
};
class CfgSounds
{
	class default
	{
		name = "";
		titles[] = {};
	};
	class Expansion_C130J_Loop_Sound: default
	{
		sound[] = {"\DayZExpansion\Sounds\Airdrop\Expansion_Airdrop_Airplane",1,1,1000};
	};
};
