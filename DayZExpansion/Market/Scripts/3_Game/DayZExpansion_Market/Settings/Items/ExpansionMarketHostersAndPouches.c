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

		AddItem("chestholster", 	1075, 	1795,	1,100);
		
		AddItem("platecarrierholster", 	1035, 	1725,	1,100,null,{"PlateCarrierHolster_Green","PlateCarrierHolster_Black","PlateCarrierHolster_Camo","platecarrierholster_winter"});
		AddItem("platecarrierholster_camo", 	7105, 	11845,	1,100);
		AddItem("platecarrierholster_winter", 	1080, 	1800,	1,100);

		AddItem("platecarrierpouches", 	1035, 	1725,	1,100,null,{"PlateCarrierPouches_Green","PlateCarrierPouches_Black","PlateCarrierPouches_Camo","platecarrierpouches_winter"});
		AddItem("platecarrierpouches_camo", 	7105, 	11845,	1,100);
		AddItem("platecarrierpouches_winter", 	1190, 	1980,	1,100);

		AddItem("nylonknifesheath", 	515, 	855,	1,100);
	}
};