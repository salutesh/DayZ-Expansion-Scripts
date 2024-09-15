class CfgPatches
{
	class DayZExpansion_NotificationScheduler_Scripts
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
	class DZ_Expansion_NotificationScheduler
	{
		dir="DayZExpansion/NotificationScheduler";
		credits="$STR_MOD_EXPANSION_AUTHOR";
		extra=0;
		type="mod";
		name="DayZ Expansion - Notification Scheduler";
		picture="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver="DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip="DayZ Expansion - Notification Scheduler";
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
					"DayZExpansion/NotificationScheduler/Scripts/Common",
					"DayZExpansion/NotificationScheduler/Scripts/1_Core"
				};
			};
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NotificationScheduler/Scripts/Common",
					"DayZExpansion/NotificationScheduler/Scripts/2_GameLib"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NotificationScheduler/Scripts/Common",
					"DayZExpansion/NotificationScheduler/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NotificationScheduler/Scripts/Common",
					"DayZExpansion/NotificationScheduler/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DayZExpansion/NotificationScheduler/Scripts/Common",
					"DayZExpansion/NotificationScheduler/Scripts/5_Mission"
				};
			};
		};
	};
};
