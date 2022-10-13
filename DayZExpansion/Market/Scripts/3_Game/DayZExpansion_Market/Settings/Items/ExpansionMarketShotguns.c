/**
 * ExpansionMarketShotguns.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		
		AddItem("Mp133Shotgun", 			500,	1000,	1,		100);
		AddItem("Izh43Shotgun", 			900,	1800,	1,		100);
#ifndef DAYZ_1_18
		AddItem("Izh18Shotgun", 			350,	700,	1,		100);
#endif
		AddItem("Saiga", 					1500,	3000,	1,		100,	{"Saiga_Bttstck","Mag_Saiga_8Rnd"});
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_BenelliM4", 		1500,	3000,	1,		100);
		AddItem("Expansion_DT11", 			800,	1600,	1,		100);
	#endif
	}
};