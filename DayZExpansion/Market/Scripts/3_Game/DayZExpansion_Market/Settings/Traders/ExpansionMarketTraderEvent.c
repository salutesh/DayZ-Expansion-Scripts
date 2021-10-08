/**
 * ExpansionMarketTraderEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderEvent: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Event";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_EVENT";
		m_FileName = "Event";
		TraderIcon = "Questionmark";
		
		AddItem("EasterEgg");
		AddItem("CrookedNose");
		AddItem("Cauldron");
		AddItem("WitchHat");
		AddItem("WitchHood_Black");
		//AddItem("WitchHood_Brown");
		//AddItem("WitchHood_Red");
		AddItem("Broom");
		AddItem("Broom_Birch");
	}
}