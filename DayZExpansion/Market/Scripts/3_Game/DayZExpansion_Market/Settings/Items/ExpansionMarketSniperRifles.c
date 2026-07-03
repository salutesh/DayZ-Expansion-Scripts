/**
 * ExpansionMarketSniperRifles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SNIPER_RIFLES";
		m_FileName = "Sniper_Rifles";

		AddItem("vss", 	5885, 	9810,	1,100,{"Mag_VSS_10Rnd"}, {"VSS_Clean", "VSS_Black", "VSS_Green", "VSS_Yellow", "VSS_Orange", "VSS_Brown", "VSS_White", "VSS_Grey", "VSS_Blue", "VSS_Red", "VSS_Pink"});
		AddItem("asval", 	6770, 	11285,	1,100,{"Mag_VAL_20Rnd"});
		AddItem("vikhr", 	6040, 	10070,	1,100,{"Mag_Vikhr_30Rnd"});
		AddItem("b95", 	4205, 	7005,	1,100, null, {"B95_Clean", "B95_Black", "B95_Green", "B95_Yellow", "B95_Orange", "B95_Brown", "B95_White", "B95_Grey", "B95_Blue", "B95_Red", "B95_Pink"});
		AddItem("svd", 	1225, 	2040,	1,100,{"Mag_SVD_10Rnd"}, {"SVD_Clean", "SVD_Black", "SVD_Green", "SVD_Yellow", "SVD_Orange", "SVD_Brown", "SVD_White", "SVD_Grey", "SVD_Blue", "SVD_Red", "SVD_Pink"});
		AddItem("sv98", 	4425, 	7370,	1,100,{"Mag_SV98_10Rnd"});

		if (ExpansionStatic.GetCanonicalWorldName() == "enoch")
		{
		AddItem("scout_livonia", 	3670, 	6120,	1,100,{"Mag_Scout_5Rnd"});
		}
		else if (ExpansionStatic.GetCanonicalWorldName() == "chernarusplus")
		{
		AddItem("scout_chernarus", 	3610, 	6020,	1,100,{"Mag_Scout_5Rnd"});
		}
		else
		{
		AddItem("scout", 	305, 	510,	1,100,{"Mag_Scout_5Rnd"});
		}
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_AWM", 			5000,	10000,	1,		100,	{"Mag_Expansion_AWM_5rnd"}, {"Expansion_AWM_Black", "Expansion_AWM_Green"});
	#endif
	}
};