class CfgPatches
{
	class DayZExpansion_Sounds_Vehicles_Water_Zodiac
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
	class Expansion_Zodiac_Base_Int_SoundShader
	{
		range=20;
	};
	class Expansion_Zodiac_Base_Ext_SoundShader
	{
		range=500;
	};
	class Expansion_Zodiac_Engine_Int_SoundShader: Expansion_Zodiac_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Water\Zodiac\Expansion_Zodiac_Engine_Ext",
				1
			}
		};
		frequency="0.5 + (engineOn * (rpm / 16))";
		volume="(engineOn * 0.4) + (engineOn * (rpm / 4))";
	};
	class Expansion_Zodiac_Idle_Int_SoundShader: Expansion_Zodiac_Base_Ext_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Vehicles\Water\Zodiac\Expansion_Zodiac_Idle_Ext",
				1
			}
		};
		volume="(engineOn * 0.4) + (engineOn * (rpm / 4))";
	};
};
class CfgSoundSets
{
	class Expansion_Zodiac_Base_Ext_SoundSet
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
	class Expansion_Zodiac_Base_Int_SoundSet
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
	class Expansion_Zodiac_Engine_Ext_SoundSet: Expansion_Zodiac_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Zodiac_Engine_Int_SoundShader"
		};
		volumeFactor=0.12;
	};
	class Expansion_Zodiac_Idle_Ext_SoundSet: Expansion_Zodiac_Base_Ext_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Zodiac_Idle_Int_SoundShader"
		};
		volumeFactor=0.12;
	};
};
