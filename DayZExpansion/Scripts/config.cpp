#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion
	{
		dir = "DayZExpansion";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		versionPath = "DayZExpansion/scripts/Data/Version.hpp";
		inputs = "DayZExpansion/Scripts/Data/Inputs.xml";
		extra = 0;
		type = "mod";
		CF_ModStorage = 1;
		name = "DayZ Expansion";
		picture = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion";
		overview = "$STR_MOD_EXPANSION_DESC";
		action = "";
		author = "$STR_MOD_EXPANSION_AUTHOR";
		authorID = "";
		expansionSkins[] = {"DayZExpansion/Skins/Character/HeadGear/","DayZExpansion/Skins/Gear/Container/","DayZExpansion/Skins/Gear/Cooking/","DayZExpansion/Skins/Gear/Tools/","DayZExpansion/Skins/Gear/Traps/"};
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class widgetStyles
			{
				files[] = {"DayZExpansion/GUI/looknfeel/dayzexpansionwidgets.styles"};
			};
			class engineScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/5_Mission"};
			};
		};
	};
};
