/**
 * ExpansionMarketCoatsAndJackets.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCoatsAndJackets: ExpansionMarketCategory
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

		CategoryID = 15;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_COATS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_JACKETS");
		m_FileName = "Coats_And_Jackets";
		
		AddItem("HikingJacket_Black", 					5,		7,		1,		250,	0 );
		AddItem("HikingJacket_Blue", 					5,		7,		1,		250,	0 );
		AddItem("HikingJacket_Red", 					5,		7,		1,		250,	0 );
		AddItem("HikingJacket_Green", 					5,		7,		1,		250,	0 );
	
		AddItem("Raincoat_Orange", 						7,		9,		1,		250,	0 );
		AddItem("Raincoat_Green", 						7,		9,		1,		250,	0 );
		AddItem("Raincoat_Yellow", 						7,		9,		1,		250,	0 );
		AddItem("Raincoat_Pink", 						7,		9,		1,		250,	0 );
		AddItem("Raincoat_Red", 						7,		9,		1,		250,	0 );
		AddItem("Raincoat_Blue", 						7,		9,		1,		250,	0 );
		AddItem("Raincoat_Black", 						7,		9,		1,		250,	0 );
		
		AddItem("M65Jacket_Black", 						13,		15,		1,		250,	0 );
		AddItem("M65Jacket_Khaki", 						13,		15,		1,		250,	0 );
		AddItem("M65Jacket_Tan", 						13,		15,		1,		250,	0 );
		AddItem("M65Jacket_Olive", 						13,		15,		1,		250,	0 );
		
		AddItem("TTsKOJacket_Camo", 					10,		13,		1,		250,	0 );
		
		AddItem("GorkaEJacket_Summer", 					17,		17,		1,		250,	0 );
		AddItem("GorkaEJacket_Flat", 					17,		17,		1,		250,	0 );
		AddItem("GorkaEJacket_Autumn", 					17,		17,		1,		250,	0 );
		AddItem("GorkaEJacket_PautRev", 				17,		17,		1,		250,	0 );
		
		AddItem("RidersJacket_Black", 					2,		4,		1,		250,	0 );
		
		AddItem("WoolCoat_Black", 						2,		4,		1,		250,	0 );
		AddItem("WoolCoat_Red", 						2,		4,		1,		250,	0 );
		AddItem("WoolCoat_Blue", 						2,		4,		1,		250,	0 );
		AddItem("WoolCoat_Green", 						2,		4,		1,		250,	0 );
		AddItem("WoolCoat_Beige", 						2,		4,		1,		250,	0 );
		AddItem("WoolCoat_RedCheck", 					2,		4,		1,		250,	0 );
		AddItem("WoolCoat_BlackCheck", 					2,		4,		1,		250,	0 );
		AddItem("WoolCoat_BlueCheck", 					2,		4,		1,		250,	0 );
		AddItem("WoolCoat_GreyCheck", 					2,		4,		1,		250,	0 );
		AddItem("WoolCoat_BrownCheck", 					2,		4,		1,		250,	0 );
		
		AddItem("TrackSuitJacket_Black", 				2,		4,		1,		250,	0 );
		AddItem("TrackSuitJacket_Blue", 				2,		4,		1,		250,	0 );
		AddItem("TrackSuitJacket_Green", 				2,		4,		1,		250,	0 );
		AddItem("TrackSuitJacket_LightBlue", 			2,		4,		1,		250,	0 );
		AddItem("TrackSuitJacket_Red", 					2,		4,		1,		250,	0 );
		
		AddItem("PoliceJacket", 						4,		6,		1,		250,	0 );
		AddItem("PoliceJacketOrel", 					4,		6,		1,		250,	0 );
		
		AddItem("ParamedicJacket_Blue", 				4,		6,		1,		250,	0 );
		AddItem("ParamedicJacket_Crimson", 				4,		6,		1,		250,	0 );
		AddItem("ParamedicJacket_Green", 				4,		6,		1,		250,	0 );
		
		AddItem("FirefighterJacket_Beige", 				3,		5,		1,		250,	0 );
		AddItem("FirefighterJacket_Black", 				3,		5,		1,		250,	0 );
	
		AddItem("PrisonUniformJacket", 					3,		5,		1,		250,	0 );
		
		AddItem("QuiltedJacket_Black", 					3,		5,		1,		250,	0 );
		AddItem("QuiltedJacket_Green", 					3,		5,		1,		250,	0 );
		AddItem("QuiltedJacket_Blue", 					3,		5,		1,		250,	0 );
		AddItem("QuiltedJacket_Red", 					3,		5,		1,		250,	0 );
		AddItem("QuiltedJacket_Grey", 					3,		5,		1,		250,	0 );
		AddItem("QuiltedJacket_Orange", 				3,		5,		1,		250,	0 );
		AddItem("QuiltedJacket_Yellow", 				3,		5,		1,		250,	0 );
		AddItem("QuiltedJacket_Violet", 				3,		5,		1,		250,	0 );
		
		AddItem("BomberJacket_Black", 					3,		5,		1,		250,	0 );
		AddItem("BomberJacket_Brown", 					3,		5,		1,		250,	0 );
		AddItem("BomberJacket_Blue", 					3,		5,		1,		250,	0 );
		AddItem("BomberJacket_Grey", 					3,		5,		1,		250,	0 );
		AddItem("BomberJacket_Maroon", 					3,		5,		1,		250,	0 );
		AddItem("BomberJacket_Olive", 					3,		5,		1,		250,	0 );
		AddItem("BomberJacket_SkyBlue", 				3,		5,		1,		250,	0 );

		AddItem("HuntingJacket_Autumn", 				12,		15,		1,		250,	0 );
		AddItem("HuntingJacket_Brown", 					12,		15,		1,		250,	0 );
		AddItem("HuntingJacket_Spring", 				12,		15,		1,		250,	0 );
		AddItem("HuntingJacket_Summer", 				12,		15,		1,		250,	0 );
		AddItem("HuntingJacket_Winter", 				12,		15,		1,		250,	0 );
		
		AddItem("LabCoat", 								1,		3,		1,		250,	0 );
		
		AddItem("USMCJacket_Desert", 					14,		16,		1,		250,	0 );
		AddItem("USMCJacket_Woodland", 					14,		16,		1,		250,	0 );
		
		AddItem("NBCJacketGray", 						25,		25,		1,		250,	0 );
		
		AddItem("DenimJacket", 							2,		2,		1,		250,	0 );
		
		AddItem("JumpsuitJacket_Blue", 					3,		5,		1,		250,	0 );
		AddItem("JumpsuitJacket_Gray", 					3,		5,		1,		250,	0 );
		AddItem("JumpsuitJacket_Green", 				3,		5,		1,		250,	0 );
		AddItem("JumpsuitJacket_Red", 					3,		5,		1,		250,	0 );
		
		AddItem("BDUJacket", 							10,		12,		1,		250,	0 );
	}
};