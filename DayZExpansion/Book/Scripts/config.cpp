#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Book_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_Book
	{
		dir = "DayZExpansion/Book";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		inputs = "DayZExpansion/Book/Scripts/Data/Inputs.xml";
		extra = 0;
		type = "mod";
		name = "$STR_MOD_EXPANSION_NAME";
		picture = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion - Book";
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
				files[] = {"DayZExpansion/Book/GUI/imagesets/expansion_book_assets.imageset"};
			};
			class engineScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Book/Scripts/Common","DayZExpansion/Book/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Book/Scripts/Common","DayZExpansion/Book/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Book/Scripts/Common","DayZExpansion/Book/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Book/Scripts/Common","DayZExpansion/Book/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Book/Scripts/Common","DayZExpansion/Book/Scripts/5_Mission"};
			};
		};
	};
};
