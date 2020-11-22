/**
 * ExpansionMarketTraderLinda.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license); visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderLinda: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderLinda";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CONSUMABLES";
		m_FileName = "Consumables_Trader_Linda";
		
		// Vanilla Drinks
		AddItem("Canteen");
		AddItem("WaterBottle");
		AddItem("Vodka");
		AddItem("SodaCan_Pipsi");
		AddItem("SodaCan_Cola");
		AddItem("SodaCan_Spite");
		AddItem("SodaCan_Kvass");
		
		// Expansion Drinks
		AddItem("ExpansionMilkBottle");
		
		// Vanilla Food
		AddItem("BoxCerealCrunchin"); 
		AddItem("PowderedMilk"); 				
		AddItem("Rice"); 			
		AddItem("Marmalade"); 		
		AddItem("BakedBeansCan"); 	
		AddItem("PeachesCan"); 		
		AddItem("TacticalBaconCan"); 
		AddItem("SpaghettiCan"); 	
		AddItem("SardinesCan"); 		
		AddItem("TunaCan"); 	
				
		// Expansion Food
		AddItem("ExpansionBread1");
		AddItem("ExpansionBread2");
		AddItem("ExpansionBread3");
		AddItem("ExpansionCheese1" );
		AddItem("ExpansionCheese2");
		AddItem("ExpansionCheese3");
		AddItem("ExpansionCheese4");		
		
		// Vanilla Vegetables
		AddItem("Apple");			
		AddItem("Tomato"); 			
		AddItem("GreenBellPepper"); 	
		AddItem("Zucchini"); 		
		AddItem("Pumpkin"); 			
		AddItem("SlicedPumpkin"); 	
		AddItem("Potato");		
		AddItem("SambucusBerry"); 	
		AddItem("CaninaBerry");		
		AddItem("Plum"); 			
		AddItem("Pear"); 			
		AddItem("AgaricusMushroom"); 
		AddItem("AmanitaMushroom"); 	
		AddItem("MacrolepiotaMushroom");
		AddItem("LactariusMushroom"); 
		AddItem("PsilocybeMushroom"); 
		AddItem("AuriculariaMushroom");
		AddItem("BoletusMushroom"); 	
		AddItem("PleurotusMushroom"); 	
		
		// Vanilla Meat
		AddItem("BearSteakMeat");
		AddItem("GoatSteakMeat");
		AddItem("BoarSteakMeat");
		AddItem("PigSteakMeat");
		AddItem("DeerSteakMeat");
		AddItem("WolfSteakMeat");
		AddItem("BearSteakMeat");
		AddItem("CowSteakMeat");
		AddItem("SheepSteakMeat");
		AddItem("ChickenBreastMeat");
		AddItem("RabbitLegMeat");
	}
}