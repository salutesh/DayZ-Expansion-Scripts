class CfgPatches
{
	class DayZExpansion_Sounds_Common
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
	class Expansion_Horn_SoundShader
	{
		range=150;
	};
	class Expansion_Horn_Ext_SoundShader: Expansion_Horn_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Ext",
				1
			}
		};
		volume=1;
	};
	class Expansion_Horn_Int_SoundShader: Expansion_Horn_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Int",
				1
			}
		};
		volume=1;
	};
	class Expansion_Truck_Horn_Ext_SoundShader: Expansion_Horn_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Ext",
				1
			}
		};
		volume=1;
	};
	class Expansion_Truck_Horn_Int_SoundShader: Expansion_Horn_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Common\Expansion_Truck_Horn_Int",
				1
			}
		};
		volume=1;
	};
	class Expansion_CarLock_SoundShader
	{
		range=20;
	};
	class Expansion_Car_Lock_SoundShader: Expansion_CarLock_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Common\Expansion_CarLock",
				1
			}
		};
		volume=1;
	};
	class Expansion_Wind_SoundShader
	{
		range=200;
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Common\wind_ext",
				1
			}
		};
		volume=1;
	};
	class Expansion_Alarm_SoundShader
	{
		range=200;
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Common\alarm_loop1",
				1
			}
		};
		frequency=1;
		volume=0.5;
	};
};
class CfgSoundSets
{
	class Expansion_BaseVehicles_SoundSet
	{
		sound3DProcessingType="Vehicle_Ext_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleEngineAttenuationCurve";
		volumeFactor=1;
		spatial=1;
	};
	class Expansion_Horn_SoundSet: Expansion_BaseVehicles_SoundSet
	{
		loop=0;
	};
	class Expansion_Horn_Ext_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Horn_Ext_SoundShader"
		};
	};
	class Expansion_Horn_Int_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Horn_Int_SoundShader"
		};
	};
	class Expansion_Truck_Horn_Ext_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Truck_Horn_Ext_SoundShader"
		};
	};
	class Expansion_Truck_Horn_Int_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Truck_Horn_Int_SoundShader"
		};
	};
	class Expansion_Car_Lock_SoundSet: Expansion_Horn_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Car_Lock_SoundShader"
		};
	};
	class Expansion_Wind_SoundSet: Expansion_BaseVehicles_SoundSet
	{
		loop=1;
		soundShaders[]=
		{
			"Expansion_Wind_SoundShader"
		};
	};
	class Expansion_Alarm_SoundSet: Expansion_BaseVehicles_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Alarm_SoundShader"
		};
	};
};
