/**
 * ExpansionMarketSupplies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSupplies: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 54;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SUPPLIES";
		m_FileName = "Supplies";
		
		// Supplies
		AddItem("Paper", 						1,			3,			1,		250,	0 );
		AddItem("Pen_Black", 					1,			3,			1,		250,	0 );
		AddItem("Pen_Blue", 					1,			3,			1,		250,	0 );
		AddItem("Pen_Green", 					1,			3,			1,		250,	0 );
		AddItem("Pen_Red",	 					1,			3,			1,		250,	0 );
		AddItem("Rope", 						1,			3,			1,		250,	0 );
		AddItem("TannedLeather", 				1,			3,			1,		250,	0 );
		AddItem("MetalWire", 					25,			27,			1,		250,	0 );
		AddItem("EpoxyPutty", 					1,			3,			1,		250,	0 );
		AddItem("DuctTape", 					1,			3,			1,		250,	0 );
		AddItem("XmasLights", 					1,			3,			1,		250,	0 );
		AddItem("HandcuffKeys", 				10,			12,			1,		250,	0 );
		AddItem("Handcuffs", 					50,			52,			1,		250,	0 );
		AddItem("Netting", 						25,			27,			1,		250,	0 );
		AddItem("BurlapSack", 					50,			52,			1,		250,	0 );
		AddItem("WoodenPlank", 					50,			52,			1,		250,	0 );
		AddItem("MetalPlate", 					200,		220,		1,		250,	0 );
		AddItem("SmallGasCanister", 			50,			52,			1,		250,	0 );
		AddItem("MediumGasCanister", 			70,			72,			1,		250,	0 );
		AddItem("LargeGasCanister", 			100,		100,		1,		250,	0 );
		AddItem("NailBox", 						70,			72,			1,		250,	0 );
		AddItem("Nail", 						1,			3,			1,		250,	0 );
		AddItem("BarbedWire", 					10,			12,			1,		250,	0 );
		AddItem("Camonet", 						250,		270,		1,		250,	0 );
		AddItem("HescoBox", 					200,		220,		1,		250,	0 );
		AddItem("PowerGenerator", 				500,		520,		1,		250,	0 );
	}
}