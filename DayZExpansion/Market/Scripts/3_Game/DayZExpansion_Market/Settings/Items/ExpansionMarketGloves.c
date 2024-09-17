/**
 * ExpansionMarketGloves.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketGloves: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 26;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_GLOVES";
		m_FileName = "Gloves";

		AddItem("surgicalgloves_blue", 	640, 	1065,	1,100,null,{"SurgicalGloves_LightBlue","SurgicalGloves_Green","SurgicalGloves_White"});
		
		AddItem("workinggloves_yellow", 	370, 	615,	1,100,null,{"WorkingGloves_Black","WorkingGloves_Beige","WorkingGloves_Brown"});
		AddItem("workinggloves_black", 	375, 	625,	1,100);
		AddItem("workinggloves_beige", 	370, 	615,	1,100);
		AddItem("workinggloves_brown", 	370, 	615,	1,100);
	
	#ifdef EXPANSIONMOD
		AddItem("tacticalgloves_black", 	1055, 	1755,	1,100,null,{"TacticalGloves_Beige","TacticalGloves_Green","ExpansionTacticalGlovesDesert"});
	#else
		AddItem("tacticalgloves_black", 	1055, 	1755,	1,100,null,{"TacticalGloves_Beige","TacticalGloves_Green"});
	#endif
		AddItem("tacticalgloves_beige", 	1055, 	1755,	1,100);
		AddItem("tacticalgloves_green", 	1055, 	1755,	1,100);
	
		AddItem("omnogloves_gray", 	4430, 	7385,	1,100,null,{"OMNOGloves_Brown"});
		
		AddItem("nbcglovesgray", 	7900, 	13165,	1,100,null,{"NBCGlovesYellow"});
		
		//! 1.21+
		AddItem("woolgloves_green", 	3905, 	6505,	1,100,null,{"WoolGloves_Black","WoolGloves_Tan","WoolGloves_White","woolgloves_christmasblue","woolgloves_christmasred"});
		AddItem("woolglovesfingerless_green", 	3970, 	6620,	1,100,null,{"WoolGlovesFingerless_Black","WoolGlovesFingerless_Tan","WoolGlovesFingerless_White","woolglovesfingerless_christmasblue","woolglovesfingerless_christmasred"});

		#ifndef DAYZ_1_25
		AddItem("skigloves_90s", 	145, 	240,	1,100,null,{"skigloves_blue","skigloves_red"});
		AddItem("skigloves_blue", 	145, 	240,	1,100);
		AddItem("skigloves_red", 	145, 	240,	1,100);
		#endif
		
		AddItem("paddedgloves_beige", 	175, 	295,	1,100,null,{"paddedgloves_brown","paddedgloves_threat"});
		AddItem("paddedgloves_brown", 	280, 	470,	1,100);
		AddItem("paddedgloves_threat", 	650, 	1080,	1,100);
	}
};