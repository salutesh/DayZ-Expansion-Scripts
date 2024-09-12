#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Ground_Vehicle_CivilianSedan
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundSets
{
	class baseVehicles_SoundSet;
	class Vehicle_CivilianSedan_damper_left_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"CivilianSedan_damper_metal_SoundShader"};
		volumeCurve = "vehicleBumperVolumeCurve";
		volumeFactor = 0.5;
		frequencyFactor = 1;
		frequencyRandomizer = 3;
		volumeRandomizer = 1;
		positionOffset[] = {0.5,0,0};
		sound3DProcessingType = "Vehicle_Damper_Ext_3DProcessingType";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Vehicle_CivilianSedan_damper_right_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"CivilianSedan_damper_metal_SoundShader"};
		volumeFactor = 0.5;
		frequencyFactor = 1;
		frequencyRandomizer = 3;
		volumeRandomizer = 1;
		volumeCurve = "vehicleBumperVolumeCurve";
		positionOffset[] = {-0.5,0,0};
		sound3DProcessingType = "Vehicle_Damper_Ext_3DProcessingType";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Vehicle_CivilianSedan_Engine_Thrust_Down_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"Offroad_Engine_Thrust_Down_SoundShader"};
		volumeFactor = 1;
		frequencyFactor = 1;
		volumeCurve = "vehicleBumperVolumeCurve";
		positionOffset[] = {0,0,0.3};
		sound3DProcessingType = "Vehicle_Damper_Ext_3DProcessingType";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
};
class CfgVehicles
{
	class ExpansionVehicleCarBase;
	class Vehicle_CivilianSedan: ExpansionVehicleCarBase
	{
		class ExpansionSounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"CivilianSedan_Engine_Offload_Ext_Rpm1_SoundSet","CivilianSedan_Engine_Offload_Ext_Rpm2_SoundSet","CivilianSedan_Engine_Offload_Ext_Rpm3_SoundSet","CivilianSedan_Engine_Offload_Ext_Rpm4_SoundSet","CivilianSedan_Engine_Offload_Ext_Rpm5_SoundSet","CivilianSedan_Engine_Ext_Rpm0_SoundSet","CivilianSedan_Engine_Ext_Rpm1_SoundSet","CivilianSedan_Engine_Ext_Rpm2_SoundSet","CivilianSedan_Engine_Ext_Rpm3_SoundSet","CivilianSedan_Engine_Ext_Rpm4_SoundSet","CivilianSedan_Engine_Ext_Rpm5_SoundSet","CivilianSedan_Engine_Ext_Broken_SoundSet","CivilianSedan_Tires_ice_slow_Ext_SoundSet","CivilianSedan_Tires_ice_fast_Ext_SoundSet","CivilianSedan_Tires_snow_slow_Ext_SoundSet","CivilianSedan_Tires_snow_fast_Ext_SoundSet","CivilianSedan_Tires_rock_slow_Ext_SoundSet","CivilianSedan_Tires_rock_fast_Ext_SoundSet","CivilianSedan_Tires_grass_slow_Ext_SoundSet","CivilianSedan_Tires_grass_fast_Ext_SoundSet","CivilianSedan_Tires_gravel_slow_Ext_SoundSet","CivilianSedan_Tires_gravel_fast_Ext_SoundSet","CivilianSedan_Tires_gravel_dust_fast_Ext_SoundSet","CivilianSedan_Tires_asphalt_slow_Ext_SoundSet","CivilianSedan_Tires_asphalt_fast_Ext_SoundSet","CivilianSedan_Tires_water_slow_Ext_SoundSet","CivilianSedan_Tires_water_fast_Ext_SoundSet","CivilianSedan_skid_dirt_SoundSet","CivilianSedan_dirt_turn_SoundSet","CivilianSedan_Rain_Ext_SoundSet","Vehicle_CivilianSedan_damper_left_SoundSet","Vehicle_CivilianSedan_damper_right_SoundSet"};
			soundSetsInt[] = {"CivilianSedan_Tires_Asphalt_Fast_General_Int_SoundSet","CivilianSedan_Wind_SoundSet"};
		};
	};
};
