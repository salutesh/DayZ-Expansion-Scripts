/**
 * ExpansionMarketFurnishings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketFurnishings: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 65;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FURNISHINGS";
		m_FileName = "Furnishings";
		
	#ifdef EXPANSIONMOD
		AddItem("ExpansionGunrack", 		3500,	7000,	1,		100,	0);
		AddItem("ExpansionStove", 			1500,	3000,	1,		100,	0);
	#endif
	}
};