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
		class SouthSvetlo
		{
			CameraPosition[] = {14170.214,3.558486,13010.97};
			CameraOrientation[] = {-70.47648,5.615775,0.0};
			PlayerPosition[] = {14163.9,2.72286,13012.5};
			PlayerOrientation[] = {56.3428,0,0};
			fov = 0.4;
			date[] = {2017,10,19,16,47};
			overcast = 0.49;
			rain = 0.0;
			fog = 0.67;
			windspeed = 0.22;
			MappingFiles[] = {"DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Svetloyarsk","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/SvetloyarskNoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/SvetRoadForestNoCollision"};
			CustomPose = 1054;
			HandItem = "";
			CanRotate = 0;
		};
		class Cherno
		{
			CameraPosition[] = {6133.798,33.112877,2933.9187};
			CameraOrientation[] = {-99.97454,2.441315,0.0};
			PlayerPosition[] = {6123.98,32.29986,2931.88};
			PlayerOrientation[] = {52.432,351.591,0};
			fov = 0.112;
			date[] = {2017,6,17,11,1};
			overcast = 0.47;
			rain = 0.0;
			fog = 0.15;
			windspeed = 0.22;
			MappingFiles[] = {"DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Enhancement1","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Enhancement1NoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Enhancement2","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Enhancement2NoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Forest1","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Grass1NoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Grass2NoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Forest1NoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Forest2","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Forest2NoCollision"};
			CustomPose = -1;
			HandItem = "";
			CanRotate = 1;
		};
		class Kamyshovo
		{
			CameraPosition[] = {12562.882,8.428674,3562.9573};
			CameraOrientation[] = {-101.180664,3.947046,0.0};
			PlayerPosition[] = {12553.7,7.93986,3561.09};
			PlayerOrientation[] = {76.412,342.156,0};
			fov = 0.184;
			date[] = {2017,10,19,16,47};
			overcast = 0.47;
			rain = 0.0;
			fog = 0.45;
			windspeed = 0.0;
			MappingFiles[] = {"DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Kamyshovo","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Kamyshovo_GrassNoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/GrassNoCollision","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/KamyshovoNoCollision"};
			CustomPose = -1;
			HandItem = "";
			CanRotate = 1;
		};
		class twospooky
		{
			CameraPosition[] = {10652.98,5.028784,2430.7776};
			CameraOrientation[] = {-110.75428,9.651175,0.0};
			PlayerPosition[] = {12553.7,7.93986,3561.09};
			PlayerOrientation[] = {76.412,342.156,0};
			fov = 0.171999;
			date[] = {2017,11,2,0,0};
			overcast = 0.9;
			rain = 0.2;
			fog = 1.0;
			windspeed = 0.0;
			MappingFiles[] = {"DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Elektrozavodsk","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/MainMenu_Elektro","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/ElektrozavodskNoCollision"};
			CustomPose = -1;
			HandItem = "";
			CanRotate = 1;
		};
		class campfire_guitar
		{
			CameraPosition[] = {6276.1436,10.150486,2424.0874};
			CameraOrientation[] = {46.794064,-3.042726,0.0};
			PlayerPosition[] = {6280.49,9.402,2427.84};
			PlayerOrientation[] = {235.915,341.607,0};
			fov = 0.639999;
			date[] = {2017,12,31,23,0};
			overcast = 0.3;
			rain = 0.0;
			fog = 1.0;
			windspeed = 0.0;
			MappingFiles[] = {"DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/MainMenu_Campfire","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Enhancement1","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Enhancement2","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Forest1","DayZExpansion/Scripts/Data/Mapping/ChernarusPlus/Chernogorsk_Forest2"};
			CustomPose = 1051;
			HandItem = "Expansion_Guitar";
			CanRotate = 0;
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
