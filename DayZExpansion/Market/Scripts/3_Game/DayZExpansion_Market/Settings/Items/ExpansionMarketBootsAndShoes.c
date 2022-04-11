/**
 * ExpansionMarketBootsAndShoes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBootsAndShoes: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 11;
		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_BOOTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_SHOES");
		m_FileName = "Boots_And_Shoes";

		AddItem("AthleticShoes_Blue", 				17,		34,		1,		100, null, {"AthleticShoes_Brown", "AthleticShoes_Grey", "AthleticShoes_Black", "AthleticShoes_Green"});
		/*AddItem("AthleticShoes_Brown", 			20,		40,		1,		100);
		AddItem("AthleticShoes_Grey", 				20,		40,		1,		100);*/
		AddItem("AthleticShoes_Black", 				23,		46,		1,		100);
		AddItem("AthleticShoes_Green", 				23,		46,		1,		100);
		
		AddItem("JoggingShoes_Black", 				30,		60,		1,		100, null, {"JoggingShoes_Blue", "JoggingShoes_Red", "JoggingShoes_Violet", "JoggingShoes_White"});
		/*AddItem("JoggingShoes_Blue", 				30,		60,		1,		100);
		AddItem("JoggingShoes_Red", 				30,		60,		1,		100);
		AddItem("JoggingShoes_Violet", 				30,		60,		1,		100);
		AddItem("JoggingShoes_White", 				30,		60,		1,		100);*/

		AddItem("Sneakers_Green", 					30,		60,		1,		100, null, {"Sneakers_Red", "Sneakers_White", "Sneakers_Black", "Sneakers_Gray"});
		/*AddItem("Sneakers_Red", 					30,		60,		1,		100);
		AddItem("Sneakers_White", 					30,		60,		1,		100);*/
		AddItem("Sneakers_Black", 					31,		62,		1,		100);
		AddItem("Sneakers_Gray", 					32,		64,		1,		100);
		
		AddItem("Ballerinas_Blue", 					32,		64,		1,		100, null, {"Ballerinas_Red", "Ballerinas_White", "Ballerinas_Yellow"});
		AddItem("Ballerinas_Red", 					32,		64,		1,		100);
		AddItem("Ballerinas_White", 				32,		64,		1,		100);
		AddItem("Ballerinas_Yellow", 				32,		64,		1,		100);	

		AddItem("DressShoes_White", 				32,		64,		1,		100, null, {"DressShoes_Beige", "DressShoes_Black", "DressShoes_Brown", "DressShoes_Sunburst"});
		/*AddItem("DressShoes_Beige", 				35,		70,		1,		100);
		AddItem("DressShoes_Black", 				35,		70,		1,		100);
		AddItem("DressShoes_Brown", 				35,		70,		1,		100);
		AddItem("DressShoes_Sunburst", 				35,		70,		1,		100);*/

		AddItem("HikingBootsLow_Blue", 				45,		90,		1,		100, null, {"HikingBootsLow_Grey", "HikingBootsLow_Beige", "HikingBootsLow_Black"});
		//AddItem("HikingBootsLow_Grey", 			45,		90,		1,		100);
		AddItem("HikingBootsLow_Beige", 			48,		96,		1,		100);
		AddItem("HikingBootsLow_Black", 			50,		100,	1,		100);
		
		AddItem("WorkingBoots_Yellow", 				55,		110,	1,		100, null, {"WorkingBoots_Grey", "WorkingBoots_Brown", "WorkingBoots_Beige", "WorkingBoots_Green"});
		AddItem("WorkingBoots_Grey", 				60,		120,	1,		100);
		AddItem("WorkingBoots_Brown", 				60,		120,	1,		100);
		AddItem("WorkingBoots_Beige", 				60,		120,	1,		100);
		AddItem("WorkingBoots_Green", 				65,		130,	1,		100);

		AddItem("HikingBoots_Brown", 				75,		150,	1,		100, null, {"HikingBoots_Black"});
		//AddItem("HikingBoots_Black", 				75,		150,	1,		100);
	
		AddItem("CombatBoots_Beige", 				80,		160,	1,		100, null, {"CombatBoots_Black", "CombatBoots_Brown", "CombatBoots_Green", "CombatBoots_Grey"});
		/*AddItem("CombatBoots_Black", 				80,		160,	1,		100);
		AddItem("CombatBoots_Brown", 				80,		160,	1,		100);
		AddItem("CombatBoots_Green", 				80,		160,	1,		100);
		AddItem("CombatBoots_Grey", 				80,		160,	1,		100);*/

		AddItem("JungleBoots_Beige", 				85,		170,	1,		100, null, {"JungleBoots_Black", "JungleBoots_Brown", "JungleBoots_Green", "JungleBoots_Olive"});
		/*AddItem("JungleBoots_Black", 				85,		170,	1,		100);
		AddItem("JungleBoots_Brown", 				85,		170,	1,		100);
		AddItem("JungleBoots_Green", 				85,		170,	1,		100);
		AddItem("JungleBoots_Olive", 				85,		170,	1,		100);*/

		AddItem("Wellies_Black", 					85,		170,	1,		100, null, {"Wellies_Brown", "Wellies_Grey", "Wellies_Green"});
		/*AddItem("Wellies_Brown", 					85,		170,	1,		100);
		AddItem("Wellies_Grey", 					85,		170,	1,		100);*/
		AddItem("Wellies_Green", 					90,		180,	1,		100);

		AddItem("TTSKOBoots", 						100,	200,	1,		100);

		AddItem("MilitaryBoots_Redpunk", 			100,	200,	1,		100, null, {"MilitaryBoots_Bluerock", "MilitaryBoots_Beige", "MilitaryBoots_Black", "MilitaryBoots_Brown"});
		AddItem("MilitaryBoots_Bluerock", 			105,	210,	1,		100);
		AddItem("MilitaryBoots_Beige", 				110,	220,	1,		100);
		AddItem("MilitaryBoots_Black", 				110,	220,	1,		100);
		AddItem("MilitaryBoots_Brown", 				110,	220,	1,		100);
			
		AddItem("NBCBootsGray", 					150,	300,	1,		100);
		AddItem("NBCBootsYellow", 					150,	300,	1,		100);
	}
};