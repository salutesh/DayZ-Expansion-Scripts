/**
 * ExpansionMarketShirtsAndTShirts.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketShirtsAndTShirts: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 48;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_SHIRTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_TSHIRTS");
		m_FileName = "Shirts_And_TShirts";

		AddItem("tshirt_blue", 	440, 	740,	1,100,null,{"TShirt_OrangeWhiteStripes","TShirt_Red","TShirt_RedBlackStripes","TShirt_Beige","TShirt_Grey","TShirt_Black","TShirt_Green","TShirt_White"});
		AddItem("tshirt_orangewhitestripes", 	440, 	740,	1,100);
		AddItem("tshirt_red", 	440, 	740,	1,100);
		AddItem("tshirt_redblackstripes", 	440, 	740,	1,100);
		AddItem("tshirt_beige", 	440, 	740,	1,100);
		AddItem("tshirt_grey", 	440, 	740,	1,100);
		AddItem("tshirt_black", 	440, 	740,	1,100,null,{"Tshirt_10thAnniversary"});
		AddItem("tshirt_green", 	440, 	740,	1,100);
		
	#ifdef EXPANSIONMOD
		AddItem("ExpansionTee", 550, 	950,	1,		100);
	#endif

		AddItem("telnyashkashirt", 	210, 	355,	1,100);

		AddItem("shirt_bluecheck", 	610, 	1015,	1,100,null,{"Shirt_BlueCheckBright","Shirt_RedCheck","Shirt_WhiteCheck","Shirt_PlaneBlack","Shirt_GreenCheck"});
		AddItem("shirt_bluecheckbright", 	610, 	1015,	1,100);
		AddItem("shirt_redcheck", 	610, 	1015,	1,100);
		AddItem("shirt_whitecheck", 	610, 	1015,	1,100);
		AddItem("shirt_planeblack", 	945, 	1575,	1,100);
		AddItem("shirt_greencheck", 	610, 	1015,	1,100);
		
		AddItem("medicalscrubsshirt_blue", 	650, 	1080,	1,100,null,{"MedicalScrubsShirt_Green","MedicalScrubsShirt_White"});
		AddItem("medicalscrubsshirt_green", 	650, 	1080,	1,100);
		AddItem("medicalscrubsshirt_white", 	650, 	1080,	1,100);
		
		AddItem("chernarussportshirt", 	235, 	390,	1,100);

		AddItem("tacticalshirt_grey", 	1035, 	1725,	1,100,null,{"TacticalShirt_Tan","TacticalShirt_Black","TacticalShirt_Olive"});
		AddItem("tacticalshirt_tan", 	1035, 	1725,	1,100);
		AddItem("tacticalshirt_black", 	6925, 	11545,	1,100);
		AddItem("tacticalshirt_olive", 	1035, 	1725,	1,100);
	}
};