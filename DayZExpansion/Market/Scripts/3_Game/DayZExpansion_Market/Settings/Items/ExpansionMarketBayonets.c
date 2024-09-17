/**
 * ExpansionMarketBayonets.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		
		AddItem("mosin_bayonet", 	1390, 	2315,	1,100);
		AddItem("sks_bayonet", 	3590, 	5985,	1,100);
		AddItem("m9a1_bayonet", 	3150, 	5245,	1,100);
		AddItem("ak_bayonet", 	3150, 	5245,	1,100);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Kar98_Bayonet", 		1390, 	2315,	1,		100);
	#endif
	}
};