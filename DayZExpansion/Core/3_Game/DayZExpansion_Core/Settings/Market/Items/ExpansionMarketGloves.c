/**
 * ExpansionMarketGloves.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketGloves: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
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
		super.Defaults();

		CategoryID = 26;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_GLOVES";
		m_FileName = "Gloves";

		AddItem("SurgicalGloves_Blue", 				1,		3,		1,		250,	0 );
		AddItem("SurgicalGloves_LightBlue",			1,		3,		1,		250,	0 );
		AddItem("SurgicalGloves_Green", 			1,		3,		1,		250,	0 );
		AddItem("SurgicalGloves_White", 			1,		3,		1,		250,	0 );
		
		AddItem("WorkingGloves_Black", 				3,		5,		1,		250,	0 );
		AddItem("WorkingGloves_Beige", 				3,		5,		1,		250,	0 );
		AddItem("WorkingGloves_Brown", 				3,		5,		1,		250,	0 );
		AddItem("WorkingGloves_Yellow", 			3,		5,		1,		250,	0 );
	
		AddItem("TacticalGloves_Black", 			4,		6,		1,		250,	0 );
		AddItem("TacticalGloves_Beige", 			4,		6,		1,		250,	0 );
		AddItem("TacticalGloves_Green", 			4,		6,		1,		250,	0 );
		
		AddItem("OMNOGloves_Gray", 					6,		8,		1,		250,	0 );
		AddItem("OMNOGloves_Brown", 				6,		8,		1,		250,	0 );
		
		AddItem("NBCGlovesGray", 					7,		9,		1,		250,	0 );
	}
};