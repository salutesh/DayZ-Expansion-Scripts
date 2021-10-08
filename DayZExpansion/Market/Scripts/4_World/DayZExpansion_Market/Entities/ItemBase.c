/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	}
}
