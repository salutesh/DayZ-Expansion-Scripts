class CfgPatches
{
	class DayZExpansion_Sounds_Missions
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
				"\DayZExpansion\Missions\Sounds\Expansion_Airdrop_Airplane",
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
	class Expansion_StarDestroyer_Engine_SoundShader
	{
		range=3200;
		samples[]=
		{
			
			{
				"\DayZExpansion\Missions\Sounds\stardestroyer_engine_loop",
				1
			}
		};
		volume=1;
	};
	class Expansion_StarDestroyer_Warp_SoundShader
	{
		range=3200;
		volume=1;
	};
	class Expansion_StarDestroyer_WarpIn_SoundShader: Expansion_StarDestroyer_Warp_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\Missions\Sounds\stardestroyer_warp_in",
				1
			}
		};
	};
	class Expansion_StarDestroyer_WarpOut_SoundShader: Expansion_StarDestroyer_Warp_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\Missions\Sounds\stardestroyer_warp_out",
				1
			}
		};
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
	class Expansion_StarDestroyer_Engine_SoundSet
	{
		sound3DProcessingType="Vehicle_Ext_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleEngineAttenuationCurve";
		volumeFactor=0.55000001;
		spatial=1;
		loop=0;
		soundShaders[]=
		{
			"Expansion_StarDestroyer_Engine_SoundShader"
		};
	};
	class Expansion_StarDestroyer_Warp_SoundSet
	{
		sound3DProcessingType="ThunderNear3DProcessingType";
		distanceFilter="none";
		volumeCurve="Constant1Curve";
		volumeFactor=1;
		spatial=1;
		doppler=0;
		loop=0;
	};
	class Expansion_StarDestroyer_WarpIn_SoundSet: Expansion_StarDestroyer_Warp_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_StarDestroyer_WarpIn_SoundShader"
		};
	};
	class Expansion_StarDestroyer_WarpOut_SoundSet: Expansion_StarDestroyer_Warp_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_StarDestroyer_WarpOut_SoundShader"
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
			"\DayZExpansion\Missions\Sounds\Expansion_Airdrop_Airplane",
			1,
			1,
			1000
		};
	};
};
