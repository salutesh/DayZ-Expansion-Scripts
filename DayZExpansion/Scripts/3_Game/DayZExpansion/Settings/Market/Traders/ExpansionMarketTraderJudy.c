/**
 * ExpansionMarketTraderJudy.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderJudy: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderJudy";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_AIRCRAFT";
		m_FileName = "Aircraft_Trader_Judy";
		
		AddItem( "ExpansionMerlin" );
		AddItem( "ExpansionUh1h" );
		AddItem( "ExpansionMh6" );

		AddItem( "ExpansionSpraycanMerlinDefault" );
		AddItem( "ExpansionSpraycanMerlinDahoman" );
		AddItem( "ExpansionSpraycanMerlinIon" );
		AddItem( "ExpansionSpraycanMerlinPresident" );
		AddItem( "ExpansionSpraycanMerlinVrana" );
		AddItem( "ExpansionSpraycanMerlinWave" );

		AddItem( "ExpansionSpraycanUh1hDefault" );
		AddItem( "ExpansionSpraycanUh1hCivilian" );
		AddItem( "ExpansionSpraycanUh1hMedical" );
		AddItem( "ExpansionSpraycanUh1hUber" );

		AddItem( "ExpansionSpraycanMh6Default" );
		AddItem( "ExpansionSpraycanMh6Blueline" );
		AddItem( "ExpansionSpraycanMh6Digital" );
		AddItem( "ExpansionSpraycanMh6Elliptical" );
		AddItem( "ExpansionSpraycanMh6Furious" );
		AddItem( "ExpansionSpraycanMh6GrayWatcher" );
		AddItem( "ExpansionSpraycanMh6Jeans" );
		AddItem( "ExpansionSpraycanMh6Shadow" );
		AddItem( "ExpansionSpraycanMh6Speedy" );
		AddItem( "ExpansionSpraycanMh6Sunset" );
		AddItem( "ExpansionSpraycanMh6Vrana" );
		AddItem( "ExpansionSpraycanMh6Wasp" );
		AddItem( "ExpansionSpraycanMh6Wave" );
	}
}