class CfgPatches
{
	class DayZExpansion_Sounds_Airdrop
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
	class Expansion_C130J_Engine_SoundShader
	{
		range=2100;
	};
	class Expansion_C130J_Loop_SoundShader: Expansion_C130J_Engine_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\Sounds\Airdrop\Expansion_Airdrop_Airplane",
				1
			}
		};
		volume=1;
	};
	class Expansion_Airdrop_ZSpawn_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\Characters\movement\roll\unarmed_dirt_1",
				1
			},
			
			{
				"\DZ\sounds\Characters\movement\roll\unarmed_dirt_2",
				1
			},
			
			{
				"\DZ\sounds\Characters\movement\roll\unarmed_dirt_3",
				1
			}
		};
		volume=1;
	};
};
class CfgSoundSets
{
	class baseCharacter_SoundSet;
	class Expansion_C130J_Engine_SoundSet
	{
		sound3DProcessingType="Vehicle_Ext_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleEngineAttenuationCurve";
		volumeFactor=1;
		spatial=1;
		loop=0;
	};
	class Expansion_C130J_Loop_SoundSet: Expansion_C130J_Engine_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_C130J_Loop_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Airdrop_ZSpawn_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Airdrop_ZSpawn_SoundShader"
		};
	};
};
class CfgSounds
{
	class default
	{
		name="";
		titles[]={};
	};
	class Expansion_C130J_Loop_Sound: default
	{
		sound[]=
		{
			"\DayZExpansion\Sounds\Airdrop\Expansion_Airdrop_Airplane",
			1,
			1,
			1000
		};
	};
};
