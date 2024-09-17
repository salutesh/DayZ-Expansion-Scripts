/**
 * ExpansionMarketMuzzles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMuzzles: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 42;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MUZZLES";
		m_FileName = "Muzzles";
		
		// COMPENSATORS
		AddItem("mosin_compensator", 	1065, 	1775,	1,100);
		AddItem("mp5_compensator", 	1790, 	2985,	1,100);
		
		// SUPRESSORS
		AddItem("m4_suppressor", 	4855, 	8090,	1,100);
		AddItem("ak_suppressor", 	4855, 	8090,	1,100);
		AddItem("pistolsuppressor", 	2815, 	4690,	1,100);
	}
};