#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_NamalskAdventure
	{
		dir = "DayZExpansion/NamalskAdventure";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		name = "$STR_MOD_EXPANSION_NAME";
		picture = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion - Namalsk Adventure";
		overview = "$STR_MOD_EXPANSION_DESC";
		action = "";
		author = "$STR_MOD_EXPANSION_AUTHOR";
		authorID = "";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/NamalskAdventure/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/NamalskAdventure/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/NamalskAdventure/Scripts/5_Mission"};
			};
		};
	};
};
