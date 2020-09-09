#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Ground_N250
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class baseEngineExpansion_250N_SoundShader
	{
		range = 80;
	};
	class Expansion_250N_Engine_Ext_Rpm0_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Rpm0",1}};
		frequency = "1 * ((850 + ((rpm - 850)/(8000/5600))) max 850) / 850";
		volume = "0.75 * 1 * engineOn * 0.4 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((850+1200)/2) + 2.5*50),(((850+1200)/2) - 50)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Ext_Rpm1_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Rpm1",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 1200";
		volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.6 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((850+1200)/2) - 2.5*50),(((850+1200)/2) + 50)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1200+1700)/2) + 2.5*150),(((1200+1700)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm1_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Offload_Rpm1",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 1200";
		volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.6 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((850+1200)/2) - 2.5*50),(((850+1200)/2) + 50)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1200+1700)/2) + 2.5*150),(((1200+1700)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Ext_Rpm2_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Rpm2",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 1700";
		volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.85 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1200+1700)/2) - 2.5*150),(((1200+1700)/2) + 150)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1700+2300)/2) + 2.5*150),(((1700+2300)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm2_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Offload_Rpm2",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 1700";
		volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.85 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1200+1700)/2) - 2.5*150),(((1200+1700)/2) + 150)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1700+2300)/2) + 2.5*150),(((1700+2300)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Ext_Rpm3_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Rpm3",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 2300";
		volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1700+2300)/2) - 2.5*150),(((1700+2300)/2) + 150)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((2300+3150)/2) + 2.5*200),(((2300+3150)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm3_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Offload_Rpm3",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 2300";
		volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((1700+2300)/2) - 2.5*150),(((1700+2300)/2) + 150)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((2300+3150)/2) + 2.5*200),(((2300+3150)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Ext_Rpm4_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Rpm4",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 3150";
		volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((2300+3150)/2) - 2.5*200),(((2300+3150)/2) + 200)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((3150+4400)/2) + 2.5*200),(((3150+4400)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm4_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Offload_Rpm4",2}};
		frequency = "1 * (850 + ((rpm - 850)/(8000/5600))) / 3150";
		volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((2300+3150)/2) - 2.5*200),(((2300+3150)/2) + 200)]) * ((850 + ((rpm - 850)/(8000/5600))) factor [(((3150+4400)/2) + 2.5*200),(((3150+4400)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Ext_Rpm5_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Rpm5",2}};
		frequency = "(1 * (850 + ((rpm - 850)/(8000/5600))) / 4400) min (8000/5600)";
		volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((3150+4400)/2) - 2.5*200),(((3150+4400)/2) + 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm5_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Offload_Rpm5",2}};
		frequency = "(1 * (850 + ((rpm - 850)/(8000/5600))) / 4400) min (8000/5600)";
		volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((3150+4400)/2) - 2.5*200),(((3150+4400)/2) + 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Expansion_250N_Engine_Ext_Broken_SoundShader: baseEngineExpansion_250N_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_Ext_Broken",2}};
		frequency = 0.9;
		volume = "0.75 * 1 * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((850 + ((rpm - 850)/(8000/5600))) factor [(((3150+4400)/2) - 2.5*200),(((3150+4400)/2) + 200)]) * ((1 - 0.25*doors) max campos) * (rpm factor[4800,6200])";
	};
	class baseEngineExpansion_250N2_SoundShader
	{
		range = 50;
	};
	class zeroy_old_bike_start_SoundShader: baseEngineExpansion_250N2_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_ext_start",1}};
		volume = 0.7;
	};
	class zeroy_old_bike_stop_SoundShader: baseEngineExpansion_250N2_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Vehicles\Ground\N250\Expansion_250N_Engine_ext_stop",1}};
		volume = 0.5;
	};
};
class CfgSoundSets
{
	class baseEngine_EXT_SoundSet
	{
		sound3DProcessingType = "Vehicle_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		spatial = 1;
		loop = 1;
	};
	class Expansion_250N_Engine_Ext_Rpm0_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Ext_Rpm0_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm1_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Offload_Ext_Rpm1_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Ext_Rpm1_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Ext_Rpm1_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Ext_Rpm2_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Ext_Rpm2_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm2_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Offload_Ext_Rpm2_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Ext_Rpm3_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Ext_Rpm3_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm3_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Offload_Ext_Rpm3_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Ext_Rpm4_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Ext_Rpm4_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm4_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Offload_Ext_Rpm4_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Ext_Rpm5_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Ext_Rpm5_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Offload_Ext_Rpm5_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Offload_Ext_Rpm5_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_250N_Engine_Ext_Broken_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_250N_Engine_Ext_Broken_SoundShader"};
		volumeFactor = 1;
	};
	class baseV3S_Character_SoundSet
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class baseOffroad_Character_SoundSet
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class V3S_Gear_SoundSet: baseV3S_Character_SoundSet
	{
		soundShaders[] = {"V3S_Gear_SoundShader","V3S_Gear_Metal_SoundShader","V3S_Gear_Pedal_SoundShader"};
		volumefactor = 0.5;
		frequencyrandomizer = 1;
		volumerandomizer = 1;
	};
	class V3S_Key_On_SoundSet: baseOffroad_Character_SoundSet
	{
		soundShaders[] = {"Expansion250N_start_SoundShader"};
		volumefactor = 0.5;
		frequencyrandomizer = 1;
		volumerandomizer = 1;
	};
	class V3S_Key_Off_SoundSet: baseOffroad_Character_SoundSet
	{
		soundShaders[] = {"Expansion250N_stop_SoundShader"};
		volumefactor = 0.5;
		frequencyrandomizer = 1;
		volumerandomizer = 1;
	};
	class V3S_lights_SoundSet: baseOffroad_Character_SoundSet
	{
		soundShaders[] = {"V3S_lights_SoundShader"};
		volumefactor = 0.5;
		frequencyrandomizer = 1;
		volumerandomizer = 1;
	};
};
