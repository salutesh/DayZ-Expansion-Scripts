/**
 * ExpansionMarketBackpacks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketGhillies: ExpansionMarketCategory
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

		CategoryID = 25;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_GHILLIES";
		m_FileName = "Ghillies";
		
		AddItem("GhillieHood_Tan", 					2200,		2400,		1,		250,	0 );
		AddItem("GhillieHood_Woodland", 			2200,		2400,		1,		250,	0 );
		AddItem("GhillieHood_Mossy", 				2200,		2400,		1,		250,	0 );
		
		AddItem("GhillieBushrag_Tan", 				5000,		5200,		1,		250,	0 );
		AddItem("GhillieBushrag_Woodland", 			5000,		5200,		1,		250,	0 );
		AddItem("GhillieBushrag_Mossy", 			5000,		5200,		1,		250,	0 );
		
		AddItem("GhillieTop_Tan", 					7000,		7200,		1,		250,	0 );
		AddItem("GhillieTop_Woodland", 				7000,		7200,		1,		250,	0 );
		AddItem("GhillieTop_Mossy", 				7000,		7200,		1,		250,	0 );
		
		AddItem("GhillieSuit_Tan", 					10000,		12000,		1,		250,	0 );
		AddItem("GhillieSuit_Woodland", 			10000,		12000,		1,		250,	0 );
		AddItem("GhillieSuit_Mossy", 				10000,		12000,		1,		250,	0 );
	}
};