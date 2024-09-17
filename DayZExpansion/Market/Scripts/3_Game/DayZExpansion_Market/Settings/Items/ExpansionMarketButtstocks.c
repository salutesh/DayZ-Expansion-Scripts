/**
 * ExpansionMarketButtstocks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		
		AddItem("mp5k_stockbttstck", 	1290, 	2150,	1,100);

		AddItem("fal_oebttstck", 	1705, 	2840,	1,100);
		AddItem("fal_foldingbttstck", 	1705, 	2840,	1,100);

		AddItem("saiga_bttstck", 	1280, 	2130,	1,100);

		AddItem("aks74u_bttstck", 	5950, 	9915,	1,100);

		AddItem("ak74_woodbttstck", 	1280, 	2130,	1,100);

		AddItem("ak_plasticbttstck", 	1280, 	2130,	1,100);
		AddItem("ak_woodbttstck", 	1280, 	2130,	1,100);
		AddItem("ak_foldingbttstck", 	1280, 	2130,	1,100);

		AddItem("m4_oebttstck", 	1280, 	2130,	1,100);
		AddItem("m4_mpbttstck", 	1280, 	2130,	1,100);
		AddItem("m4_cqbbttstck", 	1280, 	2130,	1,100);

		AddItem("pp19_bttstck", 	1290, 	2150,	1,100);
			
		AddItem("GhillieAtt_Tan", 				2630,	4390,	1,		100, null, {"GhillieAtt_Woodland", "ghillieatt_mossy","ghillieatt_winter"});
	}
};