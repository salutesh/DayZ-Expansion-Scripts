#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Weapons_RPG7
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class base_closeShot_SoundShader;
	class Expansion_RPG_closeShot_SoundShader: base_closeShot_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Rocket.ogg",1}};
		volume = 1;
	};
};
class CfgSoundSets
{
	class Rifle_Shot_Base_SoundSet;
	class Expansion_RPG_Shot_SoundSet: Rifle_Shot_Base_SoundSet
	{
		soundShaders[] = {"Expansion_RPG_closeShot_SoundShader"};
	};
};
