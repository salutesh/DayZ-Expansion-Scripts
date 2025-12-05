/**
 * Expansion_Kar98_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Kar98_Base: BoltActionRifle_InnerMagazine_Base
{	
	override bool CanEnterIronsights()
	{
		ItemOptics optic = GetAttachedOptics();
		if (optic && PUScopeOptic.Cast(optic))
			return true;
		return super.CanEnterIronsights();
	}
	override RecoilBase SpawnRecoilObject()
	{
		return new MosinRecoil(this);
	}	
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.GetInventory().CreateInInventory( "ExpansionKar98ScopeOptic" );
			entity.GetInventory().CreateInInventory( "Expansion_Kar98_Bayonet" );
		}
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		if (attachment && attachment.IsInherited(ItemOptics) && IsInitialized())
		{
			//! Don't allow to attach to deprecated Kar98Optics slot
			if (slotId == InventorySlots.GetSlotIdFromString("ExpansionKar98Optics"))
				return false;
		}

		return true;
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (slot_name == "ExpansionKar98Optics")
			Expansion_HandleKar98OpticsSlot(item, InventoryMode.LOCAL);
	}

	//! Handle deprecated Kar98Optics slot (move to MosinOptics slot or drop)
	void Expansion_HandleKar98OpticsSlot(EntityAI optic, InventoryMode mode)
	{
		EXError.Info(optic, "Moving to weaponOpticsMosin slot on " + this);

		InventoryLocation src = new InventoryLocation();

		if (optic.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation();

			dst.SetAttachment(this, optic, InventorySlots.GetSlotIdFromString("weaponOpticsMosin"));

			if (!GetInventory().TakeToDst(mode, src, dst))
			{
				EXError.Warn(optic, "Couldn't add to slot weaponOpticsMosin, dropping");

				vector mat[4];
				Math3D.MatrixIdentity4(mat);

				EntityAI owner = GetHierarchyRoot();

				if (!GameInventory.PrepareDropEntityPos(owner, optic, mat, false, GameConstants.INVENTORY_ENTITY_DROP_OVERLAP_DEPTH))
				{
					EXError.Error(optic, "Couldn't prepare drop position, owner = " + owner);
					return;
				}

				dst.SetGround(optic, mat);

				if (!GetInventory().TakeToDst(mode, src, dst))
				{
					EXError.Error(optic, "Couldn't drop at " + mat[3].ToString());
					return;
				}
			}
		}
		else
		{
			EXError.Warn(optic, "Couldn't get current inventory location");
		}
	}
};