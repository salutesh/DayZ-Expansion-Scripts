/**
 * ExpansionMarketTraderMedicals.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderMedicals: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Medicals";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_MEDICALS";
		m_FileName = "Medicals";
		TraderIcon = "Medic Box";
		
		AddItem("CharcoalTablets");
		AddItem("BandageDressing");
		AddItem("DisinfectantAlcohol");
		AddItem("PurificationTablets");
		AddItem("BloodTestKit");
		//AddItem("InjectionVial");
		AddItem("Thermometer");
		AddItem("VitaminBottle");
		AddItem("DisinfectantSpray");
		AddItem("TetracyclineAntibiotics");
		AddItem("PainkillerTablets");
		//AddItem("Rag");
		//AddItem("Syringe");
		AddItem("StartKitIV");
		AddItem("Heatpack");
		AddItem("SalineBag");
		AddItem("BloodBagEmpty");
		AddItem("FirstAidKit");
		AddItem("Epinephrine");
		AddItem("Morphine");
		//AddItem("Splint");
		AddItem("IodineTincture");
		//AddItem("Defibrillator");
		AddItem("AntiChemInjector");
	}
}