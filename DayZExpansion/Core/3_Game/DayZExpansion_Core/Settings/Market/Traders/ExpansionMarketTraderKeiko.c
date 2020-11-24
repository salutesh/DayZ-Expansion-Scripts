/**
 * ExpansionMarketTraderKeiko.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderKeiko: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderKeiko";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_MEDICALS";
		m_FileName = "Medicals_Trader_Keiko";
		
		AddItem("AntiPestsSpray");
		//AddItem("Rag");
		AddItem("BandageDressing");
		AddItem("BloodTestKit");
		//AddItem("InjectionVial");
		AddItem("Thermometer");
		AddItem("DisinfectantSpray");
		//AddItem("Syringe");
		AddItem("StartKitIV");
		AddItem("SalineBag");
		AddItem("BloodBagEmpty");
		AddItem("FirstAidKit");
		//AddItem("Defibrillator");
		AddItem("CharcoalTablets");
		AddItem("DisinfectantAlcohol");
		AddItem("VitaminBottle");
		AddItem("TetracyclineAntibiotics");
		AddItem("PainkillerTablets");
		AddItem("Epinephrine");
		AddItem("Morphine");
	}
}