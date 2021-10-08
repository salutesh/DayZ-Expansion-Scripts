/**
 * ExpansionMarketHatsAndHoods.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		
		AddItem("MedicalScrubsHat_Blue", 			2,		4,		1,		100,	0, null, {"MedicalScrubsHat_White", "MedicalScrubsHat_Green"});
		/*AddItem("MedicalScrubsHat_White", 		2,		4,		1,		100,	0);
		AddItem("MedicalScrubsHat_Green", 			2,		4,		1,		100,	0);*/

	#ifdef EXPANSIONMOD
		AddItem("CowboyHat_Brown", 					20,		40,		1,		100,	0, null, {"CowboyHat_black", "CowboyHat_darkBrown", "CowboyHat_green", "ExpansionCowboyHatGator", "ExpansionCowboyHatSnake", "ExpansionCowboyHatRattleSnake"});
		/*AddItem("ExpansionCowboyHatGator", 		20,		40,		1,		100,	0);
		AddItem("ExpansionCowboyHatSnake", 			20,		40,		1,		100,	0);
		AddItem("ExpansionCowboyHatRattleSnake", 	20,		40,		1,		100,	0);*/
	#else
		AddItem("CowboyHat_Brown", 					20,		40,		1,		100,	0, null, {"CowboyHat_black", "CowboyHat_darkBrown", "CowboyHat_green"});
		/*AddItem("CowboyHat_black", 				20,		40,		1,		100,	0);
		AddItem("CowboyHat_darkBrown", 				20,		40,		1,		100,	0);
		AddItem("CowboyHat_green", 					20,		40,		1,		100,	0);*/
	#endif

		AddItem("MilitaryBeret_ChDKZ", 				26,		52,		1,		100,	0, null, {"MilitaryBeret_Red", "MilitaryBeret_UN", "MilitaryBeret_CDF", "MilitaryBeret_NZ"});
		/*AddItem("MilitaryBeret_Red", 				26,		52,		1,		100,	0);
		AddItem("MilitaryBeret_UN", 				26,		52,		1,		100,	0);*/
		AddItem("MilitaryBeret_CDF", 				27,		54,		1,		100,	0);
		AddItem("MilitaryBeret_NZ", 				27,		54,		1,		100,	0);

		AddItem("BeanieHat_Pink", 					30,		60,		1,		100,	0, null, {"BeanieHat_Beige", "BeanieHat_Blue", "BeanieHat_Brown", "BeanieHat_Grey", "BeanieHat_Red", "BeanieHat_Black", "BeanieHat_Green"});		
		/*AddItem("BeanieHat_Beige", 				30,		60,		1,		100,	0);
		AddItem("BeanieHat_Blue", 					30,		60,		1,		100,	0);
		AddItem("BeanieHat_Brown", 					30,		60,		1,		100,	0);
		AddItem("BeanieHat_Grey", 					30,		60,		1,		100,	0);
		AddItem("BeanieHat_Red", 					30,		60,		1,		100,	0);*/
		AddItem("BeanieHat_Black", 					31,		62,		1,		100,	0);
		AddItem("BeanieHat_Green", 					32,		64,		1,		100,	0);
		
		AddItem("Ushanka_Black", 					50,		100,	1,		100,	0, null, {"Ushanka_Blue", "Ushanka_Green"});
		//AddItem("Ushanka_Blue", 					50,		100,	1,		100,	0);
		AddItem("Ushanka_Green", 					55,		110,	1,		100,	0);
		
		AddItem("BoonieHat_Blue", 					16,		32,		1,		100,	0, null, {"BoonieHat_NavyBlue", "BoonieHat_Orange", "BoonieHat_Red", "BoonieHat_Black", "BoonieHat_Tan", "BoonieHat_Olive", "BoonieHat_DPM", "BoonieHat_Dubok", "BoonieHat_Flecktran"});
		/*AddItem("BoonieHat_NavyBlue", 			16,		32,		1,		100,	0);
		AddItem("BoonieHat_Orange", 				16,		32,		1,		100,	0);
		AddItem("BoonieHat_Red", 					16,		32,		1,		100,	0);*/
		AddItem("BoonieHat_Black", 					17,		34,		1,		100,	0);
		AddItem("BoonieHat_Tan", 					18,		36,		1,		100,	0);
		AddItem("BoonieHat_Olive", 					18,		36,		1,		100,	0);
		AddItem("BoonieHat_DPM", 					20,		40,		1,		100,	0);
		AddItem("BoonieHat_Dubok", 					20,		40,		1,		100,	0);
		AddItem("BoonieHat_Flecktran", 				20,		40,		1,		100,	0);

		AddItem("OfficerHat", 						25,		50,		1,		100,	0);
		
		AddItem("NBCHoodGray", 						100,	200,	1,		100,	0);
		AddItem("NBCHoodYellow", 					100,	200,	1,		100,	0);
	}
};