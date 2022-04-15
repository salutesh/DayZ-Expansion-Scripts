/**
 * ExpansionMarketTraderAttachments.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderAttachments: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Attachments";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_ATTACHMENTS";
		m_FileName = "Attachments";
		TraderIcon = "Scrap Metal";
		
		Categories.Insert("Magazines");
		Categories.Insert("Muzzles");
		Categories.Insert("Bayonets");
		Categories.Insert("Optics");
		Categories.Insert("Buttstocks");
		Categories.Insert("Handguards");
		
		// LIGHTS
		AddItem("UniversalLight");
		AddItem("TLRLight");
	}
}