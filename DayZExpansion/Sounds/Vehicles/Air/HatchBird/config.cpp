class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Air_HatchBird
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
	class Expansion_HatchBird_Base_Int_SoundShader
	{
		range=2000;
	};
	class Expansion_HatchBird_Base_Ext_SoundShader
	{
		range=2000;
	};
	class Expansion_HatchBird_Engine_Int_SoundShader: Expansion_HatchBird_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\HatchBird\HatchBird_Engine",
				1
			}
		};
		frequency="rpm";
		volume="rpm * (1 - camPos ) * 0.4";
	};
	class Expansion_HatchBird_Engine_Ext_SoundShader: Expansion_HatchBird_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\HatchBird\HatchBird_Engine",
				1
			}
		};
		frequency="rpm";
		volume="rpm * camPos";
	};
	class Expansion_HatchBird_Rotor_Int_SoundShader: Expansion_HatchBird_Base_Int_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\Expansion_Mh6_Rotor_Int",
				1
			}
		};
		frequency="rpm * speed";
		volume="rpm * speed * (1 - camPos) * 0.3";
	};
	class Expansion_HatchBird_Rotor_Ext_SoundShader: Expansion_HatchBird_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Air\Mh6\Expansion_Mh6_Rotor_Ext",
				1
			}
		};
		frequency="rpm * speed";
		volume="rpm * speed * camPos";
	};
};
class CfgSoundSets
{
	class Expansion_HatchBird_Base_Ext_SoundSet
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
	class Expansion_HatchBird_Base_Int_SoundSet
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
	class Expansion_HatchBird_Engine_Int_SoundSet: Expansion_HatchBird_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_HatchBird_Engine_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_HatchBird_Engine_Ext_SoundSet: Expansion_HatchBird_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_HatchBird_Engine_Ext_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_HatchBird_Rotor_Int_SoundSet: Expansion_HatchBird_Base_Int_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_HatchBird_Rotor_Int_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_HatchBird_Rotor_Ext_SoundSet: Expansion_HatchBird_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_HatchBird_Rotor_Ext_SoundShader"
		};
		volumeFactor=1;
	};
};
