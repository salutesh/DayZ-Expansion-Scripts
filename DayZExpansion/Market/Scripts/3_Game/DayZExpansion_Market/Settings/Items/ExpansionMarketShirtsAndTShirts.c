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


		AddItem("TShirt_Blue", 							8,		16,		1,		100, null, {"TShirt_OrangeWhiteStripes", "TShirt_Red", "TShirt_RedBlackStripes", "TShirt_Beige", "TShirt_Grey", "TShirt_Black", "TShirt_Green", "TShirt_White"});
		/*AddItem("TShirt_OrangeWhiteStripes", 			8,		16,		1,		100);
		AddItem("TShirt_Red", 							8,		16,		1,		100);
		AddItem("TShirt_RedBlackStripes", 				8,		16,		1,		100);*/
		AddItem("TShirt_Beige", 						10,		20,		1,		100);
		AddItem("TShirt_Grey", 							10,		20,		1,		100);
		AddItem("TShirt_Black", 						12,		24,		1,		100, null, {"TShirt_10thAnniversary"});
		AddItem("TShirt_Green", 						12,		24,		1,		100);
		
	#ifdef EXPASIONMOD
		AddItem("ExpansionTee", 						12,		24,		1,		100);
	#endif

		AddItem("TelnyashkaShirt", 						14,		28,		1,		100);

		AddItem("Shirt_BlueCheck", 						16,		32,		1,		100, null, {"Shirt_BlueCheckBright", "Shirt_RedCheck", "Shirt_WhiteCheck", "Shirt_PlaneBlack", "Shirt_GreenCheck"});
		/*AddItem("Shirt_BlueCheckBright", 				16,		32,		1,		100);
		AddItem("Shirt_RedCheck", 						16,		32,		1,		100);
		AddItem("Shirt_WhiteCheck", 					16,		32,		1,		100);*/
		AddItem("Shirt_PlaneBlack",						17,		34,		1,		100);
		AddItem("Shirt_GreenCheck", 					18,		36,		1,		100);
		
		AddItem("MedicalScrubsShirt_Blue", 				20,		40,		1,		100, null, {"MedicalScrubsShirt_Green", "MedicalScrubsShirt_White"});
		/*AddItem("MedicalScrubsShirt_Green", 			20,		40,		1,		100);
		AddItem("MedicalScrubsShirt_White", 			20,		40,		1,		100);*/
		
		AddItem("ChernarusSportShirt", 					25,		50,		1,		100);

		AddItem("TacticalShirt_Grey", 					36,		72,		1,		100, null, {"TacticalShirt_Tan", "TacticalShirt_Black", "TacticalShirt_Olive"});
		AddItem("TacticalShirt_Tan", 					37,		74,		1,		100);
		AddItem("TacticalShirt_Black", 					38,		76,		1,		100);
		AddItem("TacticalShirt_Olive", 					40,		80,		1,		100);
	}
};