#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Ground_Hatchback_02
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders{};
class CfgSoundSets{};
class CfgVehicles
{
	class ExpansionVehicleCarBase;
	class Vehicle_Hatchback_02: ExpansionVehicleCarBase
	{
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Hatchback_02_Engine_Offload_Ext_Rpm1_SoundSet","Hatchback_02_Engine_Offload_Ext_Rpm2_SoundSet","Hatchback_02_Engine_Offload_Ext_Rpm3_SoundSet","Hatchback_02_Engine_Offload_Ext_Rpm4_SoundSet","Hatchback_02_Engine_Offload_Ext_Rpm5_SoundSet","Hatchback_02_Engine_Ext_Rpm0_SoundSet","Hatchback_02_Engine_Ext_Rpm1_SoundSet","Hatchback_02_Engine_Ext_Rpm2_SoundSet","Hatchback_02_Engine_Ext_Rpm3_SoundSet","Hatchback_02_Engine_Ext_Rpm4_SoundSet","Hatchback_02_Engine_Ext_Rpm5_SoundSet","Hatchback_02_Engine_Ext_Broken_SoundSet","Hatchback_02_Tires_rock_slow_Ext_SoundSet","Hatchback_02_Tires_rock_fast_Ext_SoundSet","Hatchback_02_Tires_grass_slow_Ext_SoundSet","Hatchback_02_Tires_grass_fast_Ext_SoundSet","Hatchback_02_Tires_gravel_slow_Ext_SoundSet","Hatchback_02_Tires_gravel_fast_Ext_SoundSet","Hatchback_02_Tires_gravel_dust_fast_Ext_SoundSet","Hatchback_02_Tires_asphalt_slow_Ext_SoundSet","Hatchback_02_Tires_asphalt_fast_Ext_SoundSet","Hatchback_02_Tires_water_slow_Ext_SoundSet","Hatchback_02_Tires_water_fast_Ext_SoundSet","Hatchback_02_skid_dirt_SoundSet","Hatchback_02_dirt_turn_SoundSet","Hatchback_02_Rain_Ext_SoundSet","Hatchback_02_damper_left_SoundSet","Hatchback_02_damper_right_SoundSet"};
			soundSetsInt[] = {"Hatchback_02_Tires_Asphalt_Fast_General_Int_SoundSet","Hatchback_02_Wind_SoundSet"};
		};
	};
};
