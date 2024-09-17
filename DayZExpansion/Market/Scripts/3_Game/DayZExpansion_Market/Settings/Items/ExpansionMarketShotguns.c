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
		
		AddItem("mp133shotgun", 	4980, 	8295,	1,100);
		AddItem("izh43shotgun", 	640, 	1065,	1,100);
		AddItem("izh18shotgun", 	520, 	865,	1,100);
		AddItem("saiga", 	4165, 	6940,	1,100,{"Saiga_Bttstck","Mag_Saiga_8Rnd"});
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_BenelliM4", 		4160,	6940,	1,		100);
		AddItem("Expansion_DT11", 			650,	1060,	1,		100);
	#endif
	}
};