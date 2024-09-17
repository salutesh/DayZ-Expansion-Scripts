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

class ExpansionMarketVests: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 58;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_VESTS";
		m_FileName = "Vests";

		AddItem("reflexvest", 	520, 	870,	1,100);
		
		AddItem("policevest", 	660, 	1100,	1,100);
		
		AddItem("pressvest_blue", 	2525, 	4205,	1,100,null,{"PressVest_LightBlue"});
		AddItem("pressvest_lightblue", 	2525, 	4205,	1,100);
		
		AddItem("ukassvest_black", 	1025, 	1710,	1,100,null,{"UKAssVest_Khaki","UKAssVest_Olive","UKAssVest_Camo","ukassvest_winter"});
		AddItem("ukassvest_camo", 	1060, 	1770,	1,100);
		AddItem("ukassvest_winter", 	1080, 	1800,	1,100);

		AddItem("smershvest", 	5490, 	9150,	1,100);
			
		AddItem("highcapacityvest_black", 	7105, 	11845,	1,100,null,{"HighCapacityVest_Olive"});
		AddItem("highcapacityvest_olive", 	7830, 	13055,	1,100);
		
		AddItem("platecarriervest", 	7230, 	12045,	1,100,null,{"PlateCarrierVest_Green","PlateCarrierVest_Black","PlateCarrierVest_Camo","platecarriervest_winter"});
		AddItem("platecarriervest_camo", 	7105, 	11845,	1,100);
		AddItem("platecarriervest_winter", 	1080, 	1800,	1,100);
		AddItem("huntingvest", 	645, 	1075,	1,100,null,{"huntingvest_winter"});
		
		AddItem("chestplate", 	355, 	590,	1,100);
	}
};