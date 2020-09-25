/**
 * ExpansionMarketExchange.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketExchange: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 0;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_EXCHANGE";
		m_FileName = "Exchange";

		AddStaticItem("ExpansionGoldBar",			10000	);
		AddStaticItem("ExpansionGoldNugget",		100		);
		AddStaticItem("ExpansionSilverBar",			10		);
		AddStaticItem("ExpansionSilverNugget",		1		);
	}
};