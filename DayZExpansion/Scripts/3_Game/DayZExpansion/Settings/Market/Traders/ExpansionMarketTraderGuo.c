/**
 * ExpansionMarketTraderGuo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderGuo: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderGuo";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CLOTHING_ACCESSORIES";
		m_FileName = "Clothing_Accessoires_Trader_Guo";
		
		// Caps
		AddItem("BaseballCap_Blue");
		AddItem("BaseballCap_Beige");
		AddItem("BaseballCap_Black");
		AddItem("BaseballCap_Olive");
		AddItem("BaseballCap_Pink");
		AddItem("BaseballCap_Red");
		AddItem("BaseballCap_Camo");
		AddItem("BaseballCap_CMMG_Black");
		AddItem("BaseballCap_CMMG_Pink");
		
		AddItem("RadarCap_Black");
		AddItem("RadarCap_Blue");
		AddItem("RadarCap_Brown");
		AddItem("RadarCap_Green");
		AddItem("RadarCap_Red");
		
		AddItem("FlatCap_Black");
		AddItem("FlatCap_Blue");
		AddItem("FlatCap_Red");
		AddItem("FlatCap_Brown");
		AddItem("FlatCap_Grey");
		AddItem("FlatCap_BrownCheck");
		AddItem("FlatCap_GreyCheck");
		AddItem("FlatCap_BlackCheck");
		
		AddItem("ZmijovkaCap_Black");
		AddItem("ZmijovkaCap_Blue");
		AddItem("ZmijovkaCap_Brown");
		AddItem("ZmijovkaCap_Green");
		AddItem("ZmijovkaCap_Red");
		
		AddItem("PoliceCap");
		
		AddItem("PilotkaCap");
		
		AddItem("PrisonerCap");
		
		// Hats & Hoods
		AddItem("BeanieHat_Beige");
		AddItem("BeanieHat_Black");
		AddItem("BeanieHat_Blue");
		AddItem("BeanieHat_Brown");
		AddItem("BeanieHat_Green");
		AddItem("BeanieHat_Grey");
		AddItem("BeanieHat_Pink");
		AddItem("BeanieHat_Red");
		
		AddItem("Ushanka_Black");
		AddItem("Ushanka_Blue");
		AddItem("Ushanka_Green");
		
		AddItem("MilitaryBeret_Red");
		AddItem("MilitaryBeret_UN");
		AddItem("MilitaryBeret_NZ");
		AddItem("MilitaryBeret_ChDKZ");
		AddItem("MilitaryBeret_CDF");
		
		AddItem("CowboyHat_Brown");
		AddItem("CowboyHat_black");
		AddItem("CowboyHat_darkBrown");
		AddItem("CowboyHat_green");
		
		AddItem("BoonieHat_Black");
		AddItem("BoonieHat_Blue");
		AddItem("BoonieHat_DPM");
		AddItem("BoonieHat_Dubok");
		AddItem("BoonieHat_Flecktran");
		AddItem("BoonieHat_NavyBlue");
		AddItem("BoonieHat_Olive");
		AddItem("BoonieHat_Orange");
		AddItem("BoonieHat_Red");
		AddItem("BoonieHat_Tan");
		
		AddItem("OfficerHat");
		
		AddItem("WeldingMask");

		AddItem("GhillieHood_Tan");
		
		AddItem("GhillieHood_Woodland");
		AddItem("GhillieHood_Mossy");
		
		AddItem("MedicalScrubsHat_Blue");
		AddItem("MedicalScrubsHat_White");
		AddItem("MedicalScrubsHat_Green");
		
		AddItem("NBCHoodGray");
		
		// Armbands
		AddItem("Armband_White");
		AddItem("Armband_Yellow");
		AddItem("Armband_Orange");
		AddItem("Armband_Red");
		AddItem("Armband_Green");
		AddItem("Armband_Pink");
		AddItem("Armband_Blue");
		AddItem("Armband_Black");
		
		// Backpacks
		AddItem("TaloonBag_Blue");
		AddItem("TaloonBag_Green");
		AddItem("TaloonBag_Orange");
		AddItem("TaloonBag_Violet");
		AddItem("TortillaBag");
		AddItem("DryBag_Orange");
		AddItem("DryBag_Yellow");
		AddItem("DryBag_Blue");
		AddItem("DryBag_Green");
		AddItem("DryBag_Black");
		AddItem("DryBag_Red");
		AddItem("HuntingBag");
		AddItem("MountainBag_Red");
		AddItem("MountainBag_Blue");
		AddItem("MountainBag_Orange");
		AddItem("MountainBag_Green");
		AddItem("SmershBag");
		AddItem("ChildBag_Green");
		AddItem("ChildBag_Red");
		AddItem("ChildBag_Blue");
		AddItem("AssaultBag_Ttsko");
		AddItem("AssaultBag_Black");
		AddItem("AssaultBag_Green");
		AddItem("CoyoteBag_Brown");
		AddItem("CoyoteBag_Green");
		AddItem("AliceBag_Green");
		AddItem("AliceBag_Black");
		AddItem("AliceBag_Camo");
		
		// Eyewear
		AddItem("SportGlasses_Black");
		AddItem("SportGlasses_Blue");
		AddItem("SportGlasses_Green");
		AddItem("SportGlasses_Orange");
		AddItem("AviatorGlasses");
		AddItem("DesignerGlasses");
		AddItem("ThickFramesGlasses");
		AddItem("ThinFramesGlasses");
		AddItem("TacticalGoggles");
		AddItem("ExpansionMilGoggles");
		
		// Gloves
		AddItem("WorkingGloves_Black");
		AddItem("WorkingGloves_Beige");
		AddItem("WorkingGloves_Brown");
		AddItem("WorkingGloves_Yellow");
		AddItem("TacticalGloves_Black");
		AddItem("TacticalGloves_Beige");
		AddItem("TacticalGloves_Green");
		AddItem("NBCGlovesGray");
		AddItem("SurgicalGloves_Blue");
		AddItem("SurgicalGloves_LightBlue");
		AddItem("SurgicalGloves_Green");
		AddItem("SurgicalGloves_White");
		AddItem("OMNOGloves_Gray");
		AddItem("OMNOGloves_Brown");
		
		// Masks
		AddItem("GasMask");
		AddItem("BandanaMask_RedPattern");
		AddItem("BandanaMask_BlackPattern");
		AddItem("BandanaMask_PolkaPattern");
		AddItem("BandanaMask_GreenPattern");
		AddItem("BandanaMask_CamoPattern");
		AddItem("NioshFaceMask");
		AddItem("BalaclavaMask_Black");
		AddItem("BalaclavaMask_Blackskull");
		AddItem("BalaclavaMask_Beige");
		AddItem("BalaclavaMask_Blue");
		AddItem("BalaclavaMask_Green");
		AddItem("BalaclavaMask_Pink");
		AddItem("BalaclavaMask_White");
		AddItem("AirborneMask");
		AddItem("GP5GasMask");
		AddItem("Balaclava3Holes_Beige");
		AddItem("Balaclava3Holes_Blue");
		AddItem("Balaclava3Holes_Black");
		AddItem("Balaclava3Holes_Green");
		AddItem("SurgicalMask");
		AddItem("HockeyMask");
		
		// Shoes
		AddItem("AthleticShoes_Blue");
		AddItem("AthleticShoes_Black");
		AddItem("AthleticShoes_Brown");
		AddItem("AthleticShoes_Green");
		AddItem("AthleticShoes_Grey");
		AddItem("HikingBoots_Brown");
		AddItem("HikingBoots_Black");
		AddItem("HikingBootsLow_Black");
		AddItem("HikingBootsLow_Blue");
		AddItem("HikingBootsLow_Beige");
		AddItem("HikingBootsLow_Grey");
		AddItem("Wellies_Black");
		AddItem("Wellies_Brown");
		AddItem("Wellies_Green");
		AddItem("Wellies_Grey");
		AddItem("WorkingBoots_Grey");
		AddItem("WorkingBoots_Brown");
		AddItem("WorkingBoots_Green");
		AddItem("WorkingBoots_Yellow");
		AddItem("WorkingBoots_Beige");
		AddItem("JungleBoots_Beige");
		AddItem("JungleBoots_Black");
		AddItem("JungleBoots_Brown");
		AddItem("JungleBoots_Green");
		AddItem("JungleBoots_Olive");
		AddItem("DressShoes_Beige");
		AddItem("DressShoes_Black");
		AddItem("DressShoes_Brown");
		AddItem("DressShoes_Sunburst");
		AddItem("DressShoes_White");
		AddItem("MilitaryBoots_Beige");
		AddItem("MilitaryBoots_Black");
		AddItem("MilitaryBoots_Bluerock");
		AddItem("MilitaryBoots_Brown");
		AddItem("MilitaryBoots_Redpunk");
		AddItem("CombatBoots_Beige");
		AddItem("CombatBoots_Black");
		AddItem("CombatBoots_Brown");
		AddItem("CombatBoots_Green");
		AddItem("CombatBoots_Grey");
		AddItem("JoggingShoes_Black");
		AddItem("JoggingShoes_Blue");
		AddItem("JoggingShoes_Red");
		AddItem("JoggingShoes_Violet");
		AddItem("JoggingShoes_White");
		AddItem("Sneakers_Black");
		AddItem("Sneakers_Gray");
		AddItem("Sneakers_Green");
		AddItem("Sneakers_Red");
		AddItem("Sneakers_White");
		AddItem("NBCBootsGray");
		AddItem("Ballerinas_Blue");
		AddItem("Ballerinas_Red");
		AddItem("Ballerinas_White");
		AddItem("Ballerinas_Yellow");
		AddItem("TTSKOBoots");
		
		// Vests
		AddItem("PlateCarrierVest");
		AddItem("SmershVest");
		AddItem("PressVest_Blue");
		AddItem("PressVest_LightBlue");
		AddItem("UKAssVest_Black");
		AddItem("UKAssVest_Camo");
		AddItem("UKAssVest_Khaki");
		AddItem("UKAssVest_Olive");
		AddItem("PoliceVest");
		AddItem("PlateCarrierHolster");
		AddItem("ChestHolster");
		AddItem("HighCapacityVest_Black");
		AddItem("HighCapacityVest_Olive");
		AddItem("HuntingVest");
		AddItem("ReflexVest");
		
		// Ghillies
		AddItem("GhillieBushrag_Tan" );
		AddItem("GhillieBushrag_Woodland");
		AddItem("GhillieBushrag_Mossy");
		AddItem("GhillieTop_Tan");
		AddItem("GhillieTop_Woodland");
		AddItem("GhillieTop_Mossy");
		AddItem("GhillieSuit_Tan");
		AddItem("GhillieSuit_Woodland");
		AddItem("GhillieSuit_Mossy");
		
		// Belts
		AddItem("CivilianBelt");
		AddItem("MilitaryBelt");
	}
}