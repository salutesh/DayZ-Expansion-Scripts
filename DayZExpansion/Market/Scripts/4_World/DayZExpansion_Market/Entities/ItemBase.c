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

modded class ItemBase
{
	//! Fix NULL pointer when giving trader NPCs guns
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		bool callSuper = true;

		if ( newLoc.GetParent() )
		{
			Man man = newLoc.GetParent().GetHierarchyRootPlayer();
			if (man && man.IsInherited(ExpansionTraderNPCBase))
				callSuper = false;
		}

		if (callSuper)
			super.EEItemLocationChanged(oldLoc,newLoc);

	#ifndef EXPANSION_GEQ_1_9_38
		//! Fix bug in Core when you move an item in cargo to an attachment slot or vice versa within the same parent,
		//! it doesn't deduct from/add to cargo count
		//! TODO: Can be removed from here once 1.9.38 is released since it'll be in Core
		if (newLoc.GetParent() == oldLoc.GetParent())
		{
			if (oldLoc.GetType() == InventoryLocationType.CARGO)
			{
				//! Moved within same parent from cargo to att slot
				if (newLoc.GetType() != InventoryLocationType.CARGO)
					Expansion_UpdateParentCargoCount(oldLoc, -1);
			}
			else if (newLoc.GetType() == InventoryLocationType.CARGO)
			{
				//! Moved within same parent from att slot to cargo
				Expansion_UpdateParentCargoCount(oldLoc, 1);
			}
		}
	#endif
	}

#ifndef EXPANSION_GEQ_1_9_38
	//! Fix bug in Core where attachment deleted in vehicle deducts from cargo count
	//! TODO: Can be removed from here once 1.9.38 is released since it'll be in Core
	override void Expansion_UpdateParentCargoCountEx(EntityAI parent, int delta)
	{
		if ((!ToDelete() && !IsPendingDeletion()) || (!parent.ToDelete() && !parent.IsPendingDeletion() && GetInventory().IsInCargo()))
			super.Expansion_UpdateParentCargoCountEx(parent, delta);
	}
#endif
}
