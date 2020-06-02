/**
 * ExpansionMarketBlousesAndSuits.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBlousesAndSuits: ExpansionMarketCategory
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

		CategoryID = 9;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_BLOUSES" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_SUITS");
		m_FileName = "Blouses_And_Suits";

		AddItem("Blouse_Blue", 							5,		10,		1,		250,	0 );
		AddItem("Blouse_Green", 						5,		10,		1,		250,	0 );
		AddItem("Blouse_Violet", 						5,		10,		1,		250,	0 );
		AddItem("Blouse_White", 						5,		10,		1,		250,	0 );
			
		AddItem("ManSuit_Beige", 						7,		12,		1,		250,	0 );
		AddItem("ManSuit_Black", 						7,		12,		1,		250,	0 );
		AddItem("ManSuit_Blue", 						7,		12,		1,		250,	0 );
		AddItem("ManSuit_Brown", 						7,		12,		1,		250,	0 );
		AddItem("ManSuit_DarkGrey",						7,		12,		1,		250,	0 );
		AddItem("ManSuit_Khaki", 						7,		12,		1,		250,	0 );
		AddItem("ManSuit_LightGrey", 					7,		12,		1,		250,	0 );
		AddItem("ManSuit_White", 						7,		12,		1,		250,	0 );
			
		AddItem("WomanSuit_Beige", 						8,		13,		1,		250,	0 );
		AddItem("WomanSuit_Black", 						8,		13,		1,		250,	0 );
		AddItem("WomanSuit_Blue", 						8,		13,		1,		250,	0 );
		AddItem("WomanSuit_Brown", 						8,		13,		1,		250,	0 );
		AddItem("WomanSuit_DarkGrey", 					8,		13,		1,		250,	0 );
		AddItem("WomanSuit_Khaki", 						8,		13,		1,		250,	0 );
		AddItem("WomanSuit_LightGrey", 					8,		13,		1,		250,	0 );
		AddItem("WomanSuit_White", 						8,		13,		1,		250,	0 );
	}
}