/**
 * ExpansionMarketAmmo.c
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

class ExpansionMarketAmmo: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 1;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_AMMO";
		m_FileName = "Ammo";

		AddItem("ammo_12gapellets", 	30, 	50,	1,500);
		AddItem("ammo_12garubberslug", 	45, 	80,	1,500);
		AddItem("ammo_12gaslug", 	25, 	45,	1,500);
		
		AddItem("ammo_22", 	30, 	50,	1,500);
		
		AddItem("ammo_308win", 	40, 	65,	1,500,null,{"Ammo_308WinTracer"});
		AddItem("ammo_308wintracer", 	445, 	745,	1,500);
		
		AddItem("ammo_357", 	15, 	25,	1,500);
		AddItem("ammo_380", 	35, 	55,	1,500);
		AddItem("ammo_45acp", 	75, 	120,	1,500);
		
		AddItem("ammo_545x39", 	85, 	140,	1,500,null,{"Ammo_545x39Tracer"});
		AddItem("ammo_545x39tracer", 	105, 	180,	1,500);
		
		AddItem("ammo_556x45", 	305, 	505,	1,500,null,{"Ammo_556x45Tracer"});
		AddItem("ammo_556x45tracer", 	225, 	370,	1,500);
		
		AddItem("ammo_762x39", 	90, 	150,	1,500,null,{"Ammo_762x39Tracer"});
		AddItem("ammo_762x39tracer", 	425, 	710,	1,500);
		
		AddItem("ammo_762x54", 	155, 	255,	1,500,null,{"Ammo_762x54Tracer"});
		AddItem("ammo_762x54tracer", 	595, 	990,	1,500);
		
		AddItem("ammo_9x19", 	15, 	25,	1,500);
		AddItem("ammo_9x39", 	115, 	190,	1,500,null,{"Ammo_9x39AP"});
		AddItem("ammo_9x39ap", 	390, 	655,	1,500);

		AddItem("ammo_flare", 	60, 	100,	1,500,null,{"Ammo_FlareBlue","Ammo_FlareGreen","Ammo_FlareRed"});

		AddItem("ammo_40mm_explosive", 	4490, 	7480,	1,500,null,{"Ammo_40mm_Chemgas","Ammo_40mm_Smoke_Black","Ammo_40mm_Smoke_Green","Ammo_40mm_Smoke_White","Ammo_40mm_Smoke_Red"});

		AddItem("ammo_huntingbolt", 	215, 	355,	1,500);
	#ifdef EXPANSIONMODWEAPONS
		AddItem("ExpansionAmmoLAW", 						400,	800,	1,		500);
		AddItem("ExpansionAmmoRPG", 						400,	800,	1,		500);

		AddItem("Ammo_Expansion_M203_Smoke_Teargas", 		450,	900,	1,		500);

		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White", 	300,	600,	1,		500, null, {"Ammo_Expansion_M203_Sticky_Smoke_Red", "Ammo_Expansion_M203_Sticky_Smoke_Green", "Ammo_Expansion_M203_Sticky_Smoke_Yellow", "Ammo_Expansion_M203_Sticky_Smoke_Purple"});
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas", 450,	900,	1,		500);
		
		AddItem("Ammo_Expansion_46x30", 					2,		5,	  	1,		500);
		AddItem("Ammo_Expansion_338", 						2,		5,	  	1,		500);
		AddItem("Expansion_Ammo_8mm", 						2,		5,	  	1,		500);
		AddItem("Expansion_Ammo_Taser", 						1,		1,	  	1,		1);
	#endif
	}
};
