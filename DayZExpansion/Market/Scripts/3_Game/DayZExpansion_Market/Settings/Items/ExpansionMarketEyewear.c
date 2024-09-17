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

		AddItem("sportglasses_orange", 	260, 	430,	1,100,null,{"SportGlasses_Blue","SportGlasses_Black","SportGlasses_Green"});
		AddItem("sportglasses_blue", 	260, 	430,	1,100);
		AddItem("sportglasses_black", 	260, 	430,	1,100);
		AddItem("sportglasses_green", 	260, 	430,	1,100);
		
		AddItem("thinframesglasses", 	210, 	350,	1,100);
		AddItem("thickframesglasses", 	210, 	350,	1,100);
		
		AddItem("designerglasses", 	275, 	455,	1,100);

		AddItem("aviatorglasses", 	505, 	840,	1,100);
	
		AddItem("tacticalgoggles", 	5490, 	9150,	1,100);
		
		AddItem("nvgheadstrap", 	9695, 	16160,	1,100);

		#ifndef DAYZ_1_25
		AddItem("skigoggles_blackyellow", 	145, 	240,	1,100,null,{"skigoggles_whiteclear","skigoggles_whitedark"});
		#endif

		// Crafted item !
		//AddItem("EyePatch_Improvised", 				10,		20,		1,		100);
		
		AddItem("eyemask_black", 	355, 	590,	1,100,null,{"EyeMask_Blue","EyeMask_Christmas","EyeMask_Dead","EyeMask_NewYears","EyeMask_Red","EyeMask_Valentines","EyeMask_Yellow"});
		
	}
};