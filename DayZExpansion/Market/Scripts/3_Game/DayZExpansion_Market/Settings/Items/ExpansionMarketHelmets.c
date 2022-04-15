/**
 * ExpansionMarketHelmets.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHelmets: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 30;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_HELMETS";
		m_FileName = "Helmets";

		AddItem("ConstructionHelmet_Blue", 			30,		60,		1,		100, null, {"ConstructionHelmet_Orange", "ConstructionHelmet_Red", "ConstructionHelmet_White", "ConstructionHelmet_Yellow", "ConstructionHelmet_Lime"});
		/*AddItem("ConstructionHelmet_Orange", 		30,		60,		1,		100);
		AddItem("ConstructionHelmet_Red", 			30,		60,		1,		100);
		AddItem("ConstructionHelmet_White", 		30,		60,		1,		100);
		AddItem("ConstructionHelmet_Yellow", 		30,		60,		1,		100);*/
		AddItem("ConstructionHelmet_Lime", 			33,		66,		1,		100);
		
		AddItem("SkateHelmet_Blue",					45,		90,		1,		100, null, {"SkateHelmet_Gray", "SkateHelmet_Red", "SkateHelmet_Black", "SkateHelmet_Green"});
		/*AddItem("SkateHelmet_Gray", 				45,		90,		1,		100);
		AddItem("SkateHelmet_Red", 					45,		90,		1,		100);*/
		AddItem("SkateHelmet_Black", 				47,		94,		1,		100);
		AddItem("SkateHelmet_Green", 				50,		100,	1,		100);

		AddItem("HockeyHelmet_Blue", 				50,		100,	1,		100, null, {"HockeyHelmet_Red", "HockeyHelmet_White", "HockeyHelmet_Black"});
		/*AddItem("HockeyHelmet_Red", 				50,		100,	1,		100);
		AddItem("HockeyHelmet_White", 				50,		100,	1,		100);*/
		AddItem("HockeyHelmet_Black", 				55,		110,	1,		100);

		AddItem("DirtBikeHelmet_Mouthguard", 		15,		30,		1,		100);
		AddItem("DirtBikeHelmet_Visor", 			25,		50,		1,		100);
		
		AddItem("DirtBikeHelmet_Chernarus", 		60,		120,	1,		100, {"DirtBikeHelmet_Mouthguard", "DirtBikeHelmet_Visor"}, {"DirtBikeHelmet_Police", "DirtBikeHelmet_Red", "DirtBikeHelmet_Green", "DirtBikeHelmet_Blue", "DirtBikeHelmet_Black", "DirtBikeHelmet_Khaki"});
		/*AddItem("DirtBikeHelmet_Police", 			60,		120,	1,		100, {"DirtBikeHelmet_Mouthguard", "DirtBikeHelmet_Visor"});
		AddItem("DirtBikeHelmet_Red", 				60,		120,	1,		100, {"DirtBikeHelmet_Mouthguard", "DirtBikeHelmet_Visor"});
		AddItem("DirtBikeHelmet_Green", 			60,		120,	1,		100, {"DirtBikeHelmet_Mouthguard", "DirtBikeHelmet_Visor"});
		AddItem("DirtBikeHelmet_Blue", 				60,		120,	1,		100, {"DirtBikeHelmet_Mouthguard", "DirtBikeHelmet_Visor"});*/
		AddItem("DirtBikeHelmet_Black", 			65,		130,	1,		100, {"DirtBikeHelmet_Mouthguard", "DirtBikeHelmet_Visor"});
		AddItem("DirtBikeHelmet_Khaki", 			70,		140,	1,		100, {"DirtBikeHelmet_Mouthguard", "DirtBikeHelmet_Visor"});

		AddItem("MotoHelmet_Lime", 					70,		140,	1,		100, null, {"MotoHelmet_Blue", "MotoHelmet_Red", "MotoHelmet_Yellow", "MotoHelmet_White", "MotoHelmet_Grey", "MotoHelmet_Black", "MotoHelmet_Green", "DarkMotoHelmet_Lime", "DarkMotoHelmet_Blue", "DarkMotoHelmet_Red", "DarkMotoHelmet_White", "DarkMotoHelmet_Grey", "DarkMotoHelmet_Black", "DarkMotoHelmet_Green"});
		/*AddItem("MotoHelmet_Blue", 				70,		140,	1,		100);
		AddItem("MotoHelmet_Red", 					70,		140,	1,		100);
		AddItem("MotoHelmet_White", 				70,		140,	1,		100);*/
		AddItem("MotoHelmet_Grey", 					75,		150,	1,		100);
		AddItem("MotoHelmet_Black",					75,		150,	1,		100);
		AddItem("MotoHelmet_Green", 				80,		160,	1,		100);

		AddItem("DarkMotoHelmet_Grey", 				80,		170,	1,		100);
		AddItem("DarkMotoHelmet_Lime", 			80,		170,	1,		100);
		AddItem("DarkMotoHelmet_Blue", 				80,		170,	1,		100);
		AddItem("DarkMotoHelmet_Red", 				80,		170,	1,		100);
		AddItem("DarkMotoHelmet_White", 			80,		170,	1,		100);
		AddItem("DarkMotoHelmet_Black", 			85,		170,	1,		100);
		AddItem("DarkMotoHelmet_Green", 			90,		180,	1,		100);

		AddItem("TankerHelmet", 					100,	200,	1,		100);
		
		AddItem("GreatHelm", 						120,	240,	1,		100);
	
		AddItem("ZSh3PilotHelmet", 					130,	260,	1,		100, null, {"ZSh3PilotHelmet_Green", "ZSh3PilotHelmet_Black"});
		AddItem("ZSh3PilotHelmet_Green", 			135,	270,	1,		100);
		AddItem("ZSh3PilotHelmet_Black", 			135,	270,	1,		100);
			
		AddItem("FirefightersHelmet_Red", 			150,	300,	1,		100, null, {"FirefightersHelmet_White", "FirefightersHelmet_Yellow"});
		/*AddItem("FirefightersHelmet_White", 		150,	300,	1,		100);
		AddItem("FirefightersHelmet_Yellow", 		150,	300,	1,		100);*/
				
	#ifdef EXPANSIONMOD
		AddItem("Mich2001Helmet", 					150,	300,	1,		100, null, {"ExpansionMich2001Desert"});
		//AddItem("ExpansionMich2001Desert", 		150,	300,	1,		100);
	#else
		AddItem("Mich2001Helmet", 					150,	300,	1,		100);
	#endif
		
		AddItem("GorkaHelmetVisor", 				50,		100,	1,		100);
		AddItem("GorkaHelmet", 						240,	480,	1,		100, {"GorkaHelmetVisor"}, {"GorkaHelmet_Black"});
		//AddItem("GorkaHelmet_Black", 				240,	480,	1,		100, {"GorkaHelmetVisor"});

		AddItem("Ssh68Helmet", 						250,	500,	1,		100);

		AddItem("BallisticHelmet_UN", 				250,	500,	1,		100, null, {"BallisticHelmet_Black", "BallisticHelmet_Green"});
		AddItem("BallisticHelmet_Black", 			300,	600,	1,		100);
		AddItem("BallisticHelmet_Green", 			350,	700,	1,		100);
	}
};