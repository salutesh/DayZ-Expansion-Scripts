#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Lore
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Consumables","DZ_Structures_Residential","DZ_Structures_Bliss_Underground","ns2_build_a3","DayZExpansion_Objects_Gear_Electricity"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class ExpansionGenerator_Base;
	class Grenade_Base;
	class Expansion_Satellite_Generator: ExpansionGenerator_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MOBILE_GENERATOR";
		descriptionShort = "Not needed.";
		model = "\DayZExpansion\Objects\Gear\Electricity\NewGenerator.p3d";
		inputRange = 24;
		fuelTankCapacity = 7000;
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		storageCategory = 10;
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 1;
			energyStorageMax = 10000;
			energyUsagePerSecond = 0.28;
			reduceMaxEnergyByDamageCoef = 0.5;
			energyAtSpawn = 5000;
			powerSocketsCount = 4;
			compatiblePlugTypes[] = {2,"PLUG_PAS_TERMINAL",6};
		};
		class AnimationSources
		{
			class socket_1_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class socket_2_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class socket_3_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class socket_4_plugged
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class dial_fuel
			{
				source = "user";
				animPeriod = 1;
				initPhase = 0;
			};
		};
	};
	class Expansion_Satellite_Control: Inventory_Base
	{
		scope = 2;
		displayName = "Satellite Control";
		descriptionShort = "Not needed.";
		model = "\nst\ns2\build\a3\proxy\a3_flaxbt_panel.p3d";
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		storageCategory = 10;
		attachments[] = {"Att_ExpansionKeyCardAntenna"};
		class GUIInventoryAttachmentsProps
		{
			class Attachments_Access
			{
				name = "Key Card";
				description = "Not needed.";
				attachmentSlots[] = {"Att_ExpansionKeyCardAntenna"};
				icon = "set:dayz_inventory image:cat_fp_tents";
				view_index = 1;
			};
		};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class Melee
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
			};
		};
	};
	class Expansion_Satellite_Panel_Lever: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\Underground\Entrance\Underground_Panel_Lever.p3d";
		hiddenSelections[] = {"LED_Red","LED_Green"};
		class AnimationSources
		{
			class PanelLever
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
		};
	};
	class Expansion_CommunityGoals_Board: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\structures\Residential\Misc\Misc_NoticeBoard2.p3d";
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		storageCategory = 10;
	};
	class ExpansionFusionCore: Grenade_Base
	{
		scope = 2;
		displayName = "Fusion Core";
		descriptionShort = "PLACEHOLDER";
		model = "DayZExpansion\NamalskAdventure\Dta\Objects\Lore\Expansion_Fusion_Core.p3d";
		weight = 400;
		hiddenSelections[] = {"body","core"};
		hiddenSelectionsTextures[] = {"DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\fusion_core_co.paa","#(argb,8,8,3)color(0,1,1,1.0,CO)"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\Expansion_Fusion_Core.rvmat",""};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100000;
					healthLevels[] = {};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 0;
					};
				};
				class Meele
				{
					class Health
					{
						damage = 0;
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Grenade_unpin
				{
					soundSet = "Grenade_unpin_SoundSet";
					id = 201;
				};
				class turnOnRadio_Pin
				{
					soundSet = "Grenade_pin_SoundSet";
					id = 1006;
				};
			};
		};
		class AnimationSources
		{
			class AnimRotate
			{
				source = "core";
				animPeriod = 0.018;
				initPhase = 0;
			};
			class Core_Rotation: AnimRotate{};
		};
		soundImpactType = "metal";
	};
	class NA_Documents_Sanctuary1: Inventory_Base
	{
		model = "\nst\ns_dayz\gear\lore\paper_files.p3d";
		scope = 2;
		title = "Plan - Project Sanctuary";
		author = "PRI";
		file = "DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\sanctuary1.html";
		displayName = "Plan - Project Sanctuary";
		descriptionShort = "PLACEHOLDER";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"nst\ns_dayz\gear\lore\data\paper_files2_co.paa"};
		rotationFlags = 1;
		weight = 100;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLabels[] = {1.0,0.7,0.5,0.3,0.0};
					healthLevels[] = {{1.0,{"nst\ns_dayz\gear\lore\data\lore_paper.rvmat"}},{0.7,{"nst\ns_dayz\gear\lore\data\lore_paper.rvmat"}},{0.5,{"nst\ns_dayz\gear\lore\data\lore_paper_damage.rvmat"}},{0.3,{"nst\ns_dayz\gear\lore\data\lore_paper_damage.rvmat"}},{0.0,{"nst\ns_dayz\gear\lore\data\lore_paper_destruct.rvmat"}}};
				};
			};
		};
	};
	class NA_Documents_Sanctuary2: Inventory_Base
	{
		model = "\nst\ns_dayz\gear\lore\paper_files.p3d";
		scope = 2;
		title = "Warp System Manual - WT-001";
		author = "PRI";
		file = "DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\sanctuary2.html";
		displayName = "Warp System Manual - WT-001";
		descriptionShort = "PLACEHOLDER";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"nst\ns_dayz\gear\lore\data\paper_files2_co.paa"};
		rotationFlags = 1;
		weight = 100;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLabels[] = {1.0,0.7,0.5,0.3,0.0};
					healthLevels[] = {{1.0,{"nst\ns_dayz\gear\lore\data\lore_paper.rvmat"}},{0.7,{"nst\ns_dayz\gear\lore\data\lore_paper.rvmat"}},{0.5,{"nst\ns_dayz\gear\lore\data\lore_paper_damage.rvmat"}},{0.3,{"nst\ns_dayz\gear\lore\data\lore_paper_damage.rvmat"}},{0.0,{"nst\ns_dayz\gear\lore\data\lore_paper_destruct.rvmat"}}};
				};
			};
		};
	};
	class NA_Documents_SanctuarySecrets: Inventory_Base
	{
		model = "\nst\ns_dayz\gear\lore\paper_files.p3d";
		scope = 2;
		title = "Investigation Report of A1-B1";
		author = "Unknown Author";
		file = "DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\sanctuary3.html";
		displayName = "Investigation Report of A1-B1";
		descriptionShort = "A document from a unkown author that was investigating the A1 bunker.";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"nst\ns_dayz\gear\lore\data\paper_files2_co.paa"};
		rotationFlags = 1;
		weight = 100;
		attachments[] = {"Att_ExpansionKeyCardAntenna","Att_ExpansionDokumentBeringOutpost"};
		class GUIInventoryAttachmentsProps
		{
			class Attachments_KeyCard
			{
				name = "Key Card";
				description = "Not needed.";
				attachmentSlots[] = {"Att_ExpansionKeyCardAntenna"};
				icon = "set:dayz_inventory image:cat_fp_tents";
				view_index = 1;
			};
			class Attachments_Dokument
			{
				name = "Dokument";
				description = "Not needed.";
				attachmentSlots[] = {"Att_ExpansionDokumentBeringOutpost"};
				icon = "set:dayz_inventory image:cat_fp_tents";
				view_index = 2;
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLabels[] = {1.0,0.7,0.5,0.3,0.0};
					healthLevels[] = {{1.0,{"nst\ns_dayz\gear\lore\data\lore_paper.rvmat"}},{0.7,{"nst\ns_dayz\gear\lore\data\lore_paper.rvmat"}},{0.5,{"nst\ns_dayz\gear\lore\data\lore_paper_damage.rvmat"}},{0.3,{"nst\ns_dayz\gear\lore\data\lore_paper_damage.rvmat"}},{0.0,{"nst\ns_dayz\gear\lore\data\lore_paper_destruct.rvmat"}}};
				};
			};
		};
	};
	class NA_Documents_ProjectSanctuary: Inventory_Base
	{
		model = "\dz\gear\consumables\Paper.p3d";
		scope = 2;
		title = "Status Report - Project Sanctuary";
		author = "PRI";
		file = "DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\project_sanctuary.html";
		displayName = "Status Report - Project Sanctuary";
		descriptionShort = "A document that describes the building process of a unkown facility.";
		weight = 2;
		itemSize[] = {1,1};
		rotationFlags = 16;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\project_sanctuary.paa"};
		inventorySlot[] = {"Att_ExpansionDokumentBeringOutpost"};
		varWetMax = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15;
					healthLevels[] = {{1,{"DZ\gear\consumables\data\Loot_Paper.rvmat"}},{0.7,{"DZ\gear\consumables\data\Loot_Paper.rvmat"}},{0.5,{"DZ\gear\consumables\data\Loot_Paper_damage.rvmat"}},{0.3,{"DZ\gear\consumables\data\Loot_Paper_damage.rvmat"}},{0,{"DZ\gear\consumables\data\Loot_Paper_destruct.rvmat"}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "pickUpPaper_SoundSet";
					id = 797;
				};
			};
		};
	};
	class NA_BeringOutpostNegative: Inventory_Base
	{
		model = "\dz\gear\consumables\Paper.p3d";
		scope = 2;
		displayName = "Drone Picture";
		descriptionShort = "A picture negative taken by a drone that shows a area with a satellite antenna.";
		weight = 2;
		itemSize[] = {1,1};
		rotationFlags = 16;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\bering_outpost_negative.paa"};
		varWetMax = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15;
					healthLevels[] = {{1,{"DZ\gear\consumables\data\Loot_Paper.rvmat"}},{0.7,{"DZ\gear\consumables\data\Loot_Paper.rvmat"}},{0.5,{"DZ\gear\consumables\data\Loot_Paper_damage.rvmat"}},{0.3,{"DZ\gear\consumables\data\Loot_Paper_damage.rvmat"}},{0,{"DZ\gear\consumables\data\Loot_Paper_destruct.rvmat"}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "pickUpPaper_SoundSet";
					id = 797;
				};
			};
		};
	};
	class Expansion_3DPrinter: Inventory_Base
	{
		scope = 2;
		displayName = "Advanced 3D Printer";
		descriptionShort = "Not needed.";
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		storageCategory = 10;
		itemsCargoSize[] = {10,10};
		model = "\nst\ns3\structures\oilrig\sea_oilrig_printer.p3d";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class Melee
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
			};
		};
		class AnimationSources
		{
			class PrinterHead
			{
				source = "user";
				animPeriod = 1;
				initPhase = 0;
			};
		};
	};
};
