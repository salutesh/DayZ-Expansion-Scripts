#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Missions_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core_Scripts","DZ_Weapons_Ammunition","DZ_Weapons_Explosives"};
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
		name = "DayZ Expansion - Missions";
		picture = "set:expansion_mod_imageset image:mod_mi";
		logo = "set:expansion_mod_imageset image:mod_mi";
		logoSmall = "set:expansion_mod_imageset image:mod_mi";
		logoOver = "set:expansion_mod_imageset image:mod_mi";
		tooltip = "DayZ Expansion - Missions";
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
	class Container_Base;
	class ExpansionAirdropContainerBase: Container_Base
	{
		weight = 300000;
	};
};
class CfgMagazines
{
	class Ammo_Flare;
	class Ammo_FlareRed;
	class Ammo_FlareGreen;
	class Ammo_FlareBlue;
	class Expansion_Ammo_FlareSupplyBase: Ammo_Flare
	{
		displayName = "$STR_EXPANSION_SUPPLY_GRENADE";
		descriptionShort = "$STR_EXPANSION_SUPPLY_GRENADE_DESC";
	};
	class Expansion_Ammo_FlareSupplyRed: Ammo_FlareRed{};
	class Expansion_Ammo_FlareSupplyGreen: Ammo_FlareGreen{};
	class Expansion_Ammo_FlareSupplyBlue: Ammo_FlareBlue{};
};
