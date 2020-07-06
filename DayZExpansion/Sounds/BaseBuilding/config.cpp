#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_BaseBuilding
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgSoundShaders
{
	class Expansion_BaseBuilding_SoundShader
	{
		volume = 5;
		frequency = 1;
		range = 10;
		rangeCurve[] = {{0,1},{5,0.7},{10,0}};
	};
	class Expansion_CodeLock_Locks_SoundShader: Expansion_BaseBuilding_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\BaseBuilding\Expansion_Codelock_Lock_01",1},{"\DayZExpansion\Sounds\BaseBuilding\Expansion_Codelock_Lock_02",1},{"\DayZExpansion\Sounds\BaseBuilding\Expansion_Codelock_Lock_03",1}};
	};
	class Expansion_CodeLock_Lock1_SoundShader: Expansion_BaseBuilding_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\BaseBuilding\Expansion_Codelock_Lock_01",1}};
	};
	class Expansion_CodeLock_Lock2_SoundShader: Expansion_BaseBuilding_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\BaseBuilding\Expansion_Codelock_Lock_02",1}};
	};
	class Expansion_CodeLock_Lock3_SoundShader: Expansion_BaseBuilding_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\BaseBuilding\Expansion_Codelock_Lock_03",1}};
	};
	class Expansion_CodeLock_Unlock1_SoundShader: Expansion_BaseBuilding_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\BaseBuilding\Expansion_Codelock_Unlock_01",1}};
	};
};
class CfgSoundSets
{
	class Expansion_BaseBuilding_SoundSet
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Expansion_CodeLock_Locks_SoundSet: Expansion_BaseBuilding_SoundSet
	{
		soundShaders[] = {"Expansion_CodeLock_Locks_SoundShader"};
	};
	class Expansion_CodeLock_Lock1_SoundSet: Expansion_BaseBuilding_SoundSet
	{
		soundShaders[] = {"Expansion_CodeLock_Lock1_SoundShader"};
	};
	class Expansion_CodeLock_Lock2_SoundSet: Expansion_BaseBuilding_SoundSet
	{
		soundShaders[] = {"Expansion_CodeLock_Lock2_SoundShader"};
	};
	class Expansion_CodeLock_Lock3_SoundSet: Expansion_BaseBuilding_SoundSet
	{
		soundShaders[] = {"Expansion_CodeLock_Lock3_SoundShader"};
	};
	class Expansion_CodeLock_Unlock1_SoundSet: Expansion_BaseBuilding_SoundSet
	{
		soundShaders[] = {"Expansion_CodeLock_Unlock1_SoundShader"};
	};
};
