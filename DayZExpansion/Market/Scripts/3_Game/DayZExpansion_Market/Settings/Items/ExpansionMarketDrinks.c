/**
 * ExpansionMarketDrinks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("sodacan_pipsi", 	605, 	1005,	1,100);
		AddItem("sodacan_cola", 	605, 	1005,	1,100);
		AddItem("sodacan_spite", 	605, 	1005,	1,100);
		AddItem("sodacan_kvass", 	605, 	1005,	1,100);
		AddItem("sodacan_fronta", 	605, 	1005,	1,100);
		AddItem("waterbottle", 	65, 	105,	1,100);
		AddItem("canteen", 	725, 	1210,	1,100);
		#ifndef DAYZ_1_25
		AddItem("glassbottle", 	130, 	220,	1,100);
		AddItem("filteringbottle", 	145, 	240,	1,100);
		#endif
	#ifdef EXPANSIONMOD
		AddItem("ExpansionMilkBottle", 		130,		220,		1,		100);
	#endif
	}
};