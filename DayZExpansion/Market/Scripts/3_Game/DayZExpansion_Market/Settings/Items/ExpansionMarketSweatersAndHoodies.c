/**
 * ExpansionMarketSweatersAndHoodies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("sweater_gray", 	635, 	1060,	1,100);
		AddItem("sweater_blue", 	1125, 	1875,	1,100);
		AddItem("sweater_red", 	635, 	1060,	1,100);
		AddItem("sweater_green", 	4190, 	6980,	1,100);

		AddItem("hoodie_blue", 	620, 	1035,	1,100);
		AddItem("hoodie_black", 	610, 	1015,	1,100);
		AddItem("hoodie_brown", 	620, 	1035,	1,100);
		AddItem("hoodie_grey", 	620, 	1035,	1,100);
		AddItem("hoodie_red", 	620, 	1035,	1,100);
		AddItem("hoodie_green", 	610, 	1015,	1,100);
	}
};