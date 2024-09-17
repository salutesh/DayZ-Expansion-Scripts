/**
 * ExpansionMarketFlags.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("flag_chernarus", 	375, 	625,	1,100);
		AddItem("flag_chedaki", 	375, 	625,	1,100);
		AddItem("flag_napa", 	375, 	625,	1,100);
		AddItem("flag_cdf", 	375, 	625,	1,100);
		AddItem("flag_livonia", 	200, 	335,	1,100);
		AddItem("flag_altis", 	200, 	335,	1,100);
		AddItem("flag_ssahrani", 	200, 	335,	1,100);
		AddItem("flag_nsahrani", 	200, 	335,	1,100);
		AddItem("flag_dayz", 	375, 	625,	1,100);
		AddItem("flag_livoniaarmy", 	200, 	335,	1,100);
		AddItem("flag_white", 	375, 	625,	1,100);
		AddItem("flag_bohemia", 	375, 	625,	1,100);
		AddItem("flag_apa", 	200, 	335,	1,100);
		AddItem("flag_uec", 	375, 	625,	1,100);
		AddItem("flag_pirates", 	375, 	625,	1,100);
		AddItem("flag_cannibals", 	375, 	625,	1,100);
		AddItem("flag_bear", 	200, 	335,	1,100);
		AddItem("flag_wolf", 	375, 	625,	1,100);
		AddItem("flag_babydeer", 	375, 	625,	1,100);
		AddItem("flag_rooster", 	375, 	625,	1,100);
		AddItem("flag_livoniapolice", 	200, 	335,	1,100);
		AddItem("flag_cmc", 	375, 	625,	1,100);
		AddItem("flag_tec", 	375, 	625,	1,100);
		AddItem("flag_chel", 	375, 	625,	1,100);
		AddItem("flag_zenit", 	375, 	625,	1,100);
		AddItem("flag_hunterz", 	375, 	625,	1,100);
		AddItem("flag_brainz", 	200, 	335,	1,100);
		AddItem("flag_refuge", 	375, 	625,	1,100);
		AddItem("flag_rsta", 	375, 	625,	1,100);
		AddItem("flag_snake", 	375, 	625,	1,100);
		//! New 1.21 flags
		AddItem("flag_crook", 	200, 	335,	1,100);
		AddItem("flag_rex", 	200, 	335,	1,100);
		AddItem("flag_zagorky", 	200, 	335,	1,100);
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("Expansion_Flag_Expansion", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_White", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Australia", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_Canada", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Chernarus", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_France", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Germany", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Latvia", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Luxembourg", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_Mexico", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Netherlands", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_NewZealand", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_Norway", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Poland", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Russia", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_UK", 				15,		30,		1,		100);
		AddItem("Expansion_Flag_USA", 				15,		30,		1,		100);
		AddItem("Expansion_Flag_Scotland", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Finland", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Sweden", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Spain", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Denmark", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Brazil", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Portugal", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Belgium", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Livonia", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Japan", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_China", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_SouthKorea", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_UN", 				15,		30,		1,		100);
		AddItem("Expansion_Flag_NATO", 				15,		30,		1,		100);
		AddItem("Expansion_Flag_Pirate", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Chedaki", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_NAPA", 				15,		30,		1,		100);
		AddItem("Expansion_Flag_CDF", 				15,		30,		1,		100);
		AddItem("Expansion_Flag_NuevoRico", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_Borduria", 			15,		30,		1,		100);
		AddItem("Expansion_Flag_Biohazard", 		15,		30,		1,		100);
		AddItem("Expansion_Flag_AnyoneInCherno",	15,		30,		1,		100);
		AddItem("Expansion_Flag_Ireland",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Wales",				15,		30,		1,		100);
		AddItem("Expansion_Flag_Switzerland",		15,		30,		1,		100);
		AddItem("Expansion_Flag_SriLanka",			15,		30,		1,		100);
		AddItem("Expansion_Flag_SouthAfrica",		15,		30,		1,		100);
		AddItem("Expansion_Flag_Sicily",			15,		30,		1,		100);
		AddItem("Expansion_Flag_OffWithHead",		15,		30,		1,		100);
		AddItem("Expansion_Flag_Gibraltar",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Czechia",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Fari",				15,		30,		1,		100);
		AddItem("Expansion_Flag_DayZWhite",			15,		30,		1,		100);
		AddItem("Expansion_Flag_DayZBlack",			15,		30,		1,		100);
		AddItem("Expansion_Flag_DoubleAxe",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Grenade",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Red",				15,		30,		1,		100);
		AddItem("Expansion_Flag_Blue",				15,		30,		1,		100);
		AddItem("Expansion_Flag_Green",				15,		30,		1,		100);
		AddItem("Expansion_Flag_Yellow",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Orange",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Pink",				15,		30,		1,		100);
		AddItem("Expansion_Flag_Purple",			15,		30,		1,		100);
		AddItem("Expansion_Flag_Rainbow",			15,		30,		1,		100);
	#endif
	}
};