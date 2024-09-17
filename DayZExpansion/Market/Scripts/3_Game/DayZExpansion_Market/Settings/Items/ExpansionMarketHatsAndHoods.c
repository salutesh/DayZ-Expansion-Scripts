/**
 * ExpansionMarketHatsAndHoods.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHatsAndHoods: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 28;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_HATS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_HOODS");
		m_FileName = "Hats_And_Hoods";
		
		AddItem("medicalscrubshat_blue", 	655, 	1090,	1,100,null,{"MedicalScrubsHat_White","MedicalScrubsHat_Green"});
		AddItem("medicalscrubshat_white", 	655, 	1090,	1,100);
		AddItem("medicalscrubshat_green", 	655, 	1090,	1,100);

	#ifdef EXPANSIONMOD
		AddItem("cowboyhat_brown", 	1475, 	2460,	1,100,null,{"CowboyHat_black","CowboyHat_darkBrown","CowboyHat_green","ExpansionCowboyHatGator","ExpansionCowboyHatSnake","ExpansionCowboyHatRattleSnake"});
	#else
		AddItem("cowboyhat_brown", 	1475, 	2460,	1,100,null,{"CowboyHat_black","CowboyHat_darkBrown","CowboyHat_green"});
		AddItem("cowboyhat_black", 	1440, 	2400,	1,100);
		AddItem("cowboyhat_darkbrown", 	1475, 	2460,	1,100);
		AddItem("cowboyhat_green", 	1475, 	2460,	1,100);
	#endif

		AddItem("militaryberet_chdkz", 	5585, 	9310,	1,100,null,{"MilitaryBeret_Red","MilitaryBeret_UN","MilitaryBeret_CDF","MilitaryBeret_NZ"});
		AddItem("militaryberet_red", 	5585, 	9310,	1,100);
		AddItem("militaryberet_un", 	1060, 	1770,	1,100);
		AddItem("militaryberet_cdf", 	5585, 	9310,	1,100);
		AddItem("militaryberet_nz", 	1060, 	1770,	1,100);

		AddItem("beaniehat_pink", 	930, 	1550,	1,100,null,{"BeanieHat_Beige","BeanieHat_Blue","BeanieHat_Brown","BeanieHat_Grey","BeanieHat_Red","BeanieHat_Black","BeanieHat_Green"});
		AddItem("beaniehat_blue", 	175, 	295,	1,100); // easier to find
		
		AddItem("ushanka_black", 	905, 	1510,	1,100,null,{"Ushanka_Blue","Ushanka_Green"});
		AddItem("ushanka_blue", 	905, 	1510,	1,100);
		AddItem("ushanka_green", 	170, 	285,	1,100);
		
		AddItem("booniehat_blue", 	820, 	1370,	1,100,null,{"BoonieHat_NavyBlue","BoonieHat_Orange","BoonieHat_Red","BoonieHat_Black","BoonieHat_Tan","BoonieHat_Olive","BoonieHat_DPM","BoonieHat_Dubok","BoonieHat_Flecktran","booniehat_winter"});
		AddItem("booniehat_navyblue", 	560, 	935,	1,100);
		AddItem("booniehat_dpm", 	805, 	1340,	1,100);
		AddItem("booniehat_dubok", 	805, 	1340,	1,100);
		AddItem("booniehat_flecktran", 	805, 	1340,	1,100);
		#ifndef DAYZ_1_25
		AddItem("booniehat_winter", 	665, 	1110,	1,100);
		#endif
		
		AddItem("budenovkahat_gray", 	145, 	240,	1,100);

		AddItem("officerhat", 	1035, 	1725,	1,100);
		
		AddItem("nbchoodgray", 	7900, 	13165,	1,100,null,{"nbchoodyellow","nbchoodwhite"});

		#ifndef DAYZ_1_25
		AddItem("morozkohat", 	360, 	600,	1,100);
		AddItem("okzkcap_beige", 	1060, 	1770,	1,100,null,{"okzkcap_green"});
		AddItem("petushokhat_black", 	285, 	470,	1,100,null,{"petushokhat_green","petushokhat_yellow"});
		AddItem("snowstormushanka_brown", 	145, 	240,	1,100,null,{"snowstormushanka_navy","snowstormushanka_olive","snowstormushanka_white"});
		AddItem("wintercoif_black", 	175, 	290,	1,100,null,{"wintercoif_blue","wintercoif_green","wintercoif_skull"});
		#endif
	}
};