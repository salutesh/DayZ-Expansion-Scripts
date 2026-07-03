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

		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SHOTGUNS";
		m_FileName = "Shotguns";
		
		AddItem("mp133shotgun", 	4980, 	8295,	1,100, null, {"Mp133Shotgun_Clean", "Mp133Shotgun_Black", "Mp133Shotgun_Green", "Mp133Shotgun_Yellow", "Mp133Shotgun_Orange", "Mp133Shotgun_Brown", "Mp133Shotgun_White", "Mp133Shotgun_Grey", "Mp133Shotgun_Blue", "Mp133Shotgun_Red", "Mp133Shotgun_Pink"});
		AddItem("izh43shotgun", 	640, 	1065,	1,100, null, {"Izh43Shotgun_Clean", "Izh43Shotgun_Black", "Izh43Shotgun_Green", "Izh43Shotgun_Yellow", "Izh43Shotgun_Orange", "Izh43Shotgun_Brown", "Izh43Shotgun_White", "Izh43Shotgun_Grey", "Izh43Shotgun_Blue", "Izh43Shotgun_Red", "Izh43Shotgun_Pink"});
		AddItem("izh18shotgun", 	520, 	865,	1,100);
		AddItem("saiga", 	4165, 	6940,	1,100,{"Saiga_Bttstck","Mag_Saiga_8Rnd"});
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_BenelliM4", 		4160,	6940,	1,		100);
		AddItem("Expansion_DT11", 			650,	1060,	1,		100);
	#endif
		AddItem("SawedoffIzh18Shotgun", 800, 1600, 1, 100);
		AddItem("SawedoffIzh43Shotgun", 800, 1600, 1, 100);
}
};