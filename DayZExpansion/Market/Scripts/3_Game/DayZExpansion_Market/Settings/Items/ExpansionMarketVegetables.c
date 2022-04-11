/**
 * ExpansionMarketVegetables.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketVegetables: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 56;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_FRUIT_AND_VEGETABLES";
		m_FileName = "Fruit_And_Vegetables";

		AddItem("Apple", 						7,		9,		1,		100);
		AddItem("GreenBellPepper", 				7,		9,		1,		100);
		AddItem("Zucchini", 					10,		12,		1,		100);
		AddItem("Pumpkin", 						15,		17,		1,		100);
		AddItem("SlicedPumpkin", 				7,		9,		1,		100);
		AddItem("PotatoSeed", 					7,		9,		1,		100);
		AddItem("Potato", 						7,		9,		1,		100);
		AddItem("Tomato", 						7,		9,		1,		100);
		AddItem("SambucusBerry", 				7,		9,		1,		100);
		AddItem("CaninaBerry", 					7,		9,		1,		100);
		AddItem("Plum", 						7,		9,		1,		100);
		AddItem("Pear", 						7,		9,		1,		100);
		//AddItem("Kiwi", 						7,		9,		1,		100);
		//AddItem("Orange", 					7,		9,		1,		100);
		//AddItem("Banana", 					7,		9,		1,		100);
				
		AddItem("AgaricusMushroom", 			7,		9,		1,		100);
		AddItem("AmanitaMushroom", 				7,		9,		1,		100);
		AddItem("MacrolepiotaMushroom", 		7,		9,		1,		100);
		AddItem("LactariusMushroom", 			7,		9,		1,		100);
		AddItem("PsilocybeMushroom", 			7,		9,		1,		100);
		AddItem("AuriculariaMushroom", 			7,		9,		1,		100);
		AddItem("BoletusMushroom", 				7,		9,		1,		100);
		AddItem("PleurotusMushroom", 			7,		9,		1,		100);
	}
};