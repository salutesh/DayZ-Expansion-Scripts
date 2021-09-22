#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Core
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"JM_CF_Scripts","DF_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_Core
	{
		dir = "DayZExpansion/Core";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		CF_ModStorage = 1;
		name = "$STR_MOD_EXPANSION_NAME";
		picture = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion - Core";
		overview = "$STR_MOD_EXPANSION_DESC";
		action = "";
		author = "$STR_MOD_EXPANSION_AUTHOR";
		authorID = "";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class imageSets
			{
				files[] = {"DayZExpansion/Core/GUI/imagesets/expansion_widgets.imageset","DayZExpansion/Core/GUI/imagesets/arrows.imageset","DayZExpansion/Core/GUI/imagesets/expansion_gui.imageset","DayZExpansion/Core/GUI/imagesets/expansion_iconset.imageset","DayZExpansion/Core/GUI/imagesets/expansion_notification_iconset.imageset","DayZExpansion/Core/GUI/imagesets/expansion_inventory.imageset"};
			};
			class widgetStyles
			{
				files[] = {"DayZExpansion/Core/GUI/looknfeel/expansion_widgets.styles"};
			};
			class engineScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Core/Common","DayZExpansion/Core/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Core/Common","DayZExpansion/Core/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Core/Common","DayZExpansion/Core/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Core/Common","DayZExpansion/Core/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Core/Common","DayZExpansion/Core/5_Mission"};
			};
		};
	};
};
