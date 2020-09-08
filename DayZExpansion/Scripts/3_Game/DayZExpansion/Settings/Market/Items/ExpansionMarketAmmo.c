/**
 * ExpansionMarketAmmo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketAmmo: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	//
	// Price calculation and legend:
	// SN - Silver Nugget 	- 0.1
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

		CategoryID = 1;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_AMMO";
		m_FileName = "Ammo";

		AddItem("ExpansionAmmoFlareLight", 		0.30,		0.50,		1,		500,	0 );
		AddItem("Ammo_308Win", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_308WinTracer", 			0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_22", 						0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_762x39", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_762x54", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_12gaPellets", 			0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_12gaSlug", 				0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_45ACP", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_9x19", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_9x39", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_9x39AP", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_380", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_545x39", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_545x39Tracer", 			0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_556x45", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Ammo_Flare", 					0.50,		1.00,		1,		500,	0 );
		AddItem("Expansion_Ammo_ArrowBolt", 			0.50,		1.00,		1,		500,	0 );
		
		AddItem("ExpansionAmmoFlare", 			100.00,		125.00,		1,		500,	0 );
		AddItem("ExpansionAmmoLAW", 			50.00,		75.00,		1,		500,	0 );
		AddItem("ExpansionAmmoRPG", 			50.00,		75.00,		1,		500,	0 );
		
		AddItem("Ammo_Expansion_M203_Smoke_White", 			0.60,		1.10,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Smoke_Red", 			0.60,		1.10,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Smoke_Green", 			0.60,		1.10,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Smoke_Yellow", 		0.60,		1.10,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Smoke_Purple", 		0.60,		1.10,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Smoke_Teargas", 		5.00,		6.50,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White", 	0.80,		1.30,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Red", 	0.80,		1.30,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Green", 	0.80,		1.30,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Yellow", 	0.80,		1.30,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Purple", 	0.80,		1.30,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas", 5.80,		7.30,	  1,	250,	0 );
		AddItem("Ammo_Expansion_M203_HE", 					50.00,		75.00,	  1,	250,	0 );
		
		AddItem("Ammo_Expansion_46x30", 					0.60,		1.10,	  1,	500,	0 );
		AddItem("Ammo_Expansion_338", 						0.50,		1.00,	  1,	500,	0 );
		AddItem("Expansion_Ammo_8mm", 						0.50,		1.00,	  1,	500,	0 );
	}
}