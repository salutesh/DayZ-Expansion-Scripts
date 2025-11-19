/**
 * ExpansionCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionCodeLock
{
	override void ExpansionUnlock()
	{
		super.ExpansionUnlock();

		if (g_Game.IsServer() && GetInventory().IsAttachment())
		{
			ItemBase parent;
			if (Class.CastTo(parent, GetHierarchyParent()) && parent.Expansion_CanUseVirtualStorage(true))
				parent.Expansion_RestoreContents();
		}
	}

	override void ExpansionLock()
	{
		if (g_Game.IsServer() && GetInventory().IsAttachment())
		{
			ItemBase parent;
			if (Class.CastTo(parent, GetHierarchyParent()) && parent.Expansion_CanUseVirtualStorage())
				parent.Expansion_StoreContents();
		}

		super.ExpansionLock();
	}
}
