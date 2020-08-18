/**
 * ExpansionMarketExplosives.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketExplosives: ExpansionMarketCategory
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

		CategoryID = 20;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_EXPLOSIVES" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_GRENADES");
		m_FileName = "Explosives_And_Grenades";
		
		// Explosives	
		AddItem("RGD5Grenade", 				50,		52,		1,		150,	0 );
		AddItem("M67Grenade", 				50,		52,		1,		150,	0 );
		
		// Smokes
		AddItem("M18SmokeGrenade_Red", 		8,		10,		1,		250,	0 );
		AddItem("M18SmokeGrenade_Green", 	8,		10,		1,		250,	0 );
		AddItem("M18SmokeGrenade_Yellow", 	8,		10,		1,		250,	0 );
		AddItem("M18SmokeGrenade_Purple", 	8,		10,		1,		250,	0 );
		AddItem("M18SmokeGrenade_White", 	8,		10,		1,		250,	0 );
		AddItem("RDG2SmokeGrenade_Black", 	8,		10,		1,		250,	0 );
		AddItem("RDG2SmokeGrenade_White", 	8,		10,		1,		250,	0 );
		
		AddItem("FlashGrenade", 			6,		8,		1,		250,	0 );
		
		// Expansion Explosives
		AddItem("ExpansionSatchel", 	25000,	26000,	1,		100,	0 );
	}
}