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

		AddItem("medicalscrubspants_blue", 	655, 	1090,	1,100,null,{"MedicalScrubsPants_Green","MedicalScrubsPants_White"});
		
		AddItem("tracksuitpants_black", 	990, 	1645,	1,100,null,{"TrackSuitPants_Blue","TrackSuitPants_Green","TrackSuitPants_Red","TrackSuitPants_LightBlue"});
		
		AddItem("prisonuniformpants", 	355, 	590,	1,100);

		AddItem("breeches_pink", 	515, 	860,	1,100,null,{"Breeches_Red","Breeches_White","Breeches_Blue","Breeches_Beetcheck","Breeches_Beige","Breeches_Browncheck","Breeches_Black","Breeches_Blackcheck","Breeches_Green"});
		AddItem("breeches_beige", 	620, 	1035,	1,100);
		AddItem("breeches_browncheck", 	960, 	1605,	1,100);
		AddItem("breeches_black", 	620, 	1035,	1,100);
		AddItem("breeches_blackcheck", 	960, 	1605,	1,100);
		AddItem("breeches_green", 	620, 	1035,	1,100);

		AddItem("slackspants_beige", 	280, 	465,	1,100,null,{"SlacksPants_Blue","SlacksPants_DarkGrey","SlacksPants_LightGrey","SlacksPants_White","SlacksPants_Brown","SlacksPants_Black","SlacksPants_Khaki"});
		AddItem("slackspants_brown", 	280, 	465,	1,100);
		AddItem("slackspants_black", 	280, 	465,	1,100);
		AddItem("slackspants_khaki", 	280, 	465,	1,100);

		AddItem("canvaspantsmidi_blue", 	175, 	295,	1,100,null,{"CanvasPantsMidi_Grey","CanvasPantsMidi_Red","CanvasPantsMidi_Violet","CanvasPantsMidi_Beige"});
		AddItem("canvaspantsmidi_beige", 	175, 	295,	1,100);

		AddItem("canvaspants_blue", 	170, 	290,	1,100,null,{"CanvasPants_Grey","CanvasPants_Red","CanvasPants_Violet","CanvasPants_Beige"});
		AddItem("canvaspants_beige", 	170, 	290,	1,100);

		AddItem("jumpsuitpants_blue", 	1645, 	2740,	1,100,null,{"JumpsuitPants_Green","JumpsuitPants_Grey","JumpsuitPants_Red"});
		
		AddItem("policepants", 	660, 	1100,	1,100);

		AddItem("paramedicpants_blue", 	645, 	1075,	1,100,null,{"ParamedicPants_Crimson","ParamedicPants_Green"});
		
		AddItem("firefighterspants_beige", 	355, 	590,	1,100,null,{"FirefightersPants_Black"});
		
		AddItem("cargopants_beige", 	1875, 	3125,	1,100,null,{"CargoPants_Black","CargoPants_Blue","CargoPants_Green","CargoPants_Grey"});
		
		AddItem("shortjeans_blue", 	835, 	1390,	1,100,null,{"ShortJeans_Brown","ShortJeans_Darkblue","ShortJeans_Red","ShortJeans_Black","ShortJeans_Green"});
		AddItem("shortjeans_black", 	835, 	1390,	1,100);
		AddItem("shortjeans_green", 	835, 	1390,	1,100);

		AddItem("jeans_blue", 	790, 	1320,	1,100,null,{"Jeans_Brown","Jeans_Grey","Jeans_BlueDark","Jeans_Green","Jeans_Black"});
		AddItem("jeans_green", 	790, 	1320,	1,100);
		AddItem("jeans_black", 	790, 	1320,	1,100);
		
		AddItem("ttskopants", 	1035, 	1725,	1,100);

		AddItem("bdupants", 	1055, 	1755,	1,100);

		AddItem("usmcpants_desert", 	1055, 	1755,	1,100,null,{"USMCPants_Woodland"});
		AddItem("usmcpants_woodland", 	1055, 	1755,	1,100);

		AddItem("policepantsorel", 	660, 	1100,	1,100);

		AddItem("hunterpants_winter", 	4385, 	7305,	1,100,null,{"HunterPants_Autumn","HunterPants_Brown","HunterPants_Spring","HunterPants_Summer"});
		AddItem("hunterpants_autumn", 	4270, 	7120,	1,100);
		AddItem("hunterpants_brown", 	4270, 	7120,	1,100);
		AddItem("hunterpants_spring", 	4455, 	7430,	1,100);
		AddItem("hunterpants_summer", 	3385, 	5645,	1,100);
	
		AddItem("gorkapants_summer", 	1055, 	1755,	1,100,null,{"GorkaPants_Autumn","GorkaPants_Flat","GorkaPants_PautRev","GorkaPants_Winter"});
		
		AddItem("nbcpantsgray", 	7900, 	13165,	1,100,null,{"NBCPantsYellow","NBCPantsWhite"});
		//! 1.21+
		AddItem("chainmail_leggings", 	355, 	595,	1,100);
		
		#ifndef DAYZ_1_25
		AddItem("omkpants_navy", 	145, 	240,	1,100);
		AddItem("navyuniformpants", 	145, 	240,	1,100);
		#endif
	}
};