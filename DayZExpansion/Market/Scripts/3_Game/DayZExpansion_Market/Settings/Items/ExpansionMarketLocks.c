/**
 * ExpansionMarketLocks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketLocks: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 36;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_LOCKS";
		m_FileName = "Locks";

		AddItem("CombinationLock", 				700,		1400,		1,		100);
		AddItem("CombinationLock4", 			900,		1800,		1,		100);
	#ifdef EXPANSIONMODBASEBUILDING		
		AddItem("ExpansionCodeLock", 			1500,		3000,		1,		100);
	#endif
	}
};