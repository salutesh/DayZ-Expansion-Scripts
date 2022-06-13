#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Quests_Objects_Containers
	{
		units[] = {"ExpansionTemporaryOwnedQuestContainer","ExpansionQuestSeaChest"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Container_Base;
	class ExpansionTemporaryOwnedQuestContainer: Container_Base
	{
		scope = 2;
		displayName = "#STR_EXPANSION_TEMPORARY_STORAGE";
		descriptionShort = "#STR_EXPANSION_TEMPORARY_STORAGE_DESC";
		model = "\DZ\gear\camping\wooden_case.p3d";
		itemSize[] = {10,5};
		attachments[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband"};
		class Cargo
		{
			itemsCargoSize[] = {10,100};
		};
	};
	class ExpansionQuestSeaChest: Container_Base
	{
		scope = 2;
		displayName = "#STR_EXPANSION_TEMPORARY_STORAGE";
		descriptionShort = "#STR_EXPANSION_TEMPORARY_STORAGE_DESC";
		model = "\DZ\gear\camping\sea_chest.p3d";
		itemSize[] = {10,5};
		attachments[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband"};
		class Cargo
		{
			itemsCargoSize[] = {10,100};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet = "seachest_movement_SoundSet";
					id = 1;
				};
				class pickUpItem_Light
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 796;
				};
				class pickUpItem
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "seachest_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
};
