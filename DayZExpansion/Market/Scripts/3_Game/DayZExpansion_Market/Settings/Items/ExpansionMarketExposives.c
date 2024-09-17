/**
 * ExpansionMarketExplosives.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		
		// EXPLOSIVE GRENADES
		AddItem("rgd5grenade", 	13250, 	22085,	1,100);
		AddItem("m67grenade", 	13250, 	22085,	1,100);
		
		// FLASHBANGS
		AddItem("flashgrenade", 	2070, 	3450,	1,100);

		//Base Raiding
		AddItem("remotedetonator", 	7125, 	11875,	1,100);
		AddItem("RemoteDetonatorTrigger", 		300,		600,		1,		100);
		AddItem("ImprovisedExplosive", 			300,		600,		1,		100);
		AddItem("plastic_explosive", 	10180, 	16965,	1,100);
		AddItem("grenade_chemgas", 	290, 	480,	1,100);

		// SMOKE GRANADES
	#ifdef EXPANSIONMODWEAPONS
		AddItem("m18smokegrenade_red", 	2070, 	3450,	1,100,null,{"M18SmokeGrenade_Green","M18SmokeGrenade_Yellow","M18SmokeGrenade_Purple","M18SmokeGrenade_White","Expansion_M18SmokeGrenade_Teargas"});
		AddItem("Expansion_M18SmokeGrenade_Teargas", 	260,	520,	1,		100);
	#else
		AddItem("m18smokegrenade_red", 	2070, 	3450,	1,100,null,{"M18SmokeGrenade_Green","M18SmokeGrenade_Yellow","M18SmokeGrenade_Purple","M18SmokeGrenade_White"});
	#endif
		AddItem("rdg2smokegrenade_black", 	1655, 	2760,	1,100,null,{"RDG2SmokeGrenade_White"});
		AddItem("rdg2smokegrenade_white", 	1655, 	2760,	1,100);
		
	#ifdef EXPANSIONMODMISSIONS
		// Airdrop smoke
		AddItem("ExpansionSupplySignal", 	1000000,	2000000,	1,		100);
	#endif

	#ifdef EXPANSIONMODBASEBUILDING

		// EXPLOSIVE CHARGES
		AddItem("ExpansionSatchel", 		500,		1000,		1,		100);
	#endif

		//! MINES
		AddItem("landminetrap", 	15265, 	25445,	1,100);
		AddItem("claymoremine", 	15975, 	26625,	1,100);
	}
};