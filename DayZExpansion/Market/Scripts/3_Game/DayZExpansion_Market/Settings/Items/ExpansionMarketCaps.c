/**
 * ExpansionMarketCaps.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCaps: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 13;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_CAPS";
		m_FileName = "Caps";
		
		AddItem("BaseballCap_CMMG_Pink", 			12,		24,		1,		100, null, {"BaseballCap_Pink", "BaseballCap_Blue", "BaseballCap_Beige", "BaseballCap_Red", "BaseballCap_CMMG_Black", "BaseballCap_Black", "BaseballCap_Olive", "BaseballCap_Camo"});
		/*AddItem("BaseballCap_Pink", 				12,		24,		1,		100);
		AddItem("BaseballCap_Blue", 				12,		24,		1,		100);
		AddItem("BaseballCap_Beige", 				12,		24,		1,		100);
		AddItem("BaseballCap_Red", 					12,		24,		1,		100);
		AddItem("BaseballCap_CMMG_Black", 			12,		24,		1,		100);*/
		AddItem("BaseballCap_Black", 				13,		26,		1,		100);
		AddItem("BaseballCap_Olive", 				14,		28,		1,		100);
		AddItem("BaseballCap_Camo", 				16,		32,		1,		100);
		
		AddItem("PrisonerCap", 						16,		32,		1,		100);

		AddItem("PilotkaCap", 						25,		50,		1,		100);

		AddItem("PoliceCap", 						25,		50,		1,		100);

		AddItem("FlatCap_Blue", 					32,		64,		1,		100, null, {"FlatCap_Red", "FlatCap_Brown", "FlatCap_Grey", "FlatCap_BrownCheck", "FlatCap_GreyCheck", "FlatCap_Black", "FlatCap_BlackCheck"});
		/*AddItem("FlatCap_Red", 					32,		64,		1,		100);
		AddItem("FlatCap_Brown", 					32,		64,		1,		100);
		AddItem("FlatCap_Grey", 					32,		64,		1,		100);
		AddItem("FlatCap_BrownCheck", 				32,		64,		1,		100);
		AddItem("FlatCap_GreyCheck", 				32,		64,		1,		100);
		AddItem("FlatCap_Black", 					32,		64,		1,		100);
		AddItem("FlatCap_BlackCheck", 				32,		64,		1,		100);*/

		AddItem("ZmijovkaCap_Blue", 				36,		72,		1,		100, null, {"ZmijovkaCap_Brown", "ZmijovkaCap_Red", "ZmijovkaCap_Black", "ZmijovkaCap_Green"});
		/*AddItem("ZmijovkaCap_Brown",				36,		72,		1,		100);
		AddItem("ZmijovkaCap_Red", 					36,		72,		1,		100);*/
		AddItem("ZmijovkaCap_Black", 				37,		74,		1,		100);
		AddItem("ZmijovkaCap_Green", 				38,		76,		1,		100);

		AddItem("RadarCap_Blue", 					40,		80,		1,		100, null, {"RadarCap_Brown", "RadarCap_Red", "RadarCap_Black", "RadarCap_Green"});
		/*AddItem("RadarCap_Brown", 					40,		80,		1,		100);
		AddItem("RadarCap_Red", 					40,		80,		1,		100);*/
		AddItem("RadarCap_Black", 					42,		84,		1,		100);
		AddItem("RadarCap_Green",					45,		90,		1,		100);
	}
};