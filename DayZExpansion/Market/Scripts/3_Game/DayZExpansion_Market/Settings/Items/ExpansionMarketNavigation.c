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
		
		AddItem("compass", 	960, 	1600,	1,100);
		AddItem("orienteeringcompass", 	330, 	555,	1,100);

		if (ExpansionStatic.GetCanonicalWorldName() == "namalsk")
		{
			AddItem("dzn_map_namalsk", 			545,		910,		1,		100);
		}
		else
		{
		AddItem("chernarusmap", 	545, 	910,	1,100);
		}

		AddItem("binoculars", 	915, 	1530,	1,100);
		AddItem("gpsreceiver", 	855, 	1425,	1,100,{"Battery9V"});
	#ifdef EXPANSIONMODNAVIGATION
		AddItem("ExpansionGPS", 950, 	1450,	1,		100,	{"Battery9V"});
	#endif
	}
};