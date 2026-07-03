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

		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_RIFLES";
		m_FileName = "Rifles";

		AddItem("izh18", 	530, 	885,	1,100, null, {"Izh18_Clean", "Izh18_Black", "Izh18_Green", "Izh18_Yellow", "Izh18_Orange", "Izh18_Brown", "Izh18_White", "Izh18_Grey", "Izh18_Blue", "Izh18_Red", "Izh18_Pink"});
		AddItem("ruger1022", 	865, 	1440,	1,100,{"Mag_Ruger1022_15Rnd"}, {"Ruger1022_Clean", "Ruger1022_Black", "Ruger1022_Green", "Ruger1022_Yellow", "Ruger1022_Orange", "Ruger1022_Brown", "Ruger1022_White", "Ruger1022_Grey", "Ruger1022_Blue", "Ruger1022_Red", "Ruger1022_Pink"});
		AddItem("repeater", 	1500, 	2500,	1,100, null, {"Repeater_Clean", "Repeater_Black", "Repeater_Green", "Repeater_Yellow", "Repeater_Orange", "Repeater_Brown", "Repeater_White", "Repeater_Grey", "Repeater_Blue", "Repeater_Red", "Repeater_Pink"});
		AddItem("mosin9130", 	1230, 	2050,	1,100, null, {"Mosin9130_Clean", "Mosin9130_Black", "Mosin9130_Green", "Mosin9130_Yellow", "Mosin9130_Orange", "Mosin9130_Brown", "Mosin9130_White", "Mosin9130_Grey", "Mosin9130_Blue", "Mosin9130_Red", "Mosin9130_Pink", "Mosin9130_Camo"});
		AddItem("cz527", 	780, 	1300,	1,100,{"Mag_CZ527_5Rnd"}, {"CZ527_Clean", "CZ527_Black", "CZ527_Green", "CZ527_Yellow", "CZ527_Orange", "CZ527_Brown", "CZ527_White", "CZ527_Grey", "CZ527_Blue", "CZ527_Red", "CZ527_Pink", "CZ527_Camo"});
		AddItem("cz550", 	3725, 	6210,	1,100,{"mag_cz550_10rnd"});
		AddItem("winchester70", 	3725, 	6210,	1,100, null, {"Winchester70_Clean", "Winchester70_Black", "Winchester70_Green", "Winchester70_Yellow", "Winchester70_Orange", "Winchester70_Brown", "Winchester70_White", "Winchester70_Grey", "Winchester70_Blue", "Winchester70_Red", "Winchester70_Pink"});
		AddItem("ssg82", 	3960, 	6600,	1,100,{"Mag_SSG82_5rnd"});
		AddItem("sks", 	980, 	1635,	1,100,{"SKS_Bayonet"}, {"SKS_Clean", "SKS_Black", "SKS_Green", "SKS_Yellow", "SKS_Orange", "SKS_Brown", "SKS_White", "SKS_Grey", "SKS_Blue", "SKS_Red", "SKS_Pink"});
		AddItem("m14", 	6715, 	11190,	1,100,{"Mag_M14_10Rnd"});

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Kar98", 			1230, 	2050,	1,		100);
		AddItem("Expansion_M1A", 			6715, 	11190,	1,		100,	{"Mag_Expansion_M14_10Rnd"}, {"Expansion_M1A_Black", "Expansion_M1A_Tan", "Expansion_M1A_Green"});
	#endif
		AddItem("PM73Rak", 2000, 4000, 1, 100);
		AddItem("R12", 2000, 4000, 1, 100);
		AddItem("SVD_Wooden", 2000, 4000, 1, 100);
		AddItem("SawedoffMosin9130", 2000, 4000, 1, 100, null, {"SawedoffMosin9130_Black", "SawedoffMosin9130_Camo", "SawedoffMosin9130_Green"});
		AddItem("SawedoffB95", 800, 1600, 1, 100);
		AddItem("SawedoffIzh18", 800, 1600, 1, 100);
}
};