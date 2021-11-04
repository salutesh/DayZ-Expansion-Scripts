/**
 * ExpansionMarketAmmo.c
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

class ExpansionMarketAmmo: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 1;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_AMMO";
		m_FileName = "Ammo";

		AddItem("Ammo_12gaPellets", 						5,		10,		1,		500);
		AddItem("Ammo_12gaRubberSlug", 						4,		8,		1,		500);
		AddItem("Ammo_12gaSlug", 							5,		10,		1,		500);
		
		AddItem("Ammo_22", 									2,		4,		1,		500);
		
		AddItem("Ammo_308Win", 								2,		4,		1,		500,	null, {"Ammo_308WinTracer"});
		AddItem("Ammo_308WinTracer", 						4,		8,		1,		500);
		
		AddItem("Ammo_357", 								2,		4,		1,		500);
		AddItem("Ammo_380", 								2,		4,		1,		500);
		AddItem("Ammo_45ACP", 								2,		4,		1,		500);
		
		AddItem("Ammo_545x39", 								2,		4,		1,		500, null, {"Ammo_545x39Tracer"});
		AddItem("Ammo_545x39Tracer", 						4,		8,		1,		500);
		
		AddItem("Ammo_556x45", 								2,		4,		1,		500, null, {"Ammo_556x45Tracer"});
		AddItem("Ammo_556x45Tracer", 						4,		8,		1,		500);
		
		AddItem("Ammo_762x39", 								2,		4,		1,		500, null, {"Ammo_762x39Tracer"});
		AddItem("Ammo_762x39Tracer", 						4,		8,		1,		500);
		
		AddItem("Ammo_762x54", 								2,		4,		1,		500, null, {"Ammo_762x54Tracer"});
		AddItem("Ammo_762x54Tracer", 						4,		8,		1,		500);
		
		AddItem("Ammo_9x19", 								2,		4,		1,		500);
		AddItem("Ammo_9x39", 								2,		4,		1,		500, null, {"Ammo_9x39AP"});
		AddItem("Ammo_9x39AP", 								4,		8,		1,		500);

		AddItem("Ammo_Flare", 								7,		14,		1,		500, null, {"Ammo_FlareBlue", "Ammo_FlareGreen", "Ammo_FlareRed"});

	#ifdef EXPANSIONMOD
		AddItem("Expansion_Ammo_ArrowBolt", 				5,		10,		1,		500);

		AddItem("ExpansionAmmoLAW", 						400,	800,	1,		500);
		AddItem("ExpansionAmmoRPG", 						400,	800,	1,		500);
		
		AddItem("Ammo_Expansion_M203_Smoke_White", 			300,	600,	1,		500, null, {"Ammo_Expansion_M203_Smoke_Red", "Ammo_Expansion_M203_Smoke_Green", "Ammo_Expansion_M203_Smoke_Yellow", "Ammo_Expansion_M203_Smoke_Purple"});
		/*AddItem("Ammo_Expansion_M203_Smoke_Red", 			300,	600,	1,		500);
		AddItem("Ammo_Expansion_M203_Smoke_Green", 			300,	600,	1,		500);
		AddItem("Ammo_Expansion_M203_Smoke_Yellow", 		300,	600,	1,		500);
		AddItem("Ammo_Expansion_M203_Smoke_Purple", 		300,	600,	1,		500);*/

		AddItem("Ammo_Expansion_M203_Smoke_Teargas", 		450,	900,	1,		500);

		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White", 	300,	600,	1,		500, null, {"Ammo_Expansion_M203_Sticky_Smoke_Red", "Ammo_Expansion_M203_Sticky_Smoke_Green", "Ammo_Expansion_M203_Sticky_Smoke_Yellow", "Ammo_Expansion_M203_Sticky_Smoke_Purple"});
		/*AddItem("Ammo_Expansion_M203_Sticky_Smoke_Red", 	300,	600,	1,		500);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Green", 	300,	600,	1,		500);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Yellow", 	300,	600,	1,		500);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Purple", 	300,	600,	1,		500);*/
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas", 450,	900,	1,		500);

		AddItem("Ammo_Expansion_M203_HE", 					500,	1000,	1,		500);
		
		AddItem("Ammo_Expansion_46x30", 					2,		5,	  	1,		500);
		AddItem("Ammo_Expansion_338", 						2,		5,	  	1,		500);
		AddItem("Expansion_Ammo_8mm", 						2,		5,	  	1,		500);
	#endif
	}
};