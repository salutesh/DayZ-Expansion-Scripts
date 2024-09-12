/**
 * ExpansionMarketMagazines.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMagazines: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 37;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MAGAZINES";
		m_FileName = "Magazines";

		AddItem("Mag_IJ70_8Rnd", 				30,		60,		1,		100);

		AddItem("Mag_CZ75_15Rnd", 				40,		80,		1,		100);

		AddItem("Mag_Glock_15Rnd", 				40,		80,		1,		100);

		AddItem("Mag_MKII_10Rnd", 				40,		80,		1,		100);

		AddItem("Mag_1911_7Rnd", 				40,		80,		1,		100);

		AddItem("Mag_FNX45_15Rnd", 				40,		80,		1,		100);

		AddItem("Mag_Deagle_9Rnd", 				45,		90,		1,		100);

		AddItem("Mag_CZ527_5rnd", 				60,		120,	1,		100);

		AddItem("Mag_CZ61_20Rnd", 				100,	200,	1,		100);

		AddItem("Mag_PP19_64Rnd", 				100,	200,	1,		100);

		AddItem("Mag_UMP_25Rnd", 				150,	300,	1,		100);

		AddItem("Mag_MP5_15Rnd", 				150,	300,	1,		100);
		AddItem("Mag_MP5_30Rnd", 				200,	400,	1,		100);

		AddItem("Mag_FAL_20Rnd", 				100,	200,	1,		100);

		AddItem("Mag_Saiga_5Rnd", 				100,	200,	1,		100);
		AddItem("Mag_Saiga_8Rnd", 				220,	440,	1,		100);
		AddItem("Mag_Saiga_Drum20Rnd", 			480,	960,	1,		100);

		AddItem("Mag_AKM_30Rnd", 				220,	440,	1,		100);
		AddItem("Mag_AKM_Palm30Rnd", 			220,	440,	1,		100);
		AddItem("Mag_AKM_Drum75Rnd", 			480,	960,	1,		100);
		
		AddItem("Mag_AK101_30Rnd", 				220,	440,	1,		100);

		AddItem("Mag_AK74_30Rnd", 				220,	440,	1,		100);
		AddItem("Mag_AK74_45Rnd", 				300,	600,	1,		100);

		AddItem("Mag_STANAG_30Rnd", 			220,	440,	1,		100);
		AddItem("Mag_STANAGCoupled_30Rnd", 		220,	440,	1,		100);
		AddItem("Mag_STANAG_60Rnd", 			480,	960,	1,		100);

		AddItem("Mag_CMAG_10Rnd", 				120,	240,	1,		100);
		AddItem("Mag_CMAG_20Rnd", 				180,	360,	1,		100);
		AddItem("Mag_CMAG_30Rnd", 				220,	440,	1,		100);
		AddItem("Mag_CMAG_40Rnd", 				300,	600,	1,		100);
	
		AddItem("Mag_VSS_10Rnd", 				220,	440,	1,		100);
		AddItem("Mag_VAL_20Rnd", 				280,	560,	1,		100);
		AddItem("Mag_Vikhr_30Rnd", 				310,	580,	1,		100);

		//AddItem("Mag_Ruger1022_10Rnd", 		4,		8,		1,		100);
		AddItem("Mag_Ruger1022_15Rnd", 			250,	500,	1,		100);
		AddItem("Mag_Ruger1022_30Rnd", 			310,	620,	1,		100);

		AddItem("Mag_SVD_10Rnd", 				250,	500,	1,		100);

		AddItem("Mag_SV98_10Rnd", 				275,	500,	1,		100);

		AddItem("Mag_Scout_5Rnd", 				250,	500,	1,		100);

		//AddItem("Mag_CLIP762x54_5Rnd", 		1,		12,		1,		100);
		//AddItem("Mag_CLIP762x39_10Rnd", 		5,		7,		1,		100);
		
		AddItem("Mag_FAMAS_25Rnd", 				220,	440,	1,		100);
		
		AddItem("Mag_Aug_30Rnd", 				220,	440,	1,		100);

		AddItem("Mag_P1_8Rnd", 					45,		90,		1,		100);

		AddItem("Mag_CZ550_4rnd", 				150,	250,	1,		100);
		AddItem("Mag_CZ550_10rnd", 				210,	320,	1,		100);

		AddItem("Mag_SSG82_5rnd", 				180,	280,	1,		100);
		
		AddItem("Mag_M14_10Rnd", 				200,	400,	1,		100);
		AddItem("Mag_M14_20Rnd", 				260,	520,	1,		100);

	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Mag_Expansion_Taser", 			20,		40,		1,		100);

		AddItem("Mag_Expansion_M9_15Rnd", 		40,		80,		1,		100);
		
		AddItem("Mag_Expansion_Kedr_20Rnd", 	200,	400,	1,		100);	

		AddItem("Mag_Expansion_MP7_40Rnd", 		260,	520,	1,		100);
		
		AddItem("Mag_Expansion_Vityaz_30Rnd", 	250,	500,	1,		100);

		AddItem("Mag_Expansion_MPX_50Rnd", 		320,	640,	1,		100);
		
		AddItem("Mag_Expansion_M14_10Rnd", 		200,	400,	1,		100);
		AddItem("Mag_Expansion_M14_20Rnd", 		260,	520,	1,		100);
		
		AddItem("Mag_Expansion_G36_30Rnd", 		250,	500,	1,		100);

		AddItem("Mag_Expansion_AWM_5Rnd", 		500,	1000,	1,		100);
	#endif
	}
};