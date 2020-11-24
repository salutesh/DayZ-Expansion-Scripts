/**
 * ExpansionMarketShirtsAndTShirts.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

		AddItem("Shirt_BlueCheck", 						2,		2,		1,		250,	0 );
		AddItem("Shirt_BlueCheckBright", 				2,		2,		1,		250,	0 );
		AddItem("Shirt_GreenCheck", 					2,		2,		1,		250,	0 );
		AddItem("Shirt_RedCheck", 						2,		2,		1,		250,	0 );
		AddItem("Shirt_WhiteCheck", 					2,		2,		1,		250,	0 );
		AddItem("Shirt_PlaneBlack",						2,		2,		1,		250,	0 );
		
		AddItem("TShirt_Beige", 						1,		1,		1,		250,	0 );
		AddItem("TShirt_Black", 						1,		1,		1,		250,	0 );
		AddItem("TShirt_Blue", 							1,		1,		1,		250,	0 );
		AddItem("TShirt_Green", 						1,		1,		1,		250,	0 );
		AddItem("TShirt_OrangeWhiteStripes", 			1,		1,		1,		250,	0 );
		AddItem("TShirt_Red", 							1,		1,		1,		250,	0 );
		AddItem("TShirt_RedBlackStripes", 				1,		1,		1,		250,	0 );
		AddItem("TShirt_White", 						1,		1,		1,		250,	0 );
		AddItem("TShirt_Grey", 							1,		1,		1,		250,	0 );
		
		AddItem("TacticalShirt_Grey", 					4,		5,		1,		250,	0 );
		AddItem("TacticalShirt_Black", 					4,		5,		1,		250,	0 );
		AddItem("TacticalShirt_Olive", 					4,		5,		1,		250,	0 );
		AddItem("TacticalShirt_Tan", 					4,		5,		1,		250,	0 );
		AddItem("ExpansionPCUCherPat", 					4,		5,		1,		250,	0 );
		AddItem("ExpansionPCUJacketBlue", 				4,		5,		1,		250,	0 );
		
		AddItem("TelnyashkaShirt", 						2,		3,		1,		250,	0 );
		
		AddItem("ChernarusSportShirt", 					3,		4,		1,		250,	0 );
		
		AddItem("MedicalScrubsShirt_Blue", 				1,		2,		1,		250,	0 );
		AddItem("MedicalScrubsShirt_Green", 			1,		2,		1,		250,	0 );
		AddItem("MedicalScrubsShirt_White", 			1,		2,		1,		250,	0 );
	}
};