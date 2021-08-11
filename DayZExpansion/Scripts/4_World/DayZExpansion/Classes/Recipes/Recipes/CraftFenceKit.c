/**
 * CraftFenceKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		CraftFenceKit
 * @brief		Removing craft of vanilla fence	
 **/
modded class CraftFenceKit
{	
	override bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		bool canCraft = GetExpansionSettings().GetBaseBuilding().CanCraftVanillaBasebuilding;

		if ( canCraft )
		{
			return super.CanDo( ingredients, player );
		}
		else 
		{
			return false;
		}

		return super.CanDo( ingredients, player );
	}
}