#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Quests_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts","DayZExpansion_Book_Scripts","DayZExpansion_Navigation_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_Quests
	{
		dir = "DayZExpansion/Quests";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		name = "$STR_MOD_EXPANSION_NAME";
		picture = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion - Quests";
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
				files[] = {"DayZExpansion/Quests/Scripts/Common","DayZExpansion/Quests/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Quests/Scripts/Common","DayZExpansion/Quests/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Quests/Scripts/Common","DayZExpansion/Quests/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Quests/Scripts/Common","DayZExpansion/Quests/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Quests/Scripts/Common","DayZExpansion/Quests/Scripts/5_Mission"};
			};
		};
	};
};
