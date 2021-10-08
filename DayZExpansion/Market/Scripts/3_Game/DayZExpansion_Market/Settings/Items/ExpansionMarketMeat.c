/**
 * ExpansionMarketMeat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMeat: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 39;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MEAT";
		m_FileName = "Meat";

		AddItem("BearSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("GoatSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("BoarSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("PigSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("DeerSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("WolfSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("CowSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("SheepSteakMeat", 				8,		16,		1,		100,	0);
		AddItem("ChickenBreastMeat", 			8,		16,		1,		100,	0);
	#ifdef EXPANSIONMOD	
		AddItem("RabbitLegMeat", 				8,		16,		1,		100,	0);
	#endif
	}
};