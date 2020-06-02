/**
 * ExpansionMarketLights.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketLights: ExpansionMarketCategory
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

		CategoryID = 34;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_LIGHTS";
		m_FileName = "Lights";
		
		// Lights
		AddItem("Chemlight_Blue", 				2,		4,		1,		250,	0 );
		AddItem("Chemlight_Green", 				2,		4,		1,		250,	0 );
		AddItem("Chemlight_Red", 				2,		4,		1,		250,	0 );
		AddItem("Chemlight_White", 				2,		4,		1,		250,	0 );
		AddItem("Chemlight_Yellow", 			2,		4,		1,		250,	0 );
		AddItem("Roadflare", 					5,		7,		1,		250,	0 );
		AddItem("Matchbox", 					7,		9,		1,		250,	0 );
		AddItem("Heatpack", 					1,		4,		1,		250,	0 );
		AddItem("Flashlight", 					2,		6,		1,		250,	0 );
		AddItem("PortableGasLamp", 				3,		7,		1,		250,	0 );
		AddItem("PortableGasStove", 			3,		7,		1,		250,	0 );
		AddItem("Headtorch_Black", 				4,		8,		1,		250,	0 );
		AddItem("Headtorch_Grey", 				4,		8,		1,		250,	0 );
		AddItem("Spotlight", 					50,		54,		1,		250,	0 );
	}
}