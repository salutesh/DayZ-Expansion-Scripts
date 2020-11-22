/**
 * ExpansionMarketTraderLina.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderLina: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderLina";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_FISHING";
		m_FileName = "Fishing_Trader_Lina";

		// Fishing Items
		AddItem("Hook");
		AddItem("BoneHook");
		AddItem("FishingRod");
		
		// Fish
		AddItem("Carp");
		AddItem("Sardines");
		AddItem("Mackerel");
	}
}