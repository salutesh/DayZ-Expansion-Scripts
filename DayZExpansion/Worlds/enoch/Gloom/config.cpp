/**
 * config.cpp
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Config
 * @brief		This file handle expansion core configs
 **/
class CfgPatches
{
	class DayZExpansion_Worlds_enoch_Gloom
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]= 
		{ 
			"DZ_Data"
		};
	};
};

class CfgWorlds
{
    class enoch;

    class Weather;

    class Overcast;
	class Weather1;
	class Weather2;
	class Weather3;
	class Weather4;
	class Weather5;

    class LightingNew;

    class C_Night;
    class C_Astro1;
    class C_Astro2;
    class C_Astro3;
    class C_Nautical1;
    class C_Nautical2;
    class C_Nautical3;
    class C_Civil1;
    class C_Civil2;
    class C_Civil3;
    class C_Day0;
    class C_Day4;
    class C_Day8;
    class C_Day12;
    class C_Day16;
    class C_Day35;
    class C_Day60;
    class O_Night;
    class O_Astro1;
    class O_Nautical1;
    class O_Civil1;
    class O_Civil2;
    class O_Day0;
    class O_Day4;
    class O_Day8;
    class O_Day12;
    class O_Day35;
    class O_Day60;

    class VolFog;

    class SoundMapValues;
    class Subdivision;
    class Fractal;
    class WhiteNoise; 

    class DefaultLighting;

