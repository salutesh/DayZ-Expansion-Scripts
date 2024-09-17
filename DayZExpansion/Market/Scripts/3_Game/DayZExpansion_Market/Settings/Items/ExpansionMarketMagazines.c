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

		AddItem("mag_ij70_8rnd", 	425, 	705,	1,100);

		AddItem("mag_cz75_15rnd", 	665, 	1105,	1,100);

		AddItem("mag_glock_15rnd", 	530, 	880,	1,100);

		AddItem("mag_mkii_10rnd", 	500, 	830,	1,100);

		AddItem("mag_1911_7rnd", 	2565, 	4275,	1,100);

		AddItem("mag_fnx45_15rnd", 	4235, 	7060,	1,100);

		AddItem("mag_deagle_9rnd", 	190, 	320,	1,100);

		AddItem("mag_cz527_5rnd", 	685, 	1140,	1,100);

		AddItem("mag_cz61_20rnd", 	1340, 	2230,	1,100);

		AddItem("mag_pp19_64rnd", 	1665, 	2775,	1,100);

		AddItem("mag_ump_25rnd", 	1840, 	3070,	1,100);

		AddItem("mag_mp5_15rnd", 	1665, 	2775,	1,100);
		AddItem("mag_mp5_30rnd", 	1875, 	3125,	1,100);

		AddItem("mag_fal_20rnd", 	535, 	895,	1,100);

		AddItem("mag_saiga_5rnd", 	3360, 	5600,	1,100);
		AddItem("mag_saiga_8rnd", 	3360, 	5600,	1,100);
		AddItem("mag_saiga_drum20rnd", 	6655, 	11095,	1,100);

		AddItem("mag_akm_30rnd", 	2995, 	4995,	1,100);
		AddItem("mag_akm_palm30rnd", 	2720, 	4530,	1,100);
		AddItem("mag_akm_drum75rnd", 	11750, 	19580,	1,100);
		
		AddItem("mag_ak101_30rnd", 	1210, 	2015,	1,100);

		AddItem("mag_ak74_30rnd", 	2650, 	4415,	1,100);
		AddItem("mag_ak74_45rnd", 	2665, 	4440,	1,100);

		AddItem("mag_stanag_30rnd", 	2695, 	4490,	1,100);
		AddItem("Mag_STANAGCoupled_30Rnd", 		220,	440,	1,		100);
		AddItem("mag_stanag_60rnd", 	2105, 	3510,	1,100);

		AddItem("mag_cmag_10rnd", 	2865, 	4780,	1,100);
		AddItem("mag_cmag_20rnd", 	1420, 	2370,	1,100);
		AddItem("mag_cmag_30rnd", 	630, 	1055,	1,100);
		AddItem("mag_cmag_40rnd", 	2710, 	4515,	1,100);
	
		AddItem("mag_vss_10rnd", 	5390, 	8985,	1,100);
		AddItem("mag_val_20rnd", 	5435, 	9060,	1,100);
		AddItem("mag_vikhr_30rnd", 	4660, 	7765,	1,100);

		//AddItem("Mag_Ruger1022_10Rnd", 		4,		8,		1,		100);
		AddItem("mag_ruger1022_15rnd", 	445, 	740,	1,100);
		AddItem("mag_ruger1022_30rnd", 	360, 	600,	1,100);

		AddItem("mag_svd_10rnd", 	1085, 	1805,	1,100);

		AddItem("mag_sv98_10rnd", 	2210, 	3685,	1,100);

		AddItem("mag_scout_5rnd", 	1420, 	2365,	1,100);

		//AddItem("Mag_CLIP762x54_5Rnd", 		1,		12,		1,		100);
		//AddItem("Mag_CLIP762x39_10Rnd", 		5,		7,		1,		100);
		
		AddItem("mag_famas_25rnd", 	1085, 	1805,	1,100);
		
		AddItem("mag_aug_30rnd", 	1085, 	1805,	1,100);

		AddItem("mag_p1_8rnd", 	390, 	645,	1,100);

		// unused ?
		//AddItem("Mag_CZ550_4rnd", 				150,	250,	1,		100);
		AddItem("mag_cz550_10rnd", 	3325, 	5540,	1,100);

		AddItem("mag_ssg82_5rnd", 	3960, 	6600,	1,100);
		
		AddItem("mag_m14_10rnd", 	3960, 	6600,	1,100);
		AddItem("mag_m14_20rnd", 	3995, 	6655,	1,100);

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