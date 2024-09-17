/**
 * ExpansionMarketEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketEvent: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 60;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_EVENT";
		m_FileName = "Event";

		AddItem("easteregg", 	305, 	510,	1,250);
		AddItem("crookednose", 	1080, 	1800,	1,250);
		AddItem("cauldron", 	1080, 	1800,	1,250);
		AddItem("witchhat", 	1080, 	1800,	1,250);
		AddItem("witchhood_black", 	1080, 	1800,	1,250,null,{"WitchHood_Brown","WitchHood_Red"});
		
		AddItem("paydaymask_dallas", 	145, 	240,	1,250,null,{"PaydayMask_Wolf","PaydayMask_Hoxton","PaydayMask_Chains"});
		
		AddItem("candycane_green", 	305, 	510,	1,250,null,{"candycane_red","candycane_redgreen","candycane_yellow"});

		AddItem("christmasheadband_antlers", 	360, 	600,	1,250,null,{"christmasheadband_gingerbread","christmasheadband_trees"});

		AddItem("pumpkinhelmet", 	1425, 	2370,	1,250);
		AddItem("santasbeard", 	255, 	425,	1,250);
		AddItem("santashat", 	1485, 	2470,	1,250);

		AddItem("anniversarybox", 	395, 	660,	1,100);
		AddItem("anniversary_fireworkslauncher", 	200, 	335,	1,100);
		AddItem("giftbox_large_1", 	280, 	465,	1,100,null,{"GiftBox_Large_2","GiftBox_Large_3","GiftBox_Large_4","GiftBox_Medium_1","GiftBox_Medium_2","GiftBox_Medium_3","GiftBox_Medium_4","GiftBox_Small_1","GiftBox_Small_2","GiftBox_Small_3","GiftBox_Small_4"});
	}
};