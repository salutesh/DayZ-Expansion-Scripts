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

		AddItem("SurgicalGloves_Blue", 				5,		10,		1,		100, null, {"SurgicalGloves_LightBlue", "SurgicalGloves_Green", "SurgicalGloves_White"});
		/*AddItem("SurgicalGloves_LightBlue",			5,		10,		1,		100);
		AddItem("SurgicalGloves_Green", 			5,		10,		1,		100);
		AddItem("SurgicalGloves_White", 			5,		10,		1,		100);*/
		
		AddItem("WorkingGloves_Yellow", 			25,		50,		1,		100, null, {"WorkingGloves_Black", "WorkingGloves_Beige", "WorkingGloves_Brown"});
		AddItem("WorkingGloves_Black", 				30,		60,		1,		100);
		AddItem("WorkingGloves_Beige", 				30,		60,		1,		100);
		AddItem("WorkingGloves_Brown", 				30,		60,		1,		100);
	
	#ifdef EXPANSIONMOD
		AddItem("TacticalGloves_Black", 			35,		70,		1,		100, null, {"TacticalGloves_Beige", "TacticalGloves_Green", "ExpansionTacticalGlovesDesert"});
		//AddItem("ExpansionTacticalGlovesDesert", 	35,		70,		1,		100);
	#else
		AddItem("TacticalGloves_Black", 			35,		70,		1,		100, null, {"TacticalGloves_Beige", "TacticalGloves_Green"});
	#endif
		//AddItem("TacticalGloves_Beige", 			35,		70,		1,		100);
		//AddItem("TacticalGloves_Green", 			35,		70,		1,		100);
	
		AddItem("OMNOGloves_Gray", 					40,		80,		1,		100, null, {"OMNOGloves_Brown"});
		//AddItem("OMNOGloves_Brown", 				40,		80,		1,		100);
		
		AddItem("NBCGlovesGray", 					65,		130,	1,		100);
		AddItem("NBCGlovesYellow", 					65,		130,	1,		100);
	}
};