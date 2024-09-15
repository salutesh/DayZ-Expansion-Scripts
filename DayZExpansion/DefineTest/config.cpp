class CfgPatches
{
	class DayZExpansion_DefineTest
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={};
	};
};
class CfgMods
{
	class DZ_Expansion_DefineTest
	{
		type="mod";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class engineScriptModule
			{
				files[]=
				{
					"DayZExpansion/DefineTest/Common",
					"DayZExpansion/DefineTest/1_Core"
				};
			};
			class gameLibScriptModule
			{
				files[]=
				{
					"DayZExpansion/DefineTest/Common",
					"DayZExpansion/DefineTest/2_GameLib"
				};
			};
			class gameScriptModule
			{
				files[]=
				{
					"DayZExpansion/DefineTest/Common",
					"DayZExpansion/DefineTest/3_Game"
				};
			};
			class worldScriptModule
			{
				files[]=
				{
					"DayZExpansion/DefineTest/Common",
					"DayZExpansion/DefineTest/4_World"
				};
			};
			class missionScriptModule
			{
				files[]=
				{
					"DayZExpansion/DefineTest/Common",
					"DayZExpansion/DefineTest/5_Mission"
				};
			};
		};
	};
};
