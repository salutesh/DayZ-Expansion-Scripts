class CfgPatches
{
	class DayZExpansion_VanillaFixes_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_Core_Scripts"
		};
	};
};
class CfgMods
{
	class DZ_Expansion_VanillaFixes
	{
		dir="DayZExpansion/VanillaFixes";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="DayZ Expansion - VanillaFixes";
		picture="set:expansion_mod_imageset image:mod_c";
		logo="set:expansion_mod_imageset image:mod_c";
		logoSmall="set:expansion_mod_imageset image:mod_c";
		logoOver="set:expansion_mod_imageset image:mod_c";
		tooltip="DayZ Expansion - VanillaFixes";
		overview="$STR_MOD_EXPANSION_DESC";
		action="";
		author="$STR_MOD_EXPANSION_AUTHOR";
		authorID="";
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
				value="";
				files[]=
				{
					"DayZExpansion/VanillaFixes/Scripts/Common",
					"DayZExpansion/VanillaFixes/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/VanillaFixes/Scripts/Common",
					"DayZExpansion/VanillaFixes/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/VanillaFixes/Scripts/Common",
					"DayZExpansion/VanillaFixes/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/VanillaFixes/Scripts/Common",
					"DayZExpansion/VanillaFixes/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/VanillaFixes/Scripts/Common",
					"DayZExpansion/VanillaFixes/Scripts/5_Mission"
				};
			};
		};
	};
};
