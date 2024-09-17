/**
 * ExpansionMarketHandguards.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHandguards: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 27;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_HANDGUARDS";
		m_FileName = "Handguards";
			
		AddItem("mp5_plastichndgrd", 	1805, 	3010,	1,100);
		AddItem("mp5_railhndgrd", 	1805, 	3010,	1,100);

		AddItem("ak74_hndgrd", 	1790, 	2985,	1,100);

		AddItem("ak_woodhndgrd", 	1790, 	2985,	1,100);
		AddItem("ak_railhndgrd", 	1790, 	2985,	1,100);
		AddItem("ak_plastichndgrd", 	1790, 	2985,	1,100);
		
		AddItem("m4_plastichndgrd", 	1790, 	2985,	1,100);
		AddItem("m4_rishndgrd", 	1790, 	2985,	1,100);
		AddItem("m4_mphndgrd", 	1790, 	2985,	1,100);
	}
};