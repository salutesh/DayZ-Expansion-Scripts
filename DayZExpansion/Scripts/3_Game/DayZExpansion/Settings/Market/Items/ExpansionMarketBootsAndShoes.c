/**
 * ExpansionMarketBootsAndShoes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBootsAndShoes: ExpansionMarketCategory
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

		CategoryID = 11;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_BOOTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_SHOES");
		m_FileName = "Boots_And_Shoes";

		AddItem("AthleticShoes_Blue", 				2,	5,	1,		250,	0 );
		AddItem("AthleticShoes_Black", 				2,	5,	1,		250,	0 );
		AddItem("AthleticShoes_Brown", 				2,	5,	1,		250,	0 );
		AddItem("AthleticShoes_Green", 				2,	5,	1,		250,	0 );
		AddItem("AthleticShoes_Grey", 				2,	5,	1,		250,	0 );
		
		AddItem("HikingBoots_Brown", 				8,	10,	1,		250,	0 );
		AddItem("HikingBoots_Black", 				8,	10,	1,		250,	0 );
		
		AddItem("HikingBootsLow_Black", 			5,	7,	1,		250,	0 );
		AddItem("HikingBootsLow_Blue", 				5,	7,	1,		250,	0 );
		AddItem("HikingBootsLow_Beige", 			5,	7,	1,		250,	0 );
		AddItem("HikingBootsLow_Grey", 				5,	7,	1,		250,	0 );
	
		AddItem("Wellies_Black", 					9,	15,	1,		250,	0 );
		AddItem("Wellies_Brown", 					9,	15,	1,		250,	0 );
		AddItem("Wellies_Green", 					9,	15,	1,		250,	0 );
		AddItem("Wellies_Grey", 					9,	15,	1,		250,	0 );
		
		AddItem("WorkingBoots_Grey", 				6,	8,	1,		250,	0 );
		AddItem("WorkingBoots_Brown", 				6,	8,	1,		250,	0 );
		AddItem("WorkingBoots_Green", 				6,	8,	1,		250,	0 );
		AddItem("WorkingBoots_Yellow", 				6,	8,	1,		250,	0 );
		AddItem("WorkingBoots_Beige", 				6,	8,	1,		250,	0 );
	
		AddItem("JungleBoots_Beige", 				9,	15,	1,		250,	0 );
		AddItem("JungleBoots_Black", 				9,	15,	1,		250,	0 );
		AddItem("JungleBoots_Brown", 				9,	15,	1,		250,	0 );
		AddItem("JungleBoots_Green", 				9,	15,	1,		250,	0 );
		AddItem("JungleBoots_Olive", 				9,	15,	1,		250,	0 );
	
		AddItem("DressShoes_Beige", 				4,	6,	1,		250,	0 );
		AddItem("DressShoes_Black", 				4,	6,	1,		250,	0 );
		AddItem("DressShoes_Brown", 				4,	6,	1,		250,	0 );
		AddItem("DressShoes_Sunburst", 				4,	6,	1,		250,	0 );
		AddItem("DressShoes_White", 				4,	6,	1,		250,	0 );
	
		AddItem("MilitaryBoots_Beige", 				11,	13,	1,		250,	0 );
		AddItem("MilitaryBoots_Black", 				11,	13,	1,		250,	0 );
		AddItem("MilitaryBoots_Bluerock", 			11,	13,	1,		250,	0 );
		AddItem("MilitaryBoots_Brown", 				11,	13,	1,		250,	0 );
		AddItem("MilitaryBoots_Redpunk", 			11,	13,	1,		250,	0 );
		
		AddItem("CombatBoots_Beige", 				8,	10,	1,		250,	0 );
		AddItem("CombatBoots_Black", 				8,	10,	1,		250,	0 );
		AddItem("CombatBoots_Brown", 				8,	10,	1,		250,	0 );
		AddItem("CombatBoots_Green", 				8,	10,	1,		250,	0 );
		AddItem("CombatBoots_Grey", 				8,	10,	1,		250,	0 );
		
		AddItem("JoggingShoes_Black", 				3,	5,	1,		250,	0 );
		AddItem("JoggingShoes_Blue", 				3,	5,	1,		250,	0 );
		AddItem("JoggingShoes_Red", 				3,	5,	1,		250,	0 );
		AddItem("JoggingShoes_Violet", 				3,	5,	1,		250,	0 );
		AddItem("JoggingShoes_White", 				3,	5,	1,		250,	0 );

		AddItem("Sneakers_Black", 					4,	6,	1,		250,	0 );
		AddItem("Sneakers_Gray", 					4,	6,	1,		250,	0 );
		AddItem("Sneakers_Green", 					4,	6,	1,		250,	0 );
		AddItem("Sneakers_Red", 					4,	6,	1,		250,	0 );
		AddItem("Sneakers_White", 					4,	6,	1,		250,	0 );
		
		AddItem("NBCBootsGray", 					15,	17,	1,		250,	0 );
		
		AddItem("Ballerinas_Blue", 					2,	4,	1,		250,	0 );
		AddItem("Ballerinas_Red", 					2,	4,	1,		250,	0 );
		AddItem("Ballerinas_White", 				2,	4,	1,		250,	0 );
		AddItem("Ballerinas_Yellow", 				2,	4,	1,		250,	0 );
		
		AddItem("TTSKOBoots", 						10,	12,	1,		250,	0 );
	}
};