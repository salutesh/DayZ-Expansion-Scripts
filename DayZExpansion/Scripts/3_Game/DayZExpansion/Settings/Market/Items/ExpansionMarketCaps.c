/**
 * ExpansionMarketCaps.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCaps: ExpansionMarketCategory
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

		CategoryID = 13;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_CAPS";
		m_FileName = "Caps";
		
		AddItem("BaseballCap_Blue", 				1,		2,		1,		100,	0 );
		AddItem("BaseballCap_Beige", 				1,		2,		1,		100,	0 );
		AddItem("BaseballCap_Black", 				1,		2,		1,		100,	0 );
		AddItem("BaseballCap_Olive", 				1,		2,		1,		100,	0 );
		AddItem("BaseballCap_Pink", 				1,		2,		1,		100,	0 );
		AddItem("BaseballCap_Red", 					1,		2,		1,		100,	0 );
		AddItem("BaseballCap_Camo", 				1,		2,		1,		100,	0 );
		AddItem("BaseballCap_CMMG_Black", 			1,		2,		1,		100,	0 );
		AddItem("BaseballCap_CMMG_Pink", 			1,		2,		1,		100,	0 );
		
		AddItem("RadarCap_Black", 					4,		5,		1,		100,	0 );
		AddItem("RadarCap_Blue", 					4,		5,		1,		100,	0 );
		AddItem("RadarCap_Brown", 					4,		5,		1,		100,	0 );
		AddItem("RadarCap_Green",					4,		5,		1,		100,	0 );
		AddItem("RadarCap_Red", 					4,		5,		1,		100,	0 );
		
		AddItem("FlatCap_Black", 					4,		4,		1,		100,	0 );
		AddItem("FlatCap_Blue", 					4,		4,		1,		100,	0 );
		AddItem("FlatCap_Red", 						4,		4,		1,		100,	0 );
		AddItem("FlatCap_Brown", 					4,		4,		1,		100,	0 );
		AddItem("FlatCap_Grey", 					4,		4,		1,		100,	0 );
		AddItem("FlatCap_BrownCheck", 				4,		4,		1,		100,	0 );
		AddItem("FlatCap_GreyCheck", 				4,		4,		1,		100,	0 );
		AddItem("FlatCap_BlackCheck", 				4,		4,		1,		100,	0 );
		
		AddItem("PoliceCap", 						3,		3,		1,		100,	0 );
		
		AddItem("PilotkaCap", 						3,		3,		1,		100,	0 );
		
		AddItem("PrisonerCap", 						3,		3,		1,		100,	0 );
	}
}