/**
 * ExpansionMarketRifles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketRifles: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 47;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_RIFLES";
		m_FileName = "Rifles";

		AddItem("Izh18", 					350,	700,	1,		100);
		AddItem("Ruger1022", 				350,	700,	1,		100,	{"Mag_Ruger1022_15Rnd"});
		AddItem("Repeater", 				350,	700,	1,		100);
		AddItem("Mosin9130", 				400,	800,	1,		100);
		AddItem("CZ527", 					500,	1000,	1,		100,	{"Mag_CZ527_5Rnd"});
		AddItem("CZ550", 					500,	1000,	1,		100,	{"Mag_CZ550_4Rnd"});
		AddItem("Winchester70", 			500,	1000,	1,		100);
		AddItem("SSG82", 					450,	1000,	1,		100,	{"Mag_SSG82_5rnd"});
		AddItem("SKS", 						450,	1000,	1,		100,	{"SKS_Bayonet"});
		AddItem("M14", 						500,	1000,	1,		100,	{"M14_10Rnd","M14_20Rnd"});

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Kar98", 			500,	1000,	1,		100);
		AddItem("Expansion_M14", 			500,	1000,	1,		100,	{"Mag_Expansion_M14_10Rnd"});
		AddItem("Expansion_M1A", 			500,	1000,	1,		100,	{"Mag_Expansion_M14_10Rnd"});
	#endif
	}
};