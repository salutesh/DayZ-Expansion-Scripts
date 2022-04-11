/**
 * CraftTerritoryFlagKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CraftTerritoryFlagKit extends RecipeBase
{	
	// ------------------------------------------------------------
	// CraftTerritoryFlagKit CanDo
	// ------------------------------------------------------------	
	override bool CanDo( ItemBase ingredients[], PlayerBase player )
	{	
		if ( !GetExpansionSettings().GetBaseBuilding().CanCraftTerritoryFlagKit )
			return false;
		
		return super.CanDo(ingredients, player);
	}
}
