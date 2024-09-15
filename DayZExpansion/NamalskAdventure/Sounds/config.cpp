class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Sounds
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Sounds_Effects"
		};
	};
};
class CfgSoundCurves
{
	class SatelliteDistantAttenuationCurve
	{
		points[]=
		{
			{0,1},
			{2000,1},
			{3500,0.5},
			{4000,0}
		};
	};
};
class CfgSoundShaders
{
	class Expansion_Anomaly_SoundShader
	{
		volume=1;
		frequency=1;
		limitation=0;
		radius=50;
		range=50;
	};
	class Expansion_Satillite_SoundShader
	{
		volume=1;
		frequency=1;
		limitation=0;
		radius=150;
		range=150;
	};
	class Expansion_Geiger_SoundShader
	{
		volume=1;
		frequency=2;
		limitation=0;
		radius=5;
		range=5;
	};
	class Expansion_AnomalyRumble_SoundShader: Expansion_Anomaly_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\deep_rumble",
				1
			}
		};
	};
	class Expansion_AnomalyWindBlowActivated_SoundShader: Expansion_Anomaly_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\wind_blow_short",
				1
			}
		};
	};
	class Expansion_AnomalyArea1_SoundShader: Expansion_Anomaly_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\anomaly_area_1",
				1
			}
		};
	};
	class Expansion_SatelliteEngine_Boot_SoundShader: Expansion_Satillite_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\electric_engine",
				1
			}
		};
		range=200;
	};
	class Expansion_SatelliteEngine_Active_SoundShader: Expansion_Satillite_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\engine_loop",
				1
			}
		};
		range=200;
	};
	class Expansion_Satellite_Cry_Distance_SoundShader: Expansion_Satillite_SoundShader
	{
		samples[]=
		{
			
			{
				"nst\ns_dayz\effects\sounds\data\phoenix_foghorn_distance",
				1
			}
		};
		range=4000;
	};
	class Expansion_Geiger1_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_1",
				1
			}
		};
	};
	class Expansion_Geiger2_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_2",
				1
			}
		};
	};
	class Expansion_Geiger3_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_3",
				1
			}
		};
	};
	class Expansion_Geiger4_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_4",
				1
			}
		};
	};
	class Expansion_Geiger5_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_5",
				1
			}
		};
	};
	class Expansion_Geiger6_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_6",
				1
			}
		};
	};
	class Expansion_Geiger7_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_7",
				1
			}
		};
	};
	class Expansion_Geiger8_SoundShader: Expansion_Geiger_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\NamalskAdventure\Sounds\data\geiger_8",
				1
			}
		};
	};
};
class CfgSoundSets
{
	class Expansion_AnomalyIdle_Soundset
	{
		sound3DProcessingType="character3DProcessingType";
		volumeCurve="characterAttenuationCurve";
		spatial=1;
		doppler=0;
		loop=1;
		distanceFilter="defaultDistanceFilter";
	};
	class Expansion_AnomalyActivated_Soundset
	{
		sound3DProcessingType="character3DProcessingType";
		volumeCurve="characterAttenuationCurve";
		spatial=1;
		doppler=0;
		loop=0;
		distanceFilter="defaultDistanceFilter";
	};
	class Expansion_Satellite_Soundset
	{
		sound3DProcessingType="character3DProcessingType";
		volumeCurve="characterAttenuationCurve";
		spatial=1;
		doppler=0;
		loop=0;
		distanceFilter="defaultDistanceFilter";
	};
	class Expansion_Geiger_Soundset
	{
		sound3DProcessingType="character3DProcessingType";
		volumeCurve="characterAttenuationCurve";
		spatial=1;
		doppler=0;
		loop=1;
		distanceFilter="defaultDistanceFilter";
	};
	class Expansion_AnomalyRumble_Soundset: Expansion_AnomalyIdle_Soundset
	{
		soundShaders[]=
		{
			"Expansion_AnomalyRumble_SoundShader"
		};
	};
	class Expansion_AnomalyWindBlowActivated_Soundset: Expansion_AnomalyActivated_Soundset
	{
		soundShaders[]=
		{
			"Expansion_AnomalyWindBlowActivated_SoundShader"
		};
	};
	class Expansion_AnomalyArea1_Soundset: Expansion_AnomalyIdle_Soundset
	{
		soundShaders[]=
		{
			"Expansion_AnomalyArea1_SoundShader"
		};
		volumeFactor="0.15 * 0.4 * 1.6";
		spatial=0;
	};
	class Expansion_Satellite_Boot_Soundset: Expansion_Satellite_Soundset
	{
		soundShaders[]=
		{
			"Expansion_SatelliteEngine_Boot_SoundShader"
		};
	};
	class Expansion_Satellite_Active_Soundset: Expansion_Satellite_Soundset
	{
		soundShaders[]=
		{
			"Expansion_SatelliteEngine_Active_SoundShader"
		};
	};
	class Expansion_Satellite_Cry_Distance_SoundSet: Expansion_Satellite_Soundset
	{
		soundShaders[]=
		{
			"Phoenix_FogHorn_Distance_SoundShader"
		};
		volumeCurve="SatelliteDistantAttenuationCurve";
		volumeFactor=0.34999999;
	};
	class Expansion_Geiger1_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger1_SoundShader"
		};
	};
	class Expansion_Geiger2_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger2_SoundShader"
		};
	};
	class Expansion_Geiger3_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger3_SoundShader"
		};
	};
	class Expansion_Geiger4_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger4_SoundShader"
		};
	};
	class Expansion_Geiger5_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger5_SoundShader"
		};
	};
	class Expansion_Geiger6_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger7_SoundShader"
		};
	};
	class Expansion_Geiger7_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger7_SoundShader"
		};
	};
	class Expansion_Geiger8_Soundset: Expansion_Geiger_Soundset
	{
		soundShaders[]=
		{
			"Expansion_Geiger8_SoundShader"
		};
	};
};
