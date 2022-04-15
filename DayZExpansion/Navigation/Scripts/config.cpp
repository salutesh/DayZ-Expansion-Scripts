#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Navigation_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_Navigation
	{
		dir = "DayZExpansion/Navigation";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		inputs = "DayZExpansion/Navigation/Scripts/Data/Inputs.xml";
		extra = 0;
		type = "mod";
		name = "DayZ Expansion - Navigation";
		picture = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion - Navigation";
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
				files[] = {"DayZExpansion/Navigation/Scripts/Common","DayZExpansion/Navigation/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Navigation/Scripts/Common","DayZExpansion/Navigation/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Navigation/Scripts/Common","DayZExpansion/Navigation/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Navigation/Scripts/Common","DayZExpansion/Navigation/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Navigation/Scripts/Common","DayZExpansion/Navigation/Scripts/5_Mission"};
			};
		};
	};
};
