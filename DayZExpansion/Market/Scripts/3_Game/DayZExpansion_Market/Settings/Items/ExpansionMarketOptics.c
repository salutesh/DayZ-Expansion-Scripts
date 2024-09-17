/**
 * ExpansionMarketOptics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketOptics: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 44;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_OPTICS";
		m_FileName = "Optics";
		
		AddItem("pistoloptic", 	6050, 	10085,	1,100);
		AddItem("reflexoptic", 	5070, 	8450,	1,100,{"Battery9V"});
		AddItem("m4_carryhandleoptic", 	1490, 	2485,	1,100);
		AddItem("buisoptic", 	5185, 	8640,	1,100);
		AddItem("fnp45_mrdsoptic", 	9810, 	16355,	1,100,{"Battery9V"});
		AddItem("acogoptic", 	1480, 	2465,	1,100);
		AddItem("acogoptic_6x", 	7985, 	13310,	1,100);
		AddItem("m68optic", 	1440, 	2400,	1,100,{"Battery9V"});
		AddItem("m4_t3nrdsoptic", 	6540, 	10905,	1,100,{"Battery9V"});
		AddItem("kobraoptic", 	1440, 	2400,	1,100,{"Battery9V"});
		AddItem("kashtanoptic", 	1440, 	2400,	1,100);
		AddItem("puscopeoptic", 	1070, 	1785,	1,100);
		AddItem("huntingoptic", 	4715, 	7855,	1,100);
		AddItem("pso1optic", 	1440, 	2400,	1,100,{"Battery9V"});
		AddItem("pso11optic", 	6430, 	10715,	1,100,{"Battery9V"});
		AddItem("kazuaroptic", 	4405, 	7345,	1,100,{"Battery9V"});
		AddItem("starlightoptic", 	4405, 	7345,	1,100,{"Battery9V"});
		
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_M1A_RailAtt", 		150,		300,	1,		100);
		AddItem("Expansion_MP5_RailAtt", 		150,		300,	1,		100);
		AddItem("ExpansionReflexMRSOptic", 		150,		300,	1,		100,	{"Battery9V"});
		AddItem("ExpansionDeltapointOptic", 	150,		300,	1,		100);
		AddItem("ExpansionEXPS3HoloOptic", 		200,		400,	1,		100,	{"Battery9V"});
		AddItem("ExpansionHAMROptic", 			400,		800,	1,		100,	{"Battery9V","ExpansionDeltapointOptic"});
		AddItem("Expansion_PMII25Optic", 		1100,		2200,	1,		100);
		AddItem("ExpansionKar98ScopeOptic", 	1100,		2200,	1,		100);
		
		//! For other players, laser points to the side (owner view ok)
		//AddItem("Expansion_ANPEQ15_Red", 		200,		400,	1,		100);
		//AddItem("Expansion_ANPEQ15_Green", 	200,		400,	1,		100);
	#endif
	}	
};