/**
 * ExpansionMarketTraderIrena.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderIrena: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderIrena";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_AIRCRAFT";
		m_FileName = "Aircraft_Trader_Irena";
		
		AddItem( "ExpansionMerlin" );
		AddItem( "ExpansionUh1h" );
		AddItem( "ExpansionMh6" );
		AddItem( "ExpansionGyrocopter" );
	}
}