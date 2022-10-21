#define _ARMA_

class CfgPatches
{
	class DayZExpansion_VirtualContainerStorage_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_BaseBuilding_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_VirtualContainerStorage
	{
		dir = "DayZExpansion/VirtualContainerStorage";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		name = "DayZ Expansion - VirtualContainerStorage";
		picture = "set:expansion_mod_imageset image:mod_bb";
		logo = "set:expansion_mod_imageset image:mod_bb";
		logoSmall = "set:expansion_mod_imageset image:mod_bb";
		logoOver = "set:expansion_mod_imageset image:mod_bb";
		tooltip = "DayZ Expansion - VirtualContainerStorage";
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
				files[] = {"DayZExpansion/VirtualContainerStorage/Scripts/Common","DayZExpansion/VirtualContainerStorage/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/VirtualContainerStorage/Scripts/Common","DayZExpansion/VirtualContainerStorage/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/VirtualContainerStorage/Scripts/Common","DayZExpansion/VirtualContainerStorage/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/VirtualContainerStorage/Scripts/Common","DayZExpansion/VirtualContainerStorage/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/VirtualContainerStorage/Scripts/Common","DayZExpansion/VirtualContainerStorage/Scripts/5_Mission"};
			};
		};
	};
};
