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
    override bool CanPutInCargo( EntityAI parent )
    {
        return CanPutInCargoClothingConditions( parent );
    }
    
    override bool CanReceiveItemIntoCargo( EntityAI item )
    {
        return CanReceiveItemIntoCargoClothingConditions( item );
    }
    
    override bool CanLoadItemIntoCargo(EntityAI item)
    {
        return CanLoadItemIntoCargoClothingConditions( item );
    }

    override bool CanPutInCargoClothingConditions( EntityAI parent )
    {
        if ( parent.IsInherited( UndergroundStash ) )
            return super.CanPutInCargo( parent );

        if ( GetNumberOfItems() != 0 )
            return false;

        return true;
    }
    
    override bool CanReceiveItemIntoCargoClothingConditions( EntityAI item )
    {
        return !GetInventory().IsInCargo();
    }
    
	//! @note this will also be checked when splitting item
    override bool CanLoadItemIntoCargoClothingConditions( EntityAI item )
    {
		EntityAI parent = GetHierarchyParent();

		if ( !parent || parent.IsInherited( UndergroundStash ) )
			return true;

        return !GetInventory().IsInCargo();
    }
};
