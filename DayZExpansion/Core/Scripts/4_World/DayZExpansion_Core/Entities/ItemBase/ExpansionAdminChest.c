/**
 * ExpansionAdminChest.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAdminChest: SeaChest
{
	void ExpansionAdminChest()
	{
		if (g_Game.IsServer())
			SetAllowDamage(false);
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		return Expansion_HasPermission();
	}

	override bool CanDisplayCargo()
	{
		if (!super.CanDisplayCargo())
			return false;

		return Expansion_HasPermission();
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		if (!super.CanPutInCargo(parent))
			return false;

		return g_Game.IsDedicatedServer() || Expansion_HasPermission();
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		if (!super.CanPutIntoHands(parent))
			return false;

		return g_Game.IsDedicatedServer() || Expansion_HasPermission();
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		return g_Game.IsDedicatedServer() || Expansion_HasPermission();
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		return g_Game.IsDedicatedServer() || Expansion_HasPermission();
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
			return false;

		return g_Game.IsDedicatedServer() || Expansion_HasPermission();
	}

	override bool CanReleaseCargo(EntityAI cargo)
	{
		if (!super.CanReleaseCargo(cargo))
			return false;

		return g_Game.IsDedicatedServer() || Expansion_HasPermission();
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		if (!super.CanSwapItemInCargo(child_entity, new_entity))
			return false;

		return g_Game.IsDedicatedServer() || Expansion_HasPermission();
	}

	override bool IsInventoryVisible()
	{
		return Expansion_HasPermission();
	}

	bool Expansion_HasPermission()
	{
		return GetPermissionsManager().HasPermission("Entity.Spawn");
	}
}
