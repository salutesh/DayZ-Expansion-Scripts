/**
 * ExpansionMarketContainers.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketContainers: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 16;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_CONTAINERS";
		m_FileName = "Containers";

		AddItem("smallprotectorcase", 	3405, 	5670,	1,100);
		AddItem("ammobox", 	34450, 	57420,	1,100);
		AddItem("BarrelHoles_Blue", 			34450, 	57420,		1,		100, null, {"BarrelHoles_Green", "BarrelHoles_Red", "BarrelHoles_Yellow"});
		AddItem("barrel_blue", 	4500, 	7500,	1,100,null,{"Barrel_Green","Barrel_Red","Barrel_Yellow"});
		AddItem("seachest", 	5425, 	9040,	1,100);
		AddItem("WoodenCrate", 					2000,		4000,		1,		100);
	
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionToolBox", 			3405, 	5670,		1,		100);
		AddItem("ExpansionSafeMini", 			5425, 	9040,		1,		100);
		AddItem("ExpansionSafeMedium", 			6000,		12000,		1,		100);
		AddItem("ExpansionSafeLarge", 			8000,		16000,		1,		100);
	#endif
	}
};