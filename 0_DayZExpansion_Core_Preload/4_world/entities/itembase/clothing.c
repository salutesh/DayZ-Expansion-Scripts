/**
 * Clothing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Clothing
{
	//! Always allow smershbag attachment on smershvest
	//! --------------------------------------------------------------------------------------------------------------

	override bool SmershException(EntityAI hierarchyParent)
	{
		return IsInherited(SmershBag) && hierarchyParent.IsInherited(SmershVest);
	}
    
	//! Allow items to go into cargo of clothing as long as cloting itself is not in cargo
	//! --------------------------------------------------------------------------------------------------------------

	override bool CanReceiveItemIntoCargoClothingConditions( EntityAI item )
	{
		EntityAI hierarchyParent = GetHierarchyParent();
		return !hierarchyParent || !GetInventory().IsInCargo() || SmershException(hierarchyParent);
	}

	//! @note this will also be checked when splitting item
	override bool CanLoadItemIntoCargoClothingConditions( EntityAI item )
	{
		EntityAI parent = GetHierarchyParent();
		
		if ( parent && parent.IsInherited( UndergroundStash ) )
			return true;

		return !parent || !GetInventory().IsInCargo() || SmershException(parent);
	}
};
