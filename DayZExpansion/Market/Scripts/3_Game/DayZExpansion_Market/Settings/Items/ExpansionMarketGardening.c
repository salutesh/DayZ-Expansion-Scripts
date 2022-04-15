/**
 * ExpansionMarketGardening.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketGardening: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 25;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_GARDENING";
		m_FileName = "Gardening";
		
		AddItem("GardenLime", 					4,		8,		1,		100, null, {});
		AddItem("PepperSeeds", 					4,		8,		1,		100, null, {});
		AddItem("TomatoSeeds", 					4,		8,		1,		100, null, {});
		AddItem("ZucchiniSeeds", 				6,		12,		1,		100, null, {});
		AddItem("PumpkinSeeds", 				10,		20,		1,		100, null, {});
	}
};