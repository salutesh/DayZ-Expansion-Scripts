#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Chat_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_Chat
	{
		dir = "DayZExpansion/Chat";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		inputs = "DayZExpansion/Chat/Scripts/Data/Inputs.xml";
		extra = 0;
		type = "mod";
		name = "DayZ Expansion - Chat";
		picture = "set:expansion_mod_imageset image:mod_ch";
		logo = "set:expansion_mod_imageset image:mod_ch";
		logoSmall = "set:expansion_mod_imageset image:mod_ch";
		logoOver = "set:expansion_mod_imageset image:mod_ch";
		tooltip = "DayZ Expansion - Chat";
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
				files[] = {"DayZExpansion/Chat/Scripts/Common","DayZExpansion/Chat/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Chat/Scripts/Common","DayZExpansion/Chat/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Chat/Scripts/Common","DayZExpansion/Chat/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Chat/Scripts/Common","DayZExpansion/Chat/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Chat/Scripts/Common","DayZExpansion/Chat/Scripts/5_Mission"};
			};
		};
	};
};
