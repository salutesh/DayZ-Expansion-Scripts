/**
 * ExpansionMarketTents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTents: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 56;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_TENTS";
		m_FileName = "Tents";

		AddItem("ExpansionCamoTent",				100,	120,	1,		250,	0 );
		AddItem("MediumTent", 						300,	320,	1,		250,	0 );
		AddItem("MediumTent_Green", 				300,	320,	1,		250,	0 );
		AddItem("MediumTent_Orange", 				300,	320,	1,		250,	0 );
		AddItem("CarTent", 							500,	520,	1,		250,	0 );
		AddItem("LargeTent", 						700,	720,	1,		250,	0 );
		AddItem("PartyTent", 						400,	450,	1,		250,	0 );
		AddItem("PartyTent_Blue", 					400,	450,	1,		250,	0 );
		AddItem("PartyTent_Brown", 					400,	450,	1,		250,	0 );
		AddItem("PartyTent_Lunapark", 				400,	450,	1,		250,	0 );
	}
};