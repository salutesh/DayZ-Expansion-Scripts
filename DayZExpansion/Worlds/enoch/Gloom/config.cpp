#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Worlds_enoch_Gloom
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		worlds[] = {};
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
					ambient[] = {0.016,0.0316,0.048};
					filmGrainIntensity = 0.0;
					desiredLuminanceCoef = 0;
				};
				class C_Astro1: C_Astro1
				{
					ambient[] = {0.032,0.0632,0.096};
					desiredLuminanceCoef = 0;
					filmGrainIntensity = 0.0;
				};
				class C_Astro2: C_Astro2
				{
					ambient[] = {0.032,0.0632,0.096};
					desiredLuminanceCoef = 0;
					filmGrainIntensity = 0.0;
				};
				class C_Astro3: C_Astro3
				{
					ambient[] = {0.032,0.0632,0.096};
					desiredLuminanceCoef = 0;
					filmGrainIntensity = 0.0;
				};
				class C_Nautical1: C_Nautical1
				{
					ambient[] = {0.064,0.1264,0.192};
					desiredLuminanceCoef = 0;
					filmGrainIntensity = 0.0;
				};
				class C_Nautical2: C_Nautical2
				{
					ambient[] = {0.064,0.1264,0.192};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class C_Nautical3: C_Nautical3
				{
					ambient[] = {0.064,0.1264,0.192};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class C_Civil1: C_Civil1
				{
					ambient[] = {0.128,0.2528,0.384};
					desiredLuminanceCoef = 0.05;
					filmGrainIntensity = 0.0;
				};
				class C_Civil2: C_Civil2
				{
					ambient[] = {{0.128,0.2528,0.384},2};
					desiredLuminanceCoef = 0.25;
					filmGrainIntensity = 0.0;
				};
				class C_Civil3: C_Civil3
				{
					ambient[] = {0.2,0.23,0.45};
					desiredLuminanceCoef = 0.3;
					filmGrainIntensity = 0.0;
				};
				class C_Day0: C_Day0
				{
					ambient[] = {0.2,0.23,0.45};
					desiredLuminanceCoef = 0.35;
					filmGrainIntensity = 0;
				};
				class C_Day4: C_Day4
				{
					ambient[] = {{0.2,0.23,0.45},4.5};
					desiredLuminanceCoef = 0.35;
					filmGrainIntensity = 0;
				};
				class C_Day8: C_Day8
				{
					ambient[] = {{0.12,0.18,0.37},9};
					desiredLuminanceCoef = 0.45;
					filmGrainIntensity = 0;
				};
				class C_Day12: C_Day12
				{
					ambient[] = {{0.12,0.18,0.34},9};
					desiredLuminanceCoef = 0.54;
					filmGrainIntensity = 0;
				};
				class C_Day16: C_Day16
				{
					ambient[] = {{0.12,0.18,0.32},9};
					desiredLuminanceCoef = 0.6;
					filmGrainIntensity = 0;
				};
				class C_Day35: C_Day35
				{
					ambient[] = {{0.12,0.18,0.32},9};
					desiredLuminanceCoef = 0.64;
					filmGrainIntensity = 0;
				};
				class C_Day60: C_Day60
				{
					ambient[] = {{0.12,0.18,0.32},9};
					desiredLuminanceCoef = 0.68;
					filmGrainIntensity = 0;
				};
				class O_Night: O_Night
				{
					ambient[] = {0.036,0.057,0.085};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Astro1: O_Astro1
				{
					ambient[] = {0.036,0.057,0.085};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Nautical1: O_Nautical1
				{
					ambient[] = {0.036,0.057,0.085};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Civil1: O_Civil1
				{
					ambient[] = {0.1,0.18,0.29};
					desiredLuminanceCoef = 0.0;
					filmGrainIntensity = 0.0;
				};
				class O_Civil2: O_Civil2
				{
					ambient[] = {0.145,0.227,0.349};
					desiredLuminanceCoef = 0.05;
					filmGrainIntensity = 0.0;
				};
				class O_Day0: O_Day0
				{
					ambient[] = {0.145,0.227,0.349};
					desiredLuminanceCoef = 0.3;
					filmGrainIntensity = 0;
				};
				class O_Day4: O_Day4
				{
					ambient[] = {{0.145,0.227,0.349},9};
					desiredLuminanceCoef = 0.4;
					filmGrainIntensity = 0;
				};
				class O_Day8: O_Day8
				{
					ambient[] = {{0.145,0.227,0.349},9};
					desiredLuminanceCoef = 0.45;
					filmGrainIntensity = 0;
				};
				class O_Day12: O_Day12
				{
					ambient[] = {{0.145,0.227,0.349},9};
					desiredLuminanceCoef = 0.53;
					filmGrainIntensity = 0;
				};
				class O_Day35: O_Day35
				{
					ambient[] = {{0.15,0.2,0.25},9};
					desiredLuminanceCoef = 0.58;
					filmGrainIntensity = 0;
				};
				class O_Day60: O_Day60
				{
					ambient[] = {{0.2,0.24,0.29},9};
					desiredLuminanceCoef = 0.6;
					filmGrainIntensity = 0;
				};
			};
			dayTemperature = 18;
			nightTemperature = 4;
			minHeight = 100;
			shoreTop = 10;
			peakWaveTop = 1000;
			peakWaveBottom = -1000;
			class VolFog: VolFog
			{
				CameraFog = 1;
				Item1[] = {800,0.06,0.93,0.13,1};
				Item2[] = {1300,0.02,0.9,0.05,1};
			};
			class Overcast: Overcast
			{
				class Weather1: Weather1
				{
					overcast = 0;
					lightingOvercast = 0;
					sky = "DZ\data\data\sky_clear_sky.paa";
					skyR = "DZ\data\data\sky_clear_lco.paa";
					horizon = "DZ\data\data\sky_clear_horizont_sky.paa";
					alpha = 0;
					bright = 0;
					speed = 0.1;
					size = 0.5;
					height = 1;
					through = 1;
					diffuse = 1;
					cloudDiffuse = 1;
					waves = 100;
				};
				class Weather2: Weather2
				{
					overcast = 0.3;
					lightingOvercast = 0.3;
					sky = "DZ\data\data\sky_almostclear_sky.paa";
					skyR = "DZ\data\data\sky_almostclear_lco.paa";
					horizon = "DZ\data\data\sky_almostclear_horizont_sky.paa";
					alpha = 0.1;
					bright = 0.2;
					speed = 0.3;
					size = 0.55;
					height = 0.9;
					through = 1;
					diffuse = 1;
					cloudDiffuse = 1;
					waves = 100;
				};
				class Weather3: Weather3
				{
					overcast = 0.5;
					lightingOvercast = 0.5;
					sky = "DZ\data\data\sky_semicloudy_sky.paa";
					skyR = "DZ\data\data\sky_semicloudy_lco.paa";
					horizon = "DZ\data\data\sky_semicloudy_horizont_sky.paa";
					alpha = 0.25;
					bright = 0.2;
					speed = 0.5;
					size = 0.65;
					height = 0.85;
					through = 0.55;
					diffuse = 1;
					cloudDiffuse = 1;
					waves = 100;
				};
				class Weather4: Weather4
				{
					overcast = 0.7;
					lightingOvercast = 0.7;
					sky = "DZ\data\data\sky_cloudy_sky.paa";
					skyR = "DZ\data\data\sky_cloudy_lco.paa";
					horizon = "DZ\data\data\sky_cloudy_horizont_sky.paa";
					alpha = 0.35;
					bright = 0.2;
					speed = 0.75;
					size = 0.85;
					height = 0.8;
					through = 0.25;
					diffuse = 1;
					cloudDiffuse = 1;
					waves = 100;
				};
				class Weather5: Weather5
				{
					overcast = 1.01;
					lightingOvercast = 1;
					sky = "DZ\data\data\sky_mostlycloudy_sky.paa";
					skyR = "DZ\data\data\sky_mostlycloudy_lco.paa";
					horizon = "DZ\data\data\sky_mostlycloudy_horizont_sky.paa";
					alpha = 0.35;
					bright = 0.1;
					speed = 0.8;
					size = 1;
					height = 0.75;
					through = 0;
					diffuse = 1;
					cloudDiffuse = 1;
					waves = 100;
				};
			};
		};
		envTexture = "DZ\Data\data\env_co.paa";
		clouds[] = {"DZ\Data\data\mrak1.p3d","DZ\Data\data\mrak2.p3d","DZ\Data\data\mrak3.p3d","DZ\Data\data\mrak4.p3d"};
		skyObject = "DZ\Data\data\obloha.p3d";
		starsObject = "DZ\Data\data\stars.p3d";
		pointObject = "DZ\Data\data\point.p3d";
		horizontObject = "DZ\Data\data\horizont.p3d";
		haloObject = "DZ\Data\data\sunHalo.p3d";
		sunObject = "DZ\Data\data\sun.p3d";
		rainbowObject = "DZ\Data\data\rainbow.p3d";
		moonObject = "DZ\Data\data\moon.p3d";
		starsIntensity = 0.141;
		hazeDistCoef = 0;
		hazeFogCoef = 1;
		rainMaterial = "{AD76A616D0EF1ABC}Graphics/Materials/postprocess/rain.emat";
		satelliteNormalBlendStart = 100;
		satelliteNormalBlendEnd = 500;
		flareMinDistance = 2;
		flareMinDistanceLerp = 1;
		farLightSpotMul = 2;
		farLightPointMul = 2;
		minTreesInForestSquare = 10;
		minRocksInRockSquare = 5;
		soundMapAttenCoef = 0.01;
		class Subdivision: Subdivision
		{
			class Fractal: Fractal
			{
				rougness = 5;
				maxRoad = 0.02;
				maxTrack = 0.5;
				maxSlopeFactor = 0.05;
			};
			class WhiteNoise: WhiteNoise
			{
				rougness = 2;
				maxRoad = 0.01;
				maxTrack = 0.05;
				maxSlopeFactor = 0.0025;
			};
			minY = 0;
			minSlope = 0.02;
		};
		class Lighting: DefaultLighting
		{
			groundReflection[] = {0.06,0.06,0.03};
			moonObjectColorFull[] = {0.9,0.9,1,0.7};
			moonHaloObjectColorFull[] = {0.9,0.9,1,0.01};
			moonsetObjectColor[] = {0.9,0.75,0.4};
			moonsetHaloObjectColor[] = {0.9,0.5,0.2};
			nightAngle = 5;
			sunSunset = 20;
			endSunset = 10;
		};
	};
};
