/**
 * ExpansionMarketVehicleParts.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketLiquids: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 35;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_LIQUIDS";
		m_FileName = "Liquids";

		AddItem("EngineOil", 					70,		140,	1,		100,	0);	
		AddItem("CanisterGasoline", 			300,	600,	1,		100,	0);
	}
};