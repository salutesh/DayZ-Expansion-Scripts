/**
 * ExpansionMarketEyewear.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketEyewear: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 21;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_EYEWEAR";
		m_FileName = "Eyewear";

		AddItem("SportGlasses_Orange", 				17,		34,		1,		100, null, {"SportGlasses_Blue", "SportGlasses_Black", "SportGlasses_Green"});
		AddItem("SportGlasses_Blue", 				18,		36,		1,		100);
		AddItem("SportGlasses_Black", 				20,		40,		1,		100);
		AddItem("SportGlasses_Green", 				20,		40,		1,		100);
		
		AddItem("ThinFramesGlasses", 				20,		40,		1,		100);
		AddItem("ThickFramesGlasses", 				23,		46,		1,		100);
		
		AddItem("DesignerGlasses", 					30,		60,		1,		100);

		AddItem("AviatorGlasses", 					40,		80,		1,		100);
	
		AddItem("TacticalGoggles", 					60,		120,	1,		100);
		
		AddItem("NVGHeadstrap", 					200,	400,	1,		100);

		AddItem("EyePatch_Improvised", 				10,		20,		1,		100);
		
		AddItem("EyeMask_Black", 					30,		100,	1,		100, null, {"EyeMask_Blue","EyeMask_Christmas","EyeMask_Dead","EyeMask_NewYears","EyeMask_Red","EyeMask_Valentines","EyeMask_Yellow"});
		
	}
};