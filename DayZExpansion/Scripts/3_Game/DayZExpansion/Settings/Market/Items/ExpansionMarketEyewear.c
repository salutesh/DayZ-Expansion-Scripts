/**
 * ExpansionMarketEyewear.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketEyewear: ExpansionMarketCategory
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

		CategoryID = 21;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_EYEWEAR";
		m_FileName = "Eyewear";

		AddItem("SportGlasses_Black", 				2,		4,		1,		250,	0 );
		AddItem("SportGlasses_Blue", 				2,		4,		1,		250,	0 );
		AddItem("SportGlasses_Green", 				2,		4,		1,		250,	0 );
		AddItem("SportGlasses_Orange", 				2,		4,		1,		250,	0 );
		AddItem("AviatorGlasses", 					4,		6,		1,		250,	0 );
		AddItem("DesignerGlasses", 					3,		5,		1,		250,	0 );
		AddItem("ThickFramesGlasses", 				2,		4,		1,		250,	0 );
		AddItem("ThinFramesGlasses", 				2,		4,		1,		250,	0 );
		AddItem("TacticalGoggles", 					6,		8,		1,		250,	0 );
		AddItem("NVGHeadstrap", 					20,		22,		1,		250,	0 );
		
		AddItem("ExpansionMilGoggles", 				8,		10,		1,		250,	0 );
	}
}