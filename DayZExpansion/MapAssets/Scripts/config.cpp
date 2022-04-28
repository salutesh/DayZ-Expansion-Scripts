#define _ARMA_

class CfgPatches
{
	class DayZExpansion_MapAssets_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {};
	};
};
class CfgMods
{
	class DZ_Expansion_MapAssets
	{
		dir = "DayZExpansion/MapAssets";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		extra = 0;
		type = "mod";
		name = "DayZ Expansion - MapAssets";
		picture = "set:expansion_mod_imageset image:mod_ma";
		logo = "set:expansion_mod_imageset image:mod_ma";
		logoSmall = "set:expansion_mod_imageset image:mod_ma";
		logoOver = "set:expansion_mod_imageset image:mod_ma";
		tooltip = "DayZ Expansion - MapAssets";
		overview = "$STR_MOD_EXPANSION_DESC";
		action = "";
		author = "$STR_MOD_EXPANSION_AUTHOR";
		authorID = "";
		dependencies[] = {"World"};
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/MapAssets/Scripts/Common","DayZExpansion/MapAssets/Scripts/4_World"};
			};
		};
	};
};
