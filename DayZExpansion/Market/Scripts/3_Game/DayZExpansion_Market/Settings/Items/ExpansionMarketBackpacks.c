/**
 * ExpansionMarketBackpacks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBackpacks: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 5;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_BACKPACKS";
		m_FileName = "Backpacks";
		
		AddItem("ChildBag_Red", 					20,			40,			1,		100, null, {"ChildBag_Blue", "ChildBag_Green"});
		//AddItem("ChildBag_Blue", 					20,			40,			1,		100);
		AddItem("ChildBag_Green", 					25,			50,			1,		100);
		
		AddItem("DryBag_Orange", 					30,			60,			1,		100, null, {"DryBag_Yellow", "DryBag_Blue", "DryBag_Black", "DryBag_Red", "DryBag_Green"});
		/*AddItem("DryBag_Yellow", 					30,			60,			1,		100);
		AddItem("DryBag_Blue", 						30,			60,			1,		100);
		AddItem("DryBag_Black", 					30,			60,			1,		100);
		AddItem("DryBag_Red", 						30,			60,			1,		100);*/
		AddItem("DryBag_Green", 					35,			70,			1,		100);
		
		AddItem("TaloonBag_Blue", 					50,			100,		1,		100, null, {"TaloonBag_Orange", "TaloonBag_Violet", "TaloonBag_Green"});
		/*AddItem("TaloonBag_Orange", 				50,			100,		1,		100);
		AddItem("TaloonBag_Violet", 				50,			100,		1,		100);*/
		AddItem("TaloonBag_Green", 					70,			140,		1,		100);
	
		AddItem("SmershBag", 						100,		200,		1,		100);
		
		AddItem("AssaultBag_Black", 				110,		220,		1,		100, null, {"AssaultBag_Green", "AssaultBag_Ttsko"});
		//AddItem("AssaultBag_Green", 				110,		220,		1,		100);
		AddItem("AssaultBag_Ttsko", 				130,		260,		1,		100);
		
		AddItem("HuntingBag", 						130,		260,		1,		100);
	
	#ifdef EXPANSIONMOD	
		AddItem("TortillaBag", 						180,		360,		1,		100, null, {"ExpansionDesertTortilla"});
	#else
		AddItem("TortillaBag", 						180,		360,		1,		100);
	#endif	
	
	#ifdef EXPANSIONMOD
		AddItem("CoyoteBag_Brown", 					180,		360,		1,		100, null, {"CoyoteBag_Green", "ExpansionCoyoteBlack"});
		/*AddItem("CoyoteBag_Green", 				180,		360,		1,		100);
		AddItem("ExpansionCoyoteBlack", 			180,		360,		1,		100);*/
	#else
		AddItem("CoyoteBag_Brown", 					180,		360,		1,		100, null, {"CoyoteBag_Green"});
	#endif	

		AddItem("MountainBag_Red", 					300,		600,		1,		100, null, {"MountainBag_Blue", "MountainBag_Orange", "MountainBag_Green"});
		/*AddItem("MountainBag_Blue", 				300,		600,		1,		100);
		AddItem("MountainBag_Orange", 				300,		600,		1,		100);
		AddItem("MountainBag_Green", 				300,		600,		1,		100);*/
		
		AddItem("AliceBag_Green", 					400,		800,		1,		100, null, {"AliceBag_Black", "AliceBag_Camo"});
		//AddItem("AliceBag_Black", 				400,		800,		1,		100);
		AddItem("AliceBag_Camo", 					500,		1000,		1,		100);
	}
};