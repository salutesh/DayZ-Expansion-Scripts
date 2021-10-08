/**
 * ExpansionMarketAmmoBoxes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

		AddItem("AmmoBox_00buck_10rnd", 					40,			80,			1,		250,	0);
		AddItem("AmmoBox_12gaSlug_10Rnd", 					40,			80,			1,		250,	0);
		AddItem("AmmoBox_12gaRubberSlug_10Rnd", 			32,			64,			1,		250,	0);
		
		AddItem("AmmoBox_22_50Rnd", 						80,			160,		1,		250,	0);
		
		AddItem("AmmoBox_308Win_20Rnd", 					32,			64,			1,		250,	0, null, {"AmmoBox_308WinTracer_20Rnd"});
		AddItem("AmmoBox_308WinTracer_20Rnd", 				64,			128,		1,		250,	0);
		
		AddItem("AmmoBox_357_20Rnd", 						32,			64,			1,		250,	0);
		AddItem("AmmoBox_380_35rnd", 						56,			112,		1,		250,	0);
		AddItem("AmmoBox_45ACP_25rnd", 						40,			80,			1,		250,	0);
		
		AddItem("AmmoBox_545x39_20Rnd", 					32,			64,			1,		250,	0, null, {"AmmoBox_545x39Tracer_20Rnd"});
		AddItem("AmmoBox_545x39Tracer_20Rnd", 				64,			128,		1,		250,	0);
		
		AddItem("AmmoBox_556x45_20Rnd", 					32,			64,			1,		250,	0, null, {"AmmoBox_556x45Tracer_20Rnd"});
		AddItem("AmmoBox_556x45Tracer_20Rnd", 				64,			128,		1,		250,	0);
		
		AddItem("AmmoBox_762x39_20Rnd", 					32,			64,			1,		250,	0, null, {"AmmoBox_762x39Tracer_20Rnd"});
		AddItem("AmmoBox_762x39Tracer_20Rnd", 				64,			128,		1,		250,	0);
		
		AddItem("AmmoBox_762x54_20Rnd", 					32,			64,			1,		250,	0, null, {"AmmoBox_762x54Tracer_20Rnd"});
		AddItem("AmmoBox_762x54Tracer_20Rnd", 				64,			128,		1,		250,	0);
		
		AddItem("AmmoBox_9x19_25rnd", 						40,			80,			1,		250,	0);
		AddItem("AmmoBox_9x39_20rnd", 						16,			32,			1,		250,	0, null, {"AmmoBox_9x39AP_20rnd"});
		AddItem("AmmoBox_9x39AP_20rnd", 					64,			128,		1,		250,	0);
	#ifdef EXPANSIONMOD	
		AddItem("AmmoBox_Expansion_46x30_25rnd", 			40,			80,			1,		250,	0);
		AddItem("AmmoBox_Expansion_338_15rnd", 				24,			48,			1,		250,	0);
		AddItem("Expansion_AmmoBox_8mm_15rnd", 				24,			48,			1,		250,	0);
	#endif
	}
};