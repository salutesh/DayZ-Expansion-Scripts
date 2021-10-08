/**
 * ExpansionMarketSkirtsAndDresses.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

		CategoryID = 50;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_SKIRTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_DRESSES");
		m_FileName = "Skirts_And_Dresses";
		
		AddItem("Skirt_Blue", 						30,		60,		1,		100,	0, null, {"Skirt_Red", "Skirt_White", "Skirt_Yellow"});
		/*AddItem("Skirt_Red", 						30,		60,		1,		100,	0);
		AddItem("Skirt_White", 						30,		60,		1,		100,	0);
		AddItem("Skirt_Yellow", 					30,		60,		1,		100,	0);*/
		
		AddItem("MiniDress_Pink", 					35,		70,		1,		100,	0, null, {"MiniDress_PinkChecker", "MiniDress_RedChecker", "MiniDress_BlueChecker", "MiniDress_BlueWithDots", "MiniDress_WhiteChecker", "MiniDress_BrownChecker", "MiniDress_GreenChecker"});
		/*AddItem("MiniDress_PinkChecker", 			35,		70,		1,		100,	0);
		AddItem("MiniDress_RedChecker", 			35,		70,		1,		100,	0);
		AddItem("MiniDress_BlueChecker", 			35,		70,		1,		100,	0);
		AddItem("MiniDress_BlueWithDots", 			35,		70,		1,		100,	0);
		AddItem("MiniDress_WhiteChecker", 			35,		70,		1,		100,	0);
		AddItem("MiniDress_BrownChecker", 			35,		70,		1,		100,	0);
		AddItem("MiniDress_GreenChecker", 			35,		70,		1,		100,	0);*/
		
		AddItem("NurseDress_Blue", 					40,		80,		1,		100,	0, null, {"NurseDress_White"});
		//AddItem("NurseDress_White", 				40,		80,		1,		100,	0);
	}
};