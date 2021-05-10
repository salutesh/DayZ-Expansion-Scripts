#define _ARMA_

class CfgPatches
{
	class A0_DayZExpansion_Core
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {};
	};
};
class CfgMods
{
	class A0_DZ_Expansion_Core
	{
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class engineScriptModule
			{
				files[] = {"DayZExpansion/A0_Core/Common","DayZExpansion/A0_Core/1_Core"};
			};
			class gameLibScriptModule
			{
				files[] = {"DayZExpansion/A0_Core/Common","DayZExpansion/A0_Core/2_GameLib"};
			};
			class gameScriptModule
			{
				files[] = {"DayZExpansion/A0_Core/Common","DayZExpansion/A0_Core/3_Game"};
			};
			class worldScriptModule
			{
				files[] = {"DayZExpansion/A0_Core/Common","DayZExpansion/A0_Core/4_World"};
			};
			class missionScriptModule
			{
				files[] = {"DayZExpansion/A0_Core/Common","DayZExpansion/A0_Core/5_Mission"};
			};
		};
	};
};
