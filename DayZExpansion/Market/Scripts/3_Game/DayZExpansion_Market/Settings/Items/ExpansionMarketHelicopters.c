/**
 * ExpansionMarketHelicopters.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketHelicopters: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_HELICOPTER";
		m_FileName = "Helicopters";

	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionGyrocopter", 			500000,			1000000,		1,		10,	{"ExpansionHelicopterBattery","SparkPlug"});
		AddItem("ExpansionBigGyrocopter", 		1000000,	    2000000,		1,		10,	{"ExpansionHelicopterBattery","SparkPlug"});
		AddItem("ExpansionMh6", 				1500000,		3000000,		1,		10,	{"ExpansionHelicopterBattery", "ExpansionIgniterPlug", "ExpansionHydraulicHoses",  "HeadlightH7", "Expansion_Mh6_Door_1_1", "Expansion_Mh6_Door_1_2", "Expansion_Mh6_Door_2_1", "Expansion_Mh6_Door_2_2"}, {"ExpansionMh6_Green", "ExpansionMh6_Blueline", "ExpansionMh6_Digital", "ExpansionMh6_Elliptical", "ExpansionMh6_Furious", "ExpansionMh6_GrayWatcher", "ExpansionMh6_Jeans", "ExpansionMh6_Resistance", "ExpansionMh6_Shadow", "ExpansionMh6_Sheriff", "ExpansionMh6_Speedy", "ExpansionMh6_Sunset", "ExpansionMh6_Vrana", "ExpansionMh6_Wasp", "ExpansionMh6_Wave", "ExpansionMh6_Black"});
		AddItem("ExpansionUh1h", 				1700000,		3400000,		1,		10, 	{"ExpansionHelicopterBattery", "ExpansionIgniterPlug", "ExpansionHydraulicHoses", "HeadlightH7", "ExpansionUh1hDoor_1_1", "ExpansionUh1hDoor_1_2", "ExpansionUh1hDoor_2_1", "ExpansionUh1hDoor_2_2"}, {"ExpansionUh1h_Green", "ExpansionUh1h_Civilian", "ExpansionUh1h_Medical", "ExpansionUh1h_Uber", "ExpansionUh1h_Resistance"});
		AddItem("ExpansionMerlin", 				2000000,		4000000,		1,		10, 	{"ExpansionHelicopterBattery", "ExpansionIgniterPlug", "ExpansionHydraulicHoses", "HeadlightH7", "ExpansionMerlinFrontWheel", "ExpansionMerlinFrontWheel", "ExpansionMerlinBackWheel", "ExpansionMerlinBackWheel"}, {"ExpansionMerlin_Green", "ExpansionMerlin_Dahoman", "ExpansionMerlin_Ion", "ExpansionMerlin_President", "ExpansionMerlin_Vrana", "ExpansionMerlin_Wave"});
	#endif		
	}	
};