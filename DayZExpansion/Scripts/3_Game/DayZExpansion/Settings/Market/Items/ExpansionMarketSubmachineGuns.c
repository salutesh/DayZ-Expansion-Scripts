/**
 * ExpansionMarketSubmachineGuns.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSubmachineGuns: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 53;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SUBMACHINE_GUNS";
		m_FileName = "Submachine_Guns";

		// Submachine Guns
		AddItem("CZ61", 				70,		90,		1,		250,	0 );
		AddItem("UMP45", 				80,		100,	1,		250,	0 );
		AddItem("MP5K", 				100,	120,	1,		250,	0 );
		
		AddItem("Expansion_Kedr", 		150,	170,	1,		250,	0 );
		AddItem("Expansion_MPX", 		200,	220,	1,		250,	0 );
		AddItem("Expansion_MP5", 		200,	220,	1,		250,	0 );
		AddItem("Expansion_MP7", 		200,	220,	1,		250,	0 );
		AddItem("Expansion_G36", 		200,	220,	1,		250,	0 );
		AddItem("Expansion_VityazSN", 	200,	220,	1,		250,	0 );
	}
};