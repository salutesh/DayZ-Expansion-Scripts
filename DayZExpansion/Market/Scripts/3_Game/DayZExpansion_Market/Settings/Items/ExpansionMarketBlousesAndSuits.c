/**
 * ExpansionMarketBlousesAndSuits.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("blouse_blue", 	280, 	470,	1,100,null,{"Blouse_Violet","Blouse_White","Blouse_Green"});
			
		AddItem("mansuit_beige", 	830, 	1380,	1,100,null,{"ManSuit_Black","ManSuit_Blue","ManSuit_Brown","ManSuit_DarkGrey","ManSuit_Khaki","ManSuit_LightGrey","ManSuit_White"});
		AddItem("mansuit_white", 	690, 	1150,	1,100);
			
		AddItem("womansuit_beige", 	830, 	1380,	1,100,null,{"WomanSuit_Black","WomanSuit_Blue","WomanSuit_Brown","WomanSuit_DarkGrey","WomanSuit_Khaki","WomanSuit_LightGrey","WomanSuit_White"});
		AddItem("womansuit_black", 	1285, 	2140,	1,100);
		AddItem("womansuit_blue", 	1285, 	2140,	1,100);
		AddItem("womansuit_brown", 	1285, 	2140,	1,100);
		AddItem("womansuit_darkgrey", 	830, 	1380,	1,100);
		AddItem("womansuit_khaki", 	830, 	1380,	1,100);
		AddItem("womansuit_lightgrey", 	830, 	1380,	1,100);
		AddItem("womansuit_white", 	690, 	1150,	1,100);
	}
};