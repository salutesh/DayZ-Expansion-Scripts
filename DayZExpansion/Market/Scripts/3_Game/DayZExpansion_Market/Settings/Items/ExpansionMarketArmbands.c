/**
 * ExpansionMarketArmbands.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketArmbands: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 3;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_ARMBANDS";
		m_FileName = "Armbands";

		AddItem("Armband_White", 					15,		30,		1,		250,	0);
		AddItem("Armband_Yellow", 					15,		30,		1,		250,	0);
		AddItem("Armband_Orange", 					15,		30,		1,		250,	0);
		AddItem("Armband_Red", 						15,		30,		1,		250,	0);
		AddItem("Armband_Green",					15,		30,		1,		250,	0);
		AddItem("Armband_Pink", 					15,		30,		1,		250,	0);
		AddItem("Armband_Blue", 					15,		30,		1,		250,	0);
		AddItem("Armband_Black", 					15,		30,		1,		250,	0);
	}
};