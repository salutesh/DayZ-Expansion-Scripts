/**
 * ExpansionMarketRifles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketRifles: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 47;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_RIFLES";
		m_FileName = "Rifles";
		
		// Rifles
		AddItem("Izh18", 					35,	37,	1,		250,	0 );
		AddItem("Mosin9130", 				40,	42,	1,		250,	0 );
		AddItem("CZ527", 					50,	52,	1,		250,	0 );
		AddItem("Winchester70", 			50,	52,	1,		250,	0 );
		AddItem("SKS", 						60,	62,	1,		250,	0 );
		
		AddItem("Expansion_DT11", 			50,	52,	1,		250,	0 );
	}
}