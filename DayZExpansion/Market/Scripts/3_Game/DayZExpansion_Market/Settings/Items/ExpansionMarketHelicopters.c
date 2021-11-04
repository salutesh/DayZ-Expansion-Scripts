/**
 * ExpansionMarketHelicopters.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

		CategoryID = 29;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_HELICOPTER";
		m_FileName = "Helicopters";

	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionGyrocopter", 			500000,			1000000,		1,		10,	{"ExpansionHelicopterBattery","SparkPlug"});
		AddItem("ExpansionMh6", 				1500000,		3000000,		1,		10,	{"ExpansionHelicopterBattery", "ExpansionIgniterPlug", "ExpansionHydraulicHoses",  "HeadlightH7", "Expansion_Mh6_Door_1_1", "Expansion_Mh6_Door_1_2", "Expansion_Mh6_Door_2_1", "Expansion_Mh6_Door_2_2"});
		AddItem("ExpansionUh1h", 				1700000,		3400000,		1,		10, 	{"ExpansionHelicopterBattery", "ExpansionIgniterPlug", "ExpansionHydraulicHoses", "HeadlightH7", "ExpansionUh1hDoor_1_1", "ExpansionUh1hDoor_1_2"});
		AddItem("ExpansionMerlin", 				2000000,		4000000,		1,		10, 	{"ExpansionHelicopterBattery", "ExpansionIgniterPlug", "ExpansionHydraulicHoses", "HeadlightH7", "ExpansionMerlinFrontWheel", "ExpansionMerlinFrontWheel", "ExpansionMerlinBackWheel", "ExpansionMerlinBackWheel"});
	#endif		
	}	
};