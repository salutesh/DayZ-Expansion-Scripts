/**
 * ExpansionMarketVegetables.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketVegetables: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 58;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_VEGETABLES";
		m_FileName = "Vegetables";

		AddItem("Apple", 						2,		4,		1,		250,	0 );
		AddItem("GreenBellPepper", 				2,		4,		1,		250,	0 );
		AddItem("Zucchini", 					5,		7,		1,		250,	0 );
		AddItem("Pumpkin", 						10,		12,		1,		250,	0 );
		AddItem("SlicedPumpkin", 				2,		4,		1,		250,	0 );
		AddItem("Potato", 						2,		4,		1,		250,	0 );
		AddItem("Tomato", 						2,		4,		1,		250,	0 );
		AddItem("SambucusBerry", 				2,		4,		1,		250,	0 );
		AddItem("CaninaBerry", 					2,		4,		1,		250,	0 );
		AddItem("Plum", 						2,		4,		1,		250,	0 );
		AddItem("Pear", 						2,		4,		1,		250,	0 );
				
		AddItem("AgaricusMushroom", 			2,		4,		1,		250,	0 );
		AddItem("AmanitaMushroom", 				2,		4,		1,		250,	0 );
		AddItem("MacrolepiotaMushroom", 		2,		4,		1,		250,	0 );
		AddItem("LactariusMushroom", 			2,		4,		1,		250,	0 );
		AddItem("PsilocybeMushroom", 			2,		4,		1,		250,	0 );
		AddItem("AuriculariaMushroom", 			2,		4,		1,		250,	0 );
		AddItem("BoletusMushroom", 				2,		4,		1,		250,	0 );
		AddItem("PleurotusMushroom", 			2,		4,		1,		250,	0 );
	}
}

