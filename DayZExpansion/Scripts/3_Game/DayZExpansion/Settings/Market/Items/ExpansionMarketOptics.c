/**
 * ExpansionMarketOptics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketOptics: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	//
	// Price calculation and legend:
	// SN - Silver Nugget 	- .1
	// SB - Silver Bar 		- 1
	// GN - Gold Nugget 	- 10
	// GB - Gold Bar 		- 1000
	//
	//	string 	ClassName 			- Item class name that gets defined
	//	float 	MinPriceThreshold 	- Min price for the price threshold calculation
	//	float	MaxPriceThreshold 	- Max price for the price threshold calculation
	//	int		MinStockThreshold 	- Min stock for the price threshold calculation
	//	int		MaxStockThreshold 	- Max stock for the price threshold calculation
	//	int		PurchaseType		- 0 = Normal Item Trader / 1 = Vehicle Trader
	//	array	SpawnAttachments	- Array of item class names that gets attached to this item on buy.
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 44;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_OPTICS";
		m_FileName = "Optics";
		
		// OPTICS
		AddItem("M4_CarryHandleOptic", 			15,			17,		1,		250,	0 );
		AddItem("BUISOptic", 					15,			17,		1,		250,	0 );
		AddItem("M68Optic", 					50,			70,		1,		250,	0 );
		AddItem("M4_T3NRDSOptic", 				50,			70,		1,		250,	0 );
		AddItem("FNP45_MRDSOptic", 				25,			45,		1,		250,	0 );
		AddItem("ReflexOptic", 					15,			35,		1,		250,	0 );
		AddItem("ACOGOptic", 					30,			50,		1,		250,	0 );
		AddItem("PUScopeOptic", 				90,			110,	1,		250,	0 );
		AddItem("KashtanOptic", 				90,			110,	1,		250,	0 );
		AddItem("HuntingOptic", 				100,		120,	1,		250,	0 );
		AddItem("PSO1Optic", 					110,		130,	1,		250,	0 );
		AddItem("PSO11Optic", 					140,		160,	1,		250,	0 );
		AddItem("KobraOptic", 					60,			80,		1,		250,	0 );
		AddItem("PistolOptic", 					60,			80,		1,		250,	0 );

		AddItem("ExpansionReflexMRSOptic", 		15,			35,		1,		250,	0 );
		AddItem("ExpansionDeltapointOptic", 	15,			35,		1,		250,	0 );
		AddItem("ExpansionReflexMRSOptic", 		15,			35,		1,		250,	0 );
		AddItem("ExpansionEXPS3HoloOptic", 		25,			45,		1,		250,	0 );
		AddItem("ExpansionHAMROptic", 			45,			65,		1,		250,	0 );
		AddItem("Expansion_PMII25Optic", 		55,			75,		1,		250,	0 );
		AddItem("ExpansionKar98ScopeOptic", 	55,			75,		1,		250,	0 );
		AddItem("Expansion_M1A_RailAtt", 		55,			75,		1,		250,	0 );
		AddItem("Expansion_ANPEQ15_Red", 		55,			75,		1,		250,	0 );
		AddItem("Expansion_ANPEQ15_Green", 		55,			75,		1,		250,	0 );
	}	
}