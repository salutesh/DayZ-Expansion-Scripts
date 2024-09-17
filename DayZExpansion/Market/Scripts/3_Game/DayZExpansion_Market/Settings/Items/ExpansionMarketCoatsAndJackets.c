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

		AddItem("labcoat", 	640, 	1065,	1,100);

		AddItem("tracksuitjacket_black", 	990, 	1645,	1,100,null,{"TrackSuitJacket_Blue","TrackSuitJacket_Green","TrackSuitJacket_LightBlue","TrackSuitJacket_Red"});
		AddItem("tracksuitjacket_blue", 	635, 	1060,	1,100);
		AddItem("tracksuitjacket_green", 	530, 	885,	1,100);
		AddItem("tracksuitjacket_lightblue", 	530, 	885,	1,100);
		AddItem("tracksuitjacket_red", 	635, 	1060,	1,100);
		
		#ifndef DAYZ_1_25
		AddItem("downjacket_blue", 	145, 	240,	1,100,null,{"downjacket_green","downjacket_orange","downjacket_red"});
		#endif

		AddItem("denimjacket", 	210, 	350,	1,100);

		AddItem("woolcoat_red", 	810, 	1350,	1,100,null,{"WoolCoat_Blue","WoolCoat_Beige","WoolCoat_RedCheck","WoolCoat_BlueCheck","WoolCoat_GreyCheck","WoolCoat_BrownCheck","WoolCoat_Black","WoolCoat_BlackCheck","WoolCoat_Green"});
		AddItem("woolcoat_blue", 	1005, 	1675,	1,100);
		AddItem("woolcoat_beige", 	1145, 	1910,	1,100);
		AddItem("woolcoat_redcheck", 	810, 	1350,	1,100);
		AddItem("woolcoat_bluecheck", 	1005, 	1675,	1,100);
		AddItem("woolcoat_greycheck", 	650, 	1080,	1,100);
		AddItem("woolcoat_browncheck", 	1445, 	2410,	1,100);
		AddItem("woolcoat_black", 	1460, 	2430,	1,100);
		AddItem("woolcoat_blackcheck", 	1445, 	2410,	1,100);
		AddItem("woolcoat_green", 	1015, 	1690,	1,100);
	
	#ifdef EXPANSIONMOD
		AddItem("ridersjacket_black", 	835, 	1390,	1,100,null,{"ExpansionRidersJacketBrown","ExpansionRidersJacketDarkBlue"});
	#else
		AddItem("ridersjacket_black", 	835, 	1390,	1,100);
	#endif
	
		AddItem("firefighterjacket_beige", 	355, 	590,	1,100,null,{"FirefighterJacket_Black"});
		AddItem("firefighterjacket_black", 	355, 	590,	1,100);
		
		AddItem("jumpsuitjacket_blue", 	1815, 	3030,	1,100,null,{"JumpsuitJacket_Gray","JumpsuitJacket_Green","JumpsuitJacket_Red"});
		AddItem("jumpsuitjacket_gray", 	1170, 	1955,	1,100);
		AddItem("jumpsuitjacket_red", 	1170, 	1955,	1,100);

		AddItem("bomberjacket_brown", 	1005, 	1675,	1,100,null,{"BomberJacket_Blue","BomberJacket_Grey","BomberJacket_Maroon","BomberJacket_SkyBlue","BomberJacket_Black","BomberJacket_Olive"});
		AddItem("bomberjacket_olive", 	7045, 	11745,	1,100);

		AddItem("quiltedjacket_blue", 	1125, 	1875,	1,100,null,{"QuiltedJacket_Red","QuiltedJacket_Grey","QuiltedJacket_Orange","QuiltedJacket_Yellow","QuiltedJacket_Violet","QuiltedJacket_Black","QuiltedJacket_Green"});
		AddItem("quiltedjacket_red", 	990, 	1645,	1,100);
		AddItem("quiltedjacket_orange", 	990, 	1645,	1,100);
		AddItem("quiltedjacket_yellow", 	990, 	1645,	1,100);
		AddItem("quiltedjacket_violet", 	990, 	1645,	1,100);
		AddItem("quiltedjacket_black", 	1420, 	2370,	1,100);
		AddItem("quiltedjacket_green", 	1420, 	2370,	1,100);

		AddItem("prisonuniformjacket", 	355, 	590,	1,100);

		AddItem("policejacketorel", 	660, 	1100,	1,100);
		AddItem("policejacket", 	660, 	1100,	1,100);
		#ifndef DAYZ_1_25
		AddItem("bushlatpolicejacket_blue", 	4740, 	7895,	1,100);
		#endif

		AddItem("paramedicjacket_blue", 	645, 	1075,	1,100,null,{"ParamedicJacket_Crimson","ParamedicJacket_Green"});

		AddItem("hikingjacket_black", 	1125, 	1875,	1,100,null,{"HikingJacket_Red","HikingJacket_Blue","HikingJacket_Green"});
		AddItem("hikingjacket_green", 	1420, 	2370,	1,100);
	
		AddItem("raincoat_pink", 	245, 	405,	1,100,null,{"Raincoat_Orange","Raincoat_Yellow","Raincoat_Red","Raincoat_Blue","Raincoat_Black","Raincoat_Green"});

		AddItem("ttskojacket_camo", 	1035, 	1725,	1,100);

		AddItem("bdujacket", 	1055, 	1755,	1,100);

		AddItem("huntingjacket_autumn", 	4310, 	7180,	1,100,null,{"HuntingJacket_Brown","HuntingJacket_Spring","HuntingJacket_Summer","HuntingJacket_Winter"});
		AddItem("huntingjacket_brown", 	4235, 	7055,	1,100);
		AddItem("huntingjacket_spring", 	4455, 	7430,	1,100);
		AddItem("huntingjacket_summer", 	3355, 	5595,	1,100);
		AddItem("huntingjacket_winter", 	4385, 	7305,	1,100);

		AddItem("m65jacket_black", 	7105, 	11845,	1,100,null,{"M65Jacket_Khaki","M65Jacket_Tan","M65Jacket_Olive"});
		AddItem("m65jacket_olive", 	7830, 	13055,	1,100);
		
		AddItem("gorkaejacket_summer", 	1055, 	1755,	1,100,null,{"GorkaEJacket_Flat","GorkaEJacket_Autumn","GorkaEJacket_PautRev","GorkaEJacket_Winter"});
		
		AddItem("usmcjacket_desert", 	1055, 	1755,	1,100,null,{"USMCJacket_Woodland"});
		AddItem("usmcjacket_woodland", 	1055, 	1755,	1,100);
		
		AddItem("nbcjacketgray", 	7900, 	13165,	1,100,null,{"nbcjacketyellow","nbcjacketwhite"});
		
		AddItem("chainmail", 	355, 	595,	1,100);
		
		#ifndef DAYZ_1_25
		AddItem("omkjacket_navy", 	145, 	240,	1,100);
		AddItem("navyuniformjacket", 	145, 	240,	1,100);
		#endif
	}
};