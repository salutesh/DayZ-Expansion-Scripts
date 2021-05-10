#define _ARMA_

class CfgPatches
{
	class A0_DayZExpansion_Vehicles
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {};
	};
};
class CfgMods
{
	class A0_DZ_Expansion_Vehicles
	{
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class engineScriptModule
			{
				files[] = {"DayZExpansion/A0_Vehicles/Common","DayZExpansion/A0_Vehicles/1_Core"};
			};
			class gameLibScriptModule
			{
				files[] = {"DayZExpansion/A0_Vehicles/Common","DayZExpansion/A0_Vehicles/2_GameLib"};
			};
			class gameScriptModule
			{
				files[] = {"DayZExpansion/A0_Vehicles/Common","DayZExpansion/A0_Vehicles/3_Game"};
			};
			class worldScriptModule
			{
				files[] = {"DayZExpansion/A0_Vehicles/Common","DayZExpansion/A0_Vehicles/4_World"};
			};
			class missionScriptModule
			{
				files[] = {"DayZExpansion/A0_Vehicles/Common","DayZExpansion/A0_Vehicles/5_Mission"};
			};
		};
	};
};
