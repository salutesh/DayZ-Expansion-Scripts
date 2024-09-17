/**
 * ExpansionMarketFood.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("zagorky", 	1225, 	2045,	1,100);
		AddItem("zagorkychocolate", 	780, 	1300,	1,100);
		AddItem("zagorkypeanuts", 	400, 	665,	1,100);

		AddItem("powderedmilk", 	940, 	1565,	1,100);
		AddItem("boxcerealcrunchin", 	640, 	1065,	1,100);
		AddItem("rice", 	560, 	930,	1,100);

		AddItem("marmalade", 	675, 	1130,	1,100);
		AddItem("honey", 	1075, 	1790,	1,100);

		AddItem("saltysticks", 	700, 	1165,	1,100);
		AddItem("crackers", 	700, 	1165,	1,100);
		AddItem("chips", 	795, 	1320,	1,100);

		AddItem("pajka", 	700, 	1165,	1,100);
		AddItem("pate", 	700, 	1165,	1,100);
		AddItem("brisketspread", 	795, 	1320,	1,100);

		AddItem("sardinescan", 	765, 	1275,	1,100);
		AddItem("tunacan", 	1105, 	1840,	1,100);
		AddItem("dogfoodcan", 	1030, 	1715,	1,100);
		AddItem("catfoodcan", 	1075, 	1790,	1,100);
		AddItem("porkcan", 	1030, 	1715,	1,100);
		AddItem("lunchmeat", 	950, 	1585,	1,100);

		AddItem("unknownfoodcan", 	315, 	520,	1,100);
		AddItem("peachescan", 	930, 	1550,	1,100);
		AddItem("spaghettican", 	1075, 	1790,	1,100);
		AddItem("bakedbeanscan", 	935, 	1560,	1,100);
		AddItem("tacticalbaconcan", 	1240, 	2070,	1,100);
		
		#ifndef DAYZ_1_25
		AddItem("crabcan", 	175, 	290,	1,100);
		#endif
	
	#ifdef EXPANSIONMOD	
		AddItem("ExpansionBread1", 				15,			30,			1,		100);
		AddItem("ExpansionBread2", 				15,			30,			1,		100);
		AddItem("ExpansionBread3", 				15,			30,			1,		100);
		AddItem("ExpansionCheese1", 			15,			30,			1,		100);
		AddItem("ExpansionCheese2", 			15,			30,			1,		100);
		AddItem("ExpansionCheese3", 			15,			30,			1,		100);
		AddItem("ExpansionCheese4", 			15,			30,			1,		100);
	#endif
	}
};