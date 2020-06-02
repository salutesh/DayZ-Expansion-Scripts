/**
 * ExpansionMarketTraderCyril.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderCyril: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderCyril";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CLOTHING";
		m_FileName = "Clothing_Trader_Cyril";
		
		// Helmets
		AddItem("BallisticHelmet_Green");
		AddItem("BallisticHelmet_Black");
		AddItem("BallisticHelmet_UN");
		
		AddItem("ZSh3PilotHelmet");
		AddItem("ZSh3PilotHelmet_Green");
		AddItem("ZSh3PilotHelmet_Black");
		
		AddItem("ConstructionHelmet_Blue");
		AddItem("ConstructionHelmet_Lime");
		AddItem("ConstructionHelmet_Orange");
		AddItem("ConstructionHelmet_Red");
		AddItem("ConstructionHelmet_White");
		AddItem("ConstructionHelmet_Yellow");
		
		AddItem("MotoHelmet_Black");
		AddItem("MotoHelmet_Grey");
		AddItem("MotoHelmet_Green");
		AddItem("MotoHelmet_Lime");
		AddItem("MotoHelmet_Blue");
		AddItem("MotoHelmet_Red");
		AddItem("MotoHelmet_White");
		
		AddItem("DarkMotoHelmet_Black");
		AddItem("DarkMotoHelmet_Grey");
		AddItem("DarkMotoHelmet_Green");
		AddItem("DarkMotoHelmet_Lime");
		AddItem("DarkMotoHelmet_Blue");
		AddItem("DarkMotoHelmet_Red");
		AddItem("DarkMotoHelmet_White");
		
		AddItem("TankerHelmet");
		
		AddItem("GorkaHelmetVisor");
		AddItem("GorkaHelmet");
		AddItem("GorkaHelmet_Green");
		AddItem("GorkaHelmet_Black");
		
		AddItem("GreatHelm");
		
		AddItem("Ssh68Helmet");
		
		AddItem("DirtBikeHelmet_Visor");
		AddItem("DirtBikeHelmet_Mouthguard");
		AddItem("DirtBikeHelmet_Green");
		AddItem("DirtBikeHelmet_Chernarus");
		AddItem("DirtBikeHelmet_Khaki");
		AddItem("DirtBikeHelmet_Police");
		AddItem("DirtBikeHelmet_Red");
		AddItem("DirtBikeHelmet_Black");
		AddItem("DirtBikeHelmet_Blue");
		
		AddItem("HockeyHelmet_Black");
		AddItem("HockeyHelmet_Blue");
		AddItem("HockeyHelmet_Red");
		AddItem("HockeyHelmet_White");
				
		AddItem("Mich2001Helmet");
		
		AddItem("FirefightersHelmet_Red");
		AddItem("FirefightersHelmet_White");
		AddItem("FirefightersHelmet_Yellow");
		
		AddItem("SkateHelmet_Black");
		AddItem("SkateHelmet_Blue");
		AddItem("SkateHelmet_Gray");
		AddItem("SkateHelmet_Green");
		AddItem("SkateHelmet_Red");
		
		// Pants
		AddItem("Jeans_Black");
		AddItem("Jeans_Blue");
		AddItem("Jeans_Brown");
		AddItem("Jeans_Green");
		AddItem("Jeans_Grey");
		AddItem("Jeans_BlueDark");
		
		AddItem("CargoPants_Beige");
		AddItem("CargoPants_Black");
		AddItem("CargoPants_Blue");
		AddItem("CargoPants_Green");
		AddItem("CargoPants_Grey");
		
		AddItem("TTSKOPants");
		
		AddItem("HunterPants_Autumn");
		AddItem("HunterPants_Brown");
		AddItem("HunterPants_Spring");
		AddItem("HunterPants_Summer");
		AddItem("HunterPants_Winter");
		
		AddItem("CanvasPants_Beige");
		AddItem("CanvasPants_Blue");
		AddItem("CanvasPants_Grey");
		AddItem("CanvasPants_Red");
		AddItem("CanvasPants_Violet");
		AddItem("CanvasPantsMidi_Beige");
		AddItem("CanvasPantsMidi_Blue");
		AddItem("CanvasPantsMidi_Grey");
		AddItem("CanvasPantsMidi_Red");
		AddItem("CanvasPantsMidi_Violet");
		
		AddItem("TrackSuitPants_Black");
		AddItem("TrackSuitPants_Blue");
		AddItem("TrackSuitPants_Green");
		AddItem("TrackSuitPants_Red");
		AddItem("TrackSuitPants_LightBlue");
		
		AddItem("GorkaPants_Summer");
		AddItem("GorkaPants_Autumn");
		AddItem("GorkaPants_Flat");
		AddItem("GorkaPants_PautRev");
		
		AddItem("PolicePants");
		
		AddItem("PolicePantsOrel");
		
		AddItem("ParamedicPants_Blue");
		AddItem("ParamedicPants_Crimson");
		AddItem("ParamedicPants_Green");
		
		AddItem("FirefightersPants_Beige");
		AddItem("FirefightersPants_Black");
		AddItem("PrisonUniformPants");
		
		//AddItem("LeatherPants_Natural");
		//AddItem("LeatherPants_Beige");
		//AddItem("LeatherPants_Brown");
		//AddItem("LeatherPants_Black");
		
		AddItem("MedicalScrubsPants_Blue");
		AddItem("MedicalScrubsPants_Green");
		AddItem("MedicalScrubsPants_White");
		
		AddItem("USMCPants_Desert");
		AddItem("USMCPants_Woodland");
		
		AddItem("SlacksPants_Beige");
		AddItem("SlacksPants_Black");
		AddItem("SlacksPants_Blue");
		AddItem("SlacksPants_Brown");
		AddItem("SlacksPants_DarkGrey");
		AddItem("SlacksPants_Khaki");
		AddItem("SlacksPants_LightGrey");
		AddItem("SlacksPants_White");
		
		AddItem("BDUPants");
		AddItem("NBCPantsGray");
		
		AddItem("Breeches_Pink");
		AddItem("Breeches_Red");
		AddItem("Breeches_White");
		AddItem("Breeches_Green");
		AddItem("Breeches_Browncheck");
		AddItem("Breeches_Blue");
		AddItem("Breeches_Blackcheck");
		AddItem("Breeches_Black");
		AddItem("Breeches_Beige");
		AddItem("Breeches_Beetcheck");
	
		AddItem("ShortJeans_Black");
		AddItem("ShortJeans_Blue");
		AddItem("ShortJeans_Brown");
		AddItem("ShortJeans_Darkblue");
		AddItem("ShortJeans_Green");
		AddItem("ShortJeans_Red");
		
		AddItem("Skirt_Blue");
		AddItem("Skirt_Red");
		AddItem("Skirt_White");
		AddItem("Skirt_Yellow");
		
		AddItem("MiniDress_Pink");
		AddItem("MiniDress_PinkChecker");
		AddItem("MiniDress_RedChecker");
		AddItem("MiniDress_BlueChecker");
		AddItem("MiniDress_BlueWithDots");
		AddItem("MiniDress_WhiteChecker");
		AddItem("MiniDress_BrownChecker");
		AddItem("MiniDress_GreenChecker");
		
		AddItem("NurseDress_Blue");
		AddItem("NurseDress_White");
		
		AddItem("JumpsuitPants_Blue");
		AddItem("JumpsuitPants_Green");
		AddItem("JumpsuitPants_Grey");
		AddItem("JumpsuitPants_Red");
		
		// Tops
		AddItem("Sweater_Gray");
		AddItem("Sweater_Blue");
		AddItem("Sweater_Green");
		AddItem("Sweater_Red");
		
		AddItem("Shirt_BlueCheck");
		AddItem("Shirt_BlueCheckBright");
		AddItem("Shirt_GreenCheck");
		AddItem("Shirt_RedCheck");
		AddItem("Shirt_WhiteCheck");
		AddItem("Shirt_PlaneBlack");
		
		AddItem("TShirt_Beige");
		AddItem("TShirt_Black");
		AddItem("TShirt_Blue");
		AddItem("TShirt_Green");
		AddItem("TShirt_OrangeWhiteStripes");
		AddItem("TShirt_Red");
		AddItem("TShirt_RedBlackStripes");
		AddItem("TShirt_White");
		AddItem("TShirt_Grey");
		
		AddItem("Hoodie_Blue");
		AddItem("Hoodie_Black");
		AddItem("Hoodie_Brown");
		AddItem("Hoodie_Green");
		AddItem("Hoodie_Grey");
		AddItem("Hoodie_Red");
		
		AddItem("TacticalShirt_Grey");
		AddItem("TacticalShirt_Black");
		AddItem("TacticalShirt_Olive");
		AddItem("TacticalShirt_Tan");
		
		AddItem("HikingJacket_Black");
		AddItem("HikingJacket_Blue");
		AddItem("HikingJacket_Red");
		AddItem("HikingJacket_Green");
		
		AddItem("Raincoat_Orange");
		AddItem("Raincoat_Green");
		AddItem("Raincoat_Yellow");
		AddItem("Raincoat_Pink");
		AddItem("Raincoat_Red");
		AddItem("Raincoat_Blue");
		AddItem("Raincoat_Black");
		
		AddItem("M65Jacket_Black");
		AddItem("M65Jacket_Khaki");
		AddItem("M65Jacket_Tan");
		AddItem("M65Jacket_Olive");
		
		AddItem("TTsKOJacket_Camo");
		
		AddItem("GorkaEJacket_Summer");
		AddItem("GorkaEJacket_Flat");
		AddItem("GorkaEJacket_Autumn");
		AddItem("GorkaEJacket_PautRev");
		AddItem("RidersJacket_Black");
		
		AddItem("WoolCoat_Black");
		AddItem("WoolCoat_Red");
		AddItem("WoolCoat_Blue");
		AddItem("WoolCoat_Green");
		AddItem("WoolCoat_Beige");
		AddItem("WoolCoat_RedCheck");
		AddItem("WoolCoat_BlackCheck");
		AddItem("WoolCoat_BlueCheck");
		AddItem("WoolCoat_GreyCheck");
		AddItem("WoolCoat_BrownCheck");
		
		AddItem("TrackSuitJacket_Black");
		AddItem("TrackSuitJacket_Blue");
		AddItem("TrackSuitJacket_Green");
		AddItem("TrackSuitJacket_LightBlue");
		AddItem("TrackSuitJacket_Red");
		
		AddItem("PoliceJacket");
		AddItem("PoliceJacketOrel");
		
		AddItem("ParamedicJacket_Blue");
		AddItem("ParamedicJacket_Crimson");
		AddItem("ParamedicJacket_Green");
		
		AddItem("FirefighterJacket_Beige");
		AddItem("FirefighterJacket_Black");
		AddItem("PrisonUniformJacket");
		
		AddItem("MiniDress_Pink");
		AddItem("MiniDress_PinkChecker");
		AddItem("MiniDress_RedChecker");
		AddItem("MiniDress_WhiteChecker");
		AddItem("MiniDress_GreenChecker");
		AddItem("MiniDress_BrownChecker");
		AddItem("MiniDress_BlueChecker");
		AddItem("MiniDress_BlueWithDots");
		
		AddItem("QuiltedJacket_Black");
		AddItem("QuiltedJacket_Green");
		AddItem("QuiltedJacket_Blue");
		AddItem("QuiltedJacket_Red");
		AddItem("QuiltedJacket_Grey");
		AddItem("QuiltedJacket_Orange");
		AddItem("QuiltedJacket_Yellow");
		AddItem("QuiltedJacket_Violet");
		
		AddItem("BomberJacket_Black");
		AddItem("BomberJacket_Brown");
		AddItem("BomberJacket_Blue");
		AddItem("BomberJacket_Grey");
		AddItem("BomberJacket_Maroon");
		AddItem("BomberJacket_Olive");
		AddItem("BomberJacket_SkyBlue");
		
		//AddItem("LeatherJacket_Natural");
		//AddItem("LeatherJacket_Beige");
		//AddItem("LeatherJacket_Brown");
		//AddItem("LeatherJacket_Black");
		
		AddItem("HuntingJacket_Autumn");
		AddItem("HuntingJacket_Brown");
		AddItem("HuntingJacket_Spring");
		AddItem("HuntingJacket_Summer");
		AddItem("HuntingJacket_Winter");
		
		AddItem("MedicalScrubsShirt_Blue");
		AddItem("MedicalScrubsShirt_Green");
		AddItem("MedicalScrubsShirt_White");
		
		AddItem("LabCoat");
		
		AddItem("NurseDress_White");
		AddItem("NurseDress_Blue");
		
		AddItem("USMCJacket_Desert");
		AddItem("USMCJacket_Woodland");
		
		AddItem("Blouse_Blue");
		AddItem("Blouse_Green");
		AddItem("Blouse_Violet");
		AddItem("Blouse_White");
		
		AddItem("NBCJacketGray");
		
		AddItem("DenimJacket");
		
		AddItem("TelnyashkaShirt");
		
		AddItem("ChernarusSportShirt");
		
		AddItem("JumpsuitJacket_Blue");
		AddItem("JumpsuitJacket_Gray");
		AddItem("JumpsuitJacket_Green");
		AddItem("JumpsuitJacket_Red");
		
		AddItem("BDUJacket");
		
		AddItem("ManSuit_Beige");
		AddItem("ManSuit_Black");
		AddItem("ManSuit_Blue");
		AddItem("ManSuit_Brown");
		AddItem("ManSuit_DarkGrey");
		AddItem("ManSuit_Khaki");
		AddItem("ManSuit_LightGrey");
		AddItem("ManSuit_White");
		
		AddItem("WomanSuit_Beige");
		AddItem("WomanSuit_Black");
		AddItem("WomanSuit_Blue");
		AddItem("WomanSuit_Brown");
		AddItem("WomanSuit_DarkGrey");
		AddItem("WomanSuit_Khaki");
		AddItem("WomanSuit_LightGrey");
		AddItem("WomanSuit_White");
		
		//AddItem("LeatherShirt_Natural");
	}
}