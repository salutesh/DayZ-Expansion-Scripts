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
		
		AddItem("baseballcap_cmmg_pink", 	250, 	420,	1,100,null,{"BaseballCap_Pink","BaseballCap_Blue","BaseballCap_Beige","BaseballCap_Red","BaseballCap_CMMG_Black","BaseballCap_Black","BaseballCap_Olive","BaseballCap_Camo"});
		AddItem("baseballcap_pink", 	250, 	420,	1,100);
		AddItem("baseballcap_blue", 	265, 	440,	1,100);
		AddItem("baseballcap_beige", 	265, 	440,	1,100);
		AddItem("baseballcap_cmmg_black", 	265, 	440,	1,100);
		AddItem("baseballcap_black", 	265, 	440,	1,100);
		AddItem("baseballcap_camo", 	610, 	1020,	1,100);
		
		AddItem("prisonercap", 	355, 	590,	1,100);

		AddItem("pilotkacap", 	5490, 	9150,	1,100);

		AddItem("policecap", 	690, 	1150,	1,100);

		AddItem("flatcap_blue", 	275, 	455,	1,100,null,{"FlatCap_Red","FlatCap_Brown","FlatCap_Grey","FlatCap_BrownCheck","FlatCap_GreyCheck","FlatCap_Black","FlatCap_BlackCheck"});

		AddItem("zmijovkacap_blue", 	1450, 	2420,	1,100,null,{"ZmijovkaCap_Brown","ZmijovkaCap_Red","ZmijovkaCap_Black","ZmijovkaCap_Green"});
		AddItem("zmijovkacap_black", 	2630, 	4385,	1,100);
		AddItem("zmijovkacap_green", 	3355, 	5595,	1,100);

		AddItem("radarcap_blue", 	375, 	625,	1,100,null,{"RadarCap_Brown","RadarCap_Red","RadarCap_Black","RadarCap_Green"});
		AddItem("radarcap_brown", 	275, 	455,	1,100);
		AddItem("radarcap_red", 	275, 	455,	1,100);
		AddItem("radarcap_green", 	275, 	455,	1,100);
	}
};