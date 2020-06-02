/**
 * ExpansionMarketMagazines.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMagazines: ExpansionMarketCategory
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

		CategoryID = 37;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MAGAZINES";
		m_FileName = "Magazines";

		// MAGAZINES
		AddItem("Mag_FNX45_15Rnd", 				4,		6,		1,		250,	0 );
		AddItem("Mag_CZ75_15Rnd", 				6,		8,		1,		250,	0 );
		AddItem("Mag_Glock_15Rnd", 				1,		5,		1,		250,	0 );
		AddItem("Mag_IJ70_8Rnd", 				3,		5,		1,		250,	0 );
		AddItem("Mag_MP5_30Rnd", 				20,		22,		1,		250,	0 );
		AddItem("Mag_CZ61_20Rnd", 				10,		12,		1,		250,	0 );
		AddItem("Mag_MKII_10Rnd", 				1,		12,		1,		250,	0 );
		AddItem("Mag_CLIP762x54_5Rnd", 			1,		12,		1,		250,	0 );
		AddItem("Mag_CLIP762x39_10Rnd", 		5,		7,		1,		250,	0 );
		AddItem("Mag_AKM_30Rnd", 				22,		24,		1,		250,	0 );
		AddItem("Mag_CZ527_5rnd", 				6,		8,		1,		250,	0 );
		AddItem("Mag_STANAG_30Rnd", 			22,		24,		1,		250,	0 );
		AddItem("Mag_AK101_30Rnd", 				22,		24,		1,		250,	0 );
		AddItem("Mag_AK101_30Rnd_Black", 		22,		24,		1,		250,	0 );
		AddItem("Mag_AK101_30Rnd_Green", 		22,		24,		1,		250,	0 );
		AddItem("Mag_AK74_30Rnd", 				22,		24,		1,		250,	0 );
		AddItem("Mag_AK74_30Rnd_Black", 		22,		24,		1,		250,	0 );
		AddItem("Mag_AK74_30Rnd_Green", 		22,		24,		1,		250,	0 );
		AddItem("Mag_SVD_10Rnd", 				25,		27,		1,		250,	0 );
		AddItem("Mag_FAL_20Rnd", 				10,		12,		1,		250,	0 );
		AddItem("Mag_UMP_25Rnd", 				15,		17,		1,		250,	0 );
		AddItem("Mag_VSS_10Rnd", 				22,		24,		1,		250,	0 );
		
		AddItem("Mag_Expansion_M9_15Rnd", 		5,		7,		1,		250,	0 );
		AddItem("Mag_Expansion_MPX_40Rnd", 		20,		22,		1,		250,	0 );
		AddItem("Mag_Expansion_MPX_50Rnd", 		22,		24,		1,		250,	0 );
		AddItem("Mag_Expansion_Keldr_20Rnd", 	15,		17,		1,		250,	0 );
		AddItem("Mag_Expansion_AWM_5Rnd", 		15,		17,		1,		250,	0 );
	}
}