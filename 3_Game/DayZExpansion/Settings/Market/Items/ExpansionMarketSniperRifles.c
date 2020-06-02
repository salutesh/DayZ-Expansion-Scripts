/**
 * ExpansionMarketSniperRifles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSniperRifles: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 51;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SNIPER_RIFLES";
		m_FileName = "Sniper_Rifles";

		AddItem("VSS", 						160,	180,	1,		250,	0 );
		AddItem("SVD", 						200,	220,	1,		250,	0 );
		
		AddItem("Expansion_AWM", 			1000,	1200,	1,		250,	0 );
		AddItem("Expansion_AWM_Black", 		1000,	1200,	1,		250,	0 );
	}
}