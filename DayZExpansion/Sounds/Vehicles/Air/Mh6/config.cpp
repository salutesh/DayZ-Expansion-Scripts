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
	class baseCharacter_SoundShader;
	class baseVehicles_SoundShader;
	class Expansion_Mh6_Base_Int_SoundShader
	{
		range=1500;
	};
	class Expansion_Mh6_Base_Ext_SoundShader
	{
		range=1500;
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
		volume="rpm * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
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
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor[0.2, 0.65]) * (1 - camPos) * 0.3";
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
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor[0.2, 0.65]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
	};
	class Expansion_Mh6_Engine_Idle_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_engine-idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[1000, 2500]))";
		volume="(1-camPos)*((rpm factor [900, 1300])*(rpm factor [3600,2500]))";
	};
	class Expansion_Mh6_Engine_Idle_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_engine-idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[1000, 3000]))";
		volume="((rpm factor [900, 1300])*(rpm factor [3600,2500])) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1500;
	};
	class Expansion_Mh6_Engine_Full_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_engine-full",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[2000, 4000]))";
		volume="(1-camPos)*(rpm factor [2000, 4000])";
	};
	class Expansion_Mh6_Engine_Full_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_engine-full",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[2000, 4000]))";
		volume="(rpm factor [2000, 4000]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1500;
	};
	class Expansion_Mh6_Engine_Hum_Int_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\heli-light_hum",
				1
			}
		};
		frequency=1;
		volume="(1-camPos)*engineOn";
	};
	class Expansion_Mh6_Engine_Hum_Ext_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\heli-light_hum",
				1
			}
		};
		frequency=1;
		volume="camPos*engineOn";
	};
	class Expansion_Mh6_Rotor_Starter_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_rotor-starter",
				1
			}
		};
		frequency="0.5 + (0.5*(speed factor[0.05, 0.15]))";
		volume="(1-camPos)*(speed factor [0.001, 0.2]) * (speed factor[0.6, 0.3])";
	};
	class Expansion_Mh6_Rotor_Idle_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_rotor-idle",
				1
			}
		};
		frequency=1;
		volume="(1-camPos)*(speed factor [0.15, 0.5]) * (speed factor[0.8, 0.65])";
	};
	class Expansion_Mh6_Rotor_Full_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_int_rotor-full",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(1-camPos)*(speed factor [0.6, 0.85]) * 0.9";
	};
	class Expansion_Mh6_Rotor_Starter_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_rotor-starter",
				1
			}
		};
		frequency="0.5 + (0.5*(speed factor[0.05, 0.2]))";
		volume="(speed factor [0.001, 0.2]) * (speed factor[0.6, 0.3]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=800;
	};
	class Expansion_Mh6_Rotor_Idle_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_rotor-idle",
				1
			}
		};
		frequency=1;
		volume="(speed factor [0.15, 0.5]) * (speed factor[0.8, 0.65]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1200;
	};
	class Expansion_Mh6_Rotor_Full_Ext_SoundShader: Expansion_Mh6_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\new-heli-light_ext_rotor-full",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor [0.6, 0.85]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
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
		range=300;
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
		range=300;
	};
	class Expansion_Mh6_Starter_Click_Int_SoundShader: Expansion_Mh6_Base_Int_SoundShader
	{
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
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\starter_click2",
				1
			}
		};
		frequency=1;
		volume=0.5;
		range=100;
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
		range=800;
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
		frequency="speed * speed";
		volume="speed * speed * (1 - camPos ) * 0.4";
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
		frequency="speed * speed";
		volume="speed * speed * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=600;
	};
	class Expansion_Mh6_Warning_SoundShader: baseVehicles_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\warning",
				1
			}
		};
		frequency=1;
		volume=0.89999998;
		range=35;
	};
};
class CfgSoundSets
{
	class baseCharacter_SoundSet;
	class baseVehicles_SoundSet;
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
	class Expansion_Mh6_Engine_Idle_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Idle_Int_SoundShader"
		};
		volumeFactor=0.60000002;
	};
	class Expansion_Mh6_Engine_Idle_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Idle_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Engine_Full_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Full_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Engine_Full_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Full_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Engine_Hum_Int_SoundSet: baseCharacter_SoundSet
	{
		loop=1;
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Hum_Int_SoundShader"
		};
		volumeFactor=0.60000002;
	};
	class Expansion_Mh6_Engine_Hum_Ext_SoundSet: baseCharacter_SoundSet
	{
		loop=1;
		soundShaders[]=
		{
			"Expansion_Mh6_Engine_Hum_Ext_SoundShader"
		};
		volumeFactor=0.60000002;
	};
	class Expansion_Mh6_Rotor_Starter_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Starter_Int_SoundShader"
		};
		volumeFactor=0.25;
	};
	class Expansion_Mh6_Rotor_Idle_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Idle_Int_SoundShader"
		};
		volumeFactor=0.30000001;
	};
	class Expansion_Mh6_Rotor_Full_Int_SoundSet: Expansion_Mh6_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Full_Int_SoundShader"
		};
		volumeFactor=0.30000001;
	};
	class Expansion_Mh6_Rotor_Starter_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Starter_Ext_SoundShader"
		};
		volumeFactor=0.60000002;
	};
	class Expansion_Mh6_Rotor_Idle_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Idle_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Mh6_Rotor_Full_Ext_SoundSet: Expansion_Mh6_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Mh6_Rotor_Full_Ext_SoundShader"
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
	class Expansion_Mh6_Warning_SoundSet: baseVehicles_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Mh6_Warning_SoundShader"
		};
		volumeFactor=1;
	};
};