	class enochGloom: enoch
	{
		class Weather: Weather
		{
			class LightingNew: LightingNew
			{
				class C_Night: C_Night
				{
					ambient[]={0.016000001,0.031599998,0.048};
					filmGrainIntensity = 0.0;
					desiredLuminanceCoef=0;
				};
				class C_Astro1: C_Astro1
				{	
					ambient[]={0.032000002,0.063199997,0.096000001};
					desiredLuminanceCoef=0;
					filmGrainIntensity = 0.0;
				};
				class C_Astro2: C_Astro2
				{
					ambient[]={0.032000002,0.063199997,0.096000001};
					desiredLuminanceCoef=0;
					filmGrainIntensity = 0.0;
				};
				class C_Astro3: C_Astro3
				{
					ambient[]={0.032000002,0.063199997,0.096000001};
					desiredLuminanceCoef=0;
					filmGrainIntensity = 0.0;
				};
				class C_Nautical1: C_Nautical1
				{
					ambient[]={0.064000003,0.12639999,0.192};
					desiredLuminanceCoef=0;
					filmGrainIntensity = 0.0;
				};
				class C_Nautical2: C_Nautical2
				{
					ambient[]={0.064000003,0.12639999,0.192};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class C_Nautical3: C_Nautical3
				{
					ambient[]={0.064000003,0.12639999,0.192};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class C_Civil1: C_Civil1
				{
					ambient[]={0.12800001,0.25279999,0.384};
					desiredLuminanceCoef=0.050000001;
					filmGrainIntensity = 0.0;
				};
				class C_Civil2: C_Civil2
				{
					ambient[]=
					{
						{0.12800001,0.25279999,0.384},
						2
					};
					desiredLuminanceCoef=0.25;
					filmGrainIntensity = 0.0;
				};
				class C_Civil3: C_Civil3
				{
					ambient[]={0.2,0.23,0.44999999};
					desiredLuminanceCoef=0.30000001;
					filmGrainIntensity = 0.0;
				};
				class C_Day0: C_Day0
				{
					ambient[]={0.2,0.23,0.44999999};
					desiredLuminanceCoef=0.34999999;
					filmGrainIntensity=0;
				};
				class C_Day4: C_Day4
				{	
					ambient[]=
					{
						{0.2,0.23,0.44999999}, // Bug is just before
						4.5	
					};
					desiredLuminanceCoef=0.34999999;
					filmGrainIntensity=0;
				};
				class C_Day8: C_Day8
				{
					ambient[]=
					{
						{0.12,0.18000001,0.37}, // Bug is just before
						9
					};
					desiredLuminanceCoef=0.44999999;
					filmGrainIntensity=0;
				};
				class C_Day12: C_Day12
				{
					ambient[]=
					{
						{0.12,0.18000001,0.34}, // Bug is just before
						9
					};
					desiredLuminanceCoef=0.54000002;
					filmGrainIntensity=0;
				};
				class C_Day16: C_Day16
				{
					ambient[]=
					{
						{0.12,0.18000001,0.31999999},
						9
					};
					desiredLuminanceCoef=0.60000002;
					filmGrainIntensity=0;
				};
				class C_Day35: C_Day35
				{
					ambient[]=
					{
						{0.12,0.18000001,0.31999999},
						9
					};
					desiredLuminanceCoef=0.63999999;
					filmGrainIntensity=0;
				};
				class C_Day60: C_Day60
				{
					ambient[]=
					{
						{0.12,0.18000001,0.31999999},
						9
					};
					desiredLuminanceCoef=0.68000001;
					filmGrainIntensity=0;
				};
				class O_Night: O_Night
				{
					ambient[]={0.035999998,0.057,0.085000001};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Astro1: O_Astro1
				{
					ambient[]={0.035999998,0.057,0.085000001};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Nautical1: O_Nautical1
				{
					ambient[]={0.035999998,0.057,0.085000001};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Civil1: O_Civil1
				{
					ambient[]={0.1,0.18000001,0.28999999};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Civil2: O_Civil2
				{
					ambient[]={0.145,0.227,0.34900001};
					desiredLuminanceCoef=0.050000001;
					filmGrainIntensity = 0.0;
				};
				class O_Day0: O_Day0
				{
					ambient[]={0.145,0.227,0.34900001};
					desiredLuminanceCoef=0.30000001;
					filmGrainIntensity=0;
				};
				class O_Day4: O_Day4
				{
					ambient[]=
					{
						{0.145,0.227,0.34900001},
						9
					};
					desiredLuminanceCoef=0.40000001;
					filmGrainIntensity=0;
				};
				class O_Day8: O_Day8
				{
					ambient[]=
					{
						{0.145,0.227,0.34900001},
						9
					};
					desiredLuminanceCoef=0.44999999;
					filmGrainIntensity=0;
				};
				class O_Day12: O_Day12
				{
					ambient[]=
					{
						{0.145,0.227,0.34900001},
						9
					};
					desiredLuminanceCoef=0.52999997;
					filmGrainIntensity=0;
				};
				class O_Day35: O_Day35
				{
					ambient[]=
					{
						{0.15000001,0.2,0.25},
						9
					};
					desiredLuminanceCoef=0.57999998;
					filmGrainIntensity=0;
				};
				class O_Day60: O_Day60
				{
					ambient[]=
					{
						{0.2,0.23999999,0.28999999},
						9
					};
					desiredLuminanceCoef=0.60000002;
					filmGrainIntensity=0;
				};
			};
			dayTemperature=18;
			nightTemperature=4;
            minHeight=100;
            shoreTop=10;
            peakWaveTop=1000;
            peakWaveBottom=-1000;
			class VolFog: VolFog
			{
				CameraFog=1;
				Item1[]={800,0.059999999,0.93000001,0.13,1};
				Item2[]={1300,0.02,0.89999998,0.050000001,1};
			};
			class Overcast: Overcast
			{
				class Weather1: Weather1
				{
					overcast=0;
					lightingOvercast=0;
					sky="DZ\data\data\sky_clear_sky.paa";
					skyR="DZ\data\data\sky_clear_lco.paa";
					horizon="DZ\data\data\sky_clear_horizont_sky.paa";
					alpha=0;
					bright=0;
					speed=0.1;
					size=0.5;
					height=1;
					through=1;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather2: Weather2
				{
					overcast=0.30000001;
					lightingOvercast=0.30000001;
					sky="DZ\data\data\sky_almostclear_sky.paa";
					skyR="DZ\data\data\sky_almostclear_lco.paa";
					horizon="DZ\data\data\sky_almostclear_horizont_sky.paa";
					alpha=0.1;
					bright=0.2;
					speed=0.30000001;
					size=0.55000001;
					height=0.89999998;
					through=1;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather3: Weather3
				{
					overcast=0.5;
					lightingOvercast=0.5;
					sky="DZ\data\data\sky_semicloudy_sky.paa";
					skyR="DZ\data\data\sky_semicloudy_lco.paa";
					horizon="DZ\data\data\sky_semicloudy_horizont_sky.paa";
					alpha=0.25;
					bright=0.2;
					speed=0.5;
					size=0.64999998;
					height=0.85000002;
					through=0.55000001;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather4: Weather4
				{
					overcast=0.69999999;
					lightingOvercast=0.69999999;
					sky="DZ\data\data\sky_cloudy_sky.paa";
					skyR="DZ\data\data\sky_cloudy_lco.paa";
					horizon="DZ\data\data\sky_cloudy_horizont_sky.paa";
					alpha=0.34999999;
					bright=0.2;
					speed=0.75;
					size=0.85000002;
					height=0.80000001;
					through=0.25;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather5: Weather5
				{
					overcast=1.01;
					lightingOvercast=1;
					sky="DZ\data\data\sky_mostlycloudy_sky.paa";
					skyR="DZ\data\data\sky_mostlycloudy_lco.paa";
					horizon="DZ\data\data\sky_mostlycloudy_horizont_sky.paa";
					alpha=0.34999999;
					bright=0.1;
					speed=0.80000001;
					size=1;
					height=0.75;
					through=0;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
			};
		};
		envTexture="DZ\Data\data\env_co.paa";
		clouds[]=
		{
			"DZ\Data\data\mrak1.p3d",
			"DZ\Data\data\mrak2.p3d",
			"DZ\Data\data\mrak3.p3d",
			"DZ\Data\data\mrak4.p3d"
		};
		skyObject="DZ\Data\data\obloha.p3d";
		starsObject="DZ\Data\data\stars.p3d";
		pointObject="DZ\Data\data\point.p3d";
		horizontObject="DZ\Data\data\horizont.p3d";
		haloObject="DZ\Data\data\sunHalo.p3d";
		sunObject="DZ\Data\data\sun.p3d";
		rainbowObject="DZ\Data\data\rainbow.p3d";
		moonObject="DZ\Data\data\moon.p3d";
		starsIntensity=0.141;
		hazeDistCoef=0;
		hazeFogCoef=1;
		rainMaterial="{AD76A616D0EF1ABC}Graphics/Materials/postprocess/rain.emat";
		satelliteNormalBlendStart=100;
		satelliteNormalBlendEnd=500;
		flareMinDistance=2;
		flareMinDistanceLerp=1;
		farLightSpotMul=2;
		farLightPointMul=2;
		minTreesInForestSquare=10;
		minRocksInRockSquare=5;
		soundMapAttenCoef=0.0099999998;
		class SoundMapValues: SoundMapValues
		{
			treehard=0.029999999;
			treesoft=0.029999999;
			bushhard=0;
			bushsoft=0;
			forest=1;
			house=0.30000001;
			church=1;
		};
		class Subdivision: Subdivision
		{
			class Fractal: Fractal
			{
				rougness=5;
				maxRoad=0.02;
				maxTrack=0.5;
				maxSlopeFactor=0.050000001;
			};
			class WhiteNoise: WhiteNoise
			{
				rougness=2;
				maxRoad=0.0099999998;
				maxTrack=0.050000001;
				maxSlopeFactor=0.0024999999;
			};
			minY=-0;
			minSlope=0.02;
		};
		//longitude=30;
		//latitude=-56;
		class Lighting: DefaultLighting
		{
			groundReflection[]={0.059999999,0.059999999,0.029999999};
            moonObjectColorFull[]={0.89999998,0.89999998,1,0.69999999};
            moonHaloObjectColorFull[]={0.89999998,0.89999998,1,0.0099999998};
            moonsetObjectColor[]={0.89999998,0.75,0.40000001};
            moonsetHaloObjectColor[]={0.89999998,0.5,0.2};
            nightAngle=5;
            sunSunset=20;
            endSunset=10;
		};
	};
};