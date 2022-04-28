#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Groups_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Book_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_Groups
	{
		dir = "DayZExpansion/Groups";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		name = "DayZ Expansion - Groups";
		picture = "set:expansion_mod_imageset image:mod_g";
		logo = "set:expansion_mod_imageset image:mod_g";
		logoSmall = "set:expansion_mod_imageset image:mod_g";
		logoOver = "set:expansion_mod_imageset image:mod_g";
		tooltip = "DayZ Expansion - Groups";
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
				files[] = {"DayZExpansion/Groups/Scripts/Common","DayZExpansion/Groups/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Groups/Scripts/Common","DayZExpansion/Groups/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Groups/Scripts/Common","DayZExpansion/Groups/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Groups/Scripts/Common","DayZExpansion/Groups/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Groups/Scripts/Common","DayZExpansion/Groups/Scripts/5_Mission"};
			};
		};
	};
};
