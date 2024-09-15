class CfgPatches
{
	class DayZExpansion_Core_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_ModStorage_Scripts",
			"DF_Scripts"
		};
	};
};
class CfgMods
{
	class DZ_Expansion_Core
	{
		dir="DayZExpansion/Core";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		creditsJson="DayZExpansion/Core/Scripts/Data/Credits.json";
		extra=0;
		type="mod";
		CF_ModStorage=1;
		name="DayZ Expansion - Core";
		picture="set:expansion_mod_imageset image:mod_c";
		logo="set:expansion_mod_imageset image:mod_c";
		logoSmall="set:expansion_mod_imageset image:mod_c";
		logoOver="set:expansion_mod_imageset image:mod_c";
		tooltip="DayZ Expansion - Core";
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
			class imageSets
			{
				files[]=
				{
					"DayZExpansion/Core/GUI/imagesets/expansion_mod_imageset.imageset",
					"DayZExpansion/Core/GUI/imagesets/expansion_widgets.imageset",
					"DayZExpansion/Core/GUI/imagesets/arrows.imageset",
					"DayZExpansion/Core/GUI/imagesets/expansion_gui.imageset",
					"DayZExpansion/Core/GUI/imagesets/expansion_iconset.imageset",
					"DayZExpansion/Core/GUI/imagesets/expansion_notification_iconset.imageset",
					"DayZExpansion/Core/GUI/imagesets/expansion_inventory.imageset"
				};
			};
			class widgetStyles
			{
				files[]=
				{
					"DayZExpansion/Core/GUI/looknfeel/expansion_widgets.styles"
				};
			};
			class engineScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Core/Scripts/Common",
					"DayZExpansion/Core/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Core/Scripts/Common",
					"DayZExpansion/Core/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Core/Scripts/Common",
					"DayZExpansion/Core/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Core/Scripts/Common",
					"DayZExpansion/Core/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/Core/Scripts/Common",
					"DayZExpansion/Core/Scripts/5_Mission"
				};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionBakedMapObject: HouseNoDestruct
	{
		scope=2;
	};
	class ExpansionPhysicsStructure: Inventory_Base
	{
		scope=2;
		physLayer="item_large";
		overrideDrawArea="8.0";
		forceFarBubble="true";
	};
};
class CfgNonAIVehicles
{
	class StaticObject;
	class ExpansionStaticMapObject: StaticObject
	{
		scope=2;
	};
	class ExpansionLampLightBase: ExpansionStaticMapObject
	{
		color="1.0 0.7 0.4";
		position="0 5 0";
	};
};
