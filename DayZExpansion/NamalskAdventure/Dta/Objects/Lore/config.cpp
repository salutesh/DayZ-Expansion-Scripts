#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Lore
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Residential","ns2_build_a3"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class Expansion_Satellite_Control: HouseNoDestruct
	{
		scope = 2;
		displayName = "Satellite Control";
		model = "\nst\ns2\build\a3\proxy\a3_flaxbt_panel.p3d";
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		storageCategory = 10;
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
	class Expansion_Fusion_Core: Inventory_Base
	{
		scope = 2;
		model = "DayZExpansion\NamalskAdventure\Dta\Objects\Lore\Expansion_Fusion_Core.p3d";
		bounding = "BSphere";
		forceFarBubble = "true";
		itemBehaviour = 2;
		handheld = "false";
		allowOwnedCargoManipulation = 1;
		attachments[] = {"Att_ExpansionAnomalyCore"};
		weight = 1000000;
		inventoryCondition = "true";
		storageCategory = 1;
		openable = 1;
		vehicleClass = "Inventory";
		physLayer = "item_large";
		hiddenSelections[] = {"body","core","core_shell"};
		hiddenSelectionsTextures[] = {"DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\fusion_core_co.paa","#(argb,8,8,3)color(0,1,1,1.0,CO)","DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\fusion_core_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\Expansion_Fusion_Core.rvmat","","DayZExpansion\NamalskAdventure\Dta\Objects\Lore\data\Expansion_Fusion_Core.rvmat"};
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
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				name = "$STR_attachment_accessories";
				description = "";
				attachmentSlots[] = {"Att_ExpansionAnomalyCore"};
				icon = "set:expansion_inventory image:anomaly";
			};
		};
	};
};
