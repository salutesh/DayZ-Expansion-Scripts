/**
 * ExpansionMarketBackpacks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketGhillies: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 25;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_GHILLIES";
		m_FileName = "Ghillies";
		
		AddItem("GhillieHood_Tan", 					220,		440,		1,		100,	0, null, {"GhillieHood_Woodland", "GhillieHood_Mossy"});
		//AddItem("GhillieHood_Woodland", 			220,		440,		1,		100,	0);
		//AddItem("GhillieHood_Mossy", 				220,		440,		1,		100,	0);
		
		AddItem("GhillieBushrag_Tan", 				400,		800,		1,		100,	0, null, {"GhillieBushrag_Woodland", "GhillieBushrag_Mossy"});
		//AddItem("GhillieBushrag_Woodland", 			400,		800,		1,		100,	0);
		//AddItem("GhillieBushrag_Mossy", 			400,		800,		1,		100,	0);
		
		AddItem("GhillieTop_Tan", 					600,		1200,		1,		100,	0, null, {"GhillieTop_Woodland", "GhillieTop_Mossy"});
		//AddItem("GhillieTop_Woodland", 				600,		1200,		1,		100,	0);
		//AddItem("GhillieTop_Mossy", 				600,		1200,		1,		100,	0);
		
		AddItem("GhillieSuit_Tan", 					1000,		2000,		1,		100,	0, null, {"GhillieSuit_Woodland", "GhillieSuit_Mossy"});
		//AddItem("GhillieSuit_Woodland", 			1000,		2000,		1,		100,	0);
		//AddItem("GhillieSuit_Mossy", 				1000,		2000,		1,		100,	0);
	}
};