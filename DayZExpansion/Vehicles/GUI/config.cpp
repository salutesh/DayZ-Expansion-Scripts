class CfgPatches
{
	class DayZExpansion_Vehicles_GUI
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={};
	};
};
class CfgSoundShaders
{
	class Expansion_SF_Base_SoundShader
	{
		range=1500;
	};
	class Expansion_SF_Comms_Falco_SoundShader: Expansion_SF_Base_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Vehicles\GUI\sounds\sf_comms_falco",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_SF_Comms_Fox_SoundShader: Expansion_SF_Base_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Vehicles\GUI\sounds\sf_comms_fox",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_SF_Comms_Peppy_SoundShader: Expansion_SF_Base_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Vehicles\GUI\sounds\sf_comms_peppy",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_SF_Comms_Slippy_SoundShader: Expansion_SF_Base_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Vehicles\GUI\sounds\sf_comms_slippy",
				1
			}
		};
		frequency=1;
		volume=1;
	};
	class Expansion_SF_StageClear_Short_SoundShader: Expansion_SF_Base_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Vehicles\GUI\sounds\sf_stageclear_short",
				1
			}
		};
		frequency=1;
		volume=1;
	};
};
class CfgSoundSets
{
	class Expansion_SF_Base_SoundSet
	{
		sound3DProcessingType="Vehicle_Ext_3DProcessingType";
		distanceFilter="softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve="vehicleLoudAttenuationCurve";
		spatial=0;
		doppler=0;
		loop=0;
	};
	class Expansion_SF_Comms_Falco_SoundSet: Expansion_SF_Base_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_SF_Comms_Falco_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_SF_Comms_Fox_SoundSet: Expansion_SF_Base_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_SF_Comms_Fox_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_SF_Comms_Peppy_SoundSet: Expansion_SF_Base_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_SF_Comms_Peppy_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_SF_Comms_Slippy_SoundSet: Expansion_SF_Base_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_SF_Comms_Slippy_SoundShader"
		};
		volumeFactor=1;
	};
	class Expansion_SF_StageClear_Short_SoundSet: Expansion_SF_Base_SoundSet
	{
		loop=0;
		soundShaders[]=
		{
			"Expansion_SF_StageClear_Short_SoundShader"
		};
		volumeFactor=1;
	};
};
