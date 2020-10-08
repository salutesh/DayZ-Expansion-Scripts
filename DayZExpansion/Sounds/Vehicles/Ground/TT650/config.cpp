#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Ground_TT650
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class baseEngineOffroad_SoundShader
	{
		range = 80;
	};
	class Expansion_TT650_Ext_Rpm0_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Rpm0",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "(1 + ((750 - rpm) / (900 - 650)))";
		ex_volume = "0.75 * engineOn * ((rpm factor [600, 750]) - (rpm factor [750, 900]))";
	};
	class Expansion_TT650_Ext_Rpm1_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Rpm1",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "(1 + ((1200 - rpm) / (1500 - 800)))";
		ex_volume = "0.75 * engineOn * ((rpm factor [800, 1200]) - (rpm factor [1200, 1500]))";
	};
	class Expansion_TT650_Offload_Ext_Rpm1_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Offload_Rpm1",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "0";
		ex_volume = "0";
	};
	class Expansion_TT650_Ext_Rpm2_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Rpm2",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "(1 + ((1850 - rpm) / (2500 - 1500)))";
		ex_volume = "0.75 * engineOn * ((rpm factor [1500, 1850]) - (rpm factor [1850, 2500]))";
	};
	class Expansion_TT650_Offload_Ext_Rpm2_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Offload_Rpm2",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "0";
		ex_volume = "0";
	};
	class Expansion_TT650_Ext_Rpm3_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Rpm3",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "(1 + ((2350 - rpm) / (4000 - 1700)))";
		ex_volume = "0.75 * engineOn * ((rpm factor [1700, 2350]) - (rpm factor [2350, 3000]))";
	};
	class Expansion_TT650_Offload_Ext_Rpm3_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Offload_Rpm3",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "0";
		ex_volume = "0";
	};
	class Expansion_TT650_Ext_Rpm4_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Rpm4",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "(1 + ((3350 - rpm) / (4000 - 2700)))";
		ex_volume = "0.75 * engineOn * ((rpm factor [2700, 3350]) - (rpm factor [3350, 4000]))";
	};
	class Expansion_TT650_Offload_Ext_Rpm4_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Offload_Rpm4",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "0";
		ex_volume = "0";
	};
	class Expansion_TT650_Ext_Rpm5_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Rpm5",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "(1 + ((4350 - rpm) / (5000 - 3700)))";
		ex_volume = "0.75 * engineOn * ((rpm factor [3700, 4350]) - (rpm factor [4350, 5000]))";
	};
	class Expansion_TT650_Offload_Ext_Rpm5_SoundShader: baseEngineOffroad_SoundShader
	{
		samples[] = {{"\DayZExpansion\Sounds\Vehicles\Ground\TT650\Expansion_TT650_Engine_Ext_Offload_Rpm5",1}};
		frequency = 1;
		volume = 1;
		ex_frequency = "0";
		ex_volume = "0";
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
	class Expansion_TT650_Ext_Rpm0_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Ext_Rpm0_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Ext_Rpm1_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Ext_Rpm1_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Offload_Ext_Rpm1_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Offload_Ext_Rpm1_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Ext_Rpm2_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Ext_Rpm2_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Offload_Ext_Rpm2_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Offload_Ext_Rpm2_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Ext_Rpm3_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Ext_Rpm3_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Offload_Ext_Rpm3_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Offload_Ext_Rpm3_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Ext_Rpm4_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Ext_Rpm4_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Offload_Ext_Rpm4_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Offload_Ext_Rpm4_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Ext_Rpm5_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Ext_Rpm5_SoundShader"};
		volumeFactor = 1;
	};
	class Expansion_TT650_Offload_Ext_Rpm5_SoundSet: baseEngine_EXT_SoundSet
	{
		soundShaders[] = {"Expansion_TT650_Offload_Ext_Rpm5_SoundShader"};
		volumeFactor = 1;
	};
};
