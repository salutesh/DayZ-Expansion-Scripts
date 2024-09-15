class CfgPatches
{
	class DayZExpansion_Teleporter_Sounds
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Sounds_Effects"
		};
	};
};
class CfgSounds
{
	class Expansion_TeleporterIdle_Sound
	{
		sound[]=
		{
			"DayZExpansion\Teleporter\Sounds\data\teleporter_idle",
			1,
			1,
			30
		};
	};
};
class CfgSoundShaders
{
	class Expansion_Teleporter_SoundShader
	{
		volume=1;
		frequency=1;
		limitation=0;
		radius=150;
		range=150;
	};
	class Expansion_TeleporterIdle_SoundShader: Expansion_Teleporter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Teleporter\Sounds\data\teleporter_idle",
				1
			}
		};
		volume=0.5;
		radius=80;
		range=150;
	};
	class Expansion_TeleporterActive_SoundShader: Expansion_Teleporter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Teleporter\Sounds\data\teleporter_active",
				1
			}
		};
		volume=0.30000001;
		radius=80;
		range=150;
	};
};
class CfgSoundSets
{
	class Expansion_Teleporter_Soundset
	{
		sound3DProcessingType="character3DProcessingType";
		volumeCurve="characterAttenuationCurve";
		spatial=1;
		doppler=0;
		loop=0;
		distanceFilter="defaultDistanceFilter";
	};
	class Expansion_Teleporter_Idle_Soundset: Expansion_Teleporter_Soundset
	{
		soundShaders[]=
		{
			"Expansion_TeleporterIdle_SoundShader"
		};
	};
	class Expansion_Teleporter_Active_Soundset: Expansion_Teleporter_Soundset
	{
		soundShaders[]=
		{
			"Expansion_TeleporterActive_SoundShader"
		};
	};
};
