class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Air_Mh6
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
	class Expansion_Mh6_Base_Int_SoundShader
	{
		range=2000;
	};
	class Expansion_Mh6_Base_Ext_SoundShader
	{
		range=2000;
	};
	class Expansion_Mh6_Engine_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_engine-full",
				1
			}
		};
		frequency="rpm";
		volume="rpm * (1 - camPos ) * 0.4";
	};
	class Expansion_Mh6_Engine_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_engine-full",
				1
			}
		};
		frequency="rpm";
		volume="rpm * camPos";
	};
	class Expansion_Mh6_Rotor_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_rotor-full",
				1
			}
		};
		frequency="rpm * speed";
		volume="rpm * speed * (1 - camPos) * 0.3";
	};
	class Expansion_Mh6_Rotor_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_rotor-full",
				1
			}
		};
		frequency="rpm * speed";
		volume="rpm * speed * camPos";
	};
	class Expansion_Mh6_Starter_Start_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_starter-start-noclick",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Mh6_Starter_Start_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_starter-start-noclick",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Mh6_Starter_Stop_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_starter-stop",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Mh6_Starter_Stop_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_starter-stop",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Mh6_Starter_Click_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		range=50;
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\starter_click2_int",
				1
			}
		};
		frequency=1;
		volume=0.5;
	};
	class Expansion_Mh6_Starter_Click_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		range=50;
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\starter_click2",
				1
			}
		};
		frequency=1;
		volume=0.5;
	};
	class Expansion_Mh6_Engine_Idle_Start_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_engine-idle-start",
				1
			}
		};
		frequency=1;
		volume=0.30000001;
	};
	class Expansion_Mh6_Engine_Idle_Start_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_engine-idle-start",
				1
			}
		};
		frequency=1;
		volume=0.30000001;
	};
	class Expansion_Mh6_Starter_Loop_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_starter-loop-noclick",
				1
			}
		};
		frequency="rpm * speed";
		volume="rpm * speed * (1 - camPos ) * 0.4";
	};
	class Expansion_Mh6_Starter_Loop_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_starter-loop-noclick",
				1
			}
		};
		frequency="rpm * speed";
		volume="rpm * speed * camPos";
	};
};
class CfgSoundSets
{
	class Expansion_Mh6_Base_Ext_SoundSet
	{
		sound3DProcessingType="Vehicle_Ext_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleEngineAttenuationCurve";
		volumeFactor=1;
		occlusionFactor=0;
		obstructionFactor=0;
		spatial=1;
		loop=1;
		positionOffset[]={0,0,1};
	};
	class Expansion_Mh6_Base_Int_SoundSet
	{
		sound3DProcessingType="Vehicle_Int_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleEngineAttenuationCurve";
		volumeFactor=1;
		occlusionFactor=0;
		obstructionFactor=0;
		spatial=1;
		loop=1;
		positionOffset[]={0,0,1};
	};
	class Expansion_Mh6_Engine_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Engine_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Rotor_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Rotor_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Start_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Start_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Start_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Start_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Stop_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Stop_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Stop_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Stop_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Click_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Click_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Click_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Click_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Engine_Idle_Start_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Idle_Start_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Engine_Idle_Start_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Idle_Start_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Loop_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Loop_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Starter_Loop_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Starter_Loop_Ext_SoundShader"
		};
		volumeFactor=1;
	};
};
