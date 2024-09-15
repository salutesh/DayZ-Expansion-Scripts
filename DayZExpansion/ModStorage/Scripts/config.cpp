class CfgPatches
{
	class DayZExpansion_ModStorage_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"JM_CF_Scripts"
		};
	};
};
class CfgMods
{
	class DZ_Expansion_ModStorage
	{
		dir="DayZExpansion/ModStorage";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="DayZ Expansion - ModStorage";
		picture="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip="DayZ Expansion - ModStorage";
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
		class defs
		{
			class engineScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/ModStorage/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/ModStorage/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/ModStorage/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/ModStorage/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/ModStorage/Scripts/5_Mission"
				};
			};
		};
	};
};
