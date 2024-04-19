#define _ARMA_

class CfgPatches
{
	class DayZExpansion_PersonalStorageNew_Objects
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Consumables"};
	};
};
class CfgVehicles
{
	class Container_Base;
	class HouseNoDestruct;
	class Paper;
	class ExpansionPersonalStorageHub: HouseNoDestruct
	{
		scope = 0;
	};
	class ExpansionPersonalStorageHubChest: ExpansionPersonalStorageHub
	{
		scope = 1;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE";
		model = "\DZ\gear\camping\sea_chest.p3d";
		expansionPersonalStorageBase = "ExpansionPersonalStorageContainer";
	};
	class ExpansionPersonalStorageContainer: Container_Base
	{
		scope = 0;
		model = "\DZ\gear\camping\sea_chest.p3d";
		itemSize[] = {10,5};
		attachments[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband"};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				name = "$STR_attachment_accessories";
				description = "";
				attachmentSlots[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband"};
				icon = "set:dayz_inventory image:shoulderleft";
			};
		};
	};
	class ExpansionPersonalStorageContainer_Level1: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL1";
		class Cargo
		{
			itemsCargoSize[] = {10,5};
		};
	};
	class ExpansionPersonalStorageContainer_Level2: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL2";
		class Cargo
		{
			itemsCargoSize[] = {10,10};
		};
	};
	class ExpansionPersonalStorageContainer_Level3: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL3";
		class Cargo
		{
			itemsCargoSize[] = {10,15};
		};
	};
	class ExpansionPersonalStorageContainer_Level4: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL4";
		class Cargo
		{
			itemsCargoSize[] = {10,20};
		};
	};
	class ExpansionPersonalStorageContainer_Level5: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL5";
		class Cargo
		{
			itemsCargoSize[] = {10,25};
		};
	};
	class ExpansionPersonalStorageContainer_Level6: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL6";
		class Cargo
		{
			itemsCargoSize[] = {10,30};
		};
	};
	class ExpansionPersonalStorageContainer_Level7: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL7";
		class Cargo
		{
			itemsCargoSize[] = {10,35};
		};
	};
	class ExpansionPersonalStorageContainer_Level8: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL8";
		class Cargo
		{
			itemsCargoSize[] = {10,40};
		};
	};
	class ExpansionPersonalStorageContainer_Level9: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL9";
		class Cargo
		{
			itemsCargoSize[] = {10,45};
		};
	};
	class ExpansionPersonalStorageContainer_Level10: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_LVL10";
		class Cargo
		{
			itemsCargoSize[] = {10,50};
		};
	};
	class ExpansionPersonalStorageUpgrade: Paper
	{
		scope = 0;
		descriptionShort = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_DESC";
		canBeSplit = 0;
		varQuantityMax = 1;
		varStackMax = 1;
	};
	class ExpansionPersonalStorageUpgrade_Level2: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL2";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level3: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL3";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level4: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL4";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level5: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL5";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level6: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL6";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level7: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL7";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level8: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL8";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level9: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL9";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level10: ExpansionPersonalStorageUpgrade
	{
		displayName = "$STR_EXPANSION_PERSONALSTORAGE_UPGRADE_LVL10";
		scope = 2;
	};
};
