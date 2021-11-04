/**
 * ExpansionMarketBandanas.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBandanas: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 6;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BANDANAS";
		m_FileName = "Bandanas";

		AddItem("Bandana_Blue", 					6,		12,		1,		100);
		AddItem("Bandana_Pink", 					6,		12,		1,		100);
		AddItem("Bandana_Yellow", 					6,		12,		1,		100);
		AddItem("Bandana_RedPattern", 				6,		12,		1,		100);
		AddItem("Bandana_BlackPattern", 			6,		12,		1,		100);
		AddItem("Bandana_PolkaPattern",				6,		12,		1,		100);
		AddItem("Bandana_Greenpattern", 			8,		16,		1,		100);
		AddItem("Bandana_CamoPattern", 				10,		20,		1,		100);
	}
};