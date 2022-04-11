/**
 * ExpansionMarketFishing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketFishing: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 23;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FISHING";
		m_FileName = "Fishing";
		
		AddItem("Worm", 			2,		4,			1,		100);
		AddItem("BoneHook", 		5,		10,			1,		100);
		AddItem("Hook", 			7,		14,			1,		100);
		AddItem("FishingRod", 		60,		120,		1,		100);
	}
};