/**
 * ExpansionMarketPistols.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("makarovij70", 	470, 	785,	1,100,{"Mag_IJ70_8Rnd"});
		AddItem("derringer_black", 	365, 	610,	1,100,null,{"Derringer_Grey","Derringer_Pink"});
		AddItem("cz75", 	735, 	1230,	1,100,{"Mag_CZ75_15Rnd"});
		AddItem("fnx45", 	4705, 	7845,	1,100,{"Mag_FNX45_15Rnd"});
		AddItem("glock19", 	510, 	850,	1,100,{"Mag_Glock_15Rnd"});
		AddItem("mkii", 	625, 	1040,	1,100,{"Mag_MKII_10Rnd"});
		AddItem("colt1911", 	3210, 	5345,	1,100,{"Mag_1911_7Rnd"});
		AddItem("engraved1911", 	3235, 	5390,	1,100,{"Mag_1911_7Rnd"});
		AddItem("magnum", 	235, 	390,	1,100);
		AddItem("deagle", 	225, 	375,	1,100,{"Mag_Deagle_9Rnd"},{"Deagle_Gold"});
		AddItem("deagle_gold", 	240, 	400,	1,100,{"Mag_Deagle_9Rnd"});
		AddItem("p1", 	520, 	865,	1,100,{"Mag_P1_8Rnd"});
		AddItem("longhorn", 	1960, 	3270,	1,100);

	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_M9", 			4600, 	7800,	1,		100,	{"Mag_Expansion_M9_15Rnd"});
	#endif
	}
};