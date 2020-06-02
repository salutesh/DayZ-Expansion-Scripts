/**
 * ExpansionMarketFish.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketFish: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 22;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FISH";
		m_FileName = "Fish";

		AddItem("CarpFilletMeat", 				20,			25,			1,		250,	0 );
		AddItem("MackerelFilletMeat", 			20,			25,			1,		250,	0 );
		AddItem("Carp", 						50,			60,			1,		250,	0 );
		AddItem("Sardines", 					50,			60,			1,		250,	0 );
		AddItem("Mackerel", 					50,			60,			1,		250,	0 );
	}
}

