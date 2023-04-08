#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Sounds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Sounds_Effects"};
	};
};
class CfgSoundCurves
{
	class SatelliteDistantAttenuationCurve
	{
		points[] = {{0,1.0},{2000,1.0},{3500,0.5},{4000,0.0}};
	};
};
class CfgSoundShaders
{
	class Expansion_Anomaly_SoundShader
	{
		volume = 1;
		frequency = 1;
		limitation = 0;
		radius = 50;
		range = 50;
	};
	class Expansion_Satillite_SoundShader
	{
		volume = 1;
		frequency = 1;
		limitation = 0;
		radius = 150;
		range = 150;
	};
	class Expansion_AnomalyRumble_SoundShader: Expansion_Anomaly_SoundShader
	{
		samples[] = {{"DayZExpansion\NamalskAdventure\Sounds\data\deep_rumble",1}};
	};
	class Expansion_AnomalyWindBlowActivated_SoundShader: Expansion_Anomaly_SoundShader
	{
		samples[] = {{"DayZExpansion\NamalskAdventure\Sounds\data\wind_blow_short",1}};
	};
	class Expansion_AnomalyArea1_SoundShader: Expansion_Anomaly_SoundShader
	{
		samples[] = {{"DayZExpansion\NamalskAdventure\Sounds\data\anomaly_area_1",1}};
	};
	class Expansion_SatelliteEngine_Boot_SoundShader: Expansion_Satillite_SoundShader
	{
		samples[] = {{"DayZExpansion\NamalskAdventure\Sounds\data\electric_engine",1}};
		range = 200;
	};
	class Expansion_SatelliteEngine_Active_SoundShader: Expansion_Satillite_SoundShader
	{
		samples[] = {{"DayZExpansion\NamalskAdventure\Sounds\data\engine_loop",1}};
		range = 200;
	};
	class Expansion_Satellite_Cry_Distance_SoundShader: Expansion_Satillite_SoundShader
	{
		samples[] = {{"nst\ns_dayz\effects\sounds\data\phoenix_foghorn_distance",1}};
		range = 4000;
	};
};
class CfgSoundSets
{
	class Expansion_AnomalyIdle_Soundset
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 1;
		distanceFilter = "defaultDistanceFilter";
	};
	class Expansion_AnomalyActivated_Soundset
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
		distanceFilter = "defaultDistanceFilter";
	};
	class Expansion_Satellite_Soundset
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
		distanceFilter = "defaultDistanceFilter";
	};
	class Expansion_AnomalyRumble_Soundset: Expansion_AnomalyIdle_Soundset
	{
		soundShaders[] = {"Expansion_AnomalyRumble_SoundShader"};
	};
	class Expansion_AnomalyWindBlowActivated_Soundset: Expansion_AnomalyActivated_Soundset
	{
		soundShaders[] = {"Expansion_AnomalyWindBlowActivated_SoundShader"};
	};
	class Expansion_AnomalyArea1_Soundset: Expansion_AnomalyIdle_Soundset
	{
		soundShaders[] = {"Expansion_AnomalyArea1_SoundShader"};
		volumeFactor = "0.15 * 0.4 * 1.6";
		spatial = 0;
	};
	class Expansion_Satellite_Boot_Soundset: Expansion_Satellite_Soundset
	{
		soundShaders[] = {"Expansion_SatelliteEngine_Boot_SoundShader"};
	};
	class Expansion_Satellite_Active_Soundset: Expansion_Satellite_Soundset
	{
		soundShaders[] = {"Expansion_SatelliteEngine_Active_SoundShader"};
	};
	class Expansion_Satellite_Cry_Distance_SoundSet: Expansion_Satellite_Soundset
	{
		soundShaders[] = {"Phoenix_FogHorn_Distance_SoundShader"};
		volumeCurve = "SatelliteDistantAttenuationCurve";
		volumeFactor = 0.35;
	};
};
