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
class CfgSoundShaders
{
	class baseVehicles_SoundShader;
	class baseTires_SoundShader;
	class baseEngineVehicle_Sedan_02_SoundShader
	{
		range = 80;
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm0_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Rpm0",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * ((900 + ((rpm - 900)/(6500/5600))) max 900) / 850";
		ex_volume = "0.75 * 1 * engineOn * 0.4 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((850+1200)/2) + 2.5*50),(((850+1200)/2) - 50)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm1_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Rpm1",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 1200";
		ex_volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.6 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((850+1200)/2) - 2.5*50),(((850+1200)/2) + 50)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1200+1700)/2) + 2.5*150),(((1200+1700)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm1_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Offload_Rpm1",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 1200";
		ex_volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * 0.8 * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.6 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((850+1200)/2) - 2.5*50),(((850+1200)/2) + 50)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1200+1700)/2) + 2.5*150),(((1200+1700)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm2_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Rpm2",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 1700";
		ex_volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.85 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1200+1700)/2) - 2.5*150),(((1200+1700)/2) + 150)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1700+2300)/2) + 2.5*150),(((1700+2300)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm2_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Offload_Rpm2",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 1700";
		ex_volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * 0.8 * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 0.85 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1200+1700)/2) - 2.5*150),(((1200+1700)/2) + 150)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1700+2300)/2) + 2.5*150),(((1700+2300)/2) - 150)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm3_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Rpm3",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 2300";
		ex_volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1700+2300)/2) - 2.5*150),(((1700+2300)/2) + 150)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((2300+3250)/2) + 2.5*200),(((2300+3250)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm3_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Offload_Rpm3",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 2300";
		ex_volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * 0.8 * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((1700+2300)/2) - 2.5*150),(((1700+2300)/2) + 150)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((2300+3250)/2) + 2.5*200),(((2300+3250)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm4_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Rpm4",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 3250";
		ex_volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((2300+3250)/2) - 2.5*200),(((2300+3250)/2) + 200)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((3250+4400)/2) + 2.5*200),(((3250+4400)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm4_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Offload_Rpm4",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "0.85 * (900 + ((rpm - 900)/(6500/5600))) / 3250";
		ex_volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * 0.8 * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((2300+3250)/2) - 2.5*200),(((2300+3250)/2) + 200)]) * ((900 + ((rpm - 900)/(6500/5600))) factor [(((3250+4400)/2) + 2.5*200),(((3250+4400)/2) - 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm5_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Rpm5",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(0.85 * (900 + ((rpm - 900)/(6500/5600))) / 4400) min (6500/5600)";
		ex_volume = "0.75 * 1 * (thrust factor[0.1,0.45]) * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((3250+4400)/2) - 2.5*200),(((3250+4400)/2) + 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm5_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\Offroad\Offroad_Engine_Ext_Offload_Rpm5",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(0.85 * (900 + ((rpm - 900)/(6500/5600))) / 4400) min (6500/5600)";
		ex_volume = "0.75 * 1 * (thrust factor[0.6,0.2]) * 0.8 * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((3250+4400)/2) - 2.5*200),(((3250+4400)/2) + 200)]) * ((1 - 0.25*doors) max campos)";
	};
	class Vehicle_Sedan_02_Engine_Ext_Broken_SoundShader: baseEngineVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\add_layers\Offroad_Engine_Ext_Broken",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = 0.9;
		ex_volume = "1.4 * 0.75 * 1 * (0.7 + 0.3 * (speed factor [10,60])) * engineOn * 1 * ((900 + ((rpm - 900)/(6500/5600))) factor [(((3250+4400)/2) - 2.5*200),(((3250+4400)/2) + 200)]) * ((1 - 0.25*doors) max campos) * (rpm factor[5400,5900])";
	};
	class baseTiresVehicle_Sedan_02_SoundShader
	{
		range = 50;
	};
	class Vehicle_Sedan_02_Tires_Asphalt_Fast_General_Int_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Asphalt_Fast_Int",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.125 + 0.075*doors) * (speed factor[10,22]) * (speed factor [-10,80]) * (1 - 0.25*water)";
	};
	class Vehicle_Sedan_02_Tires_Rock_Slow_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Rock_Slow_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * 0.65 * (speed factor[1,5]) * (speed factor[25,15]) * (speed factor [-10,80]) * 0.7 * rock*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Grass_Slow_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Grass_Slow_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * 0.65 * (speed factor[1,5]) * (speed factor[25,15]) * (speed factor [-10,80]) * grass*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Gravel_Slow_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Gravel_Slow_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * 0.65 * (speed factor[1,5]) * (speed factor[25,15]) * (speed factor [-10,80]) * 0.7 * (gravel+0.25*rock)*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Asphalt_Slow_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Asphalt_Slow_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * 0.65 * (speed factor[1,5]) * (speed factor[25,15]) * (speed factor [-10,80]) * 0.6 * asphalt*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Water_Slow_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Water_Slow_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * 0.65 * (speed factor[1,5]) * (speed factor[25,15]) * (speed factor [-10,80]) * 0.8 * water * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Rock_Fast_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Rock_Fast_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * (speed factor[10,22]) * (speed factor [-10,80]) * 0.7 * rock*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Grass_Fast_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Grass_Fast_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "1.1 * (1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * (speed factor[10,22]) * (speed factor [-10,80]) * grass*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Gravel_Fast_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Gravel_Fast_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * (speed factor[10,22]) * (speed factor [-10,80]) * 0.7 * (gravel+0.25*rock)*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Gravel_Dust_Fast_Ext_SoundShader: baseTires_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Gravel_Dust_Fast_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(((speed factor[5,30]) * 0.4 * (gravel + 0.75*rock + 0.5*grass)*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)) max ((0.2 + 0.35*(gravel+rock+grass)) * latslipDrive * (speed factor[5,10]))) * (1 - 0.2*campos)";
	};
	class Vehicle_Sedan_02_Tires_Asphalt_Fast_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Asphalt_Fast_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * (speed factor[10,22]) * (speed factor [-10,80]) * 0.6 * asphalt*(1-(water/4)) * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_Tires_Water_Fast_Ext_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_Tires_Water_Fast_Ext",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * (speed factor[10,22]) * (speed factor [-10,80]) * 0.8 * water * ((0.7 - 0.1*doors) max campos)";
	};
	class Vehicle_Sedan_02_skid_dirt_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_skid_dirt_EXT",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "(1+(0.3*(speed factor[5,60])))";
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * ((latSlipDrive factor[0.3,1]) * (speed factor[5,10]) * ((1 - 0.75*doors) max campos) * (1 - 0.5*grass) * 0.125)";
	};
	class Vehicle_Sedan_02_dirt_turn_SoundShader: baseTiresVehicle_Sedan_02_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\tires\offroad_dirt_turn_EXT",1}};
		volume = 1;
		frequency = 1;
		ex_volume = "(0.35 + 0.65 * latSlipDrive) * 0.065 * (abs(steerdelta) factor[0.01,0.05]) * (speed factor[15,0]) * ((1 - 0.75*doors) max campos)";
		ex_frequency = 1.0;
	};
	class Vehicle_Sedan_02_Rain_Ext_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\add_layers\offroad_Rain_EXT",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = "1 + 0.25*rain";
		ex_volume = "rain factor [0.1,1]";
		range = 25;
	};
	class Vehicle_Sedan_02_Wind_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\add_layers\offroad_wind",1}};
		volume = 1;
		frequency = 1;
		ex_frequency = 1.1;
		ex_volume = "(speed factor [15,65]) * (1 - doors)";
		range = 50;
	};
	class Vehicle_Sedan_02_damper_metal_SoundShader: baseVehicles_SoundShader
	{
		samples[] = {{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_1",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_2",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_3",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_4",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_5",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_6",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_7",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_8",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_9",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_10",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_11",1},{"\DZ\sounds\vehicles\shared\collisions\offroad_damper_metal_12",1}};
		volume = 1;
		frequency = 1;
		ex_volume = "0.25 + 0.75 * (speed factor[0,40])";
		ex_frequency = 1.0;
	};
};
class CfgSoundSets
{
	class baseVehicles_SoundSet;
	class baseTires_Ext_SoundSet;
	class baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		sound3DProcessingType = "Vehicle_Tires_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleAttenuationCurve";
		volumeFactor = 2;
		occlusionFactor = 0;
		obstructionFactor = 0;
		spatial = 1;
		loop = 1;
	};
	class baseTiresVehicle_Sedan_02_Int_SoundSet
	{
		spatial = 0;
		loop = 1;
	};
	class baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		sound3DProcessingType = "Vehicle_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		occlusionFactor = 0;
		obstructionFactor = 0;
		spatial = 1;
		loop = 1;
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm0_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Ext_Rpm0_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm1_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Ext_Rpm1_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm1_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Offload_Ext_Rpm1_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm2_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Ext_Rpm2_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm2_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Offload_Ext_Rpm2_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm3_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Ext_Rpm3_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm3_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Offload_Ext_Rpm3_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm4_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Ext_Rpm4_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm4_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Offload_Ext_Rpm4_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Ext_Rpm5_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Ext_Rpm5_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Offload_Ext_Rpm5_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Offload_Ext_Rpm5_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Engine_Ext_Broken_SoundSet: baseEngineVehicle_Sedan_02_EXT_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Engine_Ext_Broken_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Tires_rock_slow_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_rock_slow_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_rock_fast_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_rock_fast_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_grass_slow_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_grass_slow_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_grass_fast_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_grass_fast_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_gravel_slow_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_gravel_slow_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_gravel_fast_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_gravel_fast_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_asphalt_slow_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_asphalt_slow_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_asphalt_fast_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_asphalt_fast_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_asphalt_fast_general_INT_SoundSet: baseTiresVehicle_Sedan_02_Int_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_asphalt_fast_general_INT_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Tires_water_slow_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_water_slow_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_water_fast_EXT_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_water_fast_EXT_SoundShader"};
		volumeFactor = 2;
	};
	class Vehicle_Sedan_02_Tires_gravel_dust_fast_EXT_SoundSet: baseTires_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Tires_gravel_dust_fast_EXT_SoundShader"};
		volumeFactor = 2;
		sound3DProcessingType = "Vehicle_Tires_Dust_Ext_3DProcessingType";
	};
	class Vehicle_Sedan_02_skid_dirt_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_skid_dirt_SoundShader"};
	};
	class Vehicle_Sedan_02_dirt_turn_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_dirt_turn_SoundShader"};
	};
	class Vehicle_Sedan_02_rattling_left_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_rattling_left_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_rattling_right_SoundSet: baseTiresVehicle_Sedan_02_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_rattling_right_SoundShader"};
		volumeFactor = 1;
	};
	class Vehicle_Sedan_02_Rain_Ext_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Rain_Ext_SoundShader"};
		positionOffset[] = {0,0.3,-0.3};
		sound3DProcessingType = "Vehicle_Rain_Ext_3DProcessingType";
		volumeCurve = "vehicleAttenuationCurve";
		frequencyFactor = 1.15;
		spatial = 1;
		doppler = 0;
		loop = 1;
	};
	class Vehicle_Sedan_02_Wind_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_Wind_SoundShader"};
		spatial = 0;
		doppler = 0;
		loop = 1;
	};
	class Vehicle_Sedan_02_damper_left_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_damper_metal_SoundShader"};
		volumeCurve = "vehicleBumperex_volumeCurve";
		volumeFactor = 0.5;
		frequencyFactor = 1;
		frequencyRandomizer = 3;
		volumeRandomizer = 1;
		playTrigger = "damperLeft factor[0.29,0.30]";
		positionOffset[] = {0.5,0,0};
		sound3DProcessingType = "Vehicle_Damper_Ext_3DProcessingType";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Vehicle_Sedan_02_damper_right_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[] = {"Vehicle_Sedan_02_damper_metal_SoundShader"};
		volumeFactor = 0.5;
		frequencyFactor = 1;
		frequencyRandomizer = 3;
		volumeRandomizer = 1;
		volumeCurve = "vehicleBumperex_volumeCurve";
		playTrigger = "damperRight factor[0.29,0.30]";
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
		volumeCurve = "vehicleBumperex_volumeCurve";
		playTrigger = "(thrust factor[0.5,0.49]) * (rpm factor[1400,1500]) * engineOn";
		positionOffset[] = {0,0,0.3};
		sound3DProcessingType = "Vehicle_Damper_Ext_3DProcessingType";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
};
class CfgVehicles
{
	class ExpansionVehicleCarBase;
	class Vehicle_Sedan_02: ExpansionVehicleCarBase
	{
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Vehicle_Sedan_02_Engine_Offload_Ext_Rpm1_SoundSet","Vehicle_Sedan_02_Engine_Offload_Ext_Rpm2_SoundSet","Vehicle_Sedan_02_Engine_Offload_Ext_Rpm3_SoundSet","Vehicle_Sedan_02_Engine_Offload_Ext_Rpm4_SoundSet","Vehicle_Sedan_02_Engine_Offload_Ext_Rpm5_SoundSet","Vehicle_Sedan_02_Engine_Ext_Rpm0_SoundSet","Vehicle_Sedan_02_Engine_Ext_Rpm1_SoundSet","Vehicle_Sedan_02_Engine_Ext_Rpm2_SoundSet","Vehicle_Sedan_02_Engine_Ext_Rpm3_SoundSet","Vehicle_Sedan_02_Engine_Ext_Rpm4_SoundSet","Vehicle_Sedan_02_Engine_Ext_Rpm5_SoundSet","Vehicle_Sedan_02_Engine_Ext_Broken_SoundSet","Vehicle_Sedan_02_Tires_rock_slow_Ext_SoundSet","Vehicle_Sedan_02_Tires_rock_fast_Ext_SoundSet","Vehicle_Sedan_02_Tires_grass_slow_Ext_SoundSet","Vehicle_Sedan_02_Tires_grass_fast_Ext_SoundSet","Vehicle_Sedan_02_Tires_gravel_slow_Ext_SoundSet","Vehicle_Sedan_02_Tires_gravel_fast_Ext_SoundSet","Vehicle_Sedan_02_Tires_gravel_dust_fast_Ext_SoundSet","Vehicle_Sedan_02_Tires_asphalt_slow_Ext_SoundSet","Vehicle_Sedan_02_Tires_asphalt_fast_Ext_SoundSet","Vehicle_Sedan_02_Tires_water_slow_Ext_SoundSet","Vehicle_Sedan_02_Tires_water_fast_Ext_SoundSet","Vehicle_Sedan_02_skid_dirt_SoundSet","Vehicle_Sedan_02_dirt_turn_SoundSet","Vehicle_Sedan_02_Rain_Ext_SoundSet","Vehicle_Sedan_02_damper_left_SoundSet","Vehicle_Sedan_02_damper_right_SoundSet"};
			soundSetsInt[] = {"Vehicle_Sedan_02_Tires_Asphalt_Fast_General_Int_SoundSet","Vehicle_Sedan_02_Wind_SoundSet"};
		};
	};
};
