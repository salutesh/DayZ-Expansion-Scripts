/**
 * ExpansionMarketTraderClothingAccessoires.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderClothingAccessoires: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Clothing_Accessoires";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CLOTHING_ACCESSORIES";
		m_FileName = "Clothing_Accessoires";
		TraderIcon = "Backpack";
		
		// HELMETS
		AddItem("ConstructionHelmet_Blue");
		//AddItem("ConstructionHelmet_Orange");
		//AddItem("ConstructionHelmet_Red");
		//AddItem("ConstructionHelmet_White");
		//AddItem("ConstructionHelmet_Yellow");
		//AddItem("ConstructionHelmet_Lime");
		
		AddItem("SkateHelmet_Blue");
		//AddItem("SkateHelmet_Gray");
		//AddItem("SkateHelmet_Red");
		//AddItem("SkateHelmet_Black");
		//AddItem("SkateHelmet_Green");

		AddItem("HockeyHelmet_Blue");
		//AddItem("HockeyHelmet_Red");
		//AddItem("HockeyHelmet_White");
		//AddItem("HockeyHelmet_Black");

		AddItem("DirtBikeHelmet_Mouthguard");
		//AddItem("DirtBikeHelmet_Visor");
		//AddItem("DirtBikeHelmet_Chernarus");
		//AddItem("DirtBikeHelmet_Police");
		//AddItem("DirtBikeHelmet_Red");
		//AddItem("DirtBikeHelmet_Green");
		//AddItem("DirtBikeHelmet_Blue");
		//AddItem("DirtBikeHelmet_Black");
		//AddItem("DirtBikeHelmet_Khaki");

		AddItem("MotoHelmet_Lime");
		//AddItem("MotoHelmet_Blue");
		//AddItem("MotoHelmet_Red");
		//AddItem("MotoHelmet_White");
		//AddItem("MotoHelmet_Grey");
		//AddItem("MotoHelmet_Black");
		//AddItem("MotoHelmet_Green");

		AddItem("DarkMotoHelmet_Grey");
		//AddItem("DarkMotoHelmet_Lime");
		//AddItem("DarkMotoHelmet_Blue");
		//AddItem("DarkMotoHelmet_Red");
		//AddItem("DarkMotoHelmet_White");
		//AddItem("DarkMotoHelmet_Black");
		//AddItem("DarkMotoHelmet_Green");

		AddItem("TankerHelmet");
		
		AddItem("GreatHelm");
	
		AddItem("ZSh3PilotHelmet");
		//AddItem("ZSh3PilotHelmet_Green");
		//AddItem("ZSh3PilotHelmet_Black");
			
		AddItem("FirefightersHelmet_Red");
		//AddItem("FirefightersHelmet_White");
		//AddItem("FirefightersHelmet_Yellow");
				
		AddItem("Mich2001Helmet");
	#ifdef EXPANSIONMOD
		AddItem("ExpansionMich2001Desert");
	#endif

		AddItem("GorkaHelmetVisor");
		AddItem("GorkaHelmet");
		//AddItem("GorkaHelmet_Black");

		AddItem("Ssh68Helmet");

		AddItem("BallisticHelmet_UN");
		//AddItem("BallisticHelmet_Black");
		//AddItem("BallisticHelmet_Green");

		// CAPS
		AddItem("BaseballCap_CMMG_Pink");
		//AddItem("BaseballCap_Pink");
		//AddItem("BaseballCap_Blue");
		//AddItem("BaseballCap_Beige");
		//AddItem("BaseballCap_Red");
		//AddItem("BaseballCap_CMMG_Black");
		//AddItem("BaseballCap_Black");
		//AddItem("BaseballCap_Olive");
		//AddItem("BaseballCap_Camo");
		
		AddItem("PrisonerCap");

		AddItem("PilotkaCap");

		AddItem("PoliceCap");

		AddItem("FlatCap_Blue");
		//AddItem("FlatCap_Red");
		//AddItem("FlatCap_Brown");
		//AddItem("FlatCap_Grey");
		//AddItem("FlatCap_BrownCheck");
		//AddItem("FlatCap_GreyCheck");
		//AddItem("FlatCap_Black");
		//AddItem("FlatCap_BlackCheck");

		AddItem("ZmijovkaCap_Blue");
		//AddItem("ZmijovkaCap_Brown");
		//AddItem("ZmijovkaCap_Red");
		//AddItem("ZmijovkaCap_Black");
		//AddItem("ZmijovkaCap_Green");

		AddItem("RadarCap_Blue");
		//AddItem("RadarCap_Brown");
		//AddItem("RadarCap_Red");
		//AddItem("RadarCap_Black");
		//AddItem("RadarCap_Green");
	
		// HATS AND HOODS
		AddItem("MedicalScrubsHat_Blue");
		//AddItem("MedicalScrubsHat_White");
		//AddItem("MedicalScrubsHat_Green");

		AddItem("CowboyHat_Brown");
		//AddItem("CowboyHat_black");
		//AddItem("CowboyHat_darkBrown");
		//AddItem("CowboyHat_green");
	//#ifdef EXPANSIONMOD
		//AddItem("ExpansionCowboyHatGator");
		//AddItem("ExpansionCowboyHatSnake");
		//AddItem("ExpansionCowboyHatRattleSnake");
	//#endif

		AddItem("MilitaryBeret_ChDKZ");
		//AddItem("MilitaryBeret_Red");
		//AddItem("MilitaryBeret_UN");
		//AddItem("MilitaryBeret_CDF");
		//AddItem("MilitaryBeret_NZ");

		AddItem("BeanieHat_Pink");	
		//AddItem("BeanieHat_Beige");
		//AddItem("BeanieHat_Blue");
		//AddItem("BeanieHat_Brown");
		//AddItem("BeanieHat_Grey");
		//AddItem("BeanieHat_Red");
		//AddItem("BeanieHat_Black");
		//AddItem("BeanieHat_Green");
		
		AddItem("Ushanka_Black");
		//AddItem("Ushanka_Blue");
		//AddItem("Ushanka_Green");
		
		AddItem("BoonieHat_Blue");
		//AddItem("BoonieHat_NavyBlue");
		//AddItem("BoonieHat_Orange");
		//AddItem("BoonieHat_Red");
		//AddItem("BoonieHat_Black");
		//AddItem("BoonieHat_Tan");
		//AddItem("BoonieHat_Olive");
		//AddItem("BoonieHat_DPM");
		//AddItem("BoonieHat_Dubok");
		//AddItem("BoonieHat_Flecktran");

		AddItem("OfficerHat");
		
		AddItem("NBCHoodGray");
		AddItem("NBCHoodYellow");
		
		// ARMBANDS
		AddItem("Armband_White");
		//AddItem("Armband_Yellow");
		//AddItem("Armband_Orange");
		//AddItem("Armband_Red");
		//AddItem("Armband_Green");
		//AddItem("Armband_Pink");
		//AddItem("Armband_Blue");
		//AddItem("Armband_Black");

		// BACKPACKS
		AddItem("ChildBag_Red");
		//AddItem("ChildBag_Blue");
		//AddItem("ChildBag_Green");
		
		AddItem("DryBag_Orange");
		//AddItem("DryBag_Yellow");
		//AddItem("DryBag_Blue");
		//AddItem("DryBag_Black");
		//AddItem("DryBag_Red");
		//AddItem("DryBag_Green");
		
		AddItem("TaloonBag_Blue");
		//AddItem("TaloonBag_Orange");
		//AddItem("TaloonBag_Violet");
		//AddItem("TaloonBag_Green");
	
		AddItem("SmershBag");
		
		AddItem("AssaultBag_Black");
		//AddItem("AssaultBag_Green");
		//AddItem("AssaultBag_Ttsko");
		
		AddItem("HuntingBag");
		
		AddItem("TortillaBag");
	//#ifdef EXPANSIONMOD
		//AddItem("ExpansionDesertTortilla");
	//#endif

		AddItem("CoyoteBag_Brown");
		//AddItem("CoyoteBag_Green");
	//#ifdef EXPANSIONMOD
		//AddItem("ExpansionCoyoteBlack");
	//#endif	

		AddItem("MountainBag_Red");
		//AddItem("MountainBag_Blue");
		//AddItem("MountainBag_Orange");
		//AddItem("MountainBag_Green");
		
		AddItem("AliceBag_Green");
		//AddItem("AliceBag_Black");
		//AddItem("AliceBag_Camo");
		
		// EYEWEAR
		AddItem("SportGlasses_Orange");
		//AddItem("SportGlasses_Blue");
		//AddItem("SportGlasses_Black");
		//AddItem("SportGlasses_Green");
		
		AddItem("ThinFramesGlasses");
		AddItem("ThickFramesGlasses");
		
		AddItem("DesignerGlasses");

		AddItem("AviatorGlasses");
	
		AddItem("TacticalGoggles");
		
	#ifdef EXPANSIONMOD		
		//! Scrapped
		//AddItem("ExpansionMilGoggles");
	#endif

		// GLOVES
		AddItem("SurgicalGloves_Blue");
		//AddItem("SurgicalGloves_LightBlue");
		//AddItem("SurgicalGloves_Green");
		//AddItem("SurgicalGloves_White");
		
		AddItem("WorkingGloves_Yellow");
		//AddItem("WorkingGloves_Black");
		//AddItem("WorkingGloves_Beige");
		//AddItem("WorkingGloves_Brown");
	
		AddItem("TacticalGloves_Black");
		//AddItem("TacticalGloves_Beige");
		//AddItem("TacticalGloves_Green");
	//#ifdef EXPANSIONMOD
		//AddItem("ExpansionTacticalGlovesDesert");
	//#endif

		AddItem("OMNOGloves_Gray");
		//AddItem("OMNOGloves_Brown");
		
		AddItem("NBCGlovesGray");
		AddItem("NBCGlovesYellow");
		
		// BANDANAS
		AddItem("Bandana_Blue");
		//AddItem("Bandana_Pink");
		//AddItem("Bandana_Yellow");
		//AddItem("Bandana_RedPattern");
		//AddItem("Bandana_BlackPattern");
		//AddItem("Bandana_PolkaPattern");
		//AddItem("Bandana_Greenpattern");
		//AddItem("Bandana_CamoPattern");
		
		// MASKS
		AddItem("SurgicalMask");
				
		AddItem("NioshFaceMask");
		
		AddItem("HockeyMask");		

		AddItem("BalaclavaMask_Beige");
		//AddItem("BalaclavaMask_Blue");
		//AddItem("BalaclavaMask_Pink");
		//AddItem("BalaclavaMask_White");
		//AddItem("BalaclavaMask_Blackskull");
		//AddItem("BalaclavaMask_Black");
		//AddItem("BalaclavaMask_Green");
				
		AddItem("Balaclava3Holes_Beige");
		//AddItem("Balaclava3Holes_Blue");
		//AddItem("Balaclava3Holes_Black");
		//AddItem("Balaclava3Holes_Green");
		
		AddItem("WeldingMask");
		
		AddItem("GasMask");	
		AddItem("GP5GasMask");
		AddItem("AirborneMask");
		
		// VESTS
		AddItem("ReflexVest");
		
		AddItem("PoliceVest");
		
		AddItem("HuntingVest");
		
		AddItem("PressVest_Blue");
		//AddItem("PressVest_LightBlue");
		
		AddItem("UKAssVest_Black");
		//AddItem("UKAssVest_Khaki");
		//AddItem("UKAssVest_Olive");
		//AddItem("UKAssVest_Camo");

		AddItem("SmershVest");
			
		AddItem("HighCapacityVest_Black");
		//AddItem("HighCapacityVest_Olive");
		
		AddItem("PlateCarrierVest");
		
		// HOLSTERS AND POUCHES
		AddItem("PlateCarrierHolster");
		AddItem("PlateCarrierPouches");
		AddItem("ChestHolster");
		
		// GHILLIES
		AddItem("GhillieHood_Tan");
		//AddItem("GhillieHood_Woodland");
		//AddItem("GhillieHood_Mossy");
		
		AddItem("GhillieBushrag_Tan");
		//AddItem("GhillieBushrag_Woodland");
		//AddItem("GhillieBushrag_Mossy");
		
		AddItem("GhillieTop_Tan");
		//AddItem("GhillieTop_Woodland");
		//AddItem("GhillieTop_Mossy");
		
		AddItem("GhillieSuit_Tan");
		//AddItem("GhillieSuit_Woodland");
		//AddItem("GhillieSuit_Mossy");
		
		// BELTS
		AddItem("CivilianBelt");
		AddItem("MilitaryBelt");
	}
}