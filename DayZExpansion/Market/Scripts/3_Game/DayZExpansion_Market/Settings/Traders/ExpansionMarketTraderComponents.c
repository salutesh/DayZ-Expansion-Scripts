/**
 * ExpansionMarketTraderComponents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderComponents: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Components";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_COMPONENTS";
		m_FileName = "Components";
		TraderIcon = "Hammer";
		
		Categories.Insert("Tools");
		Categories.Insert("Supplies");
		Categories.Insert("Locks");
		Categories.Insert("Containers");
		Categories.Insert("Tents");
		Categories.Insert("Kits");
		Categories.Insert("Navigation");
		Categories.Insert("Electronics");
		Categories.Insert("Lights");

		//! Specify CanBuyAndSellAsAttachmentOnly to hide them from the "Electronics" category (these items are available in the "Special" trader)
		AddItem("Rangefinder", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("NVGoggles", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		//! Specify CanBuyAndSellAsAttachmentOnly to hide them from the "Lights" category (these items are available in the "Attachments" trader)
		AddItem("UniversalLight", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("TLRLight", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	}
}