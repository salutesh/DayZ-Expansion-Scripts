#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Explosions
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_Explosive_Base_SoundShader
	{
		range = 100;
	};
	class Expansion_Explosive_LargeBase_SoundShader
	{
		range = 1000;
	};
	class Expansion_Explosive_SmallBase_SoundShader
	{
		range = 10;
	};
	class Expansion_Rocket_Flare_SoundShader: Expansion_Explosive_Base_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Explosions\Expansion_Explosion_Flare",1}};
		volume = 1;
	};
	class Expansion_Explosive_C4_SoundShader: Expansion_Explosive_Base_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Explosions\Expansion_Explosion_C4",1}};
		volume = 1;
	};
	class Expansion_Explosive_Large_SoundShader: Expansion_Explosive_LargeBase_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Explosions\Expansion_Explosion_Global",1}};
		volume = 1;
	};
	class Expansion_Explosive_Small_SoundShader: Expansion_Explosive_SmallBase_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Explosions\Expansion_Explosion_Global",1}};
		volume = 1;
	};
	class Expansion_Explosive_Vehicle_SoundShader: Expansion_Explosive_LargeBase_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Explosions\Expansion_Explosion_Vehicle",1}};
		volume = 1;
	};
};
class CfgSoundSets
{
	class Expansion_Explosive_Base_SoundSet
	{
		sound3DProcessingType = "ExplosionHeavy3DProcessingType";
		distanceFilter = "explosionDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		spatial = 1;
		loop = 0;
	};
	class Expansion_Rocket_Flare_SoundSet: Expansion_Explosive_Base_SoundSet
	{
		soundShaders[] = {"Expansion_Rocket_Flare_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Explosive_C4_SoundSet: Expansion_Explosive_Base_SoundSet
	{
		soundShaders[] = {"Expansion_Explosive_C4_SoundShader"};
		volumeFactor = 0.5;
		spatial = 1;
	};
	class Expansion_Explosive_Large_SoundSet: Expansion_Explosive_Base_SoundSet
	{
		soundShaders[] = {"Expansion_Explosive_Large_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Explosive_Small_SoundSet: Expansion_Explosive_Base_SoundSet
	{
		soundShaders[] = {"Expansion_Explosive_Small_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Explosive_Vehicle_SoundSet: Expansion_Explosive_Base_SoundSet
	{
		soundShaders[] = {"Expansion_Explosive_Vehicle_SoundShader"};
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
	class Expansion_Explosive_C4_Sound: default
	{
		sound[] = {"\DayZExpansion\Sounds\Explosions\Expansion_Explosion_C4",1,1,100};
	};
};
