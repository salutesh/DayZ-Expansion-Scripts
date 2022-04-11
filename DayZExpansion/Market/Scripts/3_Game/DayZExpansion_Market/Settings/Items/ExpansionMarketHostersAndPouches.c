/**
 * ExpansionMarketVests.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHostersAndPouches: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 62;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_HOLSTERS";
		m_FileName = "Holsters_And_Pouches";

		AddItem("ChestHolster", 				60,			120,		1,		100);	
		AddItem("PlateCarrierHolster", 			60,			120,		1,		100);
		AddItem("PlateCarrierPouches", 			100,		200,		1,		100);
		AddItem("NylonKnifeSheath", 			30,			60,		1,		100);
	}
};