#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Water_Utility
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_Horn_SoundShader
	{
		range = 200;
	};
	class Expansion_Utility_Base_Int_SoundShader
	{
		range = 20;
	};
	class Expansion_Utility_Base_Ext_SoundShader
	{
		range = 1000;
	};
	class Expansion_Utility_Engine_Int_SoundShader: Expansion_Utility_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\Utility\Expansion_Utility_Engine_Ext",1}};
		frequency = "0.5 + (engineOn * (rpm / 16))";
		volume = "(engineOn * 0.4) + (engineOn * (rpm / 4))";
	};
	class Expansion_Utility_Idle_Int_SoundShader: Expansion_Utility_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\Utility\Expansion_Utility_Idle_Ext",1}};
		volume = "(engineOn * 0.4) + (engineOn * (rpm / 4))";
	};
	class Expansion_Utility_Horn_Ext_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\Utility\Expansion_Utility_Horn_Ext",1}};
		volume = 1;
	};
	class Expansion_Utility_Horn_Int_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\Utility\Expansion_Utility_Horn_Int",1}};
		volume = 1;
	};
};
class CfgSoundSets
{
	class Expansion_Horn_SoundSet
	{
		sound3DProcessingType = "Vehicle_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		spatial = 1;
		loop = 0;
	};
	class Expansion_Utility_Base_Ext_SoundSet
	{
		sound3DProcessingType = "Vehicle_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		occlusionFactor = 0;
		obstructionFactor = 0;
		spatial = 1;
		loop = 1;
		positionOffset[] = {0,0,0};
	};
	class Expansion_Utility_Base_Int_SoundSet
	{
		sound3DProcessingType = "Vehicle_Int_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		occlusionFactor = 0;
		obstructionFactor = 0;
		spatial = 1;
		loop = 1;
		positionOffset[] = {0,0,0};
	};
	class Expansion_Utility_Engine_Ext_SoundSet: Expansion_Utility_Base_Ext_SoundSet
	{
		soundShaders[] = {"Expansion_Utility_Engine_Int_SoundShader"};
		volumeFactor = 0.12;
	};
	class Expansion_Utility_Idle_Ext_SoundSet: Expansion_Utility_Base_Ext_SoundSet
	{
		soundShaders[] = {"Expansion_Utility_Idle_Int_SoundShader"};
		volumeFactor = 0.12;
	};
	class Expansion_Utility_Horn_Ext_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_Utility_Horn_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Utility_Horn_Int_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_Utility_Horn_Int_SoundShader"};
		volumeFactor = 1;
	};
};
class CfgVehicles
{
	class ExpansionBoatScript;
	class ExpansionBoatScriptSoundProxyBase;
	class ExpansionUtilityBoatAudio: ExpansionBoatScriptSoundProxyBase
	{
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_Utility_Engine_Ext_SoundSet"};
			soundSetsInt[] = {"offroad_Wind_SoundSet"};
		};
	};
	class ExpansionUtilityBoat: ExpansionBoatScript
	{
		class Sounds
		{
			boatSoundProxy = "ExpansionUtilityBoatAudio";
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_Utility_Engine_Ext_SoundSet","offroad_Engine_Offload_Ext_Rpm2_SoundSet","offroad_Engine_Offload_Ext_Rpm3_SoundSet","offroad_Engine_Offload_Ext_Rpm4_SoundSet","offroad_Engine_Offload_Ext_Rpm5_SoundSet","Expansion_Utility_Idle_Ext_SoundSet","offroad_Engine_Ext_Rpm1_SoundSet","offroad_Engine_Ext_Rpm2_SoundSet","offroad_Engine_Ext_Rpm3_SoundSet","offroad_Engine_Ext_Rpm4_SoundSet","offroad_Engine_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Broken_SoundSet","offroad_Tires_rock_slow_Ext_SoundSet","offroad_Tires_rock_fast_Ext_SoundSet","offroad_Tires_grass_slow_Ext_SoundSet","offroad_Tires_grass_fast_Ext_SoundSet","offroad_Tires_gravel_slow_Ext_SoundSet","offroad_Tires_gravel_fast_Ext_SoundSet","offroad_Tires_gravel_dust_fast_Ext_SoundSet","offroad_Tires_asphalt_slow_Ext_SoundSet","offroad_Tires_asphalt_fast_Ext_SoundSet","offroad_Tires_water_slow_Ext_SoundSet","offroad_Tires_water_fast_Ext_SoundSet","Offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_Rain_Ext_SoundSet","offroad_damper_left_SoundSet","offroad_damper_right_SoundSet"};
			soundSetsInt[] = {"Offroad_Tires_Asphalt_Fast_General_Int_SoundSet","Offroad_Wind_SoundSet"};
		};
	};
};
