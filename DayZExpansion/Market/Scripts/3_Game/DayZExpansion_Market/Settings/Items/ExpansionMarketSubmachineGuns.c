/**
 * ExpansionMarketSubmachineGuns.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

		CategoryID = 19;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SUBMACHINE_GUNS";
		m_FileName = "Submachine_Guns";

		AddItem("CZ61", 				700,	1400,	1,		100,	{"Mag_CZ61_20Rnd"});
		AddItem("UMP45", 				800,	1600,	1,		100,	{"Mag_UMP_25Rnd"});
		AddItem("MP5K", 				1000,	2000,	1,		100,	{"MP5k_StockBttstck","MP5_PlasticHndgrd","Mag_MP5_30Rnd"});
		AddItem("AKS74U", 				1100,	2200,	1,		100,	{"AKS74U_Bttstck","Mag_AK74_30Rnd"});

	#ifdef EXPANSIONMOD	
		AddItem("Expansion_Kedr", 		700,	1400,	1,		100,	{"Mag_Expansion_Kedr_20Rnd"});
		AddItem("Expansion_MPX", 		900,	1800,	1,		100,	{"Mag_Expansion_MPX_50Rnd"});
		AddItem("Expansion_MP5", 		1100,	2200,	1,		100,	{"Mag_MP5_30Rnd"});
		AddItem("Expansion_MP5SD", 		1300,	2500,	1,		100,	{"Mag_MP5_30Rnd"});
		AddItem("Expansion_MP7", 		1200,	2400,	1,		100,	{"Mag_Expansion_MP7_40Rnd"});
		AddItem("Expansion_VityazSN", 	1400,	2800,	1,		100,	{"AK_PlasticBttstck","AK_PlasticHndgrd","Mag_Expansion_Vityaz_30Rnd"});
	#endif
	}
};