/**
 * PluginRecipesManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PluginRecipesManager
{	
	override void RegisterRecipies()
	{
		super.RegisterRecipies();
		
		//! Basebuilding
		RegisterRecipe(new ExpansionCraftFloorKit);
		RegisterRecipe(new ExpansionCraftStairKit);
		RegisterRecipe(new ExpansionCraftRampKit);
		RegisterRecipe(new ExpansionCraftWallKit);
		//RegisterRecipe(new ExpansionCraftPillarKit);

		//RegisterRecipe(new ExpansionCraftLumber_0_5);
		//RegisterRecipe(new ExpansionCraftLumber_1);
		//RegisterRecipe(new ExpansionCraftLumber_1_5);
		RegisterRecipe(new ExpansionCraftLumber_3);
		
		RegisterRecipe(new ExpansionCraftFlagKit);
		
		//RegisterRecipe(new ExpansionCraftChickenBreaderKit);		//! NOT YET READY
		
		//! Sets
		RegisterRecipe(new ExpansionCraftHeroSet);
		RegisterRecipe(new ExpansionCraftBanditSet);

		//! Consumables
		RegisterRecipe(new ExpansionCraftMilkBottle);
	}
}