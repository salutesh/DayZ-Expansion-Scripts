/**
 * ExpansionMarketArmbands.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketArmbands: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 3;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_ARMBANDS";
		m_FileName = "Armbands";

		AddItem("Armband_White", 					15,		30,		1,		250, NULL, {"Armband_Yellow","Armband_Orange","Armband_Red","Armband_Green","Armband_Pink","Armband_Blue","Armband_Black"});
		AddItem("Armband_APA", 					    15,		30,		1,		250, NULL, {"Armband_Altis","Armband_BabyDeer","Armband_Bear","Armband_Bohemia","Armband_BrainZ","Armband_CDF","Armband_CHEL","Armband_CMC","Armband_Cannibals","Armband_Chedaki","Armband_Chernarus","Armband_DayZ","Armband_HunterZ","Armband_Livonia","Armband_LivoniaArmy","Armband_LivoniaPolice","Armband_NAPA","Armband_NSahrani","Armband_Pirates","Armband_RSTA","Armband_Refuge","Armband_Rooster","Armband_SSahrani","Armband_Snake","Armband_TEC","Armband_UEC","Armband_Wolf","Armband_Zenit"});
		#ifdef EXPANSIONMODBASEBUILDING
		AddItem("Armband_Expansion_Expansion", 		15,		30,		1,		250, NULL, {"Armband_Expansion_Australia","Armband_Expansion_Canada","Armband_Expansion_Chernarus","Armband_Expansion_France","Armband_Expansion_Germany","Armband_Expansion_Latvia","Armband_Expansion_Luxembourg","Armband_Expansion_Mexico","Armband_Expansion_Netherlands","Armband_Expansion_NewZealand","Armband_Expansion_Norway","Armband_Expansion_Poland","Armband_Expansion_Russia","Armband_Expansion_UK","Armband_Expansion_USA","Armband_Expansion_Scotland","Armband_Expansion_Sweden","Armband_Expansion_Spain","Armband_Expansion_Denmark","Armband_Expansion_Brazil","Armband_Expansion_Portugal","Armband_Expansion_Belgium","Armband_Expansion_Livonia","Armband_Expansion_Japan","Armband_Expansion_China","Armband_Expansion_SouthKorea","Armband_Expansion_UN","Armband_Expansion_NATO","Armband_Expansion_Pirate","Armband_Expansion_Chedaki","Armband_Expansion_NAPA","Armband_Expansion_CDF","Armband_Expansion_NuevoRico","Armband_Expansion_Borduria","Armband_Expansion_Biohazard","Armband_Expansion_AnyoneInCherno","Armband_Expansion_Ireland","Armband_Expansion_Italy","Armband_Expansion_Wales","Armband_Expansion_Switzerland","Armband_Expansion_SriLanka","Armband_Expansion_SouthAfrica","Armband_Expansion_Sicily","Armband_Expansion_OffWithHead","Armband_Expansion_Gibraltar","Armband_Expansion_Czechia","Armband_Expansion_Fari","Armband_Expansion_Finland","Armband_Expansion_Argentina","Armband_Expansion_Turkey","Armband_Expansion_Ukraine","Armband_Expansion_DayZWhite","Armband_Expansion_DayZBlack","Armband_Expansion_DoubleAxe","Armband_Expansion_Grenade"});
		AddItem("Armband_Expansion_White", 			15,		30,		1,		250, NULL, {"Armband_Expansion_Red","Armband_Expansion_Blue","Armband_Expansion_Green","Armband_Expansion_Yellow","Armband_Expansion_Orange","Armband_Expansion_Pink","Armband_Expansion_Purple","Armband_Expansion_Rainbow"});
		#endif
	}
};