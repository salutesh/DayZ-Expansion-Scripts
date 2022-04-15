/**
 * ExpansionMarketMeat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("BearSteakMeat", 				8,		16,		1,		100);
		AddItem("GoatSteakMeat", 				8,		16,		1,		100);
		AddItem("BoarSteakMeat", 				8,		16,		1,		100);
		AddItem("PigSteakMeat", 				8,		16,		1,		100);
		AddItem("DeerSteakMeat", 				8,		16,		1,		100);
		AddItem("WolfSteakMeat", 				8,		16,		1,		100);
		AddItem("CowSteakMeat", 				8,		16,		1,		100);
		AddItem("SheepSteakMeat", 				8,		16,		1,		100);
		AddItem("ChickenBreastMeat", 			8,		16,		1,		100);
	#ifdef EXPANSIONMOD	
		AddItem("RabbitLegMeat", 				8,		16,		1,		100);
	#endif
	}
};