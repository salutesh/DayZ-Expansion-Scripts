#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Scripts
	{
		units[] = {"ExpansionRadio"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Scripts"};
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
class CfgAddons
{
	class PreloadBanks{};
	class PreloadAddons
	{
		class DayZ
		{
			list[] = {"DZ_A_Airport","DZ_Animals","DZ_Animals_AnimConfig","DZ_Animals_Birds","DZ_Animals_Birds_Chicken","DZ_Animals_Birds_Cock","DZ_Animals_Birds_Crow","DZ_Animals_Birds_hawk","DZ_Animals_Birds_Seagull","DZ_Animals_Cow","DZ_Animals_Deer","DZ_Animals_Dogs_Fin","DZ_Animals_Dogs_Pastor","DZ_Animals_Goat","DZ_Animals_Insect","DZ_Animals_Rabbit","DZ_Animals_Rat","DZ_Animals_Sheep","DZ_Animals_WildBoar","DZ_Anims","DZ_Anims_Config_Sdr","DZ_Anims_Config_Wmn","DZ_Anims_Config_Zmb","DZ_Buildings","DZ_Buildings2","DZ_Buildings2_A_Crane_02","DZ_Buildings2_A_GENERALSTORE_01","DZ_Buildings2_A_Pub","DZ_Buildings2_A_Statue","DZ_Buildings2_Barn_Metal","DZ_Buildings2_BuildingParts","DZ_Buildings2_BuildingParts_Signs","DZ_Buildings2_BuildingParts_Signs_TEC","DZ_Buildings2_Church_01","DZ_Buildings2_Farm_Cowshed","DZ_Buildings2_Farm_WTower","DZ_Buildings2_HouseBlocks_HouseBlock_A","DZ_Buildings2_HouseBlocks_HouseBlock_B","DZ_Buildings2_HouseBlocks_HouseBlock_C","DZ_Buildings2_HouseBlocks_HouseBlock_D","DZ_Buildings2_Ind_CementWorks_Ind_Dopravnik","DZ_Buildings2_Ind_CementWorks_Ind_Expedice","DZ_Buildings2_Ind_CementWorks_Ind_MalyKomin","DZ_Buildings2_Ind_CementWorks_Ind_Mlyn","DZ_Buildings2_Ind_CementWorks_Ind_Pec","DZ_Buildings2_Ind_CementWorks_Ind_SiloMale","DZ_Buildings2_Ind_CementWorks_Ind_SiloVelke","DZ_Buildings2_Ind_CementWorks_Ind_Vysypka","DZ_Buildings2_Ind_Garage01","DZ_Buildings2_Ind_Pipeline_IndPipe2","DZ_Buildings2_Ind_Shed_01","DZ_Buildings2_Ind_Shed_02","DZ_Buildings2_Ind_Tank","DZ_Buildings2_Ind_Workshop01","DZ_Buildings2_Misc_Cargo","DZ_Buildings2_Misc_PowerStation","DZ_Buildings2_Misc_WaterStation","DZ_Buildings2_Rail_House_01","DZ_Buildings2_Shed_small","DZ_Buildings2_Shed_wooden","DZ_Characters","DZ_Characters_Backpacks","DZ_Characters_Glasses","DZ_Characters_Gloves","DZ_Characters_Headgear","DZ_Characters_Heads","DZ_Characters_Pants","DZ_Characters_Masks","DZ_Characters_Shoes","DZ_Characters_Tops","DZ_Characters_Vests","DZ_Code","DZ_Data","DZ_Data_Data_ParticleEffects","DZ_Functions","DZ_Gear_Crafting","DZ_Gear_Cooking","DZ_Gear_Consumables","DZ_Gear_Containers","DZ_Gear_Drinks","DZ_Gead_Food","DZ_Gear_Medical","DZ_Gear_Navigation","DZ_Gear_Optics","DZ_Gear_Tools","DZ_Gear_Traps","DZ_Items","DZ_Items_Drinks","DZ_Items_Food","DZ_Items_Medical","DZ_Items_Misc","DZ_Items_Trash","DZ_Items_Weapons","DZ_Misc","DZ_Misc2","DZ_Misc2_Samsite","DZ_Misc3","DZ_Modules","DZ_Roads2","DZ_Roads2_Bridge","DZ_Roads2_Dam","DZ_Rocks2","DZ_Rocks3","DZ_Signs2","DZ_Server_Data","DZ_Spawn_Proxies","DZ_Sounds_Arma2","DZ_Sounds_Mod","DZ_Sounds_Effects","DZ_Structures","DZ_Structures_A_BuildingWIP","DZ_Structures_A_CraneCon","DZ_Structures_A_MunicipalOffice","DZ_Structures_A_TVTower","DZ_Structures_Barn_W","DZ_Structures_Castle","DZ_Structures_Furniture","DZ_Structures_Houses","DZ_Structures_House_A_FuelStation","DZ_Structures_House_A_Hospital","DZ_Structures_House_A_Office01","DZ_Structures_House_A_Office02","DZ_Structures_House_A_StationHouse","DZ_Structures_House_Church_02","DZ_Structures_House_Church_03","DZ_Structures_House_Church_05R","DZ_Structures_House_HouseBT","DZ_Structures_House_HouseV","DZ_Structures_House_HouseV2","DZ_Structures_House_Street_Booth","DZ_Structures_Ind","DZ_Structures_Ind_Pipeline_IndPipe1","DZ_Structures_Ind_Quarry","DZ_Structures_Ind_SawMill","DZ_Structures_Mil","DZ_Structures_Misc","DZ_Structures_Misc_Powerlines","DZ_Structures_NAV","DZ_Structures_Nav_Boathouse","DZ_Structures_Nav_Pier","DZ_Structures_Pond","DZ_Structures_Rail","DZ_Structures_Rail_Rail_Station_big","DZ_Structures_Rail_Railway","DZ_Structures_Ruins","DZ_Structures_Shed_Ind","DZ_Structures_Shed_Shed_Small","DZ_Structures_Tenement","DZ_Structures_Wall","DZ_Structures_Ship_Wreck","DZ_UI","DZ_UIFonts","DZ_UIFonts_CoreConfigOverwrite","DZ_Vehicles_Tracked","DZ_Vehicles_Wheeled","DZ_Vehicles_Wheeled_Datsun_Armed","DZ_Vehicles_Wheeled_Hilux_Armed","DZ_Vehicles_Wrecks","DZ_Weapons_Magazines","DZ_Weapons_Lights","DZ_Weapons_Muzzles","DZ_Weapons_Optics","DZ_Weapons_Supports","DZ_Weapons_Animations","DZ_Weapons_Ammunition","DZ_Weapons_Firearms","DZ_Weapons_Firearms_Winchester70","DZ_Weapons_Firearms_SVD","DZ_Weapons_Melee","DZ_Weapons_Melee_Blunt","DZ_Weapons_Melee_Blade","DZ_Weapons_Melee_Powered","DZ_Weapons_Misc","DZ_Weapons_Explosives","DZ_Weapons_Firearms_aug","DZ_Weapons_Firearms_M4","DZ_Weapons_Firearms_MosinNagant","DZ_Weapons_Firearms_SKS","DZ_Weapons_Firearms_Ruger1022","DZ_Weapons_Pistols_1911","DZ_Weapons_Pistols_Cz75","DZ_Weapons_Pistols_Fnx45","DZ_Weapons_Pistols_Magnum","DZ_Weapons_Pistols_Mkii","DZ_Weapons_Projectiles","DZ_Weapons_Shotguns","DZ_Worlds_Test_Terrain"};
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
		name = "$STR_MOD_EXPANSION_NAME";
		picture = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logo = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logoSmall = "DayZExpansion/GUI/textures/expansion_icon.edds";
		logoOver = "DayZExpansion/GUI/textures/expansion_icon.edds";
		tooltip = "DayZ Expansion";
		overview = "$STR_MOD_EXPANSION_DESC";
		action = "";
		author = "$STR_MOD_EXPANSION_AUTHOR";
		authorID = "";
		expansionSkins[] = {"DayZExpansion/Skins/Character/HeadGear/","DayZExpansion/Skins/Gear/Container/","DayZExpansion/Skins/Gear/Cooking/","DayZExpansion/Skins/Gear/Tools/","DayZExpansion/Skins/Gear/Traps/","DayZExpansion/Skins/Vehicles/Air/","DayZExpansion/Skins/Vehicles/Water/","DayZExpansion/Skins/Vehicles/Ground/","DayZExpansion/Skins/Weapons/Explosives/","DayZExpansion/Skins/Weapons/Firearms/","DayZExpansion/Skins/Weapons/Melee/","DayZExpansion/Skins/Weapons/Pistols/","DayZExpansion/Skins/Weapons/Shotguns/","DayZExpansion/Skins/Weapons/Archery/","DayZExpansion/Skins/Weapons/Attachements/"};
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class widgetStyles
			{
				files[] = {"DayZExpansion/GUI/looknfeel/dayzexpansionwidgets.styles"};
			};
			class imageSets
			{
				files[] = {"DayZExpansion/GUI/imagesets/arrows.imageset","DayZExpansion/GUI/imagesets/expansion_book_assets.imageset","DayZExpansion/GUI/imagesets/expansion_gui.imageset","DayZExpansion/GUI/imagesets/expansion_iconset.imageset","DayZExpansion/GUI/imagesets/expansion_notification_iconset.imageset","DayZExpansion/GUI/imagesets/expansion_inventory.imageset"};
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
