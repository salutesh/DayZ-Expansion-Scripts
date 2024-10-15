modded class WeaponAttachMagazine
{
	override void OnEntry (WeaponEventBase e)
	{
		eAIBase p;
		if (e && Class.CastTo(p, e.m_player))
		{
			Magazine mag = e.m_magazine;

			InventoryLocation newSrc = new InventoryLocation;
			if (mag.GetInventory().GetCurrentInventoryLocation(newSrc))
			{
				eAI_HandleMag(e, newSrc);

			/*
				EntityAI lhandEntity = e.m_player.GetInventory().FindAttachment(InventorySlots.LEFTHAND);
				if (lhandEntity)
				{
					EXError.Warn(this, ExpansionStatic.GetDebugInfo(e.m_player) + "WeaponAttachMagazine, warning - entity in LHand " + ExpansionStatic.GetDebugInfo(lhandEntity) + ", entity to be attached " + ExpansionStatic.DumpToString(newSrc));

					if (GetGame().IsServer())
					{
						vector m4[4];
						Math3D.MatrixIdentity4(m4);
						
						//! We don't care if a valid transform couldn't be found, we just want to preferably use it instead of placing on the player
						GameInventory.PrepareDropEntityPos(e.m_player, lhandEntity, m4, false, GameConstants.INVENTORY_ENTITY_DROP_OVERLAP_DEPTH);
						InventoryLocation ground = new InventoryLocation();
						ground.SetGround(lhandEntity, m4);

						EXPrint(this, "Cloning to ground " + ExpansionStatic.DumpToString(ground));

						if (ExpansionItemSpawnHelper.Clone(lhandEntity, true, ground, true) == null)
							Error("Cloning failed " + ExpansionStatic.GetDebugInfo(lhandEntity) + " " + ExpansionStatic.DumpToString(ground));

						GetGame().ObjectDelete(lhandEntity);  //! Whether cloning succeeded or not, always delete the offending entity
					}
				}
			*/

				// move to LH
				InventoryLocation lhand = new InventoryLocation;
				lhand.SetAttachment(e.m_player, mag, InventorySlots.LEFTHAND);
				if (GameInventory.LocationSyncMoveEntity(newSrc, lhand))
				{
					if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine, ok - new magazine removed from inv (inv->LHand)"); }
				}
				else
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine, error - cannot new remove mag from inv");

				InventoryLocation il = new InventoryLocation;
				il.SetAttachment(m_weapon, mag, InventorySlots.MAGAZINE);
				m_attach.m_newMagazine = mag;
				m_attach.m_newDst = il;
			}
			else
			{
				Error(ExpansionStatic.GetDebugInfo(e.m_player) + " [wpnfsm] " + ExpansionStatic.GetDebugInfo(m_weapon) + " WeaponAttachMagazine, error - cannot get curr location of " + ExpansionStatic.GetDebugInfo(mag) + " " + ExpansionStatic.DumpToString(newSrc));
			}

			super.eAI_Vanilla_OnEntry(e);  //! WeaponStateBase::eAI_Vanilla_OnEntry
			return;
		}
		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}

	//! Like OnAbort, but w/o super call since we only want to deal with any mag stuck in LHand
	void eAI_HandleMag(WeaponEventBase e, InventoryLocation newSrc)
	{
		EntityAI leftHandItem = e.m_player.GetInventory().FindAttachment(InventorySlots.LEFTHAND);

		if (leftHandItem)
			EXError.Warn(this, ExpansionStatic.GetDebugInfo(e.m_player) + "WeaponAttachMagazine, warning - entity in LHand " + ExpansionStatic.GetDebugInfo(leftHandItem) + ", entity to be attached " + ExpansionStatic.DumpToString(newSrc));

		Magazine mag = Magazine.Cast(leftHandItem);
		
		if(mag)
		{
			e.m_player.GetInventory().ClearInventoryReservationEx( mag , null );
			InventoryLocation il = new InventoryLocation;
			e.m_player.GetInventory().FindFreeLocationFor( mag, FindInventoryLocationType.CARGO, il );
		
			if(!il || !il.IsValid())
			{
				if (DayZPlayerUtils.HandleDropMagazine(e.m_player, mag))
				{
					if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine, ok - no inventory space for old magazine - dropped to ground"); }
				}
				else
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine, error - cannot drop magazine from left hand after not found inventory space for old magazine");
				
			}
			else
			{
				InventoryLocation oldSrc = new InventoryLocation;
				mag.GetInventory().GetCurrentInventoryLocation(oldSrc);
				
				if (GameInventory.LocationSyncMoveEntity(oldSrc, il))
				{
					if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine, ok - old magazine removed from wpn (LHand->inv)"); }
				}
				else
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine, error - cannot remove old mag from wpn");
			}
		}
	}
}
