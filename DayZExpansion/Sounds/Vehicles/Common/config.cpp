#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Common
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
		range = 150;
	};
	class Expansion_Horn_Ext_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Ext",1}};
		volume = 1;
	};
	class Expansion_Horn_Int_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Int",1}};
		volume = 1;
	};
	class Expansion_Truck_Horn_Ext_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Ext",1}};
		volume = 1;
	};
	class Expansion_Truck_Horn_Int_SoundShader: Expansion_Horn_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Int",1}};
		volume = 1;
	};
	class Expansion_CarLock_SoundShader
	{
		range = 20;
	};
	class Expansion_Car_Lock_SoundShader: Expansion_CarLock_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Common\Expansion_CarLock",1}};
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
	class Expansion_Horn_Ext_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_Horn_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Horn_Int_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_Horn_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Truck_Horn_Ext_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_Truck_Horn_Ext_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Truck_Horn_Int_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_Truck_Horn_Int_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_Car_Lock_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[] = {"Expansion_Car_Lock_SoundShader"};
		volumeFactor = 1;
	};
};
