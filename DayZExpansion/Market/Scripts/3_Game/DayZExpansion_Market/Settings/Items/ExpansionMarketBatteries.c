/**
 * ExpansionMarketBatteries.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

		AddItem("Battery9V",					10,			20,			1,		100,	0);
		AddItem("CarBattery", 					100,		200,		1,		100,	0);
		AddItem("TruckBattery", 				180,		360,		1,		100,	0);
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionHelicopterBattery", 	250,		500,		1,		100,	0);
		//AddItem("ExpansionAircraftBattery", 	250,		500,		1,		100,	0);
	#endif
	}
};