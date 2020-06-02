/**
 * ExpansionMarketMedical.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMedical: ExpansionMarketCategory
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

		CategoryID = 40;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MEDICALS";
		m_FileName = "Medical";

		AddItem("AntiPestsSpray", 				8,		10,		1,		250,	0 );
		AddItem("DisinfectantSpray", 			8,		10,		1,		250,	0 );
		AddItem("DisinfectantAlcohol", 			4,		6,		1,		250,	0 );
		AddItem("PurificationTablets", 			5,		7,		1,		250,	0 );
		AddItem("CharcoalTablets", 				2,		4,		1,		250,	0 );
		AddItem("PainkillerTablets", 			1,		3,		1,		250,	0 );
		AddItem("VitaminBottle", 				6,		8,		1,		250,	0 );
		AddItem("TetracyclineAntibiotics", 		8,		10,		1,		250,	0 );
		AddItem("Rag", 							1,		3,		1,		250,	0 );
		AddItem("BandageDressing", 				2,		4,		1,		250,	0 );
		AddItem("BloodTestKit", 				5,		7,		1,		250,	0 );
		AddItem("InjectionVial", 				5,		7,		1,		250,	0 );
		AddItem("Thermometer", 					5,		7,		1,		250,	0 );
		AddItem("Syringe", 						8,		10,		1,		250,	0 );
		AddItem("StartKitIV", 					1,		3,		1,		250,	0 );
		AddItem("SalineBag", 					2,		4,		1,		250,	0 );
		AddItem("BloodBagEmpty", 				2,		4,		1,		250,	0 );
		AddItem("FirstAidKit", 					3,		5,		1,		250,	0 );
		AddItem("Defibrillator", 				4,		6,		1,		250,	0 );
		AddItem("Epinephrine", 					2,		4,		1,		250,	0 );
		AddItem("Morphine", 					3,		5,		1,		250,	0 );
	}
}