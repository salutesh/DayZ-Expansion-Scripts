/**
 * ExpansionMarketFish.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Keep in mind that when you modify the price for the fishes that players can fillet the fish and sell the fillet meat (2x meat per fish).
// Right now the price for the fillet fish meat is 20% lower then the price of the fish.

class ExpansionMarketFish: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 22;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FISH";
		m_FileName = "Fish";

		AddItem("CarpFilletMeat", 				8,			16,			1,		100);
		AddItem("MackerelFilletMeat", 			8,			16,			1,		100);
		AddItem("Carp", 						20,			40,			1,		100);
		AddItem("Sardines", 					20,			40,			1,		100);
		AddItem("Mackerel", 					20,			40,			1,		100);
	}
};