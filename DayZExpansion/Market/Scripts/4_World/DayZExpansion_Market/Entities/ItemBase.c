/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_VANILLA_EEITEMLOCATION_PB_FIX
modded class ItemBase
{
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		EntityAI oldParent = oldLoc.GetParent();
		EntityAI newParent = newLoc.GetParent();

		Man oldOwner;
		Man newOwner;

		if (oldParent)
			oldOwner = oldParent.GetHierarchyRootPlayer();
		
		if (newParent)
			newOwner = newParent.GetHierarchyRootPlayer();

		//! Vanilla ItemBase::EEItemLocationChanged wants PlayerBase class,
		//! NPCs are not PlayerBase so this is to prevent the super method from being called and causing NULL ptrs
		if ((!oldOwner || oldOwner.IsInherited(PlayerBase)) && (!newOwner || newOwner.IsInherited(PlayerBase)))
			super.EEItemLocationChanged(oldLoc, newLoc);
	}
}
#endif
