/**
 * ExpansionMarketAssaultRifles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCrossbows: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 59;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_CROSSBOWS";
		m_FileName = "Crossbows";
	
	#ifdef EXPANSIONMOD
		AddItem("Expansion_Crossbow", 			800,		1600,		1,		100,	0);
	#endif
	}
};