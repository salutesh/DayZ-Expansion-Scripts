/**
 * ExpansionMarketMelee.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		
		AddItem("BrassKnuckles_Dull", 		15,		30,		1,		100);
		AddItem("BrassKnuckles_Shiny", 		15,		30,		1,		100);
		AddItem("StunBaton", 				15,		30,		1,		100);
		AddItem("Broom", 					15,	 	30,		1,		100);
		AddItem("Broom_Birch", 				15,	 	30,		1,		100);
		AddItem("Pipe", 					25,		50,		1,		100);
		AddItem("CattleProd", 				25,		50,		1,		100);
		AddItem("BaseballBat", 				35,		70,		1,		100);
		AddItem("NailedBaseballBat", 		45,		90,		1,		100);
		AddItem("BarbedBaseballBat", 		45,		90,		1,		100);
	}
};