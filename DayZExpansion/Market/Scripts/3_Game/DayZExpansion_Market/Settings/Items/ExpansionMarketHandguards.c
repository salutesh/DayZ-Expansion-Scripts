/**
 * ExpansionMarketHandguards.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHandguards: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 27;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_HANDGUARDS";
		m_FileName = "Handguards";
			
		AddItem("MP5_PlasticHndgrd", 			100,		200,		1,		100);
		AddItem("MP5_RailHndgrd", 				150,		300,		1,		100);

		AddItem("AK74_Hndgrd", 					150,		300,		1,		100);

		AddItem("AK_WoodHndgrd", 				200,		400,		1,		100);
		AddItem("AK_RailHndgrd", 				200,		400,		1,		100);
		AddItem("AK_PlasticHndgrd", 			200,		400,		1,		100);
		
		AddItem("M4_PlasticHndgrd", 			150,		300,		1,		100);
		AddItem("M4_RISHndgrd", 				200,		400,		1,		100);
		AddItem("M4_MPHndgrd", 					200,		400,		1,		100);
	}
};