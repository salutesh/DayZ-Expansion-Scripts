class CfgPatches
{
	class DayZExpansion_BaseBuilding_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_Core_Scripts"
		};
	};
};
class CfgMods
{
	class DZ_Expansion_BaseBuilding
	{
		dir="DayZExpansion/BaseBuilding";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		inputs="DayZExpansion/BaseBuilding/Scripts/Data/Inputs.xml";
		extra=0;
		type="mod";
		name="DayZ Expansion - BaseBuilding";
		picture="set:expansion_mod_imageset image:mod_bb";
		logo="set:expansion_mod_imageset image:mod_bb";
		logoSmall="set:expansion_mod_imageset image:mod_bb";
		logoOver="set:expansion_mod_imageset image:mod_bb";
		tooltip="DayZ Expansion - BaseBuilding";
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
			class widgetStyles
			{
				files[]={};
			};
			class imageSets
			{
				files[]={};
			};
			class engineScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/BaseBuilding/Scripts/Common",
					"DayZExpansion/BaseBuilding/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/BaseBuilding/Scripts/Common",
					"DayZExpansion/BaseBuilding/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/BaseBuilding/Scripts/Common",
					"DayZExpansion/BaseBuilding/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/BaseBuilding/Scripts/Common",
					"DayZExpansion/BaseBuilding/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/BaseBuilding/Scripts/Common",
					"DayZExpansion/BaseBuilding/Scripts/5_Mission"
				};
			};
		};
	};
};
