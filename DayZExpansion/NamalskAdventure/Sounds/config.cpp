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
class CfgSoundShaders
{
	class Expansion_Anomaly_SoundShader
	{
		volume = 1;
		frequency = 1;
		limitation = 0;
		radius = 40;
		range = 40;
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
};
