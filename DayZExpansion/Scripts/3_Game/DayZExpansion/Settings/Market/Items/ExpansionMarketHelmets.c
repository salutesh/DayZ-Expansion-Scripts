/**
 * ExpansionMarketHelmets.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHelmets: ExpansionMarketCategory
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

		CategoryID = 30;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_HELMETS";
		m_FileName = "Helmets";

		AddItem("BallisticHelmet_Green", 			35,		37,		1,		250,	0 );
		AddItem("BallisticHelmet_Black", 			35,		37,		1,		250,	0 );
		AddItem("BallisticHelmet_UN", 				35,		37,		1,		250,	0 );
		
		AddItem("ZSh3PilotHelmet", 					13,		15,		1,		250,	0 );
		AddItem("ZSh3PilotHelmet_Green", 			13,		15,		1,		250,	0 );
		AddItem("ZSh3PilotHelmet_Black", 			13,		15,		1,		250,	0 );
		
		AddItem("ConstructionHelmet_Blue", 			3,		5,		1,		250,	0 );
		AddItem("ConstructionHelmet_Lime", 			3,		5,		1,		250,	0 );
		AddItem("ConstructionHelmet_Orange", 		3,		5,		1,		250,	0 );
		AddItem("ConstructionHelmet_Red", 			3,		5,		1,		250,	0 );
		AddItem("ConstructionHelmet_White", 		3,		5,		1,		250,	0 );
		AddItem("ConstructionHelmet_Yellow", 		3,		5,		1,		250,	0 );
		
		AddItem("MotoHelmet_Black",					7,		9,		1,		250,	0 );
		AddItem("MotoHelmet_Grey", 					7,		9,		1,		250,	0 );
		AddItem("MotoHelmet_Green", 				7,		9,		1,		250,	0 );
		AddItem("MotoHelmet_Lime", 					7,		9,		1,		250,	0 );
		AddItem("MotoHelmet_Blue", 					7,		9,		1,		250,	0 );
		AddItem("MotoHelmet_Red", 					7,		9,		1,		250,	0 );
		AddItem("MotoHelmet_White", 				7,		9,		1,		250,	0 );
		
		AddItem("DarkMotoHelmet_Black", 			8,		10,		1,		250,	0 );
		AddItem("DarkMotoHelmet_Grey", 				8,		10,		1,		250,	0 );
		AddItem("DarkMotoHelmet_Green", 			8,		10,		1,		250,	0 );
		AddItem("DarkMotoHelmet_Lime", 				8,		10,		1,		250,	0 );
		AddItem("DarkMotoHelmet_Blue", 				8,		10,		1,		250,	0 );
		AddItem("DarkMotoHelmet_Red", 				8,		10,		1,		250,	0 );
		AddItem("DarkMotoHelmet_White", 			8,		10,		1,		250,	0 );
		
		AddItem("TankerHelmet", 					10,		12,		1,		250,	0 );
		
		AddItem("GorkaHelmetVisor", 				5,		7,		1,		250,	0 );
		AddItem("GorkaHelmet", 						20,		22,		1,		250,	0 );
		AddItem("GorkaHelmet_Green", 				20,		22,		1,		250,	0 );
		AddItem("GorkaHelmet_Black", 				20,		22,		1,		250,	0 );
		
		AddItem("GreatHelm", 						12,		14,		1,		250,	0 );
		
		AddItem("Ssh68Helmet", 						10,		12,		1,		250,	0 );
		
		AddItem("DirtBikeHelmet_Mouthguard", 		2,		4,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Visor", 			3,		6,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Green", 			6,		8,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Chernarus", 		6,		8,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Khaki", 			6,		8,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Police", 			6,		8,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Red", 				6,		8,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Black", 			6,		8,		1,		250,	0 );
		AddItem("DirtBikeHelmet_Blue", 				6,		8,		1,		250,	0 );
		
		AddItem("HockeyHelmet_Black", 				5,		7,		1,		250,	0 );
		AddItem("HockeyHelmet_Blue", 				5,		7,		1,		250,	0 );
		AddItem("HockeyHelmet_Red", 				5,		7,		1,		250,	0 );
		AddItem("HockeyHelmet_White", 				5,		7,		1,		250,	0 );
				
		AddItem("Mich2001Helmet", 					15,		17,		1,		250,	0 );
			
		AddItem("FirefightersHelmet_Red", 			15,		17,		1,		250,	0 );
		AddItem("FirefightersHelmet_White", 		15,		17,		1,		250,	0 );
		AddItem("FirefightersHelmet_Yellow", 		15,		17,		1,		250,	0 );
		
		AddItem("SkateHelmet_Black", 				5,		7,		1,		250,	0 );
		AddItem("SkateHelmet_Blue",					5,		7,		1,		250,	0 );
		AddItem("SkateHelmet_Gray", 				5,		7,		1,		250,	0 );
		AddItem("SkateHelmet_Green", 				5,		7,		1,		250,	0 );
		AddItem("SkateHelmet_Red", 					5,		7,		1,		250,	0 );
	}
}