#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Core_Objects_Currencies
	{
		units[] = {"ExpansionGoldNugget","ExpansionSilverNugget","ExpansionSilverBar","ExpansionGoldBar"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionMoneyBase: Inventory_Base
	{
		scope = 0;
		storageCategory = 1;
		rotationFlags = 17;
		quantityBar = 0;
		iconType = 0;
		destroyOnEmpty = 1;
		canBeSplit = 1;
		simpleHeap = 1;
		itemSize[] = {1,1};
		isMeleeWeapon = 1;
		varQuantityInit = 1.0;
		varQuantityMin = 0;
		varQuantityMax = 100.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100.0;
		soundUse = "craft_rounds";
		emptySound = "craft_rounds";
	};
	class ExpansionGoldNugget: ExpansionMoneyBase
	{
		scope = 2;
		weight = 10;
		model = "\DayZExpansion\Core\Objects\Currencies\nugget_gold.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_NUGGET";
		descriptionShort = "$STR_EXPANSION_GOLD_NUGGET_DESC";
	};
	class ExpansionSilverNugget: ExpansionMoneyBase
	{
		scope = 2;
		weight = 10;
		model = "\DayZExpansion\Core\Objects\Currencies\nugget_silver.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_NUGGET";
		descriptionShort = "$STR_EXPANSION_SILVER_NUGGET_DESC";
	};
	class ExpansionSilverBar: ExpansionMoneyBase
	{
		scope = 2;
		weight = 50;
		model = "\DayZExpansion\Core\Objects\Currencies\silver_bar.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_BAR";
		descriptionShort = "$STR_EXPANSION_SILVER_BAR_DESC";
	};
	class ExpansionGoldBar: ExpansionMoneyBase
	{
		scope = 2;
		weight = 50;
		model = "\DayZExpansion\Core\Objects\Currencies\gold_bar.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_BAR";
		descriptionShort = "$STR_EXPANSION_GOLD_BAR_DESC";
	};
	class bldr_expansion_goldbar: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Core\Objects\Currencies\gold_bar.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_expansion_silverbar: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Core\Objects\Currencies\silver_bar.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_expansion_goldnugget: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Core\Objects\Currencies\nugget_gold.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_expansion_silvernugget: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Core\Objects\Currencies\nugget_silver.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
};
