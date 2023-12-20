#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Air_Merlin
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class Expansion_Merlin_Base_Int_SoundShader
	{
		range = 2000;
	};
	class Expansion_Merlin_Base_Ext_SoundShader
	{
		range = 2000;
	};
	class Expansion_Merlin_Engine_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Engine_Int",1}};
		frequency = "rpm";
		volume = "rpm * (1 - camPos) * 0.4";
	};
	class Expansion_Merlin_Engine_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Engine_Ext",1}};
		frequency = "rpm";
		volume = "rpm * camPos";
	};
	class Expansion_Merlin_Rotor_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Rotor_Int",1}};
		frequency = "rpm * speed";
		volume = "rpm * speed * (1 - camPos) * 0.3";
	};
	class Expansion_Merlin_Rotor_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Rotor_Ext",1}};
		frequency = "rpm * speed";
		volume = "rpm * speed * camPos";
	};
	class Expansion_Merlin_Starter_Start_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_starter_start",1}};
		frequency = 1;
		volume = 1;
	};
	class Expansion_Merlin_Starter_Start_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_starter_start",1}};
		frequency = 1;
		volume = 1;
	};
	class Expansion_Merlin_Starter_Stop_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_starter_stop",1}};
		frequency = 1;
		volume = 1;
	};
	class Expansion_Merlin_Starter_Stop_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_starter_stop",1}};
		frequency = 1;
		volume = 1;
	};
	class Expansion_Merlin_Engine_Idle_Start_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_idle_start",1}};
		frequency = 1;
		volume = 0.3;
	};
	class Expansion_Merlin_Engine_Idle_Start_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_idle_start",1}};
		frequency = 1;
		volume = 0.3;
	};
};
class CfgSoundSets
{
	class Expansion_Merlin_Base_Ext_SoundSet
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
	class Expansion_Merlin_Base_Int_SoundSet
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
	class Expansion_Merlin_Engine_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[] = {"Expansion_Merlin_Engine_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Engine_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[] = {"Expansion_Merlin_Engine_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Rotor_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[] = {"Expansion_Merlin_Rotor_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Rotor_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[] = {"Expansion_Merlin_Rotor_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Starter_Start_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop = 0;
		soundShaders[] = {"Expansion_Merlin_Starter_Start_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Starter_Start_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop = 0;
		soundShaders[] = {"Expansion_Merlin_Starter_Start_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Starter_Stop_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop = 0;
		soundShaders[] = {"Expansion_Merlin_Starter_Stop_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Starter_Stop_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop = 0;
		soundShaders[] = {"Expansion_Merlin_Starter_Stop_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Engine_Idle_Start_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop = 0;
		soundShaders[] = {"Expansion_Merlin_Engine_Idle_Start_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Merlin_Engine_Idle_Start_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop = 0;
		soundShaders[] = {"Expansion_Merlin_Engine_Idle_Start_Ext_SoundShader"};
		volumeFactor = 1;
	};
};
