#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Air_An2
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_An2_Base_Int_SoundShader
	{
		range = 20;
	};
	class Expansion_An2_Base_Ext_SoundShader
	{
		range = 1100;
	};
	class Expansion_An2_Engine_Int_SoundShader: Expansion_An2_Base_Int_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\An2\Expansion_An2_Engine_Int",1}};
		frequency = "rpm";
		volume = "engineOn * rpm * (1-camPos) * 0.4";
	};
	class Expansion_An2_Engine_Ext_SoundShader: Expansion_An2_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\An2\Expansion_An2_Engine_Ext",1}};
		frequency = "rpm";
		volume = "engineOn * rpm * camPos";
	};
};
class CfgSoundSets
{
	class Expansion_An2_Base_Ext_SoundSet
	{
		sound3DProcessingType = "Vehicle_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		occlusionFactor = 0;
		obstructionFactor = 0;
		spatial = 1;
		loop = 1;
		positionOffset[] = {0,0,1};
	};
	class Expansion_An2_Base_Int_SoundSet
	{
		sound3DProcessingType = "Vehicle_Int_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		occlusionFactor = 0;
		obstructionFactor = 0;
		spatial = 1;
		loop = 1;
		positionOffset[] = {0,0,1};
	};
	class Expansion_An2_Engine_Int_SoundSet: Expansion_An2_Base_Int_SoundSet
	{
		soundShaders[] = {"Expansion_An2_Engine_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_An2_Engine_Ext_SoundSet: Expansion_An2_Base_Ext_SoundSet
	{
		soundShaders[] = {"Expansion_An2_Engine_Ext_SoundShader"};
		volumeFactor = 1;
	};
};
