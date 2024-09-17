/**
 * ExpansionMarketAssaultRifles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketAssaultRifles: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 4;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_ASSAULT_RIFLES";
		m_FileName = "Assault_Rifles";

		AddItem("fal", 	6020, 	10030,	1,100,{"Fal_OeBttstck","Mag_FAL_20Rnd"});
		AddItem("akm", 	6715, 	11190,	1,100,{"AK_WoodBttstck","AK_WoodHndgrd","Mag_AKM_30Rnd"});
		AddItem("ak101", 	2730, 	4550,	1,100,{"AK_PlasticBttstck","AK_PlasticHndgrd","Mag_AK101_30Rnd"});
		AddItem("ak74", 	5940, 	9900,	1,100,{"AK74_WoodBttstck","AK74_Hndgrd","Mag_AK74_30Rnd"});
		AddItem("m4a1", 	1225, 	2040,	1,100,{"M4_CarryHandleOptic","Mag_STANAG_30Rnd","M4_OEBttstck","M4_PlasticHndgrd"});
		AddItem("m16a2", 	2730, 	4550,	1,100,{"Mag_STANAG_30Rnd"});
		AddItem("famas", 	2730, 	4550,	1,100,{"Mag_FAMAS_25Rnd"});
		
		AddItem("aug", 	1225, 	2040,	1,100,{"Mag_Aug_30Rnd"});
		AddItem("augshort", 	2730, 	4550,	1,100,{"Mag_Aug_30Rnd"});
	
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_M16", 		2730, 	4550,	1,		100,	{"Mag_STANAG_30Rnd"});
		AddItem("Expansion_G36", 		1225, 	2040,	1,		100,	{"Mag_Expansion_G36_30Rnd"});
	#endif
	}
};