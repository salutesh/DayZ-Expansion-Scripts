class CfgPatches
{
	class DayZExpansion_VanillaFixes_Sounds
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
	class baseVehicles_SoundShader;
	class offroad_engine_failed_start_battery_SoundShader: baseVehicles_SoundShader
	{
		volume=0.69999999;
	};
};
