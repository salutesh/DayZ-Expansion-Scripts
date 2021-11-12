#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Items
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionCodeLock","ExpansionLumber1","ExpansionLumber1_5","ExpansionLumber3"};
		weapons[] = {};
	};
};
class CfgSlots
{
	class Slot_Att_ExpansionCodeLock
	{
		name = "Att_ExpansionCodeLock";
		displayName = "$STR_EXPANSION_BB_CODE_LOCK";
		selection = "att_codelocklock";
		expansionghostIcon = "set:expansion_inventory image:codelock";
	};
	class Slot_Att_ExpansionCodeLock_1
	{
		name = "Att_ExpansionCodeLock_1";
		displayName = "$STR_EXPANSION_BB_CODE_LOCK";
		selection = "att_codelocklock";
		expansionghostIcon = "set:expansion_inventory image:codelock";
	};
	class Slot_Att_ExpansionCodeLock_2
	{
		name = "Att_ExpansionCodeLock_2";
		displayName = "$STR_EXPANSION_BB_CODE_LOCK";
		selection = "att_codelocklock";
		expansionghostIcon = "set:expansion_inventory image:codelock";
	};
	class Slot_Att_ExpansionLumber
	{
		name = "Att_ExpansionLumber";
		displayName = "$STR_EXPANSION_LUMBER_LARGE";
		selection = "att_lumber";
		expansionGhostIcon = "set:expansion_inventory image:lumber_3";
		stackMax = 10;
	};
	class Slot_Att_ExpansionLumberHalf
	{
		name = "Att_ExpansionLumberHalf";
		displayName = "$STR_EXPANSION_LUMBER_BIG";
		selection = "att_lumber";
		expansionGhostIcon = "set:expansion_inventory image:lumber_15";
		stackMax = 10;
	};
	class Slot_Att_ExpansionLumberThird
	{
		name = "Att_ExpansionLumberThird";
		displayName = "$STR_EXPANSION_LUMBER_MEDIUM";
		selection = "att_lumber";
		expansionGhostIcon = "set:expansion_inventory image:lumber_1";
		stackMax = 10;
	};
	class Slot_Att_ExpansionLumberFourth
	{
		name = "Att_ExpansionLumberFourth";
		displayName = "$STR_EXPANSION_LUMBER_SMALL";
		selection = "att_lumber";
		expansionGhostIcon = "set:expansion_inventory image:lumber_05";
		stackMax = 10;
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionCodeLock: Inventory_Base
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		displayName = "$STR_EXPANSION_BB_CODE_LOCK";
		descriptionShort = "$STR_EXPANSION_BB_CODE_LOCK_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Items\Codelock\CodeLock.p3d";
		weight = 500;
		inventorySlot[] = {"Att_ExpansionCodeLock","Att_ExpansionCodeLock_1","Att_ExpansionCodeLock_2","Att_CombinationLock"};
		itemSize[] = {2,2};
		hiddenSelections[] = {"light_red","light_green","light_screen"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Basebuilding\Items\Codelock\data\codelock_co.paa","\DayZExpansion\Objects\Basebuilding\Items\Codelock\data\codelock_co.paa","\DayZExpansion\Objects\Basebuilding\Items\Codelock\data\codelock_co.paa"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\Objects\Basebuilding\Items\Codelock\data\codelock.rvmat","\DayZExpansion\Objects\Basebuilding\Items\Codelock\data\codelock.rvmat","\DayZExpansion\Objects\Basebuilding\Items\Codelock\data\codelock.rvmat"};
		rotationFlags = 17;
		fragility = 0.01;
		class AnimationSources
		{
			class camo
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class Codelock
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class attach_fence
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
	};
	class ExpansionLumberBase: Inventory_Base
	{
		vehicleClass = "Expansion_Construction";
		rotationFlags = 2;
		inventorySlot[] = {"Att_ExpansionLumber","Att_ExpansionLumber1","Att_ExpansionLumber_2"};
		canBeSplit = 1;
		varQuantityInit = 1.0;
		varQuantityMin = 0.0;
		varQuantityMax = 10.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 10.0;
		lootCategory = "Tools";
		lootTag[] = {"Civilian","Camping","Work","Farm","Forester"};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickup
				{
					soundSet = "woodenlog_pickup_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "woodenlog_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class ExpansionLumber0_5: ExpansionLumberBase
	{
		scope = 0;
		displayName = "$STR_EXPANSION_LUMBER_SMALL";
		descriptionShort = "$STR_EXPANSION_LUMBER_SMALL_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Items\Lumber\Lumber_0_5.p3d";
		weight = 500;
		inventorySlot[] = {"Att_ExpansionLumberFourth"};
		itemSize[] = {2,2};
	};
	class ExpansionLumber1: ExpansionLumberBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_LUMBER_MEDIUM";
		descriptionShort = "$STR_EXPANSION_LUMBER_MEDIUM_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Items\Lumber\Lumber_1.p3d";
		inventorySlot[] = {"Att_ExpansionLumberThird"};
		weight = 500;
		itemSize[] = {2,4};
	};
	class ExpansionLumber1_5: ExpansionLumberBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_LUMBER_BIG";
		descriptionShort = "$STR_EXPANSION_LUMBER_BIG_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Items\Lumber\Lumber_1_5.p3d";
		weight = 500;
		inventorySlot[] = {"Att_ExpansionLumberHalf"};
		itemSize[] = {2,8};
	};
	class ExpansionLumber3: ExpansionLumberBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_LUMBER_LARGE";
		descriptionShort = "$STR_EXPANSION_LUMBER_LARGE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Items\Lumber\Lumber_3.p3d";
		weight = 500;
		inventorySlot[] = {"Att_ExpansionLumber"};
		itemSize[] = {2,16};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyCodelock: ProxyAttachment
	{
		model = "\DayZExpansion\Objects\Basebuilding\Items\Codelock\CodeLock.p3d";
		inventorySlot = "Att_ExpansionCodeLock";
	};
	class ProxyCodelock_1: ProxyAttachment
	{
		model = "\DayZExpansion\Objects\Basebuilding\Items\Codelock\proxies\Codelock_1.p3d";
		inventorySlot = "Att_ExpansionCodeLock_1";
	};
	class ProxyCodelock_2: ProxyAttachment
	{
		model = "\DayZExpansion\Objects\Basebuilding\Items\Codelock\proxies\Codelock_2.p3d";
		inventorySlot = "Att_ExpansionCodeLock_2";
	};
	class ProxyLumber: ProxyAttachment
	{
		model = "\DayZExpansion\Objects\Basebuilding\Items\Lumber\Lumber_0_5.p3d";
		inventorySlot = "Att_ExpansionLumber";
	};
	class ProxyLumber_1: ProxyAttachment
	{
		model = "\DayZExpansion\Objects\Basebuilding\Items\Lumber\Lumber_1.p3d";
		inventorySlot = "Att_ExpansionLumber1";
	};
	class ProxyLumber_2: ProxyAttachment
	{
		model = "\DayZExpansion\Objects\Basebuilding\Items\Lumber\Lumber_1_5.p3d";
		inventorySlot = "Att_ExpansionLumber_2";
	};
};
