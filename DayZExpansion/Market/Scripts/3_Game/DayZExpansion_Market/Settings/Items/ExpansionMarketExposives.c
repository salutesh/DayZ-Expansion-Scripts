/**
 * ExpansionMarketExplosives.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketExplosives: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 20;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_EXPLOSIVES" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_GRENADES");
		m_FileName = "Explosives_And_Grenades";
		
		// EXPLOSIVE GRANADES
		AddItem("RGD5Grenade", 				300,		600,		1,		100,	0);
		AddItem("M67Grenade", 				300,		600,		1,		100,	0);
		
		// FLASHBANGS
		AddItem("FlashGrenade", 			250,		500,		1,		100,	0);

		// SMOKE GRANADES
	#ifdef EXPANSIONMOD
		AddItem("M18SmokeGrenade_Red", 		200,		400,		1,		100,	0, null, {"M18SmokeGrenade_Green", "M18SmokeGrenade_Yellow", "M18SmokeGrenade_Purple", "M18SmokeGrenade_White", "Expansion_M18SmokeGrenade_Teargas"});
		AddItem("Expansion_M18SmokeGrenade_Teargas", 	260,	520,	1,		100,	0);
	#else
		AddItem("M18SmokeGrenade_Red", 		200,		400,		1,		100,	0, null, {"M18SmokeGrenade_Green", "M18SmokeGrenade_Yellow", "M18SmokeGrenade_Purple", "M18SmokeGrenade_White"});
	#endif
		/*AddItem("M18SmokeGrenade_Green", 	200,		400,		1,		100,	0);
		AddItem("M18SmokeGrenade_Yellow", 	200,		400,		1,		100,	0);
		AddItem("M18SmokeGrenade_Purple", 	200,		400,		1,		100,	0);
		AddItem("M18SmokeGrenade_White", 	200,		400,		1,		100,	0);*/
		AddItem("RDG2SmokeGrenade_Black", 	200,		400,		1,		100,	0, null, {"RDG2SmokeGrenade_White"});
		//AddItem("RDG2SmokeGrenade_White", 	200,		400,		1,		100,	0 );
		
	#ifdef EXPANSIONMOD
		// Airdrop smoke
		//AddItem("ExpansionSupplySignal", 	1000000,	2000000,	1,		100,	1);

		// EXPLOSIVE CHARGES
		AddItem("ExpansionSatchel", 		500,		1000,		1,		100,	0);
	#endif
	}
};