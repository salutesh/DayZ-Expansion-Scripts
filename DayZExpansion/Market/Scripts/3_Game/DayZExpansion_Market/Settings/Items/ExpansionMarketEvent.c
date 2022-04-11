/**
 * ExpansionMarketEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketEvent: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 60;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_EVENT";
		m_FileName = "Event";

		AddItem("EasterEgg", 								1,		15,		1,		250);
		AddItem("CrookedNose", 								1,		15,		1,		250);
		AddItem("Cauldron", 								1,		15,		1,		250);
		AddItem("WitchHat", 								1,		15,		1,		250);
		AddItem("WitchHood_Black", 							1,		15,		1,		250, null, {"WitchHood_Brown", "WitchHood_Red"});
		//AddItem("WitchHood_Brown", 							1,		15,		1,		250);
		//AddItem("WitchHood_Red", 							1,		15,		1,		250);
	}
};