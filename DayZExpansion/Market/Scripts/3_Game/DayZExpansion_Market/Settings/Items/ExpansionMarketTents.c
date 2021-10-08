/**
 * ExpansionMarketTents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTents: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();
		
		CategoryID = 54;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_TENTS";
		m_FileName = "Tents";
	
		AddItem("PartyTent", 						2600,	5200,	1,		100,	0, null, {"PartyTent_Blue", "PartyTent_Brown", "PartyTent_Lunapark"});
		/*AddItem("PartyTent_Blue", 					2600,	5200,	1,		100,	0);
		AddItem("PartyTent_Brown", 					2600,	5200,	1,		100,	0);
		AddItem("PartyTent_Lunapark", 				2600,	5200,	1,		100,	0);*/
		AddItem("MediumTent", 						3000,	6000,	1,		100,	0, null, {"MediumTent_Orange", "MediumTent_Green"});
		/*AddItem("MediumTent_Orange", 				3000,	6000,	1,		100,	0);
		AddItem("MediumTent_Green", 				3000,	6000,	1,		100,	0);*/
		AddItem("LargeTent", 						6000,	12000,	1,		100,	0);
		AddItem("CarTent", 							7000,	14000,	1,		100,	0);

	#ifdef EXPANSIONMOD
		AddItem("ExpansionCamoTentKit",				2600,	5200,	1,		100,	0);
		AddItem("ExpansionCamoBoxKit",				2600,	5200,	1,		100,	0);
	#endif
	}
};