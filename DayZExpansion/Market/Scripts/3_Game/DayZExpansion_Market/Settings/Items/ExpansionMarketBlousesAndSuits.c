/**
 * ExpansionMarketBlousesAndSuits.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBlousesAndSuits: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 9;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_BLOUSES" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_SUITS");
		m_FileName = "Blouses_And_Suits";

		AddItem("Blouse_Blue", 							16,		32,		1,		100, null, {"Blouse_Violet", "Blouse_White", "Blouse_Green"});
		/*AddItem("Blouse_Violet", 						16,		32,		1,		100);
		AddItem("Blouse_White", 						16,		32,		1,		100);*/
		AddItem("Blouse_Green", 						18,		36,		1,		100);
			
		AddItem("ManSuit_Beige", 						18,		36,		1,		100, null, {"ManSuit_Black", "ManSuit_Blue", "ManSuit_Brown", "ManSuit_DarkGrey", "ManSuit_Khaki", "ManSuit_LightGrey", "ManSuit_White"});
		/*AddItem("ManSuit_Black", 						18,		36,		1,		100);
		AddItem("ManSuit_Blue", 						18,		36,		1,		100);
		AddItem("ManSuit_Brown", 						18,		36,		1,		100);
		AddItem("ManSuit_DarkGrey",						18,		36,		1,		100);
		AddItem("ManSuit_Khaki", 						18,		36,		1,		100);
		AddItem("ManSuit_LightGrey", 					18,		36,		1,		100);
		AddItem("ManSuit_White", 						18,		36,		1,		100);*/
			
		AddItem("WomanSuit_Beige", 						18,		36,		1,		100, null, {"WomanSuit_Black", "WomanSuit_Blue", "WomanSuit_Brown", "WomanSuit_DarkGrey", "WomanSuit_Khaki", "WomanSuit_LightGrey", "WomanSuit_White"});
		/*AddItem("WomanSuit_Black", 					18,		36,		1,		100);
		AddItem("WomanSuit_Blue", 						18,		36,		1,		100);
		AddItem("WomanSuit_Brown", 						18,		36,		1,		100);
		AddItem("WomanSuit_DarkGrey", 					18,		36,		1,		100);
		AddItem("WomanSuit_Khaki", 						18,		36,		1,		100);
		AddItem("WomanSuit_LightGrey", 					18,		36,		1,		100);
		AddItem("WomanSuit_White", 						18,		36,		1,		100);*/
	}
};