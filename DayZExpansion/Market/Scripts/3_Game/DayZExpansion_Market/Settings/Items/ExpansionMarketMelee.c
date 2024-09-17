/**
 * ExpansionMarketMelee.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMelee: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 41;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MEELE_WEAPONS";
		m_FileName = "Melee_Weapons";
		
		AddItem("brassknuckles_dull", 	260, 	430,	1,100);
		AddItem("brassknuckles_shiny", 	1815, 	3025,	1,100);
		//AddItem("StunBaton", 				15,		30,		1,		100);
		AddItem("broom", 	285, 	475,	1,100);
		AddItem("broom_birch", 	1080, 	1800,	1,100);
		AddItem("pipe", 	610, 	1015,	1,100);
		//AddItem("CattleProd", 				25,		50,		1,		100);
		AddItem("baseballbat", 	230, 	385,	1,100);
		//AddItem("NailedBaseballBat", 		45,		90,		1,		100);
		//AddItem("BarbedBaseballBat", 		45,		90,		1,		100);
		//! 1.21+
		AddItem("mace", 	355, 	595,	1,100);
	}
};