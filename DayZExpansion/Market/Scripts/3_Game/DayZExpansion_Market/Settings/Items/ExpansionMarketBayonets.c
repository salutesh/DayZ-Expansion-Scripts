/**
 * ExpansionMarketBayonets.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBayonets: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 64;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BAYONETS";
		m_FileName = "Bayonets";
		
		AddItem("Mosin_Bayonet", 				100,	200,	1,		100);
		AddItem("SKS_Bayonet", 					100,	200,	1,		100);
		AddItem("M9A1_Bayonet", 				150,	300,	1,		100);
		AddItem("AK_Bayonet", 					150,	300,	1,		100);

	#ifdef EXPANSIONMOD
		AddItem("Expansion_Kar98_Bayonet", 		100,	200,	1,		100);
	#endif
	}
};