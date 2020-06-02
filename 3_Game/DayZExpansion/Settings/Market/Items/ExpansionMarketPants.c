/**
 * ExpansionMarketPants.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketPants: ExpansionMarketCategory
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

		CategoryID = 45;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_PANTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_SHORTS");
		m_FileName = "Pants_And_Shorts";

		AddItem("Jeans_Black", 						5,		7,		1,		250,	0 );
		AddItem("Jeans_Blue", 						5,		7,		1,		250,	0 );
		AddItem("Jeans_Brown", 						5,		7,		1,		250,	0 );
		AddItem("Jeans_Green", 						5,		7,		1,		250,	0 );
		AddItem("Jeans_Grey", 						5,		7,		1,		250,	0 );
		AddItem("Jeans_BlueDark", 					5,		7,		1,		250,	0 );
		
		AddItem("CargoPants_Beige", 				4,		6,		1,		250,	0 );
		AddItem("CargoPants_Black", 				4,		6,		1,		250,	0 );
		AddItem("CargoPants_Blue", 					4,		6,		1,		250,	0 );
		AddItem("CargoPants_Green", 				4,		6,		1,		250,	0 );
		AddItem("CargoPants_Grey", 					4,		6,		1,		250,	0 );
		
		AddItem("TTSKOPants", 						6,		8,		1,		250,	0 );
		
		AddItem("HunterPants_Autumn", 				10,		12,		1,		250,	0 );
		AddItem("HunterPants_Brown", 				10,		12,		1,		250,	0 );
		AddItem("HunterPants_Spring", 				10,		12,		1,		250,	0 );
		AddItem("HunterPants_Summer", 				10,		12,		1,		250,	0 );
		AddItem("HunterPants_Winter", 				10,		12,		1,		250,	0 );
	
		AddItem("CanvasPants_Beige", 				3,		5,		1,		250,	0 );
		AddItem("CanvasPants_Blue", 				3,		5,		1,		250,	0 );
		AddItem("CanvasPants_Grey", 				3,		5,		1,		250,	0 );
		AddItem("CanvasPants_Red", 					3,		5,		1,		250,	0 );
		AddItem("CanvasPants_Violet", 				3,		5,		1,		250,	0 );
		AddItem("CanvasPantsMidi_Beige", 			3,		5,		1,		250,	0 );
		AddItem("CanvasPantsMidi_Blue", 			3,		5,		1,		250,	0 );
		AddItem("CanvasPantsMidi_Grey", 			3,		5,		1,		250,	0 );
		AddItem("CanvasPantsMidi_Red", 				3,		5,		1,		250,	0 );
		AddItem("CanvasPantsMidi_Violet", 			3,		5,		1,		250,	0 );
	
		AddItem("TrackSuitPants_Black", 			2,		4,		1,		250,	0 );
		AddItem("TrackSuitPants_Blue", 				2,		4,		1,		250,	0 );
		AddItem("TrackSuitPants_Green", 			2,		4,		1,		250,	0 );
		AddItem("TrackSuitPants_Red", 				2,		4,		1,		250,	0 );
		AddItem("TrackSuitPants_LightBlue", 		2,		4,		1,		250,	0 );
	
		AddItem("GorkaPants_Summer", 				13,		15,		1,		250,	0 );
		AddItem("GorkaPants_Autumn", 				13,		15,		1,		250,	0 );
		AddItem("GorkaPants_Flat", 					13,		15,		1,		250,	0 );
		AddItem("GorkaPants_PautRev", 				13,		15,		1,		250,	0 );
	
		AddItem("PolicePants", 						3,		5,		1,		250,	0 );
		AddItem("PolicePantsOrel", 					7,		9,		1,		250,	0 );
	
		AddItem("ParamedicPants_Blue", 				4,		6,		1,		250,	0 );
		AddItem("ParamedicPants_Crimson", 			4,		6,		1,		250,	0 );
		AddItem("ParamedicPants_Green", 			4,		6,		1,		250,	0 );
		
		AddItem("FirefightersPants_Beige", 			4,		6,		1,		250,	0 );
		AddItem("FirefightersPants_Black", 			4,		6,		1,		250,	0 );
	
		AddItem("PrisonUniformPants", 				2,		4,		1,		250,	0 );
		
		AddItem("MedicalScrubsPants_Blue",			1,		2,		1,		250,	0 );
		AddItem("MedicalScrubsPants_Green", 		1,		2,		1,		250,	0 );
		AddItem("MedicalScrubsPants_White", 		1,		2,		1,		250,	0 );
		
		AddItem("USMCPants_Desert", 				7,		9,		1,		250,	0 );
		AddItem("USMCPants_Woodland", 				7,		9,		1,		250,	0 );
		
		AddItem("SlacksPants_Beige", 				3,		5,		1,		250,	0 );
		AddItem("SlacksPants_Black", 				3,		5,		1,		250,	0 );
		AddItem("SlacksPants_Blue", 				3,		5,		1,		250,	0 );
		AddItem("SlacksPants_Brown", 				3,		5,		1,		250,	0 );
		AddItem("SlacksPants_DarkGrey", 			3,		5,		1,		250,	0 );
		AddItem("SlacksPants_Khaki", 				3,		5,		1,		250,	0 );
		AddItem("SlacksPants_LightGrey", 			3,		5,		1,		250,	0 );
		AddItem("SlacksPants_White", 				3,		5,		1,		250,	0 );
		
		AddItem("BDUPants", 						7,		9,		1,		250,	0 );
		
		AddItem("NBCPantsGray", 					20,		22,		1,		250,	0 );
		
		AddItem("Breeches_Pink", 					2,		4,		1,		250,	0 );
		AddItem("Breeches_Red", 					2,		4,		1,		250,	0 );
		AddItem("Breeches_White", 					2,		4,		1,		250,	0 );
		AddItem("Breeches_Green", 					2,		4,		1,		250,	0 );
		AddItem("Breeches_Browncheck", 				2,		4,		1,		250,	0 );
		AddItem("Breeches_Blue", 					2,		4,		1,		250,	0 );
		AddItem("Breeches_Blackcheck", 				2,		4,		1,		250,	0 );
		AddItem("Breeches_Black", 					2,		4,		1,		250,	0 );
		AddItem("Breeches_Beige", 					2,		4,		1,		250,	0 );
		AddItem("Breeches_Beetcheck", 				2,		4,		1,		250,	0 );
		
		AddItem("ShortJeans_Black", 				4,		6,		1,		250,	0 );
		AddItem("ShortJeans_Blue", 					4,		6,		1,		250,	0 );
		AddItem("ShortJeans_Brown", 				4,		6,		1,		250,	0 );
		AddItem("ShortJeans_Darkblue", 				4,		6,		1,		250,	0 );
		AddItem("ShortJeans_Green", 				4,		6,		1,		250,	0 );
		AddItem("ShortJeans_Red", 					4,		6,		1,		250,	0 );
		
		AddItem("JumpsuitPants_Blue", 				3,		5,		1,		250,	0 );
		AddItem("JumpsuitPants_Green", 				3,		5,		1,		250,	0 );
		AddItem("JumpsuitPants_Grey", 				3,		5,		1,		250,	0 );
		AddItem("JumpsuitPants_Red", 				3,		5,		1,		250,	0 );
	}
}