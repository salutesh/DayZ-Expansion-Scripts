#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Ground_Vehicle_Sedan_02
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
	class Vehicle_Sedan_02_damper_left_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"Sedan_02_damper_metal_SoundShader"};
		volumeCurve = "vehicleBumpervolumeCurve";
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
	class Vehicle_Sedan_02_damper_right_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"Sedan_02_damper_metal_SoundShader"};
		volumeFactor = 0.5;
		frequencyFactor = 1;
		frequencyRandomizer = 3;
		volumeRandomizer = 1;
		volumeCurve = "vehicleBumpervolumeCurve";
		positionOffset[] = {-0.5,0,0};
		sound3DProcessingType = "Vehicle_Damper_Ext_3DProcessingType";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Vehicle_Sedan_02_Engine_Thrust_Down_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"Offroad_Engine_Thrust_Down_SoundShader"};
		volumeFactor = 1;
		frequencyFactor = 1;
		volumeCurve = "vehicleBumpervolumeCurve";
		positionOffset[] = {0,0,0.3};
		sound3DProcessingType = "Vehicle_Damper_Ext_3DProcessingType";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
};
class CfgVehicles
{
	class CarScript;
	class ExpansionBus: CarScript
	{
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"offroad_Engine_Offload_Ext_Rpm1_SoundSet","offroad_Engine_Offload_Ext_Rpm2_SoundSet","offroad_Engine_Offload_Ext_Rpm3_SoundSet","offroad_Engine_Offload_Ext_Rpm4_SoundSet","offroad_Engine_Offload_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Rpm0_SoundSet","offroad_Engine_Ext_Rpm1_SoundSet","offroad_Engine_Ext_Rpm2_SoundSet","offroad_Engine_Ext_Rpm3_SoundSet","offroad_Engine_Ext_Rpm4_SoundSet","offroad_Engine_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Broken_SoundSet","offroad_Tires_rock_slow_Ext_SoundSet","offroad_Tires_rock_fast_Ext_SoundSet","offroad_Tires_grass_slow_Ext_SoundSet","offroad_Tires_grass_fast_Ext_SoundSet","offroad_Tires_gravel_slow_Ext_SoundSet","offroad_Tires_gravel_fast_Ext_SoundSet","offroad_Tires_gravel_dust_fast_Ext_SoundSet","offroad_Tires_asphalt_slow_Ext_SoundSet","offroad_Tires_asphalt_fast_Ext_SoundSet","offroad_Tires_water_slow_Ext_SoundSet","offroad_Tires_water_fast_Ext_SoundSet","Offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_Rain_Ext_SoundSet","offroad_damper_left_SoundSet","offroad_damper_right_SoundSet"};
			soundSetsInt[] = {"Offroad_Tires_Asphalt_Slow_General_Int_SoundSet","Offroad_Tires_Asphalt_Fast_General_Int_SoundSet","Offroad_Wind_SoundSet"};
		};
	};
	class ExpansionVehicleCarBase;
	class Vehicle_ExpansionBus: ExpansionVehicleCarBase
	{
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"offroad_Engine_Offload_Ext_Rpm1_SoundSet","offroad_Engine_Offload_Ext_Rpm2_SoundSet","offroad_Engine_Offload_Ext_Rpm3_SoundSet","offroad_Engine_Offload_Ext_Rpm4_SoundSet","offroad_Engine_Offload_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Rpm0_SoundSet","offroad_Engine_Ext_Rpm1_SoundSet","offroad_Engine_Ext_Rpm2_SoundSet","offroad_Engine_Ext_Rpm3_SoundSet","offroad_Engine_Ext_Rpm4_SoundSet","offroad_Engine_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Broken_SoundSet","offroad_Tires_rock_slow_Ext_SoundSet","offroad_Tires_rock_fast_Ext_SoundSet","offroad_Tires_grass_slow_Ext_SoundSet","offroad_Tires_grass_fast_Ext_SoundSet","offroad_Tires_gravel_slow_Ext_SoundSet","offroad_Tires_gravel_fast_Ext_SoundSet","offroad_Tires_gravel_dust_fast_Ext_SoundSet","offroad_Tires_asphalt_slow_Ext_SoundSet","offroad_Tires_asphalt_fast_Ext_SoundSet","offroad_Tires_water_slow_Ext_SoundSet","offroad_Tires_water_fast_Ext_SoundSet","Offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_Rain_Ext_SoundSet","Vehicle_offroad_damper_left_SoundSet","Vehicle_offroad_damper_right_SoundSet"};
			soundSetsInt[] = {"Offroad_Tires_Asphalt_Slow_General_Int_SoundSet","Offroad_Tires_Asphalt_Fast_General_Int_SoundSet","Offroad_Wind_SoundSet"};
		};
	};
};
