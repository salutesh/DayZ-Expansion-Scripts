class CfgPatches
{
	class DayZExpansion_Quests_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_Core_Scripts",
			"DayZExpansion_VanillaFixes_Scripts"
		};
	};
};
class CfgMods
{
	class DZ_Expansion_Quests
	{
		dir="DayZExpansion/Quests";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		inputs="DayZExpansion/Quests/Scripts/Data/Inputs.xml";
		extra=0;
		type="mod";
		name="DayZ Expansion - Quests";
		picture="DayZExpansion/GUI/textures/expansion_icon.edds";
		logo="DayZExpansion/GUI/textures/expansion_icon.edds";
		logoSmall="DayZExpansion/GUI/textures/expansion_icon.edds";
		logoOver="DayZExpansion/GUI/textures/expansion_icon.edds";
		tooltip="DayZ Expansion - Quests";
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
			"EXPANSIONMODQUESTS",
			"EXPANSIONMODQUESTS_HUD_ENABLE"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Quests/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Quests/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Quests/Scripts/5_Mission"
				};
			};
		};
	};
};
