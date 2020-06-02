/**
 * ExpansionMarketButtstocks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketButtstocks: ExpansionMarketCategory
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

		CategoryID = 12;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BUTTSTOCKS";
		m_FileName = "Buttstocks";
		
		AddItem("M4_OEBttstck", 				15,	20,	1,		250,	0 );
		AddItem("M4_OEBttstck_Black", 			15,	20,	1,		250,	0 );
		AddItem("M4_OEBttstck_Green", 			15,	20,	1,		250,	0 );
		AddItem("M4_MPBttstck", 				10,	15,	1,		250,	0 );
		AddItem("M4_MPBttstck_Black", 			10,	15,	1,		250,	0 );
		AddItem("M4_MPBttstck_Green", 			10,	15,	1,		250,	0 );
		AddItem("M4_CQBBttstck", 				20,	25,	1,		250,	0 );
		AddItem("M4_CQBBttstck_Black", 			20,	25,	1,		250,	0 );
		AddItem("M4_CQBBttstck_Green", 			20,	25,	1,		250,	0 );
		AddItem("AK_WoodBttstck", 				15,	20,	1,		250,	0 );
		AddItem("AK_WoodBttstck_Black", 		15,	20,	1,		250,	0 );
		AddItem("AK_WoodBttstck_Camo", 			15,	20,	1,		250,	0 );
		AddItem("AK74_WoodBttstck", 			20,	25,	1,		250,	0 );
		AddItem("AK74_WoodBttstck_Black", 		20,	25,	1,		250,	0 );
		AddItem("AK74_WoodBttstck_Camo", 		20,	25,	1,		250,	0 );
		AddItem("AK_FoldingBttstck", 			25,	30,	1,		250,	0 );
		AddItem("AK_FoldingBttstck_Black", 		25,	30,	1,		250,	0 );
		AddItem("AK_FoldingBttstck_Green", 		25,	30,	1,		250,	0 );
		AddItem("AK_PlasticBttstck", 			10,	15,	1,		250,	0 );
		AddItem("AK_PlasticBttstck_Black", 		10,	15,	1,		250,	0 );
		AddItem("AK_PlasticBttstck_Green", 		10,	15,	1,		250,	0 );
		AddItem("AKS74U_Bttstck", 				25,	30,	1,		250,	0 );
		AddItem("AKS74U_Bttstck_Black", 		25,	30,	1,		250,	0 );
		AddItem("AKS74U_Bttstck_Green", 		25,	30,	1,		250,	0 );
		AddItem("MP5k_StockBttstck", 			10,	15,	1,		250,	0 );
		AddItem("Fal_OeBttstck", 				10,	15,	1,		250,	0 );
		AddItem("Fal_FoldingBttstck", 			25,	30,	1,		250,	0 );
		AddItem("GhillieAtt_Tan", 				25,	30,	1,		250,	0 );
		AddItem("GhillieAtt_Woodland", 			25,	30,	1,		250,	0 );
		AddItem("GhillieAtt_Mossy", 			25,	30,	1,		250,	0 );
	}
}