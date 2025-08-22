/**
 * ExpansionMarketSkirtsAndDresses.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSkirtsAndDresses: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_SKIRTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_DRESSES");
		m_FileName = "Skirts_And_Dresses";
		
		AddItem("skirt_blue", 	630, 	1050,	1,100,null,{"Skirt_Red","Skirt_White","Skirt_Yellow"});
		
		AddItem("minidress_pink", 	200, 	335,	1,100,null,{"MiniDress_PinkChecker","MiniDress_RedChecker","MiniDress_BlueChecker","MiniDress_BlueWithDots","MiniDress_WhiteChecker","MiniDress_BrownChecker","MiniDress_GreenChecker"});
		
		AddItem("nursedress_blue", 	645, 	1075,	1,100,null,{"NurseDress_White"});
	}
};