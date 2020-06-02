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

class ExpansionMarketBackpacks: ExpansionMarketCategory
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
		
		CategoryID = 5;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BACKPACKS";
		m_FileName = "Backpacks";
		
		AddItem("ChildBag_Green", 					20,			70,			1,		250,	0 );
		AddItem("ChildBag_Red", 					20,			70,			1,		250,	0 );
		AddItem("ChildBag_Blue", 					20,			70,			1,		250,	0 );
		AddItem("DryBag_Orange", 					30,			80,			1,		250,	0 );
		AddItem("DryBag_Yellow", 					30,			80,			1,		250,	0 );
		AddItem("DryBag_Blue", 						30,			80,			1,		250,	0 );
		AddItem("DryBag_Green", 					30,			80,			1,		250,	0 );
		AddItem("DryBag_Black", 					30,			80,			1,		250,	0 );
		AddItem("DryBag_Red", 						30,			80,			1,		250,	0 );
		AddItem("TaloonBag_Blue", 					50,			100,		1,		250,	0 );
		AddItem("TaloonBag_Green", 					50,			100,		1,		250,	0 );
		AddItem("TaloonBag_Orange", 				50,			100,		1,		250,	0 );
		AddItem("TaloonBag_Violet", 				50,			100,		1,		250,	0 );
		AddItem("SmershBag", 						100,		150,		1,		250,	0 );
		AddItem("AssaultBag_Ttsko", 				110,		150,		1,		250,	0 );
		AddItem("AssaultBag_Black", 				110,		150,		1,		250,	0 );
		AddItem("AssaultBag_Green", 				110,		150,		1,		250,	0 );
		AddItem("HuntingBag", 						130,		180,		1,		250,	0 );
		AddItem("TortillaBag", 						180,		230,		1,		250,	0 );
		AddItem("CoyoteBag_Brown", 					180,		230,		1,		250,	0 );
		AddItem("CoyoteBag_Green", 					180,		230,		1,		250,	0 );
		AddItem("MountainBag_Red", 					300,		350,		1,		250,	0 );
		AddItem("MountainBag_Blue", 				300,		350,		1,		250,	0 );
		AddItem("MountainBag_Orange", 				300,		350,		1,		250,	0 );
		AddItem("MountainBag_Green", 				300,		350,		1,		250,	0 );
		AddItem("AliceBag_Green", 					400,		450,		1,		250,	0 );
		AddItem("AliceBag_Black", 					400,		450,		1,		250,	0 );
		AddItem("AliceBag_Camo", 					400,		450,		1,		250,	0 );
	}
}