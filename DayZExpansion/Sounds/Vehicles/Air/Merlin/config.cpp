class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Air_Merlin
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
	class Expansion_Merlin_Base_Int_SoundShader
	{
		range=2000;
	};
	class Expansion_Merlin_Base_Ext_SoundShader
	{
		range=2000;
	};
	class Expansion_Merlin_Engine_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Engine_Int",
				1
			}
		};
		frequency="rpm";
		volume="rpm * (1 - camPos) * 0.4";
	};
	class Expansion_Merlin_Engine_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Engine_Ext",
				1
			}
		};
		frequency="rpm";
		volume="rpm * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
	};
	class Expansion_Merlin_Rotor_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Rotor_Int",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor[0.2, 0.65]) * (1 - camPos) * 0.3";
	};
	class Expansion_Merlin_Rotor_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\Expansion_Merlin_Rotor_Ext",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor[0.2, 0.65]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
	};
	class Expansion_Merlin_Engine_Idle_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_engine_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[1000, 8000]))";
		volume="(1-camPos)*((rpm factor [1000, 4000])*(rpm factor [9000,4000]))";
	};
	class Expansion_Merlin_Engine_Idle_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_engine_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[1000, 8000]))";
		volume="((rpm factor [1000, 4000])*(rpm factor [9000,4000])) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1500;
	};
	class Expansion_Merlin_Engine_Full_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_full_engine3int",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[5000, 13000]))";
		volume="(1-camPos)*(rpm factor [5000, 8000])";
	};
	class Expansion_Merlin_Engine_Full_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_full_engine3",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[5000, 13000]))";
		volume="(rpm factor [5000, 8000]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1500;
	};
	class Expansion_Merlin_Engine2_Idle_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_engine2_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[1000, 8000]))";
		volume="(1-camPos)*((rpm factor [1000, 4000])*(rpm factor [9000,4000]))";
	};
	class Expansion_Merlin_Engine2_Idle_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_engine2_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[1000, 8000]))";
		volume="((rpm factor [1000, 4000])*(rpm factor [9000,4000])) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1600;
	};
	class Expansion_Merlin_Engine2_Full_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_full_engine5_int",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[5000, 13000]))";
		volume="(1-camPos)*(rpm factor [5000, 8000])";
	};
	class Expansion_Merlin_Engine2_Full_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_full_engine5_ext",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[5000, 13000]))";
		volume="(rpm factor [5000, 8000]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1600;
	};
	class Expansion_Merlin_Engine_Hum_Int_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_hum",
				1
			}
		};
		frequency=1;
		volume="(1-camPos)*engineOn";
	};
	class Expansion_Merlin_Engine_Hum_Ext_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_hum",
				1
			}
		};
		frequency=1;
		volume="camPos*engineOn";
	};
	class Expansion_Merlin_Rotor_Starter_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy-rotor-int-starter3",
				1
			}
		};
		frequency="0.2 + (0.8*(speed factor[0.05, 0.15]))";
		volume="(1-camPos)*(speed factor [0.001, 0.1]) * (speed factor[0.6, 0.3])";
	};
	class Expansion_Merlin_Rotor_Idle_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy-rotor-int-idle",
				1
			}
		};
		frequency=1;
		volume="(1-camPos)*(speed factor [0.15, 0.4]) * (speed factor[0.8, 0.65])";
	};
	class Expansion_Merlin_Rotor_Full_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy-rotor-int-full",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(1-camPos)*(speed factor [0.6, 0.85])";
	};
	class Expansion_Merlin_Rotor_Starter_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy-rotor-ext-starter3",
				1
			}
		};
		frequency="0.2 + (0.8*(speed factor[0.05, 0.2]))";
		volume="(speed factor [0.001, 0.1]) * (speed factor[0.6, 0.3]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1600;
	};
	class Expansion_Merlin_Rotor_Idle_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy-rotor-ext-idle",
				1
			}
		};
		frequency=1;
		volume="(speed factor [0.15, 0.4]) * (speed factor[0.8, 0.65]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1600;
	};
	class Expansion_Merlin_Rotor_Full_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy-rotor-ext-full",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor [0.6, 0.85]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
	};
	class Expansion_Merlin_APU_Loop_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\apu-int-heavy-loop",
				1
			}
		};
		volume="(1-camPos)*(apu factor [0.4 , 1])";
		frequency=1;
	};
	class Expansion_Merlin_APU_Loop_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\apu-ext-heavy-loop",
				1
			}
		};
		volume="(apu factor [0.4 , 1]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		frequency=1;
		range=200;
	};
	class Expansion_Merlin_APU_Start_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\apu-int-heavy-start",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Merlin_APU_Start_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\apu-ext-heavy-start",
				1
			}
		};
		frequency=1;
		volume=1;
		range=350;
	};
	class Expansion_Merlin_APU_Stop_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\apu-int-heavy-stop",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Merlin_APU_Stop_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\apu-ext-heavy-stop",
				1
			}
		};
		frequency=1;
		volume=1;
		range=350;
	};
	class Expansion_Merlin_Starter_Start_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_starter_start",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Merlin_Starter_Start_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_starter_start",
				1
			}
		};
		frequency=1;
		volume=1;
		range=400;
	};
	class Expansion_Merlin_Starter_Stop_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_starter_stop",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Merlin_Starter_Stop_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_starter_stop",
				1
			}
		};
		frequency=1;
		volume=1;
		range=400;
	};
	class Expansion_Merlin_Engine_Idle_Start_Int_SoundShader: Expansion_Merlin_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_int_idle_start",
				1
			}
		};
		frequency=1;
		volume=0.30000001;
	};
	class Expansion_Merlin_Engine_Idle_Start_Ext_SoundShader: Expansion_Merlin_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\heavy_ext_idle_start",
				1
			}
		};
		frequency=1;
		volume=0.30000001;
		range=500;
	};
	class Expansion_Merlin_Warning_SoundShader: baseVehicles_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Merlin\warning",
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
	class Expansion_Merlin_Base_Ext_SoundSet
	{
		sound3DProcessingType="Vehicle_Ext_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleEngineAttenuationCurve";
		volumeFactor=1;
		occlusionFactor=0;
		obstructionFactor=0;
		spatial=1;
		loop=1;
		positionOffset[]={0,0,0};
	};
	class Expansion_Merlin_Base_Int_SoundSet
	{
		sound3DProcessingType="Vehicle_Int_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleEngineAttenuationCurve";
		volumeFactor=1;
		occlusionFactor=0;
		obstructionFactor=0;
		spatial=1;
		loop=1;
		positionOffset[]={0,0,0};
	};
	class Expansion_Merlin_Engine_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Engine_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Rotor_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Rotor_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Engine_Idle_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Idle_Int_SoundShader"
		};
		volumeFactor=0.40000001;
	};
	class Expansion_Merlin_Engine_Idle_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Idle_Ext_SoundShader"
		};
		volumeFactor=0.80000001;
	};
	class Expansion_Merlin_Engine_Full_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Full_Int_SoundShader"
		};
		volumeFactor=0.40000001;
	};
	class Expansion_Merlin_Engine_Full_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Full_Ext_SoundShader"
		};
		volumeFactor=0.80000001;
	};
	class Expansion_Merlin_Engine2_Idle_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine2_Idle_Int_SoundShader"
		};
		volumeFactor=0.126491;
	};
	class Expansion_Merlin_Engine2_Idle_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine2_Idle_Ext_SoundShader"
		};
		volumeFactor=0.079999998;
	};
	class Expansion_Merlin_Engine2_Full_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine2_Full_Int_SoundShader"
		};
		volumeFactor=0.126491;
	};
	class Expansion_Merlin_Engine2_Full_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Engine2_Full_Ext_SoundShader"
		};
		volumeFactor=0.079999998;
	};
	class Expansion_Merlin_Engine_Hum_Int_SoundSet: baseCharacter_SoundSet
	{
		loop=1;
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Hum_Int_SoundShader"
		};
		volumeFactor=0.2;
	};
	class Expansion_Merlin_Engine_Hum_Ext_SoundSet: baseCharacter_SoundSet
	{
		loop=1;
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Hum_Ext_SoundShader"
		};
		volumeFactor=0.2;
	};
	class Expansion_Merlin_Rotor_Starter_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Starter_Int_SoundShader"
		};
		volumeFactor=0.25;
	};
	class Expansion_Merlin_Rotor_Idle_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Idle_Int_SoundShader"
		};
		volumeFactor=0.30000001;
	};
	class Expansion_Merlin_Rotor_Full_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Full_Int_SoundShader"
		};
		volumeFactor=0.30000001;
	};
	class Expansion_Merlin_Rotor_Starter_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Starter_Ext_SoundShader"
		};
		volumeFactor=0.60000002;
	};
	class Expansion_Merlin_Rotor_Idle_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Idle_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Rotor_Full_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_Rotor_Full_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_APU_Loop_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_APU_Loop_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_APU_Loop_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Merlin_APU_Loop_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_APU_Start_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_APU_Start_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_APU_Start_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_APU_Start_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_APU_Stop_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_APU_Stop_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_APU_Stop_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_APU_Stop_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Starter_Start_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_Starter_Start_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Starter_Start_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_Starter_Start_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Starter_Stop_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_Starter_Stop_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Starter_Stop_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_Starter_Stop_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Engine_Idle_Start_Int_SoundSet: Expansion_Merlin_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Idle_Start_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Engine_Idle_Start_Ext_SoundSet: Expansion_Merlin_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_Engine_Idle_Start_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Merlin_Warning_SoundSet: baseVehicles_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Merlin_Warning_SoundShader"
		};
		volumeFactor=1;
	};
};
