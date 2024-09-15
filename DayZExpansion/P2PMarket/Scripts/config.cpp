class CfgPatches
{
	class DayZExpansion_P2PMarket_Scripts
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
	class DZ_Expansion_P2PMarket
	{
		dir="DayZExpansion/P2PMarket";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="DayZ Expansion - P2P Market";
		picture="set:expansion_mod_imageset image:mod_v";
		logo="set:expansion_mod_imageset image:mod_v";
		logoSmall="set:expansion_mod_imageset image:mod_v";
		logoOver="set:expansion_mod_imageset image:mod_v";
		tooltip="DayZ Expansion - P2P Market";
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
					"DayZExpansion/P2PMarket/Scripts/Common",
					"DayZExpansion/P2PMarket/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/P2PMarket/Scripts/Common",
					"DayZExpansion/P2PMarket/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/P2PMarket/Scripts/Common",
					"DayZExpansion/P2PMarket/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/P2PMarket/Scripts/Common",
					"DayZExpansion/P2PMarket/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/P2PMarket/Scripts/Common",
					"DayZExpansion/P2PMarket/Scripts/5_Mission"
				};
			};
		};
	};
};
