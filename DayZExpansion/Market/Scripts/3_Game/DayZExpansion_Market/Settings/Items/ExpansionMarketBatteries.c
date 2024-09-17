/**
 * ExpansionMarketBatteries.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBatteries: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 7;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BATTERIES";
		m_FileName = "Batteries";

		AddItem("battery9v", 	155, 	260,	1,100);
		AddItem("carbattery", 	485, 	810,	1,100);
		AddItem("truckbattery", 	535, 	895,	1,100);
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionHelicopterBattery", 	735, 	1095,		1,		100);
		//AddItem("ExpansionAircraftBattery", 	735, 	1095,		1,		100);
	#endif
	}
};