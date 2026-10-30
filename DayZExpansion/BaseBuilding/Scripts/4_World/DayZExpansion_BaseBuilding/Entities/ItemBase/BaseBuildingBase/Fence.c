/**
 * Fence.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 * Special thanks to DaemonForge for his scripts.
 *
*/

modded class Fence
{
	override bool CanBeDamaged()
	{
		auto settings = GetExpansionSettings().GetRaid();
		switch (settings.BaseBuildingRaidMode)
		{
			case ExpansionBaseBuildingRaidMode.DoorsGates:
			case ExpansionBaseBuildingRaidMode.DoorsGatesWindows:
				return HasHinges() && settings.IsRaidableNow();
			case ExpansionBaseBuildingRaidMode.All:
			case ExpansionBaseBuildingRaidMode.DoorsGatesWindowsWalls:
				return settings.IsRaidableNow();
		}

		return false;
	}

	override void AddAction(typename actionName)
	{
		super.AddAction(actionName);

		if (actionName == ActionOpenFence)
		{
			//! Order matters. Have ExpansionActionEnterCodeLock AFTER ActionOpenFence
			//! so that "Open locked" shows on locked doors/gates without having to cycle through actions in the UI.
			AddAction(ExpansionActionEnterCodeLock);
			AddAction(ExpansionActionChangeCodeLock);
		}

		//! Order matters. Have ActionCloseFence AFTER ExpansionActionEnterCodeLock
		//! so that "Lock" shows on opened doors/gates without having to cycle through actions in the UI.
	}

	override void RemoveAction(typename actionName)
	{
		super.RemoveAction(actionName);

		if (actionName == ActionOpenFence)
		{
			RemoveAction(ExpansionActionEnterCodeLock);
			RemoveAction(ExpansionActionChangeCodeLock);
		}
	}

	bool ExpansionCanAttachCodeLock()
	{
		int attachMode = GetExpansionSettings().GetBaseBuilding(false).CodelockAttachMode;
		return attachMode == ExpansionCodelockAttachMode.ExpansionAndFence || attachMode == ExpansionCodelockAttachMode.ExpansionAndFenceAndTents;
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		string slotName = InventorySlots.GetSlotName(slot_id);
		if (slotName == ATTACHMENT_SLOT_COMBINATION_LOCK)
			return FindAttachmentBySlotName(ATTACHMENT_SLOT_CODE_LOCK) == null;
		else if (slotName == ATTACHMENT_SLOT_CODE_LOCK)
			return ExpansionCanAttachCodeLock() && FindAttachmentBySlotName(ATTACHMENT_SLOT_COMBINATION_LOCK) == null;

		return true;
	}

    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		ItemBase item;
		if (Class.CastTo(item, attachment))
		{
			if (item.Expansion_IsCodeLock() && !ExpansionCanAttachCodeLock())
				return false;

			//! Safety to prevent attaching other locks if a lock is already present (but still allow moving the same lock from one slot to another)
			if (item.Expansion_IsLock())
			{
				EntityAI combo = FindAttachmentBySlotName(ATTACHMENT_SLOT_COMBINATION_LOCK);
				if (combo && item != combo)
					return false;

				EntityAI codeLock = FindAttachmentBySlotName(ATTACHMENT_SLOT_CODE_LOCK);
				if (codeLock && item != codeLock)
					return false;
			}
		}

		return true;
    }

    override void EEItemAttached(EntityAI item, string slot_name)
    {
		super.EEItemAttached(item, slot_name);

		//! move ExpansionCodelock from combolock to codelock slot
		if (item.IsInherited(ExpansionCodeLock) && slot_name == ATTACHMENT_SLOT_COMBINATION_LOCK)
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_MoveToCodeLockSlot, item);  //! Need to use call so we can check if other slot has been taken in the meanwhile (e.g. StoreLoad etc)
	}

	void Expansion_MoveToCodeLockSlot(EntityAI item)
	{
		GameInventory inventory = GetInventory();
		int dstSlot = InventorySlots.GetSlotIdFromString(ATTACHMENT_SLOT_CODE_LOCK);

		if (!inventory.FindAttachment(dstSlot))
		{
			auto srcLoc = new InventoryLocation;

			if (item.GetInventory().GetCurrentInventoryLocation(srcLoc))
			{
				int srcSlot = srcLoc.GetSlot();
				bool srcSlotLocked = inventory.GetSlotLock(srcSlot);

				auto dstLoc = new InventoryLocation;
				dstLoc.SetAttachment(this, item, dstSlot);

				inventory.SetSlotLock(srcSlot, false);
				inventory.SetSlotLock(dstSlot, false);

				GameInventory.LocationSyncMoveEntity(srcLoc, dstLoc);

				inventory.SetSlotLock(dstSlot, srcSlotLocked);
			}
		}
	}

	override bool ExpansionIsOpenable()
	{
		return HasHinges();
	}

	override bool ExpansionIsOpened()
	{
		return IsOpened();
	}

	override bool ExpansionHasCodeLock(string selection)
	{
		if (selection != "*" && selection != "wall_interact" && !GetExpansionSettings().GetBaseBuilding(false).CodelockActionsAnywhere)
			return false;

		return super.ExpansionHasCodeLock(selection);
	}

	override ExpansionCodeLock ExpansionGetCodeLock()
	{
		return ExpansionCodeLock.Cast(Expansion_GetCodeLock());
	}

	override ItemBase Expansion_GetCodeLock()
	{
		ItemBase item;

		if (Class.CastTo(item, FindAttachmentBySlotName(ATTACHMENT_SLOT_CODE_LOCK)) && item.Expansion_IsCodeLock())
			return item;

		if (Class.CastTo(item, FindAttachmentBySlotName(ATTACHMENT_SLOT_COMBINATION_LOCK)) && item.Expansion_IsCodeLock())
			return item;

		return null;
	}

	override ItemBase Expansion_GetCodeLock( string selection )
	{
		//! WARNING: Do NOT check CodelockActionsAnywhere when getting codelock entity!
		//! Reserved for HasCodeLock, else can break actions due to item <-> component mismatch
		if (selection != "*" && selection != "wall_interact")
			return null;

		return super.Expansion_GetCodeLock( selection );
	}

	//! @note vanilla fence overrides modded ItemBase::IsLocked, so super calls Fence::IsLocked
	override bool ExpansionIsLocked()
	{
		return super.IsLocked() || super.ExpansionIsLocked();
	}

	override bool CanOpenFence()
	{
		if (!super.CanOpenFence())
			return false;

		if (HasHinges() && !IsOpened() && !ExpansionIsLocked())
			return true;
		
		return false;
	}

	override bool Expansion_IsPlayerInside(PlayerBase player, string selection)
	{
		return IsPlayerInside(player, selection);
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (ctx.GetVersion() < 38)
		{
			if (!ctx.Read(m_Expansion_Locked))
				return false;

			if (!ctx.Read(m_Expansion_Code))
				return false;

			m_Expansion_CodeLength = m_Expansion_Code.Length();

			bool hasCode;
			if (!ctx.Read(hasCode))
				return false;
		}

		if (!ExpansionCanAttachCodeLock())
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ExpansionCodeLockRemove, 1000, false);
		
		return true;
	}
	#endif
	
	void ExpansionCodeLockRemove()
	{
		if ( !ExpansionCanAttachCodeLock() )
		{
			ItemBase codelock = Expansion_GetCodeLock();
			if (codelock)
				codelock.Delete();
		}
	}
	
	override void OnPartDismantledServer(notnull Man player, string part_name, int action_id)
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart(part_name);

        if ( constrution_part.IsGate() )
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if ( codelock )
				codelock.UnlockServer( player, this );
        }

        super.OnPartDismantledServer(player, part_name, action_id);
    }
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart(part_name);

        if ( constrution_part.IsGate() )
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if ( codelock )
				codelock.UnlockServer( player, this );
        }

		super.OnPartDestroyedServer( player, part_name, action_id );
	}

	override void SetOpenedState( bool state )
	{
		if ( state )
			ExpansionUnlock();

		super.SetOpenedState( state );
	}

	override void CloseAndLock( string selection )
	{
		CloseFence();

		ExpansionLock();
	}

	override array<string> OnDebugSpawnBuildExcludes()
	{
		TStringArray excludes = {"wall_platform"};
		return excludes;
	}
};