/**
 * ExpansionMarketVests.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketVests: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 60;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_VESTS";
		m_FileName = "Vests";

		AddItem("PlateCarrierVest", 			300,		320,		1,		250,	0 );
		AddItem("SmershVest", 					150,		170,		1,		250,	0 );
		AddItem("PressVest_Blue", 				90,			110,		1,		250,	0 );
		AddItem("PressVest_LightBlue", 			90,			110,		1,		250,	0 );
		AddItem("UKAssVest_Black", 				100,		120,		1,		250,	0 );
		AddItem("UKAssVest_Camo", 				100,		120,		1,		250,	0 );
		AddItem("UKAssVest_Khaki", 				100,		120,		1,		250,	0 );
		AddItem("UKAssVest_Olive", 				100,		120,		1,		250,	0 );
		AddItem("PoliceVest", 					40,			60,			1,		250,	0 );
		AddItem("PlateCarrierHolster", 			30,			50,			1,		250,	0 );
		AddItem("ChestHolster", 				150,		170,		1,		250,	0 );
		AddItem("HighCapacityVest_Black", 		250,		270,		1,		250,	0 );
		AddItem("HighCapacityVest_Olive", 		250,		270,		1,		250,	0 );
		AddItem("HuntingVest", 					60,			80,			1,		250,	0 );
		AddItem("ReflexVest", 					10,			30,			1,		250,	0 );
	}
};