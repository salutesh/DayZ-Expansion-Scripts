/**
 * ExpansionMarketSkirtsAndDresses.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		
		AddItem("Skirt_Blue", 						3,		3,		1,		250,	0 );
		AddItem("Skirt_Red", 						3,		3,		1,		250,	0 );
		AddItem("Skirt_White", 						3,		3,		1,		250,	0 );
		AddItem("Skirt_Yellow", 					3,		3,		1,		250,	0 );
		
		AddItem("MiniDress_Pink", 					4,		4,		1,		250,	0 );
		AddItem("MiniDress_PinkChecker", 			4,		4,		1,		250,	0 );
		AddItem("MiniDress_RedChecker", 			4,		4,		1,		250,	0 );
		AddItem("MiniDress_BlueChecker", 			4,		4,		1,		250,	0 );
		AddItem("MiniDress_BlueWithDots", 			4,		4,		1,		250,	0 );
		AddItem("MiniDress_WhiteChecker", 			4,		4,		1,		250,	0 );
		AddItem("MiniDress_BrownChecker", 			4,		4,		1,		250,	0 );
		AddItem("MiniDress_GreenChecker", 			4,		4,		1,		250,	0 );
		
		AddItem("NurseDress_Blue", 					4,		4,		1,		250,	0 );
		AddItem("NurseDress_White", 				4,		4,		1,		250,	0 );
	}
};