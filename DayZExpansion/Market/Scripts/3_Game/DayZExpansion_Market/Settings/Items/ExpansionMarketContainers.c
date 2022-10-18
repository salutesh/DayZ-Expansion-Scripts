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

		AddItem("SmallProtectorCase", 			70,			140,		1,		100);
		AddItem("AmmoBox", 						80,			160,		1,		100);
		AddItem("BarrelHoles_Blue", 			800,		1600,		1,		100, null, {"BarrelHoles_Green", "BarrelHoles_Red", "BarrelHoles_Yellow"});
		AddItem("Barrel_Blue", 					1500,		3000,		1,		100, null, {"Barrel_Green", "Barrel_Red", "Barrel_Yellow"});
		AddItem("SeaChest", 					2000,		4000,		1,		100);
		AddItem("WoodenCrate", 					2000,		4000,		1,		100);
	
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionToolBox", 			500,		1000,		1,		100);
		AddItem("ExpansionSafeMini", 			4000,		8000,		1,		100);
		AddItem("ExpansionSafeMedium", 			6000,		12000,		1,		100);
		AddItem("ExpansionSafeLarge", 			8000,		16000,		1,		100);
	#endif
	}
};