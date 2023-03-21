#define _ARMA_

class CfgPatches
{
	class DayZExpansion_PersonalStorage_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_PersonalStorage
	{
		dir = "DayZExpansion/PersonalStorage";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		name = "DayZ Expansion - Personal Storage";
		picture = "set:expansion_mod_imageset image:mod_v";
		logo = "set:expansion_mod_imageset image:mod_v";
		logoSmall = "set:expansion_mod_imageset image:mod_v";
		logoOver = "set:expansion_mod_imageset image:mod_v";
		tooltip = "DayZ Expansion - Personal Storage";
		overview = "$STR_MOD_EXPANSION_DESC";
		action = "";
		author = "$STR_MOD_EXPANSION_AUTHOR";
		authorID = "";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class widgetStyles
			{
				files[] = {};
			};
			class imageSets
			{
				files[] = {};
			};
			class engineScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/PersonalStorage/Scripts/Common","DayZExpansion/PersonalStorage/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/PersonalStorage/Scripts/Common","DayZExpansion/PersonalStorage/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/PersonalStorage/Scripts/Common","DayZExpansion/PersonalStorage/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/PersonalStorage/Scripts/Common","DayZExpansion/PersonalStorage/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/PersonalStorage/Scripts/Common","DayZExpansion/PersonalStorage/Scripts/5_Mission"};
			};
		};
	};
};
