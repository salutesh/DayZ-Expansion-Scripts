#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Worlds_IntroScenes
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class cfgExpansionCharacterScenes
{
	class ChernarusPlus
	{
		class expansion_market_update2
		{
			CameraPosition[] = {3709.27,403.95,5969.98};
			CameraOrientation[] = {8.0,0.0,0.0};
			PlayerPosition[] = {3709.31,402.012,5972.07};
			PlayerOrientation[] = {33.0,0,0};
			fov = 0.8;
			date[] = {2021,7,4,12,0};
			overcast = 0.0;
			rain = 0.0;
			fog = 0.0;
			windspeed = 0.1;
			MappingFiles[] = {"DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/TraderUpdateSzene2"};
			CustomPose = -1;
			HandItem = "ExpansionGoldnugget";
			CanRotate = 1;
		};
	};
	class Namalsk
	{
		class namalsk_bed
		{
			CameraPosition[] = {4508.6514,18.056488,10944.384};
			CameraOrientation[] = {-43.36185,-30.872278,0.0};
			PlayerPosition[] = {4507.556,16.149231,10945.431};
			PlayerOrientation[] = {-48.103317,0.0,0.0};
			fov = 1.096;
			date[] = {2011,9,20,20,51};
			overcast = 0.0;
			rain = 0.34;
			fog = 0.0;
			windspeed = 0.0;
			MappingFiles[] = {"DayZExpansion/Scripts/Data/Mapping/Namalsk/MainMenu_Namalsk_Bed"};
			CustomPose = 1052;
			HandItem = "";
			CanRotate = 0;
		};
		class namalsk_sub
		{
			CameraPosition[] = {11960.074,8.688129,12006.908};
			CameraOrientation[] = {-67.05381,0.234928,0.0};
			PlayerPosition[] = {11958.228,7.746279,12007.114};
			PlayerOrientation[] = {108.54003,0.0,-0.0};
			fov = 1.096;
			date[] = {2011,9,20,18,33};
			overcast = 0.24;
			rain = 0.0890308;
			fog = 0.19;
			windspeed = 0.0;
			MappingFiles[] = {};
			CustomPose = -1;
			HandItem = "";
			CanRotate = 1;
		};
	};
};
