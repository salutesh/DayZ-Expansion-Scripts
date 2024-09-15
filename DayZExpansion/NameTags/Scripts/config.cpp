class CfgPatches
{
	class DayZExpansion_NameTags_Scripts
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
	class DZ_Expansion_NameTags
	{
		dir="DayZExpansion/NameTags";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="DayZ Expansion - Name Tags";
		picture="set:expansion_mod_imageset image:mod_nt";
		logo="set:expansion_mod_imageset image:mod_nt";
		logoSmall="set:expansion_mod_imageset image:mod_nt";
		logoOver="set:expansion_mod_imageset image:mod_nt";
		tooltip="DayZ Expansion - Name Tags";
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
					"DayZExpansion/NameTags/Scripts/Common",
					"DayZExpansion/NameTags/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NameTags/Scripts/Common",
					"DayZExpansion/NameTags/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NameTags/Scripts/Common",
					"DayZExpansion/NameTags/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NameTags/Scripts/Common",
					"DayZExpansion/NameTags/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NameTags/Scripts/Common",
					"DayZExpansion/NameTags/Scripts/5_Mission"
				};
			};
		};
	};
};
