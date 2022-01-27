#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Missions_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DZ_Expansion_Missions
	{
		dir = "DayZExpansion/Missions";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		name = "$STR_MOD_EXPANSION_NAME";
		picture = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion - Name Tags";
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
				files[] = {"DayZExpansion/Missions/Scripts/Common","DayZExpansion/Missions/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Missions/Scripts/Common","DayZExpansion/Missions/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Missions/Scripts/Common","DayZExpansion/Missions/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Missions/Scripts/Common","DayZExpansion/Missions/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Missions/Scripts/Common","DayZExpansion/Missions/Scripts/5_Mission"};
			};
		};
	};
};
class CfgVehicles
{
	class M18SmokeGrenade_Purple;
	class ExpansionSupplySignal: M18SmokeGrenade_Purple
	{
		displayName = "$STR_EXPANSION_SUPPLY_GRENADE";
		descriptionShort = "$STR_EXPANSION_SUPPLY_GRENADE_DESC";
	};
};
