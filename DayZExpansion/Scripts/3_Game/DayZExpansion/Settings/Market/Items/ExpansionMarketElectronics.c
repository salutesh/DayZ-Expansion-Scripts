/**
 * ExpansionMarketElectronics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketElectronics: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	//
	// Price calculation and legend:
	// SN - Silver Nugget 	- .1
	// SB - Silver Bar 		- 1
	// GN - Gold Nugget 	- 10
	// GB - Gold Bar 		- 2500
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

		CategoryID = 18;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_ELECTRONICS";
		m_FileName = "Electronics";

		AddItem("PersonalRadio", 					100,		120,		1,		250,	0 );
		AddItem("Megaphone", 						300,		320,		1,		250,	0 );
		AddItem("ElectronicRepairKit", 				350,		370,		1,		250,	0 );
		AddItem("CableReel", 						400,		420,		1,		250,	0 );
		AddItem("BatteryCharger", 					400,		420,		1,		250,	0 );
		AddItem("BaseRadio", 						1000,		1200,		1,		250,	0 );
		AddItem("Rangefinder", 						1000,		1200,		1,		250,	0 );
		AddItem("NVGoggles", 						5000,		5500,		1,		250,	0 );
	}
}