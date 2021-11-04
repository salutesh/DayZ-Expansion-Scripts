/**
 * ExpansionMarketComponents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTools: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 55;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_TOOLS";
		m_FileName = "Tools";
		
		AddItem("Screwdriver", 					3,		6,		1,		100);
		AddItem("Wrench", 						3,		6,		1,		100);
		AddItem("Pliers", 						4,		8,		1,		100);
		AddItem("Whetstone", 					5,		10,		1,		100);
		AddItem("Hammer", 						7,		14,		1,		100);
		AddItem("Hacksaw", 						7,		14,		1,		100);
		AddItem("HandSaw", 						7,		14,		1,		100);
		AddItem("CanOpener", 					10,		20,		1,		100);
		AddItem("Hatchet", 						10,		20,		1,		100);
		AddItem("Lockpick", 					20,	 	40,		1,		100);
		
		AddItem("LugWrench", 					4,	 	8,		1,		100);
		AddItem("PipeWrench", 					5,	 	10,		1,		100);
		AddItem("FryingPan", 					5,	 	10,		1,		100);
		AddItem("Sickle", 						5,	 	10,		1,		100);
		AddItem("Crowbar", 						5,	 	10,		1,		100);
		AddItem("Shovel", 						6,	 	12,		1,		100);
		AddItem("Pickaxe", 						10,	 	20,		1,		100);
		AddItem("SledgeHammer", 				12,	 	24,		1,		100);
		AddItem("FarmingHoe", 					12,	 	24,		1,		100);
		AddItem("WoodAxe", 						20,	 	40,		1,		100);
		AddItem("FirefighterAxe", 				30,	 	60,		1,		100);

	#ifdef EXPANSIONMOD
		AddItem("ExpansionPropaneTorch", 		250,	500,	1,		100);
	#endif
	}
};