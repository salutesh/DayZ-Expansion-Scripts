/**
 * ExpansionTransferInventory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static TransferInventoryResult ExpansionTransferInventory(EntityAI sourceItem, EntityAI targetItem, bool dropUsingPhysics = false)
{
	TransferInventoryResult result = TransferInventoryResult.Ok;

	array<EntityAI> children = new array<EntityAI>;
	sourceItem.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);
	for (int i = 0; i < children.Count(); i++)
	{
		auto child = children[i];
		if (!child)
			continue;

		InventoryLocation child_src = new InventoryLocation;
		child.GetInventory().GetCurrentInventoryLocation(child_src);

		InventoryLocation child_dst = new InventoryLocation;
		if (child_src.GetType() != InventoryLocationType.HANDS)
		{
			child_dst.Copy(child_src);
			child_dst.SetParent(targetItem);
		}

		float health;

		if (GetGame().IsServer())
		{
			health = child.GetHealth();
			child.SetHealth(child.GetMaxHealth()); //! Set item to max health, so we can move ruined items
		}

		if (child_src.GetType() != InventoryLocationType.HANDS && GameInventory.LocationCanAddEntity(child_dst))
		{
			if (GetGame().IsMultiplayer())
				sourceItem.GetInventory().TakeToDst(InventoryMode.SERVER, child_src, child_dst);
			else
				sourceItem.GetInventory().TakeToDst(InventoryMode.LOCAL, child_src, child_dst);
		}
		else
		{
			if (dropUsingPhysics)
			{
				vector mat[4];
				child.GetTransform(mat);
				child_dst.SetGround(child, mat);

				if (GetGame().IsMultiplayer())
					sourceItem.GetInventory().TakeToDst(InventoryMode.SERVER, child_src, child_dst);
				else
					sourceItem.GetInventory().TakeToDst(InventoryMode.LOCAL, child_src, child_dst);

				child.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
				child.EnableDynamicCCD(true);
				child.SetDynamicPhysicsLifeTime(20);
			}
			else
			{
				if (GetGame().IsMultiplayer())
					sourceItem.GetInventory().DropEntity(InventoryMode.SERVER, sourceItem, child);
				else
					sourceItem.GetInventory().DropEntity(InventoryMode.LOCAL, sourceItem, child);
			}

			result = TransferInventoryResult.DroppedSome;
		}

		if (GetGame().IsServer())
			child.SetHealth(health);
	}
	return result;
}

static void Expansion_PhysicsDropAttachments(EntityAI parent)
{
	if (!GetGame().IsServer())
		return;

	InventoryMode invMode = InventoryMode.SERVER;
	if (!GetGame().IsMultiplayer())
		invMode = InventoryMode.LOCAL;

	CarScript car;
	bool exploded = Class.CastTo(car, parent) && car.IsExploded();
#ifdef EXPANSIONMODVEHICLE
	ExpansionVehicleBase vehicle;
	exploded |= Class.CastTo(vehicle, parent) && vehicle.IsExploded();
#endif

	//! Popping attachments off changes attachment count, so do this in reverse order
	for (int i = parent.GetInventory().AttachmentCount() - 1; i >= 0; i--)
	{
		EntityAI entity = parent.GetInventory().GetAttachmentFromIndex(i);

		vector transform[4];
		entity.GetTransform(transform);

		ItemBase ib;
		Class.CastTo(ib, entity);

		vector velocity = dBodyGetVelocityAt(parent, transform[3]);
		float mass = dBodyGetMass(entity);
		if (!mass && ib)
			mass = ib.GetWeight() / 1000;
		vector force = velocity * mass;

		//! If parent is exploded vehicle, make parts fly off violently
		if (exploded)
		{
			if (!force.Length())
				force = entity.GetDirection(); //! If vehicle is standing still, use direction vector instead of velocity
			force = force.Normalized() * 500;
		}

		if (ib)
		{
			InventoryLocation src = new InventoryLocation;
			ib.GetInventory().GetCurrentInventoryLocation(src);
			InventoryLocation dst = new InventoryLocation;
			dst.SetGround(ib, transform);

			if (ib.GetInventory().TakeToDst(invMode, src, dst))
			{
				ib.ThrowPhysically(null, force);
			}
		}
		else
		{
			if (entity.GetInventory().DropEntity(invMode, entity.GetHierarchyRoot(), entity))
			{
				dBodyApplyImpulse(entity, force);
			}
		}
	}
}
