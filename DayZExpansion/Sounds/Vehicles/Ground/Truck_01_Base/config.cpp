class CfgPatches
{
	class DayZExpansion_Sounds_Ground_Truck_01_Base
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Sounds_Effects"
		};
	};
};
class CfgSoundShaders
{
};
class CfgSoundSets
{
};
class CfgVehicles
{
	class ExpansionVehicleCarBase;
	class Vehicle_Truck_01_Base: ExpansionVehicleCarBase
	{
		class ExpansionSounds
		{
			thrust=0.60000002;
			thrustTurbo=1;
			thrustGentle=0.30000001;
			thrustSmoothCoef=0.02;
			camposSmoothCoef=0.029999999;
			soundSetsFilter[]=
			{
				"Truck_01_Engine_Offload_Ext_Rpm1_SoundSet",
				"Truck_01_Engine_Offload_Ext_Rpm2_SoundSet",
				"Truck_01_Engine_Offload_Ext_Rpm3_SoundSet",
				"Truck_01_Engine_Offload_Ext_Rpm4_SoundSet",
				"Truck_01_Engine_Ext_Rpm0_SoundSet",
				"Truck_01_Engine_Ext_Rpm1_SoundSet",
				"Truck_01_Engine_Ext_Rpm2_SoundSet",
				"Truck_01_Engine_Ext_Rpm3_SoundSet",
				"Truck_01_Engine_Ext_Rpm4_SoundSet",
				"Truck_01_Engine_Ext_Broken_SoundSet",
				"Truck_01_Tires_rock_slow_Ext_SoundSet",
				"Truck_01_Tires_rock_fast_Ext_SoundSet",
				"Truck_01_Tires_ice_slow_Ext_SoundSet",
				"Truck_01_Tires_ice_fast_Ext_SoundSet",
				"Truck_01_Tires_snow_slow_Ext_SoundSet",
				"Truck_01_Tires_snow_fast_Ext_SoundSet",
				"Truck_01_Tires_grass_slow_Ext_SoundSet",
				"Truck_01_Tires_grass_fast_Ext_SoundSet",
				"Truck_01_Tires_gravel_slow_Ext_SoundSet",
				"Truck_01_Tires_gravel_fast_Ext_SoundSet",
				"Truck_01_Tires_asphalt_slow_Ext_SoundSet",
				"Truck_01_Tires_asphalt_fast_Ext_SoundSet",
				"Truck_01_Tires_water_slow_Ext_SoundSet",
				"Truck_01_Tires_water_fast_Ext_SoundSet",
				"offroad_skid_dirt_SoundSet",
				"offroad_dirt_turn_SoundSet",
				"offroad_damper_left_SoundSet",
				"offroad_damper_right_SoundSet",
				"Truck_01_Rain_Ext_SoundSet"
			};
			soundSetsInt[]=
			{
				"Truck_01_Tires_Asphalt_Slow_Ext_SoundSet",
				"Truck_01_Wind_SoundSet",
				"Truck_01_Rattle_Int_SoundSet"
			};
		};
	};
};
