/**
 * ExpansionMarketFood.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketFood: ExpansionMarketCategory
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

		CategoryID = 24;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FOOD";
		m_FileName = "Food";

		AddItem("BoxCerealCrunchin", 			50,			52,			1,		500,	0 );
		AddItem("PowderedMilk", 				20,			22,			1,		500,	0 );
		AddItem("Rice", 						50,			52,			1,		500,	0 );
		AddItem("Marmalade", 					50,			52,			1,		500,	0 );
		AddItem("BakedBeansCan", 				100,		102,		1,		500,	0 );
		AddItem("PeachesCan", 					50,			52,			1,		500,	0 );
		AddItem("TacticalBaconCan", 			100,		102,		1,		500,	0 );
		AddItem("SpaghettiCan", 				100,		102,		1,		500,	0 );
		AddItem("SardinesCan", 					100,		102,		1,		500,	0 );
		AddItem("TunaCan", 						50,			52,			1,		500,	0 );
		
		AddItem("ExpansionBread1", 				100,		102,		1,		500,	0 );
		AddItem("ExpansionBread2", 				100,		102,		1,		500,	0 );
		AddItem("ExpansionBread3", 				100,		102,		1,		500,	0 );
		AddItem("ExpansionCheese1", 			100,		102,		1,		500,	0 );
		AddItem("ExpansionCheese2", 			100,		102,		1,		500,	0 );
		AddItem("ExpansionCheese3", 			100,		102,		1,		500,	0 );
		AddItem("ExpansionCheese4", 			100,		102,		1,		500,	0 );
	}
};