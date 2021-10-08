/**
 * ExpansionMarketFlags.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketFlags: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 63;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FLAGS";
		m_FileName = "Flags";

		AddItem("Flag_Chernarus", 					15,		30,		1,		100,	0);
		AddItem("Flag_Chedaki", 					15,		30,		1,		100,	0);
		AddItem("Flag_NAPA", 						15,		30,		1,		100,	0);
		AddItem("Flag_CDF", 						15,		30,		1,		100,	0);
		AddItem("Flag_Livonia", 					15,		30,		1,		100,	0);
		AddItem("Flag_Altis", 						15,		30,		1,		100,	0);
		AddItem("Flag_SSahrani", 					15,		30,		1,		100,	0);
		AddItem("Flag_NSahrani", 					15,		30,		1,		100,	0);
		AddItem("Flag_DayZ", 						15,		30,		1,		100,	0);
		AddItem("Flag_LivoniaArmy", 				15,		30,		1,		100,	0);
		AddItem("Flag_White", 						15,		30,		1,		100,	0);
		AddItem("Flag_Bohemia", 					15,		30,		1,		100,	0);
		AddItem("Flag_APA", 						15,		30,		1,		100,	0);
		AddItem("Flag_UEC", 						15,		30,		1,		100,	0);
		AddItem("Flag_Pirates", 					15,		30,		1,		100,	0);
		AddItem("Flag_Cannibals", 					15,		30,		1,		100,	0);
		AddItem("Flag_Bear", 						15,		30,		1,		100,	0);
		AddItem("Flag_Wolf", 						15,		30,		1,		100,	0);
		AddItem("Flag_BabyDeer", 					15,		30,		1,		100,	0);
		AddItem("Flag_Rooster", 					15,		30,		1,		100,	0);
		AddItem("Flag_LivoniaPolice", 				15,		30,		1,		100,	0);
		AddItem("Flag_CMC", 						15,		30,		1,		100,	0);
		AddItem("Flag_TEC", 						15,		30,		1,		100,	0);
		AddItem("Flag_CHEL", 						15,		30,		1,		100,	0);
		AddItem("Flag_Zenit", 						15,		30,		1,		100,	0);
		AddItem("Flag_HunterZ", 					15,		30,		1,		100,	0);
		AddItem("Flag_BrainZ", 						15,		30,		1,		100,	0);
		AddItem("Flag_Refuge", 						15,		30,		1,		100,	0);
		AddItem("Flag_RSTA", 						15,		30,		1,		100,	0);
		AddItem("Flag_Snake", 						15,		30,		1,		100,	0);
	#ifdef EXPANSIONMOD
		AddItem("Expansion_Flag_Expansion", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_White", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Australia", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Canada", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Chernarus", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_France", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Germany", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Latvia", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Luxembourg", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Mexico", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Netherlands", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Newzealand", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Norway", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Poland", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Russia", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_UK", 				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_USA", 				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Scotland", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Finland", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Sweden", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Spain", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Brazil", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Portugal", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Belgium", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Livonia", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Japan", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_China", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_SouthKorea", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_UN", 				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_NATO", 				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Pirate", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Chedaki", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Napa", 				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Cdf", 				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_NuevoRico", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Borduria", 			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Biohazard", 		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_AnyoneInCherno",	15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Ireland",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Wales",				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Switzerland",		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Skilanka",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_SouthAfrica",		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Sicily",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_OffWithHead",		15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Gubraltar",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Czechia",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Fari",				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_DayZWhite",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_DayZBlack",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_DoubleAxe",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Grenade",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Red",				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Blue",				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Green",				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Yellow",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Orange",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Pink",				15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Purple",			15,		30,		1,		100,	0);
		AddItem("Expansion_Flag_Rainbow",			15,		30,		1,		100,	0);
	#endif
	}
};