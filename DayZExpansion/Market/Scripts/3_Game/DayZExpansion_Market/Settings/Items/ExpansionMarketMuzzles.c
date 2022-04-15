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
		AddItem("Mosin_Compensator", 			150,	300,	1,		100);
		AddItem("MP5_Compensator", 				200,	400,	1,		100);
		
		// SUPRESSORS
		AddItem("M4_Suppressor", 				25,		27,		1,		100);
		AddItem("AK_Suppressor", 				25,		27,		1,		100);
		AddItem("PistolSuppressor", 			15,		17,		1,		100);
	}
};