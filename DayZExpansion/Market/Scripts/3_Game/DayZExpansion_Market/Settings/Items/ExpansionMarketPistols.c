/**
 * ExpansionMarketPistols.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketPistols: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 46;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_PISTOLS";
		m_FileName = "Pistols";

		AddItem("MakarovIJ70", 				100,	200,	1,		100,	{"Mag_IJ70_8Rnd"});
		AddItem("CZ75", 					135,	270,	1,		100,	{"Mag_CZ75_15Rnd"});
		AddItem("FNX45", 					175,	350,	1,		100,	{"Mag_FNX45_15Rnd"});
		AddItem("Glock19", 					300,	600,	1,		100,	{"Mag_Glock_15Rnd"});
		AddItem("MKII", 					320,	640,	1,		100,	{"Mag_MKII_10Rnd"});
		AddItem("Colt1911", 				340,	680,	1,		100,	{"Mag_1911_7Rnd"});
		AddItem("Engraved1911", 			340,	680,	1,		100,	{"Mag_1911_7Rnd"});
		AddItem("Magnum", 					400,	800,	1,		100);
		AddItem("Deagle", 					400,	800,	1,		100,	{"Mag_Deagle_9Rnd"}, {"Deagle_Gold"});
		AddItem("Deagle_Gold", 				500,	1000,	1,		100,	{"Mag_Deagle_9Rnd"});
	#ifdef EXPANSIONMOD	
		AddItem("Expansion_M9", 			175,	350,	1,		100,	{"Mag_Expansion_M9_15Rnd"});
		AddItem("Expansion_Longhorn",		200,	400,	1,		100);
	#endif
	}
};