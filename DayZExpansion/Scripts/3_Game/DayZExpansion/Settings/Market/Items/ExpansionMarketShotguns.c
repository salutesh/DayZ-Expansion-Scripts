/**
 * ExpansionMarketShotguns.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketShotguns: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 49;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SHOTGUNS";
		m_FileName = "Shotguns";
		
		// Shotguns
		AddItem("Mp133Shotgun", 			50,		52,		1,		250,	0 );
		AddItem("Izh43", 					50,		52,		1,		250,	0 );
		AddItem("Saiga", 					50,		52,		1,		250,	0 );
		AddItem("Expansion_BenelliM4", 		200,	22,		1,		250,	0 );
	}
}