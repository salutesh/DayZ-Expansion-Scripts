/**
 * ExpansionMarketKits.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketKits: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 31;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_KITS";
		m_FileName = "Kits";
		
		AddItem("sewingkit", 	260, 	430,	1,100);
		AddItem("leathersewingkit", 	310, 	520,	1,100);
		AddItem("weaponcleaningkit", 	1010, 	1680,	1,100);
	}
};