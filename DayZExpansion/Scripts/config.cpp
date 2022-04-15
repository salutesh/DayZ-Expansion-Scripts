#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Scripts
	{
		units[] = {"ExpansionRadio"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_BaseBuilding_Scripts","DayZExpansion_Book_Scripts","DayZExpansion_Vehicles_Scripts","DayZExpansion_Weapons_Scripts"};
	};
};
class CfgNoises
{
	class HeliExpansionNoise
	{
		type = "sound";
		continuousRange = 300;
		strength = 50;
	};
};
class CfgVehicles
{
	class Radio;
	class ExpansionRadio: Radio
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Radio0";
		descriptionShort = "$STR_CfgVehicles_Radio1";
		model = "\dz\gear\radio\unitra_wilga.p3d";
		animClass = "Knife";
		weight = 1700;
		itemSize[] = {2,3};
		fragility = 0.01;
		absorbency = 0.5;
		simulation = "ItemRadio";
		attachments[] = {"BatteryD"};
		oldpower = 0;
		repairableWithKits[] = {5,7};
		repairCosts[] = {30.0,25.0};
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 1;
			energyUsagePerSecond = 0.02;
			attachmentAction = 1;
			wetnessExposure = 0.1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] = {{1.0,{"DZ\gear\radio\data\unitra_wilga.rvmat"}},{0.7,{"DZ\gear\radio\data\unitra_wilga.rvmat"}},{0.5,{"DZ\gear\radio\data\unitra_wilga_damage.rvmat"}},{0.3,{"DZ\gear\radio\data\unitra_wilga_damage.rvmat"}},{0.0,{"DZ\gear\radio\data\unitra_wilga_destruct.rvmat"}}};
				};
			};
		};
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeLightBlunt";
				range = 1.0;
			};
			class Heavy
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 1.0;
			};
			class Sprint
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 2.8;
			};
		};
		class Channels
		{
			class Cro1
			{
				name = "Cro1";
				url = "http://amp.cesnet.cz:8000/cro1.ogg";
			};
		};
	};
};
class CfgMods
{
	class DZ_Expansion
	{
		dir = "DayZExpansion";
		credits = "$STR_MOD_EXPANSION_AUTHOR";
		creditsJson = "DayZExpansion/Scripts/Data/Credits.json";
		versionPath = "DayZExpansion/scripts/Data/Version.hpp";
		inputs = "DayZExpansion/Scripts/Data/Inputs.xml";
		extra = 0;
		type = "mod";
		CF_ModStorage = 1;
		name = "DayZ Expansion";
		picture = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/Core/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion";
		overview = "$STR_MOD_EXPANSION_DESC";
		action = "";
		author = "$STR_MOD_EXPANSION_AUTHOR";
		authorID = "";
		expansionSkins[] = {"DayZExpansion/Skins/Character/HeadGear/","DayZExpansion/Skins/Gear/Container/","DayZExpansion/Skins/Gear/Cooking/","DayZExpansion/Skins/Gear/Tools/","DayZExpansion/Skins/Gear/Traps/","DayZExpansion/Skins/Weapons/Explosives/","DayZExpansion/Skins/Weapons/Firearms/","DayZExpansion/Skins/Weapons/Melee/","DayZExpansion/Skins/Weapons/Pistols/","DayZExpansion/Skins/Weapons/Shotguns/","DayZExpansion/Skins/Weapons/Archery/","DayZExpansion/Skins/Weapons/Attachements/"};
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class widgetStyles
			{
				files[] = {"DayZExpansion/GUI/looknfeel/dayzexpansionwidgets.styles"};
			};
			class imageSets
			{
				files[] = {};
			};
			class engineScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZExpansion/Scripts/Common","DayZExpansion/Scripts/5_Mission"};
			};
		};
	};
};
