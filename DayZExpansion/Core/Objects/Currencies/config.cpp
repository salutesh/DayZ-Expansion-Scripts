class CfgPatches
{
	class DayZExpansion_Core_Objects_Currencies
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionMoneyBase: Inventory_Base
	{
		scope=0;
		storageCategory=1;
		rotationFlags=17;
		quantityBar=0;
		iconType=0;
		destroyOnEmpty=1;
		canBeSplit=1;
		simpleHeap=1;
		itemSize[]={1,1};
		isMeleeWeapon=1;
		varQuantityInit=1;
		varQuantityMin=0;
		varQuantityMax=100;
		varQuantityDestroyOnMin=1;
		varStackMax=100;
		soundUse="craft_rounds";
		emptySound="craft_rounds";
	};
	class ExpansionMoneyNugget_Base: ExpansionMoneyBase
	{
		scope=0;
	};
	class ExpansionMoneyBar_Base: ExpansionMoneyBase
	{
		scope=0;
	};
	class ExpansionMoneyCoin_Base: ExpansionMoneyBase
	{
		scope=0;
	};
	class ExpansionMoneyBanknote_Base: ExpansionMoneyBase
	{
		scope=0;
		itemSize[]={1,2};
		weight=0;
		varQuantityMax=50000;
		varStackMax=50000;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="pickUpPaper_SoundSet";
					id=797;
				};
			};
		};
	};
	class ExpansionGoldNugget: ExpansionMoneyNugget_Base
	{
		scope=2;
		weight=10;
		model="\DayZExpansion\Core\Objects\Currencies\nugget_gold.p3d";
		vehicleClass="Expansion_Trader";
		displayName="$STR_EXPANSION_GOLD_NUGGET";
		descriptionShort="$STR_EXPANSION_GOLD_NUGGET_DESC";
	};
	class ExpansionSilverNugget: ExpansionMoneyNugget_Base
	{
		scope=2;
		weight=10;
		model="\DayZExpansion\Core\Objects\Currencies\nugget_silver.p3d";
		vehicleClass="Expansion_Trader";
		displayName="$STR_EXPANSION_SILVER_NUGGET";
		descriptionShort="$STR_EXPANSION_SILVER_NUGGET_DESC";
	};
	class ExpansionSilverBar: ExpansionMoneyBar_Base
	{
		scope=2;
		weight=50;
		model="\DayZExpansion\Core\Objects\Currencies\silver_bar.p3d";
		vehicleClass="Expansion_Trader";
		displayName="$STR_EXPANSION_SILVER_BAR";
		descriptionShort="$STR_EXPANSION_SILVER_BAR_DESC";
	};
	class ExpansionGoldBar: ExpansionMoneyBar_Base
	{
		scope=2;
		weight=50;
		model="\DayZExpansion\Core\Objects\Currencies\gold_bar.p3d";
		vehicleClass="Expansion_Trader";
		displayName="$STR_EXPANSION_GOLD_BAR";
		descriptionShort="$STR_EXPANSION_GOLD_BAR_DESC";
	};
	class ExpansionMoneyBase_InsanityStack: ExpansionMoneyBase
	{
		descriptionShort="When just having money is not enough, and you need ALL the money.";
		varQuantityMax=16777216;
		varStackMax=16777216;
		itemSize[]={2,1};
		weight=0;
	};
	class ExpansionMoneyNugget_InsanityStack: ExpansionMoneyBase_InsanityStack
	{
	};
	class ExpansionMoneyBar_InsanityStack: ExpansionMoneyBase_InsanityStack
	{
	};
	class ExpansionMoneyCoin_InsanityStack: ExpansionMoneyBase_InsanityStack
	{
	};
	class ExpansionMoneyBanknote_InsanityStack: ExpansionMoneyBase_InsanityStack
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="pickUpPaper_SoundSet";
					id=797;
				};
			};
		};
	};
	class ExpansionGoldNugget_InsanityStack: ExpansionMoneyNugget_InsanityStack
	{
		scope=2;
		model="\DayZExpansion\Core\Objects\Currencies\nugget_gold.p3d";
		displayName="$STR_EXPANSION_GOLD_NUGGET";
	};
	class ExpansionSilverNugget_InsanityStack: ExpansionMoneyNugget_InsanityStack
	{
		scope=2;
		model="\DayZExpansion\Core\Objects\Currencies\nugget_silver.p3d";
		displayName="$STR_EXPANSION_SILVER_NUGGET";
	};
	class ExpansionSilverBar_InsanityStack: ExpansionMoneyBar_InsanityStack
	{
		scope=2;
		model="\DayZExpansion\Core\Objects\Currencies\silver_bar.p3d";
		displayName="$STR_EXPANSION_SILVER_BAR";
	};
	class ExpansionGoldBar_InsanityStack: ExpansionMoneyBar_InsanityStack
	{
		scope=2;
		model="\DayZExpansion\Core\Objects\Currencies\gold_bar.p3d";
		displayName="$STR_EXPANSION_GOLD_BAR";
	};
	class ExpansionBanknoteUSD_InsanityStack: ExpansionMoneyBanknote_InsanityStack
	{
		scope=2;
		displayName="$STR_EXPANSION_BANKNOTES_USD";
		model="dayzexpansion\market\objects\currency\USD.p3d";
	};
	class ExpansionBanknoteEuro_InsanityStack: ExpansionMoneyBanknote_InsanityStack
	{
		scope=2;
		displayName="$STR_EXPANSION_BANKNOTES_EURO";
		model="dayzexpansion\market\objects\currency\Euro.p3d";
	};
	class ExpansionBanknoteHryvnia_InsanityStack: ExpansionMoneyBanknote_InsanityStack
	{
		scope=2;
		displayName="$STR_EXPANSION_BANKNOTES_HRYVNIA";
		model="dayzexpansion\market\objects\currency\Hryvnia.p3d";
	};
	class ExpansionGoldNugget_InsanityStack_Billions: ExpansionGoldNugget_InsanityStack
	{
		descriptionShort="When one stack needs to be enough. The absolute max the game supports, literally billions and billions. Note that the game can NOT use amounts this large without massive rounding errors due to float ‹-› int conversion. ONLY HAVE ONE OF THESE STACKS IN INVENTORY, ELSE YOUR MONEY CAN TURN NEGATIVE DUE TO INTEGER OVERFLOW!";
		varQuantityMax=2147483520;
		varStackMax=2147483520;
	};
	class ExpansionSilverNugget_InsanityStack_Billions: ExpansionSilverNugget_InsanityStack
	{
		descriptionShort="When one stack needs to be enough. The absolute max the game supports, literally billions and billions. Note that the game can NOT use amounts this large without massive rounding errors due to float ‹-› int conversion. ONLY HAVE ONE OF THESE STACKS IN INVENTORY, ELSE YOUR MONEY CAN TURN NEGATIVE DUE TO INTEGER OVERFLOW!";
		varQuantityMax=2147483520;
		varStackMax=2147483520;
	};
	class ExpansionSilverBar_InsanityStack_Billions: ExpansionSilverBar_InsanityStack
	{
		descriptionShort="When one stack needs to be enough. The absolute max the game supports, literally billions and billions. Note that the game can NOT use amounts this large without massive rounding errors due to float ‹-› int conversion. ONLY HAVE ONE OF THESE STACKS IN INVENTORY, ELSE YOUR MONEY CAN TURN NEGATIVE DUE TO INTEGER OVERFLOW!";
		varQuantityMax=2147483520;
		varStackMax=2147483520;
	};
	class ExpansionGoldBar_InsanityStack_Billions: ExpansionGoldBar_InsanityStack
	{
		descriptionShort="When one stack needs to be enough. The absolute max the game supports, literally billions and billions. Note that the game can NOT use amounts this large without massive rounding errors due to float ‹-› int conversion. ONLY HAVE ONE OF THESE STACKS IN INVENTORY, ELSE YOUR MONEY CAN TURN NEGATIVE DUE TO INTEGER OVERFLOW!";
		varQuantityMax=2147483520;
		varStackMax=2147483520;
	};
	class ExpansionBanknoteUSD_InsanityStack_Billions: ExpansionBanknoteUSD_InsanityStack
	{
		descriptionShort="When one stack needs to be enough. The absolute max the game supports, literally billions and billions. Note that the game can NOT use amounts this large without massive rounding errors due to float ‹-› int conversion. ONLY HAVE ONE OF THESE STACKS IN INVENTORY, ELSE YOUR MONEY CAN TURN NEGATIVE DUE TO INTEGER OVERFLOW!";
		varQuantityMax=2147483520;
		varStackMax=2147483520;
	};
	class ExpansionBanknoteEuro_InsanityStack_Billions: ExpansionBanknoteEuro_InsanityStack
	{
		descriptionShort="When one stack needs to be enough. The absolute max the game supports, literally billions and billions. Note that the game can NOT use amounts this large without massive rounding errors due to float ‹-› int conversion. ONLY HAVE ONE OF THESE STACKS IN INVENTORY, ELSE YOUR MONEY CAN TURN NEGATIVE DUE TO INTEGER OVERFLOW!";
		varQuantityMax=2147483520;
		varStackMax=2147483520;
	};
	class ExpansionBanknoteHryvnia_InsanityStack_Billions: ExpansionBanknoteHryvnia_InsanityStack
	{
		descriptionShort="When one stack needs to be enough. The absolute max the game supports, literally billions and billions. Note that the game can NOT use amounts this large without massive rounding errors due to float ‹-› int conversion. ONLY HAVE ONE OF THESE STACKS IN INVENTORY, ELSE YOUR MONEY CAN TURN NEGATIVE DUE TO INTEGER OVERFLOW!";
		varQuantityMax=2147483520;
		varStackMax=2147483520;
	};
	class bldr_expansion_goldbar: HouseNoDestruct
	{
		scope=1;
		model="\DayZExpansion\Core\Objects\Currencies\gold_bar.p3d";
		physLayer="item_small";
		carveNavmesh=0;
	};
	class bldr_expansion_silverbar: HouseNoDestruct
	{
		scope=1;
		model="\DayZExpansion\Core\Objects\Currencies\silver_bar.p3d";
		physLayer="item_small";
		carveNavmesh=0;
	};
	class bldr_expansion_goldnugget: HouseNoDestruct
	{
		scope=1;
		model="\DayZExpansion\Core\Objects\Currencies\nugget_gold.p3d";
		physLayer="item_small";
		carveNavmesh=0;
	};
	class bldr_expansion_silvernugget: HouseNoDestruct
	{
		scope=1;
		model="\DayZExpansion\Core\Objects\Currencies\nugget_silver.p3d";
		physLayer="item_small";
		carveNavmesh=0;
	};
};
