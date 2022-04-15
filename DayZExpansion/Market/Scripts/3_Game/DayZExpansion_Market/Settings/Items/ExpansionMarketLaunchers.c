/**
 * ExpansionMarketLaunchers.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketLaunchers: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 33;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_LAUNCHERS";
		m_FileName = "Launchers";
	
		AddItem("Flaregun", 			500,		1000,		1,		100);
	#ifdef EXPANSIONMODWEAPONS
		AddItem("ExpansionLAW", 		2500,		5000,		1,		100);
		AddItem("ExpansionRPG7", 		2500,		5000,		1,		100);
		AddItem("Expansion_M79", 		2500,		5000,		1,		100);
	#endif
	}
};