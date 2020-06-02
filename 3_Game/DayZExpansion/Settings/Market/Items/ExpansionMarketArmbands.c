/**
 * ExpansionMarketArmbands.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketArmbands: ExpansionMarketCategory
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
		
		CategoryID = 3;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_ARMBANDS";
		m_FileName = "Armbands";

		AddItem("Armband_White", 					5,	10,		1,		250,	0 );
		AddItem("Armband_Yellow", 					5,	10,		1,		250,	0 );
		AddItem("Armband_Orange", 					5,	10,		1,		250,	0 );
		AddItem("Armband_Red", 						5,	10,		1,		250,	0 );
		AddItem("Armband_Green",					5,	10,		1,		250,	0 );
		AddItem("Armband_Pink", 					5,	10,		1,		250,	0 );
		AddItem("Armband_Blue", 					5,	10,		1,		250,	0 );
		AddItem("Armband_Black", 					5,	10,		1,		250,	0 );
	}
}