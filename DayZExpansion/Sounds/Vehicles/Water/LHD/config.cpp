#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Water_LHD
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_Horn_SoundShader
	{
		range = 350;
	};
	class Expansion_boat_lhd_base_Int_SoundShader
	{
		range = 20;
	};
	class Expansion_boat_lhd_base_Ext_SoundShader
	{
		range = 3500;
	};
	class Expansion_boat_lhd_loop_Ext_SoundShader: Expansion_boat_lhd_base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\LHD\ext-boat-engine-high-01",1}};
		frequency = "0.5 + (engineOn * (rpm / 16))";
		volume = "(engineOn * 0.4) + (engineOn * (rpm / 4))";
	};
	class Expansion_boat_lhd_loop_Int_SoundShader: Expansion_boat_lhd_base_Int_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\LHD\ext-boat-engine-high-01",1}};
		frequency = "0.5 + (engineOn * (rpm / 8))";
		volume = "(engineOn * 0.4) + (engineOn * (rpm / 4))";
	};
	class Expansion_LHD_Horn_Ext_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\LHD\Expansion_LHD_Horn_Ext",1}};
		volume = 1;
	};
	class Expansion_LHD_Horn_Int_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Water\LHD\Expansion_LHD_Horn_Int",1}};
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
	class Expansion_boat_lhd_base_Ext_SoundSet
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
	class Expansion_boat_lhd_base_Int_SoundSet
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
	class Expansion_boat_lhd_loop_Int_SoundSet: Expansion_boat_lhd_base_Int_SoundSet
	{
		soundShaders[] = {"Expansion_boat_lhd_loop_Int_SoundShader"};
		volumeFactor = 0.5;
	};
	class Expansion_boat_lhd_loop_Ext_SoundSet: Expansion_boat_lhd_base_Ext_SoundSet
	{
		soundShaders[] = {"Expansion_boat_lhd_loop_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_LHD_Horn_Ext_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_LHD_Horn_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_LHD_Horn_Int_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_LHD_Horn_Int_SoundShader"};
		volumeFactor = 1;
	};
};
class CfgVehicles
{
	class ExpansionBoatScript;
	class ExpansionBoatScriptSoundProxyBase;
	class ExpansionLHDAudio: ExpansionBoatScriptSoundProxyBase
	{
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_boat_lhd_loop_Ext_SoundSet"};
			soundSetsInt[] = {"offroad_Wind_SoundSet"};
		};
	};
	class ExpansionLHD: ExpansionBoatScript
	{
		class Sounds
		{
			boatSoundProxy = "ExpansionLHDAudio";
		};
	};
};
