/**
 * ExpansionMarketAssaultRifles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketAssaultRifles: ExpansionMarketCategory
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
		
		CategoryID = 4;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_ASSAULT_RIFLES";
		m_FileName = "Assault_Rifles";

		AddItem("FAL", 					100,		500,	1,		100,	0 );
		AddItem("AKM", 					150,		550,	1,		100,	0 );
		AddItem("M4A1", 				150,		550,	1,		100,	0 );
		AddItem("AK101", 				150,		550,	1,		100,	0 );
		AddItem("AK74", 				150,		550,	1,		100,	0 );
		AddItem("AKS74U", 				150,		550,	1,		100,	0 );
		
		AddItem("Expansion_M16", 		160,		560,	1,		100,	0 );
		AddItem("Expansion_G36", 		160,		560,	1,		100,	0 );
	}
}