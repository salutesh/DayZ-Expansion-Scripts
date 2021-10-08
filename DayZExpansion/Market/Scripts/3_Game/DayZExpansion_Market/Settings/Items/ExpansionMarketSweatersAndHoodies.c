/**
 * ExpansionMarketSweatersAndHoodies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSweatersAndHoodies: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 53;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_SWEATERS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_HOODIES");
		m_FileName = "Sweaters_And_Hoodies";

		AddItem("Sweater_Gray", 						20,		40,		1,		100,	0);
		AddItem("Sweater_Blue", 						20,		40,		1,		100,	0);
		AddItem("Sweater_Red", 							20,		40,		1,		100,	0);
		AddItem("Sweater_Green", 						25,		50,		1,		100,	0);

		AddItem("Hoodie_Blue", 							30,		60,		1,		100,	0);
		AddItem("Hoodie_Black", 						30,		60,		1,		100,	0);
		AddItem("Hoodie_Brown", 						30,		60,		1,		100,	0);
		AddItem("Hoodie_Grey", 							30,		60,		1,		100,	0);
		AddItem("Hoodie_Red", 							30,		60,		1,		100,	0);
		AddItem("Hoodie_Green", 						35,		70,		1,		100,	0);
	}
};