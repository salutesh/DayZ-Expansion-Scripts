#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Traders
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
	class ExpansionMoneyBase: Inventory_Base
	{
		scope = 0;
		weight = 10;
		itemSize[] = {1,1};
		canBeSplit = 1;
		varQuantityInit = 1.0;
		varQuantityMin = 0.0;
		varQuantityMax = 100.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100.0;
		model = "\DayZExpansion\Objects\Gear\Traders\nugget_gold.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_NUGGET";
		descriptionShort = "$STR_EXPANSION_GOLD_NUGGET_DESC";
	};
	class ExpansionGoldNugget: ExpansionMoneyBase
	{
		scope = 2;
		weight = 10;
		itemSize[] = {1,1};
		canBeSplit = 1;
		varQuantityInit = 1.0;
		varQuantityMin = 0.0;
		varQuantityMax = 100.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100.0;
		model = "\DayZExpansion\Objects\Gear\Traders\nugget_gold.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_NUGGET";
		descriptionShort = "$STR_EXPANSION_GOLD_NUGGET_DESC";
	};
	class ExpansionSilverNugget: ExpansionMoneyBase
	{
		scope = 2;
		weight = 10;
		itemSize[] = {1,1};
		canBeSplit = 1;
		varQuantityInit = 1.0;
		varQuantityMin = 0.0;
		varQuantityMax = 100.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100.0;
		model = "\DayZExpansion\Objects\Gear\Traders\nugget_silver.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_NUGGET";
		descriptionShort = "$STR_EXPANSION_SILVER_NUGGET_DESC";
	};
	class ExpansionSilverBar: ExpansionMoneyBase
	{
		scope = 2;
		weight = 100;
		itemSize[] = {2,1};
		canBeSplit = 1;
		varQuantityInit = 1.0;
		varQuantityMin = 0.0;
		varQuantityMax = 100.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100.0;
		model = "\DayZExpansion\Objects\Gear\Traders\silver_bar.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_BAR";
		descriptionShort = "$STR_EXPANSION_SILVER_BAR_DESC";
	};
	class ExpansionGoldBar: ExpansionMoneyBase
	{
		scope = 2;
		weight = 100;
		itemSize[] = {2,1};
		canBeSplit = 1;
		varQuantityInit = 1.0;
		varQuantityMin = 0.0;
		varQuantityMax = 100.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100.0;
		model = "\DayZExpansion\Objects\Gear\Traders\gold_bar.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_BAR";
		descriptionShort = "$STR_EXPANSION_GOLD_BAR_DESC";
	};
};
