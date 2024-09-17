/**
 * ExpansionMarketSupplies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSupplies: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 52;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SUPPLIES";
		m_FileName = "Supplies";
		
		AddItem("Paper", 						2,		4,		1,		100);
		AddItem("pen_black", 	340, 	570,	1,100,null,{"Pen_Blue","Pen_Green","Pen_Red"});
		AddItem("rope", 	450, 	750,	1,100);
		// Crafted
		//AddItem("TannedLeather", 				4,		8,		1,		100);
		//AddItem("AntiPestsSpray", 				8,		16,		1,		100);
		AddItem("metalwire", 	450, 	750,	1,100);
		AddItem("epoxyputty", 	610, 	1015,	1,100);
		AddItem("ducttape", 	330, 	545,	1,100);
		AddItem("pot", 	285, 	475,	1,100);
		AddItem("handcuffkeys", 	645, 	1075,	1,100);
		AddItem("handcuffs", 	615, 	1025,	1,100);
		AddItem("netting", 	495, 	830,	1,100);
		AddItem("burlapsack", 	450, 	750,	1,100);
		AddItem("WoodenPlank", 					50,		250,	1,		100);
		AddItem("metalplate", 	305, 	505,	1,100);
		AddItem("smallgascanister", 	125, 	210,	1,100);
		AddItem("mediumgascanister", 	125, 	210,	1,100);
		AddItem("largegascanister", 	125, 	210,	1,100);
		AddItem("tripod", 	540, 	895,	1,100);
		AddItem("beartrap", 	895, 	1490,	1,100);
		AddItem("bear_beige", 	215, 	355,	1,100,null,{"bear_dark","bear_pink","bear_white"});
		//----------------------------------------------------------------------------------------------------------
		//! Keep in mind here that players can buy the Nail boxes and get 70 nails from 1 box!
		// They should not get more or the same amount of the nail box price back when they sell these 70 nails!
		//----------------------------------------------------------------------------------------------------------
		AddItem("nailbox", 	450, 	750,	1,100);
		AddItem("Nail", 5,		10,		1,		100);
		//----------------------------------------------------------------------------------------------------------
		AddItem("barbedwire", 	495, 	825,	1,100);
		AddItem("fabric", 	300, 	500,	1,100);
		AddItem("camonet", 	1260, 	2100,	1,100);
		AddItem("HescoBox", 					1300,	2250,	1,		100);
		AddItem("powergenerator", 	275, 	455,	1,100);

	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionLumber1", 			50,		300,	1,		100);
		AddItem("ExpansionLumber1_5", 			50,		300,	1,		100);
		AddItem("ExpansionLumber3", 			50,		300,	1,		100);
	#endif
	}
};