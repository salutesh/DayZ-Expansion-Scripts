class CfgPatches
{
	class DayZExpansion_Animations_Preload
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={};
	};
};
class CfgMods
{
	class DZ_Expansion_Animations_Preload
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
					"0_DayZExpansion_Animations_Preload/Common",
					"DayZExpansion/0_Animations_Preload/Common",
					"0_DayZExpansion_Animations_Preload/1_Core",
					"DayZExpansion/0_Animations_Preload/1_Core"
				};
			};
			class gameLibScriptModule
			{
				files[]=
				{
					"0_DayZExpansion_Animations_Preload/Common",
					"DayZExpansion/0_Animations_Preload/Common",
					"0_DayZExpansion_Animations_Preload/2_GameLib",
					"DayZExpansion/0_Animations_Preload/2_GameLib"
				};
			};
			class gameScriptModule
			{
				files[]=
				{
					"0_DayZExpansion_Animations_Preload/Common",
					"DayZExpansion/0_Animations_Preload/Common",
					"0_DayZExpansion_Animations_Preload/3_Game",
					"DayZExpansion/0_Animations_Preload/3_Game"
				};
			};
			class worldScriptModule
			{
				files[]=
				{
					"0_DayZExpansion_Animations_Preload/Common",
					"DayZExpansion/0_Animations_Preload/Common",
					"0_DayZExpansion_Animations_Preload/4_World",
					"DayZExpansion/0_Animations_Preload/4_World"
				};
			};
			class missionScriptModule
			{
				files[]=
				{
					"0_DayZExpansion_Animations_Preload/Common",
					"DayZExpansion/0_Animations_Preload/Common",
					"0_DayZExpansion_Animations_Preload/5_Mission",
					"DayZExpansion/0_Animations_Preload/5_Mission"
				};
			};
		};
	};
};
