class CfgPatches
{
	class DayZExpansion_Teleporter_Scripts
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
	class DZ_Expansion_Teleporter
	{
		dir="DayZExpansion/Teleporter";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="DayZ Expansion - Teleporter";
		picture="set:expansion_mod_imageset image:mod_v";
		logo="set:expansion_mod_imageset image:mod_v";
		logoSmall="set:expansion_mod_imageset image:mod_v";
		logoOver="set:expansion_mod_imageset image:mod_v";
		tooltip="DayZ Expansion - Teleporter";
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
			class widgetStyles
			{
				files[]={};
			};
			class imageSets
			{
				files[]={};
			};
			class engineScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Teleporter/Scripts/Common",
					"DayZExpansion/Teleporter/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Teleporter/Scripts/Common",
					"DayZExpansion/Teleporter/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Teleporter/Scripts/Common",
					"DayZExpansion/Teleporter/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Teleporter/Scripts/Common",
					"DayZExpansion/Teleporter/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Teleporter/Scripts/Common",
					"DayZExpansion/Teleporter/Scripts/5_Mission"
				};
			};
		};
	};
};
