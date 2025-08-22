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

		DisplayName = ("#STR_EXPANSION_MARKET_CATEGORY_BOOTS" + " & " + "#STR_EXPANSION_MARKET_CATEGORY_SHOES");
		m_FileName = "Boots_And_Shoes";

		AddItem("athleticshoes_blue", 	245, 	405,	1,100,null,{"AthleticShoes_Brown","AthleticShoes_Grey","AthleticShoes_Black","AthleticShoes_Green"});
		
		AddItem("joggingshoes_black", 	205, 	340,	1,100,null,{"JoggingShoes_Blue","JoggingShoes_Red","JoggingShoes_Violet","JoggingShoes_White"});

		AddItem("sneakers_green", 	225, 	375,	1,100,null,{"Sneakers_Red","Sneakers_White","Sneakers_Black","Sneakers_Gray"});
		
		/*
		AddItem("Ballerinas_Blue", 					32,		64,		1,		100, null, {"Ballerinas_Red", "Ballerinas_White", "Ballerinas_Yellow"});
		*/

		AddItem("dressshoes_white", 	265, 	440,	1,100,null,{"DressShoes_Beige","DressShoes_Black","DressShoes_Brown","DressShoes_Sunburst"});

		AddItem("hikingbootslow_blue", 	205, 	340,	1,100,null,{"HikingBootsLow_Grey","HikingBootsLow_Beige","HikingBootsLow_Black"});
		
		AddItem("workingboots_yellow", 	255, 	425,	1,100,null,{"WorkingBoots_Grey","WorkingBoots_Brown","WorkingBoots_Beige","WorkingBoots_Green"});

		AddItem("hikingboots_brown", 	205, 	340,	1,100,null,{"HikingBoots_Black"});
	
		AddItem("combatboots_beige", 	1055, 	1755,	1,100,null,{"CombatBoots_Black","CombatBoots_Brown","CombatBoots_Green","CombatBoots_Grey"});

		AddItem("jungleboots_beige", 	1060, 	1770,	1,100,null,{"JungleBoots_Black","JungleBoots_Brown","JungleBoots_Green","JungleBoots_Olive"});

		AddItem("wellies_black", 	280, 	470,	1,100,null,{"Wellies_Brown","Wellies_Grey","Wellies_Green"});

		AddItem("ttskoboots", 	1035, 	1725,	1,100);

		AddItem("militaryboots_redpunk", 	285, 	470,	1,100,null,{"MilitaryBoots_Bluerock","MilitaryBoots_Beige","MilitaryBoots_Black","MilitaryBoots_Brown"});
		AddItem("militaryboots_bluerock", 	280, 	470,	1,100);
		AddItem("militaryboots_beige", 	1055, 	1755,	1,100);
		AddItem("militaryboots_black", 	1055, 	1755,	1,100);
		AddItem("militaryboots_brown", 	1055, 	1755,	1,100);
			
		AddItem("nbcbootsgray", 	7900, 	13165,	1,100,null,{"NBCBootsYellow","NBCBootsWhite"});

	//! 1.21+
		AddItem("medievalboots", 	355, 	590,	1,100);
	}
};