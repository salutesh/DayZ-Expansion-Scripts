class CfgPatches
{
	class DayZExpansion_Hardline_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_VanillaFixes_Scripts"
		};
	};
};
class CfgMods
{
	class DZ_Expansion_Hardline
	{
		dir="DayZExpansion/Hardline";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="$STR_MOD_EXPANSION_NAME";
		picture="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip="DayZ Expansion - Hardline";
		overview="$STR_MOD_EXPANSION_DESC";
		action="";
		author="$STR_MOD_EXPANSION_AUTHOR";
		authorID="";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		defines[]=
		{
			"EXPANSIONMODHARDLINE"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Hardline/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Hardline/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Hardline/Scripts/5_Mission"
				};
			};
		};
	};
};
