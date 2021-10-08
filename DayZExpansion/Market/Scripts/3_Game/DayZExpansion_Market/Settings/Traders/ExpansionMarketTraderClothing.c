/**
 * ExpansionMarketTraderClothing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderClothing: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Clothing";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CLOTHING";
		m_FileName = "Clothing";
		TraderIcon = "Backpack";
					
		// BLOUSES AND SUITES
		AddItem("Blouse_Blue");
		//AddItem("Blouse_Violet");
		//AddItem("Blouse_White");
		//AddItem("Blouse_Green");
			
		AddItem("ManSuit_Beige");
		//AddItem("ManSuit_Black");
		//AddItem("ManSuit_Blue");
		//AddItem("ManSuit_Brown");
		//AddItem("ManSuit_DarkGrey");
		//AddItem("ManSuit_Khaki");
		//AddItem("ManSuit_LightGrey");
		//AddItem("ManSuit_White");
			
		AddItem("WomanSuit_Beige");
		//AddItem("WomanSuit_Black");
		//AddItem("WomanSuit_Blue");
		//AddItem("WomanSuit_Brown");
		//AddItem("WomanSuit_DarkGrey");
		//AddItem("WomanSuit_Khaki");
		//AddItem("WomanSuit_LightGrey");
		//AddItem("WomanSuit_White");

		// COATS AND JACKETS
		AddItem("LabCoat");

		AddItem("TrackSuitJacket_Black");
		//AddItem("TrackSuitJacket_Blue");
		//AddItem("TrackSuitJacket_Green");
		//AddItem("TrackSuitJacket_LightBlue");
		//AddItem("TrackSuitJacket_Red");

		AddItem("DenimJacket");

		AddItem("WoolCoat_Red");
		//AddItem("WoolCoat_Blue");
		//AddItem("WoolCoat_Beige");
		//AddItem("WoolCoat_RedCheck");
		//AddItem("WoolCoat_BlueCheck");
		//AddItem("WoolCoat_GreyCheck");
		//AddItem("WoolCoat_BrownCheck");
		//AddItem("WoolCoat_Black");
		//AddItem("WoolCoat_BlackCheck");
		//AddItem("WoolCoat_Green");

		AddItem("RidersJacket_Black");
	//#ifdef EXPANSIONMOD	
		//AddItem("ExpansionRidersJacketBrown");
		//AddItem("ExpansionRidersJacketDarkBlue");
	//#endif
		
		AddItem("FirefighterJacket_Beige");
		//AddItem("FirefighterJacket_Black");
		
		AddItem("JumpsuitJacket_Blue");
		//AddItem("JumpsuitJacket_Gray");
		//AddItem("JumpsuitJacket_Green");
		//AddItem("JumpsuitJacket_Red");

		AddItem("BomberJacket_Brown");
		//AddItem("BomberJacket_Blue");
		//AddItem("BomberJacket_Grey");
		//AddItem("BomberJacket_Maroon");
		//AddItem("BomberJacket_SkyBlue");
		//AddItem("BomberJacket_Black");
		//AddItem("BomberJacket_Olive");

		AddItem("QuiltedJacket_Blue");
		//AddItem("QuiltedJacket_Red");
		//AddItem("QuiltedJacket_Grey");
		//AddItem("QuiltedJacket_Orange");
		//AddItem("QuiltedJacket_Yellow");
		//AddItem("QuiltedJacket_Violet");
		//AddItem("QuiltedJacket_Black");
		//AddItem("QuiltedJacket_Green");

		AddItem("PrisonUniformJacket");

		AddItem("PoliceJacketOrel");
		AddItem("PoliceJacket");

		AddItem("ParamedicJacket_Blue");
		//AddItem("ParamedicJacket_Crimson");
		//AddItem("ParamedicJacket_Green");

		AddItem("HikingJacket_Black");
		//AddItem("HikingJacket_Red");
		//AddItem("HikingJacket_Blue");
		//AddItem("HikingJacket_Green");
	
		AddItem("Raincoat_Pink");
		//AddItem("Raincoat_Orange");
		//AddItem("Raincoat_Yellow");
		//AddItem("Raincoat_Red");
		//AddItem("Raincoat_Blue");
		//AddItem("Raincoat_Black");
		//AddItem("Raincoat_Green");

		AddItem("TTsKOJacket_Camo");		

		AddItem("BDUJacket");

		AddItem("HuntingJacket_Autumn");
		//AddItem("HuntingJacket_Brown");
		//AddItem("HuntingJacket_Spring");
		//AddItem("HuntingJacket_Summer");
		//AddItem("HuntingJacket_Winter");

		AddItem("M65Jacket_Black");
		//AddItem("M65Jacket_Khaki");
		//AddItem("M65Jacket_Tan");
		//AddItem("M65Jacket_Olive");
		
		AddItem("GorkaEJacket_Summer");
		//AddItem("GorkaEJacket_Flat");
		//AddItem("GorkaEJacket_Autumn");
		//AddItem("GorkaEJacket_PautRev");
		
		AddItem("USMCJacket_Desert");
		//AddItem("USMCJacket_Woodland");
		
		AddItem("NBCJacketGray");
		AddItem("NBCJacketYellow");

		// SHIRTS AND T-SHIRTS
		AddItem("TShirt_Blue");
		//AddItem("TShirt_OrangeWhiteStripes");
		//AddItem("TShirt_Red");
		//AddItem("TShirt_RedBlackStripes");
		//AddItem("TShirt_White");
		//AddItem("TShirt_Beige");
		//AddItem("TShirt_Grey");
		//AddItem("TShirt_Black");
		//AddItem("TShirt_Green");
	#ifdef EXPASIONMOD
		AddItem("ExpansionTee");
	#endif

		AddItem("TelnyashkaShirt");

		AddItem("Shirt_BlueCheck");
		//AddItem("Shirt_BlueCheckBright");
		//AddItem("Shirt_RedCheck");
		//AddItem("Shirt_WhiteCheck");
		//AddItem("Shirt_PlaneBlack");
		//AddItem("Shirt_GreenCheck");
		
		AddItem("MedicalScrubsShirt_Blue");
		//AddItem("MedicalScrubsShirt_Green");
		//AddItem("MedicalScrubsShirt_White");
		
		AddItem("ChernarusSportShirt");

		AddItem("TacticalShirt_Grey");
		//AddItem("TacticalShirt_Tan");
		//AddItem("TacticalShirt_Black");
		//AddItem("TacticalShirt_Olive");

		// SKIRTS AND DRESSES
		AddItem("Skirt_Blue");
		//AddItem("Skirt_Red");
		//AddItem("Skirt_White");
		//AddItem("Skirt_Yellow");
		
		AddItem("MiniDress_Pink");
		//AddItem("MiniDress_PinkChecker");
		//AddItem("MiniDress_RedChecker");
		//AddItem("MiniDress_BlueChecker");
		//AddItem("MiniDress_BlueWithDots");
		//AddItem("MiniDress_WhiteChecker");
		//AddItem("MiniDress_BrownChecker");
		//AddItem("MiniDress_GreenChecker");
		
		AddItem("NurseDress_Blue");
		//AddItem("NurseDress_White");

		// PANTS
		AddItem("MedicalScrubsPants_Blue");
		//AddItem("MedicalScrubsPants_Green");
		//AddItem("MedicalScrubsPants_White");

		AddItem("TrackSuitPants_Black");
		//AddItem("TrackSuitPants_Blue");
		//AddItem("TrackSuitPants_Green");
		//AddItem("TrackSuitPants_Red");
		//AddItem("TrackSuitPants_LightBlue");

		AddItem("PrisonUniformPants");

		AddItem("Breeches_Pink");
		//AddItem("Breeches_Red");
		//AddItem("Breeches_White");
		//AddItem("Breeches_Blue");
		//AddItem("Breeches_Beetcheck");
		//AddItem("Breeches_Beige");
		//AddItem("Breeches_Browncheck");
		//AddItem("Breeches_Black");
		//AddItem("Breeches_Blackcheck");
		//AddItem("Breeches_Green");

		AddItem("SlacksPants_Beige");
		//AddItem("SlacksPants_Blue");
		//AddItem("SlacksPants_DarkGrey");
		//AddItem("SlacksPants_LightGrey");
		//AddItem("SlacksPants_White");
		//AddItem("SlacksPants_Brown");
		//AddItem("SlacksPants_Black");
		//AddItem("SlacksPants_Khaki");

		AddItem("CanvasPantsMidi_Blue");
		//AddItem("CanvasPantsMidi_Grey");
		//AddItem("CanvasPantsMidi_Red");
		//AddItem("CanvasPantsMidi_Violet");
		//AddItem("CanvasPantsMidi_Beige");

		AddItem("CanvasPants_Blue");
		//AddItem("CanvasPants_Grey");
		//AddItem("CanvasPants_Red");
		//AddItem("CanvasPants_Violet");
		//AddItem("CanvasPants_Beige");	

		AddItem("JumpsuitPants_Blue");
		//AddItem("JumpsuitPants_Green");
		//AddItem("JumpsuitPants_Grey");
		//AddItem("JumpsuitPants_Red");

		AddItem("PolicePants");

		AddItem("ParamedicPants_Blue");
		//AddItem("ParamedicPants_Crimson");
		//AddItem("ParamedicPants_Green");

		AddItem("FirefightersPants_Beige");
		//AddItem("FirefightersPants_Black");

		AddItem("CargoPants_Beige");
		//AddItem("CargoPants_Black");
		//AddItem("CargoPants_Blue");
		//AddItem("CargoPants_Green");
		//AddItem("CargoPants_Grey");

		AddItem("ShortJeans_Blue");
		//AddItem("ShortJeans_Brown");
		//AddItem("ShortJeans_Darkblue");
		//AddItem("ShortJeans_Red");
		//AddItem("ShortJeans_Black");
		//AddItem("ShortJeans_Green");

		AddItem("Jeans_Blue");
		//AddItem("Jeans_Brown");
		//AddItem("Jeans_Grey");
		//AddItem("Jeans_BlueDark");
		//AddItem("Jeans_Green");
		//AddItem("Jeans_Black");
		
		AddItem("TTSKOPants");

		AddItem("BDUPants");

		AddItem("USMCPants_Desert");
		//AddItem("USMCPants_Woodland");

		AddItem("PolicePantsOrel");

		AddItem("HunterPants_Winter");
		//AddItem("HunterPants_Autumn");
		//AddItem("HunterPants_Brown");
		//AddItem("HunterPants_Spring");
		//AddItem("HunterPants_Summer");
	
		AddItem("GorkaPants_Summer");
		//AddItem("GorkaPants_Autumn");
		//AddItem("GorkaPants_Flat");
		//AddItem("GorkaPants_PautRev");

		AddItem("NBCPantsGray");
		AddItem("NBCPantsYellow");

		// BOOTS AND SHOES
		AddItem("AthleticShoes_Blue");
		//AddItem("AthleticShoes_Brown");
		//AddItem("AthleticShoes_Grey");
		//AddItem("AthleticShoes_Black");
		//AddItem("AthleticShoes_Green");
		
		AddItem("JoggingShoes_Black");
		//AddItem("JoggingShoes_Blue");
		//AddItem("JoggingShoes_Red");
		//AddItem("JoggingShoes_Violet");
		//AddItem("JoggingShoes_White");

		AddItem("Sneakers_Green");
		//AddItem("Sneakers_Red");
		//AddItem("Sneakers_White");
		//AddItem("Sneakers_Black");
		//AddItem("Sneakers_Gray");
		
		AddItem("Ballerinas_Blue");
		//AddItem("Ballerinas_Red");
		//AddItem("Ballerinas_White");
		//AddItem("Ballerinas_Yellow");	

		AddItem("DressShoes_White");	
		//AddItem("DressShoes_Beige");
		//AddItem("DressShoes_Black");
		//AddItem("DressShoes_Brown");
		//AddItem("DressShoes_Sunburst");

		AddItem("HikingBootsLow_Blue");
		//AddItem("HikingBootsLow_Grey");
		//AddItem("HikingBootsLow_Beige");
		//AddItem("HikingBootsLow_Black");
		
		AddItem("WorkingBoots_Yellow");
		//AddItem("WorkingBoots_Grey");
		//AddItem("WorkingBoots_Brown");
		//AddItem("WorkingBoots_Beige");
		//AddItem("WorkingBoots_Green");

		AddItem("HikingBoots_Brown");
		//AddItem("HikingBoots_Black");
	
		AddItem("CombatBoots_Beige");
		//AddItem("CombatBoots_Black");
		//AddItem("CombatBoots_Brown");
		//AddItem("CombatBoots_Green");
		//AddItem("CombatBoots_Grey");

		AddItem("JungleBoots_Beige");
		//AddItem("JungleBoots_Black");
		//AddItem("JungleBoots_Brown");
		//AddItem("JungleBoots_Green");
		//AddItem("JungleBoots_Olive");

		AddItem("Wellies_Black");
		//AddItem("Wellies_Brown");
		//AddItem("Wellies_Grey");
		//AddItem("Wellies_Green");

		AddItem("TTSKOBoots");

		AddItem("MilitaryBoots_Redpunk");
		//AddItem("MilitaryBoots_Bluerock");
		//AddItem("MilitaryBoots_Beige");
		//AddItem("MilitaryBoots_Black");
		//AddItem("MilitaryBoots_Brown");
			
		AddItem("NBCBootsGray");
		AddItem("NBCBootsYellow");
	}
}