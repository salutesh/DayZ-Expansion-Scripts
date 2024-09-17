/**
 * ExpansionMarketAmmoBoxes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Keep in mind that when you modify the price for the ammo that players can buy the ammo boxes and sell the single bullets.
// Right now the price for the ammo boxes is 20% lower then the price of a single bullet.

class ExpansionMarketAmmoBoxes: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 2;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_AMMOBOXES";
		m_FileName = "Ammo_Boxes";

		AddItem("ammobox_00buck_10rnd", 	5980, 	9965,	1,250);
		AddItem("ammobox_12gaslug_10rnd", 	5980, 	9965,	1,250);
		AddItem("ammobox_12garubberslug_10rnd", 	1165, 	1940,	1,250);
		
		AddItem("ammobox_22_50rnd", 	330, 	550,	1,250);
		
		AddItem("ammobox_308win_20rnd", 	5755, 	9595,	1,250,null,{"AmmoBox_308WinTracer_20Rnd"});
		AddItem("ammobox_308wintracer_20rnd", 	11875, 	19795,	1,250);
		
		AddItem("ammobox_357_20rnd", 	430, 	715,	1,250);
		AddItem("ammobox_380_35rnd", 	1175, 	1955,	1,250);
		AddItem("ammobox_45acp_25rnd", 	1760, 	2930,	1,250);
		
		AddItem("ammobox_545x39_20rnd", 	11775, 	19625,	1,250,null,{"AmmoBox_545x39Tracer_20Rnd"});
		AddItem("ammobox_545x39tracer_20rnd", 	11875, 	19795,	1,250);
		
		AddItem("ammobox_556x45_20rnd", 	35325, 	58875,	1,250,null,{"AmmoBox_556x45Tracer_20Rnd"});
		AddItem("ammobox_556x45tracer_20rnd", 	35630, 	59385,	1,250);
		
		AddItem("ammobox_762x39_20rnd", 	9140, 	15230,	1,250,null,{"AmmoBox_762x39Tracer_20Rnd"});
		AddItem("ammobox_762x39tracer_20rnd", 	11875, 	19795,	1,250);
		
		AddItem("ammobox_762x54_20rnd", 	7245, 	12075,	1,250,null,{"AmmoBox_762x54Tracer_20Rnd"});
		AddItem("ammobox_762x54tracer_20rnd", 	11875, 	19795,	1,250);
		
		AddItem("ammobox_9x19_25rnd", 	1345, 	2245,	1,250);
		AddItem("ammobox_9x39_20rnd", 	1775, 	2960,	1,250,null,{"AmmoBox_9x39AP_20Rnd"});
		AddItem("ammobox_9x39ap_20rnd", 	13200, 	22005,	1,250);

	#ifdef EXPANSIONMODWEAPONS	
		AddItem("AmmoBox_Expansion_46x30_25rnd", 			40,			80,			1,		250);
		AddItem("AmmoBox_Expansion_338_15rnd", 				24,			48,			1,		250);
		AddItem("Expansion_AmmoBox_8mm_15rnd", 				24,			48,			1,		250);
	#endif
	}
};