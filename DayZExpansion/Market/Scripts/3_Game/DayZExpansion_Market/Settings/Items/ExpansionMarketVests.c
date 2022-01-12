/**
 * ExpansionMarketVests.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

		AddItem("ReflexVest", 					5,			10,			1,		100);
		
		AddItem("PoliceVest", 					35,			70,			1,		100);
		
		AddItem("HuntingVest", 					55,			110,		1,		100);
		
		AddItem("PressVest_Blue", 				85,			170,		1,		100, null, {"PressVest_LightBlue"});
		AddItem("PressVest_LightBlue", 			90,			180,		1,		100);
		
		AddItem("UKAssVest_Black", 				90,			180,		1,		100, null, {"UKAssVest_Khaki", "UKAssVest_Olive", "UKAssVest_Camo"});
		/*AddItem("UKAssVest_Khaki", 			90,			180,		1,		100);
		AddItem("UKAssVest_Olive", 				90,			180,		1,		100);*/
		AddItem("UKAssVest_Camo", 				100,		200,		1,		100);

		AddItem("SmershVest", 					150,		300,		1,		100);
			
		AddItem("HighCapacityVest_Black", 		200,		400,		1,		100, null, {"HighCapacityVest_Olive"});
		AddItem("HighCapacityVest_Olive", 		230,		460,		1,		100);
		
		AddItem("PlateCarrierVest", 			250,		500,		1,		100);
		
		AddItem("HuntingVest", 					90,			500,		1,		100);
	}
};