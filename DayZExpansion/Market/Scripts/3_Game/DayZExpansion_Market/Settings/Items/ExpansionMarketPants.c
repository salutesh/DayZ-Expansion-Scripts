/**
 * ExpansionMarketPants.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketPants: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 45;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_PANTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_SHORTS");
		m_FileName = "Pants_And_Shorts";

		AddItem("MedicalScrubsPants_Blue",			10,		20,		1,		100, null, {"MedicalScrubsPants_Green", "MedicalScrubsPants_White"});
		
		AddItem("TrackSuitPants_Black", 			15,		30,		1,		100, null, {"TrackSuitPants_Blue", "TrackSuitPants_Green", "TrackSuitPants_Red", "TrackSuitPants_LightBlue"});
		
		AddItem("PrisonUniformPants", 				17,		34,		1,		100);

		AddItem("Breeches_Pink", 					20,		40,		1,		100, null, {"Breeches_Red", "Breeches_White", "Breeches_Blue", "Breeches_Beetcheck", "Breeches_Beige", "Breeches_Browncheck", "Breeches_Black", "Breeches_Blackcheck", "Breeches_Green"});
		AddItem("Breeches_Beige", 					21,		42,		1,		100);
		AddItem("Breeches_Browncheck", 				21,		42,		1,		100);
		AddItem("Breeches_Black", 					22,		44,		1,		100);
		AddItem("Breeches_Blackcheck", 				22,		44,		1,		100);
		AddItem("Breeches_Green", 					24,		48,		1,		100);

		AddItem("SlacksPants_Beige", 				20,		40,		1,		100, null, {"SlacksPants_Blue", "SlacksPants_DarkGrey", "SlacksPants_LightGrey", "SlacksPants_White", "SlacksPants_Brown", "SlacksPants_Black", "SlacksPants_Khaki"});
		AddItem("SlacksPants_Brown", 				21,		42,		1,		100);
		AddItem("SlacksPants_Black", 				22,		44,		1,		100);
		AddItem("SlacksPants_Khaki", 				24,		48,		1,		100);

		AddItem("CanvasPantsMidi_Blue", 			24,		48,		1,		100, null, {"CanvasPantsMidi_Grey", "CanvasPantsMidi_Red", "CanvasPantsMidi_Violet", "CanvasPantsMidi_Beige"});
		AddItem("CanvasPantsMidi_Beige", 			25,		50,		1,		100);	

		AddItem("CanvasPants_Blue", 				26,		52,		1,		100, null, {"CanvasPants_Grey", "CanvasPants_Red", "CanvasPants_Violet", "CanvasPants_Beige"});
		AddItem("CanvasPants_Beige", 				28,		56,		1,		100);	

		AddItem("JumpsuitPants_Blue", 				28,		56,		1,		100, null, {"JumpsuitPants_Green", "JumpsuitPants_Grey", "JumpsuitPants_Red"});
		
		AddItem("PolicePants", 						28,		56,		1,		100);

		AddItem("ParamedicPants_Blue", 				35,		70,		1,		100, null, {"ParamedicPants_Crimson", "ParamedicPants_Green"});
		
		AddItem("FirefightersPants_Beige", 			35,		70,		1,		100, null, {"FirefightersPants_Black"});
		
		AddItem("CargoPants_Beige", 				35,		70,		1,		100, null, {"CargoPants_Black", "CargoPants_Blue", "CargoPants_Green", "CargoPants_Grey"});
		
		AddItem("ShortJeans_Blue", 					38,		76,		1,		100, null, {"ShortJeans_Brown", "ShortJeans_Darkblue", "ShortJeans_Red", "ShortJeans_Black", "ShortJeans_Green"});
		AddItem("ShortJeans_Black", 				40,		80,		1,		100);
		AddItem("ShortJeans_Green", 				42,		84,		1,		100);

		AddItem("Jeans_Blue", 						40,		80,		1,		100, null, {"Jeans_Brown", "Jeans_Grey", "Jeans_BlueDark", "Jeans_Green", "Jeans_Black"});
		AddItem("Jeans_Green", 						42,		84,		1,		100);
		AddItem("Jeans_Black", 						45,		90,		1,		100);
		
		AddItem("TTSKOPants", 						55,		110,	1,		100);

		AddItem("BDUPants", 						65,		130,	1,		100);

		AddItem("USMCPants_Desert", 				65,		130,	1,		100, null, {"USMCPants_Woodland"});
		AddItem("USMCPants_Woodland", 				70,		140,	1,		100);

		AddItem("PolicePantsOrel", 					70,		140,	1,		100);

		AddItem("HunterPants_Winter", 				90,		180,	1,		100, null, {"HunterPants_Autumn", "HunterPants_Brown", "HunterPants_Spring", "HunterPants_Summer"});	
		AddItem("HunterPants_Autumn", 				100,	200,	1,		100);
		AddItem("HunterPants_Brown", 				100,	200,	1,		100);
		AddItem("HunterPants_Spring", 				100,	200,	1,		100);
		AddItem("HunterPants_Summer", 				100,	200,	1,		100);
	
		AddItem("GorkaPants_Summer", 				130,	260,	1,		100, null, {"GorkaPants_Autumn", "GorkaPants_Flat", "GorkaPants_PautRev"});	
		
		AddItem("NBCPantsGray", 					250,	500,	1,		100, null, {"NBCPantsYellow"});
		AddItem("NBCPantsYellow", 					200,	400,	1,		100);
		//! 1.21+
		AddItem("Chainmail_Leggings", 				350,	600,	1,		100);
	}
};