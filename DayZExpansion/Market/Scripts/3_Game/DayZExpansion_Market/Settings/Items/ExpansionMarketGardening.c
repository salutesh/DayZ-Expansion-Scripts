/**
 * ExpansionMarketGardening.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketGardening: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 25;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_GARDENING";
		m_FileName = "Gardening";
		
		AddItem("gardenlime", 	355, 	595,	1,100,null,{});
		AddItem("pepperseedspack", 	350, 	585,	1,100);
		AddItem("tomatoseedspack", 	350, 	585,	1,100);
		AddItem("zucchiniseedspack", 	350, 	585,	1,100);
		AddItem("pumpkinseedspack", 	350, 	585,	1,100);
	}
};