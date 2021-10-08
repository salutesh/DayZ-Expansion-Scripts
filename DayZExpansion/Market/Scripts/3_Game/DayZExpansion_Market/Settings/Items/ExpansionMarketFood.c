/**
 * ExpansionMarketFood.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketFood: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 24;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FOOD";
		m_FileName = "Food";

		AddItem("Zagorky", 						10,			20,			1,		100,	0);
		AddItem("ZagorkyChocolate", 			10,			20,			1,		100,	0);
		AddItem("ZagorkyPeanuts", 				10,			20,			1,		100,	0);

		AddItem("PowderedMilk", 				10,			20,			1,		100,	0);
		AddItem("BoxCerealCrunchin", 			12,			24,			1,		100,	0);
		AddItem("Rice", 						13,			26,			1,		100,	0);

		AddItem("Marmalade", 					13,			26,			1,		100,	0);
		AddItem("Honey", 						13,			26,			1,		100,	0);

		AddItem("SaltySticks", 					15,			30,			1,		100,	0);
		AddItem("Crackers", 					15,			30,			1,		100,	0);
		AddItem("Chips", 						15,			30,			1,		100,	0);

		AddItem("Pajka", 						15,			30,			1,		100,	0);
		AddItem("Pate", 						15,			30,			1,		100,	0);
		AddItem("BrisketSpread", 				15,			30,			1,		100,	0);

		AddItem("SardinesCan", 					15,			30,			1,		100,	0);
		AddItem("TunaCan", 						15,			30,			1,		100,	0);		
		AddItem("DogFoodCan", 					15,			30,			1,		100,	0);
		AddItem("CatFoodCan", 					15,			30,			1,		100,	0);
		AddItem("PorkCan", 						15,			30,			1,		100,	0);
		AddItem("Lunchmeat", 					15,			30,			1,		100,	0);

		AddItem("UnknownFoodCan", 				15,			30,			1,		100,	0);
		AddItem("PeachesCan", 					15,			30,			1,		100,	0);
		AddItem("SpaghettiCan", 				20,			40,			1,		100,	0);
		AddItem("BakedBeansCan", 				20,			40,			1,		100,	0);
		AddItem("TacticalBaconCan", 			20,			40,			1,		100,	0);
	
	#ifdef EXPANSIONMOD	
		AddItem("ExpansionBread1", 				15,			30,			1,		100,	0);
		AddItem("ExpansionBread2", 				15,			30,			1,		100,	0);
		AddItem("ExpansionBread3", 				15,			30,			1,		100,	0);
		AddItem("ExpansionCheese1", 			15,			30,			1,		100,	0);
		AddItem("ExpansionCheese2", 			15,			30,			1,		100,	0);
		AddItem("ExpansionCheese3", 			15,			30,			1,		100,	0);
		AddItem("ExpansionCheese4", 			15,			30,			1,		100,	0);
	#endif
	}
};