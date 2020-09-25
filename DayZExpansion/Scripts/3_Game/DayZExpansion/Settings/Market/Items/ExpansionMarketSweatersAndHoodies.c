/**
 * ExpansionMarketSweatersAndHoodies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSweatersAndHoodies: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 55;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_SWEATERS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_HOODIES");
		m_FileName = "Sweaters_And_Hoodies";

		AddItem("Sweater_Gray", 						2,		2,		1,		250,	0 );
		AddItem("Sweater_Blue", 						2,		2,		1,		250,	0 );
		AddItem("Sweater_Green", 						2,		2,		1,		250,	0 );
		AddItem("Sweater_Red", 							2,		2,		1,		250,	0 );
		
		AddItem("Hoodie_Blue", 							3,		3,		1,		250,	0 );
		AddItem("Hoodie_Black", 						3,		3,		1,		250,	0 );
		AddItem("Hoodie_Brown", 						3,		3,		1,		250,	0 );
		AddItem("Hoodie_Green", 						3,		3,		1,		250,	0 );
		AddItem("Hoodie_Grey", 							3,		3,		1,		250,	0 );
		AddItem("Hoodie_Red", 							3,		3,		1,		250,	0 );
	}
};