/**
 * ExpansionMarketButtstocks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketButtstocks: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 12;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BUTTSTOCKS";
		m_FileName = "Buttstocks";
		
		AddItem("MP5k_StockBttstck", 			100,	200,	1,		100,	0);

		AddItem("Fal_OeBttstck", 				100,	200,	1,		100,	0);
		AddItem("Fal_FoldingBttstck", 			150,	300,	1,		100,	0);

		AddItem("Saiga_Bttstck", 				150,	300,	1,		100,	0);

		AddItem("AKS74U_Bttstck", 				150,	300,	1,		100,	0);

		AddItem("AK74_WoodBttstck", 			150,	300,	1,		100,	0);

		AddItem("AK_PlasticBttstck", 			100,	200,	1,		100,	0);
		AddItem("AK_WoodBttstck", 				150,	300,	1,		100,	0);
		AddItem("AK_FoldingBttstck", 			250,	500,	1,		100,	0);

		AddItem("M4_OEBttstck", 				150,	300,	1,		100,	0);
		AddItem("M4_MPBttstck", 				150,	300,	1,		100,	0);
		AddItem("M4_CQBBttstck", 				150,	300,	1,		100,	0);
			
		AddItem("GhillieAtt_Tan", 				220,	440,	1,		100,	0, null, {"GhillieAtt_Woodland", "GhillieAtt_Mossy"});
		//AddItem("GhillieAtt_Woodland", 			220,	440,	1,		100,	0);
		//AddItem("GhillieAtt_Mossy", 			220,	440,	1,		100,	0);
	}
};