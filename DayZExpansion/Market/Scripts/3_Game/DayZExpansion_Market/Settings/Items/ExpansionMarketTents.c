/**
 * ExpansionMarketTents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	
		AddItem("partytent", 	3060, 	5105,	1,100,null,{"PartyTent_Blue","PartyTent_Brown","PartyTent_Lunapark"});
		AddItem("partytent_blue", 	3060, 	5105,	1,100);
		AddItem("partytent_brown", 	3060, 	5105,	1,100);
		AddItem("partytent_lunapark", 	3830, 	6380,	1,100);

		AddItem("mediumtent", 	2355, 	3925,	1,100,null,{"MediumTent_Orange","MediumTent_Green"});
		AddItem("mediumtent_orange", 	2355, 	3925,	1,100);
		AddItem("mediumtent_green", 	3015, 	5025,	1,100);
		
		AddItem("largetent", 	11485, 	19140,	1,100);
		AddItem("cartent", 	5550, 	9250,	1,100);

	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionCamoTentKit",				2600,	5200,	1,		100);
		AddItem("ExpansionCamoBoxKit",				2600,	5200,	1,		100);
	#endif
	}
};