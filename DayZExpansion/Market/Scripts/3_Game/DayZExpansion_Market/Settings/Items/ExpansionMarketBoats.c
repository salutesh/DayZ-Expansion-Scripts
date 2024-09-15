/**
 * ExpansionMarketBoats.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBoats: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 10;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BOATS";
		m_FileName = "Boats";
		
	#ifdef EXPANSIONMODVEHICLE
		//! AddItem("ExpansionAmphibiaBoat", 	15000,		17000,		1,		10,		{"TruckBattery","GlowPlug"});
		AddItem("ExpansionUtilityBoat", 		22000,		44000,		1,		10,		{"TruckBattery","GlowPlug"});
	#ifdef DAYZ_1_25
		AddItem("ExpansionZodiacBoat", 			22000,		44000,		1,		10,		{"TruckBattery","GlowPlug"});
	#else
		AddItem("Boat_01_Orange", 			22000,		44000,		1,		10,		{"SparkPlug"}, {"Boat_01_Black", "Boat_01_Blue", "Boat_01_Camo"});
	#endif
		AddItem("ExpansionLHD", 			300000000,	600000000,		1,		1,		{"TruckBattery","GlowPlug","HeadlightH7","HeadlightH7"});
	#endif
	}
};