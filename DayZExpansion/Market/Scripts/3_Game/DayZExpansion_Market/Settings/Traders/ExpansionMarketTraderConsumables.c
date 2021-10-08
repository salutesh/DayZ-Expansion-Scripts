/**
 * ExpansionMarketTraderConsumables.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderConsumables: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Consumables";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_CONSUMABLES";
		m_FileName = "Consumables";
		TraderIcon = "Can Of Beans Big";
		
		// DRINKS
		AddItem("SodaCan_Pipsi");
		AddItem("SodaCan_Cola");
		AddItem("SodaCan_Spite");
		AddItem("SodaCan_Kvass");
		AddItem("SodaCan_Fronta");
		AddItem("WaterBottle");
		AddItem("Canteen");
		AddItem("Vodka");
	#ifdef EXPANSIONMOD
		AddItem("ExpansionMilkBottle");
	#endif
		
		// FOOD
		AddItem("Zagorky");
		AddItem("ZagorkyChocolate");
		AddItem("ZagorkyPeanuts");

		AddItem("PowderedMilk");
		AddItem("BoxCerealCrunchin");
		AddItem("Rice");

		AddItem("Marmalade");
		AddItem("Honey");

		AddItem("SaltySticks");
		AddItem("Crackers");
		AddItem("Chips");

		AddItem("Pajka");
		AddItem("Pate");
		AddItem("BrisketSpread");

		AddItem("SardinesCan");
		AddItem("TunaCan");		
		AddItem("DogFoodCan");
		AddItem("CatFoodCan");
		AddItem("PorkCan");
		AddItem("Lunchmeat");

		AddItem("UnknownFoodCan");
		AddItem("PeachesCan");
		AddItem("SpaghettiCan");
		AddItem("BakedBeansCan");
		AddItem("TacticalBaconCan");
	
	#ifdef EXPANSIONMOD	
		AddItem("ExpansionBread1");
		AddItem("ExpansionBread2");
		AddItem("ExpansionBread3");
		AddItem("ExpansionCheese1");
		AddItem("ExpansionCheese2");
		AddItem("ExpansionCheese3");
		AddItem("ExpansionCheese4");
	#endif
		
		// Vanilla Vegetables
		AddItem("Apple");
		AddItem("GreenBellPepper");
		AddItem("Zucchini");
		AddItem("Pumpkin");
		AddItem("SlicedPumpkin");
		AddItem("PotatoSeed");
		AddItem("Potato");
		AddItem("Tomato");
		AddItem("SambucusBerry");
		AddItem("CaninaBerry");
		AddItem("Plum");
		AddItem("Pear");
		//AddItem("Kiwi");
		//AddItem("Orange");
		//AddItem("Banana");
				
		AddItem("AgaricusMushroom");
		AddItem("AmanitaMushroom");
		AddItem("MacrolepiotaMushroom");
		AddItem("LactariusMushroom");
		AddItem("PsilocybeMushroom");
		AddItem("AuriculariaMushroom");
		AddItem("BoletusMushroom");
		AddItem("PleurotusMushroom");
		
		// MEAT
		AddItem("BearSteakMeat");
		AddItem("GoatSteakMeat");
		AddItem("BoarSteakMeat");
		AddItem("PigSteakMeat");
		AddItem("DeerSteakMeat");
		AddItem("WolfSteakMeat");
		AddItem("CowSteakMeat");
		AddItem("SheepSteakMeat");
		AddItem("ChickenBreastMeat");
	#ifdef EXPANSIONMOD	
		AddItem("RabbitLegMeat");
	#endif

		// FISH
		AddItem("CarpFilletMeat");
		AddItem("MackerelFilletMeat");
	}
}