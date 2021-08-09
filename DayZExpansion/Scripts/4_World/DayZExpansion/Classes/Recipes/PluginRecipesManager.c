/**
 * PluginRecipesManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PluginRecipesManager
{	
	// ------------------------------------------------------------
	// PluginRecipesManager RegisterRecipies
	// ------------------------------------------------------------
	override void RegisterRecipies()
	{
		super.RegisterRecipies();
		
		//! Basebuilding
		RegisterRecipe(new ExpansionCraftFloorKit);
		RegisterRecipe(new ExpansionCraftStairKit);
		RegisterRecipe(new ExpansionCraftRampKit);
		RegisterRecipe(new ExpansionCraftWallKit);
		//RegisterRecipe(new ExpansionCraftPillarKit);				//! NOT USED YET

		//RegisterRecipe(new ExpansionCraftLumber_0_5);				//! NOT USED YET
		RegisterRecipe(new ExpansionCraftLumber_1);				
		RegisterRecipe(new ExpansionCraftLumber_1_5);				
		RegisterRecipe(new ExpansionCraftLumber_3);
			
		RegisterRecipe(new ExpansionCraftBarbedWireKit);
		RegisterRecipe(new ExpansionCraftExpansionHescoKit);
		RegisterRecipe(new ExpansionCraftExpansionHelipadKit);
		
		//RegisterRecipe(new ExpansionCraftChickenBreaderKit);		//! NOT YET READY
		
		//! Sets
		//RegisterRecipe(new ExpansionCraftHeroSet);				//! NOT USED YET
		//RegisterRecipe(new ExpansionCraftBanditSet);				//! NOT USED YET

		//! Consumables
		RegisterRecipe(new ExpansionCraftMilkBottle);

		//! Ammunitions
		RegisterRecipe(new ExpansionCraftStickySmoke);
	}
};