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
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 100.0;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100.0;
		soundUse = "craft_rounds";
		emptySound = "craft_rounds";
	};
	class ExpansionMoneyNugget_Base: ExpansionMoneyBase
	{
		scope = 0;
	};
	class ExpansionMoneyBar_Base: ExpansionMoneyBase
	{
		scope = 0;
	};
	class ExpansionMoneyCoin_Base: ExpansionMoneyBase
	{
		scope = 0;
	};
	class ExpansionMoneyBanknote_Base: ExpansionMoneyBase
	{
		scope = 0;
		itemSize[] = {1,2};
		weight = 0;
		varQuantityMax = 50000;
		varStackMax = 50000;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
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
	class ExpansionGoldNugget: ExpansionMoneyNugget_Base
	{
		scope = 2;
		weight = 10;
		model = "\DayZExpansion\Core\Objects\Currencies\nugget_gold.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_NUGGET";
		descriptionShort = "$STR_EXPANSION_GOLD_NUGGET_DESC";
	};
	class ExpansionSilverNugget: ExpansionMoneyNugget_Base
	{
		scope = 2;
		weight = 10;
		model = "\DayZExpansion\Core\Objects\Currencies\nugget_silver.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_NUGGET";
		descriptionShort = "$STR_EXPANSION_SILVER_NUGGET_DESC";
	};
	class ExpansionSilverBar: ExpansionMoneyBar_Base
	{
		scope = 2;
		weight = 50;
		model = "\DayZExpansion\Core\Objects\Currencies\silver_bar.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_BAR";
		descriptionShort = "$STR_EXPANSION_SILVER_BAR_DESC";
	};
	class ExpansionGoldBar: ExpansionMoneyBar_Base
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
