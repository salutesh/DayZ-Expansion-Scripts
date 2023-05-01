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
		displayName = "Storage";
		model = "\DZ\gear\camping\sea_chest.p3d";
		expansionPersonalStorageBase = "ExpansionPersonalStorageContainer";
	};
	class ExpansionPersonalStorageContainer: Container_Base
	{
		scope = 0;
		model = "\DZ\gear\camping\sea_chest.p3d";
		itemSize[] = {10,5};
		attachments[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband"};
	};
	class ExpansionPersonalStorageContainer_Level1: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 1)";
		class Cargo
		{
			itemsCargoSize[] = {10,5};
		};
	};
	class ExpansionPersonalStorageContainer_Level2: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 2)";
		class Cargo
		{
			itemsCargoSize[] = {10,10};
		};
	};
	class ExpansionPersonalStorageContainer_Level3: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 3)";
		class Cargo
		{
			itemsCargoSize[] = {10,15};
		};
	};
	class ExpansionPersonalStorageContainer_Level4: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 4)";
		class Cargo
		{
			itemsCargoSize[] = {10,20};
		};
	};
	class ExpansionPersonalStorageContainer_Level5: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 5)";
		class Cargo
		{
			itemsCargoSize[] = {10,25};
		};
	};
	class ExpansionPersonalStorageContainer_Level6: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 6)";
		class Cargo
		{
			itemsCargoSize[] = {10,30};
		};
	};
	class ExpansionPersonalStorageContainer_Level7: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 7)";
		class Cargo
		{
			itemsCargoSize[] = {10,35};
		};
	};
	class ExpansionPersonalStorageContainer_Level8: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 8)";
		class Cargo
		{
			itemsCargoSize[] = {10,40};
		};
	};
	class ExpansionPersonalStorageContainer_Level9: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 9)";
		class Cargo
		{
			itemsCargoSize[] = {10,45};
		};
	};
	class ExpansionPersonalStorageContainer_Level10: ExpansionPersonalStorageContainer
	{
		scope = 2;
		displayName = "Storage (lv. 10)";
		class Cargo
		{
			itemsCargoSize[] = {10,50};
		};
	};
	class ExpansionPersonalStorageUpgrade: Paper
	{
		scope = 0;
		descriptionShort = "Use this to upgrade your personal storage.";
	};
	class ExpansionPersonalStorageUpgrade_Level2: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 2)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level3: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 3)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level4: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 4)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level5: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 5)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level6: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 6)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level7: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 7)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level8: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 8)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level9: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 9)";
		scope = 2;
	};
	class ExpansionPersonalStorageUpgrade_Level10: ExpansionPersonalStorageUpgrade
	{
		displayName = "Personal storage upgrade (lv. 10)";
		scope = 2;
	};
};
