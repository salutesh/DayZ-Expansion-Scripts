/**
 * ExpansionMarketCoatsAndJackets.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCoatsAndJackets: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 15;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_COATS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_JACKETS");
		m_FileName = "Coats_And_Jackets";

		AddItem("LabCoat", 								10,		20,		1,		100);

		AddItem("TrackSuitJacket_Black", 				15,		30,		1,		100, null, {"TrackSuitJacket_Blue", "TrackSuitJacket_Green", "TrackSuitJacket_LightBlue", "TrackSuitJacket_Red"});
		/*AddItem("TrackSuitJacket_Blue", 				15,		30,		1,		100);
		AddItem("TrackSuitJacket_Green", 				15,		30,		1,		100);
		AddItem("TrackSuitJacket_LightBlue", 			15,		30,		1,		100);
		AddItem("TrackSuitJacket_Red", 					15,		30,		1,		100);*/

		AddItem("DenimJacket", 							18,		36,		1,		100);

		AddItem("WoolCoat_Red", 						18,		36,		1,		100, null, {"WoolCoat_Blue", "WoolCoat_Beige", "WoolCoat_RedCheck", "WoolCoat_BlueCheck", "WoolCoat_GreyCheck", "WoolCoat_BrownCheck", "WoolCoat_Black", "WoolCoat_BlackCheck", "WoolCoat_Green"});
		/*AddItem("WoolCoat_Blue", 						18,		36,		1,		100);
		AddItem("WoolCoat_Beige", 						18,		36,		1,		100);
		AddItem("WoolCoat_RedCheck", 					18,		36,		1,		100);
		AddItem("WoolCoat_BlueCheck", 					18,		36,		1,		100);
		AddItem("WoolCoat_GreyCheck", 					18,		36,		1,		100);
		AddItem("WoolCoat_BrownCheck", 					18,		36,		1,		100);*/
		AddItem("WoolCoat_Black", 						19,		38,		1,		100);
		AddItem("WoolCoat_BlackCheck", 					19,		38,		1,		100);
		AddItem("WoolCoat_Green", 						20,		40,		1,		100);
	
	#ifdef EXPANSIONMOD
		AddItem("RidersJacket_Black", 					20,		40,		1,		100, null, {"ExpansionRidersJacketBrown", "ExpansionRidersJacketDarkBlue"});
		/*AddItem("ExpansionRidersJacketBrown", 		20,		40,		1,		100);
		AddItem("ExpansionRidersJacketDarkBlue", 		20,		40,		1,		100);*/
	#else
		AddItem("RidersJacket_Black", 					20,		40,		1,		100);
	#endif
	
		AddItem("FirefighterJacket_Beige", 				24,		48,		1,		100, null, {"FirefighterJacket_Black"});
		AddItem("FirefighterJacket_Black", 				24,		48,		1,		100);
		
		AddItem("JumpsuitJacket_Blue", 					26,		52,		1,		100, null, {"JumpsuitJacket_Gray", "JumpsuitJacket_Green", "JumpsuitJacket_Red"});
		/*AddItem("JumpsuitJacket_Gray", 				26,		52,		1,		100);
		AddItem("JumpsuitJacket_Green", 				26,		52,		1,		100);
		AddItem("JumpsuitJacket_Red", 					26,		52,		1,		100);*/

		AddItem("BomberJacket_Brown", 					26,		52,		1,		100, null, {"BomberJacket_Blue", "BomberJacket_Grey", "BomberJacket_Maroon", "BomberJacket_SkyBlue", "BomberJacket_Black", "BomberJacket_Olive"});
		/*AddItem("BomberJacket_Blue", 					26,		52,		1,		100);
		AddItem("BomberJacket_Grey", 					26,		52,		1,		100);
		AddItem("BomberJacket_Maroon", 					26,		52,		1,		100);
		AddItem("BomberJacket_SkyBlue", 				26,		52,		1,		100);*/
		AddItem("BomberJacket_Black", 					28,		56,		1,		100);
		AddItem("BomberJacket_Olive", 					30,		60,		1,		100);

		AddItem("QuiltedJacket_Blue", 					26,		52,		1,		100, null, {"QuiltedJacket_Red", "QuiltedJacket_Grey", "QuiltedJacket_Orange", "QuiltedJacket_Yellow", "QuiltedJacket_Violet", "QuiltedJacket_Black", "QuiltedJacket_Green"});
		/*AddItem("QuiltedJacket_Red", 					26,		52,		1,		100);
		AddItem("QuiltedJacket_Grey", 					26,		52,		1,		100);
		AddItem("QuiltedJacket_Orange", 				26,		52,		1,		100);
		AddItem("QuiltedJacket_Yellow", 				26,		52,		1,		100);
		AddItem("QuiltedJacket_Violet", 				26,		52,		1,		100);*/
		AddItem("QuiltedJacket_Black", 					28,		56,		1,		100);
		AddItem("QuiltedJacket_Green", 					30,		60,		1,		100);

		AddItem("PrisonUniformJacket", 					28,		56,		1,		100);

		AddItem("PoliceJacketOrel", 					32,		64,		1,		100);
		AddItem("PoliceJacket", 						34,		68,		1,		100);

		AddItem("ParamedicJacket_Blue", 				36,		72,		1,		100, null, {"ParamedicJacket_Crimson", "ParamedicJacket_Green"});
		/*AddItem("ParamedicJacket_Crimson", 				36,		72,		1,		100);
		AddItem("ParamedicJacket_Green", 				36,		72,		1,		100);*/

		AddItem("HikingJacket_Black", 					36,		72,		1,		100, null, {"HikingJacket_Red", "HikingJacket_Blue", "HikingJacket_Green"});
		/*AddItem("HikingJacket_Red", 					36,		72,		1,		100);
		AddItem("HikingJacket_Blue", 					38,	76,		1,		100);*/
		AddItem("HikingJacket_Green", 					40,		80,		1,		100);
	
		AddItem("Raincoat_Pink", 						70,		140,	1,		100, null, {"Raincoat_Orange", "Raincoat_Yellow", "Raincoat_Red", "Raincoat_Blue", "Raincoat_Black", "Raincoat_Green"});
		/*AddItem("Raincoat_Orange", 					70,		140,	1,		100);
		AddItem("Raincoat_Yellow", 						70,		140,	1,		100);
		AddItem("Raincoat_Red", 						70,		140,	1,		100);
		AddItem("Raincoat_Blue", 						70,		140,	1,		100);
		AddItem("Raincoat_Black", 						80,		160,	1,		100);*/
		AddItem("Raincoat_Green", 						100,	200,	1,		100);

		AddItem("TTsKOJacket_Camo", 					100,	200,	1,		100);		

		AddItem("BDUJacket", 							100,	200,	1,		100);

		AddItem("HuntingJacket_Autumn", 				120,	240,	1,		100, null, {"HuntingJacket_Brown", "HuntingJacket_Spring", "HuntingJacket_Summer", "HuntingJacket_Winter"});
		/*AddItem("HuntingJacket_Brown", 					120,	240,	1,		100);
		AddItem("HuntingJacket_Spring", 				120,	240,	1,		100);
		AddItem("HuntingJacket_Summer", 				120,	240,	1,		100);
		AddItem("HuntingJacket_Winter", 				120,	240,	1,		100);*/

		AddItem("M65Jacket_Black", 						130,	260,	1,		100, null, {"M65Jacket_Khaki", "M65Jacket_Tan", "M65Jacket_Olive"});
		/*AddItem("M65Jacket_Khaki", 						130,	260,	1,		100);
		AddItem("M65Jacket_Tan", 						130,	260,	1,		100);
		AddItem("M65Jacket_Olive", 						130,	260,	1,		100);*/
		
		AddItem("GorkaEJacket_Summer", 					140,	280,	1,		100, null, {"GorkaEJacket_Flat", "GorkaEJacket_Autumn", "GorkaEJacket_PautRev"});
		/*AddItem("GorkaEJacket_Flat", 					140,	280,	1,		100);
		AddItem("GorkaEJacket_Autumn", 					140,	280,	1,		100);
		AddItem("GorkaEJacket_PautRev", 				140,	280,	1,		100);*/
		
		AddItem("USMCJacket_Desert", 					140,	280,	1,		100, null, {"USMCJacket_Woodland"});
		AddItem("USMCJacket_Woodland", 					155,	310,	1,		100);
		
		AddItem("NBCJacketGray", 						250,	500,	1,		100);
		AddItem("NBCJacketYellow", 						250,	500,	1,		100);
		
	#ifndef DAYZ_1_20
	//! 1.21+
		AddItem("Chainmail", 							375,	650,	1,		100);
	#endif
	}
};