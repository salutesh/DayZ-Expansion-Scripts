#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Traders
	{
		units[] = {"ExpansionGoldNugget","ExpansionSilverNugget","ExpansionSilverBar","ExpansionGoldBar","bldr_ExpansionBriefcase","bldr_ExpansionBriefcase_Open","bldr_ExpansionBriefcase_MidOpen","ExpansionBriefcase"};
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
		model = "\DayZExpansion\Objects\Gear\Traders\nugget_gold.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_NUGGET";
		descriptionShort = "$STR_EXPANSION_GOLD_NUGGET_DESC";
	};
	class ExpansionSilverNugget: ExpansionMoneyBase
	{
		scope = 2;
		weight = 10;
		model = "\DayZExpansion\Objects\Gear\Traders\nugget_silver.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_NUGGET";
		descriptionShort = "$STR_EXPANSION_SILVER_NUGGET_DESC";
	};
	class ExpansionSilverBar: ExpansionMoneyBase
	{
		scope = 2;
		weight = 1000;
		model = "\DayZExpansion\Objects\Gear\Traders\silver_bar.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_SILVER_BAR";
		descriptionShort = "$STR_EXPANSION_SILVER_BAR_DESC";
	};
	class ExpansionGoldBar: ExpansionMoneyBase
	{
		scope = 2;
		weight = 1000;
		model = "\DayZExpansion\Objects\Gear\Traders\gold_bar.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "$STR_EXPANSION_GOLD_BAR";
		descriptionShort = "$STR_EXPANSION_GOLD_BAR_DESC";
	};
	class HouseNoDestruct;
	class bldr_ExpansionBriefcase: HouseNoDestruct
	{
		scope = 2;
		rotationFlags = 17;
		model = "\DayZExpansion\Objects\Gear\Traders\briefcase.p3d";
	};
	class bldr_ExpansionBriefcase_Open: bldr_ExpansionBriefcase
	{
		scope = 2;
		class AnimationSources
		{
			class upperpart_rotate
			{
				source = "user";
				animPeriod = -2.5;
				initPhase = 1;
			};
		};
	};
	class bldr_ExpansionBriefcase_MidOpen: bldr_ExpansionBriefcase
	{
		scope = 2;
		class AnimationSources
		{
			class upperpart_rotate
			{
				source = "user";
				animPeriod = -1.5;
				initPhase = 0.4;
			};
		};
	};
	class Container_Base;
	class ExpansionBriefcase: Container_Base
	{
		scope = 2;
		weight = 895;
		itemSize[] = {5,3};
		itemsCargoSize[] = {5,3};
		rotationFlags = 17;
		allowOwnedCargoManipulation = 1;
		isMeleeWeapon = 1;
		randomQuantity = 2;
		model = "\DayZExpansion\Objects\Gear\Traders\briefcase.p3d";
		vehicleClass = "Expansion_Trader";
		displayName = "Briefcase";
		descriptionShort = "briefcase description";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 750;
					healthLevels[] = {{1,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0,{""}}};
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage = 8;
					};
					class Blood
					{
						damage = 8;
					};
					class Shock
					{
						damage = 8;
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "smallprotectorcase_pickup_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "smallprotectorcase_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
};
