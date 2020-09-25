/**
 * ExpansionMarketComponents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTools: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 57;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_TOOLS";
		m_FileName = "Tools";
		
		// Tools
		AddItem("Screwdriver", 					1,		2,		1,		250,	0 );
		AddItem("Wrench", 						1,		2,		1,		250,	0 );
		AddItem("Pliers", 						1,		2,		1,		250,	0 );
		AddItem("Whetstone", 					1,		2,		1,		250,	0 );
		AddItem("Hammer", 						1,		2,		1,		250,	0 );
		AddItem("Hacksaw", 						1,		2,		1,		250,	0 );
		AddItem("HandSaw", 						1,		2,		1,		250,	0 );
		AddItem("CanOpener", 					1,		2,		1,		250,	0 );
		AddItem("Hatchet", 						1,		2,		1,		250,	0 );
		AddItem("Lockpick", 					2,	 	3,		1,		250,	0 );
		AddItem("LugWrench", 					1,	 	3,		1,		250,	0 );
		AddItem("Crowbar", 						1,	 	3,		1,		250,	0 );
		AddItem("Shovel", 						2,	 	3,		1,		250,	0 );
		AddItem("Pickaxe", 						2,	 	3,		1,		250,	0 );
		AddItem("SledgeHammer", 				2,	 	3,		1,		250,	0 );
		AddItem("WoodAxe", 						2,	 	3,		1,		250,	0 );
		AddItem("FirefighterAxe", 				2,	 	3,		1,		250,	0 );
	}
};