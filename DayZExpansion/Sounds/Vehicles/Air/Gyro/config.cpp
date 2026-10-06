class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Air_Gyro
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
	class Expansion_Gyro_Base_Int_SoundShader
	{
		range=1200;
	};
	class Expansion_Gyro_Base_Ext_SoundShader
	{
		range=1200;
	};
	class Expansion_Gyro_Engine_Int_SoundShader: Expansion_Gyro_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DZ\sounds\Characters\misc\Elect_Generator_Run",
				1
			}
		};
		frequency="rpm";
		volume="rpm * (1-camPos) * 0.4";
	};
	class Expansion_Gyro_Engine_Ext_SoundShader: Expansion_Gyro_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DZ\sounds\Characters\misc\Elect_Generator_Run",
				1
			}
		};
		frequency="rpm";
		volume="rpm * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos)) * 0.8";
	};
	class Expansion_Gyro_Rotor_Int_SoundShader: Expansion_Gyro_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Gyro\Gyro-rotor-ext-full",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor[0.2, 0.65]) * (1-camPos)";
	};
	class Expansion_Gyro_Rotor_Ext_SoundShader: Expansion_Gyro_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Gyro\Gyro-rotor-ext-full",
				1
			}
		};
		frequency="speed - 0.05 * (speed factor[0, 0.95]) * (speed factor[1, 0.95])";
		volume="(speed factor[0.2, 0.65]) * ((1 - 0.25*doors) max campos) * (1 - 0.2 * (1 - campos))";
	};
};
class CfgSoundSets
{
	class Expansion_Gyro_Base_Ext_SoundSet
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
	class Expansion_Gyro_Base_Int_SoundSet
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
	class Expansion_Gyro_Engine_Int_SoundSet: Expansion_Gyro_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Gyro_Engine_Int_SoundShader"
		};
		volumeFactor=0.89999998;
	};
	class Expansion_Gyro_Engine_Ext_SoundSet: Expansion_Gyro_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Gyro_Engine_Ext_SoundShader"
		};
		volumeFactor=0.89999998;
	};
	class Expansion_Gyro_Rotor_Int_SoundSet: Expansion_Gyro_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Gyro_Rotor_Int_SoundShader"
		};
		volumeFactor=0.89999998;
	};
	class Expansion_Gyro_Rotor_Ext_SoundSet: Expansion_Gyro_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Gyro_Rotor_Ext_SoundShader"
		};
		volumeFactor=0.89999998;
	};
};
