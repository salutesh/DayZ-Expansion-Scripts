/**
 * ExpansionMarketBackpacks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		
		AddItem("GhillieHood_Tan", 					3160,		5265,		1,		100, null, {"GhillieHood_Woodland","GhillieHood_Mossy","GhillieHood_Winter"});
		AddItem("GhillieBushrag_Tan", 				3160,		5265,		1,		100, null, {"GhillieBushrag_Woodland","GhillieBushrag_Mossy","GhillieBushrag_Winter"});
		AddItem("GhillieTop_Tan", 					3160,		5265,		1,		100, null, {"GhillieTop_Woodland","GhillieTop_Mossy","GhillieTop_Winter"});
		AddItem("GhillieSuit_Tan", 					3160,		5265,		1,		100, null, {"GhillieSuit_Woodland","GhillieSuit_Mossy","GhillieSuit_Winter"});
	}
};