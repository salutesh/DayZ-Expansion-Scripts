/**
 * ExpansionMarketNavigation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketNavigation: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 43;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_NAVIGATION";
		m_FileName = "Navigation";
		
		AddItem("Compass", 						10,		20,		1,		100);
		AddItem("OrienteeringCompass", 			10,		20,		1,		100);

		if (ExpansionStatic.GetCanonicalWorldName() == "namalsk")
		{
			AddItem("dzn_map_namalsk", 			15,		30,		1,		100);
		}
		else
		{
			AddItem("ChernarusMap", 			15,		30,		1,		100);
		}

		AddItem("Binoculars", 					30,		60,		1,		100);
		AddItem("GPSReceiver", 					300,	600,	1,		100,	{"Battery9V"});
	#ifdef EXPANSIONMODNAVIGATION
		AddItem("ExpansionGPS", 				300,	600,	1,		100,	{"Battery9V"});
	#endif
	}
};