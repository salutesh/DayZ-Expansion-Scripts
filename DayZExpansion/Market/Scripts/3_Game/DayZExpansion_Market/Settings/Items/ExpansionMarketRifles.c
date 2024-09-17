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

		AddItem("izh18", 	530, 	885,	1,100);
		AddItem("ruger1022", 	865, 	1440,	1,100,{"Mag_Ruger1022_15Rnd"});
		AddItem("repeater", 	1500, 	2500,	1,100);
		AddItem("mosin9130", 	1230, 	2050,	1,100);
		AddItem("cz527", 	780, 	1300,	1,100,{"Mag_CZ527_5Rnd"});
		AddItem("cz550", 	3725, 	6210,	1,100,{"mag_cz550_10rnd"});
		AddItem("winchester70", 	3725, 	6210,	1,100);
		AddItem("ssg82", 	3960, 	6600,	1,100,{"Mag_SSG82_5rnd"});
		AddItem("sks", 	980, 	1635,	1,100,{"SKS_Bayonet"});
		AddItem("m14", 	6715, 	11190,	1,100,{"M14_10Rnd","M14_20Rnd"});

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Kar98", 			1230, 	2050,	1,		100);
		AddItem("Expansion_M14", 			6715, 	11190,	1,		100,	{"Mag_Expansion_M14_10Rnd"});
		AddItem("Expansion_M1A", 			6715, 	11190,	1,		100,	{"Mag_Expansion_M14_10Rnd"});
	#endif
	}
};