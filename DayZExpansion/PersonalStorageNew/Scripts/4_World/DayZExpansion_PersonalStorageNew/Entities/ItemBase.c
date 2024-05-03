/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
/*
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);

		if (GetGame().IsServer() && oldLoc.GetType() != InventoryLocationType.UNKNOWN)
		{
			ExpansionPersonalStorageContainer container;
			if (Class.CastTo(container, newLoc.GetParent()) && container.Expansion_IsExcludedFromPS(newLoc.GetItem()))
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_UndoMoveToPS, container, newLoc.GetItem());
		}
	}

	void Expansion_UndoMoveToPS(notnull ExpansionPersonalStorageContainer container, notnull EntityAI item)
	{
		EntityAI owner;

		PlayerBase player = container.ExpansionGetContainerOwner();

		if (player)
			owner = player;
		else
			owner = container;

		bool moved;

		if (GetGame().IsMultiplayer())
			moved = owner.ServerTakeEntityToInventory(FindInventoryLocationType.ANY, item);
		else
			moved = owner.LocalTakeEntityToInventory(FindInventoryLocationType.ANY, item);

		if (!moved)
		{
			if (GetGame().IsMultiplayer())
				moved = owner.GetInventory().DropEntity(InventoryMode.SERVER, owner, item);
			else
				moved = owner.GetInventory().DropEntity(InventoryMode.LOCAL, owner, item);

			ItemBase itemBase;
			if (moved && Class.CastTo(itemBase, item))
				itemBase.Expansion_PlaceOnSurfaceProper(owner.GetPosition());
		}

		if (player)
			container.Expansion_SendExcludedFromPSNotification(item, player.GetIdentity());
	}
*/

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		//! If this is not a personal storage container but along the hierarchy of a direct attachment to one,
		//! only allow item into cargo if personal storage container itself allows attachment cargo
		if (!IsInherited(ExpansionPersonalStorageContainer))
		{
			ExpansionPersonalStorageContainer container;
			if (Class.CastTo(container, GetHierarchyRoot()) && !container.m_Expansion_PersonalStorageAllowAttachmentCargo)
			{
				EntityAI parent = this;
				while (parent.GetHierarchyParent() && !parent.GetHierarchyParent().IsInherited(ExpansionPersonalStorageContainer))
				{
					parent = parent.GetHierarchyParent();
				}

				if (parent.GetInventory().IsAttachment())
					return false;
			}
		}

		return true;
	}
}
