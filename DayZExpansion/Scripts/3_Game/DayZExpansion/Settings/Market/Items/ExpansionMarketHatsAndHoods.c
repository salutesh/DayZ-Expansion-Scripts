/**
 * ExpansionMarketHatsAndHoods.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHatsAndHoods: ExpansionMarketCategory
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

		CategoryID = 28;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_HATS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_HOODS");
		m_FileName = "Hats_And_Hoods";
		
		AddItem("BeanieHat_Beige", 					3,		3,		1,		250,	0 );
		AddItem("BeanieHat_Black", 					3,		3,		1,		250,	0 );
		AddItem("BeanieHat_Blue", 					3,		3,		1,		250,	0 );
		AddItem("BeanieHat_Brown", 					3,		3,		1,		250,	0 );
		AddItem("BeanieHat_Green", 					3,		3,		1,		250,	0 );
		AddItem("BeanieHat_Grey", 					3,		3,		1,		250,	0 );
		AddItem("BeanieHat_Pink", 					3,		3,		1,		250,	0 );
		AddItem("BeanieHat_Red", 					3,		3,		1,		250,	0 );
		
		AddItem("ZmijovkaCap_Black", 				3,		3,		1,		250,	0 );
		AddItem("ZmijovkaCap_Blue", 				3,		3,		1,		250,	0 );
		AddItem("ZmijovkaCap_Brown",				3,		3,		1,		250,	0 );
		AddItem("ZmijovkaCap_Green", 				3,		3,		1,		250,	0 );
		AddItem("ZmijovkaCap_Red", 					3,		3,		1,		250,	0 );
		
		AddItem("Ushanka_Black", 					5,		5,		1,		250,	0 );
		AddItem("Ushanka_Blue", 					5,		5,		1,		250,	0 );
		AddItem("Ushanka_Green", 					5,		5,		1,		250,	0 );
		
		AddItem("MilitaryBeret_Red", 				3,		3,		1,		250,	0 );
		AddItem("MilitaryBeret_UN", 				3,		3,		1,		250,	0 );
		AddItem("MilitaryBeret_NZ", 				3,		3,		1,		250,	0 );
		AddItem("MilitaryBeret_ChDKZ", 				3,		3,		1,		250,	0 );
		AddItem("MilitaryBeret_CDF", 				3,		3,		1,		250,	0 );
		
		AddItem("CowboyHat_Brown", 					2,		2,		1,		250,	0 );
		AddItem("CowboyHat_black", 					2,		2,		1,		250,	0 );
		AddItem("CowboyHat_darkBrown", 				2,		2,		1,		250,	0 );
		AddItem("CowboyHat_green", 					2,		2,		1,		250,	0 );
		
		AddItem("BoonieHat_Black", 					2,		2,		1,		250,	0 );
		AddItem("BoonieHat_Blue", 					2,		2,		1,		250,	0 );
		AddItem("BoonieHat_DPM", 					2,		2,		1,		250,	0 );
		AddItem("BoonieHat_Dubok", 					2,		2,		1,		250,	0 );
		AddItem("BoonieHat_Flecktran", 				2,		2,		1,		250,	0 );
		AddItem("BoonieHat_NavyBlue", 				2,		2,		1,		250,	0 );
		AddItem("BoonieHat_Olive", 					2,		2,		1,		250,	0 );
		AddItem("BoonieHat_Orange", 				2,		2,		1,		250,	0 );
		AddItem("BoonieHat_Red", 					2,		2,		1,		250,	0 );
		AddItem("BoonieHat_Tan", 					2,		2,		1,		250,	0 );
		
		AddItem("OfficerHat", 						2,		2,		1,		250,	0 );
		
		AddItem("WeldingMask", 						7,		7,		1,		250,	0 );
		
		AddItem("MedicalScrubsHat_Blue", 			5,		6,		1,		250,	0 );
		AddItem("MedicalScrubsHat_White", 			5,		6,		1,		250,	0 );
		AddItem("MedicalScrubsHat_Green", 			5,		6,		1,		250,	0 );
		
		AddItem("NBCHoodGray", 						10,		10,		1,		250,	0 );
	}
};