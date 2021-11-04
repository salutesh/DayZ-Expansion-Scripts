/**
 * ExpansionMarketDrinks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketDrinks: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 17;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_DRINKS";
		m_FileName = "Drinks";

		AddItem("SodaCan_Pipsi", 			10,			20,			1,		100);
		AddItem("SodaCan_Cola", 			10,			20,			1,		100);
		AddItem("SodaCan_Spite", 			10,			20,			1,		100);
		AddItem("SodaCan_Kvass", 			10,			20,			1,		100);
		AddItem("SodaCan_Fronta", 			10,			20,			1,		100);
		AddItem("WaterBottle", 				15,			30,			1,		100);
		AddItem("Canteen", 					18,			36,			1,		100);
		AddItem("Vodka", 					22,			44,			1,		100);
	#ifdef EXPANSIONMOD
		AddItem("ExpansionMilkBottle", 		15,			30,			1,		100);
	#endif
	}
};