class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Air_Bell412
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
	class Expansion_Bell412_Base_Int_SoundShader
	{
		range=2000;
	};
	class Expansion_Bell412_Base_Ext_SoundShader
	{
		range=2000;
	};
	class Expansion_Bell412_Engine_Idle_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_int_engine_1a_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[800, 3000]))";
		volume="(1-camPos)*((rpm factor [800, 2000])*(rpm factor [3500,1800]))";
	};
	class Expansion_Bell412_Engine_Idle_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_ext_engine_1a_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[800, 3000]))";
		volume="((rpm factor [800, 2000])*(rpm factor [3500,1800])) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1500;
	};
	class Expansion_Bell412_Engine_Full_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_int_engine_1a_full",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[2800, 5000]))";
		volume="(1-camPos)*(rpm factor [2800, 4600])";
	};
	class Expansion_Bell412_Engine_Full_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_ext_engine_1a_full",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[2800, 5000]))";
		volume="(rpm factor [2800, 4600]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1500;
	};
	class Expansion_Bell412_Engine2_Idle_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_int_engine2_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[800, 3000]))";
		volume="(1-camPos)*((rpm factor [800, 2000])*(rpm factor [3500,1800]))";
	};
	class Expansion_Bell412_Engine2_Idle_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_ext_engine2_idle",
				1
			}
		};
		frequency="(0.8)+(0.2*(rpm factor[800, 3000]))";
		volume="((rpm factor [800, 2000])*(rpm factor [3500,1800])) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1100;
	};
	class Expansion_Bell412_Engine2_Full_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_int_engine2_full",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[2800, 5000]))";
		volume="(1-camPos)*(rpm factor [2800, 4600])";
	};
	class Expansion_Bell412_Engine2_Full_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_ext_engine2_full",
				1
			}
		};
		frequency="0.3+(0.5*speed)+(0.2*(rpm factor[2800, 5000]))";
		volume="(rpm factor [2800, 4600]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1100;
	};
	class Expansion_Bell412_Engine_Hum_Int_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_hum",
				1
			}
		};
		frequency=1;
		volume="(1-camPos)*engineOn";
	};
	class Expansion_Bell412_Engine_Hum_Ext_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_hum",
				1
			}
		};
		frequency=1;
		volume="camPos*engineOn";
	};
	class Expansion_Bell412_Rotor_Starter_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium-rotor-int-starter3",
				1
			}
		};
		frequency="0.4 + (0.4*(speed factor[0.05, 0.15]))";
		volume="(1-camPos)*(speed factor [0.001, 0.3]) * (speed factor[0.6, 0.3])";
	};
	class Expansion_Bell412_Rotor_Idle_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium-rotor-int-idle",
				1
			}
		};
		frequency=1;
		volume="(1-camPos)*(speed factor [0.15, 0.5]) * (speed factor[0.8, 0.65])";
	};
	class Expansion_Bell412_Rotor_Full_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium-rotor-int-full2",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(1-camPos)*(speed factor [0.6, 0.85])";
	};
	class Expansion_Bell412_Rotor_Starter_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium-rotor-ext-starter3",
				1
			}
		};
		frequency="0.4 + (0.4*(speed factor[0.05, 0.2]))";
		volume="(speed factor [0.001, 0.3]) * (speed factor[0.6, 0.3]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1200;
	};
	class Expansion_Bell412_Rotor_Idle_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium-rotor-ext-idle",
				1
			}
		};
		frequency=1;
		volume="(speed factor [0.15, 0.5]) * (speed factor[0.8, 0.65]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
		range=1600;
	};
	class Expansion_Bell412_Rotor_Full_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium-rotor-ext-full2",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor [0.6, 0.85]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
	};
	class Expansion_Bell412_SlidingDoor_Open_SoundShader: baseVehicles_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSlideBig\doorWoodSlideBigOpen_2",
				1
			}
		};
		volume=0.89999998;
	};
	class Expansion_Bell412_SlidingDoor_Close_SoundShader: baseVehicles_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSlide\doorWoodSlideClose_4",
				1
			}
		};
		volume=0.89999998;
	};
	class Expansion_Bell412_Starter_Start_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_int_starter_start",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Bell412_Starter_Start_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_ext_starter_start",
				1
			}
		};
		frequency=1;
		volume=1;
		range=200;
	};
	class Expansion_Bell412_Starter_Stop_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_int_starter_stop",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_Bell412_Starter_Stop_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_ext_starter_stop",
				1
			}
		};
		frequency=1;
		volume=1;
		range=200;
	};
	class Expansion_Bell412_Engine_Idle_Start_Int_SoundShader: Expansion_Bell412_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_int_idle_start",
				1
			}
		};
		frequency=1;
		volume=0.30000001;
	};
	class Expansion_Bell412_Engine_Idle_Start_Ext_SoundShader: Expansion_Bell412_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\medium_ext_idle_start",
				1
			}
		};
		frequency=1;
		volume=0.30000001;
		range=600;
	};
	class Expansion_Bell412_Warning_SoundShader: baseVehicles_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Bell412\warning",
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
	class Expansion_Bell412_Base_Ext_SoundSet
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
	class Expansion_Bell412_Base_Int_SoundSet
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
	class Expansion_Bell412_Engine_Idle_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Idle_Int_SoundShader"
		};
		volumeFactor=0.22493701;
	};
	class Expansion_Bell412_Engine_Idle_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Idle_Ext_SoundShader"
		};
		volumeFactor=0.56234097;
	};
	class Expansion_Bell412_Engine_Full_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Full_Int_SoundShader"
		};
		volumeFactor=0.40000001;
	};
	class Expansion_Bell412_Engine_Full_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Full_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Engine2_Idle_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine2_Idle_Int_SoundShader"
		};
		volumeFactor=0.22493701;
	};
	class Expansion_Bell412_Engine2_Idle_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine2_Idle_Ext_SoundShader"
		};
		volumeFactor=0.56234097;
	};
	class Expansion_Bell412_Engine2_Full_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine2_Full_Int_SoundShader"
		};
		volumeFactor=0.40000001;
	};
	class Expansion_Bell412_Engine2_Full_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Engine2_Full_Ext_SoundShader"
		};
		volumeFactor=0.56234097;
	};
	class Expansion_Bell412_Engine_Hum_Int_SoundSet: baseCharacter_SoundSet
	{
		loop=1;
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Hum_Int_SoundShader"
		};
		volumeFactor=0.2;
	};
	class Expansion_Bell412_Engine_Hum_Ext_SoundSet: baseCharacter_SoundSet
	{
		loop=1;
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Hum_Ext_SoundShader"
		};
		volumeFactor=0.2;
	};
	class Expansion_Bell412_Rotor_Starter_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Rotor_Starter_Int_SoundShader"
		};
		volumeFactor=0.25;
	};
	class Expansion_Bell412_Rotor_Idle_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Rotor_Idle_Int_SoundShader"
		};
		volumeFactor=0.30000001;
	};
	class Expansion_Bell412_Rotor_Full_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Rotor_Full_Int_SoundShader"
		};
		volumeFactor=0.30000001;
	};
	class Expansion_Bell412_Rotor_Starter_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Rotor_Starter_Ext_SoundShader"
		};
		volumeFactor=0.60000002;
	};
	class Expansion_Bell412_Rotor_Idle_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Rotor_Idle_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Rotor_Full_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_Rotor_Full_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_SlidingDoor_Open_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_SlidingDoor_Open_SoundShader"
		};
	};
	class Expansion_Bell412_SlidingDoor_Close_SoundSet: baseVehicles_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Bell412_SlidingDoor_Close_SoundShader"
		};
	};
	class Expansion_Bell412_Starter_Start_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Bell412_Starter_Start_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Starter_Start_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Bell412_Starter_Start_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Starter_Stop_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Bell412_Starter_Stop_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Starter_Stop_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Bell412_Starter_Stop_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Engine_Idle_Start_Int_SoundSet: Expansion_Bell412_Base_Int_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Idle_Start_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Engine_Idle_Start_Ext_SoundSet: Expansion_Bell412_Base_Ext_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Bell412_Engine_Idle_Start_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_Bell412_Warning_SoundSet: baseVehicles_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_Bell412_Warning_SoundShader"
		};
		volumeFactor=1;
	};
};
