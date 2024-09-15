class CfgPatches
{
	class DayZExpansion_Garage_Scripts
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
	class DZ_Expansion_Garage
	{
		dir="DayZExpansion/Garage";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="DayZ Expansion - Garage";
		picture="set:expansion_mod_imageset image:mod_v";
		logo="set:expansion_mod_imageset image:mod_v";
		logoSmall="set:expansion_mod_imageset image:mod_v";
		logoOver="set:expansion_mod_imageset image:mod_v";
		tooltip="DayZ Expansion - Garage";
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
					"DayZExpansion/Garage/Scripts/Common",
					"DayZExpansion/Garage/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Garage/Scripts/Common",
					"DayZExpansion/Garage/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Garage/Scripts/Common",
					"DayZExpansion/Garage/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Garage/Scripts/Common",
					"DayZExpansion/Garage/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Garage/Scripts/Common",
					"DayZExpansion/Garage/Scripts/5_Mission"
				};
			};
		};
	};
};
