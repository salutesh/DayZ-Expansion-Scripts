/**
 * ExpansionMarketSubmachineGuns.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSubmachineGuns: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SUBMACHINE_GUNS";
		m_FileName = "Submachine_Guns";

		AddItem("cz61", 	2975, 	4960,	1,100,{"Mag_CZ61_20Rnd"}, {"CZ61_Clean", "CZ61_Black", "CZ61_Green", "CZ61_Yellow", "CZ61_Orange", "CZ61_Brown", "CZ61_White", "CZ61_Grey", "CZ61_Blue", "CZ61_Red", "CZ61_Pink"});
		AddItem("ump45", 	4165, 	6940,	1,100,{"Mag_UMP_25Rnd"}, {"UMP45_Clean", "UMP45_Black", "UMP45_Green", "UMP45_Yellow", "UMP45_Orange", "UMP45_Brown", "UMP45_White", "UMP45_Grey", "UMP45_Blue", "UMP45_Red", "UMP45_Pink"});
		AddItem("mp5k", 	3355, 	5595,	1,100,{"MP5k_StockBttstck","MP5_PlasticHndgrd","Mag_MP5_30Rnd"}, {"MP5K_Clean", "MP5K_Black", "MP5K_Green", "MP5K_Yellow", "MP5K_Orange", "MP5K_Brown", "MP5K_White", "MP5K_Grey", "MP5K_Blue", "MP5K_Red", "MP5K_Pink"});
		AddItem("aks74u", 	5390, 	8980,	1,100,{"AKS74U_Bttstck","Mag_AK74_30Rnd"}, {"AKS74U_Clean", "AKS74U_Black", "AKS74U_Green", "AKS74U_Yellow", "AKS74U_Orange", "AKS74U_Brown", "AKS74U_White", "AKS74U_Grey", "AKS74U_Blue", "AKS74U_Red", "AKS74U_Pink"});
		AddItem("pp19", 	4165, 	6940,	1,100,{"PP19_Bttstck","Mag_PP19_64Rnd"});

	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_Kedr", 		700,	1400,	1,		100,	{"Mag_Expansion_Kedr_20Rnd"});
		AddItem("Expansion_MPX", 		900,	1800,	1,		100,	{"Mag_Expansion_MPX_50Rnd"});
		AddItem("Expansion_MP5", 		1100,	2200,	1,		100,	{"Mag_MP5_30Rnd"});
		AddItem("Expansion_MP5SD", 		1300,	2500,	1,		100,	{"Mag_MP5_30Rnd"});
		AddItem("Expansion_MP7", 		1200,	2400,	1,		100,	{"Mag_Expansion_MP7_40Rnd"});
		AddItem("Expansion_VityazSN", 	1400,	2800,	1,		100,	{"AK_PlasticBttstck","AK_PlasticHndgrd","Mag_Expansion_Vityaz_30Rnd"});
	#endif
	}
};