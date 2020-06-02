/**
 * ExpansionMarketAmmoBoxes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketAmmoBoxes: ExpansionMarketCategory
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

		CategoryID = 2;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_AMMOBOXES";
		m_FileName = "Ammo_Boxes";

		AddItem("AmmoBox_556x45_20Rnd", 					100,		150,		1,		500,	0 );
		AddItem("AmmoBox_556x45Tracer_20Rnd", 				100,		150,		1,		500,	0 );
		AddItem("AmmoBox_308Win_20Rnd", 					100,		150,		1,		500,	0 );
		AddItem("AmmoBox_762x54_20Rnd", 					100,		150,		1,		500,	0 );
		AddItem("AmmoBox_762x54Tracer_20Rnd", 				100,		150,		1,		500,	0 );
		AddItem("AmmoBox_762x39_20Rnd", 					100,		200,		1,		500,	0 );
		AddItem("AmmoBox_762x39Tracer_20Rnd", 				100,		200,		1,		500,	0 );
		AddItem("AmmoBox_22_50Rnd", 						100,		200,		1,		500,	0 );
		AddItem("AmmoBox_45ACP_25rnd", 						125,		250,		1,		500,	0 );
		AddItem("AmmoBox_9x19_25rnd", 						120,		250,		1,		500,	0 );
		AddItem("AmmoBox_9x39_20rnd", 						100,		200,		1,		500,	0 );
		AddItem("AmmoBox_9x39AP_20rnd", 					100,		200,		1,		500,	0 );
		AddItem("AmmoBox_380_35rnd", 						175,		350,		1,		500,	0 );
		AddItem("AmmoBox_00buck_10rnd", 					50,			100,		1,		500,	0 );
		AddItem("AmmoBox_12gaSlug_10Rnd", 					50,			100,		1,		500,	0 );
		AddItem("AmmoBox_545x39_20Rnd", 					100,		200,		1,		500,	0 );
		AddItem("AmmoBox_545x39Tracer_20Rnd", 				100,		200,		1,		500,	0 );
		AddItem("AmmoBox_9x39_20Rnd", 						100,		200,		1,		500,	0 );
		
		AddItem("AmmoBox_Expansion_46x30_25rnd", 			100,		200,		1,		500,	0 );
		AddItem("AmmoBox_Expansion_338_15rnd", 				100,		200,		1,		500,	0 );
	}
}