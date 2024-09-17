/**
 * ExpansionMarketBackpacks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBackpacks: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 5;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BACKPACKS";
		m_FileName = "Backpacks";
		
		AddItem("childbag_red", 	765, 	1280,	1,100,null,{"ChildBag_Blue","ChildBag_Green"});
		AddItem("childbag_blue", 	765, 	1280,	1,100);
		AddItem("childbag_green", 	765, 	1280,	1,100);
		
		AddItem("drybag_orange", 	2030, 	3385,	1,100,null,{"DryBag_Yellow","DryBag_Blue","DryBag_Black","DryBag_Red","DryBag_Green"});
		AddItem("drybag_black", 	2050, 	3415,	1,100);
		AddItem("drybag_green", 	2065, 	3445,	1,100);
		
		AddItem("taloonbag_blue", 	1260, 	2100,	1,100,null,{"TaloonBag_Orange","TaloonBag_Violet","TaloonBag_Green"});
		AddItem("taloonbag_orange", 	890, 	1485,	1,100);
		AddItem("taloonbag_green", 	890, 	1485,	1,100);
	
		AddItem("smershbag", 	3415, 	5695,	1,100);
		
		#ifndef DAYZ_1_25
		AddItem("assaultbag_black", 	6195, 	10330,	1,100,null,{"AssaultBag_Green","AssaultBag_Ttsko","AssaultBag_Winter"});
		AddItem("assaultbag_winter", 	1190, 	1980,	1,100);
		#else
		AddItem("assaultbag_black", 	6195, 	10330,	1,100,null,{"AssaultBag_Green","AssaultBag_Ttsko"});
		#endif
		
		AddItem("huntingbag", 	3660, 	6100,	1,100,null,{"huntingbag_hannah"});
		
		AddItem("waterproofbag_green", 	365, 	605,	1,100,null,{"waterproofbag_orange","waterproofbag_yellow"});
	
#ifdef EXPANSIONMOD
	#ifndef DAYZ_1_25
		AddItem("tortillabag", 	1170, 	1945,	1,100,null,{"ExpansionDesertTortilla","TortillaBag_Winter"});
	#else
		AddItem("tortillabag", 	1170, 	1945,	1,100,null,{"ExpansionDesertTortilla"});
	#endif
#else
	#ifndef DAYZ_1_25
		AddItem("tortillabag", 	1170, 	1945,	1,100,null,{"TortillaBag_Winter"});
	#else
		AddItem("tortillabag", 	1170, 	1945,	1,100);
	#endif
#endif
	
	#ifdef EXPANSIONMOD
		#ifndef DAYZ_1_25
		AddItem("coyotebag_brown", 	7820, 	13030,	1,100,null,{"CoyoteBag_Green","coyotebag_winter","ExpansionCoyoteBlack"});
		AddItem("coyotebag_winter", 	1190, 	1980,	1,100);
		#else
		AddItem("coyotebag_brown", 	7820, 	13030,	1,100,null,{"CoyoteBag_Green","ExpansionCoyoteBlack"});
		#endif
	#else
		#ifndef DAYZ_1_25
		AddItem("coyotebag_brown", 	7820, 	13030,	1,100,null,{"CoyoteBag_Green","coyotebag_winter"});
		AddItem("coyotebag_winter", 	1190, 	1980,	1,100);
		#else
		AddItem("coyotebag_brown", 	7820, 	13030,	1,100,null,{"CoyoteBag_Green"});
		#endif
	#endif	

		AddItem("mountainbag_red", 	1025, 	1705,	1,100,null,{"MountainBag_Blue","MountainBag_Orange","MountainBag_Green"});
		
		AddItem("alicebag_green", 	3505, 	5840,	1,100,null,{"AliceBag_Black","AliceBag_Camo"});
		AddItem("alicebag_black", 	3505, 	5840,	1,100);
		AddItem("alicebag_camo", 	3505, 	5840,	1,100);
		
		AddItem("slingbag_black", 	925, 	1545,	1,100,null,{"Slingbag_Brown","Slingbag_Gray"});
		AddItem("duffelbagsmall_camo", 	5390, 	8980,	1,100,null,{"DuffelBagSmall_Green","DuffelBagSmall_Medical"});
		AddItem("canvasbag_medical", 	3325, 	5540,	1,100,null,{"CanvasBag_Olive"});
		AddItem("armypouch_beige", 	2580, 	4305,	1,100,null,{"ArmyPouch_Black","ArmyPouch_Camo","ArmyPouch_Green"});

		// Crafted item !
		//AddItem("DrysackBag_Green", 				120,		300,		1,		100, null, {"DrysackBag_Orange", "DrysackBag_Yellow"});
	}
};