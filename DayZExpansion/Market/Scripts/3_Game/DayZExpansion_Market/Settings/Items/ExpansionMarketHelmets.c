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

		AddItem("constructionhelmet_blue", 	480, 	800,	1,100,null,{"ConstructionHelmet_Orange","ConstructionHelmet_Red","ConstructionHelmet_White","ConstructionHelmet_Yellow","ConstructionHelmet_Lime"});
		AddItem("constructionhelmet_orange", 	480, 	800,	1,100);
		AddItem("constructionhelmet_red", 	480, 	800,	1,100);
		AddItem("constructionhelmet_white", 	480, 	800,	1,100);
		AddItem("constructionhelmet_yellow", 	480, 	800,	1,100);
		AddItem("constructionhelmet_lime", 	480, 	800,	1,100);
		
		AddItem("skatehelmet_blue", 	205, 	340,	1,100,null,{"SkateHelmet_Gray","SkateHelmet_Red","SkateHelmet_Black","SkateHelmet_Green"});
		AddItem("skatehelmet_gray", 	205, 	340,	1,100);
		AddItem("skatehelmet_red", 	205, 	340,	1,100);
		AddItem("skatehelmet_black", 	205, 	340,	1,100);
		AddItem("skatehelmet_green", 	205, 	340,	1,100);

		AddItem("hockeyhelmet_blue", 	205, 	340,	1,100,null,{"HockeyHelmet_Red","HockeyHelmet_White","HockeyHelmet_Black"});
		AddItem("hockeyhelmet_red", 	205, 	340,	1,100);
		AddItem("hockeyhelmet_white", 	205, 	340,	1,100);
		AddItem("hockeyhelmet_black", 	205, 	340,	1,100);

		AddItem("dirtbikehelmet_mouthguard", 	210, 	355,	1,100);
		AddItem("dirtbikehelmet_visor", 	210, 	355,	1,100);
		
		AddItem("dirtbikehelmet_chernarus", 	215, 	360,	1,100,{"DirtBikeHelmet_Mouthguard","DirtBikeHelmet_Visor"},{"DirtBikeHelmet_Police","DirtBikeHelmet_Red","DirtBikeHelmet_Green","DirtBikeHelmet_Blue","DirtBikeHelmet_Black","DirtBikeHelmet_Khaki"});
		AddItem("dirtbikehelmet_police", 	215, 	360,	1,100,{"DirtBikeHelmet_Mouthguard","DirtBikeHelmet_Visor"});
		AddItem("dirtbikehelmet_red", 	215, 	360,	1,100,{"DirtBikeHelmet_Mouthguard","DirtBikeHelmet_Visor"});
		AddItem("dirtbikehelmet_green", 	215, 	360,	1,100,{"DirtBikeHelmet_Mouthguard","DirtBikeHelmet_Visor"});
		AddItem("dirtbikehelmet_blue", 	215, 	360,	1,100,{"DirtBikeHelmet_Mouthguard","DirtBikeHelmet_Visor"});
		AddItem("dirtbikehelmet_black", 	215, 	360,	1,100,{"DirtBikeHelmet_Mouthguard","DirtBikeHelmet_Visor"});
		AddItem("dirtbikehelmet_khaki", 	215, 	360,	1,100,{"DirtBikeHelmet_Mouthguard","DirtBikeHelmet_Visor"});

		AddItem("motohelmet_lime", 	655, 	1095,	1,100,null,{"MotoHelmet_Blue","MotoHelmet_Red","MotoHelmet_Yellow","MotoHelmet_White","MotoHelmet_Grey","MotoHelmet_Black","MotoHelmet_Green","DarkMotoHelmet_Lime","DarkMotoHelmet_Blue","DarkMotoHelmet_Red","DarkMotoHelmet_White","DarkMotoHelmet_Grey","DarkMotoHelmet_Black","DarkMotoHelmet_Green"});
		AddItem("motohelmet_blue", 	790, 	1315,	1,100);
		AddItem("motohelmet_red", 	655, 	1095,	1,100);
		AddItem("motohelmet_white", 	655, 	1095,	1,100);
		AddItem("motohelmet_grey", 	1395, 	2325,	1,100);
		AddItem("motohelmet_black", 	1395, 	2325,	1,100);
		AddItem("motohelmet_green", 	790, 	1315,	1,100);

		AddItem("darkmotohelmet_grey", 	1420, 	2365,	1,100);
		AddItem("darkmotohelmet_lime", 	670, 	1115,	1,100);
		AddItem("darkmotohelmet_blue", 	800, 	1335,	1,100);
		AddItem("darkmotohelmet_red", 	670, 	1115,	1,100);
		AddItem("darkmotohelmet_white", 	670, 	1115,	1,100);
		AddItem("darkmotohelmet_black", 	1420, 	2365,	1,100);
		AddItem("darkmotohelmet_green", 	800, 	1335,	1,100);
		AddItem("darkmotohelmet_yellow", 	670, 	1115,	1,100);
		AddItem("darkmotohelmet_yellowscarred", 	145, 	240,	1,100);

		AddItem("tankerhelmet", 	7045, 	11745,	1,100);
		
		AddItem("greathelm", 	355, 	590,	1,100);
	
		AddItem("zsh3pilothelmet", 	6925, 	11545,	1,100,null,{"ZSh3PilotHelmet_Green","ZSh3PilotHelmet_Black"});
		AddItem("ZSh3PilotHelmet_Green", 			135,	270,	1,		100);
		AddItem("ZSh3PilotHelmet_Black", 			135,	270,	1,		100);
			
		AddItem("firefightershelmet_red", 	355, 	590,	1,100,null,{"FirefightersHelmet_White","FirefightersHelmet_Yellow"});
				
	#ifdef EXPANSIONMOD
		AddItem("mich2001helmet", 	7830, 	13055,	1,100,null,{"ExpansionMich2001Desert"});
		//AddItem("ExpansionMich2001Desert", 		150,	300,	1,		100);
	#else
		AddItem("mich2001helmet", 	7830, 	13055,	1,100);
	#endif
		
		AddItem("gorkahelmetvisor", 	5490, 	9150,	1,100);
		AddItem("gorkahelmet", 	5730, 	9550,	1,100,{"GorkaHelmetVisor"},{"GorkaHelmet_Black"});
		//AddItem("GorkaHelmet_Black", 				240,	480,	1,		100, {"GorkaHelmetVisor"});

		AddItem("ssh68helmet", 	990, 	1650,	1,100);

		#ifndef DAYZ_1_25
		AddItem("ballistichelmet_un", 	1060, 	1770,	1,100,null,{"BallisticHelmet_Black","BallisticHelmet_Green","BallisticHelmet_BDU","BallisticHelmet_Desert","BallisticHelmet_Woodland","BallisticHelmet_Navy","BallisticHelmet_Winter"});
		#else
		AddItem("ballistichelmet_un", 	1060, 	1770,	1,100,null,{"BallisticHelmet_Black","BallisticHelmet_Green","BallisticHelmet_BDU","BallisticHelmet_Desert","BallisticHelmet_Woodland"});
		#endif
		AddItem("ballistichelmet_bdu", 	1060, 	1770,	1,100);
		AddItem("ballistichelmet_woodland", 	1060, 	1770,	1,100);
		#ifndef DAYZ_1_25
		AddItem("ballistichelmet_navy", 	145, 	240,	1,100);
		AddItem("ballistichelmet_winter", 	1080, 	1800,	1,100);
		#endif

		//! 1.21+
		AddItem("chainmail_coif", 	355, 	595,	1,100);
		AddItem("norsehelm", 	145, 	240,	1,100);
	}
};