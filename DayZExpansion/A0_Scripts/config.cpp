#define _ARMA_

class CfgPatches
{
	class A0_DayZExpansion_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {};
	};
};
class CfgMods
{
	class A0_DZ_Expansion_Scripts
	{
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class engineScriptModule
			{
				files[] = {"DayZExpansion/A0_Scripts/Common","DayZExpansion/A0_Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				files[] = {"DayZExpansion/A0_Scripts/Common","DayZExpansion/A0_Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				files[] = {"DayZExpansion/A0_Scripts/Common","DayZExpansion/A0_Scripts/3_Game"};
			};
			class worldScriptModule
			{
				files[] = {"DayZExpansion/A0_Scripts/Common","DayZExpansion/A0_Scripts/4_World"};
			};
			class missionScriptModule
			{
				files[] = {"DayZExpansion/A0_Scripts/Common","DayZExpansion/A0_Scripts/5_Mission"};
			};
		};
	};
};
